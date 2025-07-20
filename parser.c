

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "vectorofvector.h"
#include "stack.h"
#include "tree.h"
#include "lexer.h"
bool issyntaxcorrect = true;
grammar G;
First_Follow F;
 parsetable T;
Stack S;
int parser_called=0;
int sz = 0;


FILE *logFile = NULL; 
FILE *tableLogFile = NULL;

/**
 * @brief Function to print the grammar rules.
 *
 * This function iterates through the grammar rules and prints them in a readable format.
 * Each rule is printed in the form: <Non-terminal> ===> <Production>.
 * If a terminal symbol or epsilon is encountered, it is printed directly.
 * Non-terminal symbols are enclosed in angle brackets.
 *
 * @param G The grammar structure containing the rules to be printed.
 */
// Function to print Grammar
void print_grammar(grammar G)
{
    for (int i = 0; i < GRAMMAR_SIZE; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (accessVectorOfVector(G->Grammar, i, j) == -1)
            {
                printf("\n");
                break;
            }
            if (j == 0)
            {
                printf("<%s> ===> ", grammarTerms[accessVectorOfVector(G->Grammar, i, j)]);
            }
            else
            {
                if (grammarTerms[accessVectorOfVector(G->Grammar, i, j)][0] == 'T' || accessVectorOfVector(G->Grammar, i, j) == EPSILON)
                    printf("%s ", grammarTerms[accessVectorOfVector(G->Grammar, i, j)]);
                else
                    printf("<%s> ", grammarTerms[accessVectorOfVector(G->Grammar, i, j)]);
            }
        }
    }
}


/**
 * @brief Prints the first set of the grammar to a file named "first_out.txt".
 *
 * This function writes the first set of each grammar term to a file. The first set
 * is a set of terminals that appear at the beginning of some string derived from
 * the non-terminal.
 *
 * @param F The First_Follow structure containing the first sets.
 * @param G The grammar structure.
 *
 * The function opens a file named "first_out.txt" in write mode. If the file cannot
 * be opened, it prints an error message and returns. It then iterates over the
 * grammar terms and writes their first sets to the file. Each term and its first
 * set are written in the format:
 * 
 *     term ===> first_set_elements
 *
 * The function closes the file before returning.
 */
