#ifndef __SCAN_H__
#define __SCAN_H__

#include <math.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int
get_value(char *buffer,
          size_t buffer_size,
          int *value);

int *
scan_array(int *array_size);

#endif  // __SCAN_H__
