#include "stdio.h"
#include "stdlib.h"

#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
/* 存储空间初始分配量 */
#define MAXSIZE 20

typedef int Status;
/* SElemType类型根据实际情况而定，这里假设为int */
typedef int SElemType;

/* 顺序栈结构 */
typedef struct
{
    SElemType data[MAXSIZE];
    int top; /* 用于栈顶指针 */
}SqStack;

/*  构造一个空栈S */
Status InitStack(SqStack *S)
{
    /* S.data=(SElemType *)malloc(MAXSIZE*sizeof(SElemType)); */
    S->top=-1;
    return OK;
}

/* 从栈底到栈顶依次对栈中每个元素显示 */
Status StackTraverse(SqStack S)
{
    int i;
    i=0;
    while(i<=S.top)
    {
        visit(S.data[i++]);
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
Status Push(SqStack *S,SElemType e)
{
    if(S->top == MAXSIZE -1) /* 栈满 */
    {
        return ERROR;
    }
    S->top++;				/* 栈顶指针增加一 */
    S->data[S->top]=e;  /* 将新插入元素赋值给栈顶空间 */
    return OK;
}

/* 若栈不空，则删除S的栈顶元素，用e返回其值，并返回OK；否则返回ERROR */
Status Pop(SqStack *S,SElemType *e)
{
    if(S->top==-1)
        return ERROR;
    *e=S->data[S->top];	/* 将要删除的栈顶元素赋值给e */
    S->top--;				/* 栈顶指针减一 */
    return OK;
}

/* 若栈不空，则用e返回S的栈顶元素，并返回OK；否则返回ERROR */
Status GetTop(SqStack S,SElemType *e)
{
    if (S.top==-1)
        return ERROR;
    else
        *e=S.data[S.top];
    return OK;
}

/* 若栈S为空栈，则返回TRUE，否则返回FALSE */
Status StackEmpty(SqStack S)
{
    if (S.top==-1)
        return TRUE;
    else
        return FALSE;
}

/* 把S置为空栈 */
Status ClearStack(SqStack *S)
{
    S->top=-1;
    return OK;
}

int main()
{
    SqStack s;
    int opp;
    int j, value, e;

    if(InitStack(&s)==OK)
    {
        printf("顺序栈初始化成功。");
        StackTraverse(s);
    }

    printf("\n1.随机给栈赋值 \n2.栈遍历 \n3.进栈 \n4.出栈");
    printf("\n5.获取栈顶元素 \n6.判断栈是否为空 \n7.将栈置空");
    printf("\n0.退出 \n请选择你的操作：\n");
    while(opp != '0'){
        scanf("%d",&opp);
        switch(opp){
            case 1:
                srand(time(0));
                for(j=1;j<=10;j++)
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
                GetTop(s,&e);
                printf("栈顶元素 e=%d\n",e);
                break;

            case 6:
                printf("栈是否为空：%d(1:空 0:否)\n",StackEmpty(s));
                break;

            case 7:
                ClearStack(&s);
                printf("清空栈后，栈是否为空：%d(1:空 0:否)\n",StackEmpty(s));
                break;

            case 0:
                exit(0);
        }
    }

    return 0;
}
