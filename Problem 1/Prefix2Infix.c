#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct StackNode {
    char *data;
    struct StackNode *next;
} StackNode;

void push(StackNode **top, char *data) {
    StackNode *newNode = malloc(sizeof(StackNode));
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

char *prefixToInfix(char *prefix) {
    StackNode *top = NULL;
    int len = strlen(prefix);
    // Traverse the prefix expression from right to left
    for (int i = len - 1; i >= 0; i--) {
        char current = prefix[i];
        if (isOperator(current)) {
            char *op1 = pop(&top);  // Pop the top operand
            char *op2 = pop(&top);
            char *expression = malloc(strlen(op1) + strlen(op2));
            sprintf(expression, "%s%c%s", op1, current, op2);
            push(&top, expression);
        } else {
            // It's an operand
            char *operand = malloc(2);
            operand[0] = current;
            operand[1] = '\0';
            push(&top, operand);
        }
    }
    return pop(&top);
}
int main() {
    char prefixExpr[110];
    scanf("%s", prefixExpr);
    // printf("%s\n", prefixExpr);
    char *infixExpr = prefixToInfix(prefixExpr);
    printf("%s", infixExpr);
    free(infixExpr);
    return 0;
}