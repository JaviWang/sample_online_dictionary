#include "t_net.h"

int sock_bind(int domain, int type, int port)
{
	int sockfd;
	struct sockaddr_in serverip;
	struct sockaddr_in clientip;
	socklen_t clientip_len;
	
	printf("创建套接字sockfd:\n");
	if((sockfd = socket(domain,type,0)) == -1){
		perror("socket failed");
		return -1;
	}
	//防止出现地址重用错误
	int on = 1;
	int opt = setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,(const char*)&on,sizeof(on));
	if(opt < 0){
		perror("setsockopt failed");
		return -1;
	}
	//初始化服务器的地址结构
	bzero(&serverip,sizeof(serverip));
	serverip.sin_family = domain;
	serverip.sin_port = htons(port);
	//点分十进制字符串ip --> 二进制形式ip
	//inet_pton(AF_INET,argv[1],&serverip.sin_addr);
	serverip.sin_addr.s_addr = htonl(INADDR_ANY);
	//绑定
	printf("绑定sockfd和服务器IP地址:\n");
	if(bind(sockfd,(struct sockaddr*)&serverip,sizeof(serverip)) == -1){
		perror("bind failed");
		return -1;
	}
	return sockfd;
}
