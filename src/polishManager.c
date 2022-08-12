#include "polishManager.h"

int checkNum(char ch) {
    return (ch >= '0' && ch <= '9');
}

int findDot(char ch, int *dot) {
    if (ch == '.') {
        (*dot)++;
    }
    return ch == '.';
}

double getOperand(char *ptr, int *index) {
    char *tmp = ptr;
    double result = 0.0;
    int count = 0, dotFlag = 0;
    while (checkNum(*tmp) || findDot(*tmp, &dotFlag)) {
        count++;
        tmp++;
    }
    if (dotFlag > 1) {
        *index = -1;
    } else {
        *index += count - 1;
        char *number = (char*)malloc(sizeof(char) * count + 1);
        memcpy(number, ptr, count);
        sscanf(number, "%lf", &result);
        free(number);
    }
    return result;
}

int findFunctions(char *ptr, int *len) {
    int result = 0;
    *len = 0;
    if (!(strncmp(ptr, "cos", 3))) {
        result = 1;
        *len = 3;
    } else if (!(strncmp(ptr, "sin", 3))) {
        result = 1;
        *len = 3;
    } else if (!(strncmp(ptr, "tan", 3))) {
        result = 1;
        *len = 3;
    } else if (!(strncmp(ptr, "acos", 4))) {
        result = 1;
        *len = 4;
    } else if (!(strncmp(ptr, "asin", 4))) {
        result = 1;
        *len = 4;
    } else if (!(strncmp(ptr, "atan", 4))) {
        result = 1;
        *len = 4;
    } else if (!(strncmp(ptr, "sqrt", 4))) {
        result = 1;
        *len = 4;
    } else if (!(strncmp(ptr, "ln", 2))) {
        result = 1;
        *len = 2;
    } else if (!(strncmp(ptr, "log", 3))) {
        result = 1;
        *len = 3;
    }
    return result;
}

void transportStackValue(stack **dst, stack **src, size_t len) {
    char *ptr = (char*)malloc(sizeof(char) * len);
    memcpy(ptr, (*src)->value, len);
    *dst = pushStack(*dst, ptr, (*src)->ch);
    popStack(src);
}

void changeControlStatus(control *ctrl, int i1, int i2, int i3, int i4) {
    ctrl->operation = i1;
    ctrl->unary = i2;
    ctrl->function = i3;
    ctrl->number = i4;
}

void stopConvert(stack **res, stack **operation, void *ptr, int *index, int len) {
    *index = len;
    clearStack(res);
    clearStack(operation);
    free(ptr);
}

void unaryOperation(stack **result, stack **operation, char *oper) {
    double *zero = (double*)malloc(sizeof(double));
    double tmp = 0.0;
    *zero = tmp;
    *result = pushStack(*result, zero, NUMBER);
    *operation = pushStack(*operation, oper, OPERATION);
}

void pushWithPriority(stack **result, stack **operation, int prior) {
    int flag = 1;
    size_t len = strlen((char*)(*operation)->value);
    while (flag) {
        char symbol = *(char*)(*operation)->value;
        if (prior >= priority(symbol) || (*operation)->ch == FUNCTION)
            transportStackValue(result, operation, len);
        else
            flag = 0;
        if (*operation == NULL)
            flag = 0;
    }
}

void workWithNumbers(stack **result, stack **operation, control *ctrl, char *exp, int *i, int len) {
    if (ctrl->number == 1) {
        changeControlStatus(ctrl, 1, 0, 0, 0);
        double *op = (double*)malloc(sizeof(double));
        *op = getOperand(exp, i);
        if (*i != -1) {
            *result = pushStack(*result, op, NUMBER);
        } else {
            stopConvert(result, operation, op, i, len);
        }
    } else {
        stopConvert(result, operation, NULL, i, len);
    }
}

void workWithFunction(stack **result, stack **operation, control *ctrl, char *exp, int *i, int len,
                      int funcLen) {
    if (ctrl->function == 1) {
        changeControlStatus(ctrl, 0, 1, 1, 1);
        char *func = (char*)malloc(sizeof(char) * funcLen + 1);
        memcpy(func, exp, funcLen);
        *operation = pushStack(*operation, func, FUNCTION);
        *i += funcLen - 1;
    } else {
        stopConvert(result, operation, NULL, i, len);
    }
}

