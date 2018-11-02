#include "customers.h"

customer_model *__get_customers_db__(void) {
    FILE *file = fopen("customers.db", "rb");
    int i = 0;
    customer_model *customers = malloc(sizeof(customer_model) * 100);
    while (1) {
        fread(&customers[i], sizeof(customer_model), 1, file);
        i++;
        if (feof(file)) {
            break;
        }
    }
    fclose(file);
    return customers;
}

int __append_customer_to_db__(customer_model customer) {
    FILE *file = fopen("customers.db", "a");
    int first_byte = (int) ftell(file);

    customer_model *object = malloc(sizeof(customer_model));
    object->customer_id = customer.customer_id;
    object->phone = customer.phone;
    strcpy((char *) object->name, (char *) customer.name);
    strcpy((char *) object->surname, (char *) customer.surname);

    if (file != NULL) {
        fwrite(object, sizeof(customer_model), 1, file);
        fclose(file);
        return first_byte;
    } else {
        fclose(file);
        return 1;
    }
}

customer_model *__read_customer_from_db__(int seek) {
    FILE *file = fopen("customers.db", "rb");

    customer_model *object = malloc(sizeof(customer_model));

    if (file != NULL) {
        fseek(file, seek, SEEK_SET);
        fread(object, sizeof(customer_model), 1, file);
    }
    fclose(file);
    return object;
}

int __update_customer_db__(int seek, customer_model *newCustomer) {
    FILE *file = fopen("customers.db", "r+");

    if (file != NULL) {
        fseek(file, seek, SEEK_SET);
        fwrite(newCustomer, sizeof(customer_model), 1, file);
        fclose(file);
        return 0;
    } else {
        fclose(file);
        return 1;
    }
}

customer_model create_customer(int phone, char *name, char *surname) {
    customer_model *customer = malloc(sizeof(customer_model));
    customer->phone = phone;
    strcpy((char *) customer->name, name);
    strcpy((char *) customer->surname, surname);
    customer->customer_id = (int)time(NULL) - 1500000000;
    return *customer;
}

int insert_customer(customer_model customer) {
    int first_byte = __append_customer_to_db__(customer);
    index_file_model customer_index = create_index(customer.customer_id, first_byte);
    append_index_to_file("customers.index", customer_index);
    return 0;
}

customer_model get_customer(int id) {
    if (id == 0) {
        return create_customer(0, "", "");
    }

    index_file_model *indexes = get_file_indexes("customers.index");
    for (int i = 0; i < 100; i++) {
        if (indexes[i].id == id) {
            return *__read_customer_from_db__(indexes[i].first_byte);
        }
    }
    return create_customer(0, "", "");
}

int update_customer(int id, customer_model customer) {
    customer.customer_id = id;

    index_file_model *indexes = get_file_indexes("customers.index");
    for (int i = 0; i < 100; i++) {
        if (indexes[i].id == id) {
            __update_customer_db__(indexes[i].first_byte, &customer);
        }
    }
    return 0;
}

int count_customers(void) {
    return count_indexes("customers.index");
}

int remove_customers_data(void) {
    remove("customers.db");
    remove("customers.index");
    write_file_content("customers.db", "", 0);
    write_file_content("customers.index", "", 0);
    return 0;
}

int remove_customer(int id) {
    customer_model found = get_customer(id);
    if (found.customer_id == 0) {
        return 1;
    }
    customer_model *customers_db = __get_customers_db__();

    customer_model *filtered_customers_db = malloc(sizeof(customer_model) * 100);
    int filtered_counter = 0;
    for (int i = 0; i < 100; i++) {
        if (customers_db[i].customer_id != id) {
            filtered_customers_db[filtered_counter++] = customers_db[i];
        }
    }

    remove_customers_data();

    for (int i = 0; i < 100; i++) {
        if (filtered_customers_db[i].customer_id != 0) {
            insert_customer(filtered_customers_db[i]);
        }
    }

    index_file_model *indexes = get_file_indexes("cars_customers.index");
    for (int i = 0; i < 100; i++){
        cars_customers_model cars_customers = get_cars_customers(indexes[i].id);
        if (cars_customers.customer_id == id){
            remove_cars_customers(indexes[i].id);
        }
    }

    return 0;
}
