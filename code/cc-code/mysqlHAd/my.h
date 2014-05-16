#ifndef	__my_h
#define	__my_h

#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <time.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in_systm.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#define LENGTH_OF_LISTEN_QUEUE 20
#define LISTEN_PORT 55555
#define COUNT 4 //ping count 

extern int FLAG;//0:on 1:off
extern char INFO[3][32];

#define CONFPATH "/var/mysqlhad.conf"

#define PATH "/var/.new.flag"  //标志文件的存放路径
#define SET   0  	//创建标志文件
#define RESET 1		//删除标志文件
#define CHECK 2		//查询标志文件是否存在

#define MGM 0
#define NDB 1
#define SQL 2
#define ALL 3

#define MP "setsid /usr/local/mysql/bin/ndb_mgmd -f /var/lib/mysql-cluster/config.ini >/dev/null 2>&1 &"
#define NP "setsid /usr/local/mysql/bin/ndbd >/dev/null 2>&1 &"
#define SP "setsid /usr/local/mysql/bin/mysqld_safe --user=root& >/dev/null 2>&1 &"
#define DOWN "setsid /usr/local/mysql/bin/ndb_mgm -e shutdown"

void mylog(const char *);
int start(int);
int down(void);
int checknet(char*,char*,int);
int checkConnect(char *ip,int count);
int arbitration(int);
int chechstatus(char*);
int makeflag(int);
void clearflag(char *);
int detect_process(char * );
void checknode(void);
void sig_chld(int);
int readconf(char *);
#endif