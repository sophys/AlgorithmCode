#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <sys/types.h>
#include <sys/socket.h>
#define BUFF_SIZE 200
enum mysql_para{dbip, dbname,dbuser,dbpasswd};
char mysql_arg[4][BUFF_SIZE]={0};
const char logpath[]="/var/cloudos/log/inittpl.log";
void writelog(const char* log,const char* path)
{
 FILE *fp;
 fp=fopen(path,"a+");
 if(!fp)
 return;
 fprintf(fp,"log:%s\n",log);
 fclose(fp);
}
int main()
{
char recvbuf[BUFF_SIZE]={0};
char tplmac[100]={0};
char tplmaxaccount[10]={0};
char tplmaxgranule[10]={0};
char sendmsg[100]={0};
int sockconn=atoi(getenv("socket"));
send(sockconn,"ok",2,0);
int recvlenth=recv(sockconn,recvbuf,BUFF_SIZE,0);
//writelog(recvbuf,logpath);
//Ω‚Œˆmacµÿ÷∑|tplmac=****|
char *start=NULL;
start=strstr(recvbuf,"=");
if(start)
{
  strcpy(tplmac,start+1);
}
else 
{
	writelog("send wrong",logpath);
}
int tpllenth=strlen(tplmac);
for (int i=0; i<tpllenth; i++)
{
	if(tplmac[i]=='|')
		tplmac[i]='\0';	
}
writelog(tplmac, logpath);
/*FILE *fp;
fp=fopen("/var/cloudos/conf/cc.conf","r");
if(!fp)
{
   printf("read conf_file fail!\n");
   return -1;
}
char para[BUFF_SIZE]={0};
fgets(para,BUFF_SIZE,fp);
char *p;
p=strstr(para,"=");
strcpy(mysql_arg[dbip],p+1);
mysql_arg[dbip][strlen(mysql_arg[dbip])-1]='\0';
writelog(mysql_arg[dbip], logpath);
memset(para,0,BUFF_SIZE);
fgets(para,BUFF_SIZE,fp);
p=strstr(para,"=");
strcpy(mysql_arg[dbname],p+1);
mysql_arg[dbname][strlen(mysql_arg[dbname])-1]='\0';
writelog(mysql_arg[dbname], logpath);
memset(para,0,BUFF_SIZE);
fgets(para,BUFF_SIZE,fp);
strcpy(mysql_arg[dbuser],p+1);
mysql_arg[dbuser][strlen(mysql_arg[dbuser])-1]='\0';
writelog(mysql_arg[dbuser],logpath);
memset(para,0,BUFF_SIZE);
fgets(para,BUFF_SIZE,fp);
p=strstr(para,"=");
strcpy(mysql_arg[dbpasswd],p+1);
mysql_arg[dbpasswd][strlen(mysql_arg[dbpasswd])-1]='\0';
writelog(mysql_arg[dbpasswd], logpath);
fclose(fp);*/
char *p;
p=getenv("DBIP");
if(p)
{
	strcpy(mysql_arg[dbip],p);
}
p=getenv("DBUSER");
if(p)
{
	strcpy(mysql_arg[dbuser],p);
}
p=getenv("DBNAME");
if(p)
{
	strcpy(mysql_arg[dbname],p);
}
p=getenv("DBPASS");
if(p)
{
	strcpy(mysql_arg[dbpasswd],p);
}
char mysql_cmd[BUFF_SIZE]={0};
MYSQL_RES *mysql_result;
MYSQL_ROW row;
MYSQL mysql;
mysql_init(&mysql);
mysql_options(&mysql,MYSQL_SET_CHARSET_NAME,"utf8");
if(mysql_real_connect(&mysql,mysql_arg[dbip],mysql_arg[dbuser],mysql_arg[dbpasswd],mysql_arg[dbname],0,NULL,0)==NULL)
{
  writelog("cannot connect database!", logpath);
  printf("cannot connect database!\n");
  return -1;
}
sprintf(mysql_cmd,"select count(*),maxaccount,maxgranule from tpl where tplmac=\"%s\";",tplmac);
// writelog(mysql_cmd, logpath);
if(0!=mysql_query(&mysql,mysql_cmd))
{
 writelog("Error making query", logpath);
 mysql_close(&mysql);
 printf("Error making query\n");           
 return -1;
 }
 mysql_result=mysql_store_result(&mysql);
 row = mysql_fetch_row(mysql_result);
 mysql_free_result(mysql_result);
 if(strcmp(row[0],"0") == 0)
 {
	   memset(mysql_cmd,0,sizeof(mysql_cmd));
	   sprintf(mysql_cmd,"select value from info where info.key=\"maxaccount\";");
	   if(0!=mysql_query(&mysql,mysql_cmd))
       {
          writelog("Error making query", logpath);
          mysql_close(&mysql);
          printf("Error making query\n");           
          return -1;
        }
       mysql_result=mysql_store_result(&mysql);
       row = mysql_fetch_row(mysql_result);
       mysql_free_result(mysql_result);
       strcpy(tplmaxaccount,row[0]);

	   memset(mysql_cmd,0,sizeof(mysql_cmd));
	   sprintf(mysql_cmd,"select value from info where info.key=\"maxgranule\";");
	   if(0!=mysql_query(&mysql,mysql_cmd))
       {
          writelog("Error making query", logpath);
          mysql_close(&mysql);
          printf("Error making query\n");           
          return -1;
        }
       mysql_result=mysql_store_result(&mysql);
       row = mysql_fetch_row(mysql_result);
       mysql_free_result(mysql_result);
       strcpy(tplmaxgranule,row[0]);

       sprintf(sendmsg,"|%s|%s|",tplmaxaccount,tplmaxgranule);
       send(sockconn,sendmsg,strlen(sendmsg),0);
       mysql_close(&mysql);
	   close(sockconn);
       return 0;
 }
else
{
   strcpy(tplmaxaccount,row[1]);
   strcpy(tplmaxgranule,row[2]);
   sprintf(sendmsg,"|%s|%s|",tplmaxaccount,tplmaxgranule);
   send(sockconn,sendmsg,strlen(sendmsg),0);
   mysql_close(&mysql);
   close(sockconn);
}
return 0;
}
