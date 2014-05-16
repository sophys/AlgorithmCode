#include <stdio.h>
void reverse(char *);
void reverseword(char *,char *);
int main()
{
	char s[]="This\n is a sentence";
	reverse(s);
	printf(s);
	return 0;
}
void reverseword(char *p,char *q)
{
	while(p<q)
	{
		char t = *p;
		*p++=*q;
		*q-- = t;
	}
}
void reverse(char *s)
{
	char *p=s,*q=s;
	while(*q != '\0')
	{
		if(*q == ' ')
		{
			reverseword(p,q-1);
			q++; p=q;
		}
		else
			q++;
	}
	reverseword(p,q-1);
	reverseword(s,q-1);
}
