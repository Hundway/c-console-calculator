#ifndef HEADER_H
#define HEADER_H

bool isNumber(const char character);
bool isOperator(const char character);
bool isInteger(double number);

bool validExpression(char** argugments, int size);

char* getNumber(char* string, int pos);
char* getOperator(char* string, int pos);
char** createExpression(char** arguments, int argc, int* size);

void removeFromExpression(char** expression, int* size, int position);
void reduceExpression(char** expression, int* size, double result, int operatorPosition);
double calculate(char** expression, int* size, int start, int end);

void freeExpression(char** expression, int size);

#endif