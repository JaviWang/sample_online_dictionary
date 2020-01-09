#ifndef __SERVER_H__
#define __SERVER_H__

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>

#define SIZE 32

//定义消息类型
#define R	1	//user - register
#define L	2	//user - longin
#define Q	3	//user - query
#define H	4	//history

#define DATABASE	"my.db"
//定义通信双方的消息结构
typedef struct{
	int type;//消息类型，登录/注册/退出...
	char name[SIZE];
	char data[256];
}msg_t;

//处理客户端请求
extern int do_client(int connfd, sqlite3* db);

//注册函数
extern void do_register(int fd, msg_t* msg, sqlite3* db);

//登录函数:成功返回0，失败返回-1
extern int do_login(int fd, msg_t* msg,  sqlite3* db);

//历史函数:成功返回0，失败返回-1
extern int do_history(int fd, msg_t* msg, sqlite3* db);

#endif //__SERVER_H__

