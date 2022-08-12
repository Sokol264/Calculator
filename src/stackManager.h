#ifndef SRC_STACKMANAGER_H_
#define SRC_STACKMANAGER_H_

#include <stdio.h>
#include <stdlib.h>

#define OPERATION 0
#define FUNCTION 1
#define NUMBER 2
#define UNKNOWN_X 3

typedef struct stack_type {
    void *value;
    struct stack_type *next;
    int ch;
} stack;

stack *pushStack(stack *head, void *value, int choice);
void popStack(stack **head);
void clearStack(stack **head);
void insertStack(stack **curr, stack **prev, void *value, int choice);
void deleteFromStack(stack **curr, stack **prev);

#endif  // SRC_STACKMANAGER_H_
