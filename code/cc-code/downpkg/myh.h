#ifndef MYH_H_
#define MYH_H_

#include <sys/socket.h>
#include <sys/stat.h>
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
#include <fcntl.h>
#include <signal.h>

off_t filesize;
int sendfile(int,char*);
void mylog(const char *);
#endif