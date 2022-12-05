#ifndef __EFFICIENCY_H__
#define __EFFICIENCY_H__

#include <limits.h>
#include <time.h>
#include "tree.h"
#include "hashtable.h"
#include "files.h"
#include "print.h"
#include "scan.h"

#define ITERATIONS 10
#define FILES_AMOUNT 5
#define SMALL_FILE_NAME "database/numbers_small.txt"
#define MEDIUM_FILE_NAME "database/numbers_medium.txt"
#define BIG_FILE_NAME "database/numbers_big.txt"
#define SORTED_FILE_NAME "database/numbers_sorted.txt"
#define SORTED_REVERSE_FILE_NAME "database/numbers_sorted_reverse.txt"

typedef struct results
{

    size_t avl[FILES_AMOUNT];
    size_t bst[FILES_AMOUNT];
    size_t file[FILES_AMOUNT];
    size_t hashtable_opened[FILES_AMOUNT];
    size_t hashtable_closed[FILES_AMOUNT];
    size_t hashtable_opened_increased[FILES_AMOUNT];
    size_t hashtable_closed_increased[FILES_AMOUNT];
    size_t avl_size[FILES_AMOUNT];
    size_t bst_size[FILES_AMOUNT];
    size_t file_size[FILES_AMOUNT];
    size_t hashtable_opened_size[FILES_AMOUNT];
    size_t hashtable_closed_size[FILES_AMOUNT];
    size_t hashtable_opened_increased_size[FILES_AMOUNT];
    size_t hashtable_closed_increased_size[FILES_AMOUNT];
} results_t;

void
print_efficiency(void);

#endif  // __EFFICIENCY_H__
