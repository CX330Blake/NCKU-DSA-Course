#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

typedef struct node {
    int key, degree, mark, visited;
    struct node *child, *left, *right, *parent;
}node;

node *min = NULL;

int node_count = 0;

void insert(int val) {
    node *new_node = malloc(sizeof(node));
    new_node->key = val;
    new_node->degree = 0;
    new_node->mark = 0;
    new_node->visited = 0;
    new_node->parent = NULL;
    new_node->child = NULL;
    new_node->left = new_node;
    new_node->right = new_node;
    if (min != NULL) {
        min->left->right = new_node;
        new_node->right = min;
        new_node->left = min->left;
        min->left = new_node;
        if (new_node->key < min->key)
            min = new_node;
    } else {
        min = new_node;
    }
    node_count++;
}

void cut(node *found, node *temp) {
    if (found == found->right)
        temp->child = NULL;
    (found->left)->right = found->right;
    (found->right)->left = found->left;
    if (found == temp->child)
        temp->child = found->right;
    temp->degree = temp->degree - 1;
    found->right = found;
    found->left = found;
    (min->left)->right = found;
    found->right = min;
    found->left = min->left;
    min->left = found;
    found->parent = NULL;
    found->mark = 0;
}

void cascading_cut(node *temp) {
    node *ptr = temp->parent;
    if (ptr != NULL) {
        if (temp->mark == 0) {
            temp->mark = 1;
        } else {
            cut(temp, ptr);
            cascading_cut(ptr);
        }
    }
}

void decrease_key(node *found, int val) {
    found->key = val;
    node *temp = found->parent;
    if (temp != NULL && found->key < temp->key) {
        cut(found, temp);
        cascading_cut(temp);
    }
    if (found->key < min->key)
        min = found;
}

void find(node *min, int old_val, int val) {
    node *found = NULL;
    node *temp = min;
    temp->visited = 1;
    node *found_ptr = NULL;
    if (temp->key == old_val) {
        found_ptr = temp;
        temp->visited = 0;
        found = found_ptr;
        decrease_key(found, val);
    }
    if (found_ptr == NULL) {
        if (temp->child != NULL)
            find(temp->child, old_val, val);
        if ((temp->right)->visited != 1)
            find(temp->right, old_val, val);
    }
    temp->visited = 0;
    found = found_ptr;
}

void fibonacci_link(node *ptr2, node *ptr1) {
    (ptr2->left)->right = ptr2->right;
    (ptr2->right)->left = ptr2->left;
    if (ptr1->right == ptr1)
        min = ptr1;
    ptr2->left = ptr2;
    ptr2->right = ptr2;
    ptr2->parent = ptr1;
    if (ptr1->child == NULL)
        ptr1->child = ptr2;
    ptr2->right = ptr1->child;
    ptr2->left = (ptr1->child)->left;
    ((ptr1->child)->left)->right = ptr2;
    (ptr1->child)->left = ptr2;
    if (ptr2->key < (ptr1->child)->key)
        ptr1->child = ptr2;
    ptr1->degree++;
}

void consolidate() {
    int temp1;
    float temp2 = (log(node_count)) / (log(2));
    int temp3 = temp2;

    node **arr = malloc((temp3 + 1) * sizeof(node *));
    for (int i = 0; i <= temp3; i++)
        arr[i] = NULL;

    node *ptr1 = min;
    node *ptr2;
    node *ptr3;
    node *ptr4 = ptr1;
    do {
        ptr4 = ptr4->right;
        temp1 = ptr1->degree;
        while (arr[temp1] != NULL) {
            ptr2 = arr[temp1];
            if (ptr1->key > ptr2->key) {
                ptr3 = ptr1;
                ptr1 = ptr2;
                ptr2 = ptr3;
            }
            if (ptr2 == min)
                min = ptr1;
            fibonacci_link(ptr2, ptr1);
            if (ptr1->right == ptr1)
                min = ptr1;
            arr[temp1] = NULL;
            temp1++;
        }
        arr[temp1] = ptr1;
        ptr1 = ptr1->right;
    } while (ptr1 != min);
    min = NULL;
    for (int j = 0; j <= temp3; j++) {
        if (arr[j] != NULL) {
            arr[j]->left = arr[j];
            arr[j]->right = arr[j];
            if (min != NULL) {
                (min->left)->right = arr[j];
                arr[j]->right = min;
                arr[j]->left = min->left;
                min->left = arr[j];
                if (arr[j]->key < min->key)
                    min = arr[j];
            } else {
                min = arr[j];
            }
            if (min == NULL)
                min = arr[j];
            else if (arr[j]->key < min->key)
                min = arr[j];
        }
    }
}

void extract_min() {
    node *temp = min;
    node *ptr = temp;
    node *x = NULL;
    if (temp->child != NULL) {
        x = temp->child;
        do {
            ptr = x->right;
            (min->left)->right = x;
            x->right = min;
            x->left = min->left;
            min->left = x;
            if (x->key < min->key)
                min = x;
            x->parent = NULL;
            x = ptr;
        } while (ptr != temp->child);
    }
    (temp->left)->right = temp->right;
    (temp->right)->left = temp->left;
    min = temp->right;
    if (temp == temp->right && temp->child == NULL)
        min = NULL;
    else {
        min = temp->right;
        consolidate();
    }
    node_count--;
}

