#ifndef __EFFICIENCY_H__
#define __EFFICIENCY_H__

#include <limits.h>
#include <time.h>
#include "tree.h"
#include "files.h"
#include "print.h"

#define ITERATIONS 5
#define SMALL_FILE_NAME "database/numbers_small.txt"
#define MEDIUM_FILE_NAME "database/numbers_medium.txt"
#define BIG_FILE_NAME "database/numbers_big.txt"
#define SORTED_FILE_NAME "database/numbers_sorted.txt"
#define SORTED_REVERSE_FILE_NAME "database/numbers_sorted_reverse.txt"

typedef struct results
{

    size_t avl_sort[5];
    size_t bst_sort[5];
    size_t file_sort[5];
    size_t avl_search[5];
    size_t bst_search[5];
    size_t file_search[5];
    double avl_factors[5];
    double bst_factors[5];
} results_t;

void
print_efficiency(void);

#endif  // __EFFICIENCY_H__
