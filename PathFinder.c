#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definizione della struttura per le autovetture
typedef struct Car
{
    unsigned int *autonomy;
    int size;
    int capacity;
} Car;

// Definizione della struttura per le stazioni
typedef struct Station
{
    unsigned int distance;
    Car *car_heap;
    struct Station *left;
    struct Station *right;
} Station;

Car *createCarHeap(unsigned int *autonomies, int num_cars);
void insertHelper(Car *heap, int index);
void heapifyDown(Car *heap, int index);

// Funzione per creare un nuovo nodo per le stazioni
Station *createStationNode(int distance, Car *heap)
{
    Station *newStation = (Station *)malloc(sizeof(Station));
    newStation->distance = distance;
    newStation->car_heap = heap;
    newStation->left = NULL;
    newStation->right = NULL;
    return newStation;
}

Car *createCarHeap(unsigned int *autonomies, int num_cars)
{

    Car *car_heap = (Car *)malloc(sizeof(Car));

    car_heap->size = 0;
    car_heap->capacity = num_cars;

    car_heap->autonomy = (int *)malloc(num_cars * sizeof(int));

    int i;
    for (i = 0; i < num_cars; i++)
    {
        car_heap->autonomy[i] = autonomies[i];
    }

    car_heap->size = i;
    i = (car_heap->size - 2) / 2;
    while (i >= 0)
    {
        heapifyDown(car_heap, i);
        i--;
    }

    return car_heap;
}

void insertHelper(Car *heap, int index)
{
    int parent = (index - 1) / 2;

    if (heap->autonomy[parent] < heap->autonomy[index])
    {
        int temp = heap->autonomy[parent];
        heap->autonomy[parent] = heap->autonomy[index];
        heap->autonomy[index] = temp;

        insertHelper(heap, parent);
    }
}

// Funzione per eseguire l'operazione di "heapify" (ristrutturazione) verso il basso
void heapifyDown(Car *heap, int index)
{
    int max = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    // Checking whether our left or child element
    // is at right index of not to avoid index error
    if (left >= heap->size || left < 0)
        left = -1;
    if (right >= heap->size || right < 0)
        right = -1;

    // store left or right element in max if
    // any of these is smaller that its parent
    if (left != -1 && heap->autonomy[left] > heap->autonomy[max])
        max = left;
    if (right != -1 && heap->autonomy[right] > heap->autonomy[max])
        max = right;

    // Swapping the nodes
    if (max != index)
    {
        int temp = heap->autonomy[max];
        heap->autonomy[max] = heap->autonomy[index];
        heap->autonomy[index] = temp;

        // recursively calling for their child elements
        // to maintain max heap
        heapifyDown(heap, max);
    }
}

// Funzione per inserire una nuova autovettura nell'heap delle autonomie
void insertCar(Car *heap, int autonomy)
{
    heap->size++;

    int index = heap->size - 1;
    heap->autonomy[index] = autonomy;

    insertHelper(heap, index);

    printf("aggiunta\n");
}

// Funzione per rimuovere un'autovettura con un'autonomia specifica dall'heap
void removeCarByAutonomy(Car *heap, int autonomy)
{
    int index = -1;

    // Cerca l'indice dell'autovettura con l'autonomia specificata
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

    // Sposta l'ultimo elemento nell'indice dell'elemento da rimuovere
    heap->autonomy[index] = heap->autonomy[heap->size - 1];

    // Riduci la dimensione dell'heap
    heap->size--;

    // Esegui l'operazione di heapify verso il basso dall'indice dell'elemento da rimuovere
    heapifyDown(heap, index);

    printf("rottamata\n");
}

// Funzione per inserire una nuova stazione nell'albero
Station *insertStation(Station *root, int distance, Car *car_heap)
{
    if (root == NULL)
    {
        root = createStationNode(distance, car_heap);
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

Station *removeStation(Station *root, int distance)
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
        // Trovata la stazione da rimuovere

        // Se ha al massimo un figlio o nessun figlio
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

        // Se ha due figli, trova il successore in ordine (il più piccolo nel sottoalbero destro)
        Station *successor = root->right;
        while (successor->left != NULL)
        {
            successor = successor->left;
        }

        // Copia il valore del successore nella stazione corrente
        root->distance = successor->distance;

        // Rimuovi il successore
        root->right = removeStation(root->right, successor->distance);

        printf("demolita\n");
    }

    return root;
}

// Funzione per trovare una stazione dato un percorso
Station *findStation(Station *root, int distance)
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

            // Inside the "aggiungi-stazione" block
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
                root = insertStation(root, distance, NULL); // Passing NULL for car_heap
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
        }
    }

    printStationRecap(root);

    return 0;
}
