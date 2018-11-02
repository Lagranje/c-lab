#include "src/io/io.h"
#include <string.h>
#include "src/db/customers/customers.h"
#include "src/db/cars/cars.h"
#include "src/db/cars_customers/cars_customers.h"
#include "src/db/index_file/index_file.h"

void clear_files(void) {
    remove_customers_data();
    remove_cars_data();
    remove_cars_customers_data();
}

void log_customers_to_console(void) {
    index_file_model *x = get_file_indexes("customers.index");
    for (int i = 0; i < 100; i++) {
        if (x[i].id != 0){
            printf("\ncustomer_id: %d, ", x[i].id);
            customer_model customer = get_customer(x[i].id);
            printf("name: %s, surname: %s, phone: %d", customer.name, customer.surname, customer.phone);
        }
    }
};

void log_cars_customers_to_console(void) {
    index_file_model *x = get_file_indexes("cars_customers.index");
    for (int i = 0; i < 100; i++) {
        if (x[i].id != 0) {
            printf("\ncars_customers_id: %d, ", x[i].id);
            cars_customers_model cars_customers = get_cars_customers(x[i].id);
            printf("car_id: %d, customer_id: %d", cars_customers.car_id, cars_customers.customer_id);
        }
    }
};

void log_cars_to_console(void) {
    index_file_model *x = get_file_indexes("cars.index");
    for (int i = 0; i < 100; i++) {
        if (x[i].id != 0){
            printf("\nid: %d, ", x[i].id);
            car_model car = get_car(x[i].id);
            printf("vin: %s", car.vin);
        }
    }
};

char *tolowercase(char *string){
    char *new_string = malloc(strlen(string) * sizeof(char));
    for (int i = 0; i < strlen(string); i++){
        new_string[i] = tolower(string[i]);
    }
    return new_string;
}

int startswith(char *pre, char *str) {
    size_t lenpre = strlen(pre),
            lenstr = strlen(str);
    return lenstr < lenpre
           ? 0
           : strncmp(pre, str, lenpre) == 0;
}

customer_model parse_customer_from_command(char *command){
    char *ss = strstr(command, "customer");
    char *res = malloc(sizeof(char) * strlen(ss) - 4);
    int res_count = 0;
    for (int i = 5; i < strlen(ss); i++){
        res[res_count++] = ss[i];
    }
    char * first_space = strchr(res, ' ');
    int first_space_index = (int)(first_space - res);
    char *name = malloc(sizeof(char) * first_space_index);
    int name_count = 0;
    for (int i = 0; i < first_space_index; i++){
        name[name_count++] = res[i];
    }
    char *next = first_space + 1;
    char * next_space = strchr(next, ' ');
    int next_space_index = (int)(next_space - next);
    char *surname = malloc(sizeof(char) * next_space_index);
    int surname_count = 0;
    for (int i = 0; i < next_space_index; i++){
        surname[surname_count++] = next[i];
    }
    int phone = atoi(next_space + 1);

    customer_model customer = create_customer(phone, name, surname);
    return customer;
}

car_model parse_car_from_command(char *command){
    char *ss = strstr(command, "car");
    char *res = malloc(sizeof(char) * strlen(ss) - 5);
    int res_count = 0;
    for (int i = 5; i < strlen(ss); i++){
        res[res_count++] = ss[i];
    }
    car_model car = create_car(res);
    return car;
}

cars_customers_model parse_cars_customers_from_command(char *command){
    char *ss = strstr(command, "cars_customers");
    int res_count = 0;
    char *res = malloc(sizeof(char) * strlen(ss) - 13);
    for (int i = 13; i < strlen(ss); i++){
        res[res_count++] = ss[i];
    }
    char * first_space = strchr(res, ' ');
    int first_space_index = (int)(first_space - res);
    char *car_id = malloc(sizeof(char) * first_space_index);
    int car_id_count = 0;
    for (int i = 0; i < first_space_index; i++){
        car_id[car_id_count++] = res[i];
    }

    cars_customers_model cars_customers = create_cars_customers(atoi(car_id), atoi(first_space + 1));
    return cars_customers;
}

char *extract(const char *string, int start, int end){
    char *result = malloc(sizeof(char) * (end - start));
    int count = 0;
    int i = start;

    while(i < end) {
        result[count++] = string[i++];
    }

    return result;
}

