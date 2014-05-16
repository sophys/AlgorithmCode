#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LEN 1000
#define MAX 65535
 struct data {
	char T[4][5];
	char L[2][5];
	char *value;
};
typedef struct data Data;

int  Hex2oct( char  *src, char  *src1);
int  Hex4oct(char  *src,char  *src1,char  *src2,char  *src3);
int mini(void);
int out[LEN][2];
int COUNT;

int main(int argc,char **argv)
{
	int countt=0;
	int countl=0;
	int t=0;
	int reallen=0;

	Data DATA[100];

	int i=0;
	char *p,*q;
	char data[LEN];

	if (scanf("%s", &data))
	{
//		printf("%s\n",data);
	}
	p=data;
	q=data;
	while(*p++)
	{
		if(*p == ',')
		{
			if(countt != 4)
			{
				strncpy(DATA[i].T[countt++],q,p-q);
				q=p+1;
			}
			else if(countl != 2)
			{
				strncpy(DATA[i].L[countl++],q,p-q);
				q=p+1;
				if(strlen(q)<5)
				{ strcpy(DATA[i].L[countl++],q);DATA[i].value="";
                                        reallen+=30;i++; }
			}
			else
			{
				int len = Hex2oct(DATA[i].L[0],DATA[i].L[1]);
				char dat[LEN]="";
				_snprintf(dat,len*5,"%s",q);
                countl=countt=0;
                reallen+=30;
                if(len==0)
                {
                        p=q;
                        DATA[i].value="";
                        i++;
                }
                else
                {
                        q=p=q+len*5;
                        DATA[i].value=dat;
                        reallen+=len*5;
                        i++;
                }

			}
		}
	}
	if(strlen(data) != (reallen-1))
	{
	
		printf("false");
		return -1;
	
	}
	COUNT = i;
	for(t;t<=COUNT;t++)
	{
		out[t][0]=t;
		out[t][1]=Hex4oct(DATA[t].T[0],DATA[t].T[1],DATA[t].T[2],DATA[t].T[3]);
	}

	//找出最小的
	int time = COUNT;
	while(time--)
	{
		int h =mini();
		printf(DATA[h].T[0]),printf(","),printf(DATA[h].T[1]),printf(","),printf(DATA[h].T[2]),printf(","),printf(DATA[h].T[3]),printf(",");
		printf(DATA[h].L[0]),printf(","),printf(DATA[h].L[1]);
		if(Hex2oct(DATA[h].L[0],DATA[h].L[1]))
		{
			printf(",");
			printf(DATA[h].value);
		}
		if(time !=0) printf(",");
	}
	return 0;
}

int  Hex2oct(char  *src,char  *src1)
{
	int n;
	char dest[128];
	char tmp[16] = "";
	src1++;
	src1++;
	strcat(tmp,src);
	strcat(tmp,src1),
	sscanf(tmp, "%x", &n);
	sprintf(dest, "%d", n);
	return atoi(dest);
}
int  Hex4oct(char *src,char  *src1,char  *src2,char  *src3)
{
	int n;
	char dest[128];
	char tmp[128] = "";
	strcat(tmp,src);

	src1++;
	src1++;
	strcat(tmp,src1);
	src2++;
	src2++;
	strcat(tmp,src2);
	src3++;
	src3++;
	strcat(tmp,src3);

	sscanf(tmp, "%x", &n);
	sprintf(dest, "%d", n);
	return atoi(dest);
}
int mini(void)
{
	int t;
	int mini =MAX;
	int n;
	int flag;
	for(t=0;t<COUNT;t++)
	{
		if(out[t][1]<mini)
		{
			n=out[t][0];
			flag=t;
			mini=out[t][1];
		}
	}
	out[flag][1]=MAX;
	return n;
}
