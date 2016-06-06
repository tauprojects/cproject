#ifndef SPBPRIORITYQUEUE_H_
#define SPBPRIORITYQUEUE_H_
#include "SPListElement.h"
#include <stdbool.h>
/**
 * SP Bounded Priority Queue summary
 * Implements a Bounded Priority Queue with an upper bound on the number of sorted elements.
 * If the queue is not full, added elements are always added.
 * If the queue is full and the added element is smaller(value) than the greatest(value) element in the queue,
 * the greatest element is removed and the new element is added.
 * If the queue is full and the added element is greater(value) than the greatest(value) element in the queue,
 * the new element is not added.
 * If the queue is full and the added element have an equal value to the greatest element value in the queue,
 * the element with the smallest index will be added/remain in queue.
 *
 * Queue elements have an int index and a double value.
 *
 * The following functions are supported:
 * spBPQueueCreate        	- Creates an empty queue with a given maximum capacity
 * spBPQueueCopy			- Creates a copy of a given queue
 * spBPQueueDestroy 		- Frees all memory allocation associated with the queue
 * spBPQueueClear			- Removes all the elements in the queue
 * spBPQueueSize			- Returns the number of elements in the queue
 * spBPQueueGetMaxSize		- Returns the maximum capacity of the queue
 * spBPQueueEnqueue			- Inserts a NEW COPY element to the queue
 * spBPQueueDequeue			- Removes the element with the lowest value
 * spBPQueuePeek			- Returns a NEW COPY of the element with the lowest value
 * spBPQueuePeekLast		- Returns a NEW COPY of the element with the highest value
 * spBPQueueMinValue		- Returns the minimum value in the queue
 * spBPQueueMaxValue		- Returns the maximum value in the queue
 * spBPQueueIsEmpty			- Returns true if the queue is empty
 * spBPQueueIsFull			- Returns true if the queue is full
 */

/** type used to define Bounded priority queue **/
typedef struct sp_bp_queue_t* SPBPQueue;

/** type for error reporting **/
typedef enum sp_bp_queue_msg_t {
	SP_BPQUEUE_OUT_OF_MEMORY,
	SP_BPQUEUE_FULL,
	SP_BPQUEUE_EMPTY,
	SP_BPQUEUE_INVALID_ARGUMENT,
	SP_BPQUEUE_SUCCESS
} SP_BPQUEUE_MSG;

/**
 * Creates a new BPQueue with the specific maximum capacity
 *
 * @param maxSize The maximum capacity of BPQueue
 * @return
 * NULL in case of memory allocation fails.
 * A new empty BPQueue with the corresponding max capacity.
 */
SPBPQueue spBPQueueCreate(int maxSize);

/**
 * Creates a copy of target BPQueue.
 *
 * The new copy will contain the same maximum capacity and the same
 * elements as in the target BPQueue.
 *
 * @param source - The source queue
 * @return
 * NULL in case memory allocation occurs
 * NULL in case source is NULL
 * Others a copy of source is returned.
 */
SPBPQueue spBPQueueCopy(SPBPQueue source);

/**
 * Destroys an BPQueue.
 * All memory allocation associated with the BPQueue will be freed
 *
 * @param source - the target BPQueue which will be freed.
 * 			   if source is NULL, then nothing is done
 */
void spBPQueueDestroy(SPBPQueue source);

/**
 * Clear an BPQueue
 * Removes all the elements in the queue
 *
 * @param source - the target BPQueue which his elements will be removed.
 * 			   if source is NULL, then nothing is done
 */
void spBPQueueClear(SPBPQueue source);

/**
 * A getter for the size of the BPQueue
 *
 * @param source - The source BPQueue
 * @return
 * The size of the BPQueue
 * -1 in case source is NULL
 */
int spBPQueueSize(SPBPQueue source);

/**
 * A getter for the  max size of the BPQueue
 *
 * @param source - The source BPQueue
 * @return
 * The max size of the BPQueue
 * -1 in case source is NULL
 */
int spBPQueueGetMaxSize(SPBPQueue source);

/**
 * Inserts a NEW COPY (must be allocated) element to the queue
 * if the BPQueue is in full capacity -
 * 		the new element will be insert iff element(value)<source(lastElementValue)
 * 		and remove the max element.
 *
 * @param source - The source BPQueue
 * @param element - The new element that will be insert to the source queue.
 * @return
 * 	SP_BPQUEUE_OUT_OF_MEMORY in case memory allocation fails
 * 	SP_BPQUEUE_FULL in case of full queue
 * 	SP_BPQUEUE_INVALID_ARGUMENT in case source==NULL
 *  SP_ELEMENT_SUCCESS otherwise
 */
SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element);

/**
 * Removes the element with the lowest value
 *
 * @param source - The source BPQueue
 * @return
 * 	SP_BPQUEUE_OUT_OF_MEMORY in case memory allocation fails
 * 	SP_BPQUEUE_EMPTY in case of empty queue
 * 	SP_BPQUEUE_INVALID_ARGUMENT in case source==NULL
 *  SP_ELEMENT_SUCCESS otherwise
 */
SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source);

/**
 * Returns a NEW COPY of the element with the lowest value
 *
 * @param source - The source BPQueue
 * @return
 * a NEW COPY of the element with the lowest value.
 * NULL in case source is NULL
 */
SPListElement spBPQueuePeek(SPBPQueue source);

/**
 * Returns a NEW COPY of the element with the highest value
 *
 * @param source - The source BPQueue
 * @return
 * a NEW COPY of the element with the highest value.
 * NULL in case source is NULL
 */
SPListElement spBPQueuePeekLast(SPBPQueue source);

/**
 * A getter for the minimum value in the queue
 *
 * @pre source != NULL
 * @param source - The source BPQueue
 * @return
 * The minimum value in the queue
 *
 */
double spBPQueueMinValue(SPBPQueue source);

/**
 * A getter for the maximum value in the queue
 *
 * @pre source != NULL
 * @param source - The source BPQueue
 * @return
 * The maximum value in the queue
 */
double spBPQueueMaxValue(SPBPQueue source);

/**
 * Checks if the queue is empty
 *
 * @param source - The source BPQueue
 * @assert source != NULL
 * @return
 * True if the queue is empty
 * 		False otherwise
 */
bool spBPQueueIsEmpty(SPBPQueue source);

/**
 * Checks if the queue is full
 *
 * @param source - The source BPQueue
 * @assert source != NULL
 * @return
 * True if the queue is full
 * 		False otherwise
 */
bool spBPQueueIsFull(SPBPQueue source);

#endif
