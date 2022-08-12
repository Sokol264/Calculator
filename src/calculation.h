#ifndef SRC_CALCULATION_H_
#define SRC_CALCULATION_H_

#include "stackManager.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define E 1e-3

double calculate(stack **notation);
int checkBinaryOperands(stack *ptr);
int checkX(stack *ptr);
void replaceXWithValue(stack **notation, double *value);

#endif  // SRC_CALCULATION_H_
