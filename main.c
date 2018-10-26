#include "src/io/io.h"
#include "src/db/customers/customers.h"
#include "src/db/autos/autos.h"
#include "src/db/index_file/index_file.h"

void clear_files(void) {
    remove_autos_data();
    remove_autos_data();
}

void log_users_to_console() {
    printf("\n");
};

int main() {
     return 0;
}