#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Structure Definitions
typedef struct Car {
    unsigned int *autonomy;
    unsigned int size;
    unsigned int capacity;
} Car;

typedef struct Station {
    unsigned int distance;
    Car *car_heap;
    struct Station *left;
    struct Station *right;
} Station;

typedef struct ListNode {
    unsigned int dest;
    struct ListNode* next;
} ListNode;

typedef struct List {
    ListNode* head;
} List;

typedef struct Graph {
    unsigned int size;
    List* array;
} Graph;

// Helper Functions
void swap(unsigned int *a, unsigned int *b) {
    unsigned int temp = *a;
    *a = *b;
    *b = temp;
}

// Car-related Functions
Car *createCarHeap(const unsigned int *autonomies, unsigned int num_cars);
void heapifyUp(Car *heap, unsigned int index);
void heapifyDown(Car *heap, unsigned int index);
void insertCar(Car *heap, unsigned int autonomy);
void removeCarByAutonomy(Car *heap, unsigned int autonomy);

// Station-related Functions
Station *createStationNode(Car *heap, unsigned int distance);
Station *insertStation(Station *root, unsigned int distance, Car *car_heap);
Station *removeStation(Station *root, unsigned int distance);
Station *findStation(Station *root, unsigned int distance);

// Graph-related Functions
ListNode* listNode(unsigned int dest);
Graph* initializeGraph(unsigned int size);
void addEdge(Graph* graph, unsigned int start, unsigned int end);
void createGraph(Graph* graph, const unsigned int *array, const unsigned int *autonomy);
void shortestPathBFS(Graph* graph, const unsigned int *distances_array);

/***CAR***/

Car *createCarHeap(const unsigned int *autonomies, unsigned int num_cars) {

    Car *car_heap = (Car *) malloc(sizeof(Car));

    car_heap->size = num_cars;
    car_heap->capacity = num_cars > 0 ? num_cars : 1;

    car_heap->autonomy = (unsigned int *) malloc(car_heap->capacity * sizeof(int));

    for (unsigned int i = 0; i < num_cars; i++) {
        car_heap->autonomy[i] = autonomies[i];
    }

    int i = (int) (car_heap->size - 2) / 2;
    while (i >= 0) {
        heapifyDown(car_heap, i);
        i--;
    }

    return car_heap;
}

// Helper for the insertion of a new car in the heap
void heapifyUp(Car *heap, unsigned int index) {
    int parent = (int) (index - 1) / 2;

    if (index > 0 && heap->autonomy[index] > heap->autonomy[parent]) {
        swap(&heap->autonomy[index], &heap->autonomy[parent]);
        heapifyUp(heap, parent);
    }
}

// Heapify down
void heapifyDown(Car *heap, unsigned int index) {
    unsigned int max = index;
    int left = (int) (2 * index + 1);
    int right = (int) (2 * index + 2);

    if (left < heap->size && heap->autonomy[left] > heap->autonomy[max]) {
        max = left;
    }
    if (right < heap->size && heap->autonomy[right] > heap->autonomy[max]) {
        max = right;
    }

    if (max != index) {
        swap(&heap->autonomy[index], &heap->autonomy[max]);

        heapifyDown(heap, max);
    }
}

// Insert a car in the heap
void insertCar(Car *heap, unsigned int autonomy) {
    if (heap->size == heap->capacity) {
        heap->capacity *= 2;
        unsigned int *temp = (unsigned int *) realloc(heap->autonomy, heap->capacity * sizeof(unsigned int));

        if (temp == NULL) {
            return;
        }

        heap->autonomy = temp;
    }

    heap->autonomy[heap->size] = autonomy;
    heapifyUp(heap, heap->size);
    heap->size++;
    printf("aggiunta\n");
}

// Remove a specific car from the heap
void removeCarByAutonomy(Car *heap, unsigned int autonomy) {
    int index = -1;

    for (int i = 0; i < heap->size; i++) {
        if (heap->autonomy[i] == autonomy) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("non rottamata\n");
        return;
    }

    heap->autonomy[index] = heap->autonomy[heap->size - 1];

    heap->size--;

    heapifyDown(heap, index);

    printf("rottamata\n");
}

/***STATION***/

