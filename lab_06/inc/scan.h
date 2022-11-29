#ifndef __SCAN_H__
#define __SCAN_H__

#include <stdlib.h>
#include <stdio.h>
#include "errors.h"
#include "print.h"

int
scan_int(int *value,
         int min_value,
         int max_value);

int
scan_menu(int *key);

int
flush_input(void);

# endif  // __SCAN_H__
