#include <sys/socket.h>
#include <mysql.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>

MYSQL *conn = NULL;

void parser(char*);
void mylog(const char *);
int openMysqlConn(void);

char postfix[64];
char appid[8];

int
main(int argc,char *argv[])
{
    int sockconn=0;
    char Rec[512];
    char Send[128];
	char sql[2048];
	char *msg;

	unsigned long flag=0;

	MYSQL_RES *res_ptr;
    MYSQL_ROW sqlrow;

    sockconn=atoi(getenv("socket"));
    send(sockconn,"ok",2,0);
    recv(sockconn,Rec,512,0);

	parser(Rec);

    if(openMysqlConn() != 0)
    {
    	mylog("Connect Database error!");
    	msg="2";
		send(sockconn,msg,strlen(msg),0);
		close(sockconn);
		mysql_close(conn);
    	return -1;
   	}
	//is db has the pkg ?
	memset(sql,0,2048);
	snprintf(sql,2048,"SELECT appid FROM cloudregister WHERE postfix = '%s'",postfix);
	if(mysql_real_query(conn,sql,strlen(sql)))
    {
        mylog(mysql_error(conn));
		msg="error";
		send(sockconn,msg,strlen(msg),0);
		close(sockconn);
		mysql_close(conn);
        return -1;
    } 
	else
	{
		res_ptr = mysql_store_result(conn);
		if(res_ptr)
		{
			flag = (unsigned long)mysql_num_rows(res_ptr);
			if(!flag)
			{
				msg="|msg=query phip by vmip error |";
				send(sockconn,msg,strlen(msg),0);
				close(sockconn);
				mysql_close(conn);
				return -1;
			}
			else
			{
				sqlrow = mysql_fetch_row(res_ptr);
				strncpy(appid,sqlrow[0],8);
			}
			
		}
		else
		{
			mylog(mysql_error(conn));
			msg="error";
			send(sockconn,msg,strlen(msg),0);
			close(sockconn);
			mysql_close(conn);
			return -1;
		}
		mysql_free_result(res_ptr);
	}
	snprintf(Send,128,"|appid=%s|",appid);
	send(sockconn,Send,strlen(Send),0);

	close(sockconn);	
    mysql_close(conn);
    return 0;

}
/*
 ct > cc: if=getappidbypostfix
cc > ct: ok
ct > cc: |postfix=***|
cc > ct: |appid=***|
*/
void
parser(char* Rec)
{
	char* q=rindex(Rec,'|');
	char* p=rindex(Rec,'=');
	strncpy(postfix,p+1,q-p-1);
}
int openMysqlConn()
{
    conn = mysql_init(NULL);

    if(mysql_real_connect(conn,getenv("DBIP"),getenv("DBUSER"),getenv("DBPASS"),getenv("DBNAME"),0,NULL,0) == NULL)
    {
        return -1;
    }
    return 0;
}
void mylog(const char *p)
{
	struct tm *timenow;
	time_t now;
	FILE *fp;
	fp=fopen("/tmp/getappidbypostfix","a+");
	time(&now);
	timenow=localtime(&now);
	fprintf(fp,"%s %s\n\n",asctime(timenow),p);
	fclose(fp);
}
