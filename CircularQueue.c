#include "CircularQueue.h"
///
///NOTE: arrayPtr is cast to a (uint8_t*) because pointer arithmetic is not allowed
///on void pointers, and uint8_t has a size of one byte
///

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
	if(queue->back >= queue->arraySize)
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
	memcpy((uint8_t*)queue->arrayPtr + queue->back, itemPtr, queue->itemSize);
	
	//update the back of the queue and increment numElements
	//Make sure the value for the back doesn't go past the size of the array
	queue->back += queue->itemSize;
	if(queue->back >= queue->arraySize)
	{
		queue->back = 0;
	}
	queue->numElements++;	
	queue->freeBytes -= queue->itemSize;
	
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
	if(queue->back >= queue->arraySize)
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
			memcpy((uint8_t*)queue->arrayPtr + queue->back, buffPtr, buffSize);
		}
		
		//Case 2: the back of the queue cannot fit the entire buffer array
		//The function will copy first part of the buffer array to the back of the queue,
		//and copy the rest of the array to the front of the queue
		else 
		{
			uint32_t bytesLeft_Back = queue->arraySize - queue->back;
			void *rest_buffPtr = (uint8_t*)buffPtr + bytesLeft_Back;
			
			uint32_t bytesLeft_buffArray = buffSize - bytesLeft_Back;
			
			memcpy((uint8_t*)queue->arrayPtr + queue->back, buffPtr, bytesLeft_Back);
			memcpy(queue->arrayPtr, rest_buffPtr, bytesLeft_buffArray);
		}
	}	
	
	//Case 2:
	//The queue does not have enough room for the array, 
	//the method would enqueue as many items as space allows.
	else if (queue->freeBytes > 0)
	{
		itemsWritten = queue->freeBytes / queue->itemSize;
		
		//Copy as many items as space allows to the back of the queue
		if (queue->arraySize - queue->back == queue->freeBytes
			|| queue->front - queue->back == queue->freeBytes)
		{
			memcpy((uint8_t*)queue->arrayPtr + queue->back, buffPtr, queue->freeBytes);
		}
		
		else
		{
			uint32_t bytesLeft_Back = queue->arraySize - queue->back;
			void *rest_buffPtr = (uint8_t*)buffPtr + bytesLeft_Back;
			
			uint32_t bytesLeft_buffArray = queue->freeBytes - bytesLeft_Back;
			
			memcpy((uint8_t*)queue->arrayPtr + queue->back, buffPtr, bytesLeft_Back);
			memcpy(queue->arrayPtr, rest_buffPtr, bytesLeft_buffArray);
		}
	}	
	
	//Case 3:
	//When the queue is full, no operation will be done,
	//and return 0 to indicate failure.
	else 
	{
		return 0;
	}
	
	//Update the number of elements in the queue and the number of free bytes
	queue->numElements += itemsWritten;
	queue->freeBytes -= itemsWritten * queue->itemSize; 

	//update the back of the queue
	//Case 1: The back does not have to wrap arround to the front of the array
	if(queue->back + itemsWritten < queue->arraySize)
	{
		queue->back += queue->itemSize * itemsWritten;
	}
	//Case 2: The back needs to wrap around to the front of the array
	else
	{
		//The new value for the back will need to be the same as the number of bytes written to the front of the array
		queue->back = (queue->itemSize * itemsWritten) - (queue->arraySize - queue->back);
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
	if(queue->front >= queue->arraySize)
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
	
	memcpy(itemOut, (uint8_t*)queue->arrayPtr + queue->front, queue->itemSize);
	
	//Update the front and decrement numElements
	//Make sure the value for the front doesn't go past the size of the array
	queue->front += queue->itemSize;
	if(queue->front >= queue->arraySize)
	{
		queue->front = 0;
	}
	queue->numElements--;
	queue->freeBytes += queue->itemSize;
	
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
	if(queue->front >= queue->arraySize)
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
	
	//Declare a return value to indicate the number of items dequeued
	int numItemsOut;
	

	//Case 1: The queue does not have enough items to be taken out,
	//		  take out whatever is left in the queue.
	if (queue->arraySize - queue->freeBytes < buffSize)
	{
		uint32_t bytesOut = queue->arraySize - queue->freeBytes;
		
		//Case 1a: If the data being dequeued does not wrap around from the 
		//		   back of the queue to the front of the queue
		if (queue->arraySize - queue->front >= bytesOut)
		{
			memcpy(buffPtr, (uint8_t*)queue->arrayPtr + queue->front, bytesOut);
		}
		
		//Case 1b: If the data being dequeued wraps around from the 
		//		   back of the queue to the front of the queue
		
		//The dequeueBuffer in this case will dequeue all elements in the queue
		else 
		{
			uint32_t bytesAfterFront = queue->arraySize - queue->front;
			uint32_t bytesBeforeBack = queue->back;	
			
			memcpy(buffPtr, (uint8_t*)queue->arrayPtr + queue->front, bytesAfterFront);
			memcpy((uint8_t*)buffPtr + bytesAfterFront, queue->arrayPtr, bytesBeforeBack);
		}

		//Update the data members of the queue to indicate changes made 
		//by the dequeue action.
		queue->front = queue->back;
		numItemsOut = queue->numElements;
		queue->numElements = 0;
		queue->freeBytes = queue->arraySize;
		return numItemsOut;
	}
	
	//Case 2: if the queue has more than enough or just enough items to be dequeued
	
	//Update the front and decrement numElements
	//Make sure the value for the front doesn't go past the size of the array
	else
	{
		//Case 2a: If the data being dequeued does not wrap around from the 
		//		   back of the queue to the front of the queue
		uint32_t bytesOut = buffSize;
		
		if (queue->arraySize - queue->front >= buffSize)
		{
			
			memcpy(buffPtr, (uint8_t*)queue->arrayPtr + queue->front, buffSize);
			
			//Update the data members of the queue to indicate changes made 
			//by the dequeue action.
			
			
			queue->front += buffSize;
			
			//If the front reaches the end of the array, 
			//wrap around the front by setting the front to 0
			if (queue->front >= queue->arraySize) 
			{
				queue->front = 0;
			}
		}
		
		//Case 2b: If the data being dequeued wraps around from the 
		//		   back of the queue to the front of the queue
		
		//TODO: implement this
		else
		{
			uint32_t bytesAfterFront = queue->arraySize - queue->front;
			uint32_t bytesBeforeBack = buffSize - bytesAfterFront;
			
			memcpy(buffPtr, (uint8_t*)queue->arrayPtr + queue->front, bytesAfterFront);
			memcpy((uint8_t*)buffPtr + bytesAfterFront, queue->arrayPtr, bytesBeforeBack);
			
			//Update the data members of the queue to indicate changes made 
			//by the dequeue action.
			queue->front = bytesBeforeBack;	
		}

		numItemsOut = buffSize / queue->itemSize;
		queue->numElements -= numItemsOut;
		queue->freeBytes += bytesOut;
		return numItemsOut;
	}
}
