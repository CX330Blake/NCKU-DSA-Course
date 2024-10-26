#include <stdio.h>
#include <stdlib.h>

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

int main() {
    int num = getNum();
    int** adjMatrix = createAdjMatrix(num);
}
