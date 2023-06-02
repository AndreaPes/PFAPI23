#include <stdio.h>
#include "stdlib.h"
#include "string.h"

typedef struct station {
    unsigned int distance;
    unsigned int *cars_autonomy;
    struct station *next_station;
    struct station *prev_station;
} Station;

typedef struct highway {
    Station *head;
    Station *tail;
} Highway;

void add_station() {

}

Station *find_station(Highway *highway, unsigned int distance) {
    Station *current_station;
    unsigned int middle_distance = (highway->head->distance + highway->tail->distance) / 2;

    if (distance <= middle_distance) {
        current_station = highway->head;
        while (current_station != NULL && current_station->distance <= distance) {
            if (current_station->distance == distance) {
                return current_station;
            } else if (current_station->distance > distance) {
                break;
            }
            current_station = current_station->next_station;
        }
    } else {
        current_station = highway->tail;
        while (current_station != NULL && current_station->distance >= distance) {
            if (current_station->distance == distance) {
                return current_station;
            } else if (current_station->distance < distance) {
                break;
            }
            current_station = current_station->prev_station;
        }
    }

    return NULL;
}

void remove_station() {

}

void add_car() {

}

int find_car() {

}

void remove_car() {

}

void calculate_path() {

}

int main() {
    char str[100];

    while (fscanf(stdin, "%s", str) != EOF) {
        if (strcmp(str, "aggiungi-stazione") == 0) {
            unsigned int distance, number_of_vehicles;
            fscanf(stdin, "%d %d", &distance, &number_of_vehicles);

            unsigned int* car_autonomy = malloc(sizeof (int) * number_of_vehicles);

            for(int i = 0; i < number_of_vehicles; i++){
                fscanf(stdin, "%d", &car_autonomy[i]);
            }
        } else if (strcmp(str, "demolisci-stazione") == 0) {

        } else if (strcmp(str, "aggiungi-auto") == 0) {

        } else if (strcmp(str, "rottama-auto") == 0) {

        } else if (strcmp(str, "pianifica-percorso") == 0) {

        }
    }

    return 0;
}