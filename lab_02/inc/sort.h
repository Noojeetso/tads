#ifndef __SORT_H__
#define __SORT_H__

#include "table.h"

#define MAX_SORT_TABLE_SIZE 5000
#define SORT_ITERATIONS 100
#define SORT_AMOUNT 6

int
compare_records_by_theater_name(const void *void_record_1,
                                const void *void_record_2);

int
compare_keys_by_theater_name(const void *void_key_1,
                             const void *void_key_2);

void
bubble_sort(void *base,
            size_t num,
            size_t size,
            int (*compare)(const void*, const void*));

int
compare_efficiency(void);

#endif // __SORT_H__

