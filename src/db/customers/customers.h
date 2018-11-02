#ifndef C_CUSTOMER_H
#define C_CUSTOMER_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "../../io/io.h"
#include "../index_file/index_file.h"
#include "../cars_customers/cars_customers.h"
	
typedef struct CUSTOMER_MODEL {
	int customer_id;
	int phone;
    char *name[50];
    char *surname[50];
} customer_model;

customer_model create_customer(int phone, char *name, char *surname);

customer_model get_customer(int id);

int insert_customer(customer_model customer);

int count_customers(void);

int remove_customer(int id);

int update_customer(int id, customer_model customer);

int remove_customers_data(void);

#endif