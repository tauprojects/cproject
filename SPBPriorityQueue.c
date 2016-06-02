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
#include <assert.h>
#include <stdlib.h>
#define MSG_MACRO { if(msg==SP_LIST_OUT_OF_MEMORY){return SP_BPQUEUE_OUT_OF_MEMORY;} else if(msg==SP_LIST_SUCCESS){ return SP_BPQUEUE_SUCCESS; }}

struct sp_bp_queue_t{
	int maxSize;
	SPList qList;
};

SPBPQueue spBPQueueCreate(int maxSize){
	if(maxSize<0){ //Case of invalid argument
		return NULL;
	}
	SPBPQueue queue=(SPBPQueue) malloc(sizeof(*queue));
	if(queue==NULL){   //Allocation fail
		return NULL;
	}
	queue->qList=spListCreate();  //Create an empty spList
	queue->maxSize=maxSize;
	return queue;
}

SPBPQueue spBPQueueCopy(SPBPQueue source){
	if(source==NULL){  //Case of NULL ocject
			return NULL;
	}
	SPBPQueue queueCopy=(SPBPQueue) malloc(sizeof(*queueCopy));
	if(queueCopy==NULL){   //Allocation fail
			return NULL;
	}
	queueCopy->qList=spListCopy(source->qList);
	queueCopy->maxSize=spBPQueueGetMaxSize(source);
	return queueCopy;
}

void spBPQueueDestroy(SPBPQueue source){
	if(source==NULL){  //Case of NULL object
		return;
	}
	spListDestroy(source->qList);
	free(source);  //Free the source
}

void spBPQueueClear(SPBPQueue source){
	if(source==NULL){  //Case of NULL object
			return;
	}
	spListClear(source->qList);  //Clear qList
}

int spBPQueueSize(SPBPQueue source){
	if(source==NULL){  //Case of NULL object
		return -1;
	}
	return spListGetSize(source->qList);
}

int spBPQueueGetMaxSize(SPBPQueue source){
	if(source==NULL){  //Case of NULL object
		return -1;
	}
	return source->maxSize;
}

void spBPQsetTailAsCurrent(SPList qlist){
	spListGetFirst(qlist); //Function returning unused element, current=head
	for(int i=0; i<spListGetSize(qlist)-1;i++){
		spListGetNext(qlist);	    //Function returning unused element.
	}
	//current=tail
}

SP_BPQUEUE_MSG spBPQueueEnqueue(SPBPQueue source, SPListElement element){
	if(source==NULL || element==NULL){  //Invalid arg. case
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	if(spBPQueueIsFull(source)){     //Case of Full BPQ
		//Case of value(element)>maxVal&Full BPQ
		if(spListElementGetValue(element)>spBPQueueMaxValue(source)){
			return SP_BPQUEUE_FULL;    //Do nothing
		}
		else if(spListElementGetValue(element)==spBPQueueMaxValue(source)){
			spBPQsetTailAsCurrent(source->qList);    //current=tail
			//Case of identical values, sort with the index
			if(spListElementGetIndex(element) < spListElementGetIndex(spListGetCurrent(source->qList))){
				spListRemoveCurrent(source->qList);
			}
			else{
				return SP_BPQUEUE_FULL;
			}
		}
		else{   //Case of value(element) < maxVal & Full BPQ
			spBPQsetTailAsCurrent(source->qList);
			spListRemoveCurrent(source->qList);
		}
	}
	SP_LIST_MSG msg;
	//Insert empty BPQ and insert minValue Case.
	if(spBPQueueIsEmpty(source) || (spListElementGetValue(element)<spBPQueueMinValue(source))){
		msg=spListInsertFirst(source->qList,element);
		MSG_MACRO
	}
	if(spListElementGetValue(element)>spBPQueueMaxValue(source)){  //Insert maxVal case
		msg=spListInsertLast(source->qList,element);
		MSG_MACRO
	}
	spListGetFirst(source->qList);
	//search new element location
	while(spListElementGetValue(spListGetCurrent(source->qList))<spListElementGetValue(element)){
		spListGetNext(source->qList);
	}
	//Case of identical values, sort with the index
	if(spListElementGetValue(spListGetCurrent(source->qList))==spListElementGetValue(element)){
		if(spListElementGetIndex(element) < spListElementGetIndex(spListGetCurrent(source->qList))){
			msg=spListInsertBeforeCurrent(source->qList,element);
		}
		else{
			msg=spListInsertAfterCurrent(source->qList,element);
		}
	}
	else{
		msg=spListInsertBeforeCurrent(source->qList,element);
	}
	MSG_MACRO
	return SP_BPQUEUE_OUT_OF_MEMORY;
}

SP_BPQUEUE_MSG spBPQueueDequeue(SPBPQueue source){
	if(source==NULL){  //Invalid Argument case
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	if(spBPQueueIsEmpty(source)){   //Empty BPQ case
		return SP_BPQUEUE_EMPTY;
	}
	spListGetFirst(source->qList); //current=head
	SP_LIST_MSG msg=spListRemoveCurrent(source->qList);
	if(msg==SP_LIST_NULL_ARGUMENT){  //fit the Msg to BPQ msg.
		return SP_BPQUEUE_INVALID_ARGUMENT;
	}
	else if(msg==SP_LIST_INVALID_CURRENT){
		return SP_BPQUEUE_EMPTY;
	}
	else{
		return SP_BPQUEUE_SUCCESS;
	}
}

SPListElement spBPQueuePeek(SPBPQueue source){
	if(source==NULL){  //Case of NULL object
		return NULL;
	}
	return (spListElementCopy(spListGetFirst(source->qList)));
}

SPListElement spBPQueuePeekLast(SPBPQueue source){
	if(source==NULL){  //Case of NULL object
		return NULL;
	}
	spBPQsetTailAsCurrent(source->qList);  //current=last
	return (spListElementCopy(spListGetCurrent(source->qList)));
}

double spBPQueueMinValue(SPBPQueue source){
	return (spListElementGetValue(spListGetFirst(source->qList)));
}

double spBPQueueMaxValue(SPBPQueue source){
	spBPQsetTailAsCurrent(source->qList);  //current=tail
	return (spListElementGetValue(spListGetCurrent(source->qList)));
}

bool spBPQueueIsEmpty(SPBPQueue source){
	assert(source!=NULL);  //Case of NULL object
	int size=spBPQueueSize(source);
	if (size==0){
		return true;
	}
	else{
		return false;
	}
}

bool spBPQueueIsFull(SPBPQueue source){
	assert(source!=NULL); //Case of NULL object
	return (spBPQueueSize(source)==spBPQueueGetMaxSize(source));
}

SPList spBPQueueGetList(SPBPQueue source){
	return source->qList;  //return qList
}




