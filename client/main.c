#include "client.h"

int main(int argc, char *argv[])
{
	if(argc != 3){
		printf("Usage:%s serverip port.\n",argv[0]);
		return -1;
	}
	int sockfd;
	msg_t msg;
	if((sockfd = connect_server(PF_INET,SOCK_STREAM,argv[1],argv[2])) == -1){
		printf("connect server failed");
		return -1;
	}
	select_menu(sockfd,&msg);
	return 0;
}
