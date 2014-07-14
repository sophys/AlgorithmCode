#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define SIZE 10

typedef struct node
{
    char data;
    struct node *lchild,*rchild;
}Tnode,Tree;

void preTrav(Tree* T)
{
    Tnode *stack[SIZE],*p;
    int top=-1;
    if(NULL != T)
    {
        top++;
        stack[top]=T;
        while(top>-1)
        {
            p=stack[top];
            top--;

            printf("%c -> ",p->data);

            if(NULL != p->rchild)
            {
                top++;
                stack[top]=p->rchild;
            }
            if(NULL != p->lchild)
            {
                top++;
                stack[top]=p->lchild;
            }
        }
        printf("\n");
    }
}
void inTrav(Tree *T)
{
    Tnode *stack[SIZE],*p;
    int top = -1;

    if(NULL != T)
    {
        p=T;
        while(top > -1 || NULL != p)
        {
            while( NULL != p)
            {
                top ++;
                stack[top]=p;
                p=p->lchild;
            }
            if(top > -1)
            {
                p = stack[top];
                top --;
                printf("%c -> ",p->data);
                p = p->rchild;

            }
        }
        printf("\n");
    }
}
void postTrav(Tree *T)
{
    Tnode *stack[SIZE],*p,*q;
    p = T;
    int flag,top=-1;

    if(NULL != T)
    {
        do
        {
            while(NULL != p)
            {
                top ++;
                stack[top] = p;
                p = p->lchild;
            }
            q = NULL;
            flag = 1;
            while( -1 != top && flag == 1)
            {
                p = stack[top];
                if(p->rchild == q)
                {
                    printf("%c -> ",p->data);
                    top --;
                    q=p;
                }
                else
                {
                    p = p->rchild;
                    flag = 0;
                }
            }
        }while(-1 != top);
        printf("\n");
    }

}
void levelTrav(Tree *T)
{
    Tnode *p;
    Tnode *queue[SIZE];
    int front,rear;

    front=rear=0;
    rear++;
    queue[rear]=T;

    while(front != rear)
    {
        front = (front+1)%SIZE;
        p = queue[front];

        printf("%c -> ",p->data);

        if(NULL != p->lchild)
        {
            rear = (rear+1)%SIZE;
            queue[rear] = p->lchild;
        }
        if(NULL != p->rchild)
        {
            rear = (rear+1)%SIZE;
            queue[rear] = p->rchild;
        }
    }
    printf("\n");
}

/*
 *       A
 *      / \
 *     B   C
 *    /   /
 *   E   D
*/
Tree* initTree(void)
{
    Tnode *A = (Tnode*)malloc(sizeof(Tnode));
    Tnode *B = (Tnode*)malloc(sizeof(Tnode));
    Tnode *C = (Tnode*)malloc(sizeof(Tnode));
    Tnode *D = (Tnode*)malloc(sizeof(Tnode));
    Tnode *E = (Tnode*)malloc(sizeof(Tnode));
    A->data = 'A';
    B->data = 'B';
    C->data = 'C';
    D->data = 'D';
    E->data = 'E';

    A->lchild = B;
    A->rchild = C;
    B->lchild = E;
    B->rchild = NULL;
    C->lchild = D;
    C->rchild = NULL;
    D->rchild = NULL;
    D->lchild = NULL;
    E->rchild = NULL;
    E->lchild = NULL;
    return A;
}
int main()
{
    Tree* T=initTree();
    printf("preOrder Travel:");
    preTrav(T);

    printf("inOrder Travel:");
    inTrav(T);

    printf("postOrder Travel:");
    postTrav(T);

    printf("leveltOrder Travel:");
    levelTrav(T);
    return 0;
}
