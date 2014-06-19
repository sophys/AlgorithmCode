#include <stdio.h>
#include <stdlib.h>
	
#define BITSPERWORD 32
#define SHIFT 5
#define MASK 0x1F
#define NN 100000000
#define MM 100000000

int a[1 + NN/BITSPERWORD];

//右移5位相当于除2^5也就是32，i & 0x1F 保留了低五位，相当于模32.
/*
void set(int i) {        a[i/32] |=  (1<<(i%32)); }
void clr(int i) {        a[i/32] &= ~(1<<(i %32)); }
int  test(int i){ return a[i/32] &   (1<<(i %32)); }
*/
void set(int i){
	a[i >> SHIFT] |= (1<<(i & MASK));
}
void clr(int i){
	a[i >> SHIFT] &= ~(1<<(i & MASK));
}
int test(int i){
	return a[i >> SHIFT] & (1<<(i & MASK));
}

int main(int argc,char **argv)
{
	int i;
	int j;
	int *arr = (int*)malloc(sizeof(int)*MM);
	int count=0;	
	
	for (i = 0; i < NN; i++) {
		clr(i);
	}

	for (j = 0; j < MM; j++) 
	{
		arr[j]=rand()%NN;
	}
	
	for (j = 0; j < MM; j++) 
	{
		set(arr[j]);
	}
	
	printf("\nAfter Sorted:\n");
	for (i = 0; i < NN; i++) 
	{
		if (test(i)) 
		{
			count++;
		}
	}
	printf("\nAfter sorted count is %d\n",count);    
	return 0;
} 
