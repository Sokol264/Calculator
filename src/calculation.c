#include "calculation.h"

int checkBinaryOperands(stack *ptr) {
    int result = 0;
    if (ptr != NULL &&
        ptr->next != NULL &&
        ptr->next->next != NULL) {
        if (ptr->next->ch == NUMBER &&
            ptr->next->next->ch == NUMBER)
            result = 1;
    } else {
        result = -1;
    }
    return result;
}

int checkFunctionOperand(stack *ptr) {
    int result = 0;
    if (ptr != NULL &&
        ptr->next != NULL) {
        if (ptr->next->ch == NUMBER)
            result = 1;
    } else {
        result = -1;
    }
    return result;
}

double resultOfBinaryOperation(double first, double second, char operation) {
    double result;
    if (operation == '*')
        result = first * second;
    else if (operation == '/')
        result = first / second;
    else if (operation == '+')
        result = first + second;
    else if (operation == '-')
        result = first - second;
    else if (operation == '%')
        result = fmod(first, second);
    else
        result = pow(first, second);
    return result;
}

double resultOfFunction(double first, char *operation) {
    double result = 0;
    if (strcmp(operation, "sin") == 0) {
        result = sinl(first);
    } else if (strcmp(operation, "cos") == 0) {
        result = cosl(first);
    } else if (strcmp(operation, "asin") == 0) {
        result = asinl(first);
    } else if (strcmp(operation, "acos") == 0) {
        result = acosl(first);
    } else if (strcmp(operation, "tan") == 0) {
        result = tanl(first);
        double check = fabs(atanl(result));
        check = fabs(check - M_PI_2);
        if (check < E) result = 0 / 0.0;
    } else if (strcmp(operation, "atan") == 0) {
        result = atanl(first);
    } else if (strcmp(operation, "sqrt") == 0) {
        result = sqrtl(first);
    } else if (strcmp(operation, "ln") == 0) {
        result = logl(first);
    } else if (strcmp(operation, "log") == 0) {
        result = log10l(first);
    }
    return result;
}

void removeValueFromStack(stack **ptr, stack **prev, stack **notation) {
    if (*ptr == *notation) {
        popStack(notation);
        *ptr = *notation;
    } else {
        deleteFromStack(ptr, prev);
    }
}

void pushAnswerIntoStack(stack **ptr, stack **prev, stack **notation, double *answer) {
    if (*notation == NULL) {
        *notation = pushStack(*notation, answer, NUMBER);
        *ptr = *notation;
    } else {
        insertStack(ptr, prev, answer, NUMBER);
    }
}

void replaceOperationWithAnswer(stack **ptr, stack **prev, stack **notation) {
    int check = checkBinaryOperands(*ptr);
    if (check == 1) {
        double firstNum, secondNum;
        char operation;
        operation = *(char*)(*ptr)->value;
        removeValueFromStack(ptr, prev, notation);
        secondNum = *(double*)(*ptr)->value;
        removeValueFromStack(ptr, prev, notation);
        firstNum = *(double*)(*ptr)->value;
        removeValueFromStack(ptr, prev, notation);
        double *ptrRes = (double*)malloc(sizeof(double));
        double res = resultOfBinaryOperation(firstNum, secondNum, operation);
        *ptrRes = res;
        pushAnswerIntoStack(ptr, prev, notation, ptrRes);
    } else if (check == -1) {
        double *ptrRes = (double*)malloc(sizeof(double));
        double res = 0 / 0.0;
        *ptrRes = res;
        clearStack(notation);
        *notation = pushStack(*notation, ptrRes, NUMBER);
    }
}

void replaceFunctionWithAnswer(stack **ptr, stack **prev, stack **notation) {
    int check = checkFunctionOperand(*ptr);
    if (check == 1) {
        double firstNum;
        char *function;
        int len = strlen((char*)(*ptr)->value);
        function = (char*)malloc(sizeof(char) * len);
        memcpy(function, (char*)(*ptr)->value, len + 1);
        removeValueFromStack(ptr, prev, notation);
        firstNum = *(double*)(*ptr)->value;
        removeValueFromStack(ptr, prev, notation);
        double *ptrRes = (double*)malloc(sizeof(double));
        double res = resultOfFunction(firstNum, function);
        *ptrRes = res;
        pushAnswerIntoStack(ptr, prev, notation, ptrRes);
        free(function);
    } else if (check == -1) {
        double *ptrRes = (double*)malloc(sizeof(double));
        double res = 0 / 0.0;
        *ptrRes = res;
        clearStack(notation);
        *notation = pushStack(*notation, ptrRes, NUMBER);
    }
}

int checkX(stack *ptr) {
    int result = 0;
    while (ptr != NULL) {
        if (ptr->ch == UNKNOWN_X)
            result = 1;
        ptr = ptr->next;
    }
    return result;
}

void replaceXWithValue(stack **notation, double *value) {
    stack *ptr = *notation;
    stack *prev = ptr;
    while (ptr != NULL) {
        if (ptr->ch == UNKNOWN_X) {
            removeValueFromStack(&ptr, &prev, notation);
            double *tmp = (double*)malloc(sizeof(double));
            memcpy(tmp, value, sizeof(double));
            pushAnswerIntoStack(&ptr, &prev, notation, tmp);
        }
        prev = ptr;
        ptr = ptr->next;
    }
}

double calculate(stack **notation) {
    double result;
    stack *prev = *notation;
    if (*notation != NULL) {
        if ((*notation)->next == NULL && (*notation)->ch != NUMBER) {
            double *ptrRes = (double*)malloc(sizeof(double));
            double res = 0 / 0.0;
            *ptrRes = res;
            clearStack(notation);
            *notation = pushStack(*notation, ptrRes, NUMBER);
        }
        while ((*notation)->next != NULL) {
            stack *ptr = *notation;
            while (ptr != NULL) {
                if (ptr->ch == OPERATION) {
                    replaceOperationWithAnswer(&ptr, &prev, notation);
                } else if (ptr->ch == FUNCTION) {
                    replaceFunctionWithAnswer(&ptr, &prev, notation);
                }
                prev = ptr;
                ptr = ptr->next;
            }
        }
        result = *(double*)(*notation)->value;
        popStack(notation);
    } else {
        result = 0 / 0.0;
    }
    return result;
}
