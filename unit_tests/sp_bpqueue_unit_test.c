#include "../SPPoint.h"
#include "../SPBPriorityQueue.h"
#include "unit_test_util.h"
#include <stdbool.h>
#include "../SPList.h"
#include <stdarg.h>

SPBPQueue quickBPQ(int maxSize,int size, ...) {   //Helper function-quickBPQ
	va_list items;
	SPBPQueue queue = spBPQueueCreate(maxSize);
	va_start(items, size);
	for (int i = 0; i < size; i++) { //Enqueue each element
		spBPQueueEnqueue(queue, va_arg(items, SPListElement));
	}
	va_end(items);
	return queue;
}

bool bpqueueCreateTest() {
	int maxSize=5;
	SPBPQueue p = spBPQueueCreate(maxSize);
	ASSERT_TRUE(p!=NULL); //checking that we create un-NULL object
	spBPQueueDestroy(p);
	return true;
}

bool bpqueueBasicCopyTest() {
	int maxSize=5;
	int listSize = 4;
	SPBPQueue p = spBPQueueCreate(maxSize); //craete a new BPQ
	SPBPQueue q = spBPQueueCopy(p);  //copy empty BPQ
	ASSERT_TRUE(spBPQueueCopy(NULL)==NULL);  //Checking NULL case
	ASSERT_TRUE(spBPQueueGetMaxSize(p) == spBPQueueGetMaxSize(q));  //Checking copy element's field
	ASSERT_TRUE(spBPQueueGetMaxSize(p) == maxSize);
	ASSERT_TRUE(spBPQueueSize(p) == spBPQueueSize(q));
	ASSERT_TRUE(spBPQueueSize(p) == 0);
	SPListElement e1 = spListElementCreate(1, 1.0);  //craete new elements
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	SPBPQueue p2 = quickBPQ(maxSize,listSize, e1, e2, e3, e4);  //create new BPQ
	SPBPQueue q2 = spBPQueueCopy(p2); //Copy BPQ with elements
	ASSERT_TRUE(spBPQueueSize(p2) == spBPQueueSize(q2));
	ASSERT_TRUE(spBPQueueSize(q2) == listSize);
	SPListElement tempElemntP;  //Initialize two tempElemnts before the loop
	SPListElement tempElementQ;
	for(int i=0;i<listSize;i++){
		tempElemntP = spBPQueuePeek(p2);
		tempElementQ = spBPQueuePeek(q2);
		ASSERT_TRUE(spListElementCompare(tempElemntP,tempElementQ)==0);
		spListElementDestroy(tempElemntP);  //Destroting every iteration
		spListElementDestroy(tempElementQ);
		spBPQueueDequeue(p2);
		spBPQueueDequeue(q2);
	}
	spListElementDestroy(e1);  //Memory free
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	spBPQueueDestroy(p);
	spBPQueueDestroy(q);
	spBPQueueDestroy(p2);
	spBPQueueDestroy(q2);
	return true;
}

bool bpqueueDestroyTest() {
	spBPQueueDestroy(NULL);  //Destroy NULL element
	return true;
}

bool bpqueueClearTest() {
	SPListElement e1 = spListElementCreate(1, 1.0);  //create new elements for testing
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	int maxSize=5;
	int listSize = 4;
	SPBPQueue p = quickBPQ(maxSize,listSize, e1, e2, e3, e4);  //new BPQ
	spBPQueueClear(p);  //Clear test
	ASSERT_TRUE(spBPQueueSize(p) == 0); // assert that BPQ clear succeed
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);   //memory free aloocation
	spListElementDestroy(e4);
	spBPQueueDestroy(p);
	return true;
}

bool bpqueueSizeTest() {
	ASSERT_TRUE(spBPQueueSize(NULL)==-1);   //Checking NULL case
	SPListElement e1 = spListElementCreate(1, 1.0);  //Create new elements for testing
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	int listSize=4;
	int maxSize=5;
	SPBPQueue q = spBPQueueCreate(maxSize);
	ASSERT_TRUE(spBPQueueSize(q) == 0);  //Checking size of empty BPQ
	SPBPQueue p = quickBPQ(maxSize,listSize, e1, e2, e3, e4);
	ASSERT_TRUE(spBPQueueSize(p) == listSize); //Checking size of BPQ with elements
	spListElementDestroy(e1);   //Memory free allocation
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	spBPQueueDestroy(p);
	spBPQueueDestroy(q);
	return true;
}

bool bpqueueMaxSizeTest() {
	ASSERT_TRUE(spBPQueueGetMaxSize(NULL)==-1);  //Checking NULL case
	int maxSize=5;
	SPBPQueue p = spBPQueueCreate(maxSize);
	ASSERT_TRUE(spBPQueueGetMaxSize(p) == maxSize);
	spBPQueueDestroy(p);  //Memory free allocation
	return true;
}

