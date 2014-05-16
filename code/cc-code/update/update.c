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

MYSQL *conn = NULL;

void parser(char*);
void mylog(const char *);
int openMysqlConn(void);

char *updatemode;
char *updatetype;
char oldversion[128];

int
main(int argc,char *argv[])
{
    int sockconn=0;
    char Rec[128];
    char SEND[1024];
	char sql[2048];
	char *msg;

	unsigned long flag=0;
	unsigned int count=0;
		
	MYSQL_RES *res_ptr;
    MYSQL_ROW sqlrow;
	
    sockconn=atoi(getenv("socket"));
    send(sockconn,"ok",2,0);
    recv(sockconn,Rec,4096,0);

	parser(Rec);

    if(openMysqlConn() != 0)
    {
    	mylog("Connect Database error!");
    	msg="|msg=error|";
		send(sockconn,msg,strlen(msg),0);
		close(sockconn);
    	return -1;
   	}

   	// is new pkg available?
 /* select count(distinct version) from (select * from ctupdate where version > '2.5.2' ) as ct;
	 0：the newest
	 1:patch
	 >1:full
*/
	memset(sql,0,2048);
	snprintf(sql,2048,"SELECT COUNT(DISTINCT version) FROM (SELECT * FROM ctupdate WHERE version > '%s') as tmp",oldversion);
	//mylog(sql);
	if(mysql_real_query(conn,sql,strlen(sql)))
    {
        mylog(mysql_error(conn));
		msg="|msg=error|";
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
			sqlrow = mysql_fetch_row(res_ptr);
			count=atoi(sqlrow[0]);
		}
		else
		{
			mylog(mysql_error(conn));
			msg="|msg=error|";
			send(sockconn,msg,strlen(msg),0);
			close(sockconn);
			mysql_close(conn);
			return -1;
		}
		mysql_free_result(res_ptr);
	}
	switch(count)
	{
		case 0:
			msg="|msg=The Newest|";
			send(sockconn,msg,strlen(msg),0);
			close(sockconn);
			mysql_close(conn);
			return 0;
		case 1:
			updatemode="patch";
			break;
		default:
			updatemode="full";
			break;
	}
	//must update or not?
	memset(sql,0,2048);
	snprintf(sql,2048,"select * from ( select * from ctupdate where version > '%s') as ct where updatetype='Y';",oldversion);
	//mylog(sql);
	if(mysql_real_query(conn,sql,strlen(sql)))
    {
        mylog(mysql_error(conn));
		msg="|msg=error|";
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
				updatetype="Y";
			}
			else
			{
				updatetype="N";
			}
			
		}
		else
		{
			mylog(mysql_error(conn));
			msg="|msg=error|";
			send(sockconn,msg,strlen(msg),0);
			close(sockconn);
			mysql_close(conn);
			return -1;
		}
		mysql_free_result(res_ptr);
	}
	//is there has patch pkg?
	if(strcmp(updatemode,"patch") == 0)
	{
		memset(sql,0,2048);
		snprintf(sql,2048,"SELECT COUNT(*) FROM ctupdate WHERE updatemode='patch' AND flag='1'");
		//mylog(sql);
		if(mysql_real_query(conn,sql,strlen(sql)))
	    {
	        mylog(mysql_error(conn));
			msg="|msg=error|";
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
				sqlrow = mysql_fetch_row(res_ptr);
				count=atoi(sqlrow[0]);		
			}
			else
			{
				mylog(mysql_error(conn));
				msg="|msg=error|";
				send(sockconn,msg,strlen(msg),0);
				close(sockconn);
				mysql_close(conn);
				return -1;
			}
			mysql_free_result(res_ptr);
		}
		if(count==0)//no patch pkg
		{
			memset(sql,0,2048);
			snprintf(sql,2048,"SELECT * FROM ctupdate WHERE updatemode='full' AND flag='1'");
			//mylog(sql);
			if(mysql_real_query(conn,sql,strlen(sql)))
		    {
		        mylog(mysql_error(conn));
				msg="|msg=error|";
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
					sqlrow = mysql_fetch_row(res_ptr);
					snprintf(SEND,1024,"|%s|%s|%s|",sqlrow[1],sqlrow[2],updatetype);	
					//mylog(SEND);		
				}
				else
				{
					mylog(mysql_error(conn));
					msg="|msg=error|";
					send(sockconn,msg,strlen(msg),0);
					close(sockconn);
					mysql_close(conn);
					return -1;
				}
				mysql_free_result(res_ptr);
			}
			send(sockconn,SEND,strlen(SEND),0);
			close(sockconn);
			mysql_close(conn);
			return 0;

		}
		else
		{
			memset(sql,0,2048);
			snprintf(sql,2048,"SELECT * FROM ctupdate WHERE updatemode='patch' AND flag='1'");
			//mylog(sql);
			if(mysql_real_query(conn,sql,strlen(sql)))
		    {
		        mylog(mysql_error(conn));
				msg="|msg=error|";
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
					sqlrow = mysql_fetch_row(res_ptr);
					snprintf(SEND,1024,"|%s|%s|%s|",sqlrow[1],sqlrow[2],updatetype);			
				}
				else
				{
					mylog(mysql_error(conn));
					msg="|msg=error|";
					send(sockconn,msg,strlen(msg),0);
					close(sockconn);
					mysql_close(conn);
					return -1;
				}
				mysql_free_result(res_ptr);
			}
			send(sockconn,SEND,strlen(SEND),0);
			close(sockconn);
			mysql_close(conn);
			return 0;
		}
	}
	else
	{
		memset(sql,0,2048);
		snprintf(sql,2048,"SELECT * FROM ctupdate WHERE updatemode='full' AND flag='1'");
		//mylog(sql);
			if(mysql_real_query(conn,sql,strlen(sql)))
		    {
		        mylog(mysql_error(conn));
				msg="|msg=error|";
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
					sqlrow = mysql_fetch_row(res_ptr);
					snprintf(SEND,1024,"|%s|%s|%s|",sqlrow[1],sqlrow[2],updatetype);			
				}
				else
				{
					mylog(mysql_error(conn));
					msg="|msg=error|";
					send(sockconn,msg,strlen(msg),0);
					close(sockconn);
					mysql_close(conn);
					return -1;
				}
				mysql_free_result(res_ptr);
			}
		send(sockconn,SEND,strlen(SEND),0);
		close(sockconn);
		mysql_close(conn);
		return 0;
	}
	close(sockconn);	
    mysql_close(conn);
    return 0;

}
/*
CT-->CC:if=update
CC-->CT:ok
CA-->CC:|version|
CC-->CT:|The Newest| or |version|updatemode|updatetype|
*/
void 
parser(char* Rec)
{	
	char *p;
	p=rindex(Rec,'|');
	snprintf(oldversion,p-Rec,"%s",Rec+1);
	//oldversion[strlen(oldversion)-1]='\0';
	//mylog(Rec);
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
	fp=fopen("/tmp/updatelog","a+");
	time(&now);
	timenow=localtime(&now);
	fprintf(fp,"%s %s\n\n",asctime(timenow),p);
	fclose(fp);
}