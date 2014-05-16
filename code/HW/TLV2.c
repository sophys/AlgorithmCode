#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LEN 1000
#define MAX 65535
 struct data {
	char T[9];
	char L[3];
	char *value;
};
typedef struct data Data;

int  Hex2oct( char  *src);

int mini(void);
int out[LEN][2];
int COUNT;

int main(int argc,char **argv)
{

	int t=0;
	int i=0;

	int len;
	int time;

	Data DATA[100];


	char *p,*q;
	char data[LEN];

	if (scanf("%s", &data))
	{
//		printf("%s\n",data);
	}

	q=data;
	while(*q)
	{

		strncpy(DATA[i].T,q,8);
		q=q+8;
		strncpy(DATA[i].L,q,4);
		q=q+4;

		len = Hex2oct(DATA[i].L);
		if(strlen(q)<len)
		{ 
			printf("msgerr\n");
			return -1;
        }
		DATA[i].value = (char*)malloc(len*2+1);
		_snprintf(DATA[i].value,len*2,"%s",q);
		q=q+len*2;
	}

	COUNT = i;

	for(t=0;t<=COUNT;t++)
	{
		out[t][0]=t;
		out[t][1]=Hex2oct(DATA[t].T);
	}

	//找出最小的
	time = COUNT;
	while(time--)
	{
		int h =mini();
		printf(DATA[h].T[0]);
		printf(DATA[h].L[0]);
		printf(DATA[h].value);
	}
	return 0;
}

int  Hex2oct(char *src)
{
	int n;
	sscanf(src, "%x", &n);
	return n;
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
