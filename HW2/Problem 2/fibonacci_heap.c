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
    found->key -= val;
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

void insert_child_sorted(node *parent, node *child) {
    // 如果 parent->child == NULL，則 child->left=child->right=child 自成環
    if (parent->child == NULL) {
        parent->child = child;
        child->left = child;
        child->right = child;
    } else {
        // 在 parent->child 的環裡，找出 child->key <= 誰，插在它前面
        // or 如果 child->key >= 所有節點，就插在 tail
        node *start = parent->child;
        node *cur = start;
        do {
            if (child->key < cur->key) {
                // 在 cur 前插入
                child->left = cur->left;
                child->right = cur;
                cur->left->right = child;
                cur->left = child;
                // 如果就是比 start->key 還小，更新 parent->child
                if (cur == start && child->key < start->key) {
                    parent->child = child;
                }
                return;
            }
            cur = cur->right;
        } while (cur != start);

        // 如果走了一圈都沒插 => child->key >= 全部 => 插在最後面
        child->left = start->left;
        child->right = start;
        start->left->right = child;
        start->left = child;
    }
}

int compare_degree_then_key(const void *a, const void *b) {
    node *n1 = *(node **)a;
    node *n2 = *(node **)b;
    // 先比較 degree
    if (n1->degree != n2->degree) {
        return (n1->degree - n2->degree); // degree 較小者在前
    }
    // degree 相同時，再比較 key
    return (n1->key - n2->key); // key 較小者在前
}

void fibonacci_link(node *y, node *x) {
    // 先把 y 從 root-list 拿掉
    y->left->right = y->right;
    y->right->left = y->left;
    // y->parent = x
    y->parent = x;
    y->mark = 0;
    // 把 y 插到 x->child (用 insert_child_sorted)
    insert_child_sorted(x, y);
    x->degree++;
}

void consolidate() {
    if (min == NULL) return;   // 空堆直接返回

    // 1) 收集所有根到一個陣列 roots[]
    int root_count = 0;
    node *cur = min;
    do {
        root_count++;
        cur = cur->right;
    } while (cur != min);

    node **roots = malloc(root_count * sizeof(node *));
    cur = min;
    for (int i = 0; i < root_count; i++) {
        roots[i] = cur;
        cur = cur->right;
    }

    // 2) 先清空原本的 root-list (等會兒再重建)
    min = NULL;

    // 3) 根據 (degree, key) 排序
    qsort(roots, root_count, sizeof(node *), compare_degree_then_key);

    // 4) 依序檢查相鄰兩顆, 如果 degree 相同 -> 合併
    int i = 0;
    while (i < root_count - 1) {
        node *x = roots[i];
        node *y = roots[i + 1];
        if (x && y && (x->degree == y->degree)) {
            // 誰 key 較小，就成為父
            if (y->key < x->key) {
                node *tmp = x; x = y; y = tmp;
            }
            // 把 y link 到 x 裏
            fibonacci_link(y, x);  // (下步會修正 fibonacci_link 內容)
            roots[i + 1] = NULL;     // y 不再是 root
            // x->degree++ 了, 有可能跟後面 roots[i+2] 又同 degree
            // 因此要重新整理, 最簡單做法: i = 0, 重新來過
            i = 0;
            // 也可把空位往前移, 再 sort 一次, 視你的邏輯而定
            // 這裡用最簡單的「回頭再掃」
        } else {
            i++;
        }
    }

    // 5) 重建 root-list, 更新 min
    for (int j = 0; j < root_count; j++) {
        if (roots[j] != NULL) {
            // 讓 roots[j] 變成一個獨立環 (left=right=自己)
            roots[j]->left = roots[j];
            roots[j]->right = roots[j];
            // 加回 root-list
            if (min == NULL) {
                min = roots[j];
            } else {
                // 簡單串接
                min->left->right = roots[j];
                roots[j]->right = min;
                roots[j]->left = min->left;
                min->left = roots[j];
                // 更新 min
                if (roots[j]->key < min->key) {
                    min = roots[j];
                }
            }
        }
    }
    free(roots);
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
    find(min, val, val);
    extract_min();
}

int compareRootByDegree(const void *a, const void *b) {
    node *r1 = *(node **)a;
    node *r2 = *(node **)b;
    return (r1->degree - r2->degree);
}

void level_order_traversal() {
    if (!min) {
        printf("Heap is empty.\n");
        return;
    }

    // 1) 收集 root-list
    int rootCount = 0;
    node *cur = min;
    do {
        rootCount++;
        cur = cur->right;
    } while (cur != min);

    node **roots = malloc(rootCount * sizeof(node *));
    cur = min;
    for (int i = 0; i < rootCount; i++) {
        roots[i] = cur;
        cur = cur->right;
    }

    // 2) 依 degree 由小到大排序
    qsort(roots, rootCount, sizeof(node *), compareRootByDegree);

    // 3) 每顆樹做 BFS，印在一行
    for (int i = 0; i < rootCount; i++) {
        // queue
        node **queue = malloc(node_count * sizeof(node *));
        int front = 0, back = 0;

        // 入隊
        queue[back++] = roots[i];
        roots[i]->visited = 1;

        // 一次 BFS
        while (front < back) {
            node *u = queue[front++];
            // 印出 key
            printf("%d ", u->key);

            // 所有 child 入隊
            if (u->child) {
                node *c = u->child;
                do {
                    if (!c->visited) {
                        c->visited = 1;
                        queue[back++] = c;
                    }
                    c = c->right;
                } while (c != u->child);
            }
        }

        // 同一棵樹印完後換行
        printf("\n");

        // 把 visited 清回 0
        for (int j = 0; j < back; j++) {
            queue[j]->visited = 0;
        }
        free(queue);
    }

    free(roots);
}



void listen() {
    while (1) {
        char command[20];
        int val1;
        int val2;

        scanf("%s", command);
        if (!strcmp(command, "exit")) {
            level_order_traversal();
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

void test() {
    insert(10);
    insert(20);
    insert(5);
    insert(3);
    insert(7);
    insert(15);
    insert(18);
    insert(22);
    insert(1);
    insert(12);
    extract_min();
    find(min, 20, 1);
    insert(25);
    insert(30);
    extract_min();
    find(min, 30, 10);
    extract_min();
    find(min, 25, 4); // This is the line will cause the crash
    insert(8);
    insert(9);
    extract_min();
    insert(1);
    extract_min();
    extract_min();
    level_order_traversal();
    return;
}

int main() {
    // test();
    listen();
    return 0;
}