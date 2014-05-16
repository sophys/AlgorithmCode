#include "my.h"
#include "app.h"

MYSQL *conn = NULL;

int main()
{
    char recvbuf[256]={'0'};
    char appid[8]={'0'};
    char userid[8]={'0'};
    char apppri[8]="";
    char sendbuf[4096]={'0'};
    char sql[2048]="";

    MYSQL_RES *res_ptr;
    MYSQL_ROW sqlrow;

    int sockconn;
    sockconn=atoi(getenv("socket"));

    if(send(sockconn,"ok",2,0)==-1)
    {
        perror("Send error!");
        exit(EXIT_FAILURE);
    }

    if(recv(sockconn,recvbuf,sizeof(recvbuf),0)==-1)
    {
        perror("Recv error!");
        exit(EXIT_FAILURE);
    }

    if(0 != Translate(recvbuf,appid,userid))
    {
        perror("parser userid appid error");
        close(sockconn);
        mysql_close(conn);
        return 0;
    }

    if(openMysqlConn() != 0)
    {
        close(sockconn);
        mysql_close(conn);
        return -1;
    }

 /*
  `userid` int(11) NOT NULL,
  `vmid` int(11) NOT NULL,
  `vmuser` varchar(20) NOT NULL,
  `vmpasswd` datetime NOT NULL,
*/
    //check the apppri
    memset(sql,0,2048);
    snprintf(sql,2048,"SELECT apppri FROM app WHERE appid = '%s'",appid);
    if(mysql_real_query(conn,sql,strlen(sql)))
    {
        close(sockconn);
        mysql_close(conn);
        return -1;
    } 
    else
    {
        res_ptr = mysql_store_result(conn);
        if(res_ptr)
        {
            sqlrow = mysql_fetch_row(res_ptr);
            strncpy(apppri,sqlrow[0],8);           
        }
        else
        {
            close(sockconn);
            mysql_close(conn);
            return -1;
        }
        mysql_free_result(res_ptr);
    }
    if(strstr(apppri,"0"))//not privilege
    {
        send(sockconn,"|1|",3,0);
        recv(sockconn,recvbuf,sizeof(recvbuf),0);
        mysql_close(conn);
        if(AppHandler(appid,userid,sockconn,sendbuf)<0)
        {
            send(sockconn,sendbuf,strlen(sendbuf),0);
            cout<<"clickappERROR"<<endl;
            return -1;
        }
        return 0;
    }
    else if(strstr(apppri,"1"))//privilege app
    {
        if(0 != getvminfo(conn,userid,appid,sendbuf,sockconn))
        {
            printf("getvminfo error");
        }

        send(sockconn,sendbuf,strlen(sendbuf),0);
        close(sockconn);
        mysql_close(conn);
        return 0;
    }
    else
    {
        send(sockconn,"|3|",3,0);
        close(sockconn);
        mysql_close(conn);
        return 0;
    }
    close(sockconn);
    mysql_close(conn);
    return 0;
}

int openMysqlConn()
{
    conn = mysql_init(NULL);

    if(mysql_real_connect(conn,getenv("DBIP"),getenv("DBUSER"),getenv("DBPASS"),getenv("DBNAME"),0,NULL,0) == NULL)
    {
        return -1;
    }
    return 0;
}

