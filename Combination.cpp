#include<iostream>
#include<vector>
#include<cstring>
using namespace std;
#include<assert.h>

//Arrangement
//Combination
//////////--------------------递归-------------
void Combination(char *string ,int number,vector<char> &result);

void Combination(char *string)
{
	assert(string != NULL);
	vector<char> result;
	int i , length = strlen(string);
//	for(i = 1 ; i <= length ; ++i)
		Combination(string , 3 ,result);
}

void Combination(char *string ,int number , vector<char> &result)
{
	assert(string != NULL);
	if(number == 0)
	{
		static int num = 1;
		printf("第%d个组合\t",num++);

		vector<char>::iterator iter = result.begin();
		for( ; iter != result.end() ; ++iter)
			printf("%c",*iter);
		printf("\n");
		return ;
	}
	if(*string == '\0')
		return ;
	result.push_back(*string);
	Combination(string + 1 , number - 1 , result);
	result.pop_back();
	Combination(string + 1 , number , result);
}
//////////----------------非递归-----------------
void print_subset(char* str,int n , int s)
{
	int i;
	int flag=0;
	char tmp[4]="";

	for(i = 0 ; i < n ; ++i)
		if(s&(1<<i))// 判断s的二进制中哪些位为1，即代表取某一位
		{
			tmp[flag++]=str[i];
		}
	if(flag == 3)
		printf("%d:%s\n",s,tmp);
}

void subset(char * str,int n)
{
	int i;
	for(i= 0 ; i < (1<<n) ; ++i)
	{
		print_subset(str,n,i);
	}
}

int main(void)
{
	char str[] = "abcdef";
	int n = strlen(str);
	Combination(str);
	subset(str,n);
	return 0;
}
