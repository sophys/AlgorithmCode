#include "app.h"

int main()
{
    char recvbuf[256]={'0'};
    char appid[8]={'0'};
    char userid[8]={'0'};
    char sendbuf[4096]={'0'};
    freopen("clickapp.log","w+",stdout);
    freopen("clickapp.log","a+",stderr);
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
