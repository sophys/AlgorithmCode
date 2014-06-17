#include <stdio.h>

int binsearch(int *a,int len,int key)
{
	int sta,mid,end;

	sta = 0;
	end = len -1;

	while(sta<=end)
	{
		mid = (sta+end)/2;
//		printf("sta mid end %d %d %d\n",sta,mid,end);
		if(a[mid] == key)
		{
			return mid;
		}
		
		if(a[mid] > key)
		{
			end = mid - 1;
		}
		else
		{
			sta = mid + 1;
		}

	}
	return -1;
}	

int main()
{
	int a[]={1,2,3,4,5,6,7,8};
	
	int r = binsearch(a,8,8);
	if(r != -1)
	{
		printf("find in %d\n",r+1);
	}
	else
	{
		printf("can't find\n");
	}

	return 0;
}
