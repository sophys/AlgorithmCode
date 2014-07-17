
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//直接插入排序
void insertSort(int A[],int n)
{
    int i,j,tmp;

    for(i=1;i!=n;i++)
    {
        tmp=A[i];
        j=i-1;
        while(j>=0 && tmp<A[j])
        {
            A[j+1]=A[j];
            j--;
        }
        A[j+1]=tmp;
    }
}
//折半插入排序
void binInsertSort(int A[],int n)
{
    int i,j,sta,end,mid;
    int tmp;

    for(i=1;i!=n;i++)
    {
        tmp=A[i];
        sta=0,end=i-1;
        while(sta<=end)
        {
            mid = (sta+end)/2;
            if(tmp<A[mid])
            {
                end = mid -1;
            }
            else
                sta = mid + 1;
        }
        for(j=i-1;j>=end+1;j--)
            A[j+1]=A[j];
        A[end+1]=tmp;
    }
}
//shell排序
void shellSort(int A[],int n)
{
    int i,j,gap;
    int tmp;

    gap=n/2;

    while(gap>0)
    {
        for(i=gap;i<n;i++)
        {
            tmp=A[i];
            j=i-gap;
            while(j>=0&&tmp<A[j])
            {
                A[j+gap]=A[j];
                j=j-gap;
            }
            A[j+gap]=tmp;
        }
        gap=gap/2;
    }
}
//起泡排序
void bubbleSort(int A[],int n)
{
    int i,j,tmp;
    for(i=0;i!=n-1;i++)
        for(j=n-1;j>i;j--)
            if(A[j]<A[j-1])
            {
                tmp=A[j];
                A[j]=A[j-1];
                A[j-1]=tmp;
            }
}
//快速排序
void quickSort(int A[],int s,int t)
{
    int i=s,j=t;
    int pivot;
    if(s<t)
    {
        pivot=A[s];//基准元素的选择可以用partition算法
        while(i!=j)
        {
            while(j>i && A[j]>pivot)
                j--;
            A[i]=A[j];
            while(i<j&&A[i]<pivot)
                i++;
            A[j]=A[i];
        }
        A[i]=pivot;

//        for(int ii=0;ii!=6;ii++)
//            printf("%d ",A[ii]);
//        printf("\n");

        quickSort(A,s,i-1);
        quickSort(A,i+1,t);
    }
}
//选择排序
void selectSort(int A[],int n)
{
    int i,j,k;
    int tmp;
    for(i=0;i<n-1;i++)
    {
        k=i;
        for(j=i+1;j<n;j++)
        {
            if(A[j]<A[k])
                k=j;
        }
        if(k!=i)
        {
            tmp=A[i];
            A[i]=A[k];
            A[k]=tmp;
        }
    }
}
//堆排序
/*      i
 *     / \
 *  2i+1  2i+2
*/
void sift(int A[],int index,int size)
{
    int max;
    int tmp;
    while(index*2+1<size)
    {
        max = index*2+1;
        if(index*2+2 <size)
        {
            if(A[max] < A[index*2+2])
                max=index*2+2;
        }
        if(A[index]>A[max])
            break;
        else
        {
            tmp=A[index];
            A[index]=A[max];
            A[max]=tmp;
            index=max;
        }
    }
}

void heapSort(int A[],int n)
{
    int i;
    int tmp;
    for(i=n/2-1;i>=0;i--)
        sift(A,i,n);
    for(i=n-1;i>=1;i--)
    {
        tmp=A[0];
        A[0]=A[i];
        A[i]=tmp;
        n--;
        sift(A,0,n);
    }
}
//归并排序
void merge(int A[],int low,int mid,int high)
{
    int *p;
    int i=low,j=mid+1,k=0;

    p=(int*)malloc((high-low+1)*sizeof(int));
    while(i<=mid && j<=high)
        if(A[i]<=A[j])
        {
            p[k]=A[i];
            i++,k++;
        }
        else
        {
            p[k]=A[j];
            j++,k++;
        }
    while(i<=mid)
    {
        p[k]=A[i];
        i++,k++;
    }
    while(j<=high)
    {
        p[k]=A[j];
        j++,k++;
    }
    for(k=0,i=low;i<=high;k++,i++)
        A[i]=p[k];
    free(p);
}

void mergePass(int A[],int length,int n)
{
    int i;
    for(i=0;i+2*length-1<n;i=i+2*length)
    {
        merge(A,i,i+length-1,i+2*length-1);
    }
    if(i+length-1 < n)
        merge(A,i,i+length-1,n-1);
}

void mergeSort(int A[],int n)
{
    int i;
    for(i=1;i<n;i=i*2)
        mergePass(A,i,n);
}

int main()
{
    int i;
    int A[6] = {3,8,2,6,5,1};

    printf("Before Sort:\n");
    for(i=0;i!=6;i++)
        printf("%d ",A[i]);
    printf("\n");

    insertSort(A,6);
    binInsertSort(A,6);
    shellSort(A,6);
    bubbleSort(A,6);
    quickSort(A,0,5);
    selectSort(A,6);
    heapSort(A,6);
    mergeSort(A,6);

    printf("After Sort:\n");
    for(i=0;i!=6;i++)
        printf("%d ",A[i]);
    printf("\n");

    return 0;
}
