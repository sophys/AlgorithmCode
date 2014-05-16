#include <stdio.h>
#include <stdlib.h>

typedef struct node{
	int a;
	struct node * next;
}node;

static int insert(node *n,int i,int data)
{
	node *p=n->next;
	node *t;
	int j=1;
	while(p&&j<i)
	{
		j++;
		p=p->next;
	}
	if(!p||j>i)
		return -1;
	t = (node*)malloc(sizeof(node));
	t->a = data;
	t->next = p->next;
	p->next = t;
	return 0;
}
static int add(node *l,int data)
{
	node *p = (node*)malloc(sizeof(node));
	p->a = data;
	p->next = l->next;
	l->next = p;
	return 0;
}
static int delete(node *l,int i)
{
	node *p = l;
	node *t;
	int j=1;
	while(p&&j<i)
	{
		j++;
		p = p->next;
	}
	if(!p||j>i)
		return -1;
	t = p->next;
	p->next = t->next;
	free(t);
	return 0;
}
static void destory(node *l)
{
	node *p,*q=l->next;
	while(q)
	{
		p = q->next;
		free(q);
		q=p;
	}
	l->next=NULL;
}

static void show(node *p)
{
	node *l=p->next;
	while(l)
	{
		printf("%d->",l->a);
		l=l->next;
	}
	printf("\n");
}

int main()
{
	node *l = (node*)malloc(sizeof(node));
	l->next = NULL;
	add(l,1);
	add(l,2);
	add(l,3);
	add(l,4);
	show(l);
	if(insert(l,4,5)!=0)
	{
		printf("insert error\n");
	}
	if(delete(l,1)!=0)
	{
		printf("delete error!\n");
	}

	show(l);
	destory(l);
	show(l);
	add(l,6);
	show(l);
	return 0;
}

