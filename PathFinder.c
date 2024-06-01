#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CARS 512

// --------------------------------------------------------------------------- Structs ---------------------------------------------------------------------------

/**
 * @brief A structure representing a MaxHeap.
 */
typedef struct max_heap {
    int *elements;           ///< Array of elements in the heap.
    unsigned short size;     ///< Current size of the heap.
} MaxHeap;

/**
 * @brief A structure representing a node in the binary search tree.
 */
typedef struct tree_node {
    int key;                 ///< Key of the node.
    MaxHeap *cars;           ///< MaxHeap of cars associated with the node.
    struct tree_node *left;  ///< Pointer to the left child.
    struct tree_node *right; ///< Pointer to the right child.
} TreeNode;

/**
 * @brief A structure representing an edge.
 */
typedef struct edge {
    int cost;                ///< Cost associated with the edge.
    int from_index;          ///< Starting index of the edge.
    int to_index;            ///< Ending index of the edge.
} Edge;

/**
 * @brief A structure representing a node in the queue.
 */
typedef struct queue_node {
    Edge edge;               ///< Edge contained in the queue node.
    struct queue_node *next; ///< Pointer to the next node in the queue.
} QueueNode;

/**
 * @brief A structure representing a queue.
 */
typedef struct queue {
    QueueNode *head;         ///< Pointer to the head of the queue.
    QueueNode *tail;         ///< Pointer to the tail of the queue.
} Queue;

// --------------------------------------------------------------------------- Structs ---------------------------------------------------------------------------

// -------------------------------------------------------------------- Functions declaration --------------------------------------------------------------------

/**
 * @brief Creates a MaxHeap.
 * @param size The initial size of the heap.
 * @param elements Array of elements to initialize the heap.
 * @return A pointer to the created MaxHeap.
 */
MaxHeap *max_heap_create(int size, const int *elements);

/**
 * @brief Inserts an element into the MaxHeap.
 * @param max_heap Pointer to the MaxHeap.
 * @param element The element to insert.
 */
void max_heap_insert(MaxHeap *max_heap, int element);

/**
 * @brief Maintains the MaxHeap property by bubbling up an element.
 * @param max_heap Pointer to the MaxHeap.
 * @param index Index of the element to bubble up.
 */
void max_heapify_bottom_up(MaxHeap *max_heap, int index);

/**
 * @brief Maintains the MaxHeap property by trickling down an element.
 * @param max_heap Pointer to the MaxHeap.
 * @param index Index of the element to trickle down.
 */
void max_heapify_top_down(MaxHeap *max_heap, int index);

/**
 * @brief Gets the index of an element in the MaxHeap.
 * @param max_heap Pointer to the MaxHeap.
 * @param element The element to find.
 * @return The index of the element, or -1 if not found.
 */
int max_heap_get_index(MaxHeap *max_heap, int element);

/**
 * @brief Gets the parent index of a given index.
 * @param index The index to find the parent for.
 * @return The parent index.
 */
int max_heap_get_parent(int index);

/**
 * @brief Moves an element up in the MaxHeap.
 * @param max_heap Pointer to the MaxHeap.
 * @param index Index of the element to move up.
 */
void max_heap_move_up(MaxHeap *max_heap, int index);

/**
 * @brief Removes an element from the MaxHeap.
 * @param max_heap Pointer to the MaxHeap.
 * @param element The element to remove.
 */
void max_heap_remove(MaxHeap *max_heap, int element);

/**
 * @brief Creates a new tree node.
 * @param key The key of the node.
 * @param car_heap Pointer to the MaxHeap of cars.
 * @return A pointer to the created TreeNode.
 */
TreeNode *tree_create_node(int key, MaxHeap *car_heap);

/**
 * @brief Searches for a node in the tree.
 * @param root Pointer to the root of the tree.
 * @param key The key to search for.
 * @return A pointer to the found TreeNode, or NULL if not found.
 */
TreeNode *tree_search_node(TreeNode *root, int key);

/**
 * @brief Inserts a node into the tree.
 * @param root Pointer to the root of the tree.
 * @param key The key of the node to insert.
 * @param car_heap Pointer to the MaxHeap of cars.
 * @return A pointer to the root of the tree.
 */
