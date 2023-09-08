#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_CARS 512

// --------------------------------------------------------------------------- Structs ---------------------------------------------------------------------------

typedef struct MaxHeap {
    int *elements;
    unsigned short size;
} MaxHeap;

typedef struct TreeNode {
    int key;
    MaxHeap *cars;
    struct TreeNode *left, *right;
} TreeNode;

typedef struct Edge {
    int cost;
    int fromIndex;
    int toIndex;
} Edge;

typedef struct QueueNode {
    Edge edge;
    struct QueueNode *next;
} QueueNode;

typedef struct Queue {
    QueueNode *head;
    QueueNode *tail;
} Queue;

// --------------------------------------------------------------------------- Structs ---------------------------------------------------------------------------

// -------------------------------------------------------------------- Functions declaration --------------------------------------------------------------------

MaxHeap *maxHeapCreate(int size, const int *elements);

void maxHeapInsert(MaxHeap *maxHeap, int element);

void maxHeapifyBottomUp(MaxHeap *maxHeap, int index);

void maxHeapifyTopDown(MaxHeap *maxHeap, int index);

int maxHeapGetIndex(MaxHeap *maxHeap, int element);

int maxHeapGetParent(int index);

void maxHeapMoveUp(MaxHeap *maxHeap, int index);

void maxHeapRemove(MaxHeap *maxHeap, int element);


TreeNode *treeCreateNode(int key, MaxHeap *carHeap);

TreeNode *treeSearchNode(TreeNode *root, int key);

TreeNode *treeInsertNode(TreeNode *root, int key, MaxHeap *carHeap);

TreeNode *treeGetMinNode(TreeNode *root);

TreeNode *treeRemoveNode(TreeNode *root, int key);


Queue *queueCreate();

int queueIsEmpty(Queue *queue);

void queueEnqueue(Queue *queue, Edge edge);

Edge queueDequeue(Queue *queue);

void queueDestroy(Queue *queue);


void pathArrayCreate(TreeNode *root, int start, int end, int *stations, int *autonomies, int *size);

void pathCalculate(int *stations, int *autonomies, int size, int start, int end);

void pathPrintReverse(const int *stations, const unsigned short *predecessors, int size);

// -------------------------------------------------------------------- Functions declaration --------------------------------------------------------------------

// ----------------------------------------------------------------------------- Main ----------------------------------------------------------------------------

int main() {
    char command[20];
    int heapSize, element, start, end, key;
    TreeNode *root = NULL;

    while (scanf("%s", command) == 1) {

        if (strcmp(command, "aggiungi-stazione") == 0) {
            (void) !scanf("%d", &key);
            (void) !scanf("%d", &heapSize);

            int *elements = (int *) malloc(heapSize * sizeof(int));
            for (int i = 0; i < heapSize; i++) {
                (void) !scanf("%d", &element);
                elements[i] = element;
            }
            MaxHeap *maxHeap = maxHeapCreate(heapSize, elements);
            root = treeInsertNode(root, key, maxHeap);

            free(elements);

        } else if (strcmp(command, "demolisci-stazione") == 0) {
            (void) !scanf("%d", &key);

            root = treeRemoveNode(root, key);

        } else if (strcmp(command, "aggiungi-auto") == 0) {
            (void) !scanf("%d", &key);
            (void) !scanf("%d", &element);

            TreeNode *tempNode = treeSearchNode(root, key);

            if (tempNode == NULL) {
                printf("non aggiunta\n");
            } else {
                maxHeapInsert(tempNode->cars, element);
                printf("aggiunta\n");
            }

        } else if (strcmp(command, "rottama-auto") == 0) {
            (void) !scanf("%d", &key);
            (void) !scanf("%d", &element);

            TreeNode *tempNode = treeSearchNode(root, key);

            if (tempNode == NULL) {
                printf("non rottamata\n");
            } else {
                maxHeapRemove(tempNode->cars, element);
            }

        } else if (strcmp(command, "pianifica-percorso") == 0) {
            (void) !scanf("%d", &start);
            (void) !scanf("%d", &end);
            if (start == end) {
                printf("%d\n", start);
            } else {

                int max_size = abs(end - start);
                int *stations = (int *) malloc((max_size) * sizeof(int));
                int *autonomies = (int *) malloc((max_size) * sizeof(int));
                int count = 0;

                pathArrayCreate(root, start, end, stations, autonomies, &count);
                pathCalculate(stations, autonomies, count, start, end);
            }
        }
    }
    return 0;
}

