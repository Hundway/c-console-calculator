#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define BUFFER_SIZE_DOUBLE 24

bool isNumber(const char character);
bool isOperator(const char character);
bool isInteger(double number);

bool validExpression(char** argugments, int size);

char* getNumber(char* string, int pos);
char* getOperator(char* string, int pos);
char** createExpression(char** arguments, int argc, int* size);

void removeFromExpression(char** expression, int* size, int position);
void reduceExpression(char** expression, int* size, double result, int operatorPosition);
int calculate(char** expression, int* size, int start, int end, double* result);

void freeExpression(char** expression, int size);

#endif
