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
	if(dim<=0 || index<0 || data==NULL){
		return NULL;
	}
	SPPoint point=NULL;
	point->data=(double*)malloc(sizeof(double)*dim);
	if(point->data==NULL){
		return NULL;
	}
	point->index=index;
	point->dim=dim;
	for(int i=0;i<dim;i++){
		point->data[i]=data[i];
	}
	return point;
}

SPPoint spPointCopy(SPPoint source){
	assert(source!=NULL);
	int dim=spPointGetDimension(source);
	int index=spPointGetIndex(source);
	double* data=(double*)malloc(sizeof(double)*dim);
	for(int i=0;i<dim;i++){
		data[i]=spPointGetAxisCoor(source,i);
	}
	return spPointCreate(data, dim,index);
}

void spPointDestroy(SPPoint point){
	free(point->data);
}

int spPointGetDimension(SPPoint point){
	return point->dim;
}

int spPointGetIndex(SPPoint point){
	return point->index;
}

double spPointGetAxisCoor(SPPoint point, int axis){
	assert(axis<point->dim);
	assert(point!=NULL);
	return point->data[axis];
}

double spPointL2SquaredDistance(SPPoint p, SPPoint q){
	assert(p!=NULL);
	assert(q!=NULL);
	int dimP=spPointGetDimension(p);
	int dimQ=spPointGetDimension(q);
	assert(dimP==dimQ);
	double sum=0,temp;
	for(int i=0;i<dimP;i++){
		temp=spPointGetAxisCoor(p,i)-spPointGetAxisCoor(q,i);
		temp=temp*temp;
		sum+=temp;
	}
	return sum;
}



