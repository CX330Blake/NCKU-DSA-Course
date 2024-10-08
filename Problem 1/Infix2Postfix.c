#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct StackNode
{
    char *data;
    struct StackNode *next;
} StackNode;