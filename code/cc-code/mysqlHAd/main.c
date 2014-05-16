#include "my.h"
/*
daemon启来后就检查网络：
0.网络畅通
	检测数据库状态：
		1. 没有启动 -> 检测flag标志：a.最新，则直接启动集群 
								  b.不是最新，则和另外节点通信以决定以何种方式启动数据库
										通信方式:node1 -> node2:|0|
												node1 <- node2:|0| or |1| or |2| (检测自己节点状态返回状态信息 0:集群没有启动 1:集群是最新但还没有启动 2:集群是最新也启动了)
										节点根据返回信息启动集群
		2.  已启动  -> 检测node各进程是否正常(检测是否有标记，有就清除标记)
										通信方式:node1 -> node2:|1|
												node1 <- node2:|0| or |1|(0:not clear 1:clear)

1.另外节点不通，网关通
	node1 down -> node2 down -->1.node1 up
								2.node2 up
								3.node1,node2 up。
2.都不通
	关掉集群
*/
int FLAG;//0:on 1:off
char INFO[3][32];
int
main(int argc,char *argv[])
{
	opterr=0;	
	char ch = getopt(argc,argv,"t");
	switch(ch)
	{
		case 't':
			printf("test mode......\n");
			break;
		default:			
			if(daemon(0,0))
			{
				mylog("Create Daemon Error");
			}
			break;
	}
	
	int s;
	if((s=readconf(CONFPATH)) != 0 )
 	{
     	if(-1 == s)
     	{
     		mylog("conf not exist:/var/mysqlhad.conf");
			printf("conf not exist:/var/mysqlhad.conf");
     	}
     	else
     	{
     		mylog("conf file error:nodeip gateway timesleep");
			printf("conf file error:nodeip gateway timesleep");
     	}
 		return -1;
 	}
	
 	pid_t pid;
	if((pid=fork())==0)
	{
		int listenfd = socket(AF_INET, SOCK_STREAM, 0);
		
		struct sockaddr_in servaddr;
		bzero(&servaddr, sizeof(servaddr));
		servaddr.sin_family      = AF_INET;
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servaddr.sin_port        = htons(LISTEN_PORT);

		bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr));
		listen(listenfd, LENGTH_OF_LISTEN_QUEUE);
		for (;;) 
		{			
			int len= sizeof(struct sockaddr_in);
			struct sockaddr_in	connaddr;

			int connfd = accept(listenfd, (struct sockaddr*) &connaddr,&len);			
			arbitration(connfd); 
		}
	}
	signal(SIGCHLD,sig_chld);
	while(1)
	{			
		int status=checknet(INFO[0],INFO[1],COUNT);
		switch(status)
		{
			case 0:// ping Node and GateWay
				checkstatus(INFO[0]);//Check Node'status
				if(makeflag(CHECK) == 0)
				{
					clearflag(INFO[0]);
				}
				printf("--------------------------network success--------------------------\n");
				break;
			case 1:// Can't ping Node error but ping GateWay 
				checknode();
				printf("------------------------other node down!!!------------------------\n");				
				break;
			case 2://Can't ping Node and GateWay
				if((0 == FLAG)&&(makeflag(CHECK) != 0))
				{
					FLAG=1;
					down();//This Node down
					mylog("===================-----this node down-----================");
					printf("===================-----this node down-----================");
				}
				printf("--------------------------network down--------------------------\n");
				break;
			default:
				break;
		}
		printf("sleep %s\n",INFO[2]);
		sleep(atoi(INFO[2]));
	}
    return 0;
}
