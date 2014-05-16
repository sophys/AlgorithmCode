#include "app.h"

int main()
{
    char recvbuf[256]={'0'};
    char appid[8]={'0'};
    char userid[8]={'0'};
    char sendbuf[4096]={'0'};
    freopen("/var/cloudos/cc/clickapp.log","w+",stdout);
    freopen("/var/cloudos/cc/clickapp.log","a+",stderr);



    char path[256];
    if(getcwd(path,256) == NULL)
    {
        perror("getcwd error");
        exit(EXIT_FAILURE);
    }
    cout<<"main:"<<path<<endl;

    chdir("/var/cloudos/cc");

    int sockfd;
    sockfd=atoi(getenv("socket"));
    if(send(sockfd,"ok",2,0)==-1)
    {
        perror("Send error!");
        exit(EXIT_FAILURE);
    }

    if(recv(sockfd,recvbuf,sizeof(recvbuf),0)==-1)
    {
        perror("Recv error!");
        exit(EXIT_FAILURE);
    }
    cout<<"recv:"<<recvbuf<<endl;
    Translate(recvbuf,appid,userid);
    cout<<"appid:"<<appid<<endl;
    cout<<"userid:"<<userid<<endl;
   // AppHandler(appid,userid,sendbuf);
    //cout<<sendbuf<<endl;
    //int sockfd=0;
   // char sendbuf[1024]={'0'};
    if(AppHandler(appid,userid,sockfd,sendbuf)<0)
    {
        send(sockfd,sendbuf,strlen(sendbuf),0);
        cout<<"clickappERROR"<<endl;
        return -1;
    }
    close(sockfd);
    return 0;
}
