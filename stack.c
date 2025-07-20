

/**
 * @file stack.c
 * @brief Implementation of a dynamic stack for TreeNode pointers.
 */

#include <stdlib.h>
#include <stdio.h>
#include "stack.h"
#include "tree.h"

/**
 * @brief Creates a dummy token with the given lexeme.
 * 
 * @param lexeme The lexeme to be assigned to the token.
 * @return Token* Pointer to the created dummy token.
 */
Token* createDummyToken(const char* lexeme);

/**
 * @brief Initializes a stack with a given capacity.
 * 
 * @param stack Pointer to the stack to be initialized.
 * @param capacity Initial capacity of the stack.
 */
void createStack(Stack *stack, int capacity);

/**
 * @brief Deletes a stack and frees its memory.
 * 
 * @param stack Pointer to the stack to be deleted.
 */
void deleteStack(Stack *stack);

/**
 * @brief Checks if the stack is full.
 * 
 * @param stack Pointer to the stack to be checked.
 * @return int 1 if the stack is full, 0 otherwise.
 */
int isStackFull(const Stack *stack);

/**
 * @brief Checks if the stack is empty.
 * 
 * @param stack Pointer to the stack to be checked.
 * @return int 1 if the stack is empty, 0 otherwise.
 */
int isStackEmpty(const Stack *stack);

/**
 * @brief Resizes the stack to double its current capacity.
 * 
 * @param stack Pointer to the stack to be resized.
 */
void resizeStack(Stack *stack);

/**
 * @brief Pushes a TreeNode pointer onto the stack.
 * 
 * @param stack Pointer to the stack.
 * @param node Pointer to the TreeNode to be pushed.
 * @return int 1 on success, 0 on failure.
 */
int pushStack(Stack *stack, TreeNode *node);

/**
 * @brief Pops the top element from the stack.
 * 
 * @param stack Pointer to the stack.
 * @return int 1 on success, 0 if the stack is empty.
 */
int popStack(Stack *stack);

/**
 * @brief Retrieves the top element of the stack without removing it.
 * 
 * @param stack Pointer to the stack.
 * @param node Pointer to the TreeNode pointer where the top element will be stored.
 * @return int 1 on success, 0 if the stack is empty.
 */
int topStack(Stack *stack, TreeNode **node);

Token* createDummyToken(const char* lexeme) {
    Token* token = malloc(sizeof(Token));
    if (token == NULL) {
        fprintf(stderr, "Memory allocation failed in createDummyToken\n");
        exit(EXIT_FAILURE);
    }
  
    strncpy(token->lexeme, lexeme, BUFFER_SIZE);
    token->lexeme[BUFFER_SIZE - 1] = '\0'; 
    
    token->type = 0;    
    token->cat = 0;     
    token->lineNo = 0;  
    return token;
}
void createStack(Stack *stack, int capacity) {
    stack->array = (TreeNode **)malloc(capacity * sizeof(TreeNode *));
    if (stack->array == NULL) {
        fprintf(stderr, "Memory allocation failed in createStack\n");
        exit(EXIT_FAILURE);
    }
    stack->capacity = capacity;
    stack->top = -1;  
}


void deleteStack(Stack *stack) {
    free(stack->array);
    stack->array = NULL;
    stack->capacity = 0;
    stack->top = -1;
}

int isStackFull(const Stack *stack) {
    return stack->top == stack->capacity - 1;
}
int isStackEmpty(const Stack *stack) {
    return stack->top == -1;
}

void resizeStack(Stack *stack) {
    int oldCapacity = stack->capacity;
    int newCapacity = oldCapacity * 2;
    TreeNode **newArray = (TreeNode **)realloc(stack->array, newCapacity * sizeof(TreeNode *));
    if (newArray == NULL) {
        fprintf(stderr, "Memory allocation failed in resizeStack\n");
        exit(EXIT_FAILURE);
    }
    stack->array = newArray;
    stack->capacity = newCapacity;
    // printf("Stack resized from %d to %d elements\n", oldCapacity, newCapacity);
}

int pushStack(Stack *stack, TreeNode *node) {
    if (isStackFull(stack)) {
        resizeStack(stack);
    }
    stack->array[++stack->top] = node;
    return 1;  // Success
}
int popStack(Stack *stack) {
    if (isStackEmpty(stack)) {
        return 0;  
    }   
    stack->top--;
    return 1;  // Success
}
int topStack(Stack *stack, TreeNode **node) {
    if (isStackEmpty(stack)) {
        return 0;  // Failure, stack is empty
    }
    *node = stack->array[stack->top];
    return 1;  
}
