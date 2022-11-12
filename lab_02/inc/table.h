#ifndef __TABLE_H__
#define __TABLE_H__

#include <pthread.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include "record.h"

#define MAIN_TABLE_SIZE 40

typedef struct record_table
{
    record_t *records;
    key_t *keys;
    size_t max_size;
    size_t size;
} record_table_t;

typedef struct print_arguments
{
    record_table_t *record_table;
    void (*print_function)(record_table_t *record_table, size_t start_index, size_t max_rows, int offset);
    char current_char;
} print_arguments_t;

int
nanosleep(const struct timespec *req,
          struct timespec *rem);

int
init_record_table(record_table_t *record_table,
                  size_t max_size);

void
clear_table(record_table_t *record_table);

void
free_table(record_table_t *record_table);

int
is_table_empty(record_table_t *record_table);

int
is_table_full(record_table_t *record_table);

int
check_table_empty(record_table_t *record_table);

int
check_table_full(record_table_t *record_table);

void
print_table_keys(record_table_t *record_table,
                 size_t start_index,
                 size_t max_rows,
                 int offset);

void
print_table(record_table_t *record_table,
            size_t start_index,
            size_t max_rows,
            int offset);

void
print_table_with_keys(record_table_t *record_table,
                      size_t start_index,
                      size_t max_rows,
                      int offset);

void
print_horizontal_scroll(print_arguments_t *print_arguments);

int
add_records_from_file(record_table_t *main_record_table,
                      char *file_name);

int
add_record_from_console(record_table_t *main_record_table);

int
delete_by_position(record_table_t *record_table);

int
print_musicals_filtered(print_arguments_t *print_arguments);

#endif  // __TABLE_H__

