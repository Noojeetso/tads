#ifndef __LONGINT_H__
#define __LONGINT_H__

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "errors.h"

#define ABS(x) x < 0 ? -x : x

#define DECIMAL_BASE ((digit)1000000000)
#define DECIMAL_SHIFT 9
#define MASK 1073741823
#define BASE 1073741824
#define SHIFT 30

typedef uint32_t digit;
typedef int32_t sdigit;
typedef uint64_t twodigits;
typedef int64_t stwodigits;
typedef int64_t signed_size_t;

typedef struct
{
    digit *digits;
    signed_size_t size;
} longint_t;

longint_t *
new_longint(signed_size_t size);

void
free_longint(longint_t **z);

void
longint_normalize(longint_t *v);

char *
longint_to_string(longint_t *a);

longint_t *
longint_from_string(const char *str);

size_t
get_decimal_longint_length(longint_t *a);

longint_t *
long_long_mul(longint_t *a,
              longint_t *b);

int
longint_digit_add(longint_t **a,
                  digit b);

void
set_longint_to_digit(longint_t *z,
                  digit number);

int
is_longint_zero(longint_t *z);

#endif // __LONGINT_H__

