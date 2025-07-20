
/**
 * @file vector_def.c
 * @brief Implementation of dynamic vector and vector of vectors in C.
 */

#include <stdlib.h>
#include <stdio.h>
#include "vectorofvector.h"

/**
 * @brief Initializes a vector with initial capacity.
 * 
 * @param v Pointer to the vector to be initialized.
 */
void initVector(Vector *v);

/**
 * @brief Adds an element to the end of the vector, resizing if necessary.
 * 
 * @param v Pointer to the vector.
 * @param value The value to be added.
 */
void pushBack(Vector *v, int value);

/**
 * @brief Frees the memory allocated for the vector.
 * 
 * @param v Pointer to the vector to be freed.
 */
void freeVector(Vector *v);

/**
 * @brief Accesses an element at a given index in the vector.
 * 
 * @param v Pointer to the vector.
 * @param index The index of the element to access.
 * @return The value at the specified index.
 */
int accessVector(Vector *v, int index);

/**
 * @brief Prints the elements of the vector.
 * 
 * @param v Pointer to the vector to be printed.
 */
void printVector(Vector *v);

/**
 * @brief Initializes a vector of vectors with initial capacity.
 * 
 * @param vv Pointer to the vector of vectors to be initialized.
 */
void initVectorOfVector(VectorOfVector *vv);

/**
 * @brief Adds a vector to the end of the vector of vectors, resizing if necessary.
 * 
 * @param vv Pointer to the vector of vectors.
 * @param v The vector to be added.
 */
void pushBackVector(VectorOfVector *vv, Vector v);

/**
 * @brief Frees the memory allocated for the vector of vectors.
 * 
 * @param vv Pointer to the vector of vectors to be freed.
 */
void freeVectorOfVector(VectorOfVector *vv);

/**
 * @brief Accesses an element in a vector within the vector of vectors.
 * 
 * @param vv Pointer to the vector of vectors.
 * @param i The index of the vector within the vector of vectors.
 * @param j The index of the element within the vector.
 * @return The value at the specified indices.
 */
int accessVectorOfVector(VectorOfVector *vv, int i, int j);

/**
 * @brief Prints the elements of the vector of vectors.
 * 
 * @param vv Pointer to the vector of vectors to be printed.
 */
void printVectorOfVector(VectorOfVector *vv);

/**
 * @brief Converts an array to a vector.
 * 
 * @param arr Pointer to the array.
 * @param size The size of the array.
 * @return Pointer to the newly created vector.
 */
Vector *array_to_vector(int *arr, int size);

void initVector(Vector *v) {
    v->capacity = INITIAL_CAPACITY;
    v->size = 0;
    v->data = (int *)malloc(v->capacity * sizeof(int));
    if (!v->data) {
        fprintf(stderr, "Memory allocation error in initVector\n");
        exit(EXIT_FAILURE);
    }
}

void pushBack(Vector *v, int value) {
    if (v->size == v->capacity) {
        v->capacity *= 2;
        int *temp = realloc(v->data, v->capacity * sizeof(int));
        if (!temp) {
            fprintf(stderr, "Memory allocation error in pushBack\n");
            exit(EXIT_FAILURE);
        }
        v->data = temp;
    }
    v->data[v->size++] = value;
}

void freeVector(Vector *v) {
    if (v->data) {
        free(v->data);
    }
    v->data = NULL;
    v->size = 0;
    v->capacity = 0;
}


int accessVector(Vector *v, int index) {
    if (index < 0 || index >= v->size) {
        fprintf(stderr, "Index %d out of bounds for vector of size %d\n", index, v->size);
        exit(EXIT_FAILURE);
    }
    return v->data[index];
}

void printVector(Vector *v) {
    printf("[");
    for (int i = 0; i < v->size; i++) {
        printf("%d", v->data[i]);
        if (i < v->size - 1)
            printf(", ");
    }
    printf("]");
}

void initVectorOfVector(VectorOfVector *vv) {
    vv->capacity = INITIAL_CAPACITY;
    vv->size = 0;
    vv->data = (Vector *)malloc(vv->capacity * sizeof(Vector));
    if (!vv->data) {
        fprintf(stderr, "Memory allocation error in initVectorOfVector\n");
        exit(EXIT_FAILURE);
    }
}

void pushBackVector(VectorOfVector *vv, Vector v) {
    if (vv->size == vv->capacity) {
        vv->capacity *= 2;
        Vector *temp = realloc(vv->data, vv->capacity * sizeof(Vector));
        if (!temp) {
            fprintf(stderr, "Memory allocation error in pushBackVector\n");
            exit(EXIT_FAILURE);
        }
        vv->data = temp;
    }
    vv->data[vv->size++] = v;
}


void freeVectorOfVector(VectorOfVector *vv) {
    if (vv->data) {
        for (int i = 0; i < vv->size; i++) {
            freeVector(&vv->data[i]);
        }
        free(vv->data);
    }
    vv->data = NULL;
    vv->size = 0;
    vv->capacity = 0;
}

int accessVectorOfVector(VectorOfVector *vv, int i, int j) {
    if (i < 0 || i >= vv->size) {
        fprintf(stderr, "Vector index %d out of bounds for vector-of-vector of size %d\n", i, vv->size);
        exit(EXIT_FAILURE);
    }
    return accessVector(&(vv->data[i]), j);
}

void printVectorOfVector(VectorOfVector *vv) {
    printf("[\n");
    for (int i = 0; i < vv->size; i++) {
        printf("  ");
        printVector(&(vv->data[i]));
        if (i < vv->size - 1)
            printf(",");
        printf("\n");
    }
    printf("]\n");
}
Vector *array_to_vector(int *arr, int size)
{
    Vector *v = (Vector *)malloc(sizeof(Vector));
    initVector(v);
    for (int i = 0; i < size; i++)
    {
        pushBack(v, arr[i]);
    }
    return v;
}