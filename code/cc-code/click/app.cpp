#include "app.h"
int OpenSO(char *app,char *appid,char *userid,MYSQL *mysqlconn,int sockfd)
{
    void *handler;
    APPFUNC f;
    char apptmp[32];//append app to "./"
    sprintf(apptmp,"./%s",app);

    char path[256];
    if(getcwd(path,256) == NULL)
    {
        perror("getcwd error");
        exit(EXIT_FAILURE);
    }
    cout<<"OpenSO:"<<path<<endl;

    handler=dlopen(apptmp,RTLD_LAZY);
    if(handler==NULL)
    {
        printf("%s\n",dlerror());
       // strcpy(msg,"dlerror");
        return -1;
    }
    f=(APPFUNC)dlsym(handler,app);
    if(f==NULL)
    {
        cout<<"not find func"<<endl;
        return -1;
    }
    if(setenv("APPID",appid,1))
    {
        perror("setenv");
        return -1;
    }
    if(setenv("USERID",userid,1))
    {
        perror("setenv");
        return -1;
    }
    char tmp[10]={'0'};
    sprintf(tmp,"%d\n",mysqlconn);
    if(setenv("DBFD",tmp,1))
    {
        perror("setenv");
        return -1;
    }
    char *p;
    p=getenv("DBFD");
    MYSQL *x;
    x=(MYSQL*)atoi(p);
    if(x==mysqlconn)
    {
        cout<<"OK"<<endl;
    }
    bzero(tmp,10);
    sprintf(tmp,"%d",sockfd);
    setenv("CLIENTFD",tmp,1);
    f();
    mysql_close(mysqlconn);
    return 0;
}
int GetDbInfo(char* db_dbadmin,char* db_dbpwd,char *db_dbip,char *db_dbname)
{
	strcpy(db_dbadmin,getenv("DBUSER"));
	strcpy(db_dbpwd,getenv("DBPASS"));
	strcpy(db_dbip,getenv("DBIP"));
	strcpy(db_dbname,getenv("DBNAME"));
	if(getenv("DBUSER")==NULL||getenv("DBPASS")==NULL||getenv("DBIP")==NULL||getenv("DBNAME")==NULL){
		return -1;

}
return 0;
}
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
int AppHandler(char *appid,char * userid,int sockfd,char *msg)
{
    int res;
    char db_admin[128]={'0'},db_dbpwd[256]={'0'}, db_dbip[32]={'0'}, db_dbname[128]={'0'};
    MYSQL mysqlconn;
    MYSQL_RES* mysql_res;
    MYSQL_ROW mysql_row;
    char sql[1024];
    char apptype[16];
    char apppri[8]="";//
   if(-1== GetDbInfo( db_admin, db_dbpwd,db_dbip,db_dbname))
    {
     cout<<"GetDbInfoError\n";
        return -1;
   }
    cout<<db_admin<<endl;
    cout<<db_dbip<<endl;
    cout<<db_dbname<<endl;
    cout<<db_dbpwd<<endl;
    mysql_init(&mysqlconn);
    if(NULL==mysql_real_connect(&mysqlconn,db_dbip,db_admin,db_dbpwd,db_dbname,0,NULL,0))
    {
        //strcpy(msg,"|result=failed|msg=connect mysql error1|");
        strcpy(msg,"|002|msg=connect mysql error1|");
        return -1;
    }
    snprintf(sql,sizeof(sql),"select apptype,apppriv from app where appid = %s",appid);

    res=mysql_query(&mysqlconn,sql);

    if(!res)
    {
        mysql_res=mysql_store_result(&mysqlconn);
        if(mysql_res)
        {
            int affected_rows=mysql_affected_rows(&mysqlconn);
            if(affected_rows==0)
            {
                strcpy(msg,"|004|msg=app not found2|");
                return -1;
            }
            else if(affected_rows>1)
            {
                strcpy(msg,"|004|msg=app not unique3|");
                return -1;
            }
            mysql_row=mysql_fetch_row(mysql_res);
        }
        cout<<mysql_row[0]<<endl;
        strcpy(apptype,mysql_row[0]);

        cout<<mysql_row[1]<<endl;//
        strcpy(apppri,mysql_row[1]);//
        mysql_free_result(mysql_res);
        char appso[32];// next open .so file
        if(atoi(apppri))
        {
            sprintf(appso,"apppriv");
        }
        else
        {
            sprintf(appso,"app%s",apptype);
        }        
        cout<<appso<<endl;

        if(OpenSO(appso,appid,userid,&mysqlconn,sockfd)<0)
        {
            cout<<"errro\n";
            strcpy(msg,"open so file failed");
            strcpy(msg,"|003|open so file failed|");
            return -1;
        }
        //cout<<msg<<endl;
        cout<<"open so file success\n";
        return 0;
       // test( &mysqlconn,1);
    }
    else {strcpy(msg,"|004|mysql query failed|");return -1;}
}

