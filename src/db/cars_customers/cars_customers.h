#ifndef C_CARS_CUSTOMERS_H
#define C_CARS_CUSTOMERS_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "../../io/io.h"
#include "../index_file/index_file.h"

typedef struct car_customerS_MODEL {
    int cars_customers_id;
    int car_id;
    int customer_id;
} cars_customers_model;

cars_customers_model create_cars_customers(int car_id, int customer_id);

cars_customers_model get_cars_customers(int id);

int insert_cars_customers(cars_customers_model car);

int count_cars(void);

int remove_cars_customers(int id);

int update_cars_customers(int id, cars_customers_model car);

int count_cars_customers(void);

int remove_cars_customers_data(void);

#endif //C_CARS_CUSTOMERS_H