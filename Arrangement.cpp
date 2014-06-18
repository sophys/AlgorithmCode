#include<iostream>
using namespace std;
#include<assert.h>
////////////-------------------递归-------------------
//在[nBegin,nEnd)区间中是否有字符与下标为pEnd的字符相等
bool IsSwap(char* pBegin , char* pEnd)
{
	char *p;
	for(p = pBegin ; p < pEnd ; p++)
	{
		if(*p == *pEnd)
			return false;
	}
	return true;
}
void Permutation(char* pStr , char *pBegin)
{
	assert(pStr);

	if(*pBegin == '\0')
	{
		static int num = 1;  //局部静态变量，用来统计全排列的个数
		printf("第%d个排列\t%s\n",num++,pStr);
	}
	else
	{
		for(char *pCh = pBegin; *pCh != '\0'; pCh++)   //第pBegin个数分别与它后面的数字交换就能得到新的排列   
		{
			if(IsSwap(pBegin , pCh))
			{
				swap(*pBegin , *pCh);
				Permutation(pStr , pBegin + 1);
				swap(*pBegin , *pCh);
			}
		}
	}
}
//////////////////-----------------------非递归----------------------
#include<iostream>
#include<algorithm>
#include<cstring>
using namespace std;
#include<assert.h>

//反转区间
void Reverse(char* pBegin , char* pEnd)
{
	while(pBegin < pEnd)
		swap(*pBegin++ , *pEnd--);
}
//下一个排列
bool Next_permutation(char a[])
{
	assert(a);
	char *p , *q , *pFind;
	char *pEnd = a + strlen(a) - 1;
	if(a == pEnd)
		return false;
	p = pEnd;
	while(p != a)
	{
		q = p;
		p--;
		if(*p < *q)  //找降序的相邻2数,前一个数即替换数  
		{
			 //从后向前找比替换点大的第一个数
			pFind = pEnd;
			while(*pFind < *p)
				--pFind;
			swap(*p , *pFind);
			//替换点后的数全部反转
			Reverse(q , pEnd);
			return true;
		}
	}
	Reverse(a , pEnd);   //如果没有下一个排列,全部反转后返回false   
	return false;
}

int cmp(const void *a,const void *b)
{
	return int(*(char *)a - *(char *)b);
}

int main(void)
{
	char str[] = "abcd";
	Permutation(str , str);
	
	int num = 1;
	qsort(str , strlen(str),sizeof(char),cmp);
	do
	{
		printf("第%d个排列\t%s\n",num++,str); 
	}while(Next_permutation(str));
	
	return 0;
}