int main() {
    char *raw_input = malloc(sizeof(char) * 200);

    while(1){
        printf("\nenter command: \n");
        scanf("%[^\n]%*c", raw_input);

        char *command = tolowercase(raw_input);
        int command_length = (int)strlen(command);

        if (startswith("exit", command)){
            break;
        }

        if (startswith("select customers for car", command)) {
            char *id_string = malloc(sizeof(char) * 50);
            printf("what car? ");
            scanf("%[^\n]%*c", id_string);
            index_file_model *x = get_file_indexes("cars_customers.index");
            for (int i = 0; i < 100; i++) {
                cars_customers_model cars_customers = get_cars_customers(x[i].id);
                if (cars_customers.car_id == atoi(id_string) && atoi(id_string) != 0) {
                    customer_model customer = get_customer(cars_customers.customer_id);
                    printf(
                            "\ncar_id: %d, name: %s, surname: %s, phone: %d",
                            customer.customer_id,
                            customer.name,
                            customer.surname,
                            customer.phone
                    );
                }
            }
            continue;
        }

        if (startswith("select cars for customer", command)) {
            char *id_string = malloc(sizeof(char) * 50);
            printf("what customer? ");
            scanf("%[^\n]%*c", id_string);
            index_file_model *x = get_file_indexes("cars_customers.index");
            for (int i = 0; i < 100; i++) {
                cars_customers_model cars_customers = get_cars_customers(x[i].id);
                if (cars_customers.customer_id == atoi(id_string) && atoi(id_string) != 0) {
                    car_model car = get_car(cars_customers.car_id);
                    printf("\ncar_id: %d, vin: %s", car.car_id, car.vin);
                }
            }
            continue;
        }

        if (startswith("select customer", command)){
            char *param = extract(command, 12, command_length);
            if (param[0] == '*'){
                log_customers_to_console();
                continue;
            }
            int number = atoi(param);
            customer_model selected = get_customer(number);
            printf(
                "\ncustomer_id: %d, name: %s, surname: %s, phone: %d\n",
                selected.customer_id,
                selected.name,
                selected.surname,
                selected.phone
            );
            continue;
        }

        if (startswith("select cars_customers", command)){
            char *param = extract(command, 20, command_length);
            if (param[0] == '*'){
                log_cars_customers_to_console();
                continue;
            }
            int number = atoi(param);
            cars_customers_model selected = get_cars_customers(number);
            printf(
                "\ncars_customers_id: %d, car_id: %d, customer_id: %d",
                selected.cars_customers_id,
                selected.car_id,
                selected.customer_id
            );
            continue;
        }

        if (startswith("select car", command)){
            char *param = extract(command, 13, command_length);
            if (param[0] == '*'){
                log_cars_to_console();
                continue;
            }
            int number = atoi(param);
            car_model selected = get_car(number);
            printf(
                "\ncar_id: %d, vin: %s",
                selected.car_id,
                selected.vin
            );
            continue;
        }

        if (startswith("insert customer", command)){
            customer_model customer = parse_customer_from_command(command);
            printf("created customer with id %d", customer.customer_id);
            insert_customer(customer);
            continue;
        }

        if (startswith("update customer", command)){
            customer_model customer = parse_customer_from_command(command);
            printf("what customer you want to update? ");
            char* id_string = malloc(sizeof(char) * 50);
            scanf("%[^\n]%*c", id_string);
            update_customer(atoi(id_string), customer);
            printf("customer %d updated.\n", customer.customer_id);
            continue;
        }

        if (startswith("insert cars_customers", command)){
            cars_customers_model cars_customers = parse_cars_customers_from_command(command);
            printf("created cars_customers with id %d", cars_customers.cars_customers_id);
            insert_cars_customers(cars_customers);
            continue;
        }

        if (startswith("update cars_customers", command)){
            cars_customers_model cars_customers = parse_cars_customers_from_command(command);
            printf("what cars_customers you want to update? ");
            char* id_string = malloc(sizeof(char) * 50);
            scanf("%[^\n]%*c", id_string);
            update_cars_customers(atoi(id_string), cars_customers);
            printf("cars_customers %d updated.\n", cars_customers.cars_customers_id);
            continue;
        }

        if (startswith("insert car", command)){
            car_model car = parse_car_from_command(command);
            printf("created car with id %d", car.car_id);
            insert_car(car);
            continue;
        }

        if (startswith("update car", command)){
            car_model car = parse_car_from_command(command);
            printf("what car you want to update? ");
            char* id_string = malloc(sizeof(char) * 50);
            scanf("%[^\n]%*c", id_string);
            update_car(atoi(id_string), car);
            printf("car %d updated.\n", car.car_id);
            continue;
        }

        if (startswith("count customers", command)){
            printf("customers count - %d", count_customers());
            continue;
        }

        if (startswith("count cars_customers", command)){
            printf("customers count - %d", count_cars_customers());
            continue;
        }

        if (startswith("count cars", command)){
            printf("customers count - %d", count_cars());
            continue;
        }

        if (startswith("remove customer", command)) {
            printf("what customer you want to remove? ");
            char *id_string = malloc(sizeof(char) * 50);
            scanf("%[^\n]%*c", id_string);
            remove_customer(atoi(id_string));
            printf("customer %s removed", id_string);
            continue;
        }

        if (startswith("remove cars_customers", command)) {
            printf("what cars_customers you want to remove? ");
            char *id_string = malloc(sizeof(char) * 50);
            scanf("%[^\n]%*c", id_string);
            remove_cars_customers(atoi(id_string));
            printf("cars_customers %s removed", id_string);
            continue;
        }

        if (startswith("remove car", command)) {
            printf("what cars_customers you want to remove? ");
            char *id_string = malloc(sizeof(char) * 50);
            scanf("%[^\n]%*c", id_string);
            remove_car(atoi(id_string));
            printf("car %s removed", id_string);
            continue;
        }

        if (startswith("clear database", command)) {
            printf("are you sure? ");
            char *answer = malloc(sizeof(char) * 50);
            scanf("%[^\n]%*c", answer);
            if (answer[0] == 'y') {
                clear_files();
            }
        }
    }

    return 0;
}
