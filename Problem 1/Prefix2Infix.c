#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct StackNode
{
    char *data;
    struct StackNode *next;
} StackNode;

void push(StackNode **top, char *data)
{
    StackNode *newNode = (StackNode *)malloc(sizeof(StackNode));
    newNode->data = data;
    newNode->next = *top;
    *top = newNode;
}

char *pop(StackNode **top)
{
    if (*top == NULL)
    {
        return NULL;
    }
    StackNode *temp = *top;
    char *data = temp->data;
    *top = temp->next;
    free(temp);
    return data;
}

int isOperator(char c)
{
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

char *prefixToInfix(char *prefix)
{
    StackNode *top = NULL;
    int len = strlen(prefix);
    for (int i = len - 1; i >= 0; i--)
    {
        char symbol = prefix[i];
        if (isOperator(symbol))
        {
            char *op1 = pop(&top);
            char *op2 = pop(&top);
            char *expression = (char *)malloc(strlen(op1) + strlen(op2) + 4);
            sprintf(expression, "(%s %c %s)", op2, symbol, op1);
            push(&top, expression);
        }
        else
        {
            // It's an operand
            char *operand = (char *)malloc(2);
            operand[0] = symbol;
            operand[1] = '\0';
            push(&top, operand);
        }
    }
    return pop(&top);
}
int main()
{
    char prefixExpr[] = "/+*abcd";
    printf("Prefix expression: %s\n", prefixExpr);
    char *infixExpr = prefixToInfix(prefixExpr);
    printf("Infix expression: %s\n", infixExpr);
    free(infixExpr);
    return 0;
}