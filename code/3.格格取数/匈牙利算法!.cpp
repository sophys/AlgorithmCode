///////////////////////////////////匈牙利算法///////////////////////////////
//2005.written by shenhui
///////////////////////////////////////////////////////////////////////////////////////////
//                                                                                        
// 理论依据是D.Konig的“矩阵中独立零元素的定理”，以及一个性质：若从指派问题的系数矩阵的 
// 某行或某列分别减去一个常数k，得到一个新的矩阵与原矩阵具有相同的最优解。               
// 算法描述：
//         一，变换系数矩阵：各行减去各行的最小元素，各列也一样。对应程序中的ChangeMatrix 
//             （）函数，Minimum（）子函数。
//         二，对一中得到的新矩阵确定独立零元素个数，若为矩阵阶数则以得到结果，退出；否则      
//             作能覆盖所有零元素的最少直线数目的直线集合，并转第三步。对应程序中的
//             Drawcircle（）函数，subroute（）子函数，Modify（）函数。                 
//         三，继续变换系数矩阵，在未被直线覆盖的元素中找到一个最小元素，对未被直线覆盖的
//             元素所在行中各元素都减去这一最小元素，为消除负元素，只要对他们所在列中各元
//             素都加上这一最小元素即可，转第二步。对应程序中的ChangeMatrix1（）函数。
//
////////////////////////////////////////////////////////////////////////////////////////// 
#include "iostream.h"
#define n  8
typedef struct node {
	      int matrix[n+1][n+1];              //系数矩阵
		  int tz[n+1][n+1];              //标记系数阵中0元素的位置
		  int ti[n+1][n+1];              //标记独立0元素的位置
		  int znr[n+1];                //记录每行0元素的个数，这些0没有被画圈和划去
		  int znc[n+1];                //记录每列0元素个数
		  int row[n+1];                //标记某行是否划‘对钩’。row[i]=1
		  int ri[n+1];                 //标记某行是否有独立0元素。ri[i]=1
		  int col[n+1];                //标记某列是否划‘对勾’。col[i]=1
          
}hungry;


hungry   s;                          //定义一个全局变量
int      m=0;                          //定义一个全局变量,记录当前系数阵中的0元素个数。

//-----------------------------求每行每列的最小元素------------------------------------//
int Minimum(int i,int flag)         
{
	int min,j;
	if(flag==1)                              //求行的最小元素时，将i行第一元素赋予min
	    min=s.matrix[i][1];
	if(flag==0)                              //求列的最小元素时，将i列第一元素赋予min
		min=s.matrix[1][i];
	if(flag==1)
	{
		for(j=1;j<=n-1;j++)                  //选择法求最小元素 
		{
			if(min>=s.matrix[i][j+1])
	 			min=s.matrix[i][j+1];
		}
	    for(int a=1;a<=n;a++)
			if(s.matrix[i][a]==min)
				{
					s.tz[i][a]=1;
			        s.znr[i]++;
		            m++;
				}
			
	}
    else if(flag==0)
			{
			 for(j=1;j<=n-1;j++ )
			 {
				 if(min>=s.matrix[j+1][i])
			        min=s.matrix[j+1][i];
			 }
			 for(int a=1;a<=n;a++)
				   if(s.matrix[a][i]==min)
				   {
					   if(s.tz[a][i]!=1)              //避免重复计算0的个数
					   {
						   m++;
						   s.znr[a]++;
					   }
					 s.tz[a][i]=1;
					 s.znc[i]++;
				   }
		}	
				 
	return min;                               //返回一行或一列的最小值到ChangeMatrix()
} 
//----------------------------------------END------------------------------------------//








