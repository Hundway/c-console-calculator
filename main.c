#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "header.h"

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
    result = calculate(expression, &size, 0, size);

    if(isInteger(result))
        printf("\nResult : %.lf\n", result);
    else
        printf("\nResult : %.2lf\n", result);

    freeExpression(expression, size);
    return EXIT_SUCCESS;
}

bool isNumber(const char character){
    char* numbers = "0123456789.";
    bool isNumber = false;
    for(int i=0; i<11 && !isNumber; i++)
        if(character == numbers[i])
            isNumber = true;
    return isNumber;
}

bool isOperator(const char character){
    char* operators = "+-x/()";
    bool isOperator = false;
    for(size_t i=0; i<strlen(operators) && !isOperator; i++)
        if(character == operators[i])
            isOperator = true;
    return isOperator;
}

bool isInteger(double number){
    int integer = (int) number;
    return (number == integer);
}

bool validExpression(char** arguments, int size){
    int i, j, k;
    bool isValid;
    char* validInputs = "123456789+-x/().";

    for(i=1; i<size; i++){
        j = 0;
        while(arguments[i][j] != '\0' && isValid){
            k = 0;
            isValid = false;
            while(validInputs[k] != '\0' && !isValid){
                if(arguments[i][j] != validInputs[k])
                    isValid = true;
                k++;
            }
            j++;
        }
    }
    return isValid;
}

char* getNumber(char* string, int pos){
    int size = 0;
    int i = pos;
    char* numberString;

    // Get needed size for allocation
    while(isNumber(string[i++]))
        size++;

    numberString = (char*) malloc((size+1) * sizeof(char));
    if(numberString == NULL){
        printf("Insuficient memory !\n");
        return NULL;
    }

    // Get number from input string
    i = 0;
    while(isNumber(string[pos]))
        numberString[i++] = string[pos++];

    numberString[i] = '\0';
    return numberString;
}

char* getOperator(char* string, int pos){
    char *operatorString = (char*) malloc(2 * sizeof(char));
    if(operatorString == NULL){
        printf("Insuficient memory !\n");
        return NULL;
    }
    operatorString[0] = string[pos];
    operatorString[1] = '\0';
    return operatorString;
}

char** createExpression(char** argv, int argc, int* size){
    if(!validExpression(argv, argc)){
        printf("Invalid expression !\n");
        return NULL;
    }

    int i, j;
    int arguments = 1, charCounter = 0;
    char* temp;
    char** expression;

    // Get the number of stored characters
    for(i=1; i<argc; i++)
        charCounter += sizeof(char) * strlen(argv[i]);

    temp = (char*) malloc(charCounter * sizeof(char));
    if(temp == NULL){
        printf("Insuficient memory !\n");
        return NULL;
    }

    // Store everything on a single string
    temp[0] = '\0';
    for(i=1; i<argc; i++)
        strcat(temp, argv[i]);

    // Get the real number of arguments in expression
    i = 1;
    while(temp[i++] != '\0'){
        if(isOperator(temp[i]))
            arguments++;
        else if(isNumber(temp[i]) && isOperator(temp[i-1]))
            arguments++;
    }

    expression = (char**) malloc(arguments * sizeof(char*));
    if(expression == NULL){
        printf("Insuficient memory !\n");
        free(temp);
        return NULL;
    }

    // Store all arguments in expression
    // Each string will be a single argument (number or operator)
    if(isNumber(temp[0]))
        expression[0] = getNumber(temp, 0);
    else
        expression[0] = NULL;
    i = 1;
    j = 1;
    while(temp[i++] != '\0'){
        if(isOperator(temp[i]))
            expression[j++] = getOperator(temp, i);
        else if(isNumber(temp[i]) && isOperator(temp[i-1]))
            expression[j++] = getNumber(temp, i);
    }

    // Verify if any argument was not stored
    for(i=0; i < arguments; i++){
        if(expression[i] == NULL){
            printf("Error while reading expression");
            freeExpression(expression, *size);
            return NULL;
        }
    }

    free(temp);
    *size = arguments;
    return expression;

}

void removeFromExpression(char** expression, int* size, int position){
    int i;
    for(i=position; i < (*size)-1; i++)
        strcpy(expression[i], expression[i+1]);
    (*size)--;
}

void reduceExpression(char** expression, int* size, double result, int operatorPosition){
    char temp[4];
    // Storing the result of the operation before the operator
    gcvt(result, 6, temp);
    strcpy(expression[operatorPosition-1], temp);
    // Remove the operator and the number after it from expression
    removeFromExpression(expression, size, operatorPosition + 1);
    removeFromExpression(expression, size, operatorPosition);
}

double calculate(char** expression, int* size, int start, int end){
    int i, j;

    for(i=start; i<end; i++)
        if(strcmp(expression[i], "(") == 0){
            printf("Parenthesis found.\n");
            // Search ")"
            for(j=end - 1; strcmp(expression[j], ")") != 0 && j > i; j--);

            if(strcmp(expression[j], ")") == 0){
                // Calculate expression inside parenthesis
                calculate(expression, size, i+1, j);
                // Remove parenthesis
                removeFromExpression(expression, size, i+2);
                removeFromExpression(expression, size, i);
                end -= 2;
            }
            else{
                printf("End of parenthesis not found.\n");
                printf("Invalid expression !\n");
                return EXIT_FAILURE;
            }
        }
    for(i=start; i<end; i++)
        if(strcmp(expression[i], "x") == 0){
            printf("Multiplying %.2lf x %.2lf.\n", atof(expression[i-1]), atof(expression[i+1]));
            reduceExpression(expression, size, atof(expression[i-1]) * atof(expression[i+1]), i);
            end -= 2;
        }
    for(i=start; i<end; i++)
        if(strcmp(expression[i], "/") == 0){
            printf("Dividing %.2lf / %.2lf.\n", atof(expression[i-1]), atof(expression[i+1]));
            reduceExpression(expression, size, atof(expression[i-1]) / atof(expression[i+1]), i);
            end -= 2;
        }
    for(i=start; i<end; i++)
        if(strcmp(expression[i], "+") == 0){
            printf("Adding %.2lf + %.2lf.\n", atof(expression[i-1]), atof(expression[i+1]));
            reduceExpression(expression, size, atof(expression[i-1]) + atof(expression[i+1]), i);
            end -= 2;
        }
    for(i=start; i<end; i++)
        if(strcmp(expression[i], "-") == 0){
            printf("Subtracting %.2lf - %.2lf.\n", atof(expression[i-1]), atof(expression[i+1]));
            reduceExpression(expression, size, atof(expression[i-1]) - atof(expression[i+1]), i);
            end -= 2;
        }

    return atof(expression[0]);
}

void freeExpression(char** expression, int size){
    for(int i=0; i<size; i++)
        free(expression[i]);
    free(expression);
}
