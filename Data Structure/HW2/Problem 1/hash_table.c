#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>


int32_t n, m; // n for buckets num, m for slots num

typedef struct bucket {
    int *slots;
    int *is_occupied; // 0 for empty, 1 for occupied
    int size;
} bucket;

bucket *hash_table = NULL;

int hash_func(int value) {
    return value % n;
}

void insert(int value) {
    int hash_value = hash_func(value);
    while (hash_table[hash_value].size == m) {
        hash_value = (hash_value + 1) % n;
    }

    bucket *b = &hash_table[hash_value];

    for (int i = 0; i < m; i++) {
        if (b->is_occupied[i] == 0) {
            b->slots[i] = value;
            b->is_occupied[i] = 1;
            b->size++;
            return;
        }
    }
}

void delete(int value) {
    int hash_value = hash_func(value);
    int start = hash_value;
    while (1) {
        bucket *b = &hash_table[hash_value];
        for (int i = 0; i < m; i++) {
            if (b->slots[i] == value && b->is_occupied[i] == 1) {
                b->is_occupied[i] = 0;
                b->size--;
                return;
            }
        }
        hash_value = (hash_value + 1) % n; // Linear probing
        if (hash_value == start) break;
    }
}

void search(int value) {
    int hash_value = hash_func(value);
    int start = hash_value; // Deep copy the value

    while (1) {
        bucket *b = &hash_table[hash_value];
        for (int i = 0; i < m; i++) {
            if (b->slots[i] == value && b->is_occupied[i] == 1) {
                printf("%d %d\n", hash_value, i);
                return;
            }
        }

        hash_value = (hash_value + 1) % n; // Linear probing
        if (hash_value == start) break;
    }
}

void leave() {
    for (int i = 0; i < n; i++) {
        free(hash_table[i].slots);
        free(hash_table[i].is_occupied);
    }
    free(hash_table);
    exit(0);
}

void listen() {
    while (1) {
        char command[20];
        int value;

        scanf("%s", command);
        if (!strcmp(command, "exit")) {
            leave();
            break;
        } else {
            scanf("%d", &value);
        }

        if (!strcmp(command, "bucket")) {
            n = value;
            // Init buckets
            hash_table = malloc(n * sizeof(bucket));
        } else if (!strcmp(command, "slot")) {
            m = value;
            // Init slots
            for (int i = 0;i < n;i++) {
                hash_table[i].slots = malloc(m * sizeof(int));
                hash_table[i].is_occupied = malloc(m * sizeof(int));
                hash_table[i].size = 0;

                for (int j = 0; j < m; j++) {
                    hash_table[i].is_occupied[j] = 0;
                }
            }
        } else if (!strcmp(command, "insert")) {
            insert(value);
        } else if (!strcmp(command, "delete")) {
            delete(value);
        } else if (!strcmp(command, "search")) {
            search(value);
        }
    }
}

int main() {
    listen();
}