//----------------------------------变换系数矩阵---------------------------------//
void ChangeMatrix()                         
{
	int i,j,min;
	for(i=1;i<=n;i++)
	{
		min=Minimum(i,1);                 //求第i行的最小元素
        for(j=1;j<=n;j++)
			s.matrix[i][j]-=min;            //该行所有元素都减去这个最小元素
	}
	for(i=1;i<=n;i++)
	{
		min=Minimum(i,0);                 //求第i列的最小元素
		for(j=1;j<=n;j++)
		   s.matrix[j][i]=s.matrix[j][i]-min; //该列所有元素都减去这个最小元素
	} 
}                                           
//---------------------------------------END--------------------------------------//

 





int subroute(int i,int indepz,int flag);
//--------------------------------------画圈----------------------------------//
int DrawCircle()
{
	int indepz=0;                      //标志独立0元素的个数
    int a=1;                           //表示第a行以前的行已经画过圈了
	int flag;
	while(m!=0)                        //当系数阵中还有0元素没有被画圈或划去，循环继续
	{
		flag=0;
		for(int i=1;i<=n;i++)
		{
			if(s.znr[i]==1&&s.ri[i]!=1 )          //判断第i行是否仅有一个0且不是独立零元素
			{
				indepz=subroute(i,indepz,1);      //是，画圈，画零 
		        flag=1;
			}
		}
			if(flag==0)                           //如果在一次循环中所有行0元素数都大于1
			{
				int d,min;
				for(int i=1;i<=n;i++)
					if(s.znr[i]!=0)
					{
						min=s.znr[i];
						d=i;
						break;
					}
				for(int c=a;c<=n;c++)
					if(min>s.znr[c]&&s.znr[c]!=0) {min=s.znr[c]; d=c;}
				indepz=subroute(d,indepz,0);
                 
			}
	}
	return indepz;
}


int subroute(int i,int indepz,int flag)
{
	for(int j=1;j<=n;j++)
	{
		if(s.tz[i][j]==1)                         //找到第i行的第一个0元素。
		{
			s.tz[i][j]=2;                         //2表示此0元素已被画圈，成为独立0
			s.ti[i][j]=1;                         //标记独立0元素的位置 在i行j列
			s.ri[i]=1;                            //表示第i行有独立0 
			s.znr[i]--;
			indepz++;                             //标记独立0的个数  
			m--;
			int a;
			if(i==1)  a=2;                        //划0
			else a=1;
			for(;a<=n;a++)
				if(s.tz[a][j]==1)
				{
					s.tz[a][j]=-1;
					m--;
					s.znr[a]--;
				}
			if(flag==0)                           //当一行有两个0没被画圈和划去时，这一步也要执行
			{
				for(int b=j+1;b<=n;b++)
                   if(s.tz[i][b]==1)
				   {
					   s.tz[i][b]=-1;
				       m--;
					   s.znr[i]--;
				   }
			}
		    break;
		}
	}

	return indepz;
}
//--------------------------------------END----------------------------------//






//----------------------------------画勾----------------------------------//
void Modify()
{
	int flag=0;
	int i,j;
	for(i=1;i<=n;i++)
	{ 
		if(s.ri[i]==1)                             //如果第i行有独立0元素
			continue;
		else if(s.ri[i]==0&&s.row[i]!=1)           //如果第i行没有独立0元素，且该行没有被画对勾
		    s.row[i]=1;                          //第i行画勾
	}
	while(1)
	{
		for(i=1;i<=n;i++)
		{
			flag=0;
			if(s.row[i]==1)
			{
				for(j=1;j<=n;j++)
			      if(s.tz[i][j]==-1&&s.col[j]!=1)    //如果i行j列的元素是被划去的0元素，且第j列没有被画对勾
				  {  
			        s.col[j]=1;
				    flag=1;
				  }
		      for(j=1;j<=n;j++)
				  if(s.col[j]==1)
				  {
					  for(int a=1;a<=n;a++)                  
		          	      if(s.tz[a][j]==2&&s.row[a]!=1)      //如果第j列有独立0且其所在行没有被画对勾
						  {
				    	     s.row[a]=1;
				    	     flag=1;
						  }
				  }
				
		      if(flag==0) return;
			}
			
				
			
		}
	}
}
//----------------------------------------END----------------------------------//	