// ----------------------------------------------------------------------------- Main ----------------------------------------------------------------------------

// ------------------------------------------------------------------- Functions implementation ------------------------------------------------------------------

MaxHeap *maxHeapCreate(int size, const int *elements) {
    MaxHeap *maxHeap = (MaxHeap *) malloc(sizeof(MaxHeap));

    if (maxHeap == NULL) {
        printf("memory allocation error!");
        return NULL;
    }
    maxHeap->size = 0;
    maxHeap->elements = (int *) malloc(MAX_CARS * sizeof(int));

    if (maxHeap->elements == NULL) {
        printf("memory allocation error!");
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        maxHeapInsert(maxHeap, elements[i]);
    }
    return maxHeap;
}


void maxHeapifyBottomUp(MaxHeap *maxHeap, int index) {

    int parentIndex = (index - 1) / 2;

    if (maxHeap->elements[parentIndex] < maxHeap->elements[index]) {

        int tempElement = maxHeap->elements[parentIndex];
        maxHeap->elements[parentIndex] = maxHeap->elements[index];
        maxHeap->elements[index] = tempElement;

        maxHeapifyBottomUp(maxHeap, parentIndex);
    }
}

void maxHeapifyTopDown(MaxHeap *maxHeap, int index) {

    int left = index * 2 + 1;
    int right = index * 2 + 2;
    int max = index;

    if (left >= maxHeap->size || left < 0)
        left = -1;
    if (right >= maxHeap->size || right < 0)
        right = -1;

    if (left != -1 && maxHeap->elements[left] > maxHeap->elements[max])
        max = left;
    if (right != -1 && maxHeap->elements[right] > maxHeap->elements[max])
        max = right;

    if (max != index) {
        int tempElement = maxHeap->elements[max];
        maxHeap->elements[max] = maxHeap->elements[index];
        maxHeap->elements[index] = tempElement;

        maxHeapifyTopDown(maxHeap, max);
    }
}

int maxHeapGetIndex(MaxHeap *maxHeap, int element) {
    for (int i = 0; i < maxHeap->size; i++) {
        if (maxHeap->elements[i] == element) {
            return i;
        }
    }
    return -1;
}

int maxHeapGetParent(int index) {
    if (index % 2 == 0)
        return (index / 2) - 1;
    else
        return (index / 2);
}


void maxHeapMoveUp(MaxHeap *maxHeap, int index) {
    if (index == 0)
        return;

    int parentIndex = maxHeapGetParent(index);

    if (maxHeap->elements[parentIndex] < maxHeap->elements[index]) {
        int temp = maxHeap->elements[parentIndex];
        maxHeap->elements[parentIndex] = maxHeap->elements[index];
        maxHeap->elements[index] = temp;
        maxHeapMoveUp(maxHeap, parentIndex);
    }
}

void maxHeapInsert(MaxHeap *maxHeap, int element) {
    if (maxHeap->size < MAX_CARS) {

        maxHeap->elements[maxHeap->size] = element;

        maxHeapifyBottomUp(maxHeap, maxHeap->size);

        maxHeap->size++;
    }
}

