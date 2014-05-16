#include "my.h"
#include "app.h"

int Translate(char *src,char *appid,char *userid)
{
    char* ptmp=src;
    char* pvalue=NULL;

    if(!src)
        return -1;

    strsep(&ptmp,"=");

    pvalue=strsep(&ptmp,"|");
    if(!pvalue)
        return -1;

    strcpy(appid,pvalue);

    strsep(&ptmp,"=");

    pvalue=strsep(&ptmp,"|");
    if(!pvalue)
        return -1;

    strcpy(userid,pvalue);

    return 0;
}

int getvminfo(MYSQL *conn ,char *userid,char *appid,char *sendbuf,int sockconn)
{
    char sql[2048]="";
    MYSQL_RES *res_ptr;
    MYSQL_ROW sqlrow;
    unsigned long flag=0;

    char vmuser[8]="";
    char vmpasswd[8]="";
    char appname[32]="";
    char apptype[8]="";
    char vmid[8]="";
    char vmip[16]="";
    char vmport[]="3389";
    char cmd[1024]="";
    char granule[8]="";

    int max;
    int min;

    memset(sql,0,2048);
    snprintf(sql,2048,"SELECT DISTINCT userprivm.vmid,vmuser,vmpasswd,vm.vmip FROM userprivm,vm WHERE userprivm.userid='%s' and vm.cbid=(SELECT cbid FROM cbtplapp WHERE appid='%s') and vm.vmid=userprivm.vmid",userid,appid);
    printf("%s\n", sql);
    if(mysql_real_query(conn,sql,strlen(sql)))
    {
        mysql_close(conn);
        return -1;
    }
    else
    {
        res_ptr = mysql_store_result(conn);

        flag = (unsigned long)mysql_num_rows(res_ptr);
        if(!flag)
        {
            if(0 != getavm(conn,vmuser,vmpasswd,vmip,appid,userid))
            {
                return -1;
            }
        }
        else
        {
            sqlrow = mysql_fetch_row(res_ptr);
            strncpy(vmid,sqlrow[0],8);
            strncpy(vmuser,sqlrow[1],8);
            strncpy(vmpasswd,sqlrow[2],8);
            strncpy(vmip,sqlrow[3],16);
        }
        mysql_free_result(res_ptr);
    }

    memset(sql,0,2048);
    snprintf(sql,2048,"SELECT appname,apptype,granule FROM app WHERE appid='%s'",appid);
    printf("%s\n", sql);
    if(mysql_real_query(conn,sql,strlen(sql)))
    {
        mysql_close(conn);
        return -1;
    }
    else
    {
        res_ptr = mysql_store_result(conn);
        if(res_ptr)
        {
            sqlrow = mysql_fetch_row(res_ptr);
            strncpy(appname,sqlrow[0],32); 
            strncpy(apptype,sqlrow[1],8);
            strncpy(granule,sqlrow[2],8);         
        }
        else
        {
            mysql_close(conn);
            return -1;
        }
        mysql_free_result(res_ptr);
    }
////////////////  check the loald
//snprintf(sql,2048,"select vm.currentgranule,tpl.maxgranule from vm,tpl where vm.vmid=''%s' and vm.vmtpl=tpl.tplname",vmid);
    memset(sql,0,2048);
    snprintf(sql,2048,"select vm.currentgranule,tpl.maxgranule from vm,tpl where vm.vmid='%s' and vm.vmtpl=tpl.tplname",vmid);

    printf("%s\n", sql);
    if(mysql_real_query(conn,sql,strlen(sql)))
    {
        mysql_close(conn);
        return -1;
    }
    else
    {
        res_ptr = mysql_store_result(conn);
            if(res_ptr)
            {
                sqlrow = mysql_fetch_row(res_ptr);
                min = atoi(sqlrow[0]);
                max = atoi(sqlrow[1]);
            }
            else
            {
                mysql_close(conn);
                return -1;
            }
         mysql_free_result(res_ptr);
    }
    if((min+atoi(granule)) > max)
    {
        mysql_close(conn);
        char recvbuf[8]="";
        send(sockconn,"|2|",3,0);
        recv(sockconn,recvbuf,sizeof(recvbuf),0);

        if(AppHandler(appid,userid,sockconn,sendbuf)<0)
        {
            send(sockconn,sendbuf,strlen(sendbuf),0);
            cout<<"clickappERROR"<<endl;
            return -1;
        }
        return 0;
    }
    else
    {
        char recvbuf[8]="";
        send(sockconn,"|0|",3,0);
        recv(sockconn,recvbuf,sizeof(recvbuf),0);
    }


/////////////////////
    memset(sql,0,2048);
    snprintf(sql,2048,"SELECT vmip FROM vm WHERE vmid = '%s'",vmid);
    printf("%s\n", sql);
    if(mysql_real_query(conn,sql,strlen(sql)))
    {
        mysql_close(conn);
        return -1;
    }
    else
    {
        res_ptr = mysql_store_result(conn);
            if(res_ptr)
            {
                sqlrow = mysql_fetch_row(res_ptr);
                strncpy(vmip,sqlrow[0],8);
            }
            else
            {
                mysql_close(conn);
                return -1;
            }
         mysql_free_result(res_ptr);
    }

    
  //|infotype=rdf|setting=PPT*172.20.250.99*vmusername*vmpasswd*3389*opener C:\Program Files\Microsoft Office\OFFICE11\POWERPNT.EXE|
    
    memset(sql,0,2048);
    snprintf(sql,2048,"SELECT cmd FROM tplapp WHERE appid='%s'",appid);
    printf("%s\n", sql);
    if(mysql_real_query(conn,sql,strlen(sql)))
    {
        mysql_close(conn);
        return -1;
    }
    else
    {
        res_ptr = mysql_store_result(conn);
        if(res_ptr)
        {
            sqlrow = mysql_fetch_row(res_ptr);
            strncpy(cmd,sqlrow[0],1024);       
        }
        else
        {
            mysql_close(conn);
            return -1;
        }
        mysql_free_result(res_ptr);
    }

    snprintf(sendbuf,4096,"|infotype=%s|setting=%s*%s*%s*%s*%s*%s*%s|",apptype,appname,vmip,vmuser,vmpasswd,vmport,cmd,granule);
    return 0;
}

