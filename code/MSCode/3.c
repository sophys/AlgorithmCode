#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define Y 128

int A[2][Y];
char B[2][Y];

int main(int argc,char** argv)
{
        int i=0;
        int k;
        int l;
        for (k = 0; k < 2; ++k)
        {
            scanf("%s",B[k]);

        }
        for(i=0;i<2;i++)
        {

            printf("%s ",B[i]);
            printf("\n");
        }
        for(i=0;i<2;i++)
        {
            l=strlen(B[i]);
            int j=0;
            for(k=0;k<l;k++)
            {
                A[i][j] = B[i][k]-48;
                j++;
                k++;
            }

        }
        for(j=0;j<2;j++)
		{
			for(i=0;i<Y;i++)
            printf("%d",A[i][j]);
			
			printf("\n");
		}
        return 0;               
}
