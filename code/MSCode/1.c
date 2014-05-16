#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define N 4
char A[N][1024];
int T[36];
char SET[]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
static int deal(char *A)
{
    int l = strlen(A);
    int i,j;
    int MAX=0;
    memset(T,0,36);
    for(i=0;i<l;i++)
    {
        switch(A[i])
        {
           case 'z':
              T[35]++;
              break;
           case 'y':
              T[34]++;
              break;
           case 'x':
              T[33]++;
              break;
           case 'w':
              T[32]++;
              break;   
           case 'v':
              T[31]++;
              break;
           case 'u':
              T[30]++;
              break;
           case 't':
              T[29]++;
              break;
           case 's':
              T[28]++;
              break;                   
           case 'r':
              T[27]++;
              break;
           case 'q':
              T[26]++;
              break;
           case 'p':
              T[25]++;
              break;
           case 'o':
              T[24]++;
              break;   
           case 'n':
              T[23]++;
              break;
           case 'm':
              T[22]++;
              break;
           case 'l':
              T[21]++;
              break;
           case 'k':
              T[20]++;
              break; 
           case 'j':
              T[19]++;
              break;
           case 'i':
              T[18]++;
              break;
           case 'h':
              T[17]++;
              break;
           case 'g':
              T[16]++;
              break;   
           case 'f':
              T[15]++;
              break;
           case 'e':
              T[14]++;
              break;
           case 'd':
              T[13]++;
              break;
           case 'c':
             T[12]++;
              break;                   
           case 'b':
              T[11]++;
              break;
           case 'a':
              T[10]++;
              break;
           case '9':
              T[9]++;
              break;
           case '8':
              T[8]++;
              break;   
           case '7':
              T[7]++;
              break;
           case '6':
              T[6]++;
              break;
           case '5':
              T[5]++;
              break;
           case '4':
              T[4]++;
              break; 
           case '3':
              T[3]++;
              break;
           case '2':
              T[2]++;
              break;
           case '1':
              T[1]++;
              break;
           case '0':
              T[0]++;
              break;   
           default:
                return -1;
              break;
        }

    }
    for(i=0;i<36;i++)
    {
            if(T[i] > MAX)
                    MAX=T[i];
    }
    for(i =0;i<MAX;i++)
    {
        for(j=0;j<36;j++)
        {
                 if(T[j] > 0)
                 {
                        printf("%c",SET[j]);
                        T[j]--; 
                 }
                       
        }       

    }
    printf("\n");
    return 0;
}
int main()
{
    int i;
    for(i=0;i<N;i++)
    {
            scanf("%s",A[i]);
    }
    
    for(i=0;i<N;i++)
    {
           if(deal(A[i]) != 0)
           {
                   printf("<invalid input string>\n");
           }
            // printf("%s\n",A[i]);
    }
    return 0;
}   