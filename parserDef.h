


#include "vectorofvector.h"
struct First_Follow
{
    VectorOfVector firstset;
    VectorOfVector followset;
};

struct grammar
{
    VectorOfVector *Grammar;
};

struct pTable
{
    VectorOfVector parser_Table;
};

typedef struct pTable *parsetable;
typedef struct grammar *grammar;
typedef struct First_Follow *First_Follow;

#define NONTERMINALS 53
#define TERMINALS 56
#define EPSILON 110
#define TERMS_SIZE 111
#define NONTERMINALS_START 57
#define NONTERMINALS_END 110
#define SYNCRO -2
static char *grammarTerms[111] = {
"$",                         // 0
//terminals
"TK_MAIN",                   // 1
"TK_END",                    // 2
"TK_FUNID",                  // 3
"TK_SEM",                    // 4
"TK_INPUT",                  // 5
"TK_PARAMETER",              // 6
"TK_LIST",                   // 7
"TK_SQL",                    // 8
"TK_SQR",                    // 9
"TK_OUTPUT",                 // 10
"TK_INT",                    // 11
"TK_REAL",                   // 12
"TK_RUID",                   // 13
"TK_COMMA",                  // 14
"TK_RECORD",                 // 15
"TK_ENDRECORD",              // 16
"TK_UNION",                  // 17
"TK_ENDUNION",               // 18
"TK_TYPE",                   // 19
"TK_COLON",                  // 20
"TK_FIELDID",                // 21
"TK_GLOBAL",                 // 22
"TK_ASSIGNOP",               // 23
"TK_WHILE",                  // 24
"TK_OP",                     // 25
"TK_CL",                     // 26
"TK_IF",                     // 27
"TK_THEN",                   // 28
"TK_ELSE",                   // 29
"TK_ENDIF",                  // 30
"TK_READ",                   // 31
"TK_WRITE",                  // 32
"TK_PLUS",                   // 33
"TK_MINUS",                  // 34
"TK_MUL",                    // 35
"TK_DIV",                    // 36
"TK_NOT",                    // 37
"TK_AND",                    // 38
"TK_OR",                     // 39
"TK_LT",                     // 40
"TK_LE",                     // 41
"TK_EQ",                     // 42
"TK_GT",                     // 43
"TK_GE",                     // 44
"TK_NE",                     // 45
"TK_RETURN",                 // 46
"TK_DEFINETYPE",             // 47
"TK_AS",                     // 48
"TK_DOT",                    // 49
"TK_CALL",                   // 50
"TK_WITH",                   // 51
"TK_PARAMETERS",             // 52
"TK_NUM",                    // 53
"TK_RNUM",                   // 54
"TK_ENDWHILE",               // 55
"TK_ID",                     // 56
// Non-Terminals
"program",                   // 57
"otherFunctions",            // 58
"mainFunction",              // 59
"function",                  // 60
"input_par",                 // 61
"output_par",                // 62
"parameter_list",            // 63
"dataType",                  // 64
"primitiveDataType",         // 65
"constructedDataType",       // 66
"remaining_list",            // 67
"stmts",                     // 68
"typeDefinitions",           // 69
"actualOrRedefined",         // 70
"typeDefinition",            // 71
"fieldDefinitions",          // 72
"fieldDefinition",           // 73
"fieldType",                 // 74
"moreFields",                // 75
"declarations",              // 76
"declaration",               // 77
"global_or_not",             // 78
"otherStmts",                // 79
"stmt",                      // 80
"assignmentStmt",            // 81
"SingleOrRecId",             // 82
"option_single_constructed", // 83
"oneExpansion",              // 84
"moreExpansions",            // 85
"funCallStmt",               // 86
"outputParameters",          // 87
"inputParameters",           // 88
"iterativeStmt",             // 89
"conditionalStmt",           // 90
"elsePart",                  // 91
"ioStmt",                    // 92
"arithmeticExpression",      // 93
"expPrime",                  // 94
"term",                      // 95
"termPrime",                 // 96
"factor",                    // 97
"lowPrecedenceOp",           // 98
"highPrecedenceOp",          // 99
"booleanExpression",         // 100
"var",                       // 101
"logicalOp",                 // 102
"relationalOp",              // 103
"returnStmt",                // 104
"optionalReturn",            // 105
"idList",                    // 106
"more_ids",                  // 107
"definetypestmt",            // 108
"A",                         // 109
"eps"                        // 110 terminal
};