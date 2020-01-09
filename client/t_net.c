#include "client.h"

int connect_server(int domain, int type, char* ip, char* port)
{
	int sockfd;
	msg_t msg;
	struct sockaddr_in serverip;
	if((sockfd = socket(domain,type,0)) == -1){
		perror("socket failed");
		return -1;
	}
	//初始化服务器的地址结构
	bzero(&serverip,sizeof(serverip));
	serverip.sin_family = domain;
	serverip.sin_port = htons(atoi(port));
	
	//点分十进制字符串ip --> 二进制形式ip
	//inet_pton(AF_INET,argv[1],&serverip.sin_addr);
	serverip.sin_addr.s_addr = inet_addr(ip);
	//连接服务器
	if(connect(sockfd,(struct sockaddr*)&serverip,sizeof(serverip)) == -1){
		perror("connect server failed");
		return -1;
	}
	return sockfd;
}
