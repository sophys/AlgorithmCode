#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <fcntl.h>
#include <arpa/inet.h>
#include <errno.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include <mysql/mysql.h>
#include <string.h>
#include <dlfcn.h>
using namespace std;
int GetDbInfo(char *,char *,char *,char *);//得到数据库的信息 0成功 －1 失败
int AppHandler(char *appid,char *userid,int sockfd,char *result);//
int OpenSO(char *app,char *appid,char *userid,MYSQL* M);
typedef int (*APPFUNC)();
