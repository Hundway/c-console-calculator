#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void reduceExpression(char* expression[], int* size, double result, int operatorPosition);
void removeFromExpression(char* expression[], int* size, int position);
double calculate(char* expression[], int* size, int start, int end);
void resolveParenthesis(char* expression[], int* size, int start, int end);
bool isInteger(double number);

int main(int argc, char* argv[]){
    double result;

    if(argc == 1){
        printf("Usage : %s <number> <operator> <number>\n", argv[0]);
        printf("Valid Operators : + - x / ( )\n");
        printf("You must put a space between a number and an operator\n");
        return EXIT_SUCCESS;
    }

    result = calculate(argv, &argc, 1, argc);

    if(isInteger(result))
        printf("\nResult : %.lf\n", result);
    else
        printf("\nResult : %.2lf\n", result);

    return EXIT_SUCCESS;
}

void reduceExpression(char* expression[], int* size, double result, int operatorPosition){
    char temp[4];
    // Storing the result of the operation before the operator
    gcvt(result, 6, temp);
    strcpy(expression[operatorPosition-1], temp);
    // Remove the operator and the number after it from expression
    removeFromExpression(expression, size, operatorPosition + 1);
    removeFromExpression(expression, size, operatorPosition);
}

void removeFromExpression(char* expression[], int* size, int position){
    int i;
    for(i=position; i < (*size)-1; i++)
        strcpy(expression[i], expression[i+1]);
    (*size)--;
}

double calculate(char* expression[], int* size, int start, int end){
    int i, j;

    for(i=start; i<end; i++)
        if(strcmp(expression[i], "(") == 0){
            printf("Parenthesis found.\n");
            // Search ")"
            for(j=end - 1; strcmp(expression[j], ")") != 0 && j > i; j--);

            if(strcmp(expression[j], ")") == 0){
                // Calculate expresion inside parenthesis
                calculate(expression, size, i+1, j);
                // Remove parenthesis
                removeFromExpression(expression, size, i+2);
                removeFromExpression(expression, size, i);
                end -= 2;
            }
            else{
                printf("End of parenthesis not found.\n");
                printf("Invalid expression !\n");
                exit(1);
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
    
    return atof(expression[1]);
}

bool isInteger(double number){
    int integer = (int) number;
    return (number == integer);
}
