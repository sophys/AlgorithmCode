#include<stdio.h>

int main()
{
    int m=0,l;
	int t=0;
    int M, S, T;
    int s;

	int k;
	int i;
	int s1=0,s2=0;
    scanf("%d%d%d",&M,&S,&T);
	if(S==0)
	{
		printf("Yes 0\n");
		return 0;
	}
	if(T==0 && S!=0)
	{
		printf("No 0\n");
		return 0;
	}

	for(i=0;i<T;i++)
	{
		if(M>=10)
		{
			s1+=60;
			M-=10;
		}
		else 
		{
			M += 4;
		}
		s2+=17;
		if(S-s1<=17 && i+1 <=T)
		{
			printf("Yes %d\n", i+2);
			return 0;
		}
		if(s1>=S || s2>=S)
		{
			printf("Yes %d\n", i+1);
			return 0;
		}
	}
	printf("No %d\n",s1>s2?s1:s2);
    return 0;
}

