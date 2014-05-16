#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define X 128
int POWER(int n)
{
        int i;
        int N=1;
        for(i=1;i<=n;i++) N=N*i;
                return N;
}


int find(int a,int b,int c)
{
        return a+b+c;
}
int main(int argc,char** argv)
{
        int i;
        int j;
        int N;
        int result[X];
        scanf("%d",&N);
        for(i=0;i<N;i++)
        {
                int a,b,c;
                int s,t;
                int count;
                scanf("%d%d%d",&a,&b,&c);
                count = POWER(a+b)/(POWER(a)*POWER(b));
                if(c > count)
                {
                      //  printf("Impossible\n");
                        result[i] = -1;
                        continue;   
                }        
                result[i] = find(a,b,c);
        }
        for(i=0;i<N;i++)
        {
              if(result[i] != -1)
              {
                printf("%d\n",result[i]);
                }
                else
                        printf("Impossible\n");
        }
                
        return 0;               
}