//------------------------------------画线-------------------------------//
void ChangeMatrix1()
{
	int min=10000;                   //此处填一个比系数矩阵所有元素都大的数即可
	int c[n+1][n+1];                     //记录没有被覆盖的元素中的最小元素的位置
	int i,j,a,b,x,y;                      
	for(x=1;x<=n;x++)
		 for(y=1;y<=n;y++)
			 c[x][y]=0;
	for(i=1;i<=n;i++)	
		for(j=1;j<=n;j++)
		{
			if(s.row[i]!=1)         //如果此行已画过横线，跳至下一行
				break;
			else if(s.col[j]==1)    
				continue;
			else if(min>=s.matrix[i][j])
			{
				min=s.matrix[i][j];
			    c[i][j]=min;
				a=i;b=j;                     //最后一次赋予a,b的值一定是最小元素
			}
		}
     

	for(x=1;x<=n;x++)                        //有多个最小元素时
		for(y=1;y<=n;y++)
		{   
			if(c[x][y]==c[a][b])
		       c[x][y]=min;
		}

    for(i=1;i<=n;i++)
	{
		if(s.row[i]==1)                            //如果第i行划对勾
		{
		 for(j=1;j<=n;j++)                         //则该行每个元素减去min
			s.matrix[i][j]=s.matrix[i][j]-min;
		}
	}

	for(i=1;i<=n;i++)                    //调整标记0数组s.tz[][]，znr[]数组
		for(j=1;j<=n;j++)
		   if(c[i][j]==min)
		   {
			   s.tz[i][j]=1;
			   s.znr[i]++;
			   s.znc[j]++;
		   }

	for(i=1;i<=n;i++)                   
	{
		if(s.row[i]==1)
			break;
		for(j=1;j<=n;j++)
           if(s.col[j]==1)
			   for(a=1;a<=n;a++)
			   {
				   s.matrix[a][j]=s.matrix[a][j]+min;
				   if(s.row[a]!=1&&s.tz[a][j]!=0)
				   {
					   s.znr[a]--;
				       s.tz[a][j]=0;
				   }
			   }
	}
	for(i=1;i<=n;i++)
	{
		s.znr[i]=0;
	    s.znc[i]=0;
		s.ri[i]=0;
		s.col[i]=0;
		s.row[i]=0;
		
	}
	for(i=1;i<=n;i++)
		for(j=1;j<=n;j++)
        {
			s.ti[i][j]=0;
			if(s.tz[i][j]!=0)			
             {
	            s.tz[i][j]=1;
                s.znr[i]++;
				s.znc[j]++;
				m++;
			}
		}
}
//-----------------------------------END------------------------------//






//-------------------------------主函数--------------------------------//
void main()

{
	int indepz;                                       
	int i,j;
	cout<<"现在请输入指派问题的系数矩阵:\n";    //初始化系数矩阵，以及其他数组置0
	for( i=1;i<=n;i++)
		for( j=1;j<=n;j++)
		{
			cin>>s.matrix[i][j];
			s.col[i]=0;
			s.ri[i]=0;
			s.row[i]=0;
			s.ti[i][j]=0;
			s.tz[i][j]=0;
			s.znr[i]=0;
		}
 
	ChangeMatrix();
    

	while(indepz!=n)                        //独立零元素不为n则继续
	{
	 	indepz=DrawCircle();
		if(indepz!=n)
		{
			Modify();
			ChangeMatrix1();
		}
	}
 
	cout<<"输出结果:\n";
	for(i=1;i<=n;i++)                          //输出原系数矩阵
	{
		for(j=1;j<=n;j++)
		    cout<<s.matrix[i][j]<<"    ";
        cout<<"\n";
		}
	for(i=1;i<=n;i++)                            //输出独立零元素在系数矩阵中的位置
	{
		for(j=1;j<=n;j++)
			cout<<s.ti[i][j]<<"     ";
		cout<<"\n";
	}
}
//-----------------------------------实现匈牙利算法----------------------------------// 