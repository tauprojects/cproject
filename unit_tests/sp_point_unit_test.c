#include "../SPPoint.h"
#include "unit_test_util.h"
#include <stdbool.h>

/**
 * Basic Point Create Unit Test
 * Checks spPointCreate function by Asserting that new point != NULL
 */
bool pointBasicCreate() {
	double data[2] = { 1.0, 1.0 };
	int dim = 2;
	int index = 1;
	SPPoint p = spPointCreate(data, dim, index);    //Create New point for testing
	ASSERT_TRUE(p!=NULL);
	spPointDestroy(p);      //Memory Re-Allocation
	return true;
}

/**
 * Basic Point Copy Unit Test
 * Checks spPointCopy function by Asserting that new point equals to source point
 * Asserts equaled points structure fields
 */
bool pointBasicCopyTest() {
	double data[2] = { 1.0, 1.0 };
	int dim = 2;
	int index = 1;
	SPPoint p = spPointCreate(data, dim, index);    //Create New point for testing
	SPPoint q = spPointCopy(p);
	ASSERT_TRUE(spPointGetDimension(p) == 2);
	ASSERT_TRUE(spPointGetIndex(p) == spPointGetIndex(q));
	ASSERT_TRUE(spPointGetDimension(p) == spPointGetDimension(q));
	for (int i = 0; i < spPointGetDimension(p); i++) {
		ASSERT_TRUE(spPointGetAxisCoor(p, i) == spPointGetAxisCoor(q, i));
	}
	spPointDestroy(p);      //Memory Re-Allocation
	spPointDestroy(q);      //Memory Re-Allocation
	return true;
}

/**
 * Basic Point L2 Distance Unit Test
 * Checks spPointL2SquaredDistance function
 * Asserts distance == 0 between identical points - expecting True
 * Asserts distance != 0 between different points - expecting False
 */
bool pointBasicL2Distance() {
	double data1[2] = { 1.0, 1.0 };
	double data2[2] = { 1.0, 0.0 };
	int dim1 = 2;
	int dim2 = 2;
	int index1 = 1;
	int index2 = 1;
	SPPoint p = spPointCreate((double *)data1, dim1, index1);   //Create New point for testing
	SPPoint q = spPointCreate((double *)data2, dim2, index2);
	ASSERT_TRUE(spPointL2SquaredDistance(p,p) == 0.0);
	ASSERT_TRUE(spPointL2SquaredDistance(q,q) == 0.0);
	ASSERT_FALSE(spPointL2SquaredDistance(p,q) == 0.0);
	spPointDestroy(p);      //Memory Re-Allocation
	spPointDestroy(q);      //Memory Re-Allocation
	return true;
}

/**
 * Basic Point Get Dimension Unit Test
 * Checks spPointGetDimension function
 * Asserts a point dimension with an expected value
 */
bool pointGetDimension(){
	double data[2] = { 1.0, 1.0 };
	int dim = 2;
	int index = 1;
	SPPoint p = spPointCreate(data, dim, index);  //Create New point for testing
	ASSERT_TRUE(spPointGetDimension(p) == dim);
	spPointDestroy(p);				//Memory Re-Allocation
	return true;
}

/**
 * Basic Point GetIndex Unit Test
 * Checks spPointGetIndex function
 * Asserts a point index with an expected value
 */
bool pointGetIndex(){
	double data[2] = { 1.0, 1.0 };
	int dim = 2;
	int index = 1;
	SPPoint p = spPointCreate(data, dim, index);  //Create New point for testing
	ASSERT_TRUE(spPointGetIndex(p) == index);
	spPointDestroy(p);				//Memory Re-Allocation
	return true;
}

/**
 * Basic Point Destroy Unit Test
 * Checks spPointDestroy function with NULL aregument
 */
bool testPointDestroy() {
	spPointDestroy(NULL);
	return true;
}


/**
 * Basic Point Get Axis Unit Test
 * Checks spPointGetAxisCoor function
 * Asserts a point Axis with an expected value
 * Asserts False to unexpected value
 */
bool pointGetAxisCoor() {
	double data[2] = { 1.0, 45.8 };
	int dim = 2;
	int index = 1;
	SPPoint p = spPointCreate(data, dim, index);  //Create New point for testing
	ASSERT_TRUE(spPointGetAxisCoor(p,0)==data[0]);
	ASSERT_TRUE(spPointGetAxisCoor(p,1)==data[1]);
	ASSERT_FALSE(spPointGetAxisCoor(p,1)==data[0]);
	spPointDestroy(p);				//Memory Re-Allocation
	return true;
}

int main() {
	RUN_TEST(pointBasicCreate);
	RUN_TEST(pointBasicCopyTest);
	RUN_TEST(pointGetDimension);
	RUN_TEST(pointGetIndex);
	RUN_TEST(testPointDestroy);
	RUN_TEST(pointGetAxisCoor);
	RUN_TEST(pointBasicL2Distance);
	return 0;
}
