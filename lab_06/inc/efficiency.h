#ifndef __EFFICIENCY_H__
#define __EFFICIENCY_H__

#include <limits.h>
#include "tree.h"
#include "files.h"
#include "print.h"

#define ITERATIONS 1
#define SMALL_FILE_NAME "database/numbers_small.txt"
#define MEDIUM_FILE_NAME "database/numbers_medium.txt"
#define BIG_FILE_NAME "database/numbers_big.txt"
#define SORTED_FILE_NAME "database/numbers_sorted.txt"
#define SORTED_REVERSE_FILE_NAME "database/numbers_sorted_reverse.txt"

void
print_efficiency(void);

#endif  // __EFFICIENCY_H__
