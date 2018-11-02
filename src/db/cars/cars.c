#include "cars.h"
#include "../cars_customers/cars_customers.h"

car_model *__get_cars_db__(void) {
    FILE *file = fopen("cars.db", "rb");
    int i = 0;
    car_model *cars = malloc(sizeof(car_model) * 100);
    while (1) {
        fread(&cars[i], sizeof(car_model), 1, file);
        i++;
        if (feof(file)) {
            break;
        }
    }
    fclose(file);
    return cars;
}

int __append_car_to_db__(car_model car) {
    FILE *file = fopen("cars.db", "a");
    int first_byte = (int) ftell(file);

    car_model *object = malloc(sizeof(car_model));
    object->car_id = car.car_id;
    strcpy((char *) object->vin, (char *) car.vin);


    if (file != NULL) {
        fwrite(object, sizeof(car_model), 1, file);
        fclose(file);
        return first_byte;
    } else {
        fclose(file);
        return 1;
    }
}

car_model *__read_car_from_db__(int seek) {
    FILE *file = fopen("cars.db", "rb");

    car_model *object = malloc(sizeof(car_model));

    if (file != NULL) {
        fseek(file, seek, SEEK_SET);
        fread(object, sizeof(car_model), 1, file);
    }
    fclose(file);
    return object;
}

int __update_car_db__(int seek, car_model *newcar) {
    FILE *file = fopen("cars.db", "r+");

    if (file != NULL) {
        fseek(file, seek, SEEK_SET);
        fwrite(newcar, sizeof(car_model), 1, file);
        fclose(file);
        return 0;
    } else {
        fclose(file);
        return 1;
    }
}

car_model create_car(char *vin) {
    car_model *car = malloc(sizeof(car_model));
    strcpy((char *) car->vin, vin);
    car->car_id = (int)time(NULL) - 1500000000;

    return *car;
}

car_model create_empty_car() {
    car_model *car = malloc(sizeof(car_model));
    strcpy((char *) car->vin, "");
    car->car_id = 0;

    return *car;
}

int insert_car(car_model car) {
    int first_byte = __append_car_to_db__(car);
    index_file_model car_index = create_index(car.car_id, first_byte);
    append_index_to_file("cars.index", car_index);
    return 0;
}

car_model get_car(int id) {
    if (id == 0) {
        return create_empty_car();
    }

    index_file_model *indexes = get_file_indexes("cars.index");
    for (int i = 0; i < 100; i++) {
        if (indexes[i].id == id) {
            return *__read_car_from_db__(indexes[i].first_byte);
        }
    }
    return create_empty_car();
}

int update_car(int id, car_model car) {
    car.car_id = id;

    index_file_model *indexes = get_file_indexes("cars.index");
    for (int i = 0; i < 100; i++) {
        if (indexes[i].id == id) {
            __update_car_db__(indexes[i].first_byte, &car);
        }
    }
    return 0;
}

int count_cars(void) {
    return count_indexes("cars.index");
}

int remove_cars_data(void) {
    remove("cars.db");
    remove("cars.index");
    write_file_content("cars.db", "", 0);
    write_file_content("cars.index", "", 0);
    return 0;
}

int remove_car(int id){
    car_model found = get_car(id);
    if (found.car_id == 0) {
        return 1;
    }
    car_model *db = __get_cars_db__();

    car_model *filtered_db = malloc(sizeof(car_model) * 100);
    int filtered_counter = 0;
    for (int i = 0; i < 100; i++) {
        if (db[i].car_id != id) {
            filtered_db[filtered_counter++] = db[i];
        }
    }

    remove_cars_data();

    for (int i = 0; i < 100; i++) {
        if (filtered_db[i].car_id != 0) {
            insert_car(filtered_db[i]);
        }
    }


    index_file_model *indexes = get_file_indexes("cars_customers.index");
    for (int i = 0; i < 100; i++){
        cars_customers_model cars_customers = get_cars_customers(indexes[i].id);
        if (cars_customers.car_id == id){
            remove_cars_customers(indexes[i].id);
        }
    }

    return 0;
}