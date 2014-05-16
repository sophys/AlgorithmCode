#include "my.h"

void mylog(const char *p)
{
	FILE *fp;
	fp=fopen("/var/mysqlhadlog","a+");
	time_t timeval;
	(void)time(&timeval);
	fprintf(fp,"%s%s\n", ctime(&timeval),p);
	fclose(fp);
}
int start(int SEL)
{
	switch(SEL)
	{
		case MGM:
			system(MP);
			mylog("--------------------------MGM start--------------------------");
			if(1 == FLAG)
			{
				FLAG = 0;
			}
			break;
		case NDB:
			system(NP);
			mylog("--------------------------NDB start--------------------------");
			if(1 == FLAG)
			{
				FLAG = 0;
			}
			break;
		case SQL:
			system(SP);
			mylog("--------------------------SQL start--------------------------");
			if(1 == FLAG)
			{
				FLAG = 0;
			}
			break;
		case ALL:
			system(MP);			
			system(NP);
			system(SP);
			mylog("--------------------------CLUSTER start--------------------------");
			if(1 == FLAG)
			{
				FLAG = 0;
			}
			break;
		default:
			break;
	}
	return 0;
}
int down()
{
	system(DOWN);
	system("killall -9 mysqld_safe");
	system("killall -9 mysqld");
	mylog("--------------------------CLUSTER down--------------------------");
}
/*
int checkConnect(char *dst, int cnt)
{
    int i = 0;
    FILE *stream;
    char recvBuf[16] = {0};
    char cmdBuf[256] = {0};

    sprintf(cmdBuf, "ping %s -c %d -i 0.2 | grep time= | wc -l", dst, cnt);
    stream = popen(cmdBuf, "r");
    fread(recvBuf, sizeof(char), sizeof(recvBuf)-1, stream);
    pclose(stream);

    if (atoi(recvBuf) > 0)
    {
    	return 0;
    }
    else
    {
    	return -1;
    }

}*/
int checknet(char* nodeip,char* gateway,int count)
{
    if(checkConnect(nodeip,count) == 0)
    {
    	return 0;
    }
    else
    {
        if(checkConnect(gateway,count) == 0)
        	return 1;
        else
        	return 2;
    }
}

int detect_process(char * process_name)
{
        FILE *ptr;
        char buff[512];
        char ps[128];
        sprintf(ps,"ps -e | grep -c '%s'",process_name);

        if((ptr=popen(ps, "r")) != NULL)
        {
            fgets(buff, 512, ptr);
        }     
        pclose(ptr);
        return atoi(buff);
}

int arbitration(int connfd)
{
	char recv[4]="";
	char send[4]="";
	read(connfd,recv,4);

	if(index(recv,'1'))
	{
		int a=detect_process("ndb_mgmd");
		int b=detect_process("ndbd");
		if(a||b)
		{
			write(connfd,"|1|\n",strlen("|1|"));
			close(connfd);
		}
		else
		{
			write(connfd,"|0|\n",strlen("|0|"));
			close(connfd);
		}
	}
	if(index(recv,'0'))
	{
		int flag=makeflag(CHECK);
		if(flag == 0)
		{
			int status=(detect_process("ndb_mgmd")&&detect_process("ndbd"));
			if(status)
			{
				sleep(20);//wait start completely...
				write(connfd,"|2|\n",strlen("|2|"));
				close(connfd);
			}
			else
			{
				write(connfd,"|1|\n",strlen("|1|"));
				close(connfd);
			}	
		}
		else
		{
			write(connfd,"|0|\n",strlen("|0|"));
			close(connfd);
		}
	}
	
	return 0;
}