void maxHeapRemove(MaxHeap *maxHeap, int element) {

    int index = maxHeapGetIndex(maxHeap, element);

    if (index > -1) {
        printf("rottamata\n");
        maxHeap->elements[index] = maxHeap->elements[maxHeap->size - 1];
        maxHeap->size--;
        int parentIndex = maxHeapGetParent(index);
        if (index == 0 || maxHeap->elements[index] < maxHeap->elements[parentIndex])
            maxHeapifyTopDown(maxHeap, index);
        else
            maxHeapMoveUp(maxHeap, index);

    } else {
        printf("non rottamata\n");
    }
}

TreeNode *treeCreateNode(int key, MaxHeap *carHeap) {

    TreeNode *tempNode = (TreeNode *) malloc(sizeof(TreeNode));
    tempNode->key = key;
    tempNode->left = tempNode->right = NULL;
    tempNode->cars = carHeap;
    return tempNode;
}

TreeNode *treeGetMinNode(TreeNode *root) {

    TreeNode *tempNode = root;

    while (tempNode->left != NULL) { tempNode = tempNode->left; }

    return tempNode;
}

TreeNode *treeSearchNode(TreeNode *root, int key) {

    if (root == NULL || root->key == key)
        return root;

    if (root->key < key)
        return treeSearchNode(root->right, key);

    return treeSearchNode(root->left, key);
}

TreeNode *treeInsertNode(TreeNode *root, int key, MaxHeap *carHeap) {

    if (root == NULL) {
        printf("aggiunta\n");
        return treeCreateNode(key, carHeap);
    }

    if (key < root->key)
        root->left = treeInsertNode(root->left, key, carHeap);
    else if (key > root->key)
        root->right = treeInsertNode(root->right, key, carHeap);
    else if (key == root->key) {
        printf("non aggiunta\n");
    }

    return root;
}

// needs a fix for the memory allocation of the heap and its array
TreeNode *treeRemoveNode(TreeNode *root, int key) {

    if (root == NULL) {
        printf("non demolita\n");
        return NULL;
    }


    if (root->key < key)
        root->right = treeRemoveNode(root->right, key);

    else if (root->key > key)
        root->left = treeRemoveNode(root->left, key);

    else {

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
            TreeNode *minNode = treeGetMinNode(root->right);
            root->key = minNode->key;
            root->cars = minNode->cars;
            root->right = treeRemoveNode(root->right, minNode->key);
        }

    }
    return root;
}


Queue *queueCreate() {
    Queue *queue = (Queue *) malloc(sizeof(Queue));
    if (queue == NULL) {
        printf("memory allocation error!\n");
    }
    queue->head = queue->tail = NULL;
    return queue;
}

int queueIsEmpty(Queue *queue) {
    return (queue->head == NULL);
}

void queueEnqueue(Queue *queue, Edge edge) {
    QueueNode *newNodeQueue = (QueueNode *) malloc(sizeof(QueueNode));
    if (newNodeQueue == NULL) {
        printf("memory allocation error!\n");
    }
    newNodeQueue->edge = edge;
    newNodeQueue->next = NULL;

    if (queueIsEmpty(queue)) {
        queue->head = queue->tail = newNodeQueue;
    } else {
        queue->tail->next = newNodeQueue;
        queue->tail = newNodeQueue;
    }
}

Edge queueDequeue(Queue *queue) {
    if (queueIsEmpty(queue)) {
        printf("error, queue is empty!\n");
    }

    QueueNode *tempNode = queue->head;
    Edge edge = tempNode->edge;
    queue->head = queue->head->next;

    if (queue->head == NULL) {
        queue->tail = NULL;
    }

    free(tempNode);
    return edge;
}

void queueDestroy(Queue *queue) {
    while (!queueIsEmpty(queue)) {
        queueDequeue(queue);
    }
    free(queue);
}

// if max auto of a station is 0 -> do not add them to the list
void pathArrayCreate(TreeNode *root, int start, int end, int *stations, int *autonomies, int *size) {
    if (root == NULL) {
        return;
    }

    if (start < end) {

        if (start < root->key) {
            pathArrayCreate(root->left, start, end, stations, autonomies, size);
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
            pathArrayCreate(root->right, start, end, stations, autonomies, size);
        }
    } else {

        if (start > root->key) {
            pathArrayCreate(root->right, start, end, stations, autonomies, size);
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
            pathArrayCreate(root->left, start, end, stations, autonomies, size);
        }
    }
}