bool bpqueueEnqueueTest() {
	SPListElement e1 = spListElementCreate(5, 12.0);  //Create new elements for testing
	SPListElement e2 = spListElementCreate(2, 8.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 15.0);
	SPListElement e5 = spListElementCreate(1, 8.0);
	SPListElement e6 = spListElementCreate(1, 12.0);
	int maxSize=5;
	SPBPQueue p = spBPQueueCreate(maxSize);
	ASSERT_TRUE(spBPQueueEnqueue(p,e1)==SP_BPQUEUE_SUCCESS);
	SPListElement tempElement = spBPQueuePeek(p);
	ASSERT_TRUE(spListElementCompare(tempElement,e1)==0);//Checking elements after insertion
	spListElementDestroy(tempElement);  //free memory allocation
	ASSERT_TRUE(spBPQueueEnqueue(p,e2)==SP_BPQUEUE_SUCCESS);
	tempElement = spBPQueuePeek(p);
	ASSERT_TRUE(spListElementCompare(tempElement,e2)==0);//Checking elements after insertion
	spListElementDestroy(tempElement);
	ASSERT_TRUE(spBPQueueEnqueue(p,e3)==SP_BPQUEUE_SUCCESS);
	tempElement = spBPQueuePeek(p);
	ASSERT_TRUE(spListElementCompare(tempElement,e3)==0); //Checking elements after insertion
	spListElementDestroy(tempElement);
	ASSERT_TRUE(spBPQueueEnqueue(p,e4)==SP_BPQUEUE_SUCCESS);
	tempElement = spBPQueuePeek(p);
	ASSERT_TRUE(spListElementCompare(tempElement,e3)==0); //Checking elements after insertion
	spListElementDestroy(tempElement);
	ASSERT_TRUE(spBPQueueEnqueue(p,NULL)==SP_BPQUEUE_INVALID_ARGUMENT);  //Checking invalid arguments cases
	ASSERT_TRUE(spBPQueueEnqueue(NULL,e1)==SP_BPQUEUE_INVALID_ARGUMENT);
	ASSERT_TRUE(spBPQueueEnqueue(p,e1)==SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueEnqueue(p,e2)==SP_BPQUEUE_SUCCESS);  //Checking enqueue to full BPQ
	tempElement = spBPQueuePeekLast(p);
	ASSERT_TRUE(spListElementCompare(tempElement,e1)==0);  //Checking elements after insertion
	spListElementDestroy(tempElement);
	tempElement = spBPQueuePeek(p);
	ASSERT_TRUE(spListElementCompare(tempElement,e3)==0);
	spListElementDestroy(tempElement);
	ASSERT_TRUE(spBPQueueEnqueue(p,e4)==SP_BPQUEUE_FULL);  //Case of (elementValue > maxValue && FULL BPQ)
	tempElement = spBPQueuePeekLast(p);
	ASSERT_TRUE(spListElementCompare(tempElement,e1)==0);//Checking elements after insertion
	spListElementDestroy(tempElement);
	tempElement = spBPQueuePeek(p);
	ASSERT_TRUE(spListElementCompare(tempElement,e3)==0);
	spListElementDestroy(tempElement);
	ASSERT_TRUE(spBPQueueEnqueue(p,e5)==SP_BPQUEUE_SUCCESS);  //Checking sort by index(identical value)
	tempElement = spBPQueuePeekLast(p);
	ASSERT_TRUE(spListElementCompare(tempElement,e1)==0);//Checking sort by index(identical value)
	spListElementDestroy(tempElement);
	tempElement = spBPQueuePeek(p);
	ASSERT_TRUE(spListElementCompare(tempElement,e3)==0);
	spListElementDestroy(tempElement);
	ASSERT_TRUE(spBPQueueEnqueue(p,e6)==SP_BPQUEUE_SUCCESS); //Checking sort by index(identical maxValue)
	tempElement = spBPQueuePeekLast(p);
	ASSERT_TRUE(spListElementCompare(tempElement,e6)==0);//Checking sort by index(identical value)
	spListElementDestroy(tempElement);
	tempElement = spBPQueuePeek(p);
	ASSERT_TRUE(spListElementCompare(tempElement,e3)==0);
	spListElementDestroy(tempElement);
	ASSERT_TRUE(spBPQueueEnqueue(p,e5)==SP_BPQUEUE_SUCCESS);
	tempElement = spBPQueuePeekLast(p);
	ASSERT_TRUE(spListElementCompare(tempElement,e2)==0);//Checking sort by index(identical value)
	spListElementDestroy(tempElement);
	tempElement = spBPQueuePeek(p);
	ASSERT_TRUE(spListElementCompare(tempElement,e3)==0);
	spListElementDestroy(tempElement);
	spListElementDestroy(e1);  //Free memory allocation
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	spListElementDestroy(e5);
	spListElementDestroy(e6);
	spBPQueueDestroy(p);
	return true;
}

