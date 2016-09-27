#include <stdio.h>
#include <assert.h>
#include "CircularQueue.h"

int main(void)
{
	int array[5];
	CircularQueue queue;
	int array2[5] = {1, 2, 3, 4, 5};	
	assert(CQ_Init(&queue, (void*)array, sizeof(int), 5) == 1);

	for(int i = 0; i < 5; i++)
	{
		CQ_Enqueue(&queue, &array2[i]);
	}

	for(int i = 0; i < 5; i++)
	{
		int number = 1;
		CQ_Dequeue(&queue, &number);
		printf("%d\n", number);
	}

	return 0;
}
