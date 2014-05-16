#include "stdio.h"
#include "stdlib.h"

#define OK 1
#define ERROR 0
#define TRUE 1
#define FALSE 0
#define MAXSIZE 20 /* 存储空间初始分配量 */

typedef int Status;
/* QElemType类型根据实际情况而定，这里假设为int */
typedef int QElemType;

/* 循环队列的顺序存储结构 */
typedef struct
{
	QElemType data[MAXSIZE];
	int front;    	/* 头指针 */
	int rear;		/* 尾指针，若队列不空，指向队列尾元素的下一个位置 */
}SqQueue;

/* 初始化一个空队列Q */
Status InitQueue(SqQueue *Q)
{
	Q->front=0;
	Q->rear=0;
	return  OK;
}

Status visit(QElemType c)
{
	printf("%d ",c);
	return OK;
}

/* 从队头到队尾依次对队列Q中每个元素输出 */
Status QueueTraverse(SqQueue Q)
{
	int i;
	i=Q.front;
	while((i+Q.front)!=Q.rear)
	{
		visit(Q.data[i]);
		i=(i+1)%MAXSIZE;
	}
	printf("\n");
	return OK;
}

/* 若队列未满，则插入元素e为Q新的队尾元素 */
Status EnQueue(SqQueue *Q,QElemType e)
{
	if ((Q->rear+1)%MAXSIZE == Q->front)	/* 队列满的判断 */
		return ERROR;
	Q->data[Q->rear]=e;			/* 将元素e赋值给队尾 */
	Q->rear=(Q->rear+1)%MAXSIZE;/* rear指针向后移一位置， */
								/* 若到最后则转到数组头部 */
	return  OK;
}

/* 若队列不空，则删除Q中队头元素，用e返回其值 */
Status DeQueue(SqQueue *Q,QElemType *e)
{
	if (Q->front == Q->rear)			/* 队列空的判断 */
		return ERROR;
	*e=Q->data[Q->front];				/* 将队头元素赋值给e */
	Q->front=(Q->front+1)%MAXSIZE;	/* front指针向后移一位置， */
									/* 若到最后则转到数组头部 */
	return  OK;
}

/* 将Q清为空队列 */
Status ClearQueue(SqQueue *Q)
{
	Q->front=Q->rear=0;
	return OK;
}

/* 若队列Q为空队列,则返回TRUE,否则返回FALSE */
Status QueueEmpty(SqQueue Q)
{
	if(Q.front==Q.rear) /* 队列空的标志 */
		return TRUE;
	else
		return FALSE;
}

/* 返回Q的元素个数，也就是队列的当前长度 */
int QueueLength(SqQueue Q)
{
	return  (Q.rear-Q.front+MAXSIZE)%MAXSIZE;
}

int main()
{
    int opp, j;
    SqQueue Q;
    QElemType d;
    InitQueue(&Q);


    printf("\n1.给队列附初始值 \n2.遍历队列 \n3.入队 \n4.出队");
    printf("\n5.判断队列是否为空 \n6.清空队列 \n7.求队列长度");
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
                    EnQueue(&Q,d);
                }
                QueueTraverse(Q);
                break;

            case 2:
                QueueTraverse(Q);
                break;

            case 3:
                printf("请输入需要入队的元素：");
                scanf("%d", &d);
                EnQueue(&Q,d);
                QueueTraverse(Q);
                break;

            case 4:
                DeQueue(&Q,&d);
                printf("删除的元素值为%d\n",d);
                break;

            case 5:
                printf("现在队列空否？%u(1:空 0:否)\n",QueueEmpty(Q));
                break;

            case 6:
                ClearQueue(&Q);
                printf("清空队列后, 队列空否？%u(1:空 0:否)\n",QueueEmpty(Q));
                break;

            case 7:
                printf("队列长度为: %d\n",QueueLength(Q));
                break;

            case 0:
                exit(0);
        }
    }
    return 0;
}
