# 简单的在线小词典

本程序是一个简单在线小词典程序，基于TCP/IP网络编程实现，并发通过多进程实现。注册并登录以后可以查询单词释义，词典文件是下载的.txt字典文件，词汇量不够全面。本程序支持保存查询历史功能，查询历史保存到了服务器目录下的my.db数据库(sqlite3)文件中.

**首先**启动服务器，**然后**启动客户端连接服务器，之后根据菜单提示进行操作即可。

## 客户端和服务器的编译和运行方法

### 客户端

```shell
cd ./client
make
./client 127.0.0.1 8000 #./client <服务器IP地址> <服务器端口号>
```

### 服务器

编译之前需要先安装sqlite3数据库

``` shell
sudo apt-get install sqlite3
```

``` shell
cd ./server
make
./server 8000	#./server <端口号>
```

