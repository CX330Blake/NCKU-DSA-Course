#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100

typedef struct PriorityQueue {
    int heap[MAX_SIZE];
    int size;
} PriorityQueue;

void init(PriorityQueue *pq) { pq->size = 0; }

int parent(int i) { return (i - 1) / 2; }

int leftChild(int i) { return 2 * i + 1; }

int rightChild(int i) { return 2 * i + 2; }

void heapifyUp(PriorityQueue *pq, int i) {
    while (i != 0 && pq->heap[parent(i)] > pq->heap[i]) {
        int temp = pq->heap[i];
        pq->heap[i] = pq->heap[parent(i)];
        pq->heap[parent(i)] = temp;
        i = parent(i);
    }
}

void heapifyDown(PriorityQueue *pq, int i) {
    int smallest = i;
    int left = leftChild(i);
    int right = rightChild(i);
    if (left < pq->size && pq->heap[left] < pq->heap[smallest]) {
        smallest = left;
    }

    if (right < pq->size && pq->heap[right] < pq->heap[smallest]) {
        smallest = right;
    }

    if (smallest != i) {
        int temp = pq->heap[i];
        pq->heap[i] = pq->heap[smallest];
        pq->heap[smallest] = temp;

        heapifyDown(pq, smallest);
    }
}

void insert(PriorityQueue *pq, int value) {
    if (pq->size == MAX_SIZE) {
        printf("Priority queue is full.\n");
        return;
    }
    pq->heap[pq->size] = value;
    pq->size++;
    heapifyUp(pq, pq->size - 1);
}

int pop(PriorityQueue *pq) {
    if (pq->size <= 0) {
        printf("Priority queue is empty.\n");
        return -1;
    }
    int minValue = pq->heap[0];
    pq->heap[0] = pq->heap[pq->size - 1];
    pq->size--;
    heapifyDown(pq, 0);
    return minValue;
}

void deleteElement(PriorityQueue *pq, int value) {
    int i;
    // Find the index of the element to be deleted
    for (i = 0; i < pq->size; i++) {
        if (pq->heap[i] == value) {
            break;
        }
    }

    if (i == pq->size) {
        printf("Element not found in the priority queue.\n");
        return;
    }

    // Use the last element to replace the value to be deleted
    pq->heap[i] = pq->heap[pq->size - 1];
    pq->size--;

    if (i != 0 && pq->heap[i] < pq->heap[parent(i)]) {
        heapifyUp(pq, i);
    } else {
        heapifyDown(pq, i);
    }
}

void printPriorityQueue(PriorityQueue *pq) {
    for (int i = 0; i < pq->size; i++) {
        printf("%d ", pq->heap[i]);
    }
    printf("\n");
}

void listenCommand(PriorityQueue *pq) {
    printf("Enter command (insert/delete): \n");
    while (1) {
        char inputLine[100];
        char command[10];
        int num;
        char *token;

        if (fgets(inputLine, 100, stdin) == NULL) {
            break;
        }

        if (inputLine[0] == '\n') {
            break;
        }

        if (strlen(inputLine) > 0 && inputLine[strlen(inputLine) - 1] == '\n') {
            inputLine[strlen(inputLine) - 1] = '\0';
        }

        token = strtok(inputLine, " ");
        if (token == NULL) {
            printf("Invalid input.\n");
            continue;
        }
        strcpy(command, token);

        token = strtok(NULL, " ");
        if (token == NULL) {
            printf("Missing number argument.\n");
            continue;
        }

        // Check if the num is a valid integer
        char *endptr;
        num = strtol(token, &endptr, 10);
        if (*endptr != '\0') {
            printf("Invalid number.\n");
            continue;
        }

        // Operate on the priority queue based on the command
        if (strcmp(command, "insert") == 0) {
            insert(pq, num);
        } else if (strcmp(command, "delete") == 0) {
            deleteElement(pq, num);
        } else {
            printf("Invalid command.\n");
        }
    }
    return;
}

int main() {
    PriorityQueue pq;
    init(&pq);
    listenCommand(&pq);

    // printf("Priority Queue after operations: ");
    printPriorityQueue(&pq);

    return 0;
}