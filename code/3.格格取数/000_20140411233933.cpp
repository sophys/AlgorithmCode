
#include<stdio.h>

typedef struct matrix
{
	int cost[101][101];
    int zeroelem[101][101];
	int costforout[101][101];
	int matrixsize;
	int personnumber;
	int jobnumber;
}matrix;

matrix sb;
int result[501][2];
int A[100][100];

void twozero(matrix &sb);
void judge(matrix &sb,int result[501][2]);
void refresh(matrix &sb);
void circlezero(matrix &sb);
matrix input(int A[100][100],int min,int isXmin,int max,int pos,int flag);
matrix inputXY(int A[100][100],int x,int y);
int output(int result[501][2],matrix sb);
void zeroout(matrix &sb);

matrix input(int A[100][100],int min,int isXmin,int max,int pos,int flag)
{
	matrix sb;
	int m=0;
	
	int i,j;
	int k;
	char w;
	int pnumber;
	int jnumber;
	int p=0;

	if(flag == 0)
	{
		if(isXmin == 1)
		{
			for(i=1;i<=min;i++)
			for(j=1;j<=min;j++)
			{
				sb.cost[i][j] = A[i-1][pos*min+i-1];
				sb.costforout[i][j]=sb.cost[i][j];
			}

		}
		else
		{
			for(i=1;i<=min;i++)
			for(j=1;j<=min;j++)
			{
				sb.cost[i][j] = A[pos*min+i-1][i-1];
				sb.costforout[i][j]=sb.cost[i][j];
			}
		}
	
		sb.matrixsize=min;
		sb.personnumber=min;
		sb.jobnumber=min;

	}
	else
	{
		
		if(isXmin == 1)
		{
			pnumber = min;
			jnumber = max - p*min;;

			for(i=1;i<=pnumber;i++)
			for(j=1;j<=jnumber;j++)
			{
				sb.cost[i][j] = A[i-1][pos*min+j-1];
				sb.costforout[i][j]=sb.cost[i][j];
			}
		}
		else
		{
			pnumber = max - p*min;
			jnumber = min;

			for(i=1;i<=pnumber;i++)
			for(j=1;j<=jnumber;j++)
			{
				sb.cost[i][j] = A[pos*min+i-1][j-1];
				sb.costforout[i][j]=sb.cost[i][j];
			}
		}
		if(jnumber>pnumber)
			for(i=pnumber+1;i<=jnumber;i++)
				for(j=1;j<=jnumber;j++)
				{
					sb.cost[i][j]=0;
					sb.costforout[i][j]=0;
				}
		else
		{
			if(pnumber>jnumber)
				for(i=1;i<=pnumber;i++)
					for(j=jnumber+1;j<=pnumber;j++)
					{
						sb.cost[i][j]=0;
						sb.costforout[i][j]=0;
					}

		}
		sb.matrixsize=pnumber;
		if(pnumber<jnumber)
			sb.matrixsize=jnumber;
		sb.personnumber=pnumber;
		sb.jobnumber=jnumber;

	}
	return sb;
}//input

