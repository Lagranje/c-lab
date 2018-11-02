#ifndef C_CAR_H
#define C_CAR_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../../io/io.h"
#include "../index_file/index_file.h"

typedef struct CAR_MODEL {
    int car_id;
    char vin[50];
} car_model;

car_model create_car(char *vin);

car_model get_car(int id);

int insert_car(car_model car);

int count_cars(void);

int remove_car(int id);

int update_car(int id, car_model car);

int remove_cars_data(void);

#endif