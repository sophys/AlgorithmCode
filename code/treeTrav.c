#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/* 树数据结构*/
typedef struct node
{
	char data;
	struct node *lchild,*rchild;
}Tnode,Tree;

/* 链栈结构 */
typedef node SElemType;
typedef struct StackNode
{
    SElemType *data;
    struct StackNode *next;
}StackNode;

typedef struct stack
{
    StackNode *top;
    int count;
}Stack;

void initStack(Stack *S)
{
	S->top=NULL;
	S->count=0;
}

void push(Stack *S,SElemType *d)
{
	StackNode *t = (StackNode*)malloc(sizeof(StackNode));

    t->data=d;
    t->next=S->top;	/* 把当前的栈顶元素赋值给新结点的直接后继，见图中① */
    S->top=t;         /* 将新的结点s赋值给栈顶指针，见图中② */
    S->count++;
}
Tnode* pop(Stack* S)
{
    StackNode *p;
    if(empty(*S))
        return -1;
		
    Tnode *e=S->top->data;
    p=S->top;					/* 将栈顶结点赋值给p，见图中③ */
    S->top=S->top->next;    /* 使得栈顶指针下移一位，指向后一结点，见图中④ */
    free(p);                    /* 释放结点p */
    S->count--;
    return e;	
}
Tnode* top(Stack *S)
{
	return S->top->data;
}

int empty(Stack *S)
{
	if(S->count)
		return 0;
	else
		return	1;
}

/*main*/
void inTrav(Tree* T)
{
	Stack *S = (Stack*)malloc(sizeof(Stack));
	initStack(S);
	Tnode* p;
	
	while(p||!empty(S))
	{
		if(p)
		{
			push(S,p);
			p=p->lchild;
		}
		else
		{
			p=pop(S);
			printf("",p->data);
			p=p->rchild;
		}
	}
}

Tree* initTree(void)
{
	Tnode *A = (Tnode*)malloc(sizeof(Tnode));
	Tnode *B  (Tnode*)malloc(sizeof(Tnode));
	Tnode *B  (Tnode*)malloc(sizeof(Tnode));
	Tnode *C  (Tnode*)malloc(sizeof(Tnode));
	A->data = 'A';
	B->data = 'B';
	C->data = 'C';
	D->data = 'D';
	
	A->lchild = B;
	A->rchild = C;
	B->lchild = NULL;
	B->rchild = NULL;
	C->lchild = D;
	C->rchild = NULL;
	D->rchild = NULL;
	D->lchild = NULL;
	return A;
}
int main()
{
	Tree* T=initTree();
	inTrav(T);
	return 0;
}