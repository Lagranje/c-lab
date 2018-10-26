#include "autos.h"

int auto_id_counter = 1;


int remove_autos_data() {
    remove("autos.db");
    remove("autos.index");

    return 0;
}

auto_model *__get_auto_db__() {
    FILE *file = fopen("autos.db", "rb");
    int i = 0;
    auto_model *auto = malloc(sizeof(auto_model) * 100);
    while (1) {
        fread(&auto[i], sizeof(auto_model), 1, file);
        i++;
        if (feof(file)) {
            break;
        }
    }
    fclose(file);
    return autos;
}

int __update_auto_db(int seek, auto_model *newAuto) {
    FILE *file = fopen("groups.db", "r+");

    if (file != NULL) {
        fseek(file, seek, SEEK_SET);
        fwrite(newAuto, sizeof(auto_model), 1, file);
        fclose(file);
        return 0;
    } else {
        fclose(file);
        return 1;
    }
}

int __append_auto_to_db__(auto_model auto) {
    FILE *file = fopen("autos.db", "a");
    int first_byte = (int) ftell(file);

    auto_model *object = malloc(sizeof(auto_model));
    object->auto_id = auto.auto_id;
    object->customer_id = auto.customer_id;
    strcpy((char *) object->vin, (char *) auto.vin);

    if (file != NULL) {
        fwrite(object, sizeof(auto_model), 1, file);
        fclose(file);
        return first_byte;
    } else {
        fclose(file);
        return 1;
    }
}

auto_model *__read_auto_from_db__(int seek) {
    FILE *file = fopen("autos.db", "rb");

    auto_model *object = malloc(sizeof(auto_model));

    if (file != NULL) {
        fseek(file, seek, SEEK_SET);
        fread(object, sizeof(auto_model), 1, file);
    }
    fclose(file);
    return object;
}

auto_model get_auto(int id) {
    if (id == 0) {
        return create_auto("");
    }

    index_file_model *indexes = get_file_indexes("autos.index");
    for (int i = 0; i < 100; i++) {
        if (indexes[i].id == id) {
            return *__read_auto_from_db__(indexes[i].first_byte);
        }
    }
    return create_auto("");
}

auto_model create_auto(char *vin, int customer_id) {
    auto_model *auto = malloc(sizeof(auto_model));
    strcpy((char *) auto->vin, vin);
    auto->customer_id = customer_id;
    auto->auto_id = auto_id_counter++;
    return *auto;
}

int insert_auto(group_model auto) {
    int first_byte = __append_auto_to_db__(auto);
    index_file_model auto_index = create_index(auto.group_id, first_byte);
    append_index_to_file("autos.index", auto_index);
    return 0;
}

int update_auto(int id, auto_model auto) {
    auto.auto_id = id;

    index_file_model *indexes = get_file_indexes("autos.index");
    for (int i = 0; i < 100; i++) {
        if (indexes[i].id == id) {
            __update_auto_db(indexes[i].first_byte, *auto);
        }
    }
    return 0;
}

int count_autos() {
    return count_indexes("autos.index");
}

int remove_auto(int id){
    auto_model found = get_auto(id);
    if (found.auto_id == 0) {
        return 1;
    }
    auto_model *db = __get_auto_db__();

    auto_model *filtered_db = malloc(sizeof(auto_model) * 100);
    int filtered_counter = 0;
    for (int i = 0; i < 100; i++) {
        if (db[i].auto_id != id) {
            filtered_db[filtered_counter++] = db[i];
        }
    }

    remove_autos_data();

    for (int i = 0; i < 100; i++) {
        if (filtered_db[i].auto_id != 0) {
            insert_auto(filtered_db[i]);
        }
    }

    return 0;
}