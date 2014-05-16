#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

void PrintArray(int *arr);
void swap(int *a,int *b);

int num = 10;

static int Partition(int *arr, int beg, int end)
{
    int j;
	int sentinel = arr[end];
	int i = beg-1;
	for(j=beg; j<=end-1; ++j)
	{
		if(arr[j] <= sentinel)
		{
			i++;
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i+1], &arr[end]);
	return i+1;
}

void QuickSort(int *arr, int beg, int end)
{
	if(beg>=end)return;
	int *tmp = (int*)malloc(sizeof(int)*(end-beg+1));
	int p=0;
	tmp[p++]=beg;
	tmp[p++]=end;

	int low,high,q;
	while(p>0)
	{
		high=tmp[--p];
		low =tmp[--p];

		if(low<high)
		{
			q=Partition(arr,low,high);

		if(q-low > high-q)
		{
			tmp[p++] = low;
			tmp[p++] = q-1;
			if(high > q)
			{
				tmp[p++] = q+1;
				tmp[p++] = high;
			}
		}
		else
		{
			tmp[p++] = q+1;
			tmp[p++] = high;
			if(q>low)
			{
				tmp[p++] = low;
				tmp[p++] =q-1;
			}
		}

/*		tmp[p++] = q+1;
		tmp[p++] = high;
		tmp[p++] = low;
		tmp[p++] =q-1;
*/
		}
	}
	free(tmp);
}

void swap(int *a,int *b)
{
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}
void PrintArray(int arr[])
{
    int i;
	for(i=0; i < num; ++i)
	{
	    printf("%d ", arr[i]);
	}
	printf("\n");
}

int main()
{
    int i;
	int arr[10];

	srand(time(0));
    for(i=0; i < 10; i++)
    {
        arr[i] = rand()%100+1;
    }
    printf("初始数组:");
    PrintArray(arr);

	QuickSort(arr, 0, num-1);
	printf("最后结果:");
	PrintArray(arr);
	return 0;
}
