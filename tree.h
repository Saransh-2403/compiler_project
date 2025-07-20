


#ifndef COMPILER_TREE_H
#define COMPILER_TREE_H

#include <stdbool.h>
#include "lexer.h" 
#include "parser.h"

typedef struct TreeNode TreeNode;

struct TreeNode {

    bool isTerminal;
    bool isNonTerminal;  
    TreeNode* parent;
    TreeNode** children;
    int numChildren;
    int symbolID;    
    char lexeme[512]; 
    int lineno;   
  
};

TreeNode* createNode();

void setChild(TreeNode* parent, int index,TreeNode* child);

void freeSyntaxTree(TreeNode* root);

#endif /* COMPILER_TREE_H */