void checknode()
{
/*	1.	node1 down -> node2 down -->1.node1 up
									2.node2 up
									3.node1,node2 up。: checkstatus()
	2.  node1 down:node2 up
*/
	int a=detect_process("ndb_mgmd");
	int b=detect_process("ndbd");
	int c=detect_process("mysqld");
	printf("***checknod*** a %d b %d c %d sum %d \n",a,b,c,(a||b||c));
	if(a||b)//	2.  node1 down:node2 up
	{
		makeflag(SET);
		if(a==0)
		{
			start(MGM);
		}
		if(b==0)
		{
			start(NDB);
		}
		if(c==0)
		{
			start(SQL);
		}
		return;
	}
	int flag=makeflag(CHECK);
	if(flag == 0)//node1 down -> node2 down --> node2 up									
	{
		if(a&&b&&c)
		{
			start(ALL);
		}
	}
	else
	{
		//This node down,after a while the other node down,and also after a while this node up ...
	}
	return ;
}
int checkstatus(char *ip)
{
	//test cluster
	int a=detect_process("ndb_mgmd");
	int b=detect_process("ndbd");
	int c=detect_process("mysqld");
	printf("###checkstatus### a %d b %d c %d sum %d \n",a,b,c,(a||b));
	if(a||b)
	{

		if(a==0)
		{
			start(MGM);
		}
		if(b==0)
		{
			start(NDB);
		}
		if(c==0)
		{
			start(SQL);	

		}
	}
	else
	{
		int flag=makeflag(CHECK);
		if(flag == 0)//This Node'Data is the newest,but the cluster doesn't start,so start it.
		{
			start(ALL);
		}	
		else//This node'data isn't the newest and can't determine to start the cluster or not,so check it with the other node.
		{
			struct sockaddr_in server_addr;
			int sockfd;
			char recvline[4];
			bzero(&server_addr,sizeof(server_addr)); 
			server_addr.sin_family = AF_INET;
			server_addr.sin_addr.s_addr = inet_addr(ip);
			server_addr.sin_port = htons(LISTEN_PORT);

			sockfd=socket(AF_INET,SOCK_STREAM,0);
			if(connect(sockfd,(struct sockaddr*)&server_addr,sizeof(struct sockaddr))==-1)
			{
		    	mylog(strerror(errno));
		    	mylog("------------------------the other node daemon down ------------------------\n");
		    	return -1;
			}
			write(sockfd,"|0|\n", strlen("|0|"));
			read(sockfd, recvline, 4);
			if(index(recvline,'1'))
			{
				// The other node'data is the newest,but doesn't started
			}
			else
			{ 
				//The other node'data isn't the newest or already starting , so starting the database.
				start(ALL);
			}
		}
	}
	return 0;
}

void clearflag(char *ip)
{
	struct sockaddr_in server_addr;
	int sockfd;
	char recvline[4];
	bzero(&server_addr,sizeof(server_addr)); 
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr(ip);
	server_addr.sin_port = htons(LISTEN_PORT);

	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(connect(sockfd,(struct sockaddr*)&server_addr,sizeof(struct sockaddr))==-1)
	{
		mylog(strerror(errno));
		mylog("------------------------the other node daemon down ------------------------\n");
		return ;
	}
	write(sockfd,"|1|\n", strlen("|1|"));
	read(sockfd, recvline, 4);
	if(index(recvline,'1'))
	{
		// clear the flag
		makeflag(RESET);
		mylog("--------------------------Flag reset--------------------------\n");
	}
	else
	{ 
		//The other node isn't starting 
	}
}
int makeflag(int CMD)
{
	int res;

	switch (CMD)
	{
		case SET:
			    {
					if(-1 == access(PATH,F_OK))
					{
						if(-1==(res=open(PATH,O_CREAT|O_RDONLY,0700)))
						{
							return -1;
						}
						else
						{
							close(res);
							return 0;
						}
					}
					else
					{
						return 0;
					}
		        }//CREATE
		case RESET:
			    {
					if(-1==(res=unlink(PATH)))
					{
						return -1;
					}
					else
					{
						return 0;
					}
		        }//REMOVE
		case CHECK:
		        {
					if(-1 == access(PATH,F_OK))
					{
						return -1;
					}
					else
					{
						return 0;
					}
		        }//CHECK
		default:
		        {
			       return -2;
		        }//default
	}//switch
}
void  sig_chld(int signo)
{
	pid_t pid;
	int stat;
	while((pid=waitpid(-1,&stat,WNOHANG))>0)
	{
	//	mylog("====================detect process terminated!!!!!!============");
	}
	return;
}
int readconf(char *filename)
{	
    FILE *fp;
    char buf[100];
    int i = 0;
    if(NULL == (fp = fopen(filename, "r")))
    {
    	return -1;
    }
    while (fgets(buf, 100, fp) != NULL ) {
        buf[strlen(buf)-1]='\0';
        strcpy(INFO[i++],buf);
        if(i==3)break;       
    }
    if(i<3)
    {
    	return -2;
    }
    return 0;
}