void workWithBrackets(stack **operation, control *ctrl, const char *exp) {
    changeControlStatus(ctrl, 0, 1, 1, 1);
    char *bracket = (char*)malloc(sizeof(char));
    *bracket = *exp;
    *operation = pushStack(*operation, bracket, OPERATION);
}

void workWithOperations(stack **result, stack **operation, control *ctrl, char *exp, int *i, int len) {
    int prior = priority(*exp);
    char *oper = (char*)malloc(sizeof(char));
    *oper = *exp;
    if (ctrl->operation == 1) {
        changeControlStatus(ctrl, 0, 0, 1, 1);
        if (*operation != NULL) {
            pushWithPriority(result, operation, prior);
        }
        *operation = pushStack(*operation, oper, OPERATION);
    } else if (ctrl->unary == 1 && prior == 3) {
        changeControlStatus(ctrl, 0, 0, 1, 1);
        unaryOperation(result, operation, oper);
    } else {
        stopConvert(result, operation, NULL, i, len);
        free(oper);
    }
}

void workWithClosingBrackets(stack **result, stack **operation, int *i, int len) {
    int flag = 1;
    while (flag == 1) {
        if (*operation != NULL) {
            char symbol = *(char*)(*operation)->value;
            if (symbol != '(') {
                transportStackValue(result, operation, strlen((char*)(*operation)->value));
            } else {
                popStack(operation);
                flag = 0;
            }
        } else {
            flag = 2;
        }
    }
    if (flag == 2) {
        stopConvert(result, operation, NULL, i, len);
    }
}

void pushWithoutPriority(stack **result, stack **operation) {
    while (*operation != NULL) {
        char symbol = *(char*)(*operation)->value;
        int len = strlen((char*)(*operation)->value);
        if (symbol == '(') {
            int tmp = 0;
            stopConvert(result, operation, NULL, &tmp, 0);
        } else {
            transportStackValue(result, operation, len);
        }
    }
}

void workWithX(stack **result, stack **operation, control *ctrl, char *exp, int *i, int len) {
    if (ctrl->number == 1) {
        changeControlStatus(ctrl, 1, 0, 0, 0);
        char *tmp = (char*)malloc(sizeof(char) + 1);
        memcpy(tmp, exp, 2);
        *result = pushStack(*result, tmp, UNKNOWN_X);
    } else {
        stopConvert(result, operation, NULL, i, len);
    }
}

stack *polishConverter(char *expression) {
    stack *result = NULL, *operation = NULL;
    int lenght = (int)strlen(expression);
    int funcLen = 0;
    control ctrl = { 0, 1, 1, 1 };
    for (int i = 0; i < lenght; i++) {
        if (checkNum(expression[i])) {
            workWithNumbers(&result, &operation, &ctrl, &expression[i], &i, lenght);
        } else if (findFunctions(&expression[i], &funcLen)) {
            workWithFunction(&result, &operation, &ctrl, &expression[i], &i, lenght, funcLen);
        } else if (expression[i] == '(') {
            workWithBrackets(&operation, &ctrl, &expression[i]);
        } else if (priority(expression[i]) != 0) {
            workWithOperations(&result, &operation, &ctrl, &expression[i], &i, lenght);
        } else if (expression[i] == ')') {
            workWithClosingBrackets(&result, &operation, &i, lenght);
        } else if (expression[i] == 'x') {
            workWithX(&result, &operation, &ctrl, &expression[i], &i, lenght);
        } else if (expression[i] != ' ' &&
                   expression[i] != '\n') {
            stopConvert(&result, &operation, NULL, &i, lenght);
        }
    }
    pushWithoutPriority(&result, &operation);
    return result;
}

int priority(char symbol) {
    int result = 0;
    if (symbol == '^')
        result = 1;
    else if (symbol == '*' ||
        symbol == '/' ||
        symbol == '%')
        result = 2;
    else if (symbol == '+' ||
        symbol == '-')
        result = 3;
    else if (symbol == '(')
        result = 4;
    return result;
}
