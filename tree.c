

/**
 * @file tree.c
 * @brief Implementation of syntax tree node creation and manipulation functions.
 *
 * This file contains functions to create, manipulate, and free nodes in a syntax tree.
 * The syntax tree is used in the context of a compiler project.
 */

#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

/**
 * @brief Creates a new syntax tree node.
 *
 * This function allocates memory for a new TreeNode and initializes its fields.
 *
 * @return A pointer to the newly created TreeNode.
 */
TreeNode* createNode();

/**
 * @brief Sets a child node for a given parent node at a specified index.
 *
 * This function assigns a child node to a parent node at the specified index.
 * It also sets the parent pointer of the child node to the parent node.
 *
 * @param parent The parent TreeNode.
 * @param index The index at which to set the child node.
 * @param child The child TreeNode to be set.
 */
void setChild(TreeNode* parent, int index, TreeNode* child);

/**
 * @brief Frees the memory allocated for a syntax tree.
 *
 * This function recursively frees the memory allocated for a syntax tree,
 * starting from the given root node.
 *
 * @param root The root TreeNode of the syntax tree to be freed.
 */
void freeSyntaxTree(TreeNode* root);


TreeNode* createNode() {
   TreeNode* node = (TreeNode*) malloc(sizeof(TreeNode));
    if (!node) {
        fprintf(stderr, "Error: Memory allocation failed for terminal node.\n");
        exit(EXIT_FAILURE);
    }
    node->children=NULL;
    node->numChildren=0;
    node->isNonTerminal=false;
    node->isTerminal=false;
    node->parent=NULL;
    node->symbolID=-1;
    strcpy(node->lexeme, "");
    node->lineno=0;
    return node;
}

void setChild(TreeNode* parent, int index, TreeNode* child) {
    if (!parent || parent->isTerminal==true) {
        fprintf(stderr, "setChild Error: Provided parent is NULL or not a nonterminal.\n");
        return;
    }
    if (index < 0 || index >= parent->numChildren) {
        fprintf(stderr, "setChild Error: Index %d out of bounds (max %d).\n", index, parent->numChildren);
        return;
    }
    parent->children[index] = child;
    if (child) {
        child->parent = parent;
    }
}

void freeSyntaxTree(TreeNode* root) {
    if (!root) return;
    
    if (root->isNonTerminal) {
        
        for (int i = 0; i < root->numChildren; i++) {
            freeSyntaxTree(root->children[i]);
        }
        free(root->children);
    }
    free(root);
}




