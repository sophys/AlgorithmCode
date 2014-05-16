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
int openMysqlConn();
int getvmid(char *id,char *mac);
 
char vmip[32];
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
char id[16];

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

	if(getvmid(id,mac) != 0)
    {
    	mylog("getvmid error!");
    	return -1;
   	}

    if(openMysqlConn() != 0)
    {
    	mylog("Connect Database error!");
    	return -1;
   	}
	//得到phid
	memset(sql,0,2048);
	snprintf(sql,2048,"SELECT phid FROM ph_list WHERE phip = '%s'",phip);
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
				msg="|msg=query phip by vmip error |";
				send(sockconn,msg,strlen(msg),0);
				close(sockconn);
				mysql_close(conn);
				return -1;
			}
			else
			{
				sqlrow = mysql_fetch_row(res_ptr);
				strncpy(phid,sqlrow[0],32);
			}
			
		}
		else
		{
			mylog(mysql_error(conn));
			msg="|msg=cs database error|";
			send(sockconn,msg,strlen(msg),0);
			close(sockconn);
			mysql_close(conn);
			return -1;
		}
		mysql_free_result(res_ptr);
	}

	//查询 vmip ，检查是 更新还是插入
	memset(sql,0,2048);
	snprintf(sql,2048,"select * from vm_list where vmmac='%s'",mac);
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
			if(!flag)//不存在,插入
			{
				memset(sql,0,2048);
				snprintf(sql,2048,"INSERT INTO vm_list(vmip,vmmac,state,phid,cpuinfos,meminfos,diskinfos,netinfos,id) VALUES ('%s','%s','on','%s','%s','%s*%s','%s*%s*%s','%s*%s','%s')",vmip,mac,phid,cpu[2],mem[2],memf[2],dskf[2],dskr[2],dskw[2],neti[2],neto[2],id);
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
				snprintf(sql,2048,"UPDATE vm_list SET vmip='%s',vmmac='%s',state='on',phid='%s',cpuinfos='%s',meminfos='%s*%s',diskinfos='%s*%s*%s',netinfos='%s*%s',id='%s' WHERE vmmac='%s'",vmip,mac,phid,cpu[2],mem[2],memf[2],dskf[2],dskr[2],dskw[2],neti[2],neto[2],id,mac);
				//mylog(sql);
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
	VM->CS：if=subtocsinfo
	CS->VM :  |msg=ok/error|
	VM->CS： 
	|vmip
	|cpuratemax*cpuratemin*cpuratearg
	|memratemax*memratemin*memratearg
	|memfreemax*memfreemin*memfreearg
	|diskratemax*diskratemin*diskratearg
	|diskfreemax*diskfreemin*diskfreearg
	|diskreadmax*diskreadmin*diskreadarg
	|diskwritemax*diskwritemin*diskwritearg
	|netinratemax*netinratemin*netinratearg
	| netoutratemax* netoutratemin*netoutratearg 
	|time
	|phip|

|172.20.250.24|6.67%*10.00%*18.589%|82%*82%*82.0%|101.32 MB*95.14 MB*97.44 MB|39%*39%*39.0%|60.02 GB*60.02 GB*60.02 GB|3.83 MB*0.00 B*205.29 KB|204.90 KB*0.00 B*67.20 KB|4.56 KB*0.00 B*1.57 KB|6.17 KB*0.00 B*2.79 KB|2013-09-14 00:29:36|172.20.2.5|
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
	strcpy(vmip,r);
	vmip[strlen(vmip)]='\0';

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
		strcpy(phip,q);
		phip[strlen(phip)]='\0';
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
int getvmid(char *id,char *mac)
{
/*
	root@debian:~# nc 127.0.0.1 50000
	if=getphipbyvmmac
	OK|mac=50:12:00:00:00:1b|
	|host=172.16.33.12|vmid=27|
*/
	int sockfd;
	struct sockaddr_in serv_addr;

	char fst[]="if=getphipbyvmmac\n";
	char snd[128];
	char rec[512];
	char *p;	

	snprintf(snd,128,"|mac=%s|\n",mac);

	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		mylog(strerror(errno));
		return -1;
	}
	
	bzero(&serv_addr,sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(50000);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	if(connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr))==-1)
	{
		mylog(strerror(errno));
		return -1;
	}
    
	send(sockfd,fst,strlen(fst),0);
	recv(sockfd,rec,512,0);
	send(sockfd,snd,strlen(snd),0);
	memset(rec,0,512);
	recv(sockfd,rec,512,0);
	//printf(rec);
	p=rindex(rec,'=');
	sprintf(id,"%s",p+1);
	id[strlen(id)-1]='\0';
	return 0;

}
int openMysqlConn()
{
    conn = mysql_init(NULL);

    if(mysql_real_connect(conn,"127.0.0.1","root","654321","cs",0,NULL,0) == NULL)
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
	fp=fopen("/tmp/subtocsinfolog","a+");
	time(&now);
	timenow=localtime(&now);
	fprintf(fp,"%s %s\n\n",asctime(timenow),p);
	fclose(fp);
}