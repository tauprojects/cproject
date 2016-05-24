#include "../SPPoint.h"
#include "../SPBPriorityQueue.c"
#include "unit_test_util.h"
#include <stdbool.h>
#include "../SPListElement.h"
#include "../SPList.h"
#include <stdarg.h>


static SPList quickList(int size, ...) {
	va_list items;
	SPList list = spListCreate();
	va_start(items, size);
	for (int i = 0; i < size; i++) {
		spListInsertLast(list, va_arg(items, SPListElement));
	}
	va_end(items);
	return list;
}

bool bpqueueCreateTest() {
	int maxSize=5;
	SPBPQueue p = spBPQueueCreate(maxSize);
	ASSERT_TRUE(p!=NULL);
	spBPQueueDestroy(p);
	return true;
}

bool bpqueueBasicCopyTest() {
	ASSERT_TRUE(spBPQueueCopy(NULL) == NULL);
	int maxSize=5;
	SPBPQueue p = spBPQueueCreate(maxSize);
	SPBPQueue q = spBPQueueCopy(p);
	ASSERT_TRUE(spBPQueueGetMaxSize(p) == spBPQueueGetMaxSize(q));
	ASSERT_TRUE(spBPQueueGetMaxSize(p) == maxSize);
	ASSERT_TRUE(spBPQueueSize(p) == spBPQueueSize(q));
	ASSERT_TRUE(spBPQueueSize(p) == 0);
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	SPList list2 = quickList(4, e1, e2, e3, e4);
	SPBPQueue p2=spBPQueueCreate(maxSize);
	p2->qList=list2;
	SPBPQueue q2 = spBPQueueCopy(p2);
	ASSERT_TRUE(spBPQueueSize(p2) == spBPQueueSize(q2));
	ASSERT_TRUE(spBPQueueSize(q2) == spListGetSize(list2));
	p2->qList->current=p2->qList->head;
	q2->qList->current=q2->qList->head;
	for(int i=0;i<spListGetSize(list2);i++){
		ASSERT_TRUE(spListElementCompare(q2->qList->current->data,p2->qList->current->data));
		spListGetNext(q2->qList);
		spListGetNext(p2->qList);
	}
	spListDestroy(list2);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	spBPQueueDestroy(p);
	spBPQueueDestroy(q);
	spBPQueueDestroy(p2);
	return true;
}

bool bpqueueDestroyTest() {
	spBPQueueDestroy(NULL);
	return true;
}

bool bpqueueClearTest() {
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	SPList list2 = quickList(4, e1, e2, e3, e4);
	int maxSize=5;
	SPBPQueue p = spBPQueueCreate(maxSize);
	ASSERT_TRUE(spBPQueueSize(p) == 0);
	spBPQueueClear(p);
	ASSERT_TRUE(spBPQueueSize(p) == 0);
	p->qList=list2;
	SPBPQueue q = spBPQueueCopy(p);
	spBPQueueClear(q);
	ASSERT_TRUE(spBPQueueSize(q) == 0);
	ASSERT_TRUE(spBPQueueSize(p) == spListGetSize(list2));
	spListDestroy(list2);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	spBPQueueDestroy(p);
	spBPQueueDestroy(q);
	return true;
}

bool bpqueueSizeTest() {
	SPListElement e1 = spListElementCreate(1, 1.0);
	SPListElement e2 = spListElementCreate(2, 2.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 4.0);
	SPList list2 = quickList(4, e1, e2, e3, e4);
	int maxSize=5;
	SPBPQueue p = spBPQueueCreate(maxSize);
	ASSERT_TRUE(spBPQueueSize(p) == 0);
	p->qList=list2;
	ASSERT_TRUE(spBPQueueSize(p) == spListGetSize(list2));
	spListDestroy(list2);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	spBPQueueDestroy(p);
	return true;
}

bool bpqueueMaxSizeTest() {
	int maxSize=5;
	SPBPQueue p = spBPQueueCreate(maxSize);
	ASSERT_TRUE(spBPQueueGetMaxSize(p) == maxSize);
	spBPQueueDestroy(p);
	return true;
}