void circlezero(matrix &sb)
{
	int i,j;
	int k;
	int p;
	for(i=0;i<=sb.matrixsize;i++)
		sb.cost[i][0]=0;
	for(j=1;j<=sb.matrixsize;j++)
		sb.cost[0][j]=0;
	for(i=1;i<=sb.matrixsize;i++)
		for(j=1;j<=sb.matrixsize;j++)
			if(sb.cost[i][j]==0)
			{
				sb.cost[i][0]++;
				sb.cost[0][j]++;
				sb.cost[0][0]++;
			}
	for(i=0;i<=sb.matrixsize;i++)
		for(j=0;j<=sb.matrixsize;j++)
			sb.zeroelem[i][j]=0;
	  
	k=sb.cost[0][0]+1;
	while(sb.cost[0][0]<k)
	{
		k=sb.cost[0][0];
		for(i=1;i<=sb.matrixsize;i++)
		{
			if(sb.cost[i][0]==1)
			{
				for(j=1;j<=sb.matrixsize;j++)
					if(sb.cost[i][j]==0&&sb.zeroelem[i][j]==0)
						break;
				sb.zeroelem[i][j]=1;
				sb.cost[i][0]--;
				sb.cost[0][j]--;
				sb.cost[0][0]--;
				if(sb.cost[0][j]>0)
					for(p=1;p<=sb.matrixsize;p++)
						if(sb.cost[p][j]==0&&sb.zeroelem[p][j]==0)
						{
							sb.zeroelem[p][j]=2;
							sb.cost[p][0]--;
							sb.cost[0][j]--;
							sb.cost[0][0]--;
						}
			}
		}
		for(j=1;j<=sb.matrixsize;j++)
		{
			if(sb.cost[0][j]==1)
			{
				for(i=1;i<=sb.matrixsize;i++)
					if(sb.cost[i][j]==0&&sb.zeroelem[i][j]==0)
						break;
				sb.zeroelem[i][j]=1;
				sb.cost[i][0]--;
				sb.cost[0][j]--;
				sb.cost[0][0]--;
				if(sb.cost[i][0]>0)
					for(p=1;p<=sb.matrixsize;p++)
						if(sb.cost[i][p]==0&&sb.zeroelem[i][p]==0)
						{
							sb.zeroelem[i][p]=2;
							sb.cost[i][0]--;
							sb.cost[0][p]--;
							sb.cost[0][0]--;
						}
			}
		}
	}//while
	if(sb.cost[0][0]>0)
		twozero(sb);
	else
		judge(sb,result);
}//circlezero


void twozero(matrix &sb)
{
	int i,j;
	int p,q;
	int m,n;
	int k;
    matrix st;
	for(i=1;i<=sb.matrixsize;i++)
		if(sb.cost[i][0]>0)
			break;
	if(i<=sb.matrixsize)
	{
		for(j=1;j<=sb.matrixsize;j++)
		{
			st=sb;//pay attention
			if(sb.cost[i][j]==0&&sb.zeroelem[i][j]==0)
			{
				sb.zeroelem[i][j]=1;
				sb.cost[i][0]--;
				sb.cost[0][j]--;
				sb.cost[0][0]--;
				for(q=1;q<=sb.matrixsize;q++)
					if(sb.cost[i][q]==0&&sb.zeroelem[i][q]==0)
					{
						sb.zeroelem[i][q]=2;
						sb.cost[i][0]--;
						sb.cost[0][q]--;
						sb.cost[0][0]--;
					}
				for(p=1;p<=sb.matrixsize;p++)
					if(sb.cost[p][j]==0&&sb.zeroelem[p][j]==0)
					{
						sb.zeroelem[p][j]=2;
						sb.cost[p][0]--;
						sb.cost[0][j]--;
						sb.cost[0][0]--;
					}

				k=sb.cost[0][0]+1;
				while(sb.cost[0][0]<k)
				{
					k=sb.cost[0][0];
					for(p=i+1;p<=sb.matrixsize;p++)
					{
						if(sb.cost[p][0]==1)
						{
							for(q=1;q<=sb.matrixsize;q++)
								if(sb.cost[p][q]==0&&sb.zeroelem[p][q]==0)
									break;
							sb.zeroelem[p][q]=1;
							sb.cost[p][0]--;
							sb.cost[0][q]--;
							sb.cost[0][0]--;
							for(m=1;m<=sb.matrixsize;m++)
								if(sb.cost[m][q]=0&&sb.zeroelem[m][q]==0)
								{
									sb.zeroelem[m][q]=2;
									sb.cost[m][0]--;
									sb.cost[0][q]--;
									sb.cost[0][0]--;
								}
						}//if
					}//for
					for(q=1;q<=sb.matrixsize;q++)
					{
						if(sb.cost[0][q]==1)
						{
							for(p=1;p<=sb.matrixsize;p++)
								if(sb.cost[p][q]==0&&sb.zeroelem[p][q]==0)
									break;
							sb.zeroelem[p][q]=1;
							sb.cost[p][q]--;
							sb.cost[0][q]--;
							sb.cost[0][0]--;
							for(n=1;n<=sb.matrixsize;n++)
								if(sb.cost[p][n]==0&&sb.zeroelem[p][n]==0)
								{
									sb.zeroelem[p][n]=2;
									sb.cost[p][0]--;
									sb.cost[0][n]--;
									sb.cost[0][0]--;
								}
						}//if
					}//for
				}//while
				if(sb.cost[0][0]>0)
					twozero(sb);
				else 
					judge(sb,result);
			}//if->p5	          
			sb=st;
		}//for->p5
	}//if->p5
}//twozero


