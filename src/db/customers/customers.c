#include "customers.h"

// autoIncrement for groups primary key
int customers_id_counter = 1;
size_t __customers_index_size = 8;

customers_model create_customer(int phone){
	customers_model customer;
	customer.customer_id = customers_id_counter++;
	customer.phone = phone;
	customer.deleted = 0;
	return customer;
}

customers_model get_customers_model(int id){
	int first_byte = -1;
	for (int i = 0; i < __customers_index_size * 1000; i += __customers_index_size) {
		index_file_model *index = read_index_from_file("customers.index", i);
		if (index->id == id) {
			first_byte = index->first_byte;
			break;
		}
	}
	if (first_byte == -1) {
		return create_customer(0, 0);
	}
	return *__read_customers_from_file("customers.db", first_byte);
}

int __append_customer_to_file(char *filename, customers_model customer) {
	FILE *file = fopen(filename, "a");
    int first_byte = (int) ftell(file);

	customers_model *object = malloc(sizeof(customers_model));
	object->customer_id = customer.customer_id;
	object->phone = customer.phone;
	object->deleted = customer.deleted;

	if (file != NULL) {
        fwrite(object, sizeof(group_users_model), 1, file);
        fclose(file);
        return first_byte;
    } else {
        return 1;
    }    
}

customers_model *__read_customers_form_file(char *filename, int seek){
	FILE *file = fopen(filename, "rb");

	customers_model *object = malloc(sizeof(customers_model));

	if (file != NULL) {
        fseek(file, seek, SEEK_SET);
        fread(object, sizeof(customers_model), 1, file);
        fclose(file);
    }
    return object;
}

int __update_customers(char *filename, int seek, customers_model *new_customer){
	FILE *file = fopen(filename, "r+");

	 if (file != NULL) {
        fseek(file, seek, SEEK_SET);
        fwrite(new_customer, sizeof(customers_model), 1, file);
        fclose(file);
        return 0;
    } else {
        return 1;
    }
}

int clear_customers_db() {
    write_file_content("customers.db", NULL, 0);
    write_file_content("customers.index", NULL, 0);
}