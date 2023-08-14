#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Data structure for cars
typedef struct Car
{
    unsigned int *autonomy;
    unsigned int size;
    unsigned int capacity;
} Car;

// Data structure for stations
typedef struct Station
{
    unsigned int distance;
    Car *car_heap;
    struct Station *left;
    struct Station *right;
} Station;

Car *createCarHeap(const unsigned int *autonomies, unsigned int num_cars);
void insertHelper(Car *heap, unsigned int index);
void heapifyDown(Car *heap, unsigned int index);

// Creates a new node of the tree for the station
Station *createStationNode(Car *heap, unsigned int distance)
{
    Station *newStation = (Station *)malloc(sizeof(Station));
    newStation->distance = distance;
    newStation->car_heap = heap;
    newStation->left = NULL;
    newStation->right = NULL;
    return newStation;
}

Car *createCarHeap(const unsigned int *autonomies, unsigned int num_cars)
{

    Car *car_heap = (Car *)malloc(sizeof(Car));

    car_heap->size = 0;
    car_heap->capacity = num_cars;

    car_heap->autonomy = (unsigned int*)malloc(num_cars * sizeof(int));

    int i;
    for (i = 0; i < num_cars; i++)
    {
        car_heap->autonomy[i] = autonomies[i];
    }

    car_heap->size = i;
    i = (int)(car_heap->size - 2) / 2;
    while (i >= 0)
    {
        heapifyDown(car_heap, i);
        i--;
    }

    return car_heap;
}

// Helper for the insertion of a new car in the heap
void insertHelper(Car *heap, unsigned int index)
{
    unsigned int parent = (index - 1) / 2;

    if (heap->autonomy[parent] < heap->autonomy[index])
    {
        unsigned int temp = heap->autonomy[parent];
        heap->autonomy[parent] = heap->autonomy[index];
        heap->autonomy[index] = temp;

        insertHelper(heap, parent);
    }
}

// Heapify down
void heapifyDown(Car *heap, unsigned int index)
{
    unsigned int max = index;
    int left = (int)(2 * index + 1);
    int right = (int)(2 * index + 2);

    if (left >= heap->size || left < 0)
        left = -1;
    if (right >= heap->size || right < 0)
        right = -1;

    if (left != -1 && heap->autonomy[left] > heap->autonomy[max])
        max = left;
    if (right != -1 && heap->autonomy[right] > heap->autonomy[max])
        max = right;

    if (max != index)
    {
        unsigned int temp = heap->autonomy[max];
        heap->autonomy[max] = heap->autonomy[index];
        heap->autonomy[index] = temp;

        heapifyDown(heap, max);
    }
}

// Insert a car in the heap
void insertCar(Car *heap, unsigned int autonomy)
{
    heap->size++;

    unsigned int index = heap->size - 1;
    heap->autonomy[index] = autonomy;

    insertHelper(heap, index);

    printf("aggiunta\n");
}

// Remove a specific car from the heap
void removeCarByAutonomy(Car *heap, unsigned int autonomy)
{
    int index = -1;

    for (int i = 0; i < heap->size; i++)
    {
        if (heap->autonomy[i] == autonomy)
        {
            index = i;
            break;
        }
    }

    if (index == -1)
    {
        printf("non rottamata\n");
        return;
    }

    heap->autonomy[index] = heap->autonomy[heap->size - 1];

    heap->size--;

    heapifyDown(heap, index);

    printf("rottamata\n");
}

// Insert a station in the tree
Station *insertStation(Station *root, unsigned int distance, Car *car_heap)
{
    if (root == NULL)
    {
        root = createStationNode(car_heap, distance);
        root->car_heap = car_heap;
        printf("aggiunta\n");
    }
    else if (distance == root->distance)
    {
        printf("non aggiunta\n");
    }

    if (distance < root->distance)
    {
        root->left = insertStation(root->left, distance, car_heap);
    }
    else if (distance > root->distance)
    {
        root->right = insertStation(root->right, distance, car_heap);
    }

    return root;
}

Station *removeStation(Station *root, unsigned int distance)
{
    if (root == NULL)
    {
        printf("non demolita\n");
        return root;
    }

    if (distance < root->distance)
    {
        root->left = removeStation(root->left, distance);
    }
    else if (distance > root->distance)
    {
        root->right = removeStation(root->right, distance);
    }
    else
    {
        //station found
        if (root->left == NULL)
        {
            Station *temp = root->right;
            free(root);
            printf("demolita\n");
            return temp;
        }
        else if (root->right == NULL)
        {
            Station *temp = root->left;
            free(root);
            printf("demolita\n");
            return temp;
        }

        Station *successor = root->right;
        while (successor->left != NULL)
        {
            successor = successor->left;
        }

        root->distance = successor->distance;

        root->right = removeStation(root->right, successor->distance);

        printf("demolita\n");
    }

    return root;
}

// Find a specific station
Station *findStation(Station *root, unsigned int distance)
{
    if (root == NULL || root->distance == distance)
    {
        return root;
    }

    if (distance < root->distance)
    {
        return findStation(root->left, distance);
    }
    else
    {
        return findStation(root->right, distance);
    }
}

// Print a recap of the stations and cars
// Helper to visualize if everything works
void printStationRecap(Station *root)
{
    if (root != NULL)
    {
        printStationRecap(root->left);
        printf("Stazione a distanza %u:\n", root->distance);
        printf("Size: %u\n", root->car_heap->size);
        printf("Capacity: %u\n", root->car_heap->capacity);
        for (int i = 0; i < root->car_heap->capacity; i++)
        {
            printf("- Auto con autonomia %u\n", root->car_heap->autonomy[i]);
        }
        printf("\n");
        printStationRecap(root->right);
    }
}

int main()
{
    char str[100];
    Station *root = NULL;

    while (fscanf(stdin, "%s", str) != EOF)
    {
        if (strcmp(str, "aggiungi-stazione") == 0)
        {

            unsigned int distance, number_of_vehicles;
            fscanf(stdin, "%u %u", &distance, &number_of_vehicles);

            if (number_of_vehicles > 0)
            {
                unsigned int *car_autonomy = malloc(sizeof(int) * number_of_vehicles);

                for (int i = 0; i < number_of_vehicles; i++)
                {
                    fscanf(stdin, "%u", &car_autonomy[i]);
                }

                Car *car_heap = createCarHeap(car_autonomy, number_of_vehicles);

                root = insertStation(root, distance, car_heap);
            }
            else
            {
                root = insertStation(root, distance, NULL);
            }
        }
        else if (strcmp(str, "demolisci-stazione") == 0)
        {
            unsigned int distance;
            fscanf(stdin, "%u", &distance);
            root = removeStation(root, distance);
        }
        else if (strcmp(str, "aggiungi-auto") == 0)
        {
            unsigned int distance, autonomy;
            fscanf(stdin, "%u %u", &distance, &autonomy);

            Station *station = findStation(root, distance);
            if (station == NULL)
            {
                printf("non aggiunta\n");
                continue;
            }

            insertCar(station->car_heap, autonomy);
        }
        else if (strcmp(str, "rottama-auto") == 0)
        {
            unsigned int distance, autonomy;
            fscanf(stdin, "%u %u", &distance, &autonomy);

            Station *station = findStation(root, distance);
            if (station == NULL)
            {
                printf("non rottamata\n");
                continue;
            }

            removeCarByAutonomy(station->car_heap, autonomy);
        }
        else if (strcmp(str, "pianifica-percorso") == 0)
        {
            printf("nessun percorso\n");
        }
    }

    //printStationRecap(root);

    return 0;
}
