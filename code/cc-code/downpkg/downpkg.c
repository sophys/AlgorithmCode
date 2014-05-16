#include "myh.h"

#define PATH "/var/cloudos/ct/"

MYSQL *conn = NULL;
char version[128];
char updatemode[8];
char filename[256];

void parser(char*);
void mylog(const char *);
int openMysqlConn(void);

/*
	CT-->CC:if=dowmpkg
	CC-->CT:ok
	CT-->CC:|version|updatemode|
	CC-->CT:|filename|size| or |request package error|
	CT-->CC:|ok|or|no|
	CC-->CT:|package|
	CT-->CC:|ok|or|no|
	CC-->CT:|package|
*/

int
main(int argc,char *argv[])
{
    int sockconn=0;
    char Rec[4096];
	char sql[2048];
	char *msg;
	char path[1024];
	char SEND[1024];
	struct stat statbuf;

	unsigned long flag=0;
		
	MYSQL_RES *res_ptr;
    MYSQL_ROW sqlrow;
    signal(SIGPIPE,SIG_IGN);
	
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
	//get filename
	memset(sql,0,2048);
	snprintf(sql,2048,"SELECT pkgname FROM ctupdate WHERE version = '%s' AND updatemode = '%s'",version,updatemode);
	if(mysql_real_query(conn,sql,strlen(sql)))
    {
        mylog(mysql_error(conn));
		msg="|msg=cs database error|";
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
				msg="|msg=error |";
				send(sockconn,msg,strlen(msg),0);
				close(sockconn);
				mysql_close(conn);
				return -1;
			}
			else
			{
				sqlrow = mysql_fetch_row(res_ptr);
				strncpy(filename,sqlrow[0],256);
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
	//close the connect to DB.
	mysql_close(conn);

	snprintf(path,1024,"%s%s",PATH,filename);
	//mylog(path);

	if(lstat(path,&statbuf) < 0)
	{	
		mylog(strerror(errno));
		mylog(path);
		msg="|request package error|";
		send(sockconn,msg,strlen(msg),0);
		close(sockconn);
		return -1;
	}
	else
	{
		filesize = statbuf.st_size;
	}
	snprintf(SEND,1024,"|%s|%d|",filename,filesize);
	//mylog(SEND);
	send(sockconn,SEND,strlen(SEND),0);
	recv(sockconn,Rec,4096,0);
	if(strstr(Rec,"ok"))
	{
		if(sendfile(sockconn,path) !=0 )
		{
			mylog("send file faild");
			close(sockconn);
			return -1;
		}
	}
	close(sockconn);	
    return 0;
}

void 
parser(char* Rec)
{
	char* r;
	char* q;

	char* delim="|";
//	mylog(Rec);
	
	r=strtok(Rec,delim); 
	strcpy(version,r);
	version[strlen(version)]='\0';

	if((q=strtok(NULL,delim)))
	{
		strcpy(updatemode,q);
		updatemode[strlen(updatemode)]='\0';
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
	fp=fopen("/tmp/downpkglog","a+");
	time(&now);
	timenow=localtime(&now);
	fprintf(fp,"%s %s\n\n",asctime(timenow),p);
	fclose(fp);
}