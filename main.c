#include <stdio.h>
#include <stdlib.h>
#include "Functions.h"

int main(int argc, char* argv[]){
    int size;
    char** expression;
    double result;

    if(argc == 1){
        printf("Usage : %s <number> <operator> <number>\n", argv[0]);
        printf("Valid Operators : + - x / ( )\n");
        return EXIT_FAILURE;
    }

    expression = createExpression(argv, argc, &size);
    if(expression == NULL)
        return EXIT_FAILURE;
    
    calculate(expression, &size, 0, size, &result);

    if(isInteger(result))
        printf("\nResult : %.lf\n", result);
    else
        printf("\nResult : %.2lf\n", result);

    freeExpression(expression, size);
    return EXIT_SUCCESS;
}