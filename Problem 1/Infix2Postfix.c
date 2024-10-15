#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct StackNode {
    char *data;
    struct StackNode *next;
} StackNode;

void push(StackNode **top, char *data) {
    StackNode *newNode = (StackNode *)malloc(sizeof(StackNode));
    newNode->data = data;
    newNode->next = *top;
    *top = newNode;
}

char *pop(StackNode **top) {
    if (*top == NULL) {
        return NULL;
    }
    StackNode *temp = *top;
    char *data = temp->data;
    *top = temp->next;
    free(temp);
    return data;
}

int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

char *infixToPostfix(char *infix) {
    StackNode *top = NULL;
    int len = strlen(infix);
    for (int i = 0; i < len; i++) {
        char current = infix[i];
        if (isOperator(current)) {
            char *nextOperand = (char *)malloc(2);
            nextOperand[0] = infix[i + 1];
            nextOperand[1] = '\0';
            push(&top, nextOperand);

            char *operator=(char *) malloc(2);
            operator[0] = current;
            operator[1] = '\0';
            push(&top, operator);
            // Jump over the next operand
            i++;
        } else {
            char *operand = (char *)malloc(2);
            operand[0] = current;
            operand[1] = '\0';
            push(&top, operand);
        }
    }

    char *result = (char *)malloc(len + 1);
    int index = len - 1;  // Start from the end of the string
    while (top != NULL) {
        char *popped = pop(&top);
        result[index--] = popped[0];
        free(popped);
    }
    result[index] = '\0';
    return result;
}

int main() {
    char infixExpr[] = " a*b/c";
    printf("Infix Expression: %s\n", infixExpr);
    char *postfixExpr = infixToPostfix(infixExpr);
    printf("Postfix Expression: %s\n", postfixExpr);
    free(postfixExpr);
    return 0;
}