void judge(matrix &sb,int result[501][2])
{
	int i,j;
	int m;
	int n;
	int k;
	m=0;
	for(i=1;i<=sb.matrixsize;i++)
		for(j=1;j<=sb.matrixsize;j++)
			if(sb.zeroelem[i][j]==1)
				m++;
	if(m==sb.matrixsize)
	{
		k=1;
		for(n=1;n<=result[0][0];n++)
		{
			for(i=1;i<=sb.matrixsize;i++)
			{
				for(j=1;j<=sb.matrixsize;j++)
					if(sb.zeroelem[i][j]==1)
						break;
				if(i<=sb.personnumber&&j<=sb.jobnumber)
					if(j!=result[k][1])
						break;
				k++;
			}
			if(i==sb.matrixsize+1)
				break;
			else
				k=n*sb.matrixsize+1;
		}
		if(n>result[0][0])
		{
			k=result[0][0]*sb.matrixsize+1;
			for(i=1;i<=sb.matrixsize;i++)
				for(j=1;j<=sb.matrixsize;j++)
					if(sb.zeroelem[i][j]==1)
					{
						result[k][0]=i;
						result[k++][1]=j;
					}
			result[0][0]++;
		}
	}
	else
	{
		refresh(sb);
	}
}//judge


void refresh(matrix &sb)
{
	int i,j;
	int k;
	int p;
	k=0;

	for(i=1;i<=sb.matrixsize;i++)
	{
		for(j=1;j<=sb.matrixsize;j++)
			if(sb.zeroelem[i][j]==1)
			{
				sb.zeroelem[i][0]=1;         //有独立零元素
				break;
			}
	}
	while(k==0)
	{
		k=1;
		for(i=1;i<=sb.matrixsize;i++)
			if(sb.zeroelem[i][0]==0)
			{
				sb.zeroelem[i][0]=2;
				for(j=1;j<=sb.matrixsize;j++)
					if(sb.zeroelem[i][j]==2)
					{
						sb.zeroelem[0][j]=1;
					}
			}
		for(j=1;j<=sb.matrixsize;j++)
		{
			if(sb.zeroelem[0][j]==1)
			{
				sb.zeroelem[0][j]=2;
				for(i=1;i<=sb.matrixsize;i++)
					if(sb.zeroelem[i][j]==1)
					{
						sb.zeroelem[i][0]=0;
						k=0;
					}
			}
		}
	}                    //为2的行或者列是打"√"的
	p=0;
	k=0;
	for(i=1;i<=sb.matrixsize;i++)
	{
		if(sb.zeroelem[i][0]==2)
		{
			for(j=1;j<=sb.matrixsize;j++)
			{
				if(sb.zeroelem[0][j]!=2)
					if(p==0)
					{
						k=sb.cost[i][j];
						p=1;
					}
					else
					{
						if(sb.cost[i][j]<k)
							k=sb.cost[i][j];
					}
			}
		}
	}
	for(i=1;i<=sb.matrixsize;i++)
	{
		if(sb.zeroelem[i][0]==2)
			for(j=1;j<=sb.matrixsize;j++)
				sb.cost[i][j]=sb.cost[i][j]-k;
	}

	for(j=1;j<=sb.matrixsize;j++)
	{
		if(sb.zeroelem[0][j]==2)
			for(i=1;i<=sb.matrixsize;i++)
				sb.cost[i][j]=sb.cost[i][j]+k;
	}                                 //化简矩阵
	for(i=0;i<=sb.matrixsize;i++)
		for(j=0;j<=sb.matrixsize;j++)
			sb.zeroelem[i][j]=0;              //清0
	circlezero(sb);	
}//refresh


