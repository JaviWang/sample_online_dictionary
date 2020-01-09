#ifndef __CLIENT_H__
#define __CLIENT_H__

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

#define SIZE 32

//定义消息类型
#define R	1	//user - register
#define L	2	//user - longin
#define Q	3	//user - query
#define H	4	//history

//定义通信双方的消息结构
typedef struct{
	int type;//消息类型，登录/注册/退出...
	char name[SIZE];
	char data[256];
}msg_t;

//选择菜单函数
extern void select_menu(int sockfd, msg_t* msg);
//注册函数
extern int do_register(int sockfd, msg_t* msg);

//登录函数:成功返回0，失败返回-1
extern int do_login(int sockfd, msg_t* msg);

//查询:成功返回0，失败返回-1
extern int do_query(int sockfd, msg_t* msg);

//历史函数:成功返回0，失败返回-1
extern int do_history(int sockfd, msg_t* msg);

//连接服务器函数
int connect_server(int domain, int type, char* ip, char* port);

#endif //__CLIENT_H__