// Creates a new node of the tree for the station
Station *createStationNode(Car *heap, unsigned int distance) {
    Station *newStation = (Station *) malloc(sizeof(Station));
    newStation->distance = distance;
    newStation->car_heap = heap;
    newStation->left = NULL;
    newStation->right = NULL;
    return newStation;
}

// Insert a station in the tree
Station *insertStation(Station *root, unsigned int distance, Car *car_heap) {
    if (root == NULL) {
        root = createStationNode(car_heap, distance);
        root->car_heap = car_heap;
        printf("aggiunta\n");
    } else if (distance == root->distance) {
        printf("non aggiunta\n");
    }

    if (distance < root->distance) {
        root->left = insertStation(root->left, distance, car_heap);
    } else if (distance > root->distance) {
        root->right = insertStation(root->right, distance, car_heap);
    }

    return root;
}

Station *removeStation(Station *root, unsigned int distance) {
    if (root == NULL) {
        printf("non demolita\n");
        return root;
    }

    if (distance < root->distance) {
        root->left = removeStation(root->left, distance);
    } else if (distance > root->distance) {
        root->right = removeStation(root->right, distance);
    } else {
        //station found
        if (root->left == NULL) {
            Station *temp = root->right;
            free(root);
            printf("demolita\n");
            return temp;
        } else if (root->right == NULL) {
            Station *temp = root->left;
            free(root);
            printf("demolita\n");
            return temp;
        }

        Station *successor = root->right;
        while (successor->left != NULL) {
            successor = successor->left;
        }

        root->distance = successor->distance;

        root->right = removeStation(root->right, successor->distance);

        printf("demolita\n");
    }

    return root;
}

// Find a specific station
Station *findStation(Station *root, unsigned int distance) {
    if (root == NULL || root->distance == distance) {
        return root;
    }

    if (distance < root->distance) {
        return findStation(root->left, distance);
    } else {
        return findStation(root->right, distance);
    }
}

/***GRAPH***/

