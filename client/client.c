#include "client.h"

//注册函数
int do_register(int sockfd, msg_t* msg)
{
	//printf("register.......\n");
	msg->type = R;
	printf("please input user name: ");
	scanf("%s",msg->name);
	getchar();
	printf("pelase input password: ");
	scanf("%s",msg->data);
	getchar();
	printf("user:%s\tpassword:%s\n",msg->name, msg->data);
	if(send(sockfd, msg, sizeof(msg_t),0) < 0){
		perror("send failed");
		return -1;
	}
	if(recv(sockfd,msg,sizeof(msg_t),0) < 0){
		perror("recv failed");
		return -1;
	}
	//现在的msg填充的是服务器的响应，ok! or user alread exist
	printf("%s\n",msg->data);
	return 0;
}

//登录函数:成功返回0，失败返回-1
int do_login(int sockfd, msg_t* msg)
{
	//printf("login.......\n");
	printf("* * * * * * * * * * * * * * * * * * *\n");
	msg->type = L;

	printf("please input user name: ");
	scanf("%s",msg->name);
	getchar();
	printf("pelase input password: ");
	scanf("%s",msg->data);
	getchar();
	printf("user:%s\tpassword:%s\n",msg->name, msg->data);
	if(send(sockfd, msg, sizeof(msg_t),0) < 0){
		perror("send failed");
		return -1;
	}
	if(recv(sockfd,msg,sizeof(msg_t),0) < 0){
		perror("recv failed");
		return -1;
	}
	if(strncmp(msg->data,"OK",3) == 0){
		system("clear");
		printf("-----登录成功！-----\n");
		return 0;
	}
	else{
		printf("登录失败:%s\n",msg->data);
		return -1;
	}
}

//查询:成功返回0，失败返回-1
int do_query(int sockfd, msg_t* msg)
{
	msg->type = Q;
	printf("----------------------------\n");
	while(1){
		printf("input word(enter # quit):");
		//客户端：输入#，返回上一级菜单
		scanf("%s",msg->data);
		if(strncmp(msg->data,"#",1) == 0){
			break;
		}
		//将要查询的单词发送给服务器
		if(send(sockfd,msg,sizeof(msg_t),0) < 0){
			perror("send to server failed");
			return -1;
		}
		//等待接收服务器回送的单词释义
		if(recv(sockfd,msg,sizeof(msg_t),0) < 0){
			perror("receive server msg failed");
			return -1;
		}
		printf("%s\n",msg->data);
	}
	return 0;
}

//历史函数:成功返回0，失败返回-1
int do_history(int sockfd, msg_t* msg)
{
	printf("开始查询历史记录:\n");
	msg->type = H;
	puts("+++++++++++++  history  +++++++++++++++++++");
	if(send(sockfd, msg, sizeof(msg_t),0) < 0){
		perror("do_history:send to server failed");
		return -1;
	}
	//接收服务器传递回来的历史记录信息
	while(1){
		if(recv(sockfd, msg, (sizeof(msg_t)),0) < 0){
			perror("do_history:recv from server failed");
			return -1;
		}
		if(msg->data[0] == '\0'){
			break;
		}
		//输出历史记录信息
		printf("%s\n",msg->data);
	}
	return 0;
}
