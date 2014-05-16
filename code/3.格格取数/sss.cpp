/**********************************************************************\
*                    指派问题的匈牙利算法                              *
*                                                                      *
*                         张志亮                                       *
*                                                                      *
*                                                                      *
*                                                                      * 
*                                                                      *
*               河海大学商学院信息管理与信息系统专业                   *
*                                                                      *
*                                                                      *
\**********************************************************************/






#include<stdio.h>

typedef struct matrix
{
	float cost[101][101];
    int zeroelem[101][101];
	float costforout[101][101];
	int matrixsize;
	int personnumber;
	int jobnumber;
}matrix;

matrix sb;
int result[501][2];

void twozero(matrix &sb);
void judge(matrix &sb,int result[501][2]);
void refresh(matrix &sb);
void circlezero(matrix &sb);
matrix input();
void output(int result[501][2],matrix sb);
void zeroout(matrix &sb);






matrix input()
{
	matrix sb;
	int m;
	int pnumber,jnumber;
	int i,j;
	float k;
	char w;
	printf("指派问题的匈牙利解法:\n");
	printf("max,请输入1;min,请输入0:\n");
	scanf("%d",&m);
	while (m!=1&&m!=0)
	{	
		printf("请输入1或0:\n");
		scanf("%d",&m);
	}
	printf("请输入人数(人数介于1和100之间):\n");
	scanf("%d",&pnumber);
	while(pnumber<1 || pnumber>100)
	{
		printf("请输入合法数据:\n");
		scanf("%d",&pnumber);
	}
	printf("请输入工件数(介于1和100之间):\n");
	scanf("%d",&jnumber);
	while(jnumber<1 || jnumber>100)
	{
		printf("请输入合法数据:\n");
		scanf("%d",&jnumber);
	}
	printf("请输入%d行%d列的矩阵,同一行内以空格间隔,不同行间以回车分隔:\n",pnumber,jnumber);
	for(i=1;i<=pnumber;i++)
		for(j=1;j<=jnumber;j++)
		{
			scanf("%f",&sb.cost[i][j]);
			sb.costforout[i][j]=sb.cost[i][j];
		}
	scanf("%c",&w);        //吸收回车符
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
	if(m==1)
	{
		k=0;
		for(i=1;i<=sb.matrixsize;i++)
			for(j=1;j<=sb.matrixsize;j++)
				if(sb.cost[i][j]>k)
					k=sb.cost[i][j];
		for(i=1;i<=sb.matrixsize;i++)
			for(j=1;j<=sb.matrixsize;j++)
				sb.cost[i][j]=k-sb.cost[i][j];
	}
	return sb;
}//input


void circlezero(matrix &sb)
{
	int i,j;
	float k;
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
	float k;
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
	float k;
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
	float k;
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


void output(int result[501][2],matrix sb)
{
	int k;
	int i;
	int j;
	int p;
	char w;
	float v;

	v=0;
	for(i=1;i<=sb.matrixsize;i++)
	{
		v=v+sb.costforout[i][result[i][1]];
	}
	printf("最优解的目标函数值为%f.\n",v);

	k=result[0][0];
	if(k>5)
	{
		printf("解的个数超过了限制.\n");
		k=5;
	}
	for(i=1;i<=k;i++)
	{
		printf("按任意键输出第%d种解.\n",i);
		scanf("%c",&w);
		p=(i-1)*sb.matrixsize+1;
		for(j=p;j<p+sb.matrixsize;j++)
			if(result[j][0]<=sb.personnumber&&result[j][1]<=sb.jobnumber)
				printf("第%d个人做第%d件工作.\n",result[j][0],result[j][1]);
		printf("\n");
	}
}//output


void main()
{
	result[0][0]=0;
	sb=input();
	zeroout(sb);
	circlezero(sb);
	output(result,sb);
}//main