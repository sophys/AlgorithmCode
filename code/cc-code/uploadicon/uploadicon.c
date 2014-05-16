#include <sys/socket.h>
#include "/usr/include/iconv.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>

static void mylog(const char *);
int code_convert(char *from_charset, char *to_charset, char *inbuf,int inlen, char *outbuf, int outlen);
int u2g(char *inbuf, int inlen, char *outbuf, int outlen);
int g2u(char *inbuf, size_t inlen, char *outbuf, size_t outlen);

typedef struct icon
{
	char name[1024];
	int size;
}icon;
/*
	c>s:if=uploadicon
	s>c:ok
	c>s:icon_info
	s>c:|msg=ok|或者|msg=icon exist|
	c>s:icon_file
	s>c:|msg=ok|或者|msg=icon error|
*/
int main(int argc,char **argv)
{
	int sockconn=0;
	char *msg;
	//接收图标
	icon icon;
	int fq;
	char path[128];
	char path_tmp[128];
	char test[64];
	int sizeNum=0;
	int nbytes=0;
	char buff[2048];
	char name[1024];
	char recv2[16];
	
    sockconn=atoi(getenv("socket"));
    send(sockconn,"ok",2,0);
	
	recv(sockconn,(char *)&icon,sizeof(struct icon),0);
	
	//转码字符串 windows  已经做
	// if(g2u(icon.name,1024,name,1024) != 0)
	// {
	// 	mylog(strerror(errno));
	// 	msg="|msg=icon error|";
	// 	send(sockconn,msg,strlen(msg),0);
	// 	close(sockconn);
	// 	return -1;
	// }

	snprintf(path,128,"/var/www/ca/icon/%s",icon.name);
	snprintf(test,64,"file size %d",icon.size);
	mylog(path);//////////////////////
	mylog(test);

	if((access(path,F_OK))==0)
	{
		/*	c>s:yes/ no
		  s>c:ok
		  c>s: icon_file
		  s>c:|msg=ok|或者|msg=icon error| */
		msg="|msg=icon exist|";
		send(sockconn,msg,strlen(msg),0);
		recv(sockconn,recv2,16,0);
		if(strstr(recv2,"yes"))
		{
			send(sockconn,"ok",2,0);
			if(remove(path) != 0)
			{
				mylog(strerror(errno));
				msg="|msg=icon error|";
				send(sockconn,msg,strlen(msg),0);
				close(sockconn);
				return -1;
			}
			if((fq=open(path,O_RDWR|O_CREAT|O_APPEND,0777))<0)
			{
				mylog(strerror(errno));
				msg="|msg=icon error|";
				send(sockconn,msg,strlen(msg),0);
				close(sockconn);
				return -1;
			}
			while(1)
			{
				if((nbytes=read(sockconn,buff,2047))<0)
				{
					mylog(strerror(errno));
					msg="|msg=icon error|";
					send(sockconn,msg,strlen(msg),0);
					close(sockconn);
					return -1;
				}
				buff[nbytes]='\0';
				sizeNum+=nbytes;				
				if(sizeNum<=icon.size)
				{
					if(write(fq,buff,nbytes)<0)
					{
						mylog(strerror(errno));
						msg="|msg=icon error|";
						send(sockconn,msg,strlen(msg),0);
						close(sockconn);
						return -1;
					}
				}
				if(sizeNum == icon.size)
				{

					mylog("receive successful");
					close(fq);
					msg="|msg=ok|";
					send(sockconn,msg,strlen(msg),0);
					break;
				}
				if(sizeNum>icon.size)
				{
					mylog(strerror(errno));
					msg="|msg=icon error|";
					send(sockconn,msg,strlen(msg),0);
					close(sockconn);
					return -1;
				}
			}	
		}
		else
		{	close(sockconn);//不覆盖，结束通信
			return 0;
		}
	}
	else
	{//对图标信息确认
		msg="|msg=ok|";
		send(sockconn,msg,strlen(msg),0);

		if((fq=open(path,O_RDWR|O_CREAT|O_APPEND,0777))<0)
		{
			mylog(strerror(errno));
			msg="|msg=icon error|";
			send(sockconn,msg,strlen(msg),0);
			close(sockconn);
			return -1;
		}
		while(1)
		{
			if((nbytes=read(sockconn,buff,2047))<0)
			{
				mylog(strerror(errno));
				msg="|msg=icon error|";
				send(sockconn,msg,strlen(msg),0);
				close(sockconn);
				return -1;
			}
			buff[nbytes]='\0';
			sizeNum+=nbytes;				
			if(sizeNum<=icon.size)
			{
				if(write(fq,buff,nbytes)<0)
				{
					mylog(strerror(errno));
					msg="|msg=icon error|";
					send(sockconn,msg,strlen(msg),0);
					close(sockconn);
					return -1;
				}
			}
			if(sizeNum == icon.size)
			{

				mylog("receive successful");
				close(fq);
				msg="|msg=ok|";
				send(sockconn,msg,strlen(msg),0);
				break;
			}
			if(sizeNum>icon.size)
			{
				mylog(strerror(errno));
				msg="|msg=icon error|";
				send(sockconn,msg,strlen(msg),0);
				close(sockconn);
				return -1;
			}
		}	
	}
    return 0;
}

int code_convert(char *from_charset, char *to_charset, char *inbuf,int inlen, char *outbuf, int outlen)
{
   iconv_t cd;
   int rc;
   size_t inl,outl;
   inl=inlen;
   outl=outlen;
   char **pin = &inbuf;
   char **pout = &outbuf;
   int inlentmp=inlen;
   int outlentmp=outlen;
   cd = iconv_open(to_charset, from_charset);
   if (cd == 0)
      return -1;
   memset(outbuf, 0, outlen);
   if (iconv(cd, pin, &inl, pout, &outl) == -1)
      return -1;
   iconv_close(cd);
   return 0;
}

//UNICODE码转为GB2312码
int u2g(char *inbuf, int inlen, char *outbuf, int outlen)
{
   return code_convert("UTF-8", "GB2312", inbuf, inlen, outbuf, outlen);
}

//GB2312码转为UNICODE码
int g2u(char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
   return code_convert("gb2312", "utf-8", inbuf, inlen, outbuf, outlen);
}

static void mylog(const char *p)
{
	struct tm *timenow;
	time_t now;
	FILE *fp;
	fp=fopen("/tmp/uploadiconlog","a+");
	time(&now);
	timenow=localtime(&now);
	fprintf(fp,"%s %s\n",asctime(timenow),p);
	fclose(fp);
}
