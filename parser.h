


#ifndef PARSER_H
#define PARSER_H
#include "tree.h"
#include "vectorofvector.h"
#include "parserDef.h"

#define GRAMMAR_SIZE 94      // Total number of productions
#define MAX_PROD_LENGTH 11   // (For documentation only)

// Function prototype for initializing the grammar.
// This creates a VectorOfVector where each production is stored as a Vector.
grammar initialize_grammar();
void parser_main(char *testfile,char* outfile);

typedef struct TreeNode TreeNode;

// Function to check whether the given term is already present in the set or not
int checker(int *temp, int n);

//Computes the FOLLOW set for a given non-terminal in a grammar.
void FirstSet(int n, grammar gmr, int buffer[150], First_Follow FF);
void FollowSet(int n, grammar gmr, int buffer[150], First_Follow FF, int check[111]);
First_Follow ComputeFirstAndFollowSets(grammar G);

void createParseTable(First_Follow F, parsetable T, grammar G);
TreeNode* parseInputSourceCode(char *testcaseFile, parsetable T,grammar G);
void printParseTree(TreeNode* root, char *outfile);
#endif // PARSER_H
