/*
 * SPBPriorityQueue.c
 *
 *  Created on: May 20, 2016
 *      Author: lirongazit
 */
#include "SPBPriorityQueue.h"
#include "SPList.h"
#include <stdbool.h>
#include <stdio.h>

struct sp_bp_queue_t{
	int maxSize;
	SPList qList;
};

SPBPQueue spBPQueueCreate(int maxSize){
	SPBPQueue queue;
	queue.qList=spListCreate();
	queue.maxSize=maxSize;
	return queue;
}

SPBPQueue spBPQueueCopy(SPBPQueue source){
	SPBPQueue queueCopy;
	queueCopy.qList=spListCopy(source.qList);
	queueCopy.maxSize=spBPQueueGetMaxSize(source);
	int size=spListGetSize(queueCopy.qList);
	return queueCopy;
}

void spBPQueueDestroy(SPBPQueue source){
	if(source!=NULL){
		return;
	}
	spListDestroy(source.qList);
}

void spBPQueueClear(SPBPQueue source){
	spListDestroy(source.qList);
	source.qList=spListCreate();
}

int spBPQueueSize(SPBPQueue source){
	return spListGetSize(source.qList);;
}

int spBPQueueGetMaxSize(SPBPQueue source){
	return source.maxSize;
}

SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element){
	if(spBPQueueIsFull(source)){
		if(spListElementGetValue(element)<spListElementGetValue(source.qList.tail.data.value)){
			source.qList.current=source.qList.tail;
			spListRemoveCurrent(source.qList);
		}
		else{
			return SP_BPQUEUE_SUCCESS;
			//////////////????????????????????????????????????????????????????????????
		}
	}
	if(spBPQueueIsEmpty){
		spListInsertFirst(source);
	}
	source.qList.current=source.qList.head;
	while(source.qList.current.data.value<=spListElementGetValue(element)){
		source.qList.current=spListGetNext(source.qList);
	}
	spListInsertBeforeCurrent(source.qList,element);
	return SP_BPQUEUE_SUCCESS;
}

SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source){
	source.qList.current=source.qList.head;
	spListRemoveCurrent(source.qList);
	//?????????????????????????????????????????????????

}

/**
 * Returns a NEW COPY of the element with the lowest value
 *
 * @param source - The source BPQueue
 * @assert source != NULL
 * @return
 * a NEW COPY of the element with the lowest value.
 */
SPListElement spBPQueuePeek(SPBPQueue source);

/**
 * Returns a NEW COPY of the element with the highest value
 *
 * @param source - The source BPQueue
 * @assert source != NULL
 * @return
 * a NEW COPY of the element with the highest value.
 */
SPListElement spBPQueuePeekLast(SPBPQueue source);

/**
 * A getter for the minimum value in the queue
 *
 * @param source - The source BPQueue
 * @assert source != NULL
 * @return
 * The minimum value in the queue
 */
double spBPQueueMinValue(SPBPQueue source);

/**
 * A getter for the maximum value in the queue
 *
 * @param source - The source BPQueue
 * @assert source != NULL
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