void delete(int val) {
    find(min, val, 0);
    extract_min();
}

void level_order_traversal() {
    if (min == NULL) {
        printf("Heap is empty.\n");
        return;
    }

    // 由於最多有 node_count 個節點，所以簡單用陣列模擬 queue
    node **queue = malloc(node_count * sizeof(node *));
    int *levels = malloc(node_count * sizeof(int));
    int front = 0, back = 0;

    // 先將所有 "root list" 上的節點都 enqueue (level=0)
    // root list 以 min 為起點的環狀雙向鏈結
    node *start = min;
    node *cur = min;
    do {
        if (cur->visited == 0) {
            cur->visited = 1;        // 標記已放入佇列
            queue[back] = cur;
            levels[back] = 0;       // 層級=0
            back++;
        }
        cur = cur->right;
    } while (cur != start);

    // 進行 BFS
    while (front < back) {
        int currentLevel = levels[front];

        // 先找出「同一層」範圍 [front, levelEnd)
        int levelEnd = front;
        while (levelEnd < back && levels[levelEnd] == currentLevel) {
            levelEnd++;
        }
        // 同層的節點數量
        int blockSize = levelEnd - front;

        // 把這些節點複製到一個陣列，方便排序
        node **sameLevelNodes = malloc(blockSize * sizeof(node *));
        for (int i = 0; i < blockSize; i++) {
            sameLevelNodes[i] = queue[front + i];
        }

        // 對同層節點依照 degree 做由小到大排序
        // （可以使用任何排序法，這裡僅示範簡易的泡沫排序）
        for (int i = 0; i < blockSize - 1; i++) {
            for (int j = i + 1; j < blockSize; j++) {
                if (sameLevelNodes[i]->degree > sameLevelNodes[j]->degree) {
                    node *temp = sameLevelNodes[i];
                    sameLevelNodes[i] = sameLevelNodes[j];
                    sameLevelNodes[j] = temp;
                }
            }
        }

        // 輸出：當前層中，各節點的 (key, degree)
        printf("Level %d: ", currentLevel);
        for (int i = 0; i < blockSize; i++) {
            printf("%d(deg=%d) ", sameLevelNodes[i]->key, sameLevelNodes[i]->degree);
        }
        printf("\n");

        // 把同層的每個節點的子節點都 enqueue，層級 = currentLevel + 1
        for (int i = 0; i < blockSize; i++) {
            node *child = sameLevelNodes[i]->child;
            if (child != NULL) {
                node *cStart = child;
                node *cCur = child;
                // child 也是環狀串列
                do {
                    if (cCur->visited == 0) {
                        cCur->visited = 1;
                        queue[back] = cCur;
                        levels[back] = currentLevel + 1;
                        back++;
                    }
                    cCur = cCur->right;
                } while (cCur != cStart);
            }
        }

        free(sameLevelNodes);
        // front 移動到下一批
        front = levelEnd;
    }

    // 走訪結束，重置 visited 標記
    // （可以再用一次 BFS 或者一次性從 queue 中回頭清理）
    for (int i = 0; i < back; i++) {
        queue[i]->visited = 0;
    }

    free(queue);
    free(levels);
}

void listen() {
    while (1) {
        char command[20];
        int val1;
        int val2;

        scanf("%s", command);
        if (!strcmp(command, "exit")) {
            level_order_traversal();
            printf("Bye\n");
            exit(0);
            break;
        } else if (!strcmp(command, "extract-min")) {
            extract_min();
            continue;
        } else if (!strcmp(command, "insert")) {
            scanf("%d", &val1);
            insert(val1);
            continue;
        } else if (!strcmp(command, "decrease")) {
            scanf("%d %d", &val1, &val2);
            find(min, val1, val2);
            continue;
        } else if (!strcmp(command, "delete")) {
            scanf("%d", &val1);
            delete(val1);
            continue;
        }
    }
}

int main() {
    // insert(10);
    // insert(20);
    // insert(5);
    // insert(3);
    // insert(7);
    // insert(15);
    // insert(18);
    // insert(22);
    // insert(1);
    // insert(12);
    // extract_min();
    // find(min, 20, 1);
    // insert(25);
    // insert(30);
    // extract_min();
    // find(min, 30, 10);
    // extract_min();
    // find(min, 25, 4); // This is the line will cause the crash
    // insert(8);
    // insert(9);
    // extract_min();
    // insert(1);
    // extract_min();
    // extract_min();

    insert(10);
    insert(20);
    insert(5);
    insert(30);
    insert(25);
    extract_min();
    find(min, 30, 22);
    insert(15);
    insert(12);
    extract_min();
    delete(12);
    extract_min();
    level_order_traversal();
    return 0;
}