#include "cars_customers.h"

cars_customers_model *__get_cars_customers_db__(void) {
    FILE *file = fopen("cars_customers.db", "rb");
    int i = 0;
    cars_customers_model *cars_customers = malloc(sizeof(cars_customers_model) * 100);
    while (1) {
        fread(&cars_customers[i], sizeof(cars_customers_model), 1, file);
        i++;
        if (feof(file)) {
            break;
        }
    }
    fclose(file);
    return cars_customers;
}

int __append_cars_customers_to_db__(cars_customers_model cars_customers) {
    FILE *file = fopen("cars_customers.db", "a");
    int first_byte = (int) ftell(file);

    cars_customers_model *object = malloc(sizeof(cars_customers_model));
    object->car_id = cars_customers.car_id;
    object->customer_id = cars_customers.customer_id;
    object->cars_customers_id = cars_customers.cars_customers_id;


    if (file != NULL) {
        fwrite(object, sizeof(cars_customers_model), 1, file);
        fclose(file);
        return first_byte;
    } else {
        fclose(file);
        return 1;
    }
}

cars_customers_model *__read_cars_customers_from_db__(int seek) {
    FILE *file = fopen("cars_customers.db", "rb");

    cars_customers_model *object = malloc(sizeof(cars_customers_model));

    if (file != NULL) {
        fseek(file, seek, SEEK_SET);
        fread(object, sizeof(cars_customers_model), 1, file);
    }
    fclose(file);
    return object;
}

int __update_cars_customers_db__(int seek, cars_customers_model *newcarscustomers) {
    FILE *file = fopen("cars_customers.db", "r+");

    if (file != NULL) {
        fseek(file, seek, SEEK_SET);
        fwrite(newcarscustomers, sizeof(cars_customers_model), 1, file);
        fclose(file);
        return 0;
    } else {
        fclose(file);
        return 1;
    }
}

//Public API functions

cars_customers_model create_cars_customers(int car_id, int customer_id) {
    cars_customers_model *cars_customers = malloc(sizeof(cars_customers_model));
    cars_customers->car_id = car_id;
    cars_customers->customer_id = customer_id;
    cars_customers->cars_customers_id = (int)time(NULL) - 1500000000;

    return *cars_customers;
}

int insert_cars_customers(cars_customers_model cars_customers) {
    int first_byte = __append_cars_customers_to_db__(cars_customers);
    index_file_model cars_customers_index = create_index(cars_customers.cars_customers_id, first_byte);
    append_index_to_file("cars_customers.index", cars_customers_index);
    return 0;
}

cars_customers_model get_cars_customers(int id) {
    if (id == 0) {
        return create_cars_customers(0, 0);
    }

    index_file_model *indexes = get_file_indexes("cars_customers.index");
    for (int i = 0; i < 100; i++) {
        if (indexes[i].id == id) {
            return *__read_cars_customers_from_db__(indexes[i].first_byte);
        }
    }
    return create_cars_customers(0, 0);
}

int update_cars_customers(int id, cars_customers_model cars_customers) {
    cars_customers.cars_customers_id = id;

    index_file_model *indexes = get_file_indexes("cars_customers.index");
    for (int i = 0; i < 100; i++) {
        if (indexes[i].id == id) {
            __update_cars_customers_db__(indexes[i].first_byte, &cars_customers);
        }
    }
    return 0;
}

int count_cars_customers(void) {
    return count_indexes("cars_customers.index");
}

int remove_cars_customers_data(void) {
    remove("cars_customers.db");
    remove("cars_customers.index");
    write_file_content("cars_customers.db", "", 0);
    write_file_content("cars_customers.index", "", 0);
    return 0;
}

int remove_cars_customers(int id){
    cars_customers_model found = get_cars_customers(id);
    if (found.cars_customers_id == 0) {
        return 1;
    }
    cars_customers_model *db = __get_cars_customers_db__();

    cars_customers_model *filtered_db = malloc(sizeof(cars_customers_model) * 100);
    int filtered_counter = 0;
    for (int i = 0; i < 100; i++) {
        if (db[i].cars_customers_id != id) {
            filtered_db[filtered_counter++] = db[i];
        }
    }

    remove_cars_customers_data();

    for (int i = 0; i < 100; i++) {
        if (filtered_db[i].cars_customers_id != 0) {
            insert_cars_customers(filtered_db[i]);
        }
    }

    return 0;
}
