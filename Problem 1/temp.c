#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Queue {
    char *data;
    struct Queue *next;
    int count;
} Queue;

void enqueue(Queue **queue, char *data) {
    Queue *newNode = malloc(sizeof(Queue));
    newNode->data = strdup(data);
    newNode->next = NULL;

    if (*queue == NULL) {
        *queue = newNode;
    } else {
        Queue *current = *queue;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
}

char *dequeue(Queue **queue) {
    if (*queue == NULL) {
        return NULL;
    }
    Queue *temp = *queue;
    char *data = temp->data;
    *queue = (*queue)->next;
    free(temp);
    return data;
}

Queue *createQueue(char *data) {
    Queue *queue = NULL;
    for (int i = 0; i < strlen(data); i++) {
        char temp[2] = { data[i], '\0' };
        enqueue(&queue, temp);
    }
    return queue;
}

int isOperator(char *data) {
    return (strcmp(data, "*") == 0 || strcmp(data, "/") == 0 || strcmp(data, "+") == 0 || strcmp(data, "-") == 0);
}

char *postfix2Prefix(char *postfix) {
    Queue *queue = createQueue(postfix);

    // 用來存儲操作數和運算符
    Queue *operands = NULL;

    while (queue && queue->count > 0) {
        char *current = dequeue(&queue);

        if (isOperator(current)) {
            // 如果是運算符，彈出兩個操作數
            char *secondOperand = dequeue(&operands);
            char *firstOperand = dequeue(&operands);

            // 創建新的表達式並入隊
            int newSize = strlen(current) + strlen(firstOperand) + strlen(secondOperand) + 2;
            char *expression = malloc(newSize);
            snprintf(expression, newSize, "%s%s%s", current, firstOperand, secondOperand);

            enqueue(&operands, expression);

            // 釋放無用的操作數
            free(secondOperand);
            free(firstOperand);
            free(expression);
        } else {
            // 如果是操作數，直接入隊
            enqueue(&operands, current);
        }
    }

    // 最後操作數應該是前綴表示式
    char *prefix = dequeue(&operands);
    return prefix;
}

int main() {
    char postfix[] = "AB*CD/-";
    char *prefix = postfix2Prefix(postfix);
    printf("Prefix: %s\n", prefix);
    free(prefix);
    return 0;
}
