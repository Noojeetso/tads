#ifndef __BIGFLOAT_H__
#define __BIGFLOAT_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "longint.h"

#define MAX_MANTISSA_DIGITS 30
#define MAX_EXPONENT_DIGITS 5

#define MAX_EXPONENT 99999
#define MIN_EXPONENT -99999

typedef struct
{
    sdigit exponent;
    longint_t *mantissa;
} bigfloat_t;

bigfloat_t *
new_bigfloat(void);

void
free_bigfloat(bigfloat_t **z);

bigfloat_t *
bigfloat_mul(bigfloat_t *a,
             bigfloat_t *b);

void
print_bigfloat(bigfloat_t *v);

void
set_bigfloat(bigfloat_t *z,
             longint_t *new_mantissa,
             sdigit new_exponent);

int
input_bigfloat(bigfloat_t *v);

#endif // __BIGFLOAT_H__

