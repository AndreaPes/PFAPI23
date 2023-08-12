#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definizione della struttura per le autovetture
typedef struct Car
{
    unsigned int autonomy;
} Car;

// Definizione della struttura per le stazioni
typedef struct Station
{
    unsigned int distance;
    Car *car_heap;
    int num_cars;
    struct Station *left;
    struct Station *right;
} Station;

// Funzione per creare un nuovo nodo per le stazioni
Station *createStationNode(int distance)
{
    Station *newStation = (Station *)malloc(sizeof(Station));
    newStation->distance = distance;
    newStation->car_heap = NULL;
    newStation->left = NULL;
    newStation->right = NULL;
    return newStation;
}

// Funzione per scambiare due elementi nel max heap
void swap(Car *a, Car *b){
    Car temp = *a;
    *a = *b;
    *b = temp;
}

Car *createCarHeap(unsigned int *autonomies, int num_cars){
    Car *car_heap = (Car *)malloc(sizeof(Car) * num_cars);
    
    for (int i = 0; i < num_cars; i++) {
        car_heap[i].autonomy = autonomies[i];
        
        int currentIndex = i;
        while (currentIndex > 0 && car_heap[currentIndex].autonomy > car_heap[(currentIndex - 1) / 2].autonomy) {
            swap(&car_heap[currentIndex], &car_heap[(currentIndex - 1) / 2]);
            currentIndex = (currentIndex - 1) / 2;
        }
    }
    
    return car_heap;
}

// Funzione per eseguire l'operazione di "heapify" (ristrutturazione) verso il basso
void heapifyDown(Car *heap, int size, int index) {
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < size && heap[left].autonomy > heap[largest].autonomy) {
        largest = left;
    }

    if (right < size && heap[right].autonomy > heap[largest].autonomy) {
        largest = right;
    }

    if (largest != index) {
        swap(&heap[index], &heap[largest]);
        heapifyDown(heap, size, largest);
    }
}

// Funzione per inserire una nuova autovettura nell'heap delle autonomie
void insertCar(Station *station, int autonomy) {
    Car newCar;
    newCar.autonomy = autonomy;

    int newSize = station->num_cars + 1;
    Car *newHeap = (Car *)malloc(sizeof(Car) * newSize);

    // Copia i dati dall'heap esistente nel nuovo heap
    for (int i = 0; i < station->num_cars; i++) {
        newHeap[i] = station->car_heap[i];
    }

    // Aggiungi la nuova autovettura all'ultimo elemento
    newHeap[newSize - 1] = newCar;

    // Esegui l'operazione di heapify verso l'alto
    int currentIndex = newSize - 1;
    while (currentIndex > 0 && newHeap[currentIndex].autonomy > newHeap[(currentIndex - 1) / 2].autonomy) {
        swap(&newHeap[currentIndex], &newHeap[(currentIndex - 1) / 2]);
        currentIndex = (currentIndex - 1) / 2;
    }

    // Libera la memoria dell'heap esistente e aggiorna i dati della stazione
    free(station->car_heap);
    station->car_heap = newHeap;
    station->num_cars = newSize;
}

// Funzione per rimuovere un'autovettura con un'autonomia specifica dall'heap
void removeCarByAutonomy(Station *station, int autonomy) {
    int index = -1;

    // Cerca l'indice dell'autovettura con l'autonomia specificata
    for (int i = 0; i < station->num_cars; i++) {
        if (station->car_heap[i].autonomy == autonomy) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Non rottamata\n");
        return;
    }

    // Sposta l'ultimo elemento nell'indice dell'elemento da rimuovere
    station->car_heap[index] = station->car_heap[station->num_cars - 1];

    // Riduci la dimensione dell'heap
    station->num_cars--;

    // Esegui l'operazione di heapify verso il basso dall'indice dell'elemento da rimuovere
    heapifyDown(station->car_heap, station->num_cars, index);

    printf("Rottamata\n");
}

// Funzione per inserire una nuova stazione nell'albero
Station *insertStation(Station *root, int distance, Car *car_heap, int cars_number)
{
    if (root == NULL)
    {
        root = createStationNode(distance);
        root->num_cars = cars_number;
        root->car_heap = car_heap;
        printf("aggiunta\n");
    }
    else if(distance == root -> distance)
    {
        printf("non aggiunta\n");
    }

    if (distance < root->distance)
    {
        root->left = insertStation(root->left, distance, car_heap, cars_number);
    }
    else if (distance > root->distance)
    {
        root->right = insertStation(root->right, distance, car_heap, cars_number);
    }

    return root;
}

Station *removeStation(Station *root, int distance)
{
    if (root == NULL)
    {
        printf("Non demolita\n");
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
            return temp;
        }
        else if (root->right == NULL)
        {
            Station *temp = root->left;
            free(root);
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

        printf("Demolita\n");
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
        for (int i = 0; i < root->num_cars; i++)
        {
            printf("- Auto con autonomia %u\n", root->car_heap[i].autonomy);
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

            unsigned int *car_autonomy = malloc(sizeof(int) * number_of_vehicles);

            for (int i = 0; i < number_of_vehicles; i++)
            {
                fscanf(stdin, "%u", &car_autonomy[i]);
            }

            Car *car_heap = createCarHeap(car_autonomy, number_of_vehicles);

            root = insertStation(root, distance, car_heap, number_of_vehicles);
        }
        else if (strcmp(str, "demolisci-stazione") == 0)
        {
            unsigned int distance;
            fscanf(stdin, "%u", &distance);
            root = removeStation(root, distance);
        }
        else if (strcmp(str, "aggiungi-auto") == 0)
        {
        }
        else if (strcmp(str, "rottama-auto") == 0)
        {
        }
        else if (strcmp(str, "pianifica-percorso") == 0)
        {
        }
    }

    printStationRecap(root);

    return 0;
}
