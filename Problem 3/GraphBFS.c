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

void BFS(int** adjMatrix, int num, int startVertex) {
    Queue* queue = createQueue(num);
    int* visited = (int*)malloc(num * sizeof(int));
    int index = 0;
    for (int i = 0; i < num; i++) {
        visited[i] = 0;
    }
    enqueue(queue, startVertex);
    visited[startVertex] = 1;

    while (!isEmpty(queue)) {
        int vertex = dequeue(queue);
        printf("%d ", vertex + 1);  // 1-based indexing output

        for (int i = 0; i < num; i++) {
            if (adjMatrix[vertex][i] == 1 && !visited[i]) {
                enqueue(queue, i);
                visited[i] = 1;
            }
        }
    }

    free(queue->data);
    free(queue);
    free(visited);
}

int main() {
    int num = getNum();
    int** adjMatrix = createAdjMatrix(num);
    BFS(adjMatrix, num, 0);

    // 2-dim array deallocation
    for (int i = 0; i < num; i++) {
        free(adjMatrix[i]);
    }
    free(adjMatrix);
    return 0;
}
