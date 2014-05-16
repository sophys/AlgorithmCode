#include <sys/socket.h>
#include <mysql/mysql.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

MYSQL *conn = NULL;

void parser(char*);
void mylog(const char *);
int openMysqlConn(void);

char phip[32];
char phid[32];
char cpu[3][32];
char mem[3][32];
char memf[3][32];
char dsk[3][32];
char dskf[3][32];
char dskr[3][32];
char dskw[3][32];
char neti[3][32];
char neto[3][32];
char mytime[32];
char mac[128];

int
main(int argc,char *argv[])
{
    int sockconn=0;
    char Rec[4096];
	char sql[2048];
	char *msg;

	int flag=0;
	
	MYSQL_RES *res_ptr;
    MYSQL_ROW sqlrow;
	
    sockconn=atoi(getenv("socket"));
    send(sockconn,"ok",2,0);
    recv(sockconn,Rec,4096,0);

	parser(Rec);

    if(openMysqlConn() != 0)
    {
    	mylog("Connect Database error!");
    	return -1;
	}
	//得到phid,没有的话，要插入新的记录
	memset(sql,0,2048);
	snprintf(sql,2048,"SELECT phid FROM ph_list WHERE  phmac = '%s'",mac);
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
			if(!flag)//空，插入ph节点，并得到phid
			{

				memset(sql,0,2048);
				snprintf(sql,2048,"INSERT INTO  ph_list(phip,phmac,state) VALUES('%s','%s','on')",phip,mac);
				//mylog(sql);
				mysql_real_query(conn,sql,strlen(sql));

				memset(sql,0,2048);
				snprintf(sql,2048,"SELECT LAST_INSERT_ID()");
				
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
					MYSQL_RES *res;
					res = mysql_store_result(conn);
					if(res)
					{
						sqlrow = mysql_fetch_row(res);
						strncpy(phid,sqlrow[0],32);						
					}
					mysql_free_result(res);
				}
			}
			else//非空，得到phid, and check phip !!!
			{
				sqlrow = mysql_fetch_row(res_ptr);
				strncpy(phid,sqlrow[0],32);
				memset(sql,0,2048);
				snprintf(sql,2048,"UPDATE ph_list SET phip='%s' WHERE phid='%s'",phip,phid);
				mysql_real_query(conn,sql,strlen(sql));
			}			
		}
		mysql_free_result(res_ptr);
	}
	
	//查询 phid ，检查是 更新还是插入
	memset(sql,0,2048);
	snprintf(sql,2048,"select * from ph_history where phid='%s'",phid);
	if(mysql_real_query(conn,sql,strlen(sql)))
    {
        mylog(mysql_error(conn));
        return -1;
    } 
	else
	{
		res_ptr = mysql_store_result(conn);
		if(res_ptr)
		{
			flag = (unsigned long)mysql_num_rows(res_ptr);
			if(!flag)//不存在要插入
			{
				memset(sql,0,2048);
				snprintf(sql,2048,"INSERT INTO ph_history(phid,cpuinfos,meminfos,diskinfos,netinfos,time) VALUES ('%s','%s','%s*%s','%s*%s*%s','%s*%s','%s')",phid,cpu[2],mem[2],memf[2],dskf[2],dskr[2],dskw[2],neti[2],neto[2],mytime);
				//mylog(sql);
				if(mysql_real_query(conn,sql,strlen(sql)))
			    {
			        mylog(mysql_error(conn));
			        return -1;
			    } 
			}
			else//存在，更新
			{
				memset(sql,0,2048);
				snprintf(sql,2048,"UPDATE ph_history SET cpuinfos='%s',meminfos='%s*%s',diskinfos='%s*%s*%s',netinfos='%s*%s',time='%s' WHERE phid='%s'",cpu[2],mem[2],memf[2],dskf[2],dskr[2],dskw[2],neti[2],neto[2],mytime,phid);
				if(mysql_real_query(conn,sql,strlen(sql)))
			    {
			        mylog(mysql_error(conn));
			        return -1;
			    } 
			}
		}
		else
		{
			mylog(mysql_error(conn));
		}
		mysql_free_result(res_ptr);
	}
	close(sockconn);	
    mysql_close(conn);
    return 0;

}
/*
	PH->CS: if=putphinfo
	CS->PH: |msg=ok/error|
	PH->CS: |phip
			|cpumax*cpumin*cpuarg
			|memmax*memmin*memarg
			|memfmax*memfmin*memfarg
			|dskmax*dskmin*dskarg
			|dskfmax*dskfmin*dskfarg
			|dskrmax*dskrmin*dskrarg
			|dskwmax*dskwmin*dskwarg
			|netimax*netimin*netiarg
			|netomax*netomin*netoarg
			|time|
*/
void 
parser(char* Rec)
{
	char* r;
	char* q;
	char* p;
	char* delim="|";
	//mylog(Rec);
	char cputmp[512],memtmp[512],memftmp[512],dsktmp[512],dskftmp[512],dskrtmp[512],dskwtmp[512],netitmp[512],netotmp[512];

	r=strtok(Rec,delim); 
	strcpy(phip,r);
	if(index(phip,'\n'))
	{
		phip[strlen(phip)-1]='\0';
	}
	else
	{
		phip[strlen(phip)]='\0';
	}
	if((q=strtok(NULL,delim)))
	{
		strcpy(cputmp,q);
		cputmp[strlen(cputmp)]='\0';
	}
	if((q=strtok(NULL,delim)))
	{
		strcpy(memtmp,q);
		memtmp[strlen(memtmp)]='\0';
	}
	if((q=strtok(NULL,delim)))
	{
		strcpy(memftmp,q);
		memftmp[strlen(memftmp)]='\0';
	}
	if((q=strtok(NULL,delim)))
	{
		strcpy(dsktmp,q);
		dsktmp[strlen(dsktmp)]='\0';
	}
	if((q=strtok(NULL,delim)))
	{
		strcpy(dskftmp,q);
		dskftmp[strlen(dskftmp)]='\0';
	}
	if((q=strtok(NULL,delim)))
	{
		strcpy(dskrtmp,q);
		dskrtmp[strlen(dskrtmp)]='\0';
	}
	if((q=strtok(NULL,delim)))
	{
		strcpy(dskwtmp,q);
		dskwtmp[strlen(dskwtmp)]='\0';
	}
	if((q=strtok(NULL,delim)))
	{
		strcpy(netitmp,q);
		netitmp[strlen(netitmp)]='\0';
	}
	if((q=strtok(NULL,delim)))
	{
		strcpy(netotmp,q);
		netotmp[strlen(netotmp)]='\0';
	}
	if((q=strtok(NULL,delim)))
	{
		strcpy(mytime,q);
		mytime[strlen(mytime)]='\0';
	}
	if((q=strtok(NULL,delim)))
	{
		strcpy(mac,q);
		mac[strlen(mac)]='\0';
	}
	char *s;
	s=rindex(cputmp,'*');
	strcpy(cpu[2],s+1);
	s=rindex(memtmp,'*');
	strcpy(mem[2],s+1);
	s=rindex(memftmp,'*');
	strcpy(memf[2],s+1);
	s=rindex(dskftmp,'*');
	strcpy(dskf[2],s+1);
	s=rindex(dskrtmp,'*');
	strcpy(dskr[2],s+1);
	s=rindex(dskwtmp,'*');
	strcpy(dskw[2],s+1);
	s=rindex(netitmp,'*');
	strcpy(neti[2],s+1);
	s=rindex(netotmp,'*');
	strcpy(neto[2],s+1);
}
int openMysqlConn()
{
    conn = mysql_init(NULL);

    if(mysql_real_connect(conn,"127.0.0.1","root","654321","cs",0,NULL,0)==NULL)
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
	fp=fopen("/tmp/putphinfolog","a+");
	time(&now);
	timenow=localtime(&now);
	fprintf(fp,"%s %s\n\n",asctime(timenow),p);
	fclose(fp);
}