TreeNode *tree_insert_node(TreeNode *root, int key, MaxHeap *car_heap);

/**
 * @brief Gets the node with the minimum key in the tree.
 * @param root Pointer to the root of the tree.
 * @return A pointer to the node with the minimum key.
 */
TreeNode *tree_get_min_node(TreeNode *root);

/**
 * @brief Removes a node from the tree.
 * @param root Pointer to the root of the tree.
 * @param key The key of the node to remove.
 * @return A pointer to the root of the tree.
 */
TreeNode *tree_remove_node(TreeNode *root, int key);

/**
 * @brief Creates a new queue.
 * @return A pointer to the created Queue.
 */
Queue *queue_create();

/**
 * @brief Checks if the queue is empty.
 * @param queue Pointer to the Queue.
 * @return 1 if the queue is empty, 0 otherwise.
 */
int queue_is_empty(Queue *queue);

/**
 * @brief Adds an edge to the queue.
 * @param queue Pointer to the Queue.
 * @param edge The Edge to add.
 */
void queue_enqueue(Queue *queue, Edge edge);

/**
 * @brief Removes and returns an edge from the queue.
 * @param queue Pointer to the Queue.
 * @return The Edge removed from the queue.
 */
Edge queue_dequeue(Queue *queue);

/**
 * @brief Destroys the queue.
 * @param queue Pointer to the Queue.
 */
void queue_destroy(Queue *queue);

/**
 * @brief Creates arrays representing the path between start and end.
 * @param root Pointer to the root of the tree.
 * @param start The starting key.
 * @param end The ending key.
 * @param stations Array to store station keys.
 * @param autonomies Array to store autonomies.
 * @param size Pointer to the size of the arrays.
 */
void path_array_create(TreeNode *root, int start, int end, int *stations, int *autonomies, int *size);

/**
 * @brief Calculates the path between start and end.
 * @param stations Array of station keys.
 * @param autonomies Array of autonomies.
 * @param size Size of the arrays.
 * @param start The starting key.
 * @param end The ending key.
 */
void path_calculate(int *stations, int *autonomies, int size, int start, int end);

/**
 * @brief Prints the path in reverse order.
 * @param stations Array of station keys.
 * @param predecessors Array of predecessors.
 * @param size Size of the arrays.
 */
void path_print_reverse(const int *stations, const unsigned short *predecessors, int size);

/**
 * @brief Prints the path in reverse order (alternate version).
 * @param stations Array of station keys.
 * @param predecessors Array of predecessors.
 * @param size Size of the arrays.
 */
void path_print_reverse2(const int *stations, const unsigned short *predecessors, int size);

// -------------------------------------------------------------------- Functions declaration --------------------------------------------------------------------

// ----------------------------------------------------------------------------- Main ----------------------------------------------------------------------------

int main() {
    char command[20];
    int heap_size, element, start, end, key;
    TreeNode *root = NULL;

    while (scanf("%s", command) == 1) {

        if (strcmp(command, "aggiungi-stazione") == 0) {
            (void) !scanf("%d", &key);
            (void) !scanf("%d", &heap_size);

            int *elements = (int *)malloc(heap_size * sizeof(int));
            for (int i = 0; i < heap_size; i++) {
                (void) !scanf("%d", &element);
                elements[i] = element;
            }
            MaxHeap *max_heap = max_heap_create(heap_size, elements);
            root = tree_insert_node(root, key, max_heap);

            free(elements);

        } else if (strcmp(command, "demolisci-stazione") == 0) {
            (void) !scanf("%d", &key);

            root = tree_remove_node(root, key);

        } else if (strcmp(command, "aggiungi-auto") == 0) {
            (void) !scanf("%d", &key);
            (void) !scanf("%d", &element);

            TreeNode *temp_node = tree_search_node(root, key);

            if (temp_node == NULL) {
                printf("non aggiunta\n");
            } else {
                max_heap_insert(temp_node->cars, element);
                printf("aggiunta\n");
            }

        } else if (strcmp(command, "rottama-auto") == 0) {
            (void) !scanf("%d", &key);
            (void) !scanf("%d", &element);

            TreeNode *temp_node = tree_search_node(root, key);

            if (temp_node == NULL) {
                printf("non rottamata\n");
            } else {
                max_heap_remove(temp_node->cars, element);
            }

        } else if (strcmp(command, "pianifica-percorso") == 0) {
            (void) !scanf("%d", &start);
            (void) !scanf("%d", &end);
            if (start == end) {
                printf("%d\n", start);
            } else {
                int max_size = abs(end - start);
                int *stations = (int *)malloc((max_size) * sizeof(int));
                int *autonomies = (int *)malloc((max_size) * sizeof(int));
                int count = 0;

                path_array_create(root, start, end, stations, autonomies, &count);
                path_calculate(stations, autonomies, count, start, end);
            }
        }
    }
    return 0;
}