void print_first_set(First_Follow F, grammar G)
{
    FILE *fp = fopen("first_out.txt", "w"); 
    if (fp == NULL)
    {
        perror("Failed to open file");
        return;
    }

    fprintf(fp, "\n"); 


    for (int i = 0; i < TERMS_SIZE; i++)
    {
        fprintf(fp, "%s ===> ", grammarTerms[i]);
    

    
        Vector *firstVec = &F->firstset.data[i];
        for (int j = 0; j < firstVec->size; j++)
        {
            
            if (firstVec->data[j] == 0)
                break;
            else
            {
                fprintf(fp, "%s, ", grammarTerms[firstVec->data[j]]);
                
            }
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
}


/**
 * @brief Prints the follow set of a given grammar to a file.
 *
 * This function writes the follow set of each non-terminal in the grammar to a file named "follow_out.txt".
 * Each non-terminal's follow set is printed in the format: "NON_TERMINAL ===> FOLLOW_SET".
 * The follow set is represented as a comma-separated list of grammar terms.
 *
 * @param F The First_Follow structure containing the follow sets.
 * @param G The grammar structure.
 *
 * @note The function assumes that the follow set data is stored in a Vector structure within the First_Follow structure.
 *       The follow set for each non-terminal is accessed using the index of the non-terminal.
 *       Special values in the follow set:
 *       - 0: Indicates the end of the follow set for a non-terminal.
 *       - -1: Represents the end-of-input symbol ($).
 *
 * @warning If the file "follow_out.txt" cannot be opened, an error message is printed to stderr.
 */
void print_follow_set(First_Follow F, grammar G)
{
    FILE *fp = fopen("follow_out.txt", "w+");
    if (fp == NULL)
    {
        perror("Error opening file");
        return;
    }

    
    fprintf(fp, "\n");
    

    for (int i = NONTERMINALS_START; i < NONTERMINALS_END; i++)
    {
      
        fprintf(fp, "%s ===> ", grammarTerms[i]);
       
        Vector *followVec = &F->followset.data[i];
        for (int j = 0; j < followVec->size; j++)
        {
      
            if (followVec->data[j] == 0)
                break;
            else if (followVec->data[j] == -1)
            {
                fprintf(fp, "$, ");
                
            }
            else
            {
                fprintf(fp, "%s, ", grammarTerms[followVec->data[j]]);
            
            }
        }
       
        fprintf(fp, "\n");
       
    }

    fclose(fp); 
}


/**
 * @brief Checks if a given number exists in an array.
 *
 * This function iterates through an array of integers and checks if the specified
 * number exists in the array. The array is assumed to be terminated by a zero.
 *
 * @param temp Pointer to the array of integers.
 * @param n The number to check for in the array.
 * @return Returns -1 if the number is found in the array, otherwise returns 0.
 */
int checker(int *temp, int n)
{
    int i = 0, flag = 0;
    while (temp[i] != 0)
    {
        if (temp[i] == n)
        {
            flag = -1;
            break;
        }
        ++i;
    }
    return flag; 
}

/**
 * @brief Computes the First set for a given non-terminal in a grammar.
 *
 * This function calculates the First set for a given non-terminal symbol `n` in the provided grammar `gmr`.
 * The First set is stored in the provided `buffer` array. The function also updates the `First_Follow` structure `FF`.
 *
 * @param n The non-terminal symbol for which the First set is to be computed.
 * @param gmr The grammar structure containing the production rules.
 * @param buffer An array to store the computed First set. The array should have a size of at least 150.
 * @param FF The First_Follow structure that contains the First and Follow sets for all symbols.
 */
void FirstSet(int n, grammar gmr, int buffer[150], First_Follow FF)
{
    int j = 0;
    
    for (int i = 0; i < 150; i++)
    {
        if (buffer[i] == 0)
        {
            j = i;
            break;
        }
    }

    
    if (n < NONTERMINALS_START || n == EPSILON)
    {
        buffer[j] = n;
    }
    else
    {
        
        for (int i = 0; i < GRAMMAR_SIZE; i++)
        {
           
            if (accessVectorOfVector(gmr->Grammar, i, 0) == n)
            {
                int rhsSymbol = accessVectorOfVector(gmr->Grammar, i, 1);
             
                if (rhsSymbol <= 56 || rhsSymbol == 110)
                {
                    if (!checker(buffer, rhsSymbol))
                    {
                        buffer[j++] = rhsSymbol;
                    }
                }
                else
                {
                    int epsn = 0;
                    
                    for (int k = 1; k < 10; k++)
                    {
                        int symbol = accessVectorOfVector(gmr->Grammar, i, k);
                        if (symbol == -1)
                            break;
                        epsn = 0;
                        
                        FirstSet(symbol, gmr, FF->firstset.data[symbol].data, FF);

                    
                        Vector *firstSetVector = &FF->firstset.data[symbol];
                        for (int l = 0; l < firstSetVector->size; l++)
                        {
                            int curr = firstSetVector->data[l];
                            
                            if (curr == 0)
                                break;
                            if (curr == EPSILON)
                                epsn = 1;
                            else
                            {
                                if (!checker(buffer, curr))
                                {
                                    buffer[j++] = curr;
                                }
                            }
                        }
                      
                        if (epsn != 1)
                            break;
                    }
                    
                    if (epsn)
                    {
                        buffer[j++] = EPSILON;
                    }
                }
            }
        }
    }
}


/**
 * @brief Computes the Follow Set for a given non-terminal in a grammar.
 *
 * This function computes the Follow Set for a given non-terminal symbol `n` in the provided grammar `gmr`.
 * The Follow Set is stored in the provided `buffer`. The function also uses the `First_Follow` structure `FF`
 * to access the first and follow sets of other symbols and a `check` array to avoid infinite recursion.
 *
 * @param n The non-terminal symbol for which the Follow Set is to be computed.
 * @param gmr The grammar structure containing the production rules.
 * @param buffer An array to store the computed Follow Set.
 * @param FF The First_Follow structure containing the first and follow sets of all symbols.
 * @param check An array used to mark symbols that have already been processed to avoid infinite recursion.
 */
void FollowSet(int n, grammar gmr, int buffer[150], First_Follow FF, int check[111])
{
    int j = 0, k;

    
    if (check[n] == 1)
    {
        check[n] = 0;
        return;
    }

    
    if (n == NONTERMINALS_START)
    {
        buffer[j++] = -1; 
    }

   
    for (int i = 0; i < GRAMMAR_SIZE; i++)
    {
        
        for (int p = 1; p < 12; p++)
        {
            int symbolAtP = accessVectorOfVector(gmr->Grammar, i, p);
            if (symbolAtP == -1)
                break;
            
            if (symbolAtP == n)
            {
                
                for (k = p + 1; accessVectorOfVector(gmr->Grammar, i, k) != -1; k++)
                {
                    int epsn = 0;
                    int followingSymbol = accessVectorOfVector(gmr->Grammar, i, k);
                    
                    Vector *firstVec = &FF->firstset.data[followingSymbol];
                    for (int l = 0; l < firstVec->size; l++)
                    {
                        int curr = firstVec->data[l];
                        if (curr == 0)
                            break; 
                        if (curr == EPSILON)
                            epsn = 1;
                        else
                        {
                            if (!checker(buffer, curr))
                            {
                                buffer[j++] = curr;
                            }
                        }
                    }
                    
                    if (!epsn)
                        break;
                }
               
                if (accessVectorOfVector(gmr->Grammar, i, k) == -1 &&
                    accessVectorOfVector(gmr->Grammar, i, 0) != n)
                {
                    int lhs = accessVectorOfVector(gmr->Grammar, i, 0);
                    check[lhs] = 1;
                    
                    FollowSet(lhs, gmr, FF->followset.data[lhs].data, FF, check);
                    check[lhs] = 0;
                    
                    Vector *followVec = &FF->followset.data[lhs];
                    for (int l = 0; l < followVec->size; l++)
                    {
                        int curr = followVec->data[l];
                        if (curr == 0)
                            break;
                        if (!checker(buffer, curr))
                        {
                            buffer[j++] = curr;
                        }
                    }
                }
            }
        }
    }
}

/**
 * @brief Computes the First and Follow sets for a given grammar.
 *
 * This function initializes and computes the First and Follow sets for the provided grammar.
 * It allocates memory for the First_Follow structure, initializes the vectors for the first and follow sets,
 * and then populates these sets by calling the appropriate functions.
 *
 * @param G The grammar for which the First and Follow sets are to be computed.
 * @return A pointer to the First_Follow structure containing the computed First and Follow sets.
 */
First_Follow ComputeFirstAndFollowSets(grammar G)
{
 
    First_Follow F = (First_Follow)malloc(sizeof(First_Follow));

    
    initVectorOfVector(&F->firstset);
    
    for (int i = 0; i < TERMS_SIZE; i++)
    {
        Vector row;
        initVector(&row);
        for (int j = 0; j < 20; j++)
        {
            pushBack(&row, 0);
        }
        pushBackVector(&F->firstset, row);
    }

    
    for (int i = 1; i < TERMS_SIZE; i++)
    {
        FirstSet(i, G, F->firstset.data[i].data, F);
    }

    
    initVectorOfVector(&F->followset);
   
    for (int i = 0; i < NONTERMINALS_START; i++)
    {
        Vector dummy;
        initVector(&dummy);
        pushBackVector(&F->followset, dummy);
    }
  
    for (int i = NONTERMINALS_START; i < NONTERMINALS_END; i++)
    {
        Vector row;
        initVector(&row);
        for (int j = 0; j < 20; j++)
        {
            pushBack(&row, 0);
        }
        pushBackVector(&F->followset, row);
    }

    int check[111] = {0};
    int passes = 1;
    while (passes != 6)
    {
        for (int i = NONTERMINALS_START; i < NONTERMINALS_END; i++)
        {
            FollowSet(i, G, F->followset.data[i].data, F, check);
        }
        passes++;
    }
    return F;
}



/**
 * @brief Frees the memory allocated for the First_Follow structure.
 *
 * This function takes a First_Follow structure and frees the memory
 * allocated for it. It is important to call this function to avoid
 * memory leaks when the First_Follow structure is no longer needed.
 *
 * @param F The First_Follow structure to be freed.
 */
void freeFirstandFollow(First_Follow F)
{
    free(F);
}

/**
 * @brief Initializes the parse table with given First and Follow sets.
 *
 * This function allocates memory for the parse table and initializes it with default values.
 * It then populates the table based on the provided First and Follow sets.
 *
 * @param F The First and Follow sets used to initialize the parse table.
 * @return The initialized parse table.
 *
 * The function performs the following steps:
 * 1. Allocates memory for the parse table.
 * 2. Initializes the parse table with default values (-1).
 * 3. Populates the parse table based on the Follow sets.
 * 4. Sets specific entries in the parse table to SYNCRO based on predefined indices.
 */
parsetable initialize_table(First_Follow F)
{
    parsetable T = (parsetable)malloc(sizeof(struct pTable));
    
    initVectorOfVector(&T->parser_Table);
    for (int i = 0; i < NONTERMINALS; i++)
    {
        Vector row;
        initVector(&row);
        for (int j = 0; j <= TERMINALS; j++)
        {
            pushBack(&row, -1);
        }
        pushBackVector(&T->parser_Table, row);
    }

    
    for (int i = 0; i < NONTERMINALS; i++)
    {
        int follow_index = i + NONTERMINALS_START;
        Vector *followRow = &F->followset.data[follow_index];
        for (int j = 0; j < followRow->size; j++)
        {
            int x = accessVector(followRow, j);
            if (x == 0)
                break;
            if (x == -1)
                T->parser_Table.data[i].data[0] = SYNCRO; 
            else
                T->parser_Table.data[i].data[x] = SYNCRO;
        }
    }

   
    for (int i = 0; i < NONTERMINALS; i++)
    {
        Vector *row = &T->parser_Table.data[i];
        // row->data[4] = SYNCRO;
        // row->data[18] = SYNCRO;
        // row->data[16] = SYNCRO;
        // row->data[29] = SYNCRO;
        // row->data[30] = SYNCRO;
        // row->data[9] = SYNCRO;
        // row->data[2] = SYNCRO;
        // row->data[26] = SYNCRO;
        // row->data[55] = SYNCRO;
        
        row->data[3] = SYNCRO;
        row->data[1] = SYNCRO;
        row->data[15] = SYNCRO;
        row->data[17] = SYNCRO;
        row->data[47] = SYNCRO;
        row->data[19] = SYNCRO;
        row->data[56] = SYNCRO;
        row->data[24] = SYNCRO;
        row->data[27] = SYNCRO;
        row->data[31] = SYNCRO;
        row->data[32] = SYNCRO;
        row->data[50] = SYNCRO;
        row->data[8] = SYNCRO;
        
    }

    return T;
}
/**
 * @brief Creates a parse table for the given grammar.
 *
 * This function populates the parse table `T` using the first and follow sets `F` 
 * and the grammar `G`. It iterates through each production in the grammar and 
 * fills the parse table based on the first and follow sets.
 *
 * @param F The first and follow sets.
 * @param T The parse table to be populated.
 * @param G The grammar containing the productions.
 *
 * The function follows these steps:
 * 1. For each production in the grammar:
 *    - Retrieve the left-hand side (lhs) of the production.
 *    - Iterate through the right-hand side (rhs) symbols of the production.
 *    - If the rhs symbol is EPSILON, mark the flag `fl` and break.
 *    - If the rhs symbol is a terminal, update the parse table and break.
 *    - For each symbol in the first set of the rhs symbol:
 *      - If the symbol is not EPSILON, update the parse table.
 *      - If the symbol is EPSILON, set the flag `fl2`.
 *    - If `fl2` is not set, break the loop.
 *    - If `fl2` is set and the next rhs symbol is the end of the production, set the flag `fl`.
 * 2. If the flag `fl` is set, update the parse table using the follow set of the lhs.
 */

void createParseTable(First_Follow F, parsetable T, grammar G)
{
    int lhs, fl, fl2;

    
    for (int i = 0; i < GRAMMAR_SIZE; i++)
    {
        lhs = accessVectorOfVector(G->Grammar, i, 0);
        fl = 0;
        for (int j = 1; j < 10; j++)
        {
            fl2 = 0;
            int rhs = accessVectorOfVector(G->Grammar, i, j);
            if (rhs == -1)
                break;
            if (rhs == EPSILON)
            {
                fl = 1;
                break;
            }
            
            if (rhs >= 1 && rhs <= 56)
            {
                T->parser_Table.data[lhs - NONTERMINALS_START].data[rhs] = i;
                break;
            }

            
            for (int k = 0; k < 20; k++)
            {
                int first_val = accessVector(&F->firstset.data[rhs], k);
                if (first_val == 0)
                    break;
                if (first_val != EPSILON)
                {
                    T->parser_Table.data[lhs - NONTERMINALS_START].data[first_val] = i;
                }
                if (first_val == EPSILON)
                    fl2 = 1;
            }
            if (fl2 == 0)
                break;
            else if (fl2 == 1 && accessVectorOfVector(G->Grammar, i, j + 1) == -1)
            {
                fl = 1;
                break;
            }
        }
     
        if (fl == 1)
        {
            Vector *followRow = &F->followset.data[lhs];
            for (int j = 0; j < followRow->size; j++)
            {
                int x = accessVector(followRow, j);
                if (x == 0)
                    break;
                if (x == -1)
                    T->parser_Table.data[lhs - NONTERMINALS_START].data[0] = i;
                else
                    T->parser_Table.data[lhs - NONTERMINALS_START].data[x] = i;
            }
        }
    }
}

/**
 * @brief Prints the parsing table to the log file.
 *
 * This function prints the parsing table in a formatted manner to the 
 * specified log file. The table includes non-terminals and their corresponding 
 * values in the parsing table.
 *
 * @param T The parsing table to be printed.
 *
 * The function iterates over the non-terminals and prints each row of the 
 * parsing table. For each element in the row, it prints the value or an 
 * appropriate error/synchronization message.
 *
 * The output format includes:
 * - Non-terminals as row headers.
 * - Values in the parsing table.
 * - "ERR" for error entries.
 * - "SYN" for synchronization entries.
 */
void printTable(parsetable T)
{   
    fprintf(tableLogFile,"NON TERMINALS  %-20s|"," ");
    for(int i=0;i<NONTERMINALS_START;i++){
        fprintf(tableLogFile,"%-9s %d% -9s|\t"," ",i," ");
        // fprintf(tableLogFile,"%s(%d)%-10s|\t",grammarTerms[i],i," ");
    }
    fprintf(tableLogFile,"\n");
    for (int i = 0; i < NONTERMINALS; i++)
    {
        fprintf(tableLogFile,"%-30s ==> |\t", grammarTerms[i + NONTERMINALS_START]);
        Vector *row = &T->parser_Table.data[i];
        // Print every element of the row.
        for (int j = 0; j < row->size; j++)
        {
            int val = accessVector(row, j);
            if (val == -1)
                fprintf(tableLogFile,"ERR%-16s|\t"," ");
            else if (val == SYNCRO)
                fprintf(tableLogFile,"SYN%-16s|\t"," ");
            else
                fprintf(tableLogFile,"%d%-17s|\t", val," ");
        }
        fprintf(tableLogFile,"\n");
    }

    fprintf(tableLogFile,"---------- TERMINALS Legend------------\n");
    fprintf(tableLogFile,"              Name   | Index used in above table\n");
    for(int i=0;i<NONTERMINALS_START;i++){
        // fprintf(tableLogFile,"%-9s %d% -9s|\t"," ",i," ");
        fprintf(tableLogFile,"%-20s |\t %d\n",grammarTerms[i],i);
    }
}

/**
 * @brief Frees the memory allocated for the grammar structure.
 *
 * This function deallocates the memory used by the grammar structure,
 * including the memory for the grammar itself and the structure pointer.
 *
 * @param G A pointer to the grammar structure to be freed.
 */

void freeGrammar(grammar G)
{
   
    free(G->Grammar);
    free(G);
}

/**
 * @brief Finds the index of a given token in the grammar terms array.
 *
 * This function iterates through the grammar terms array and compares each term
 * with the string representation of the given token's type. If a match is found,
 * the index of the matching term is returned.
 *
 * @param ts The token whose index is to be found.
 * @return The index of the token in the grammar terms array.
 */
int indexoftoken(Token ts){
    int indx = 0;
        for (int i = 0; i < TERMS_SIZE; i++)
        {
            if (strcmp(grammarTerms[i], getTokenStr(ts.type)) == 0)
            {
                indx = i;
            }
        }
        return indx;
}
/**
 * @brief Generates tokens from the given testcase file.
 *
 * This function drives the token generation process by invoking the lexer
 * and then retrieves the array of tokens generated from the given testcase file.
 * It also prints an informational message indicating the completion of the lexing process.
 *
 * @param testcaseFile The path to the testcase file from which tokens are to be generated.
 * @return A pointer to the array of generated tokens.
 */

Token *togettokens(char *testcaseFile)
{
    driverToken(testcaseFile,0);

    Token *tokens = getarrayoftokens(testcaseFile);

    printf("[INFO] Lexing completed and tokens generated (To view token press 2)...\n");

    sz = getnooftokens();

    // printf("%d\n", sz);
    // for (int i = 0; i < sz; i++)
    // {
    //     printf("%s ", tokens[i].lexeme);
    //     printf("%s ",getTokenStr(tokens[i].type));
    //     printf("%d",indexoftoken(tokens[i]));
    //     printf("\n");
    // }
    return tokens;
}


/**
 * @brief Initializes the log file for the parser.
 *
 * This function checks if the log file is already open. If not, it attempts to open
 * the file "parser_output.txt" in write mode. If the file cannot be opened, an error
 * message is printed using perror.
 *
 * @note If the file cannot be opened, appropriate error handling should be implemented,
 *       such as exiting the program or returning from the function.
 */
void initLogFile() {
    if (logFile == NULL) {
        logFile = fopen("parser_output.txt", "w");
        if (logFile == NULL) {
            perror("Could not open parser_output.txt");
            // Handle error, e.g. exit or return
        }
    }
}

/**
 * @brief Initializes the log file for the parse table.
 *
 * This function opens the file "parse_table_output.txt" in write mode and assigns
 * the file pointer to the global variable `tableLogFile`. If the file is already
 * open, it does nothing. If the file cannot be opened, it prints an error message
 * using `perror`.
 *
 * @note Ensure that `tableLogFile` is declared as a global variable before using this function.
 */
void initTableLogFile() {
    if (tableLogFile == NULL) {
        tableLogFile = fopen("parse_table_output.txt", "w");
        if (tableLogFile == NULL) {
            perror("Could not open parse_table_output.txt");
            // Handle error, e.g. exit or return
        }
    }
}
/**
 * @brief Closes the log file if it is open.
 *
 * This function checks if the global log file pointer `logFile` is not NULL.
 * If it is not NULL, it closes the file and sets the pointer to NULL.
 */

void closeLogFile() {
    if (logFile != NULL) {
        fclose(logFile);
        logFile = NULL;
    }
}
/**
 * @brief Closes the table log file if it is open.
 *
 * This function checks if the global file pointer `tableLogFile` is not NULL.
 * If it is not NULL, it closes the file and sets the pointer to NULL.
 */

void closeTableLogFile() {
    if (tableLogFile != NULL) {
        fclose(tableLogFile);
        tableLogFile = NULL;
    }
}

/**
 * @brief Parses a given token using the provided parse table, stack, and grammar.
 *
 * This function processes a token and updates the parsing stack and syntax tree
 * based on the parsing table and grammar rules. It handles lexical errors, matches
 * terminals, and applies grammar rules to non-terminals.
 *
 * @param ts The token to be parsed.
 * @param T The parse table used for parsing.
 * @param s The stack used for parsing.
 * @param G The grammar used for parsing.
 *
 * The function performs the following steps:
 * 1. Checks for lexical errors and logs them.
 * 2. Skips comments.
 * 3. Processes the token and updates the stack and syntax tree.
 * 4. Handles terminal and non-terminal symbols.
 * 5. Applies grammar rules and updates the stack accordingly.
 *
 * The function logs various parsing actions and errors to a log file.
 */
void parseToken(Token ts, parsetable T, Stack *s,grammar G)
{
    if(ts.cat != NORMAL){
        
        if (ts.cat == LENGTHEXCEEDED)
        {
            if(ts.type == TK_FUNID){
                fprintf(logFile,"[Lexcial Error] Line no. %d Error: Function Identifier is longer than the prescribed length\n\n", ts.lineNo);
                printf("[Lexcial Error] Line no. %d Error: Function Identifier is longer than the prescribed length\n", ts.lineNo);
        }
            else
            {
                fprintf(logFile,"[Lexcial Error] Line no. %d Error: Variable Identifier is longer than the prescribed length \n\n", ts.lineNo);
                printf("[Lexcial Error] Line no. %d Error: Variable Identifier is longer than the prescribed length \n", ts.lineNo);
        
        }
        }
        else if (ts.cat == ERROR)
        {
            fprintf(logFile,"[Lexcial Error] Line no. %d Error: Unknown pattern <%s> \n\n", ts.lineNo, ts.lexeme);
            printf("[Lexcial Error] Line no. %d Error: Unknown pattern <%s> \n", ts.lineNo, ts.lexeme);
        }
      
        return;
    }
    if (strcmp(getTokenStr(ts.type), "TK_COMMENT") == 0)
        return;
    bool fl = false;//when to move the input pointer
    bool er_fl = false;
    
       
    while (fl== false)
    {   
        if (isStackEmpty(s))
            break;
        TreeNode *topNode = (TreeNode *)malloc(sizeof(TreeNode));
        int temp = topStack(s, &topNode);
        if (temp == 0)
            break;

        int indx = 0;
        for (int i = 0; i < TERMS_SIZE; i++)
        {
            if (strcmp(grammarTerms[i], getTokenStr(ts.type)) == 0)
            {
                indx = i;
            }
        }

         fprintf(logFile,"Top of Stack --> %s  Current input pointer -->%s\n",grammarTerms[topNode->symbolID],getTokenStr(ts.type));//top of stack node

       

        if ((topNode->isTerminal == true) && strcmp(grammarTerms[topNode->symbolID], getTokenStr(ts.type)) == 0)
        {   
            strcpy(topNode->lexeme, ts.lexeme);
            popStack(s);
            fprintf(logFile,"Terminal at top of stack matched with Terminal at current input pointer\n\n");
            fl = true;
            topNode->lineno = ts.lineNo;
            break;
        }
        else if ((topNode->isTerminal == true) && strcmp(grammarTerms[topNode->symbolID], getTokenStr(ts.type)) != 0)
        {    
            if (er_fl == false){
                fprintf(logFile,"[Parser Error] Line %d Error: The token %s for lexeme %s does not match with the expected token %s\n", ts.lineNo, getTokenStr(ts.type), ts.lexeme, grammarTerms[topNode->symbolID]);
                printf("[Parser Error] Line %d Error: The token %s for lexeme %s does not match with the expected token %s\n", ts.lineNo, getTokenStr(ts.type), ts.lexeme, grammarTerms[topNode->symbolID]);
            
            }
            popStack(s);
             fprintf(logFile,"The terminal at top of stack doesnt match with terminal at current input pointer so the terminal at top of stack is popped out \n\n");

            er_fl = true;
            issyntaxcorrect = false;
            
        }
        else
        {   
            int rule = accessVectorOfVector(&T->parser_Table, topNode->symbolID - NONTERMINALS_START, indx);
           
            if (rule == -1)
            {   
                fprintf(logFile,"no rule found\n");
                if (er_fl == false){
                    fprintf(logFile,"[Parser Error] Line %d Error: Invalid token %s encountered with the value %s stack top %s\n\n", ts.lineNo, getTokenStr(ts.type), ts.lexeme, grammarTerms[topNode->symbolID]);
                    printf("[Parser Error] Line %d Error: Invalid token %s encountered with the value %s stack top %s\n", ts.lineNo, getTokenStr(ts.type), ts.lexeme, grammarTerms[topNode->symbolID]);
                
                }
                issyntaxcorrect = false;
                er_fl = true;
                fl = true;
                return;
            }
            else if (rule == SYNCRO) // folow of the nonterminal
            {   
                 fprintf(logFile,"Syn rule found\n");
                if (er_fl == false){
                    printf("[Parser Error] Line %d Error: Invalid token %s encountered with the value %s stack top %s\n", ts.lineNo, getTokenStr(ts.type), ts.lexeme, grammarTerms[topNode->symbolID]);
                    fprintf(logFile,"[Parser Error] Line %d Error: Invalid token %s encountered with the value %s stack top %s\n", ts.lineNo, getTokenStr(ts.type), ts.lexeme, grammarTerms[topNode->symbolID]);
                }popStack(s);

                fprintf(logFile,"Non Terminal at the top of Stack is popped out\n\n");
                issyntaxcorrect = false;
                er_fl = true;
            }
            else if (rule != -1)
            {   
                fprintf(logFile,"%s==>",grammarTerms[accessVectorOfVector(G->Grammar,rule,0)]);
                for(int i=1;i<10;i++){
                    if(accessVectorOfVector(G->Grammar,rule,i)==-1) {
                        fprintf(logFile,"\n");
                        break;}
                fprintf(logFile,"%s   ",grammarTerms[accessVectorOfVector(G->Grammar,rule,i)]);}

                fprintf(logFile,"\n");
                popStack(s);
               
                bool fl2 = false;
              
                
                for (int i = 9; i >= 1; i--) 
                {
                    
                    if (accessVectorOfVector(G->Grammar, rule, i) == -1)
                    {   
                        
                        fl2 = true;
                        topNode->children = (TreeNode **)malloc((i - 1) * sizeof(struct TreeNode*));

                        topNode->numChildren = i - 1;
                    }

                    if (fl2 == true && accessVectorOfVector(G->Grammar, rule, i) != -1)
                    {

                        if (accessVectorOfVector(G->Grammar, rule, i) == EPSILON)
                        {
                            TreeNode *tempNode = createNode(); // create a new node and push it into the stack

                            tempNode->children = NULL;
                            tempNode->parent = topNode;
                            tempNode->symbolID = 110;
                            topNode->children[i - 1] = tempNode;
                        }
                        else if(accessVectorOfVector(G->Grammar, rule, i) < NONTERMINALS_START){

                            TreeNode *tempNode = createNode();

                            tempNode->symbolID = accessVectorOfVector(G->Grammar, rule, i);

                            tempNode->parent = topNode;

                            topNode->children[i - 1] = tempNode;

                            tempNode->isTerminal = true;
                            pushStack(s, tempNode);
                        }
                        else
                        {

                            TreeNode *tempNode = createNode();

                            tempNode->symbolID = accessVectorOfVector(G->Grammar, rule, i);

                            tempNode->parent = topNode;

                            topNode->children[i - 1] = tempNode;


                            pushStack(s, tempNode);
                        }
                    }
                }
            }
        }
    }
}

/**
 * @brief Parses the input source code from a given file and constructs a parse tree.
 *
 * This function reads the source code from the specified file, tokenizes it, and constructs
 * a parse tree using the provided parse table and grammar. The parse tree is returned as a 
 * TreeNode structure.
 *
 * @param testcaseFile The path to the file containing the source code to be parsed.
 * @param T The parse table used for parsing the source code.
 * @param G The grammar used for parsing the source code.
 * @return TreeNode* The root of the constructed parse tree.
 *
 * @note The function allocates memory for the stack and the root TreeNode. It is the caller's
 * responsibility to free the memory allocated for the parse tree.
 *
 * @warning The function exits the program if memory allocation for the stack or the root TreeNode
 * fails, or if tokenization fails.
 */
// hepler parse tree->parsefile->

TreeNode* parseInputSourceCode(char *testcaseFile, parsetable T,grammar G)
{    

    Stack *s = (Stack *)malloc(sizeof(Stack));
    if (!s) {
        printf("Memory allocation failed for Stack.\n");
        exit(1);
    }
    createStack(s, 10);
    
    TreeNode *root = createNode();
    if (!root) {
        printf("Memory allocation failed for TreeNode.\n");
        exit(1);
    }
    root->parent = NULL;
    root->symbolID = 57;
    
    pushStack(s, root);
    
    Token *tokens = togettokens(testcaseFile);
    if (!tokens) {
        printf("Tokenization failed.\n");
        exit(1);
    }
    
    for (int i = 0; i < sz; i++) {
    
        parseToken(tokens[i], T, s, G);
    }
    deleteStack(s);
    return root;
    
}
/**
 * @brief Recursively prints the details of each node in the parse tree to a file.
 *
 * This function traverses the parse tree in a pre-order manner and prints the details
 * of each node to the specified file. The details include the lexeme, line number,
 * symbol ID, and other relevant information.
 *
 * @param root Pointer to the root node of the parse tree.
 * @param fd Pointer to the file where the details will be printed.
 */

void printhelper(TreeNode *root, FILE *fd)
{
    int treesz = root->numChildren;
    if (treesz >= 1)
    {
        printhelper(root->children[0], fd);
    }
    char leaf[4];
    char dummy[5] = "----";
    if (treesz == 0)
    {
        strcpy(leaf, "yes");
        if (root->symbolID == EPSILON)
            strcpy(root->lexeme, "eps");
    }
    else
    {
        strcpy(leaf, "no");
        strcpy(root->lexeme, dummy);
    }

    if (root->symbolID == 57)
    {
        fprintf(fd, "%-15s\t %-3d\t %-22s\t %-10s\t %-22s\t %-4s\t %-22s\t \n", "----", root->lineno, grammarTerms[root->symbolID], "----", "$", leaf, grammarTerms[root->symbolID]);
    }
    else
    {
        if (strcmp(grammarTerms[root->symbolID], "TK_NUM") == 0)
            fprintf(fd, "%-15s\t %-3d\t %-22s\t %-10d\t %-22s\t %-4s\t %-22s\t \n", root->lexeme, root->lineno, grammarTerms[root->symbolID], atoi(root->lexeme), grammarTerms[root->parent->symbolID], leaf, grammarTerms[root->symbolID]);
        else if (strcmp(grammarTerms[root->symbolID], "TK_RNUM") == 0)
            fprintf(fd, "%-15s\t %-3d\t %-22s\t %-10.5f\t %-22s\t %-4s\t %-22s\t \n", root->lexeme, root->lineno, grammarTerms[root->symbolID], atof(root->lexeme), grammarTerms[root->parent->symbolID], leaf, grammarTerms[root->symbolID]);
        else
            fprintf(fd, "%-15s\t %-3d\t %-22s\t %-10s\t %-22s\t %-4s\t %-22s\t \n", root->lexeme, root->lineno, grammarTerms[root->symbolID], "----", grammarTerms[root->parent->symbolID], leaf, grammarTerms[root->symbolID]);
    }

    for (int i = 1; i < treesz; i++)
    {
        printhelper(root->children[i], fd);
    }
}

/**
 * @brief Prints the parse tree to a specified output file.
 *
 * This function takes the root of a parse tree and an output file path,
 * and writes the parse tree to the file in a formatted manner.
 *
 * @param root Pointer to the root node of the parse tree.
 * @param outfile Path to the output file where the parse tree will be written.
 *
 * @note The output file will be overwritten if it already exists.
 *       If the file cannot be opened, the function will print an error message
 *       and terminate the program.
 */
void printParseTree(TreeNode* root, char *outfile)
{
    FILE *f = fopen(outfile, "w");
    if (f == NULL)
    {
        printf("Error in opening the file");
        exit(1);
    }
    fprintf(f, "%-15s %-3s %-22s %-10s %-22s %-4s %-22s\n", "Lexeme", "LineNo", "TokenName", "ValueIfNumber", "Parent", "isLeaf  ", "Node");
    printhelper(root, f);
    fclose(f);
}


/**
 * @brief Initializes the grammar for the compiler project.
 * 
 * This function allocates memory for a VectorOfVector structure to hold the grammar productions.
 * It then initializes the grammar with predefined productions by converting arrays to vectors and 
 * pushing them into the VectorOfVector structure. Each production is represented as an array of integers.
 * 
 * @return grammar A pointer to the initialized grammar structure.
 * 
 * The grammar productions are as follows:
 * 
 * Production 1: <program> ===> <otherFunctions> <mainFunction>
 * Production 2: <mainFunction> ===> TK_MAIN <stmts> TK_END
 * Production 3: <otherFunctions> ===> <function> <otherFunctions>
 * Production 4: <otherFunctions> ===> <eps>
 * Production 5: <function> ===> TK_FUNID <input_par> <output_par> TK_SEM <stmts> TK_END
 * Production 6: <input_par> ===> TK_INPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR
 * Production 7: <output_par> ===> TK_OUTPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR
 * Production 8: <output_par> ===> <eps>
 * Production 9: <parameter_list> ===> <dataType> TK_ID <remaining_list>
 * Production 10: <dataType> ===> <primitiveDataType>
 * Production 11: <dataType> ===> <constructedDataType>
 * Production 12: <primitiveDataType> ===> TK_INT
 * Production 13: <primitiveDataType> ===> TK_REAL
 * Production 14: <constructedDataType> ===> <A> TK_RUID
 * Production 15: <constructedDataType> ===> TK_RUID
 * Production 16: <remaining_list> ===> TK_COMMA <parameter_list>
 * Production 17: <remaining_list> ===> <eps>
 * Production 18: <stmts> ===> <typeDefinitions> <declarations> <otherStmts> <returnStmt>
 * Production 19: <typeDefinitions> ===> <actualOrRedefined> <typeDefinitions>
 * Production 20: <typeDefinitions> ===> <eps>
 * Production 21: <actualOrRedefined> ===> <typeDefinition>
 * Production 22: <actualOrRedefined> ===> <definetypestmt>
 * Production 23: <typeDefinition> ===> TK_RECORD TK_RUID <fieldDefinitions> TK_ENDRECORD
 * Production 24: <typeDefinition> ===> TK_UNION TK_RUID <fieldDefinitions> TK_ENDUNION
 * Production 25: <fieldDefinitions> ===> <fieldDefinition> <fieldDefinition> <moreFields>
 * Production 26: <fieldDefinition> ===> TK_TYPE <fieldType> TK_COLON TK_FIELDID TK_SEM
 * Production 27: <fieldType> ===> <primitiveDataType>
 * Production 28: <fieldType> ===> <constructedDataType>
 * Production 29: <moreFields> ===> <fieldDefinition> <moreFields>
 * Production 30: <moreFields> ===> <eps>
 * Production 31: <declarations> ===> <declaration> <declarations>
 * Production 32: <declarations> ===> <eps>
 * Production 33: <declaration> ===> TK_TYPE <dataType> TK_COLON TK_ID <global_or_not> TK_SEM
 * Production 34: <global_or_not> ===> TK_COLON TK_GLOBAL
 * Production 35: <global_or_not> ===> <eps>
 * Production 36: <otherStmts> ===> <stmt> <otherStmts>
 * Production 37: <otherStmts> ===> <eps>
 * Production 38: <stmt> ===> <assignmentStmt>
 * Production 39: <stmt> ===> <iterativeStmt>
 * Production 40: <stmt> ===> <conditionalStmt>
 * Production 41: <stmt> ===> <ioStmt>
 * Production 42: <stmt> ===> <funCallStmt>
 * Production 43: <assignmentStmt> ===> <SingleOrRecId> TK_ASSIGNOP <arithmeticExpression> TK_SEM
 * Production 44: <SingleOrRecId> ===> TK_ID <option_single_constructed>
 * Production 45: <option_single_constructed> ===> <oneExpansion> <moreExpansions>
 * Production 46: <option_single_constructed> ===> <eps>
 * Production 47: <moreExpansions> ===> <oneExpansion> <moreExpansions>
 * Production 48: <moreExpansions> ===> <eps>
 * Production 49: <oneExpansion> ===> TK_DOT TK_FIELDID
 * Production 50: <funCallStmt> ===> <outputParameters> TK_CALL TK_FUNID TK_WITH TK_PARAMETERS <inputParameters> TK_SEM
 * Production 51: <outputParameters> ===> TK_SQL <idList> TK_SQR TK_ASSIGNOP
 * Production 52: <outputParameters> ===> <eps>
 * Production 53: <inputParameters> ===> TK_SQL <idList> TK_SQR
 * Production 54: <iterativeStmt> ===> TK_WHILE TK_OP <booleanExpression> TK_CL <stmt> <otherStmts> TK_ENDWHILE
 * Production 55: <conditionalStmt> ===> TK_IF TK_OP <booleanExpression> TK_CL TK_THEN <stmt> <otherStmts> <elsePart>
 * Production 56: <elsePart> ===> TK_ELSE <stmt> <otherStmts> TK_ENDIF
 * Production 57: <elsePart> ===> TK_ENDIF
 * Production 58: <ioStmt> ===> TK_READ TK_OP <var> TK_CL TK_SEM
 * Production 59: <ioStmt> ===> TK_WRITE TK_OP <var> TK_CL TK_SEM
 * Production 60: <arithmeticExpression> ===> <term> <expPrime>
 * Production 61: <expPrime> ===> <lowPrecedenceOp> <term> <expPrime>
 * Production 62: <expPrime> ===> <eps>
 * Production 63: <term> ===> <factor> <termPrime>
 * Production 64: <termPrime> ===> <highPrecedenceOp> <factor> <termPrime>
 * Production 65: <termPrime> ===> <eps>
 * Production 66: <factor> ===> TK_OP <arithmeticExpression> TK_CL
 * Production 67: <factor> ===> <var>
 * Production 68: <lowPrecedenceOp> ===> TK_PLUS
 * Production 69: <lowPrecedenceOp> ===> TK_MINUS
 * Production 70: <highPrecedenceOp> ===> TK_MUL
 * Production 71: <highPrecedenceOp> ===> TK_DIV
 * Production 72: <booleanExpression> ===> TK_OP <booleanExpression> TK_CL <logicalOp> TK_OP <booleanExpression> TK_CL
 * Production 73: <booleanExpression> ===> <var> <relationalOp> <var>
 * Production 74: <booleanExpression> ===> TK_NOT TK_OP <booleanExpression> TK_CL
 * Production 75: <var> ===> <SingleOrRecId>
 * Production 76: <var> ===> TK_NUM
 * Production 77: <var> ===> TK_RNUM
 * Production 78: <logicalOp> ===> TK_AND
 * Production 79: <logicalOp> ===> TK_OR
 * Production 80: <relationalOp> ===> TK_LT
 * Production 81: <relationalOp> ===> TK_LE
 * Production 82: <relationalOp> ===> TK_EQ
 * Production 83: <relationalOp> ===> TK_GT
 * Production 84: <relationalOp> ===> TK_GE
 * Production 85: <relationalOp> ===> TK_NE
 * Production 86: <returnStmt> ===> TK_RETURN <optionalReturn> TK_SEM
 * Production 87: <optionalReturn> ===> TK_SQL <idList> TK_SQR
 * Production 88: <optionalReturn> ===> <eps>
 * Production 89: <idList> ===> TK_ID <more_ids>
 * Production 90: <more_ids> ===> TK_COMMA <idList>
 * Production 91: <more_ids> ===> <eps>
 * Production 92: <definetypestmt> ===> TK_DEFINETYPE <A> TK_RUID TK_AS TK_RUID
 * Production 93: <A> ===> TK_RECORD
 * Production 94: <A> ===> TK_UNION
 */
grammar initialize_grammar()
{
    VectorOfVector *gr = (VectorOfVector *)malloc(sizeof(VectorOfVector));
    if (!gr)
    {
        fprintf(stderr, "Memory allocation error in initialize_grammar\n");
        exit(EXIT_FAILURE);
    }
    initVectorOfVector(gr);

    // Production 1: <program> ===> <otherFunctions> <mainFunction>
    {
        int arr1[] = {57, 58, 59, -1, -10, -10, -10, -10, -10, -10};
        Vector *v1 = array_to_vector(arr1, 10);
        // printf("\nsize of 0th vectir is %d \n",v1->size);
        pushBackVector(gr, *v1);
    }
    // Production 2: <mainFunction> ===> TK_MAIN <stmts> TK_END
    {
        int arr2[] = {59, 1, 68, 2, -1, -10, -10, -10, -10, -10};
        Vector *v2 = array_to_vector(arr2, 10);
        pushBackVector(gr, *v2);
    }
    // Production 3: <otherFunctions> ===> <function> <otherFunctions>
    {
        int arr3[] = {58, 60, 58, -1, -10, -10, -10, -10, -10, -10};
        Vector *v3 = array_to_vector(arr3, 10);
        pushBackVector(gr, *v3);
    }
    // Production 4: <otherFunctions> ===> <eps>
    {
        int arr4[] = {58, 110, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v4 = array_to_vector(arr4, 10);
        pushBackVector(gr, *v4);
    }
    // Production 5: <function> ===> TK_FUNID <input_par> <output_par> TK_SEM <stmts> TK_END
    {
        int arr5[] = {60, 3, 61, 62, 4, 68, 2, -1, -10, -10};
        Vector *v5 = array_to_vector(arr5, 10);
        pushBackVector(gr, *v5);
    }
    // Production 6: <input_par> ===> TK_INPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR
    {
        int arr6[] = {61, 5, 6, 7, 8, 63, 9, -1, -10, -10};
        Vector *v6 = array_to_vector(arr6, 10);
        pushBackVector(gr, *v6);
    }
    // Production 7: <output_par> ===> TK_OUTPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR
    {
        int arr7[] = {62, 10, 6, 7, 8, 63, 9, -1, -10, -10};
        Vector *v7 = array_to_vector(arr7, 10);
        pushBackVector(gr, *v7);
    }
    // Production 8: <output_par> ===> <eps>
    {
        int arr8[] = {62, 110, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v8 = array_to_vector(arr8, 10);
        pushBackVector(gr, *v8);
    }
    // Production 9: <parameter_list> ===> <dataType> TK_ID <remaining_list>
    {
        int arr9[] = {63, 64, 56, 67, -1, -10, -10, -10, -10, -10};
        Vector *v9 = array_to_vector(arr9, 10);
        pushBackVector(gr, *v9);
    }
    // Production 10: <dataType> ===> <primitiveDataType>
    {
        int arr10[] = {64, 65, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v10 = array_to_vector(arr10, 10);
        pushBackVector(gr, *v10);
    }
    // Production 11: <dataType> ===> <constructedDataType>
    {
        int arr11[] = {64, 66, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v11 = array_to_vector(arr11, 10);
        pushBackVector(gr, *v11);
    }
    // Production 12: <primitiveDataType> ===> TK_INT
    {
        int arr12[] = {65, 11, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v12 = array_to_vector(arr12, 10);
        pushBackVector(gr, *v12);
    }
    // Production 13: <primitiveDataType> ===> TK_REAL
    {
        int arr13[] = {65, 12, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v13 = array_to_vector(arr13, 10);
        pushBackVector(gr, *v13);
    }
    // Production 14: <constructedDataType> ===> <A> TK_RUID
    {
        int arr14[] = {66, 109, 13, -1, -10, -10, -10, -10, -10, -10};
        Vector *v14 = array_to_vector(arr14, 10);
        pushBackVector(gr, *v14);
    }
    // Production 15: <constructedDataType> ===> TK_RUID
    {
        int arr15[] = {66, 13, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v15 = array_to_vector(arr15, 10);
        pushBackVector(gr, *v15);
    }
    // Production 16: <remaining_list> ===> TK_COMMA <parameter_list>
    {
        int arr16[] = {67, 14, 63, -1, -10, -10, -10, -10, -10, -10};
        Vector *v16 = array_to_vector(arr16, 10);
        pushBackVector(gr, *v16);
    }
    // Production 17: <remaining_list> ===> <eps>
    {
        int arr17[] = {67, 110, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v17 = array_to_vector(arr17, 10);
        pushBackVector(gr, *v17);
    }
    // Production 18: <stmts> ===> <typeDefinitions> <declarations> <otherStmts> <returnStmt>
    {
        int arr18[] = {68, 69, 76, 79, 104, -1, -10, -10, -10, -10};
        Vector *v18 = array_to_vector(arr18, 10);
        pushBackVector(gr, *v18);
    }
    // Production 19: <typeDefinitions> ===> <actualOrRedefined> <typeDefinitions>
    {
        int arr19[] = {69, 70, 69, -1, -10, -10, -10, -10, -10, -10};
        Vector *v19 = array_to_vector(arr19, 10);
        pushBackVector(gr, *v19);
    }
    // Production 20: <typeDefinitions> ===> <eps>
    {
        int arr20[] = {69, 110, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v20 = array_to_vector(arr20, 10);
        pushBackVector(gr, *v20);
    }
    // Production 21: <actualOrRedefined> ===> <typeDefinition>
    {
        int arr21[] = {70, 71, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v21 = array_to_vector(arr21, 10);
        pushBackVector(gr, *v21);
    }
    // Production 22: <actualOrRedefined> ===> <definetypestmt>
    {
        int arr22[] = {70, 108, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v22 = array_to_vector(arr22, 10);
        pushBackVector(gr, *v22);
    }
    // Production 23: <typeDefinition> ===> TK_RECORD TK_RUID <fieldDefinitions> TK_ENDRECORD
    {
        int arr23[] = {71, 15, 13, 72, 16, -1, -10, -10, -10, -10};
        Vector *v23 = array_to_vector(arr23, 10);
        pushBackVector(gr, *v23);
    }
    // Production 24: <typeDefinition> ===> TK_UNION TK_RUID <fieldDefinitions> TK_ENDUNION
    {
        int arr24[] = {71, 17, 13, 72, 18, -1, -10, -10, -10, -10};
        Vector *v24 = array_to_vector(arr24, 10);
        pushBackVector(gr, *v24);
    }
    // Production 25: <fieldDefinitions> ===> <fieldDefinition> <fieldDefinition> <moreFields>
    {
        int arr25[] = {72, 73, 73, 75, -1, -10, -10, -10, -10, -10};
        Vector *v25 = array_to_vector(arr25, 10);
        pushBackVector(gr, *v25);
    }
    // Production 26: <fieldDefinition> ===> TK_TYPE <fieldType> TK_COLON TK_FIELDID TK_SEM
    {
        int arr26[] = {73, 19, 74, 20, 21, 4, -1, -10, -10, -10};
        Vector *v26 = array_to_vector(arr26, 10);
        pushBackVector(gr, *v26);
    }
    // Production 27: <fieldType> ===> <primitiveDataType>
    {
        int arr27[] = {74, 65, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v27 = array_to_vector(arr27, 10);
        pushBackVector(gr, *v27);
    }
    // Production 28: <fieldType> ===> <constructedDataType>
    {
        int arr28[] = {74, 66, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v28 = array_to_vector(arr28, 10);
        pushBackVector(gr, *v28);
    }
    // Production 29: <moreFields> ===> <fieldDefinition> <moreFields>
    {
        int arr29[] = {75, 73, 75, -1, -10, -10, -10, -10, -10, -10};
        Vector *v29 = array_to_vector(arr29, 10);
        pushBackVector(gr, *v29);
    }
    // Production 30: <moreFields> ===> <eps>
    {
        int arr30[] = {75, 110, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v30 = array_to_vector(arr30, 10);
        pushBackVector(gr, *v30);
    }
    // Production 31: <declarations> ===> <declaration> <declarations>
    {
        int arr31[] = {76, 77, 76, -1, -10, -10, -10, -10, -10, -10};
        Vector *v31 = array_to_vector(arr31, 10);
        pushBackVector(gr, *v31);
    }
    // Production 32: <declarations> ===> <eps>
    {
        int arr32[] = {76, 110, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v32 = array_to_vector(arr32, 10);
        pushBackVector(gr, *v32);
    }
    // Production 33: <declaration> ===> TK_TYPE <dataType> TK_COLON TK_ID <global_or_not> TK_SEM
    {
        int arr33[] = {77, 19, 64, 20, 56, 78, 4, -1, -10, -10};
        Vector *v33 = array_to_vector(arr33, 10);
        pushBackVector(gr, *v33);
    }
    // Production 34: <global_or_not> ===> TK_COLON TK_GLOBAL
    {
        int arr34[] = {78, 20, 22, -1, -10, -10, -10, -10, -10, -10};
        Vector *v34 = array_to_vector(arr34, 10);
        pushBackVector(gr, *v34);
    }
    // Production 35: <global_or_not> ===> <eps>
    {
        int arr35[] = {78, 110, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v35 = array_to_vector(arr35, 10);
        pushBackVector(gr, *v35);
    }
    // Production 36: <otherStmts> ===> <stmt> <otherStmts>
    {
        int arr36[] = {79, 80, 79, -1, -10, -10, -10, -10, -10, -10};
        Vector *v36 = array_to_vector(arr36, 10);
        pushBackVector(gr, *v36);
    }
    // Production 37: <otherStmts> ===> <eps>
    {
        int arr37[] = {79, 110, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v37 = array_to_vector(arr37, 10);
        pushBackVector(gr, *v37);
    }
    // Production 38: <stmt> ===> <assignmentStmt>
    {
        int arr38[] = {80, 81, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v38 = array_to_vector(arr38, 10);
        pushBackVector(gr, *v38);
    }
    // Production 39: <stmt> ===> <iterativeStmt>
    {
        int arr39[] = {80, 89, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v39 = array_to_vector(arr39, 10);
        pushBackVector(gr, *v39);
    }
    // Production 40: <stmt> ===> <conditionalStmt>
    {
        int arr40[] = {80, 90, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v40 = array_to_vector(arr40, 10);
        pushBackVector(gr, *v40);
    }
    // Production 41: <stmt> ===> <ioStmt>
    {
        int arr41[] = {80, 92, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v41 = array_to_vector(arr41, 10);
        pushBackVector(gr, *v41);
    }
    // Production 42: <stmt> ===> <funCallStmt>
    {
        int arr42[] = {80, 86, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v42 = array_to_vector(arr42, 10);
        pushBackVector(gr, *v42);
    }
    // Production 43: <assignmentStmt> ===> <SingleOrRecId> TK_ASSIGNOP <arithmeticExpression> TK_SEM
    {
        int arr43[] = {81, 82, 23, 93, 4, -1, -10, -10, -10, -10};
        Vector *v43 = array_to_vector(arr43, 10);
        pushBackVector(gr, *v43);
    }
    // Production 44: <SingleOrRecId> ===> TK_ID <option_single_constructed>
    {
        int arr44[] = {82, 56, 83, -1, -10, -10, -10, -10, -10, -10};
        Vector *v44 = array_to_vector(arr44, 10);
        pushBackVector(gr, *v44);
    }
    // Production 45: <option_single_constructed> ===> <oneExpansion> <moreExpansions>
    {
        int arr45[] = {83, 84, 85, -1, -10, -10, -10, -10, -10, -10};
        Vector *v45 = array_to_vector(arr45, 10);
        pushBackVector(gr, *v45);
    }
    // Production 46: <option_single_constructed> ===> <eps>
    {
        int arr46[] = {83, 110, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v46 = array_to_vector(arr46, 10);
        pushBackVector(gr, *v46);
    }
    // Production 47: <moreExpansions> ===> <oneExpansion> <moreExpansions>
    {
        int arr47[] = {85, 84, 85, -1, -10, -10, -10, -10, -10, -10};
        Vector *v47 = array_to_vector(arr47, 10);
        pushBackVector(gr, *v47);
    }
    // Production 48: <moreExpansions> ===> <eps>
    {
        int arr48[] = {85, 110, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v48 = array_to_vector(arr48, 10);
        pushBackVector(gr, *v48);
    }
    // Production 49: <oneExpansion> ===> TK_DOT TK_FIELDID
    {
        int arr49[] = {84, 49, 21, -1, -10, -10, -10, -10, -10, -10};
        Vector *v49 = array_to_vector(arr49, 10);
        pushBackVector(gr, *v49);
    }
    // Production 50: <funCallStmt> ===> <outputParameters> TK_CALL TK_FUNID TK_WITH TK_PARAMETERS <inputParameters> TK_SEM
    {
        int arr50[] = {86, 87, 50, 3, 51, 52, 88, 4, -1, -10};
        Vector *v50 = array_to_vector(arr50, 10);
        pushBackVector(gr, *v50);
    }
    // Production 51: <outputParameters> ===> TK_SQL <idList> TK_SQR TK_ASSIGNOP
    {
        int arr51[] = {87, 8, 106, 9, 23, -1, -10, -10, -10, -10};
        Vector *v51 = array_to_vector(arr51, 10);
        pushBackVector(gr, *v51);
    }
    // Production 52: <outputParameters> ===> <eps>
    {
        int arr52[] = {87, 110, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v52 = array_to_vector(arr52, 10);
        pushBackVector(gr, *v52);
    }
    // Production 53: <inputParameters> ===> TK_SQL <idList> TK_SQR
    {
        int arr53[] = {88, 8, 106, 9, -1, -10, -10, -10, -10, -10};
        Vector *v53 = array_to_vector(arr53, 10);
        pushBackVector(gr, *v53);
    }
    // Production 54: <iterativeStmt> ===> TK_WHILE TK_OP <booleanExpression> TK_CL <stmt> <otherStmts> TK_ENDWHILE
    {
        int arr54[] = {89, 24, 25, 100, 26, 80, 79, 55, -1, -10};
        Vector *v54 = array_to_vector(arr54, 10);
        pushBackVector(gr, *v54);
    }
    // Production 55: <conditionalStmt> ===> TK_IF TK_OP <booleanExpression> TK_CL TK_THEN <stmt> <otherStmts> <elsePart>
    {
        int arr55[] = {90, 27, 25, 100, 26, 28, 80, 79, 91, -1};
        Vector *v55 = array_to_vector(arr55, 10);
        pushBackVector(gr, *v55);
    }
    // Production 56: <elsePart> ===> TK_ELSE <stmt> <otherStmts> TK_ENDIF
    {
        int arr56[] = {91, 29, 80, 79, 30, -1, -10, -10, -10, -10};
        Vector *v56 = array_to_vector(arr56, 10);
        pushBackVector(gr, *v56);
    }
    // Production 57: <elsePart> ===> TK_ENDIF
    {
        int arr57[] = {91, 30, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v57 = array_to_vector(arr57, 10);
        pushBackVector(gr, *v57);
    }
    // Production 58: <ioStmt> ===> TK_READ TK_OP <var> TK_CL TK_SEM
    {
        int arr58[] = {92, 31, 25, 101, 26, 4, -1, -10, -10, -10};
        Vector *v58 = array_to_vector(arr58, 10);
        pushBackVector(gr, *v58);
    }
    // Production 59: <ioStmt> ===> TK_WRITE TK_OP <var> TK_CL TK_SEM
    {
        int arr59[] = {92, 32, 25, 101, 26, 4, -1, -10, -10, -10};
        Vector *v59 = array_to_vector(arr59, 10);
        pushBackVector(gr, *v59);
    }
    // Production 60: <arithmeticExpression> ===> <term> <expPrime>
    {
        int arr60[] = {93, 95, 94, -1, -10, -10, -10, -10, -10, -10};
        Vector *v60 = array_to_vector(arr60, 10);
        pushBackVector(gr, *v60);
    }
    // Production 61: <expPrime> ===> <lowPrecedenceOp> <term> <expPrime>
    {
        int arr61[] = {94, 98, 95, 94, -1, -10, -10, -10, -10, -10};
        Vector *v61 = array_to_vector(arr61, 10);
        pushBackVector(gr, *v61);
    }
    // Production 62: <expPrime> ===> <eps>
    {
        int arr62[] = {94, 110, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v62 = array_to_vector(arr62, 10);
        pushBackVector(gr, *v62);
    }
    // Production 63: <term> ===> <factor> <termPrime>
    {
        int arr63[] = {95, 97, 96, -1, -10, -10, -10, -10, -10, -10};
        Vector *v63 = array_to_vector(arr63, 10);
        pushBackVector(gr, *v63);
    }
    // Production 64: <termPrime> ===> <highPrecedenceOp> <factor> <termPrime>
    {
        int arr64[] = {96, 99, 97, 96, -1, -10, -10, -10, -10, -10};
        Vector *v64 = array_to_vector(arr64, 10);
        pushBackVector(gr, *v64);
    }
    // Production 65: <termPrime> ===> <eps>
    {
        int arr65[] = {96, 110, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v65 = array_to_vector(arr65, 10);
        pushBackVector(gr, *v65);
    }
    // Production 66: <factor> ===> TK_OP <arithmeticExpression> TK_CL
    {
        int arr66[] = {97, 25, 93, 26, -1, -10, -10, -10, -10, -10};
        Vector *v66 = array_to_vector(arr66, 10);
        pushBackVector(gr, *v66);
    }
    // Production 67: <factor> ===> <var>
    {
        int arr67[] = {97, 101, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v67 = array_to_vector(arr67, 10);
        pushBackVector(gr, *v67);
    }
    // Production 68: <lowPrecedenceOp> ===> TK_PLUS
    {
        int arr68[] = {98, 33, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v68 = array_to_vector(arr68, 10);
        pushBackVector(gr, *v68);
    }
    // Production 69: <lowPrecedenceOp> ===> TK_MINUS
    {
        int arr69[] = {98, 34, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v69 = array_to_vector(arr69, 10);
        pushBackVector(gr, *v69);
    }
    // Production 70: <highPrecedenceOp> ===> TK_MUL
    {
        int arr70[] = {99, 35, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v70 = array_to_vector(arr70, 10);
        pushBackVector(gr, *v70);
    }
    // Production 71: <highPrecedenceOp> ===> TK_DIV
    {
        int arr71[] = {99, 36, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v71 = array_to_vector(arr71, 10);
        pushBackVector(gr, *v71);
    }
    // Production 72: <booleanExpression> ===> TK_OP <booleanExpression> TK_CL <logicalOp> TK_OP <booleanExpression> TK_CL
    {
        int arr72[] = {100, 25, 100, 26, 102, 25, 100, 26, -1, -10};
        Vector *v72 = array_to_vector(arr72, 10);
        pushBackVector(gr, *v72);
    }
    // Production 73: <booleanExpression> ===> <var> <relationalOp> <var>
    {
        int arr73[] = {100, 101, 103, 101, -1, -10, -10, -10, -10, -10};
        Vector *v73 = array_to_vector(arr73, 10);
        pushBackVector(gr, *v73);
    }
    // Production 74: <booleanExpression> ===> TK_NOT TK_OP <booleanExpression> TK_CL
    {
        int arr74[] = {100, 37, 25, 100, 26, -1, -10, -10, -10, -10};
        Vector *v74 = array_to_vector(arr74, 10);
        pushBackVector(gr, *v74);
    }
    // Production 75: <var> ===> <SingleOrRecId>
    {
        int arr75[] = {101, 82, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v75 = array_to_vector(arr75, 10);
        pushBackVector(gr, *v75);
    }
    // Production 76: <var> ===> TK_NUM
    {
        int arr76[] = {101, 53, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v76 = array_to_vector(arr76, 10);
        pushBackVector(gr, *v76);
    }
    // Production 77: <var> ===> TK_RNUM
    {
        int arr77[] = {101, 54, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v77 = array_to_vector(arr77, 10);
        pushBackVector(gr, *v77);
    }
    // Production 78: <logicalOp> ===> TK_AND
    {
        int arr78[] = {102, 38, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v78 = array_to_vector(arr78, 10);
        pushBackVector(gr, *v78);
    }
    // Production 79: <logicalOp> ===> TK_OR
    {
        int arr79[] = {102, 39, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v79 = array_to_vector(arr79, 10);
        pushBackVector(gr, *v79);
    }
    // Production 80: <relationalOp> ===> TK_LT
    {
        int arr80[] = {103, 40, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v80 = array_to_vector(arr80, 10);
        pushBackVector(gr, *v80);
    }
    // Production 81: <relationalOp> ===> TK_LE
    {
        int arr81[] = {103, 41, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v81 = array_to_vector(arr81, 10);
        pushBackVector(gr, *v81);
    }
    // Production 82: <relationalOp> ===> TK_EQ
    {
        int arr82[] = {103, 42, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v82 = array_to_vector(arr82, 10);
        pushBackVector(gr, *v82);
    }
    // Production 83: <relationalOp> ===> TK_GT
    {
        int arr83[] = {103, 43, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v83 = array_to_vector(arr83, 10);
        pushBackVector(gr, *v83);
    }
    // Production 84: <relationalOp> ===> TK_GE
    {
        int arr84[] = {103, 44, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v84 = array_to_vector(arr84, 10);
        pushBackVector(gr, *v84);
    }
    // Production 85: <relationalOp> ===> TK_NE
    {
        int arr85[] = {103, 45, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v85 = array_to_vector(arr85, 10);
        pushBackVector(gr, *v85);
    }
    // Production 86: <returnStmt> ===> TK_RETURN <optionalReturn> TK_SEM
    {
        int arr86[] = {104, 46, 105, 4, -1, -10, -10, -10, -10, -10};
        Vector *v86 = array_to_vector(arr86, 10);
        pushBackVector(gr, *v86);
    }
    // Production 87: <optionalReturn> ===> TK_SQL <idList> TK_SQR
    {
        int arr87[] = {105, 8, 106, 9, -1, -10, -10, -10, -10, -10};
        Vector *v87 = array_to_vector(arr87, 10);
        pushBackVector(gr, *v87);
    }
    // Production 88: <optionalReturn> ===> <eps>
    {
        int arr88[] = {105, 110, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v88 = array_to_vector(arr88, 10);
        pushBackVector(gr, *v88);
    }
    // Production 89: <idList> ===> TK_ID <more_ids>
    {
        int arr89[] = {106, 56, 107, -1, -10, -10, -10, -10, -10, -10};
        Vector *v89 = array_to_vector(arr89, 10);
        pushBackVector(gr, *v89);
    }
    // Production 90: <more_ids> ===> TK_COMMA <idList>
    {
        int arr90[] = {107, 14, 106, -1, -10, -10, -10, -10, -10, -10};
        Vector *v90 = array_to_vector(arr90, 10);
        pushBackVector(gr, *v90);
    }
    // Production 91: <more_ids> ===> <eps>
    {
        int arr91[] = {107, 110, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v91 = array_to_vector(arr91, 10);
        pushBackVector(gr, *v91);
    }
    // Production 92: <definetypestmt> ===> TK_DEFINETYPE <A> TK_RUID TK_AS TK_RUID
    {
        int arr92[] = {108, 47, 109, 13, 48, 13, -1, -10, -10, -10};
        Vector *v92 = array_to_vector(arr92, 10);
        pushBackVector(gr, *v92);
        // printVector(v92);
    }
    // Production 93: <A> ===> TK_RECORD
    {
        int arr93[] = {109, 15, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v93 = array_to_vector(arr93, 10);
        pushBackVector(gr, *v93);
    }
    // Production 94: <A> ===> TK_UNION
    {
        int arr94[] = {109, 17, -1, -10, -10, -10, -10, -10, -10, -10};
        Vector *v94 = array_to_vector(arr94, 10);
        pushBackVector(gr, *v94);
    }
    // printVectorOfVector(gr);

    grammar G = (grammar)malloc(sizeof(struct grammar));
    G->Grammar = (VectorOfVector *)malloc(sizeof(VectorOfVector));
    initVectorOfVector(G->Grammar);
    // G->Grammar=gr;
    for (int i = 0; i < GRAMMAR_SIZE; i++)
    {
        Vector v;
        initVector(&v);
        for (int j = 0; j < 10; j++)
        {
            pushBack(&v, accessVectorOfVector(gr, i, j));

        }

        pushBackVector(G->Grammar, v);
    }

    return G;
}


/**
 * @brief Main function for the parser module.
 *
 * This function initializes the necessary components for the parser, including
 * grammar, FIRST and FOLLOW sets, and the parse table. It then parses the input
 * source code and generates the parse tree if the code is syntactically correct.
 * The results are printed to various output files.
 *
 * @param testfile Path to the input source code file to be parsed.
 * @param outfile Path to the output file where the parse tree will be printed if the code is syntactically correct.
 */
void parser_main(char *testfile,char* outfile)
{
    initTableLogFile();
    initLogFile();
     if (parser_called == 0)
    {
        printf("[INFO] Initializing grammar...\n");
        G = initialize_grammar();
        printf("[INFO] Computing FIRST and FOLLOW sets...\n");
        F = ComputeFirstAndFollowSets(G);
        printf("[INFO] First and Follow sets automated \n");
         printf("[INFO] Initializing parse table...\n");
        T = initialize_table(F);
        createParseTable(F, T,G);
        parser_called = 1;
    }



    print_first_set(F, G);
    printf("[INFO] First Set has been printed successfully in first_out.txt ...\n");
    print_follow_set(F, G);
    printf("[INFO] Follow Set has been printed successfully in follow_out.txt ...\n");

    printTable(T);
    printf("[INFO] Parse table has been printed successfully in parse_table_ouput.txt ...\n");
    char dummy[5] = "----\0";
    TreeNode* root=parseInputSourceCode(testfile,T,G);
    printf("[INFO] Entire Parsing Process and Logic is printed successfully in parser_output.txt ...\n");
    if(issyntaxcorrect){ 
         printf("[INFO] Code is syntactically correct so parse tree is generated successfully in %s ...\n\n",outfile);
        printParseTree(root,outfile);}
    else{
        printf("[INFO] Code is syntactically incorrect so parse tree is constructed but printParseTree is not called\n\n");
    }
    freeSyntaxTree(root);
    closeLogFile();
    closeTableLogFile();
    // printf("[] Testing complete!\n");
}