bool bpqueueEnqueueTest() {
	SPListElement e1 = spListElementCreate(1, 12.0);
	SPListElement e2 = spListElementCreate(2, 8.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 15.0);
	int maxSize=5;
	SPBPQueue p = spBPQueueCreate(maxSize);
	ASSERT_TRUE(spBPQueueEnqueue(p,e1)==SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(p->qList->head->data->value==e1->value);
	ASSERT_TRUE(p->qList->tail->data->value==e1->value);
	ASSERT_TRUE(spBPQueueEnqueue(p,e2)==SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(p->qList->head->data->value==e2->value);
	ASSERT_TRUE(p->qList->tail->data->value==e1->value);
	ASSERT_TRUE(spBPQueueEnqueue(p,e3)==SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(p->qList->head->data->value==e3->value);
	ASSERT_TRUE(p->qList->tail->data->value==e1->value);
	ASSERT_TRUE(spBPQueueEnqueue(p,e4)==SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(p->qList->head->data->value==e3->value);
	ASSERT_TRUE(p->qList->tail->data->value==e4->value);
	ASSERT_TRUE(spBPQueueEnqueue(NULL,e1)==SP_BPQUEUE_INVALID_ARGUMENT);
	ASSERT_TRUE(spBPQueueEnqueue(p,e1)==SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueEnqueue(p,e2)==SP_BPQUEUE_FULL);
	ASSERT_TRUE(p->qList->head->data->value==e1->value);
	ASSERT_TRUE(spBPQueueEnqueue(p,e4)==SP_BPQUEUE_FULL);
	ASSERT_TRUE(p->qList->head->data->value==e1->value);
	ASSERT_TRUE(spBPQueueEnqueue(p,NULL)==SP_BPQUEUE_INVALID_ARGUMENT);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	spBPQueueDestroy(p);
	return true;
}

bool bpqueueDqueueTest() {
	SPListElement e1 = spListElementCreate(1, 12.0);
	SPListElement e2 = spListElementCreate(2, 8.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 10.0);
	SPListElement eMin = spListElementCreate(5, 1.0);
	int maxSize=5;
	SPBPQueue p = spBPQueueCreate(maxSize);
	ASSERT_TRUE(spBPQueueDequeue(NULL)==SP_BPQUEUE_INVALID_ARGUMENT);
	ASSERT_TRUE(spBPQueueDequeue(p)==SP_BPQUEUE_EMPTY);
	ASSERT_TRUE(spBPQueueEnqueue(p,e1)==SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueEnqueue(p,eMin)==SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueEnqueue(p,e2)==SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueEnqueue(p,e3)==SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueEnqueue(p,e4)==SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueDequeue(p)==SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(p->qList->head->data->value==e3->value);
	ASSERT_TRUE(spBPQueueDequeue(p)==SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(p->qList->head->data->value==e2->value);
	ASSERT_TRUE(spBPQueueDequeue(p)==SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(p->qList->head->data->value==e4->value);
	ASSERT_TRUE(spBPQueueDequeue(p)==SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(p->qList->head->data->value==e1->value);
	ASSERT_TRUE(spBPQueueDequeue(p)==SP_BPQUEUE_SUCCESS);
	ASSERT_TRUE(spBPQueueDequeue(p)==SP_BPQUEUE_EMPTY);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	spListElementDestroy(eMin);
	spBPQueueDestroy(p);
	return true;
}


bool bpqueuePeekTest() {
	SPListElement e1 = spListElementCreate(1, 12.0);
	SPListElement e2 = spListElementCreate(2, 8.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 10.0);
	SPListElement e5 = spListElementCreate(5, 1.0);
	int maxSize=7;
	SPBPQueue p = spBPQueueCreate(maxSize);
	spBPQueueEnqueue(p,e1);
	spBPQueueEnqueue(p,e5);
	spBPQueueEnqueue(p,e2);
	spBPQueueEnqueue(p,e3);
	spBPQueueEnqueue(p,e4);
	ASSERT_TRUE(spListElementCompare(spBPQueuePeek(p),e5));
	ASSERT_FALSE(spListElementCompare(spBPQueuePeek(p),e4));
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	spListElementDestroy(e5);
	spBPQueueDestroy(p);
	return true;
}

bool bpqueuePeekLastTest() {
	SPListElement e1 = spListElementCreate(1, 12.0);
	SPListElement e2 = spListElementCreate(2, 8.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 10.0);
	SPListElement e5 = spListElementCreate(5, 1.0);
	int maxSize=7;
	SPBPQueue p = spBPQueueCreate(maxSize);
	spBPQueueEnqueue(p,e1);
	spBPQueueEnqueue(p,e5);
	spBPQueueEnqueue(p,e2);
	spBPQueueEnqueue(p,e3);
	spBPQueueEnqueue(p,e4);
	ASSERT_TRUE(spListElementCompare(spBPQueuePeekLast(p),e1));
	ASSERT_FALSE(spListElementCompare(spBPQueuePeekLast(p),e5));
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	spListElementDestroy(e5);
	spBPQueueDestroy(p);
	return true;
}

bool bpqueueMinValueTest() {
	SPListElement e1 = spListElementCreate(1, 12.0);
	SPListElement e2 = spListElementCreate(2, 8.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 10.0);
	SPListElement e5 = spListElementCreate(5, 1.0);
	int maxSize=7;
	SPBPQueue p = spBPQueueCreate(maxSize);
	spBPQueueEnqueue(p,e1);
	spBPQueueEnqueue(p,e5);
	spBPQueueEnqueue(p,e2);
	spBPQueueEnqueue(p,e3);
	spBPQueueEnqueue(p,e4);
	ASSERT_TRUE(spBPQueueMinValue(p)==e5->value);
	ASSERT_FALSE(spBPQueueMinValue(p)==e4->value);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	spListElementDestroy(e5);
	spBPQueueDestroy(p);
	return true;
}

bool bpqueueMaxValueTest() {
	SPListElement e1 = spListElementCreate(1, 12.0);
	SPListElement e2 = spListElementCreate(2, 8.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 10.0);
	SPListElement e5 = spListElementCreate(5, 1.0);
	int maxSize=7;
	SPBPQueue p = spBPQueueCreate(maxSize);
	spBPQueueEnqueue(p,e1);
	spBPQueueEnqueue(p,e5);
	spBPQueueEnqueue(p,e2);
	spBPQueueEnqueue(p,e3);
	spBPQueueEnqueue(p,e4);
	ASSERT_TRUE(spBPQueueMaxValue(p)==e1->value);
	ASSERT_FALSE(spBPQueueMaxValue(p)==e4->value);
	spListElementDestroy(e1);
	spListElementDestroy(e2);
	spListElementDestroy(e3);
	spListElementDestroy(e4);
	spListElementDestroy(e5);
	spBPQueueDestroy(p);
	return true;
}

bool bpqueueIsEmptyTest() {
	SPListElement e1 = spListElementCreate(1, 12.0);
	int maxSize=5;
	SPBPQueue p = spBPQueueCreate(maxSize);
	ASSERT_TRUE(spBPQueueIsEmpty(p));
	spBPQueueEnqueue(p,e1);
	ASSERT_FALSE(spBPQueueIsEmpty(p));
	spBPQueueDestroy(p);
	return true;
}

bool bpqueueIsFullTest() {
	SPListElement e1 = spListElementCreate(1, 12.0);
	SPListElement e2 = spListElementCreate(2, 8.0);
	SPListElement e3 = spListElementCreate(3, 3.0);
	SPListElement e4 = spListElementCreate(4, 10.0);
	SPListElement e5 = spListElementCreate(5, 1.0);
	int maxSize=5;
	SPBPQueue p = spBPQueueCreate(maxSize);
	ASSERT_FALSE(spBPQueueIsFull(p));
	spBPQueueEnqueue(p,e1);
	spBPQueueEnqueue(p,e5);
	spBPQueueEnqueue(p,e2);
	ASSERT_FALSE(spBPQueueIsFull(p));
	spBPQueueEnqueue(p,e3);
	spBPQueueEnqueue(p,e4);
	ASSERT_TRUE(spBPQueueIsFull(p));
	spListElementDestroy(e1);
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