// ----------------------------------------------------------------------------- Main ----------------------------------------------------------------------------

// ------------------------------------------------------------------- Functions implementation ------------------------------------------------------------------

MaxHeap *max_heap_create(int size, const int *elements) {
    MaxHeap *max_heap = (MaxHeap *)malloc(sizeof(MaxHeap));

    if (max_heap == NULL) {
        printf("memory allocation error!");
        return NULL;
    }
    max_heap->size = 0;
    max_heap->elements = (int *)malloc(MAX_CARS * sizeof(int));

    if (max_heap->elements == NULL) {
        printf("memory allocation error!");
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        max_heap_insert(max_heap, elements[i]);
    }
    return max_heap;
}

void max_heapify_bottom_up(MaxHeap *max_heap, int index) {
    int parent_index = (index - 1) / 2;

    if (max_heap->elements[parent_index] < max_heap->elements[index]) {
        int temp_element = max_heap->elements[parent_index];
        max_heap->elements[parent_index] = max_heap->elements[index];
        max_heap->elements[index] = temp_element;

        max_heapify_bottom_up(max_heap, parent_index);
    }
}

void max_heapify_top_down(MaxHeap *max_heap, int index) {
    int left = index * 2 + 1;
    int right = index * 2 + 2;
    int max = index;

    if (left >= max_heap->size || left < 0) {
        left = -1;
    }
    if (right >= max_heap->size || right < 0) {
        right = -1;
    }

    if (left != -1 && max_heap->elements[left] > max_heap->elements[max]) {
        max = left;
    }
    if (right != -1 && max_heap->elements[right] > max_heap->elements[max]) {
        max = right;
    }

    if (max != index) {
        int temp_element = max_heap->elements[max];
        max_heap->elements[max] = max_heap->elements[index];
        max_heap->elements[index] = temp_element;

        max_heapify_top_down(max_heap, max);
    }
}

int max_heap_get_index(MaxHeap *max_heap, int element) {
    for (int i = 0; i < max_heap->size; i++) {
        if (max_heap->elements[i] == element) {
            return i;
        }
    }
    return -1;
}

int max_heap_get_parent(int index) {
    if (index % 2 == 0) {
        return (index / 2) - 1;
    } else {
        return (index / 2);
    }
}

void max_heap_move_up(MaxHeap *max_heap, int index) {
    if (index == 0) {
        return;
    }

    int parent_index = max_heap_get_parent(index);

    if (max_heap->elements[parent_index] < max_heap->elements[index]) {
        int temp = max_heap->elements[parent_index];
        max_heap->elements[parent_index] = max_heap->elements[index];
        max_heap->elements[index] = temp;
        max_heap_move_up(max_heap, parent_index);
    }
}

void max_heap_insert(MaxHeap *max_heap, int element) {
    if (max_heap->size < MAX_CARS) {
        max_heap->elements[max_heap->size] = element;

        max_heapify_bottom_up(max_heap, max_heap->size);

        max_heap->size++;
    }
}

void max_heap_remove(MaxHeap *max_heap, int element) {
    int index = max_heap_get_index(max_heap, element);

    if (index > -1) {
        printf("rottamata\n");
        max_heap->elements[index] = max_heap->elements[max_heap->size - 1];
        max_heap->size--;
        int parent_index = max_heap_get_parent(index);
        if (index == 0 || max_heap->elements[index] < max_heap->elements[parent_index]) {
            max_heapify_top_down(max_heap, index);
        } else {
            max_heap_move_up(max_heap, index);
        }
    } else {
        printf("non rottamata\n");
    }
}

