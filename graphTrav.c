
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define SIZE 5

typedef struct ANode
{
    int adjvex;
    struct ANode *nextarc;
}ArcNode;
typedef struct Vnode
{
    int vnode;
    ArcNode *firstarc;
}VNode;

typedef struct
{
    VNode adjlist[SIZE];
    int n,e;
}AGraph;
/*
 *       1
 *     / | \
 *    2--3  0
 *    \     /
 *     \   /
 *       4
*/
int matrix[SIZE][SIZE]=
{
    {0,1,0,0,1},
    {1,0,1,1,0},
    {0,1,0,0,1},
    {0,1,1,0,0},
    {1,0,1,0,0}
};
AGraph *initGraph()
{
    ArcNode *p;
    AGraph *G = (AGraph*)malloc(sizeof(AGraph));
    G->n=SIZE;
    G->e=6;

    int i,j;

    for(i=0;i!=SIZE;i++)
        G->adjlist[i].firstarc = NULL;

    for(i=0;i!=SIZE;i++)
        for(j=0;j!=SIZE;j++)
            if(matrix[i][j] !=0)
            {
                p=(ArcNode*)malloc(sizeof(ArcNode));
                p->adjvex=j;
                p->nextarc=G->adjlist[i].firstarc;
                G->adjlist[i].firstarc = p;
            }

    return G;
}

int visited[SIZE];
void DFS(AGraph *G,int v)
{
    ArcNode *p;
    visited[v]=1;
    printf("%d ",v);

    p = G->adjlist[v].firstarc;

    while(NULL != p)
    {
        if(visited[p->adjvex] == 0)
            DFS(G,p->adjvex);
        p=p->nextarc;
    }
}
void BFS(AGraph *G,int v)
{
    ArcNode *p;
    int queue[SIZE],front=0,rear=0;
    int visited[SIZE];

    int w,i;
    for(i=0;i!=G->n;i++)
        visited[i]=0;
    printf("%d ",v);
    visited[v]=1;

    rear = (rear+1)%SIZE;
    queue[rear]=v;

    while(front != rear)
    {
        front = (front+1)%SIZE;
        w=queue[front];

        p = G->adjlist[w].firstarc;

        while (NULL != p) {
            if(visited[p->adjvex] == 0)
            {
                printf("%d ",p->adjvex);
                visited[p->adjvex]=1;
                rear = (rear+1)%SIZE;
                queue[rear] = p->adjvex;
            }
            p=p->nextarc;
        }
    }
}

int main()
{
    AGraph* G=initGraph();

    printf("DFS Travel:");
    DFS(G,1);

    printf("BFS Travel:");
    BFS(G,1);

    return 0;
}