bool bpqueueDqueueTest() {
	SPListElement e1 = spListElementCreate(1, 12.0);//Create new elements for testing
	SPListElement e2 = spListElementCreate(2, 8.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 10.0);
	SPListElement eMin = spListElementCreate(5, 1.0);
	int maxSize=5;
	SPBPQueue p = spBPQueueCreate(maxSize);
	ASSERT_TRUE(spBPQueueDequeue(NULL)==SP_BPQUEUE_INVALID_ARGUMENT);  //Checking invalid arguments case
	ASSERT_TRUE(spBPQueueDequeue(p)==SP_BPQUEUE_EMPTY);  //Checking Empty BPQ case
	ASSERT_TRUE(spBPQueueEnqueue(p,e1)==SP_BPQUEUE_SUCCESS);  //insertion of elements
	ASSERT_TRUE(spBPQueueEnqueue(p,eMin)==SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueEnqueue(p,e2)==SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueEnqueue(p,e3)==SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueEnqueue(p,e4)==SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueDequeue(p)==SP_BPQUEUE_SUCCESS);
	SPListElement tempElement = spBPQueuePeek(p);
	ASSERT_TRUE(spListElementCompare(tempElement,e3)==0);  //Checking elements after dequeue
	spListElementDestroy(tempElement);
	ASSERT_TRUE(spBPQueueDequeue(p)==SP_BPQUEUE_SUCCESS);
	tempElement = spBPQueuePeek(p);
	ASSERT_TRUE(spListElementCompare(tempElement,e2)==0);//Checking elements after dequeue
	spListElementDestroy(tempElement);
	ASSERT_TRUE(spBPQueueDequeue(p)==SP_BPQUEUE_SUCCESS);
	tempElement = spBPQueuePeek(p);
	ASSERT_TRUE(spListElementCompare(tempElement,e4)==0); //Checking elements after dequeue
	spListElementDestroy(tempElement);
	ASSERT_TRUE(spBPQueueDequeue(p)==SP_BPQUEUE_SUCCESS);
	tempElement = spBPQueuePeek(p);
	ASSERT_TRUE(spListElementCompare(tempElement,e1)==0); //Checking elements after dequeue
	spListElementDestroy(tempElement);
	ASSERT_TRUE(spBPQueueDequeue(p)==SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueDequeue(p)==SP_BPQUEUE_EMPTY); //Checking Empty BPQ
	spListElementDestroy(e1); //Memory free allocation
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	spListElementDestroy(eMin);
	spBPQueueDestroy(p);
	return true;
}

bool bpqueuePeekTest() {
	ASSERT_TRUE(spBPQueuePeek(NULL)==NULL);  //Checking NULL object case
	SPListElement e1 = spListElementCreate(1, 12.0);//Create new elements for testing
	SPListElement e2 = spListElementCreate(2, 8.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 10.0);
	SPListElement e5 = spListElementCreate(5, 1.0);
	int maxSize=7;
	SPBPQueue p = spBPQueueCreate(maxSize);  //Create BPQ with elements for testing
	spBPQueueEnqueue(p,e1);
	spBPQueueEnqueue(p,e5);
	spBPQueueEnqueue(p,e2);
	spBPQueueEnqueue(p,e3);
	spBPQueueEnqueue(p,e4);
	SPListElement tempElement = spBPQueuePeek(p);
	ASSERT_TRUE(spListElementCompare(tempElement,e5)==0);  //checking Peek element
	ASSERT_FALSE(spListElementCompare(tempElement,e4)==0);
	spListElementDestroy(tempElement);  //Free memory allocation
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	spListElementDestroy(e5);
	spBPQueueDestroy(p);
	return true;
}

bool bpqueuePeekLastTest() {
	ASSERT_TRUE(spBPQueuePeekLast(NULL)==NULL);  //Checking NULL object case
	SPListElement e1 = spListElementCreate(1, 12.0);//Create new elements for testing
	SPListElement e2 = spListElementCreate(2, 8.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 10.0);
	SPListElement e5 = spListElementCreate(5, 1.0);
	int maxSize=7;
	SPBPQueue p = spBPQueueCreate(maxSize);  //Create BPQ with elements for testing
	spBPQueueEnqueue(p,e1);
	spBPQueueEnqueue(p,e5);
	spBPQueueEnqueue(p,e2);
	spBPQueueEnqueue(p,e3);
	spBPQueueEnqueue(p,e4);
	SPListElement tempElement = spBPQueuePeekLast(p);
	ASSERT_TRUE(spListElementCompare(tempElement,e1)==0); //Checking peekLast
	ASSERT_FALSE(spListElementCompare(tempElement,e5)==0);
	spListElementDestroy(tempElement);  //free memory allocation
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	spListElementDestroy(e5);
	spBPQueueDestroy(p);
	return true;
}