TreeNode *tree_create_node(int key, MaxHeap *car_heap) {
    TreeNode *temp_node = (TreeNode *)malloc(sizeof(TreeNode));
    temp_node->key = key;
    temp_node->left = temp_node->right = NULL;
    temp_node->cars = car_heap;
    return temp_node;
}

TreeNode *tree_get_min_node(TreeNode *root) {
    TreeNode *temp_node = root;

    while (temp_node->left != NULL) {
        temp_node = temp_node->left;
    }

    return temp_node;
}

TreeNode *tree_search_node(TreeNode *root, int key) {
    if (root == NULL || root->key == key) {
        return root;
    }

    if (root->key < key) {
        return tree_search_node(root->right, key);
    }

    return tree_search_node(root->left, key);
}

TreeNode *tree_insert_node(TreeNode *root, int key, MaxHeap *car_heap) {
    if (root == NULL) {
        printf("aggiunta\n");
        return tree_create_node(key, car_heap);
    }

    if (key < root->key) {
        root->left = tree_insert_node(root->left, key, car_heap);
    } else if (key > root->key) {
        root->right = tree_insert_node(root->right, key, car_heap);
    } else if (key == root->key) {
        printf("non aggiunta\n");
    }

    return root;
}

TreeNode *tree_remove_node(TreeNode *root, int key) {
    if (root == NULL) {
        printf("non demolita\n");
        return NULL;
    }

    if (root->key < key) {
        root->right = tree_remove_node(root->right, key);
    } else if (root->key > key) {
        root->left = tree_remove_node(root->left, key);
    } else {
        if (root->left == NULL && root->right == NULL) {
            free(root);
            printf("demolita\n");
            return NULL;
        } else if (root->left == NULL) {
            TreeNode *temp = root->right;

            free(root);
            printf("demolita\n");
            return temp;
        } else if (root->right == NULL) {
            TreeNode *temp = root->left;

            free(root);
            printf("demolita\n");
            return temp;
        } else {
            TreeNode *min_node = tree_get_min_node(root->right);
            root->key = min_node->key;
            root->cars = min_node->cars;
            root->right = tree_remove_node(root->right, min_node->key);
        }
    }
    return root;
}

Queue *queue_create() {
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    if (queue == NULL) {
        printf("memory allocation error!\n");
    }
    queue->head = queue->tail = NULL;
    return queue;
}

int queue_is_empty(Queue *queue) {
    return (queue->head == NULL);
}

void queue_enqueue(Queue *queue, Edge edge) {
    QueueNode *new_node_queue = (QueueNode *)malloc(sizeof(QueueNode));
    if (new_node_queue == NULL) {
        printf("memory allocation error!\n");
    }
    new_node_queue->edge = edge;
    new_node_queue->next = NULL;

    if (queue_is_empty(queue)) {
        queue->head = queue->tail = new_node_queue;
    } else {
        queue->tail->next = new_node_queue;
        queue->tail = new_node_queue;
    }
}

Edge queue_dequeue(Queue *queue) {
    if (queue_is_empty(queue)) {
        printf("error, queue is empty!\n");
    }

    QueueNode *temp_node = queue->head;
    Edge edge = temp_node->edge;
    queue->head = queue->head->next;

    if (queue->head == NULL) {
        queue->tail = NULL;
    }

    free(temp_node);
    return edge;
}

void queue_destroy(Queue *queue) {
    while (!queue_is_empty(queue)) {
        queue_dequeue(queue);
    }
    free(queue);
}

