#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "CircularQueue.h"

void assert(int comparison, int line)
{
	printf("Test on line #%d ", line);
	if(comparison == false)
	{
		printf("has failed\n");
	}
	else
	{
		printf("has passed\n");
	}
}

int main(void)
{
	int array[5];
	CircularQueue queue;
	int array2[5] = {1, 2, 3, 4, 5};

	assert(CQ_Init(&queue, (void*)array, sizeof(int), 5) == 1, __LINE__);

	for(int i = 0; i < 5; i++)
	{
		CQ_Enqueue(&queue, &array2[i]);
	}
	
	//Test that the queue does not allow having more elements than there is capacity
	assert(CQ_Enqueue(&queue, &array2[3]) == 0, __LINE__);

	for(int i = 0; i < 5; i++)
	{
		int number = 1;
		CQ_Dequeue(&queue, &number);
		//printf("%d\n", number);
	}

	//Test that the queue reports a failure when trying to dequeue when there are no elements in the queue
	int i;
	assert(CQ_Dequeue(&queue, &i) == 0, __LINE__);

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

	assert(strcmp(helloStr, recievingArray) == 0, __LINE__);
	
	//Make sure calling enqueue/dequeue with a null value returns 0
	assert(CQ_Enqueue(NULL, &array2[1]) == 0, __LINE__);
	assert(CQ_Enqueue(&queue, NULL) == 0, __LINE__);
	assert(CQ_Dequeue(NULL, &array2[1]) == 0, __LINE__);

	//Test EnqueueBuffer
	CircularQueue queue3;
	char emptyArray3[5];
	char output;	
	char outputArray[6];
	outputArray[5] = '\0';
	
	CQ_Init(&queue3, (void*)emptyArray3, sizeof(char),5);
	
	i = CQ_EnqueueBuffer(&queue3, helloStr, 5*sizeof(char));
	assert(i == 5, __LINE__);

	//Test DequeueBuffer with a simple case
	CQ_DequeueBuffer(&queue3, &outputArray, 5*sizeof(char));
	assert(strcmp("Hello", outputArray) == 0, __LINE__);

	//Make sure you cannot add more to the queue when it is full
	CQ_EnqueueBuffer(&queue3, helloStr, 5*sizeof(char));
	assert(CQ_EnqueueBuffer(&queue3, helloStr, 2*sizeof(char)) == 0, __LINE__);
	
	//Make sure EnqueueBuffer only enqueues as much as it has room for
	CQ_Dequeue(&queue3, &output);
	i = CQ_EnqueueBuffer(&queue3, helloStr, 2*sizeof(char)); 
	assert(i == 1, __LINE__);
	//Print the output of queue3
	CQ_DequeueBuffer(&queue3, &outputArray, 5*sizeof(char)); 
	printf("%s\n", outputArray);

	assert(strcmp("elloH", outputArray) == 0, __LINE__);

	//Test case where the data has to wrap around the array
	CQ_Init(&queue3, (void*)emptyArray3, sizeof(char), 5);
	
	CQ_EnqueueBuffer(&queue3, helloStr, 4*sizeof(char));

	CQ_Dequeue(&queue3, &output);

	CQ_EnqueueBuffer(&queue3, helloStr, 2*sizeof(char));

	//print and test the contents of queue3
	CQ_DequeueBuffer(&queue3, &outputArray, 5*sizeof(char));
	printf("%s\n", outputArray);
	assert(strcmp("ellHe", outputArray) == 0, __LINE__);

	//Test calling DequeueBuffer asking for more elements than there are in the queue
	for(int i =0; i < 6; i++)
	{
		outputArray[i] = '\0';
	}
	CQ_EnqueueBuffer(&queue3, helloStr, 3*sizeof(char));
	assert(CQ_DequeueBuffer(&queue3, &outputArray, 5*sizeof(char)) == 3, __LINE__);
	printf("%s\n", outputArray);
	assert(strcmp("Hel", outputArray) == 0, __LINE__);

	return 0;
}
