#include "server.h"

//数据库回调函数
static int history_callback(void* arg, int f_num, char** f_value, char** f_name);
//查询单词函数
static int do_search_word(int connfd, msg_t* msg, char* word);

//获取系统时间
static int get_date(char* date)
{
	time_t t;
	time(&t);
	struct tm* local = localtime(&t);
	sprintf(date,"%d-%d-%d %d:%d:%d",local->tm_year+1900,
			local->tm_mon + 1, local->tm_mday,local->tm_hour,
			local->tm_min,local->tm_sec);
	return 0;
}

//查询:成功返回0，失败返回-1
static int do_query(int connfd, msg_t* msg, sqlite3* db)
{
	char word[64];
	int found = 0;
	char date[128];
	char sql[128];
	char* errmsg;
	//取出msg结构体中要查询的单词
	strcpy(word,msg->data);
	//printf("查询单词:%s\n",word);
	found = do_search_word(connfd,msg,word);
	//表示找到了单词，那么此时应该将用户名、
	//时间、单词插入到history历史记录表中
	if(found == 0){
		//获取系统时间
		get_date(date);
		sprintf(sql,"INSERT INTO RECORD VALUES('%s','%s','%s');",
				msg->name,date,word);
		//printf("sql = %s\n",sql);
		if(sqlite3_exec(db,sql,NULL,NULL,&errmsg) != SQLITE_OK){
			printf("插入失败:%s\n",errmsg);
			return -1;
		}
	}
	//表示没有找到单词
	else{
		strcpy(msg->data,"not found");
	}

	//将查询结果回送给客户端
	if(send(connfd,msg,sizeof(msg_t),0) < 0){
		perror("send to client failed");
		return -1;
	}
	return 0;
}
//处理客户端请求
int do_client(int connfd, sqlite3* db)
{
	printf("开始处理客户端请求:\n");
	msg_t msg;
	//recv(connfd,&msg,sizeof(msg_t),0);
	while(recv(connfd,&msg,sizeof(msg_t),0) > 0){
	//while(1){
		//ssize_t rsize = recv(connfd,&msg,sizeof(msg_t),0);
		//printf("收到客户端请求:");
		//printf("%d %s %s \n",msg.type,msg.name,msg.data);
		//printf("type: %d\n",msg.type);
		switch(msg.type){
			case R:
				do_register(connfd, &msg, db);
				break;
			case L:
				do_login(connfd, &msg, db);
				break;
			case Q:
				do_query(connfd,&msg,db);
				break;
			case H:
				do_history(connfd,&msg,db);
				break;
			default:
				printf("Invaild data msg\n");
				break;
		}
	}
	//printf("client exit...\n");
	close(connfd);
	exit(0);
}

//注册函数
void do_register(int connfd, msg_t* msg, sqlite3* db)
{
	char *errmsg;
	char sql[128];
	sprintf(sql,"insert into usr values('%s','%s');",msg->name,msg->data);
	//printf("%s\n",sql);
	if(sqlite3_exec(db,sql,NULL,NULL,&errmsg) != SQLITE_OK){
		printf("%s\n",errmsg);
		strcpy(msg->data,"usr name already exist.");
	}
	else{
		printf("client register ok!\n");
		strcpy(msg->data,"ok!");
	}
	if(send(connfd,msg,sizeof(msg_t),0)<0){
		perror("send failed");
		return ;
	}
	return;
}

//登录函数:成功返回0，失败返回-1
int do_login(int connfd, msg_t* msg, sqlite3* db)
{
	char sql[128] = {};
	char *errmsg;
	int nrow;
	int ncloumn;
	char **resultp;
	sprintf(sql,"SELECT *FROM USR WHERE NAME = '%s'	AND PASS = '%s';",msg->name,msg->data);
	//printf("%s\n",sql);
	if(sqlite3_get_table(db,sql,&resultp,&nrow,&ncloumn,&errmsg) != SQLITE_OK){
		printf("查询失败:%s",errmsg);
		return -1;
	}
	else{
		//printf("get table ok!\n");//只是表示sql语句执行成功
	}
	//查询成功，数据库中拥有此用户
	if(nrow == 1){
		//printf("登录成功...\n");
		strcpy(msg->data, "OK");
		send(connfd, msg, sizeof(msg_t),0);//回送客户端信息
		return 0;
	}
	if(nrow == 0){//用户名或者密码错误
		printf("用户名或者密码错误.....\n");
		strcpy(msg->data,"usr name or password wrong!");
		send(connfd, msg, sizeof(msg_t),0);//回送客户端信息
		return -1;
	}
}

//查询单词函数
static int do_search_word(int connfd, msg_t* msg, char* word)
{
	//printf("打开字典文件\n");
	//打开文件，读取文件，进行比对
	FILE* fp = fopen("dict.txt","r");
	if(!fp){
		perror("fopen dict failed");
		strcpy(msg->data,"failed to open dict.txt");
		send(connfd, msg, sizeof(msg_t), 0);
		return -1;
	}
	//打印出要查询的单词
	int len = strlen(word);
	//printf("%s, len = %d\n", word, len);

	//读文件，查询单词
	char temp[512];
	int res;
	while(fgets(temp,512,fp) != NULL){
		//printf("%s",temp);
		res = strncmp(temp, word, len);
		if(res < 0){
			continue;
		}
		if(res > 0 || temp[len] != ' '){
			break;
		}
		//以上两种都是没有找到，下面就是找到了
		char* p = temp + len;
		//跳过所有的空格
		while(*p == ' '){
			p++;
		}
		//将结果填充到msg中
		strcpy(msg->data, p);
		//printf("%s\n",p);
		fclose(fp);
		return 0;
	}
	//循环正常结束，说明没有找到
	fclose(fp);
	fp = NULL;
	return -1;
}

//查询数据库的回调函数,得到查询结果并且需要将历史记录发送给客户端
//查询结果保存到了f_value字符串列表中
static int history_callback(void* arg, int f_num, char** f_value, char** f_name)
{
	//数据库表结构：record：name:date:word
	int connfd = *(int*)arg;
	msg_t msg;
	sprintf(msg.data,"%s, %s",f_value[1],f_value[2]);
	if(send(connfd, &msg, sizeof(msg_t),0) < 0){
		perror("history_callback:send failed");
		return -1;
	}
	return 0;
}
//历史函数:成功返回0，失败返回-1
int do_history(int connfd, msg_t* msg, sqlite3* db)
{
	printf("history.......\n");
	char sql[128];
	char *errmsg;
	sprintf(sql,"SELECT * FROM RECORD WHERE NAME = '%s';", msg->name);
	printf("sql:%s\n",sql);

	//查询数据库
	if(sqlite3_exec(db, sql, history_callback,(void*)&connfd,
				&errmsg) != SQLITE_OK){
		printf("%s\n",errmsg);
		return -1;
	}
	printf("query record done.\n");
	//所有的记录查询发送完毕之后，给客户端发出一个结束信息
	msg->data[0] = '\0';
	if(send(connfd, msg, sizeof(msg_t),0) < 0){
		perror("do_history: send failed");
		return -1;
	}
	return 0;
}
