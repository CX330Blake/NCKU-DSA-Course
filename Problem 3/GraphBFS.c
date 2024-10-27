#include <stdio.h>
#include <stdlib.h>

typedef struct Queue {
    int front, rear, size;
    unsigned capacity;
    int* data;
} Queue;

Queue* createQueue(unsigned capacity) {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->capacity = capacity;
    queue->front = 0;
    queue->size = 0;
    queue->rear = capacity - 1;
    queue->data = (int*)malloc(queue->capacity * sizeof(int));
    return queue;
}

int isFull(Queue* queue) { return queue->size == queue->capacity; }

int isEmpty(Queue* queue) { return queue->size == 0; }

void enqueue(Queue* queue, int item) {
    if (isFull(queue)) return;
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->data[queue->rear] = item;
    queue->size++;
}

int dequeue(Queue* queue) {
    if (isEmpty(queue)) {
        return -1;
    }
    int item = queue->data[queue->front];
    // Make it a circular queue
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size--;
    return item;
}

int getNum() {
    int num;
    scanf("%d", &num);
    return num;
}

int** createAdjMatrix(int num) {
    int** adjMatrix = (int**)malloc(num * sizeof(int*));
    for (int i = 0; i < num; i++) {
        adjMatrix[i] = (int*)malloc(num * sizeof(int));
    }
    for (int i = 0; i < num; i++) {
        for (int j = 0; j < num; j++) {
            scanf("%d", &adjMatrix[i][j]);
        }
    }
    return adjMatrix;
}

int* BFS(int** adjMatrix) {}

int main() {
    int num = getNum();
    int** adjMatrix = createAdjMatrix(num);
    Queue* queue = createQueue(num);
}
