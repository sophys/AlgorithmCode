#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <errno.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <signal.h>
#include <mysql/mysql.h>
#include <string.h>

#define COUNT 100

int Translate(char *,char *,char *);
void mylog(const char *p);
int openMysqlConn(void);
int getvminfo(MYSQL *conn ,char *userid,char *appid,char *sendbuf,int sockconn);
int getavm(MYSQL *con,char* vmuser,char* vmpasswd,char* vmip,char *appid,char *userid);
int testthevm(char *vmip,char* vmuser,char* vmpasswd);
int insertthevm(MYSQL *conn,char* userid,int vmid,char *vmuser,char* vmpasswd);