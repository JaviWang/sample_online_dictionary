# 使用方法


## 客户端

```shell
cd ./client
make
./client 127.0.0.1 8000 #./client <服务器IP地址> <服务器端口号>
```

## 服务器

``` shell
cd ./server
make
./server 8000	#./server <端口号>
```

本程序使用的字典文件是在网上下载的.txt文本文件

支持历史记录功能，将查询历史保存到了my.db数据库文件中