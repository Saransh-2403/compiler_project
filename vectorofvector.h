

#ifndef VECTOROFVECTOR_H
#define VECTOROFVECTOR_H

#include <stdlib.h>
#include <stdio.h>

#define INITIAL_CAPACITY 4

typedef struct
{
    int *data;
    size_t size;
    size_t capacity;
 } Vector;
 

 void initVector(Vector *v);

 void pushBack(Vector *v, int value);
 
 void freeVector(Vector *v);
 
 int accessVector(Vector *v, int index);
 
  void printVector(Vector *v);
 Vector *array_to_vector(int *arr, int size);
 
 typedef struct {
     Vector *data;
     size_t size;
     size_t capacity;
 } VectorOfVector;
 
 void initVectorOfVector(VectorOfVector *vv);
 
 void pushBackVector(VectorOfVector *vv, Vector v);
 
 void freeVectorOfVector(VectorOfVector *vv);
 
 int accessVectorOfVector(VectorOfVector *vv, int i, int j);
 
 void printVectorOfVector(VectorOfVector *vv);
 #endif // VECTOROFVECTOR_H