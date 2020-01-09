#include "server.h"
#include "t_net.h"

//太平间信号处理
static void sigchld(int signum)
{
	for(;;){
		pid_t pid = waitpid(-1, NULL, WNOHANG);
		if(pid == -1){
			if(errno != ECHILD){
				perror("waitpid");
				exit(-1);
			}
			//printf("回收完了\n");
			break;
		}
		if(pid == 0){
			//printf("暂时没有子进程退出\n");
			break;
		}
		else{
			//printf("有子进程退出\n");
		}
	}
}
int main(int argc, char *argv[])
{
	if(argc != 2){
		printf("Usage:%s serverport.\n",argv[0]);
		return -1;
	}
	int sockfd;
	int choice;	//选择一级菜单
	msg_t msg;
	struct sockaddr_in serverip;
	struct sockaddr_in clientip;
	socklen_t clientip_len;
	
	//打开数据库
	sqlite3 *db;
	if(sqlite3_open(DATABASE, &db) != SQLITE_OK){
		printf("%s\n",sqlite3_errmsg(db));
		return -1;
	}
	else{
		printf("open database success.\n");
	}
	sockfd = sock_bind(AF_INET,SOCK_STREAM,atoi(argv[1]));
	printf("开始监听");
	if(listen(sockfd,10) == -1){
		perror("listen failed");
		return -1;
	}
	/*处理僵尸进程：父进程忽略SIGCHLD信号，则由内核负责回收
	signal(SIGCHLD,SIG_IGN);
	*/
	if(signal(SIGCHLD,sigchld)==SIG_ERR)
	{
		perror("signal");
		return -1;
	}
	printf("等待客户端连接.......\n");
	while(1){
		int connfd = accept(sockfd,(struct sockaddr*)&clientip,&clientip_len);
		if(connfd == -1){
			perror("accept failed");
			return -1;
		}
		pid_t pid = fork();
		if(pid == -1){
			perror("fork failed");
			return -1;
		}
		//子进程和客户端通信，处理具体的消息
		if(pid == 0){
			printf("开始和客户端通信...\n");
			close(sockfd);
			do_client(connfd,db);
		}
		//父进程，用来接收客户端的连接请求
		else{
			close(connfd);
		}
	}
	return 0;
}
