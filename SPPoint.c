/*
 * SPPoint.c
 *
 *  Created on: May 20, 2016
 *      Author: MatanGizunterman and LironGazit
 */

#include "SPPoint.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct sp_point_t{
	double* data;
	int dim;
	int index;
};

SPPoint spPointCreate(double* data, int dim, int index){
	//Validate legal arguments
	if(dim<=0 || index<0 || data==NULL){
		return NULL;
	}
	SPPoint point=(SPPoint) malloc(sizeof(*point));    //Allocating memory for new point structure
	if(point==NULL){										   //Validate no allocation failure
		return NULL;
	}
	point->data=(double*) malloc(sizeof(double)*dim);  //Allocating memory for data field in point structure
	if(point->data==NULL){										//Validate no allocation failure
		return NULL;
	}
	for(int i=0;i<dim;i++){
		point->data[i]=data[i];
	}
	point->dim=dim;
	point->index=index;
	return point;
}

SPPoint spPointCopy(SPPoint source){
// Asserting legal arguments value
	assert(source!=NULL);
	int dim=spPointGetDimension(source);
	int index=spPointGetIndex(source);
	double* data=source->data;
	return spPointCreate(data, dim,index);          //use the spPointCreate function to create new allocated point copy
}

void spPointDestroy(SPPoint point){
// Validates legal arguments value
	if (point == NULL) {
		return;
	}
	free(point->data);										//Memory re-allocation
	free(point);												//Memory re-allocation
}

int spPointGetDimension(SPPoint point){
// Asserting legal arguments value
	assert(point!=NULL);
	return point->dim;
}

int spPointGetIndex(SPPoint point){
// Asserting legal arguments value
	assert(point!=NULL);
	return point->index;
}

double spPointGetAxisCoor(SPPoint point, int axis){
// Asserting legal arguments value
	assert(point!=NULL);
	assert(axis>=0);
	assert(axis<point->dim);
	return point->data[axis];
}

double spPointL2SquaredDistance(SPPoint p, SPPoint q){
// Asserting legal arguments value
	assert(p!=NULL);
	assert(q!=NULL);
	int dimP=spPointGetDimension(p);
	int dimQ=spPointGetDimension(q);
// Asserting dimension size
	assert(dimP==dimQ);
	double sum=0,temp;
	for(int i=0;i<dimP;i++){
		temp=spPointGetAxisCoor(p,i)-spPointGetAxisCoor(q,i);
		temp=temp*temp;
		sum+=temp;
	}
	return sum;
}



