#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include <stdlib.h>
#include <stdbool.h>
#include "print.h"
#include "errors.h"

#define HASH_OPENED_COEFFICIENT 0.72
#define HASH_CLOSED_COEFFICIENT 1.5
#define HASHTABLE_STEP_DIVISION 10
#define EXPAND_FACTOR 2.0f
#define MIN_COMPARES 1.0f
#define MAX_COMPARES 100.0f

typedef struct hashtable_opened hashtable_opened_t;

typedef struct hashtable_closed hashtable_closed_t;

size_t
get_hashtable_opened_size(hashtable_opened_t *hashtable);

size_t
get_hashtable_closed_size(hashtable_closed_t *hashtable);

size_t
get_hashtable_opened_size_bytes(hashtable_opened_t *hashtable);

size_t
get_hashtable_closed_size_bytes(hashtable_closed_t *hashtable);

size_t
get_hashtable_comparisons(void);

void
set_hashtable_comparisons(size_t new_comparisons);

hashtable_opened_t *
new_hashtable_opened(size_t max_size);

void
clear_table_opened(hashtable_opened_t *hashtable);

void
free_table_opened(hashtable_opened_t *hashtable);

hashtable_closed_t *
new_hashtable_closed(size_t max_size);

void
clear_table_closed(hashtable_closed_t *hashtable);

void
free_table_closed(hashtable_closed_t *hashtable);

size_t
get_hash_opened(int key,
                size_t max_size);

size_t
get_hash_closed(int key,
                size_t max_size,
                size_t step,
                size_t iterations);

int
push_table_opened(hashtable_opened_t *hashtable,
                  int value);

int
push_table_closed(hashtable_closed_t *hashtable,
                  int value);

int
fill_hashtable_opened(FILE *input_file,
                      hashtable_opened_t *hashtable);

int
fill_hashtable_closed(FILE *input_file,
                      hashtable_closed_t *hashtable);

int
hashtable_opened_pop(hashtable_opened_t *hashtable,
                     int value);

int
hashtable_closed_pop(hashtable_closed_t *hashtable,
                     int value);

int
hashtable_opened_find(hashtable_opened_t *hashtable,
                      int value);

void
hashtable_opened_search(hashtable_opened_t *hashtable,
                        int value);

int
hashtable_closed_find(hashtable_closed_t *hashtable,
                      int value);

void
hashtable_closed_search(hashtable_closed_t *hashtable,
                        int value);

void
print_hashtable_opened(hashtable_opened_t *hashtable);

void
print_hashtable_closed(hashtable_closed_t *hashtable);

#endif  // __HASHTABLE_H__
