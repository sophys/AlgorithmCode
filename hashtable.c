#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#define SIZE 101

typedef struct node
{
	char *key;
	char *value;
	struct node *next;
}node;

static node *hashtab[SIZE];

void init()
{
	int i=0;
	for(;i<SIZE;i++)
		hashtab[i]=NULL;
}

unsigned int hash(char *s)
{
	unsigned int h=0;
	for(;*s;s++)
		h=*s+h*31;
	return h%SIZE;
}

node* find(char *key)
{
	unsigned int hi=hash(key);
	node* np=hashtab[hi];
	for(;np!=NULL;np=np->next)
	{
		if(!strcmp(np->key,key))
			return np;
	}
	return NULL;
}

static char* Strdup(char *str)
{
	int l=strlen(str)+1;
	char *ns=(char*)malloc(l*sizeof(char));
	strcpy(ns,str);
	if(ns==NULL)
		return NULL;
	else
		return ns;
}

char* getvalue(char *key)
{
	node* n=find(key);
	if(n==NULL)
		return NULL;
	else
		return n->value;
}

char insert(char* key,char* value)
{
	unsigned int hi;
	node *np;

	if((np=find(key)) == NULL)
	{
		hi=hash(key);
		np=(node*)malloc(sizeof(node));
		if(np==NULL)
			return 0;
		np->key=Strdup(key);
		np->next=hashtab[hi];//不带头节点的头插法
		hashtab[hi]=np;
	}
	np->value=Strdup(value);
	return 1;
}

void show()
{
	int i;
	node *t;
	for(i=0;i<SIZE;i++)
	{
		if(hashtab[i]==NULL)
			printf("()\n");
		else
		{
			t=hashtab[i];
			printf("(");
			for(;t!=NULL;t=t->next)
				printf("(%s->%s)",t->key,t->value);
			printf(")\n");
		}
	}
}
void destory()
{
	int i;
	node *np,*t;

	for(i=0;i<SIZE;i++)
	{
		if(hashtab[i]!=NULL)
		{
			np=hashtab[i];
			while(np!=NULL)
			{
				t=np->next;
				free(np->key);
				free(np->value);
				free(np);
				np=t;
			}
		}
	}
}



int main()
{
	int i;
	char* key[]={"name"};
	char* value[]={"Ys"};

	insert(key[0],value[0]);

	show();
	destory();
	return 0;
}
