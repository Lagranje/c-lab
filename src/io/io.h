#ifndef C_IO_H
#define C_IO_H
#include <wchar.h>

int write_file_content(char *filename, char *content, size_t content_length);

char *read_file_content(char *filename, int start, size_t content_length);

size_t get_file_content_length(char *filename);

#endif //C_IO_H
