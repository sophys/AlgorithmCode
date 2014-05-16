#include<stdio.h>
#include<stdlib.h>

#define X 100
#define Y 100
#define MAX 65535
int A[X][Y]; 
int inx[X];
int iny[Y];
int B[10000][3];

int notin(int a,int A[X],int count)
{
	int i;
	for(i=0;i<count;i++)
		if(A[i]==a)
			return 0;
	return 1;
}

int find(int A[100][100],int a,int b)
{
	int i=0,j,s,t;
	int D=0;
	int n = a>b?b:a;

//	printf("geshu wei : %d",n);
	int flag=1;
	int N = a*b;

	int temp[3];

	int o=1,p=1;

	for(s=0;s<a;s++)
	{
		for(t=0;t<b;t++)
	    {
	    	B[i][0] = s;
	    	B[i][1] = t;
	    	B[i][2] = A[s][t];
	    	i++;
	    }
	}	
	for(i=1;i<N;i++)
	{
		if(B[i][2]<B[i-1][2])
		{
			temp[0]=B[i][0];
			temp[1]=B[i][1];
			temp[2]=B[i][2];

			j=0;
			for(j=i-1;j>=0&&temp[2]<B[j][2];--j)
			{
		    	B[j+1][0]=B[j][0];
		    	B[j+1][1]=B[j][1];
		    	B[j+1][2]=B[j][2];
			}
			B[j+1][0]=temp[0];
			B[j+1][1]=temp[1];
			B[j+1][2]=temp[2];
		} 
	}
//	for(i=0;i<N;i++)
//		printf("%d %d %d \n", B[i][0], B[i][1], B[i][2]);

	D+=B[0][2];
	inx[0]=B[0][0];
	iny[0]=B[0][1];
	flag=1;
	for(i=1;i<N;i++)
	{
		if(notin(B[i][0],inx,flag)&&notin(B[i][1],iny,flag))
		{
			D+=B[i][2];
			flag+=1;
			inx[o++]=B[i][0];
			iny[p++]=B[i][1];
//			printf("in %d\n",B[i][2]);
		}
		if(flag == n)
			break;

	}
    return D;
}
int main(int argc,char** argv)
{
	int i;
	int j;
	int n;
	int result[X];
	scanf("%d",&n);
	for(i=0;i<n;i++)
	{
		int a,b;
		int s,t;
		scanf("%d%d",&a,&b);

		for(s=0;s<a;s++)
			for(t=0;t<b;t++)
			scanf("%d",&A[s][t]);
		result[i] = find(A,a,b);
	}
	for(i=0;i<n;i++)
		printf("Case %d: %d\n",i+1,result[i]);
	return 0;		
}