void pathPrintReverse2(const int *stations, const unsigned short* predecessors, int size);

void pathCalculate(int *stations, int *autonomies, int size, int start, int end) {

    // path and cost arrays
    bool *visited = (bool *) malloc((size) * sizeof(bool));
    unsigned short *predecessors = (unsigned short *) malloc((size) * sizeof(unsigned short));

    for (int i = 0; i < size; i++) {
        visited[i] = false;
        predecessors[i] = -1;
    }

    Queue *queue = queueCreate();

    if (start > end) {
        Edge elFromQ = {0, size - 1, size - 1};
        visited[size - 1] = true;
        queueEnqueue(queue, elFromQ);
    } else {
        Edge elFromQ = {0, 0, 0};
        visited[0] = true;
        queueEnqueue(queue, elFromQ);
    }

    Edge elFromQ;
    while (!queueIsEmpty(queue)) {
        elFromQ = queueDequeue(queue);
        int fromIndex = elFromQ.toIndex;
        predecessors[fromIndex] = elFromQ.fromIndex;

        if (start > end) {
            for (int j = fromIndex - 1; j >= 0; j--) {
                if (visited[j] == false &&
                    autonomies[j] >= abs(stations[j] - stations[fromIndex])) {
                    Edge e1 = {0, fromIndex, j};

                    queueEnqueue(queue, e1);
                    visited[j] = true;
                }
            }
        } else {
            for (int j = fromIndex + 1; j < size; j++) {
                if (visited[j] == false &&
                    autonomies[fromIndex] >= abs(stations[j] - stations[fromIndex])) {
                    Edge e1 = {0, fromIndex, j};

                    queueEnqueue(queue, e1);
                    visited[j] = true;
                }
            }
        }

        if (queueIsEmpty(queue)) {
            break;
        }
    }

    if (start > end) {
        if (visited[0] == false) {
            printf("nessun percorso\n");
        } else {
            //printf("percorso\n");
            pathPrintReverse2(stations, predecessors, size);
        }
    } else {
        if (visited[size - 1] == false) {
            printf("nessun percorso\n");
        } else {
            pathPrintReverse(stations, predecessors, size);
        }
    }

    free(stations);
    free(autonomies);
    free(visited);
    free(predecessors);
    queueDestroy(queue);
}

void pathPrintReverse(const int *stations, const unsigned short* predecessors, int size) {
    int cursorEnd = size - 1;
    int *stations_path_min = (int *)malloc(size * sizeof(int));
    if (stations_path_min == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    int real_size = 0;
    while (cursorEnd != 0) {
        stations_path_min[real_size] = stations[cursorEnd];
        real_size++;
        cursorEnd = predecessors[cursorEnd];
    }
    stations_path_min[real_size] = stations[0];

    for (int i = real_size; i > 0; i--) {
        printf("%d ", stations_path_min[i]);
    }
    printf("%d\n", stations_path_min[0]);

    free(stations_path_min);
}

void pathPrintReverse2(const int *stations, const unsigned short* predecessors, int size) {
    int cursorEnd = 0;
    int *stations_path_min = (int *)malloc(size * sizeof(int));
    if (stations_path_min == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    int real_size = 0;
    while (cursorEnd < size - 1 && cursorEnd >= 0) {
        stations_path_min[real_size] = stations[cursorEnd];
        real_size++;
        cursorEnd = predecessors[cursorEnd];
    }
    stations_path_min[real_size] = stations[size - 1];

    for (int i = 0; i < real_size; i++) {
        printf("%d ", stations_path_min[i]);
    }
    printf("%d\n", stations_path_min[real_size]);

    free(stations_path_min);
}

// ------------------------------------------------------------------- Functions implementation ------------------------------------------------------------------
