#ifndef SRC_POLISHMANAGER_H_
#define SRC_POLISHMANAGER_H_

#include "stackManager.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    int operation;
    int unary;
    int function;
    int number;
} control;

stack *polishConverter(char *expression);
int priority(char symbol);
int checkNum(char ch);
int findDot(char ch, int *dot);

#endif  // SRC_POLISHMANAGER_H_

