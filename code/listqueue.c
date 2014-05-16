#include "stdio.h"
#include "stdlib.h"
#include "io.h"
#include "math.h"
#include "time.h"

#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define MAXSIZE 20 /* 存储空间初始分配量 */

typedef int Status;
/* QElemType类型根据实际情况而定，这里假设为int */
typedef int QElemType;

typedef struct QNode	/* 结点结构 */
{
    QElemType data;
    struct QNode *next;
} QNode,*QueuePtr;

typedef struct			/* 队列的链表结构 */
{
    QueuePtr front,rear; /* 队头、队尾指针 */
} LinkQueue;

/* 构造一个空队列Q */
Status InitQueue(LinkQueue *Q)
{
    Q->front = Q->rear = (QueuePtr)malloc(sizeof(QNode));
    if(!Q->front)
        exit(OVERFLOW);
    Q->front->next=NULL;
    return OK;
}

/* 从队头到队尾依次对队列Q中每个元素输出 */
Status QueueTraverse(LinkQueue Q)
{
    QueuePtr p;
    p=Q.front->next;
    while(p)
    {
        visit(p->data);
        p=p->next;
    }
    printf("\n");
    return OK;
}

Status visit(QElemType c)
{
    printf("%d ",c);
    return OK;
}

/* 插入元素e为Q的新的队尾元素 */
Status EnQueue(LinkQueue *Q,QElemType e)
{
    QueuePtr s=(QueuePtr)malloc(sizeof(QNode));
    if(!s) /* 存储分配失败 */
        exit(OVERFLOW);
    s->data=e;
    s->next=NULL;
    Q->rear->next=s;	/* 把拥有元素e的新结点s赋值给原队尾结点的后继，见图中① */
    Q->rear=s;		/* 把当前的s设置为队尾结点，rear指向s，见图中② */
    return OK;
}

/* 若队列不空,删除Q的队头元素,用e返回其值,并返回OK,否则返回ERROR */
Status DeQueue(LinkQueue *Q,QElemType *e)
{
    QueuePtr p;
    if(Q->front==Q->rear)
        return ERROR;
    p=Q->front->next;		/* 将欲删除的队头结点暂存给p，见图中① */
    *e=p->data;				/* 将欲删除的队头结点的值赋值给e */
    Q->front->next=p->next;/* 将原队头结点的后继p->next赋值给头结点后继，见图中② */
    if(Q->rear==p)		/* 若队头就是队尾，则删除后将rear指向头结点，见图中③ */
        Q->rear=Q->front;
    free(p);
    return OK;
}

/* 若队列不空,则用e返回Q的队头元素,并返回OK,否则返回ERROR */
Status GetHead(LinkQueue Q,QElemType *e)
{
    QueuePtr p;
    if(Q.front==Q.rear)
        return ERROR;
    p=Q.front->next;
    *e=p->data;
    return OK;
}

/* 求队列的长度 */
int QueueLength(LinkQueue Q)
{
    int i=0;
    QueuePtr p;
    p=Q.front;
    while(Q.rear!=p)
    {
        i++;
        p=p->next;
    }
    return i;
}

/* 若Q为空队列,则返回TRUE,否则返回FALSE */
Status QueueEmpty(LinkQueue Q)
{
    if(Q.front==Q.rear)
        return TRUE;
    else
        return FALSE;
}

/* 将Q清为空队列 */
Status ClearQueue(LinkQueue *Q)
{
    QueuePtr p,q;
    Q->rear=Q->front;
    p=Q->front->next;
    Q->front->next=NULL;
    while(p)
    {
        q=p;
        p=p->next;
        free(q);
    }
    return OK;
}

/* 销毁队列Q */
Status DestroyQueue(LinkQueue *Q)
{
    while(Q->front)
    {
        Q->rear=Q->front->next;
        free(Q->front);
        Q->front=Q->rear;
    }
    return OK;
}

int main()
{
    int i, j, opp;
    QElemType d;
    LinkQueue q;
    InitQueue(&q);

    printf("\n1.给队列赋初始值 \n2.遍历队列 \n3.入队 \n4.出队");
    printf("\n5.返回队头元素 \n6.求队列长度 \n7.判断队列是否为空 \n8.置空链队列");
    printf("\n9.销毁链队列");
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
                d = rand()%100+1;
                EnQueue(&q,d);
            }
            QueueTraverse(q);
            break;

        case 2:
            QueueTraverse(q);
            break;

        case 3:
            printf("请输入需要入队的元素：");
            scanf("%d", &d);
            EnQueue(&q,d);
            QueueTraverse(q);
            break;

        case 4:
            DeQueue(&q,&d);
            printf("删除的元素值为%d\n",d);
            break;

        case 5:
            GetHead(q,&d);
            printf("队头元素为%d\n",d);
            break;

        case 6:
            printf("队列的长度为%d\n",QueueLength(q));
            break;

        case 7:
            printf("是否空队列？%d (1:空 0:否) \n",QueueEmpty(q));
            break;

        case 8:
            ClearQueue(&q);
            printf("清空队列后,q.front=%u q.rear=%u q.front->next=%u\n",q.front,q.rear,q.front->next);
            break;

        case 9:
            DestroyQueue(&q);
            printf("销毁队列后,q.front=%u q.rear=%u\n",q.front, q.rear);
            break;

        case 0:
            exit(0);
        }
    }

    return 0;
}