ListNode* listNode(unsigned int dest) {
    ListNode* newNode = (ListNode*) malloc(sizeof(ListNode));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

Graph* initializeGraph(unsigned int size) {
    Graph* graph = (Graph*) malloc(sizeof(Graph));
    graph->size = size;
    graph->array = (List*) malloc(size * sizeof(List));
    for (int i = 0; i < size; i++)
        graph->array[i].head = NULL;
    return graph;
}

void addEdge(Graph* graph, unsigned  int start, unsigned int end) {
    ListNode* newNode = listNode(end);
    newNode->next = graph->array[start].head;
    graph->array[start].head = newNode;
}

void createGraphUp(Graph* graph, const unsigned int *array, const unsigned int *autonomy) {
    for(int i = 0; i < graph->size - 1; i++){
        for(int k = i; k < graph->size - 1; k++){
            if(autonomy[i] >= (array[k+1] - array[i])){
                addEdge(graph, i, k+1);
            } else {
                break;
            }
        }
    }
}

void createGraphDown(Graph* graph, const unsigned int *array, const unsigned int *autonomy) {
    for(int i = 0; i < graph->size - 1; i++){
        for(int k = i; k < graph->size - 1; k++){
            if(autonomy[i] >= (array[k+1] - array[i])){
                addEdge(graph, i, k+1);
            } else {
                break;
            }
        }
    }
}

void initializeArray(Station *root, unsigned int start, unsigned int end, unsigned int *array, unsigned int *autonomy, int *count) {
    if (root == NULL) {
        return;
    }

    if (start < end) {

        if (start < root->distance) {
            initializeArray(root->left, start, end, array, autonomy, count);
        }
        if (start <= root->distance && root->distance <= end) {
            array[*count] = root->distance;
            autonomy[*count] = root->car_heap->autonomy[0];
            (*count)++;
        }
        if (root->distance < end) {
            initializeArray(root->right, start, end, array, autonomy, count);
        }
    } else {

        if (start > root->distance) {
            initializeArray(root->right, start, end, array, autonomy, count);
        }
        if (end <= root->distance && root->distance <= start) {
            array[*count] = root->distance;
            autonomy[*count] = root->car_heap->autonomy[0];
            (*count)++;

        }
        if (root->distance > end) {
            initializeArray(root->left, start, end, array, autonomy, count);
        }
    }
}

void shortestPathBFS(Graph* graph, const unsigned int *distances_array) {
    unsigned int size = graph->size;

    bool visited[size];
    unsigned int distance[size];
    int predecessor[size];

    for(unsigned int i = 0; i < size; i++) {
        visited[i] = false;
        distance[i] = __UINT8_MAX__;
        predecessor[i] = -1;
    }

    unsigned int* queue = (unsigned int*)malloc(size * sizeof(unsigned int));
    unsigned int head = 0, tail = 0;

    visited[0] = true;
    distance[0] = 0;
    queue[tail++] = 0;

    while (head < tail) {
    unsigned int current = queue[--tail];
    ListNode* temp = graph->array[current].head;

    while (temp) {
        if (!visited[temp->dest]) {
            visited[temp->dest] = true;
            distance[temp->dest] = distance[current] + 1;
            queue[tail++] = temp->dest;
            predecessor[temp->dest] = (int)current;

            if (temp->dest == size-1) {

                    unsigned int stack[size];
                    int top = -1;

                    int pathNode = (int)size-1;
                    while (pathNode != -1) {
                        stack[++top] = distances_array[pathNode];
                        pathNode = predecessor[pathNode];
                    }

                    while (top != -1) {
                        printf("%d ", stack[top--]);
                    }
                    printf("\n");
                    free(queue);
                    return;
                }
            }
            temp = temp->next;
        }
    }
    printf("nessun percorso\n");
    free(queue);
}

void printGraph(Graph* graph, unsigned int *distance_array) {
    for (unsigned int i = 0; i < graph->size; i++) {
        ListNode* node = graph->array[i].head;
        printf("\n Lista di adiacenza del nodo %u\n head", distance_array[i]);
        while (node) {
            printf(" -> %u", distance_array[node->dest]);
            node = node->next;
        }
        printf("\n");
    }
}


int main() {
    char str[100];
    Station *root = NULL;
    int result = 0;

    while (fscanf(stdin, "%s", str) != EOF) {
        if (result == EOF) {
            printf("Error");
        }
        if (strcmp(str, "aggiungi-stazione") == 0) {

            unsigned int distance, number_of_vehicles;
            result = fscanf(stdin, "%u %u", &distance, &number_of_vehicles);

            unsigned int *car_autonomy = malloc(sizeof(int) * number_of_vehicles);

            for (int i = 0; i < number_of_vehicles; i++) {
                result = fscanf(stdin, "%u", &car_autonomy[i]);
            }

            Car *car_heap = createCarHeap(car_autonomy, number_of_vehicles);

            root = insertStation(root, distance, car_heap);
        } else if (strcmp(str, "demolisci-stazione") == 0) {
            unsigned int distance;
            result = fscanf(stdin, "%u", &distance);
            root = removeStation(root, distance);
        } else if (strcmp(str, "aggiungi-auto") == 0) {
            unsigned int distance, autonomy;
            result = fscanf(stdin, "%u %u", &distance, &autonomy);

            Station *station = findStation(root, distance);
            if (station == NULL) {
                printf("non aggiunta\n");
                continue;
            }

            insertCar(station->car_heap, autonomy);
        } else if (strcmp(str, "rottama-auto") == 0) {
            unsigned int distance, autonomy;
            result = fscanf(stdin, "%u %u", &distance, &autonomy);

            Station *station = findStation(root, distance);
            if (station == NULL) {
                printf("non rottamata\n");
                continue;
            }

            removeCarByAutonomy(station->car_heap, autonomy);
        } else if (strcmp(str, "pianifica-percorso") == 0) {
            unsigned int start, end;
            result = fscanf(stdin, "%u %u", &start, &end);
            if(start < end){
                unsigned int distances_array[end-start];
                unsigned int autonomy[end-start];
                int count = 0;
                initializeArray(root, start, end, distances_array, autonomy, &count);
                Graph * graph = initializeGraph(count);
                createGraphUp(graph, distances_array, autonomy);
                //printGraph(graph, distances_array);
                shortestPathBFS(graph, distances_array);
            } else if (start > end) {
                unsigned int distances_array[start-end];
                unsigned int autonomy[start-end];
                int count = 0;
                initializeArray(root, start, end, distances_array, autonomy, &count);
            } else {
                printf("%u\n", start);
            }
        }
    }
    return 0;
}