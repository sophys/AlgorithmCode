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

char version[64];
char updatemode[8];
char flag[8];
char updatetype[8];
char pkgname[128];

int
main(int argc,char *argv[])
{
    int sockconn=0;
    char Rec[4096];
	char sql[2048];
	char *msg;

	unsigned long flag=0;

	MYSQL_RES *res_ptr;
    MYSQL_ROW sqlrow;

    sockconn=atoi(getenv("socket"));
    send(sockconn,"ok",2,0);
    recv(sockconn,Rec,4096,0);

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
	snprintf(sql,2048,"SELECT ctid FROM ctupdate WHERE version = '%s' AND updatemode = '%s'",version,updatemode);
	if(mysql_real_query(conn,sql,strlen(sql)))
    {
        mylog(mysql_error(conn));
		msg="2";
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
			if(flag)
			{
				msg="1";
				send(sockconn,msg,strlen(msg),0);
				close(sockconn);
				mysql_close(conn);
				return 0;
			}
			
		}
		else
		{
			mylog(mysql_error(conn));
			msg="2";
			send(sockconn,msg,strlen(msg),0);
			close(sockconn);
			mysql_close(conn);
			return -1;
		}
		mysql_free_result(res_ptr);
	}
	//DB doesn't have the pkg,so insert it.
	// clear the newest pkg flag;
	memset(sql,0,2048);
	snprintf(sql,2048,"UPDATE ctupdate SET flag='0' WHERE flag='1' AND version<>'%s'",version);
	mysql_real_query(conn,sql,strlen(sql));
			
	memset(sql,0,2048);
	snprintf(sql,2048,"INSERT INTO ctupdate(version,updatemode,flag,updatetype,pkgname) VALUES ('%s','%s','1','%s','%s')",version,updatemode,updatetype,pkgname);
	if(mysql_real_query(conn,sql,strlen(sql)))
    {
        mylog(mysql_error(conn));
        msg="2";
		send(sockconn,msg,strlen(msg),0);
		close(sockconn);
		mysql_close(conn);
        return -1;
    }
    else
    {
    	msg="0";
		send(sockconn,msg,strlen(msg),0);
    }
	close(sockconn);	
    mysql_close(conn);
    return 0;

}
/*
CA-->CC:if=putcttodb
CC-->CA:ok
CA-->CC:|version|updatemode|flag|updatetype|pkgname|
CC-->CA:ok
CA-->CC:pkg_info
CC-->CA:|ok|or|no|
CA-->CC:package
CC-->CA:ok
*/
void
parser(char* Rec)
{
	char* q;
	char* delim="|";
	mylog(Rec);

	if((q=strtok(Rec,delim)))
	{
		strcpy(version,q);
		version[strlen(version)]='\0';
	}
	if((q=strtok(NULL,delim)))
	{
		strcpy(updatemode,q);
		updatemode[strlen(updatemode)]='\0';
	}
	if((q=strtok(NULL,delim)))
	{
		strcpy(flag,q);
		flag[strlen(flag)]='\0';
	}
	if((q=strtok(NULL,delim)))
	{
		strcpy(updatetype,q);
		updatetype[strlen(updatetype)]='\0';
	}
	if((q=strtok(NULL,delim)))
	{
		strcpy(pkgname,q);
		pkgname[strlen(pkgname)]='\0';
	}
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
	fp=fopen("/tmp/putcttodblog","a+");
	time(&now);
	timenow=localtime(&now);
	fprintf(fp,"%s %s\n\n",asctime(timenow),p);
	fclose(fp);
}
