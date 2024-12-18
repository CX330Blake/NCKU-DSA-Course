#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct node {
    int data;
    struct node *child, *left, *right, *parent;
    int child_cut;
}node;

node *min = NULL;


void leave() {
    exit(0);
}

node *create_node(int value) {
    node *new_node = malloc(sizeof(node));
    new_node->data = value;
    new_node->child = new_node->left = new_node->right = new_node->parent = NULL;
    new_node->child_cut = 0;
    return new_node;
}

void insert(int value) {

    node *new_node = create_node(value);

    if (min == NULL) {
        min = new_node;
        new_node->left = new_node->right = new_node;
    } else {
        // Insert new node to the root list
        new_node->left = min;
        new_node->right = min->right;
        min->right->left = new_node;
        min->right = new_node;

        // Update min
        if (new_node->data < min->data) {
            min = new_node;
        }
    }
    printf("Inserted %d\n", value);
}

void consolidate() {
    int max_degree = 32; // 假設最大節點數為 2^32
    node *degree_table[max_degree];
    memset(degree_table, 0, sizeof(degree_table));

    node *current = min;
    node *start = min;

    do {
        node *x = current;
        int degree = x->degree;

        while (degree_table[degree] != NULL) {
            node *y = degree_table[degree];
            if (y->data < x->data) {
                node *temp = x;
                x = y;
                y = temp;
            }
            link(y, x);
            degree_table[degree] = NULL;
            degree++;
        }

        degree_table[degree] = x;
        current = current->right;
    } while (current != start);

    // 更新最小值指標
    min = NULL;
    for (int i = 0; i < max_degree; i++) {
        if (degree_table[i] != NULL) {
            if (min == NULL || degree_table[i]->data < min->data) {
                min = degree_table[i];
            }
        }
    }
}


void delete(node *n) {
    // Step 1: Remove the node from sibling list
    if (n->right == n) {
        return; // n is the only node in the list
    }
    n->left->right = n->right;
    n->right->left = n->left;

    n->left = n->right = n;

    // Step 2: Add the child to the root list
    if (n->child != NULL) {
        node *child = n->child;
        node *current = child;

        do {
            // Set parent to NULL for all children
            current->parent = NULL;
            current = current->right;
        } while (current != child);

        // Merge child list to the root list
        node *min_left = min->left;
        node *child_left = child->left;

        min_left->right = child;
        child->left = min_left;

        child_left->right = min;
        min->left = child_left;
    }

    n->child = NULL; // Clear the child pointer

    // Step 3: 更新最小節點（如果刪除的是 min 節點）
    if (n == min) {
        if (n->right == n) {
            // Heap is empty
            min = NULL;
        } else {
            // Update min
            min = n->right;
            consolidate();
        }
    }
}


void decrease_key() {}

void extract_min() {
    if (min != NULL) {

    }
}

void listen() {


    while (1) {
        char command[20];
        int value;

        scanf("%s", command);
        if (!strcmp(command, "exit")) {
            leave();
        } else if (!strcmp(command, "extract-min")) {
            extract_min();
        } else {
            scanf("%d", &value);
        }

        if (!strcmp(command, "insert")) {
            insert(value);
        } else if (!strcmp(command, "decrease")) {
            decrease_key(value);
        } else if (!strcmp(command, "delete")) {
            delete(value);
        }

    }
}

int main() {}
