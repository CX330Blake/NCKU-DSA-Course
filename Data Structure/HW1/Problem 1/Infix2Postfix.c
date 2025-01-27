#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 110

typedef struct StackNode {
    char data;
    struct StackNode *next;
} StackNode;

void push(StackNode **top, char data) {
    StackNode *newNode = malloc(sizeof(StackNode));
    newNode->data = data;
    newNode->next = *top;
    *top = newNode;
}

char pop(StackNode **top) {
    if (*top == NULL) {
        return '\0';
    }
    StackNode *temp = *top;
    char data = temp->data;
    *top = temp->next;
    free(temp);
    return data;
}

char peek(StackNode *top) {
    if (top == NULL) {
        return '\0';
    }
    return top->data;
}

int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

int precedence(char c) {
    if (c == '+' || c == '-') {
        return 1;
    }
    if (c == '*' || c == '/') {
        return 2;
    }
    return 0;
}

char *infixToPostfix(char *infix) {
    StackNode *stack = NULL;
    char *postfix = malloc(MAX_SIZE);
    int index = 0;  // Index for postfix array

    for (int i = 0; i < strlen(infix); i++) {
        char current = infix[i];

        // If the scanned character is an operand, add it to output
        if (isalnum(current)) {
            postfix[index++] = current;
        } else if (isOperator(current)) {
            // If an operator is scanned
            while (stack != NULL &&
                   precedence(peek(stack)) >= precedence(current)) {
                postfix[index++] = pop(&stack);
            }
            push(&stack, current);
        }
    }

    // Pop all the remaining operators from the stack
    while (stack != NULL) {
        postfix[index++] = pop(&stack);
    }

    postfix[index] = '\0';  // Null-terminate the postfix expression
    return postfix;
}

int main() {
    char infixExpr[MAX_SIZE];
    scanf("%s", infixExpr);

    char *postfixExpr = infixToPostfix(infixExpr);
    printf("%s\n", postfixExpr);
    free(postfixExpr);
    return 0;
}
