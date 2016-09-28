#include "CircularQueue.h"

//Initialize a new Circular Queue
int CQ_Init(CircularQueue* queue, void* arrayPtr, int itemSize, int arrayCapacity)
{	
	queue->arrayPtr = arrayPtr;
	queue->front = queue->back = 0;
	queue->itemSize = itemSize;
	queue->capacity = arrayCapacity;
	queue->arraySize = arrayCapacity * itemSize;
	queue->numElements = 0;	

	return 1;
}

//Add a new element to the back of the queue
int CQ_Enqueue(CircularQueue* queue, void* itemPtr)
{
	//Check for null inputs
	if(queue == NULL || itemPtr == NULL)
	{
		return 0;
	}
	
	//Make sure the back is within the bounds of the array
	if(queue->back < 0 || queue->back >= queue->arraySize)
	{
		return 0;
	}

	//If the number of elements is equal to the capacity of the queue, there is nowhere
	//to put the new item, so return 0
	if(queue->numElements == queue->capacity)
	{
		return 0;
 	}

	//put the value of new item in the back of the queue
	//The value stored in back is the offset in bytes where the next element of the array is, so we add it to arrayPtr
	//to put the data in the right place
	memcpy(queue->arrayPtr + queue->back, itemPtr, queue->itemSize);	
	
	//update the back of the queue and increment numElements
	//Make sure the value for the back doesn't go past the size of the array
	queue->back += queue->itemSize;
	if(queue->back >= queue->arraySize)
	{
		queue->back = 0;
	}
	queue->numElements++;	

	//return 1 to indicate success
	return 1;
}

//Add an array of items to the back of the queue
int CQ_EnqueueBuffer(CircularQueue* queue, void* buffPtr, int buffSize)
{
	//Check for invalid array size
	if (arrSize <=0) 
	{
		return 0
	}
	
	//Check for null pointers
	if (queue == NULL || arrayPtr == NULL) 
	{
		return 0;
	}
	
	//Make sure the back is within the bounds of the array
	if(queue->back < 0 || queue->back >= queue->arraySize)
	{
		return 0;
	}

	//If the number of elements is equal to the capacity of the queue, there is nowhere
	//to put the new item, so return 0
	if(queue->numElements == queue->capacity)
	{
		return 0;
 	}
	
	//Check if the queue has enough room for the entire array,
	
	//Case 1:
	//The queue does have enough room for the entire array,
	//copy the entire array to the back of the queue
	if (queue->capacity >= buffSize)
	{
		//Check if the back of the queue can fit the entire buffer array
		
		//Case 1: the back of the queue can fit the entire buffer array
		//The function will copy the buffer array to the back of the queue
		if (queue->capacity - queue->back >= buffSize) 
		{
			uint32_t = buffSize_bytes = queue->itemSize * buffSize;
			memccpy(queue->arrayPtr + queue->back, buffPtr, buffSize_bytes);
			
			return 1;
		}
		
		//Case 2: the back of the queue cannot fit the entire buffer array
		//The function will copy first part of the buffer array to the back of the queue,
		//and copy the rest of the array to the front of the queue
		else 
		{
			uint32_t bytesLeft_Back = (queue->capacity * queue->itemSize - queue->back);
			void *rest_buffPtr = queue->arrayPtr + bytesLeft_Back;
			
			uint32_t bytesLeft_buffArray = buffSize - bytesLeft_Back;
			
			memccpy(queue->arrayPtr + queue->back, buffPtr, bytesLeft_Back);
			memccpy(queue->arrayPtr, rest_buffPtr, bytesLeft_buffArray);
			
			return 1;
		}
	}	
	
	//Case 2:
	//The queue does not have enough room for the array, 
	//no operation will be done, and the function returns 0 to indicate failure.
	else
	{
		return 0
	}	
	
	//If the queue does not have enough room for teh entire array,
	//copy the first part the buff array to the back of the queue,
	//then copy the rest of the array to the front of the queue.
	else 
	{
		
	}
	
	//Go through the array and enqueue each item
	for (int i = 0; i < arrSize; i++) 
	{
		//Check if the queue can store any more item
		if(queue->numElements == queue->capacity)
		{
			return 0;
		
		}
		
		//put the value of each array element in the back of the queue
		//The value stored in back is the offset in bytes where the next element of the array is, so we add it to arrayPtr
		//to put the data in the right place
		memcpy(queue->arrayPtr + queue->back, arrayPtr[i], queue->itemSize);
		
		//update the back of the queue and increment numElements
		//Make sure the value for the back doesn't go past the size of the array
		queue->back += queue->itemSize;
		if(queue->back >= queue->arraySize)
		{
			queue->back = 0;
		}
		queue->numElements++;	
	}
	
	//return 1 to indicate success
	return 1;
}

//Remove an element from the front of the queue
int CQ_Dequeue(CircularQueue* queue, void* itemOut)
{
	//Check for null inputs
	if(queue == NULL)
	{
		return 0;
	}

	//Make sure the front is within the bounds
	if(queue->front < 0 || queue->front >= queue->arraySize)
	{
		return 0;
	}
	
	//Make sure there is at least one item in the Queue
	if(queue->numElements <= 0)
	{
		return 0;
	}

	//Grab the item in the front of the queue
	//The value stored in front is the offset in bytes to where the next element in the queue is, so we add it to arrayPtr
	//to get the data from the right place
	memcpy(itemOut, queue->arrayPtr + queue->front, queue->itemSize);
	
	//Update the front and decrement numElements
	//Make sure the value for the front doesn't go past the size of the array
	queue->front += queue->itemSize;
	if(queue->front >= queue->arraySize)
	{
		queue->front = 0;
	}
	queue->numElements--;

	return 1;
}
