#ifndef __FILES_H__
#define __FILES_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "errors.h"

#define READ_MODE "r"
#define WRITE_MODE "w"
#define APPEND_MODE "a"
#define READ_WRITE_MODE "r+"

typedef struct array
{
    int *data;
    size_t size;
    size_t max_size;
} array_t;

size_t
get_file_comparisons(void);

void
set_file_comparisons(size_t new_comparisons);

size_t
get_array_size_bytes(array_t *array);

array_t *
new_array(size_t size);

void
free_array(array_t *array);

array_t *
file_to_array(const char *file_name);

int
file_push(char *file_name,
          int value);

int
file_pop(char *file_name,
         int value);

int
file_find(char *file_name,
          int value);

void
file_search(char *file_name,
            int value);

size_t
get_records_amount(FILE *file);

int
array_search_number(array_t *array,
                    int value,
                    size_t *index);

#endif  // __FILES_H__
