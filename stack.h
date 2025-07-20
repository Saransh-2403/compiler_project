


#ifndef STACK_H
#define STACK_H
#include <stdlib.h>
#include <stdio.h>
#include "tree.h"

typedef struct {
    TreeNode **array;    
    int capacity;        
    int top;            
} Stack;

void createStack(Stack *stack, int capacity);
void deleteStack(Stack *stack);
int isStackFull(const Stack *stack);
int isStackEmpty(const Stack *stack);
void resizeStack(Stack *stack);
int pushStack(Stack *stack, TreeNode *node);
int popStack(Stack *stack);
int topStack(Stack *stack, TreeNode **node);
void printStack(const Stack *stack);



#endif // STACK_H
// #endif  // STACK_H