void zeroout(matrix &sb)
{
	int i,j; 
	int k;
	for(i=1;i<=sb.matrixsize;i++)
	{
		k=sb.cost[i][1];
		for(j=2;j<=sb.matrixsize;j++)
			if(sb.cost[i][j]<k)
				k=sb.cost[i][j];
		for(j=1;j<=sb.matrixsize;j++)
			sb.cost[i][j]=sb.cost[i][j]-k;
	}
	for(j=1;j<=sb.matrixsize;j++)
	{
		k=sb.cost[1][j];
		for(i=2;i<=sb.matrixsize;i++)
			if(sb.cost[i][j]<k)
				k=sb.cost[i][j];
		for(i=1;i<=sb.matrixsize;i++)
			sb.cost[i][j]=sb.cost[i][j]-k;
	}
}//zeroout


int output(int result[501][2],matrix sb)
{
	int k;
	int i;
	int j;
	int p;
	char w;
	int v;

	v=0;
	for(i=1;i<=sb.matrixsize;i++)
	{
		v=v+sb.costforout[i][result[i][1]];
	}
	return v;

}//output

matrix inputXY(int A[100][100],int x,int y)
{
	matrix sb;
	int pnumber = x;
	int jnumber = y;
	int i,j;
	float k;
	char w;

	for(i=1;i<=pnumber;i++)
		for(j=1;j<=jnumber;j++)
		{
			sb.cost[i][j] = A[i-1][j-1];
			sb.costforout[i][j]=sb.cost[i][j];
		}

	if(jnumber>pnumber)
		for(i=pnumber+1;i<=jnumber;i++)
			for(j=1;j<=jnumber;j++)
			{
				sb.cost[i][j]=0;
				sb.costforout[i][j]=0;
			}
	else
	{
		if(pnumber>jnumber)
			for(i=1;i<=pnumber;i++)
				for(j=jnumber+1;j<=pnumber;j++)
				{
					sb.cost[i][j]=0;
					sb.costforout[i][j]=0;
				}

	}
	sb.matrixsize=pnumber;
	if(pnumber<jnumber)
		sb.matrixsize=jnumber;
	sb.personnumber=pnumber;
	sb.jobnumber=jnumber;
	return sb;
}//input

int  main()
{
	int outResult[100];
	int k;
	int l;
	 
	int X,Y;
	
	int i,j;
	
	scanf("%d",&k);
	
	for(l=0;l<k;l++)
	{
		int D=0;
		scanf("%d",&X);//hang
		scanf("%d",&Y);//lie
		for(i=0;i<X;i++)
		for(j=0;j<Y;j++)
		{			
			scanf("%d",&A[i][j]);
		}
		int min= X>Y?Y:X;
		int max= X>Y?X:Y;
		int flag;
		if(X == Y)//行列相等
		{
			sb=inputXY(A,X,Y);
			zeroout(sb);
			circlezero(sb);
			outResult[l] = output(result,sb);
			continue;
		}
		else	
		{
			if(max%min)
			{
				flag = max/min +1;
			}
			else
			{
				flag = max/min;
			}
		
		}
			
		for(int p=0;p<flag;p++)
		{
			if(X == min)
			{
				result[0][0]=0;
				if(p == flag-1)
				{
					sb=input(A,X,1,Y,p,1);
				}
				{
					sb=input(A,X,1,Y,p,0);
				}				
				zeroout(sb);
				circlezero(sb);
				D += output(result,sb);			
			}
			else
			{
				result[0][0]=0;
				if(p == flag-1)
				{
					sb=input(A,X,0,Y,p,1);
				}
				{
					sb=input(A,X,0,Y,p,0);
				}
				zeroout(sb);
				circlezero(sb);
				D += output(result,sb);
			}	
		}
		outResult[l] = D;
	}
	for(l=0;l<k;l++)
		printf("Case %d: %d\n",l+1,(int)outResult[l]);
	return 0;
}//main