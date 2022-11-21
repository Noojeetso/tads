#ifndef __FILES_H__
#define __FILES_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "errors.h"

#define READ_MODE "r"
#define WRITE_MODE "w"
#define READ_WRITE_MODE "r+"

int
file_push(char *file_name,
          int value);

int
file_pop(char *file_name,
         int value);

#endif  // __FILES_H__
