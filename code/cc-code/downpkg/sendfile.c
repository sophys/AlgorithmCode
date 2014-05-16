#include "myh.h"
#define BUFSIZE 4096
int sendfile(int sockfd,char *path)
{
	int fd,n=0,m;
	off_t sendsize = 0;

	void *buf = NULL;

	if((buf = malloc(BUFSIZE)) == NULL)
	{
		mylog("malloc error in filewrite for file\n");
		return -1;
	}
	if((fd = open(path,O_RDONLY)) < 0)
	{
		mylog("open pkgfile error\n");
		free(buf);
		return -1;
	}

	while(sendsize < filesize)
	{
		if((n = pread(fd,buf,BUFSIZE,sendsize)) < 0)
		{
			mylog("pread error in filewrite");
			return -1;
			break;
		}
		send(sockfd,buf,n,0);
		sendsize = sendsize+n;
		memset(buf,0,BUFSIZE);
	}
	close(fd);
	free(buf);
	return 0;
}