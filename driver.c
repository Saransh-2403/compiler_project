

/**
 * @file driver.c
 * @brief This file contains the main driver program for the Compiler Project.
 * 
 * The program provides a command-line interface to perform various tasks such as:
 * - Removing comments from the input file.
 * - Printing tokens from the input file.
 * - Parsing the input file.
 * - Measuring the time taken for lexing and parsing.
 * 
 * 
 * The program will prompt the user to enter a command to perform the desired task.
 * 
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * 
 * @return EXIT_SUCCESS on successful execution, EXIT_FAILURE on error.
 */
#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "parser.h"


int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <input_file> <ouput_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    while (1)
    {
        int input;
        printf("Enter 0 to exit, 1 to remove comments, 2 to print tokens, 3 to get parse, 4 to get time\n");
        scanf("%d", &input);

        switch (input)
        {
        case 0:
            printf("Exit program\n");
            return EXIT_SUCCESS;
            break;
        case 1:
           // driverToken("sample.txt");
            removeComments(argv[1],argv[2]);
            printf("[INFO] Comments removed successfully and have been written to %s\n",argv[2]);
            break;
        case 2:
            //FOR GETTING THE OUPTUT OF tokens
            driverToken(argv[1],1);
            break;
        case 3:
            parser_main(argv[1],argv[2]);
            break;
        case 4:
        {
            clock_t start_time, end_time;
            double total_CPU_time, total_CPU_time_in_seconds;
            start_time = clock();
            parser_main(argv[1], argv[2]);
            end_time = clock();

            total_CPU_time = (double)(end_time - start_time);
            total_CPU_time_in_seconds = total_CPU_time / CLOCKS_PER_SEC;

            // Print both total_CPU_time and total_CPU_time_in_seconds
            printf("\nTotal CPU time for Lexer and Parser is: %f\n", total_CPU_time);
            printf("Total CPU time in seconds for Lexer and Parser: %f\n", total_CPU_time_in_seconds);
            printf("Clocks per second of processor: %f\n\n", CLOCKS_PER_SEC);
        }
        break;

        default:
            printf("Exit program\n");
            return EXIT_SUCCESS;
            break;
        }
    }
}
