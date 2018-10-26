#ifndef C_AUTO_H

#define C_AUTO_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../io/io.h"
#include "../index_file/index_file.h"

typedef struct AUTO_MODEL {
    int auto_id;
    int customer_id;
    char vin[10];
} auto_model;

auto_model create_auto(char *vin, int customer_id);

int insert_auto(auto_model auto);

int remove_autos_data();

int remove_auto(int id);

auto_model get_auto(int id);

int update_auto(int id, auto_model auto);

int count_autos();

#endif