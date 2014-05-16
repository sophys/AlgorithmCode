#include "my.h"
#define PACKET_SIZE     4096
 
unsigned short cal_chksum(unsigned short *addr, int len)
{
    int nleft=len;
    int sum=0;
    unsigned short *w=addr;
    unsigned short answer=0;
 
    while(nleft > 1)
    {
        sum += *w++;
        nleft -= 2;
    }
 
    if( nleft == 1)
    {       
        *(unsigned char *)(&answer) = *(unsigned char *)w;
        sum += answer;
    }
 
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;
 
    return answer;
}
int ping(char *ips)  
{      
    struct timeval *tval;        
    int maxfds = 0;  
    fd_set readfds;  
 
    struct sockaddr_in addr;  
    struct sockaddr_in from;  
 
    bzero(&addr,sizeof(addr));  
    addr.sin_family = AF_INET;  
    addr.sin_addr.s_addr = inet_addr(ips);  
 
    int sockfd;  

    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);  
    if (sockfd < 0)  
    {   
		mylog(strerror(errno));
        close(sockfd);
		return -1;  
    }  
 
    struct timeval timeo;  
    timeo.tv_sec = 2;  
    timeo.tv_usec = 0;  
 
    if (setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, &timeo, sizeof(timeo)) == -1)  
    {   
        mylog(strerror(errno));
		close(sockfd);
		return -1;  
    }  
 
    char sendpacket[PACKET_SIZE];  
    char recvpacket[PACKET_SIZE];  
    memset(sendpacket, 0, sizeof(sendpacket));  
 
    pid_t pid;  
    pid=getpid();  
 
    struct ip *iph;  
    struct icmp *icmp;  
 
    icmp=(struct icmp*)sendpacket;  
    icmp->icmp_type=ICMP_ECHO;
    icmp->icmp_code=0;  
    icmp->icmp_cksum=0;  
    icmp->icmp_seq=0;  
    icmp->icmp_id=pid; 
    tval= (struct timeval *)icmp->icmp_data;  
    gettimeofday(tval,NULL);  
    icmp->icmp_cksum=cal_chksum((unsigned short *)icmp,sizeof(struct icmp));
    int n = sendto(sockfd, (char *)&sendpacket, sizeof(struct icmp), 0, (struct sockaddr *)&addr, sizeof(addr));  
    if (n < 1)  
    {   
		mylog(strerror(errno));
        close(sockfd);
		return -1;  
    }  
 
    while(1)  
    {   
        FD_ZERO(&readfds);  
        FD_SET(sockfd, &readfds);  
        maxfds = sockfd + 1;  
        n = select(maxfds, &readfds, NULL, NULL, &timeo);  
        if (n <= 0)  
        {   
            mylog(strerror(errno));
			close(sockfd);  
            return -1;  
        }  
 
        memset(recvpacket, 0, sizeof(recvpacket));  
        int fromlen = sizeof(from);  
        n = recvfrom(sockfd, recvpacket, sizeof(recvpacket), 0, (struct sockaddr *)&from, (socklen_t *)&fromlen);
        if (n < 1) {  
            mylog(strerror(errno));
			close(sockfd);
			return -1;
        }  
        char *from_ip = (char *)inet_ntoa(from.sin_addr);  
        if (strcmp(from_ip,ips) != 0)  
        {  
			close(sockfd);
			return -1;
        }  
		iph = (struct ip *)recvpacket;   
        icmp=(struct icmp *)(recvpacket + (iph->ip_hl<<2));  

        if (icmp->icmp_type == ICMP_ECHOREPLY && icmp->icmp_id == pid)
        {
			close(sockfd);
			return 0; 
        }  
        else  
        {  
            continue;  
        }  
    }  
} 
int checkConnect(char *ip,int count)
{
	int i=0;
	while(i<count)
	{
		if(ping(ip) == 0)
		{
			printf("ping count %d\n",i);
			return 0;
		}
		i++;
	}
	return -1;
}
