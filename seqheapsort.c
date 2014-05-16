/*
the i-th element in array
i: parent:size/2-1
   left:2*i+1
   right:2*i+2	
*/

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
void HeapSort(int *heap,int len)
{
	int i;
	int size = len;
	BuildHeap(heap,size);

	for(i=size-1;i>=1;i--)
	{
		swap(heap,0,i);
		size--;
		MaxHeap(heap,0,size);
	}
}
		
int main(int argc, const char * argv[])
{
    int k;
    int i;
	
	int heap[10]={9,8,7,6,5,4,3,2,1,0};

	HeapSort(heap,10);

    for (i=0; i<10; i++) {
        printf("%d ",heap[i]);
    }
    
    return 0;
}

