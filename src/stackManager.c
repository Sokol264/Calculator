#include "stackManager.h"

stack *pushStack(stack *head, void *value, int choice) {
    stack *ptr = (stack*)malloc(sizeof(stack));
    ptr->ch = choice;
    ptr->value = value;
    ptr->next = head;
    return ptr;
}

void insertStack(stack **curr, stack **prev, void *value, int choice) {
    stack *ptr = (stack*)malloc(sizeof(stack));
    ptr->ch = choice;
    ptr->value = value;
    ptr->next = *curr;
    *curr = ptr;
    (*prev)->next = ptr;
}

void deleteFromStack(stack **curr, stack **prev) {
    if (*curr != NULL) {
        stack *tmp = *curr;
        (*prev)->next = (*curr)->next;
        *curr = (*prev)->next;
        free(tmp->value);
        free(tmp);
    }
}

void popStack(stack **head) {
    if (*head != NULL) {
        stack *tmp = *head;
        *head = tmp->next;
        free(tmp->value);
        free(tmp);
    }
}

void clearStack(stack **head) {
    while (*head != NULL) {
        stack *tmp = *head;
        *head = tmp->next;
        free(tmp->value);
        free(tmp);
    }
}
