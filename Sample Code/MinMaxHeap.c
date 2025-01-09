#include <stdio.h>  
#include <stdlib.h>
#include <math.h>

#define INITIAL_CAPACITY 100

typedef struct {
    int *data, size, capacity;
} min_max_heap;

// 创建堆
min_max_heap *create_min_max_heap(int capacity) {
    min_max_heap *heap = malloc(sizeof(min_max_heap));
    heap->data = malloc(sizeof(int) * capacity);
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

// 释放堆
void destroy_min_max_heap(min_max_heap *heap) {
    free(heap->data);
    free(heap);
}

// 判断节点是否在最小层
int is_min_level(int index) {
    if (index < 0) return 1; // 边界情况
    int level = (int)floor(log2(index + 1));
    return (level % 2) == 0;
}

// 交换两个整数
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// 获取父节点
int parent(int index) {
    if (index == 0) return -1;
    return (index - 1) / 2;
}

// 获取祖父节点
int grandparent(int index) {
    int p = parent(index);
    if (p == -1) return -1;
    return parent(p);
}

// 获取左子节点
int left_child(int index) {
    return 2 * index + 1;
}

// 获取右子节点
int right_child(int index) {
    return 2 * index + 2;
}

// 判断一个节点是否为另一个节点的孙节点
int is_grandchild_of(int parent_index, int child_index) {
    int p = parent(child_index);
    if (p == -1) return 0;
    return parent(p) == parent_index;
}

// 插入元素
void insert(min_max_heap *heap, int value) {
    if (heap->size == heap->capacity) {
        heap->capacity *= 2;
        heap->data = (int *)realloc(heap->data, sizeof(int) * heap->capacity);
    }

    heap->data[heap->size] = value;
    int index = heap->size++;

    if (is_min_level(index)) {
        if (parent(index) != -1 && heap->data[index] > heap->data[parent(index)]) {
            swap(&heap->data[index], &heap->data[parent(index)]);
            // 上滤最大值：确保不与根节点交换
            int gp = grandparent(index);
            while (gp != -1 && gp != 0 && heap->data[index] > heap->data[gp]) {
                swap(&heap->data[index], &heap->data[gp]);
                index = gp;
                gp = grandparent(index);
            }
        } else {
            // 上滤最小值
            int gp = grandparent(index);
            while (gp != -1 && heap->data[index] < heap->data[gp]) {
                swap(&heap->data[index], &heap->data[gp]);
                index = gp;
                gp = grandparent(index);
            }
        }
    } else {
        if (parent(index) != -1 && heap->data[index] < heap->data[parent(index)]) {
            swap(&heap->data[index], &heap->data[parent(index)]);
            // 上滤最小值：确保不与根节点交换
            int gp = grandparent(index);
            while (gp != -1 && gp != 0 && heap->data[index] < heap->data[gp]) {
                swap(&heap->data[index], &heap->data[gp]);
                index = gp;
                gp = grandparent(index);
            }
        } else {
            // 上滤最大值
            int gp = grandparent(index);
            while (gp != -1 && heap->data[index] > heap->data[gp]) {
                swap(&heap->data[index], &heap->data[gp]);
                index = gp;
                gp = grandparent(index);
            }
        }
    }
}

// 查找最小值
int find_min(min_max_heap *heap) {
    if (heap->size == 0) {
        printf("Heap is empty!\n");
        exit(0);
    }
    return heap->data[0];
}

// 查找最大值
int find_max(min_max_heap *heap) {
    if (heap->size == 0) {
        printf("Heap is empty!\n");
        exit(0);
    }
    if (heap->size == 1) {
        return heap->data[0];
    } else if (heap->size == 2) {
        return heap->data[1];
    } else {
        return (heap->data[1] > heap->data[2]) ? heap->data[1] : heap->data[2];
    }
}

// heapify_down 函数
void heapify_down(min_max_heap *heap, int index) {
    if (is_min_level(index)) {
        // 最小层
        int smallest = index;
        int child = left_child(index);
        int right = right_child(index);

        // 找出子节点中的最小值
        if (child < heap->size && heap->data[child] < heap->data[smallest]) {
            smallest = child;
        }
        if (right < heap->size && heap->data[right] < heap->data[smallest]) {
            smallest = right;
        }

        // 检查孙节点
        int left_grand = left_child(child);
        int right_grand = right_child(child);
        if (left_grand < heap->size && heap->data[left_grand] < heap->data[smallest]) {
            smallest = left_grand;
        }
        if (right_grand < heap->size && heap->data[right_grand] < heap->data[smallest]) {
            smallest = right_grand;
        }

        if (smallest != index) {
            swap(&heap->data[index], &heap->data[smallest]);

            // 检查是否为孙节点
            if (is_grandchild_of(index, smallest)) {
                // 确保交换后的孙节点符合堆的性质
                if (heap->data[smallest] > heap->data[parent(smallest)]) {
                    swap(&heap->data[smallest], &heap->data[parent(smallest)]);
                }
            }
            heapify_down(heap, smallest);
        }
    } else {
        // 最大层
        int largest = index;
        int child = left_child(index);
        int right = right_child(index);

        // 找出子节点中的最大值
        if (child < heap->size && heap->data[child] > heap->data[largest]) {
            largest = child;
        }
        if (right < heap->size && heap->data[right] > heap->data[largest]) {
            largest = right;
        }

        // 检查孙节点
        int left_grand = left_child(child);
        int right_grand = right_child(child);
        if (left_grand < heap->size && heap->data[left_grand] > heap->data[largest]) {
            largest = left_grand;
        }
        if (right_grand < heap->size && heap->data[right_grand] > heap->data[largest]) {
            largest = right_grand;
        }

        if (largest != index) {
            swap(&heap->data[index], &heap->data[largest]);

            // 检查是否为孙节点
            if (is_grandchild_of(index, largest)) {
                // 确保交换后的孙节点符合堆的性质
                if (heap->data[largest] < heap->data[parent(largest)]) {
                    swap(&heap->data[largest], &heap->data[parent(largest)]);
                }
            }
            heapify_down(heap, largest);
        }
    }
}

// 删除最小值
int delete_min(min_max_heap *heap) {
    if (heap->size == 0) {
        printf("Heap is empty!\n");
        exit(0);
    }
    int min = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    heapify_down(heap, 0);
    return min;
}

// 删除最大值
int delete_max(min_max_heap *heap) {
    if (heap->size == 0) {
        printf("Heap is empty!\n");
        exit(0);
    }
    int max_index;
    if (heap->size == 1) {
        max_index = 0;
    } else if (heap->size == 2) {
        max_index = 1;
    } else {
        max_index = (heap->data[1] > heap->data[2]) ? 1 : 2;
    }

    int max = heap->data[max_index];
    heap->data[max_index] = heap->data[heap->size - 1];
    heap->size--;
    heapify_down(heap, max_index);
    return max;
}

// 打印堆的当前状态（用于调试）
void print_heap(min_max_heap *heap) {
    printf("Heap: ");
    for (int i = 0; i < heap->size; i++) {
        printf("%d ", heap->data[i]);
    }
    printf("\n");
}

// 主函数
int main() {
    min_max_heap *heap = create_min_max_heap(INITIAL_CAPACITY);

    // 插入一些元素
    insert(heap, 10);
    insert(heap, 20);
    insert(heap, 5);
    insert(heap, 30);
    insert(heap, 2);
    insert(heap, 8);
    insert(heap, 15);

    print_heap(heap); // 调试输出

    printf("Min: %d\n", find_min(heap)); // 应该是 2
    printf("Max: %d\n", find_max(heap)); // 应该是 30

    printf("Deleting min: %d\n", delete_min(heap)); // 删除 2
    printf("New Min: %d\n", find_min(heap)); // 应该是 5
    print_heap(heap); // 调试输出

    printf("Deleting max: %d\n", delete_max(heap)); // 删除 30
    printf("New Max: %d\n", find_max(heap)); // 应该是 20
    print_heap(heap); // 调试输出

    // 释放堆
    destroy_min_max_heap(heap);

    return 0;
}
