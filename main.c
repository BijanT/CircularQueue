#include <stdio.h>
#include <stdbool.h>
#include "CircularQueue.h"

int testNum = 1;

void assert(int comparison)
{
	printf("Test #%d ", testNum);
	if(comparison == false)
	{
		printf("has failed\n");
	}
	else
	{
		printf("has passed\n");
	}

	testNum++;
}

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
	
	//Test that the queue does not allow having more elements than there is capacity
	assert(CQ_Enqueue(&queue, &array2[3]) == 0);

	for(int i = 0; i < 5; i++)
	{
		int number = 1;
		CQ_Dequeue(&queue, &number);
		//printf("%d\n", number);
	}

	//Test that the queue reports a failure when trying to dequeue when there are no elements in the queue
	int i;
	assert(CQ_Dequeue(&queue, &i) == 0);

	//Test queueing chars
	CircularQueue charQueue;
	char emptyCharArray[13];
	char* helloStr = "Hello World!";
	char recievingArray[13];

	CQ_Init(&charQueue, (void*)emptyCharArray, sizeof(char), 13);

	for(int i = 0; i < 13; i++)
	{
		CQ_Enqueue(&charQueue, &helloStr[i]);
	}
	for(int i = 0; i < 13; i++)
	{
		CQ_Dequeue(&charQueue, &recievingArray[i]);
	}

	assert(strcmp(helloStr, recievingArray) == 0);
	
	//Make sure calling enqueue/dequeue with a null value returns 0
	assert(CQ_Enqueue(NULL, &array2[1]) == 0);
	assert(CQ_Enqueue(&queue, NULL) == 0);
	assert(CQ_Dequeue(NULL, &array2[1]) == 0);

	return 0;
}