int getavm(MYSQL *conn,char* vmuser,char* vmpasswd,char* vmip,char *appid,char *userid)
{
    char sql[2048]="";
    MYSQL_RES *res_ptr;
    MYSQL_ROW sqlrow;  
    int  tmpvmid[COUNT];
    int i=0;
    int N=0;

    memset(sql,0,2048);
    snprintf(sql,2048,"SELECT vmid FROM vm  WHERE vmip<>'' AND cbid = (SELECT cbid FROM cbtplapp WHERE appid = %s)",appid);
    printf("%s\n", sql);
    if(mysql_real_query(conn,sql,strlen(sql)))
    {
        mysql_close(conn);
        return -1;
    } 
    else
    {
        res_ptr = mysql_store_result(conn);
        if(res_ptr)
        {
            while((sqlrow=mysql_fetch_row(res_ptr)))
            {
                tmpvmid[i++] = atoi(sqlrow[0]); 
            }
            N=i;                  
        }
        else
        {
            mysql_close(conn);
            return -1;
        }
        mysql_free_result(res_ptr);
    }
    // get a vmid random
    int times=0;
    while(1)
    {
        srand( time(NULL) );    
        memset(sql,0,2048);
        int vmid=tmpvmid[rand()%N];
        snprintf(sql,2048,"SELECT vmip FROM vm  WHERE vmid = '%d' AND (select count(vmid) from userprivm where vmid='%d')<(select distinct tpl.maxpriaccount from tpl,vm,userprivm where tpl.tplname=vm.vmtpl and vm.vmid=userprivm.vmid and vm.vmid='%d')",vmid,vmid,vmid);
      //  snprintf(sql,2048,"SELECT vmip FROM vm  WHERE vmid = '%d'",vmid);
        printf("%s\n", sql);
        printf("test the vm %s\n", vmip);
        if(mysql_real_query(conn,sql,strlen(sql)))
        {
            mysql_close(conn);
            return -1;
        } 
        else
        {
            res_ptr = mysql_store_result(conn);
            if(res_ptr)
            {
                if(res_ptr)
                {
                    sqlrow = mysql_fetch_row(res_ptr);
                    strncpy(vmip,sqlrow[0],16);       
                }                  
            }
            else
            {
                mysql_close(conn);
                return -1;
            }
            mysql_free_result(res_ptr);
        }
        if(vmip)
        {
            if(testthevm(vmip,vmuser,vmpasswd) == 0)
            {
                if(0 == insertthevm(conn,userid,vmid,vmuser,vmpasswd))break;
            }
        }

        times++;
        if(times++ > 5)
        {
            return -1;
        } 
    }
    return 0;
}

int testthevm(char *vmip,char* vmuser,char* vmpasswd)
{
    int sockfd ;
    struct sockaddr_in serv_addr;
    char SendBuf[1024]="";
    char RecvBuf[1024]="";

    if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
    {
        printf("1%s\n",strerror(errno));
        return -1;
    }

    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(50000);
    serv_addr.sin_addr.s_addr = inet_addr(vmip);

    if(connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr))==-1)
    {
        printf("2%s\n",strerror(errno));
        return -1;
    }

    if(send(sockfd,"if=getpriuser\n", strlen("if=getpriuser\n"), 0) < 0)
    {
        printf("3%s\n",strerror(errno));
        return -1;
    }

    if(recv(sockfd, RecvBuf, sizeof(RecvBuf), 0) < 0)
    {
        printf("4%s\n",strerror(errno));
        return -1;
    }
    //|username|passwd|
    char *p,*q;
    int count = 0;
    p=q=RecvBuf;
    p++;
    q++;
    //printf(RecvBuf);
    while(*p++)
    {
        if(count == 0)
        {
            if(*p == '|')
            {           
                strncpy(vmuser,q,p-q);
                q=p+1;
                p++;
                count=1;
            }

        }
        if(count == 1)
        {
            if(*p == '|')
            
            {
                strncpy(vmpasswd,q,p-q);
                q=p+1;
            }   

        }    
    }
    return 0;
}
int insertthevm(MYSQL *conn,char* userid,int vmid,char *vmuser,char* vmpasswd)
{
    char sql[2048]="";
    memset(sql,0,2048);
    snprintf(sql,2048,"INSERT INTO userprivm(userid,vmid,vmuser,vmpasswd) VALUES ('%s','%d','%s','%s')",userid,vmid,vmuser,vmpasswd);
    if(mysql_real_query(conn,sql,strlen(sql)))
    {
        mysql_close(conn);
        return -1;
    } 
    return 0;
}
void mylog(const char *p)
{
    struct tm *timenow;
    time_t now;
    FILE *fp;
    fp=fopen("/tmp/clickpriapp","a+");
    time(&now);
    timenow=localtime(&now);
    fprintf(fp,"%s %s\n\n",asctime(timenow),p);
    fclose(fp);
}
