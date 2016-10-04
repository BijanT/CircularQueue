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
	queue->freeBytes = queue->arraySize;

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
	queue->freeBytes -= itemSize;
	
	//return 1 to indicate success
	return 1;
}

//Add an array of items to the back of the queue
int CQ_EnqueueBuffer(CircularQueue* queue, void* buffPtr, int buffSize)
{
	//Check for invalid array size
	if (buffSize <=0) 
	{
		return 0;
	}
	
	//Check for null pointers
	if (queue == NULL || buffPtr == NULL) 
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

	uint32_t itemsWritten;

	//Check if the queue has enough room for the entire array,
	
	//Case 1:
	//The queue does have enough room for the entire array,
	//copy the entire array to the back of the queue
	if (queue->freeBytes >= buffSize)
	{
		//Calculate how many items will be written to the queue
		itemsWritten = buffSize / queue->itemSize;

		//Check if the back of the queue can fit the entire buffer array
		
		//Case 1: the back of the queue can fit the entire buffer array
		//The function will copy the buffer array to the back of the queue
		if (queue->arraySize - queue->back >= buffSize) 
		{
			memcpy(queue->arrayPtr + queue->back, buffPtr, buffSize);	
		}
		
		//Case 2: the back of the queue cannot fit the entire buffer array
		//The function will copy first part of the buffer array to the back of the queue,
		//and copy the rest of the array to the front of the queue
		else 
		{
			uint32_t bytesLeft_Back = queue->arraySize - queue->back;
			void *rest_buffPtr = queue->arrayPtr + bytesLeft_Back;
			
			uint32_t bytesLeft_buffArray = buffSize - bytesLeft_Back;
			
			memcpy(queue->arrayPtr + queue->back, buffPtr, bytesLeft_Back);
			memcpy(queue->arrayPtr, rest_buffPtr, bytesLeft_buffArray);
		}
		
		//Update the number of elements in the queue and the number of free bytes
		queue->numElements += itemsWritten;
		queue->freeBytes += buffSize;

		return itemsWritten;
 
	}	
	
	//Case 2:
	//The queue does not have enough room for the array, 
	//no operation will be done, and the function returns 0 to indicate failure.
	else
	{
		//TODO change this so it adds all of the items it can
		return 0;
	}	
	
	//Update the number of elements in the queue and the number of free bytes
	queue->numElements += itemsWritten;
	queue->freeBytes += buffSize;

	//update the back of the queue
	//Case 1: The back does not have to wrap arround to the front of the array
	if(queue->back + itemsWritten < queue->arraySize)
	{
		queue->back += itemsWritten;
	}
	//Case 2: The back needs to wrap around to the front of the array
	else
	{
		//The new value for the back will need to be the same as the number of bytes written to the front of the array
		queue->back = itemsWritten - (queue->arraySize - queue->back);
	}
	
	//Return the number of items written to indicate success
	return itemsWritten;
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
	queue->freeBytes += itemSize;
	
	return 1;
}


//Remove an array of items from the front of the queue
int CQ_DequeueBuffer(CircularQueue* queue, void* buffPtr, int buffSize)
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
	
	//If the queue is already empty, no item could be dequeued,
	//so return 0
	if(queue->numElements <= 0)
	{
		return 0;
 	}
	
	//Check if the queue has more items than what's desired
	uint32_t freeBytes = queue->arraySize - queue->numElements * queue->itemSize
	
	//Case 1: The queue does not have enough items to be taken out,
	//		  take out whatever is left in the queue.
	if (queue->numElements *  < buffSize)
	{
		memccpy(buffPtr, queue->arrayPtr + queue->front, queue->itemSize * queue->numElements);
		return 1;
	}
	
	//Case 2: The queue does have more than what's desired
	uint32_t itemsOut = queue->itemSize * buffSize;
	int numItemsOut = buffSize / 
	memccpy(buffPtr, queue->arrayPtr + queue->front, itemsOut);
	
	//Update the front and decrement numElements
	//Make sure the value for the front doesn't go past the size of the array
	queue->front += itemsOut;
	if (queue->front >= queue->arraySize)
	{
		queue->front = 0;
	}
	queue->numElements -= buffSize;
	
	//Return 1 to indicate success
	return 1;
	
}
