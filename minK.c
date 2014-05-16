//
//  main.c
//  minK
//
//  Created by Ysan on 14-4-22.
//  Copyright (c) 2014年 Ysan. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void swap(int *heap,int a,int b)
{
    int tmp;
    tmp = heap[a];
    heap[a] = heap[b];
    heap[b] = tmp;
}
void MaxHeap(int *heap,int index,int len)
{
    int largeIndex = -1;
    
    while (index*2+1<len) {
        largeIndex = index*2+1;
        if (index*2+2 <len) {
            if (heap[largeIndex]<heap[index*2+2]) {
                largeIndex = index*2+2;
            }
        }
        
        if(heap[index]>=heap[largeIndex])
            break;
        else
        {
            swap(heap,index,largeIndex);
            index = largeIndex;
        }
    }
}
void BuildHeap(int *heap,int len)
{
    if(heap == NULL)
        return;
    int index = len / 2-1;
    for (int i=index; i>=0; i--) {
        MaxHeap(heap, i, len);
    }
}
int main(int argc, const char * argv[])
{
    int k;
    int i;
    
    freopen("data.txt", "r",stdin);
    
    scanf("%d",&k);
    
    int *heap = (int* )malloc(k);
    
    for (i = 0; i<k; i++) {
        scanf("%d",&heap[i]);
    }
    
    BuildHeap(heap, k);
    
    for (i=0; i<k; i++) {
        printf("%d ",heap[i]);
    }
    printf("\n");
    int data;
    while(scanf("%d",&data)) {
        if(data == 100)
            break;
        if (data < heap[0]) {
            heap[0] = data;
            MaxHeap(heap, 0, k);
            
            for (i = 0; i< k; i++) {
                printf("%d ",heap[i]);
            }
            printf("\n");
        }
    }
    for (i=0; i<k; i++) {
        printf("%d ",heap[i]);
    }
    printf("\n");
    fclose(stdin);
    free(heap);
    
    return 0;
}