void path_array_create(TreeNode *root, int start, int end, int *stations, int *autonomies, int *size) {
    if (root == NULL) {
        return;
    }

    if (start < end) {
        if (start < root->key) {
            path_array_create(root->left, start, end, stations, autonomies, size);
        }
        if (start <= root->key && root->key <= end) {
            stations[*size] = root->key;
            if (root->cars->size > 0) {
                autonomies[*size] = root->cars->elements[0];
            } else {
                autonomies[*size] = 0;
            }

            (*size)++;
        }
        if (root->key < end) {
            path_array_create(root->right, start, end, stations, autonomies, size);
        }
    } else {
        if (start > root->key) {
            path_array_create(root->right, start, end, stations, autonomies, size);
        }
        if (end <= root->key && root->key <= start) {
            stations[*size] = root->key;
            if (root->cars->size > 0) {
                autonomies[*size] = root->cars->elements[0];
            } else {
                autonomies[*size] = 0;
            }
            (*size)++;
        }
        if (root->key > end) {
            path_array_create(root->left, start, end, stations, autonomies, size);
        }
    }
}

void path_calculate(int *stations, int *autonomies, int size, int start, int end) {
    bool *visited = (bool *)malloc((size) * sizeof(bool));
    unsigned short *predecessors = (unsigned short *)malloc((size) * sizeof(unsigned short));

    for (int i = 0; i < size; i++) {
        visited[i] = false;
        predecessors[i] = -1;
    }

    Queue *queue = queue_create();

    if (start > end) {
        Edge el_from_q = {0, size - 1, size - 1};
        visited[size - 1] = true;
        queue_enqueue(queue, el_from_q);
    } else {
        Edge el_from_q = {0, 0, 0};
        visited[0] = true;
        queue_enqueue(queue, el_from_q);
    }

    Edge el_from_q;
    while (!queue_is_empty(queue)) {
        el_from_q = queue_dequeue(queue);
        int from_index = el_from_q.to_index;
        predecessors[from_index] = el_from_q.from_index;

        if (start > end) {
            for (int j = from_index - 1; j >= 0; j--) {
                if (visited[j] == false && autonomies[j] >= abs(stations[j] - stations[from_index])) {
                    Edge e1 = {0, from_index, j};

                    queue_enqueue(queue, e1);
                    visited[j] = true;
                }
            }
        } else {
            for (int j = from_index + 1; j < size; j++) {
                if (visited[j] == false && autonomies[from_index] >= abs(stations[j] - stations[from_index])) {
                    Edge e1 = {0, from_index, j};

                    queue_enqueue(queue, e1);
                    visited[j] = true;
                }
            }
        }

        if (queue_is_empty(queue)) {
            break;
        }
    }

    if (start > end) {
        if (visited[0] == false) {
            printf("nessun percorso\n");
        } else {
            path_print_reverse2(stations, predecessors, size);
        }
    } else {
        if (visited[size - 1] == false) {
            printf("nessun percorso\n");
        } else {
            path_print_reverse(stations, predecessors, size);
        }
    }

    free(stations);
    free(autonomies);
    free(visited);
    free(predecessors);
    queue_destroy(queue);
}

void path_print_reverse(const int *stations, const unsigned short *predecessors, int size) {
    int cursor_end = size - 1;
    int *stations_path_min = (int *)malloc(size * sizeof(int));
    if (stations_path_min == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    int real_size = 0;
    while (cursor_end != 0) {
        stations_path_min[real_size] = stations[cursor_end];
        real_size++;
        cursor_end = predecessors[cursor_end];
    }
    stations_path_min[real_size] = stations[0];

    for (int i = real_size; i > 0; i--) {
        printf("%d ", stations_path_min[i]);
    }
    printf("%d\n", stations_path_min[0]);

    free(stations_path_min);
}

void path_print_reverse2(const int *stations, const unsigned short *predecessors, int size) {
    int cursor_end = 0;
    int *stations_path_min = (int *)malloc(size * sizeof(int));
    if (stations_path_min == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    int real_size = 0;
    while (cursor_end < size - 1 && cursor_end >= 0) {
        stations_path_min[real_size] = stations[cursor_end];
        real_size++;
        cursor_end = predecessors[cursor_end];
    }
    stations_path_min[real_size] = stations[size - 1];

    for (int i = 0; i < real_size; i++) {
        printf("%d ", stations_path_min[i]);
    }
    printf("%d\n", stations_path_min[real_size]);

    free(stations_path_min);
}

// ------------------------------------------------------------------- Functions implementation ------------------------------------------------------------------