bool bpqueueMinValueTest() {
	SPListElement e1 = spListElementCreate(1, 12.0);//Create new elements for testing
	SPListElement e2 = spListElementCreate(2, 8.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 10.0);
	SPListElement e5 = spListElementCreate(5, 1.0);
	int maxSize=7;
	SPBPQueue p = spBPQueueCreate(maxSize);  //Create BPQ with elements for testing
	spBPQueueEnqueue(p,e1);
	spBPQueueEnqueue(p,e5);
	spBPQueueEnqueue(p,e2);
	spBPQueueEnqueue(p,e3);
	spBPQueueEnqueue(p,e4);
	ASSERT_TRUE(spBPQueueMinValue(p)==spListElementGetValue(e5));//Checking minValue element
	ASSERT_FALSE(spBPQueueMinValue(p)==spListElementGetValue(e4));
	spListElementDestroy(e1);//free memory allocation
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	spListElementDestroy(e5);
	spBPQueueDestroy(p);
	return true;
}

bool bpqueueMaxValueTest() {
	SPListElement e1 = spListElementCreate(1, 12.0);//Create new elements for testing
	SPListElement e2 = spListElementCreate(2, 8.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 10.0);
	SPListElement e5 = spListElementCreate(5, 1.0);
	int maxSize=7;
	SPBPQueue p = spBPQueueCreate(maxSize);  //Create BPQ with elements for testing
	spBPQueueEnqueue(p,e1);
	spBPQueueEnqueue(p,e5);
	spBPQueueEnqueue(p,e2);
	spBPQueueEnqueue(p,e3);
	spBPQueueEnqueue(p,e4);
	ASSERT_TRUE(spBPQueueMaxValue(p)==spListElementGetValue(e1));//Checking maxValue element
	ASSERT_FALSE(spBPQueueMaxValue(p)==spListElementGetValue(e2));
	spListElementDestroy(e1);//free memory allocation
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	spListElementDestroy(e5);
	spBPQueueDestroy(p);
	return true;
}

bool bpqueueIsEmptyTest() {
	SPListElement e1 = spListElementCreate(1, 12.0);//Create new element for testing
	int maxSize=5;
	SPBPQueue p = spBPQueueCreate(maxSize);
	ASSERT_TRUE(spBPQueueIsEmpty(p)); //Checking empty BPQ
	spBPQueueEnqueue(p,e1);
	ASSERT_FALSE(spBPQueueIsEmpty(p)); //Checking un-Empty BPQ
	spBPQueueDestroy(p);//free memory allocation
	spListElementDestroy(e1);
	return true;
}

bool bpqueueIsFullTest() {
	SPListElement e1 = spListElementCreate(1, 12.0);//Create new elements for testing
	SPListElement e2 = spListElementCreate(2, 8.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 10.0);
	SPListElement e5 = spListElementCreate(5, 1.0);
	int maxSize=5;
	SPBPQueue p = spBPQueueCreate(maxSize);  //Create BPQ with elements for testing
	ASSERT_FALSE(spBPQueueIsFull(p));//Checking un-Full BPQ
	spBPQueueEnqueue(p,e1);
	spBPQueueEnqueue(p,e5);
	spBPQueueEnqueue(p,e2);
	ASSERT_FALSE(spBPQueueIsFull(p)); //Checking un-Full BPQ
	spBPQueueEnqueue(p,e3);
	spBPQueueEnqueue(p,e4);
	ASSERT_TRUE(spBPQueueIsFull(p));//Checking full BPQ
	spListElementDestroy(e1);//free memory allocation
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	spListElementDestroy(e5);
	spBPQueueDestroy(p);
	return true;
}

int main() {
	RUN_TEST(bpqueueCreateTest);
	RUN_TEST(bpqueueBasicCopyTest);
	RUN_TEST(bpqueueDestroyTest);
	RUN_TEST(bpqueueClearTest);
	RUN_TEST(bpqueueSizeTest);
	RUN_TEST(bpqueueMaxSizeTest);
	RUN_TEST(bpqueueEnqueueTest);
	RUN_TEST(bpqueueDqueueTest);
	RUN_TEST(bpqueuePeekTest);
	RUN_TEST(bpqueuePeekLastTest);
	RUN_TEST(bpqueueMinValueTest);
	RUN_TEST(bpqueueMaxValueTest);
	RUN_TEST(bpqueueIsEmptyTest);
	RUN_TEST(bpqueueIsFullTest);
	return 0;
}
