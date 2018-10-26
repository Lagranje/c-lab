#ifndef C_CUSTOMERS_H
#define C_CUSTOMERS_H

#include <string.h>
#include <stdlib.h>
#include "../../io/io.h"
#include "../index_file/index_file.h"
	
typedef struct CUSTOMERS_MODEL {
	int customer_id;
	int deleted;
	int phone;
} customers_model;

customers_model create_customer(int phone);

int __append_customer_to_file(char *filename, customers_model customer);

customers_model *__read_customers_form_file(char *filename, int seek);

int __update_customers(char *filename, int seek, customers_model *new_customer);

int clear_customers_db();

#endif