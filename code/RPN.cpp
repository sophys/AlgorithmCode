#include <stack>
#include<string.h>
using namespace std;
int main()
{
	int N;// 表示数据的组数；
	char ch[1003]; // 用来存取表达式；
	char resultStr[1003]; //  用来存放输出结果的字符串；
	cin>>N;
	while(N--)
	{
		  stack<char> chStack; // 用来存放字符的栈；
		  int f=0;// 用来标识当前 ResultStr 中的实际字符数；
		  memset(ch,'\0',sizeof(ch));
		  memset(resultStr,'\0',sizeof(resultStr)); //  由于是多组数据的测试，所以每次用之前必须要对字符数组做 置空 处理，否则会出现意想不到的错误；
		  cin>>ch;
		  for(int i=0;i<(int)strlen(ch);i++)
		  {
			   if(ch[i]>='0'&&ch[i]<='9')  // 如果是数据，则用字符数组存起来；
			   {
				    resultStr[f]=ch[i];f++;
			   }
			   else
			   {
				   // 可以把小面的分情况讨论写成一个用于判断的函数，那样的话可能会清晰一点，很简单，在这里就不写啦；
				   if(chStack.empty()) {chStack.push(ch[i]);continue;}
				   
                   if((chStack.top()=='*'||chStack.top()=='/')&&(ch[i]=='*'||ch[i]=='/')) 
				   { resultStr[f]=chStack.top();f++;chStack.pop(); i--;continue; }
				   
				   if((chStack.top()=='+'||chStack.top()=='-')&&(ch[i]=='+'||ch[i]=='-')) 
				   { resultStr[f]=chStack.top();f++;chStack.pop(); i--;continue; }
				   
				   if((chStack.top()=='*'||chStack.top()=='/')&&(ch[i]=='+'||ch[i]=='-'))
				   { resultStr[f]=chStack.top();f++;chStack.pop(); i--;continue; }
				   
				   if((chStack.top()=='+'||chStack.top()=='-')&&(ch[i]=='*'||ch[i]=='/')) 
				   { chStack.push(ch[i]); continue; }
				   
				   if(chStack.top()=='('&&ch[i]!=')')  { chStack.push(ch[i]); continue; }
				   
				   if(chStack.top()=='('&&ch[i]==')') {chStack.pop();continue;}
				   
				   if(chStack.top()!='('&&ch[i]==')') {resultStr[f]=chStack.top();f++;chStack.pop(); i--;continue;}
				   
				   if(ch[i]=='(') { chStack.push(ch[i]); continue; }
			   }
		  }

		cout<<resultStr;
		while(!chStack.empty()) //  如果栈中还有元素，那么就一次弹出所有的元素；
		{
			cout<<chStack.top();
			chStack.pop();
		}
		cout<<endl;
	}
	return 0;
}