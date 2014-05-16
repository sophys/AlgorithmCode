#include "stdio.h"
#include "stdlib.h"

#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define MAXSIZE 20 /* 存储空间初始分配量 */

typedef int Status;
/* SElemType类型根据实际情况而定，这里假设为int */
typedef int SElemType;

/* 链栈结构 */
typedef struct StackNode
{
    SElemType data;
    struct StackNode *next;
} StackNode,*LinkStackPtr;

typedef struct
{
    LinkStackPtr top;
    int count;
} LinkStack;

/*  构造一个空栈S */
Status InitStack(LinkStack *S)
{
    S->top = (LinkStackPtr)malloc(sizeof(StackNode));
    if(!S->top)
        return ERROR;
    S->top=NULL;
    S->count=0;
    return OK;
}

Status StackTraverse(LinkStack S)
{
    LinkStackPtr p;
    p=S.top;
    while(p)
    {
        visit(p->data);
        p=p->next;
    }
    printf("\n");
    return OK;
}

Status visit(SElemType c)
{
    printf("%d ",c);
    return OK;
}

/* 插入元素e为新的栈顶元素 */
Status Push(LinkStack *S,SElemType e)
{
    LinkStackPtr s=(LinkStackPtr)malloc(sizeof(StackNode));
    s->data=e;
    s->next=S->top;	/* 把当前的栈顶元素赋值给新结点的直接后继，见图中① */
    S->top=s;         /* 将新的结点s赋值给栈顶指针，见图中② */
    S->count++;
    return OK;
}

/* 若栈不空，则删除S的栈顶元素，用e返回其值，并返回OK；否则返回ERROR */
Status Pop(LinkStack *S,SElemType *e)
{
    LinkStackPtr p;
    if(StackEmpty(*S))
        return ERROR;
    *e=S->top->data;
    p=S->top;					/* 将栈顶结点赋值给p，见图中③ */
    S->top=S->top->next;    /* 使得栈顶指针下移一位，指向后一结点，见图中④ */
    free(p);                    /* 释放结点p */
    S->count--;
    return OK;
}

/* 若栈S为空栈，则返回TRUE，否则返回FALSE */
Status StackEmpty(LinkStack S)
{
    if (S.count==0)
        return TRUE;
    else
        return FALSE;
}

/* 把S置为空栈 */
Status ClearStack(LinkStack *S)
{
    LinkStackPtr p,q;
    p=S->top;
    while(p)
    {
        q=p;
        p=p->next;
        free(q);
    }
    S->count=0;
    return OK;
}

int main()
{
    LinkStack s;
    int opp;
    int j, value, e;

    if(InitStack(&s)==OK)
    {
        printf("链栈初始化成功。");
    }

    printf("\n1.随机给栈赋值 \n2.栈遍历 \n3.进栈 \n4.出栈");
    printf("\n5.置空链栈 \n6.判断链栈是否为空");
    printf("\n0.退出 \n请选择你的操作：\n");
    while(opp != '0')
    {
        scanf("%d",&opp);
        switch(opp)
        {
        case 1:
            srand(time(0));
            for(j=1; j<=10; j++)
            {
                Push(&s,rand()%100+1);
            }
            StackTraverse(s);
            break;

        case 2:
            StackTraverse(s);
            break;

        case 3:
            printf("请输入需要进栈的元素：");
            scanf("%d", &value);
            Push(&s, value);
            StackTraverse(s);
            break;

        case 4:
            Pop(&s,&e);
            printf("弹出的栈顶元素 e=%d\n",e);
            StackTraverse(s);
            break;

        case 5:
            ClearStack(&s);
            printf("清空栈后，栈是否为空：%d(1:空 0:否)\n",StackEmpty(s));
            break;

        case 6:
            printf("栈是否为空：%d(1:空 0:否)\n",StackEmpty(s));
            break;

        case 0:
            exit(0);
        }
    }
    return 0;
}

