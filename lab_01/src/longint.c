#include "longint.h"

longint_t *
new_longint(signed_size_t size)
{
    longint_t *new_longint = malloc(sizeof(longint_t));
    if (new_longint == NULL)
    {
        fprintf(stderr, "%s\n", "Память для указателя на объект типа longint не была выделена");
        return NULL;
    }
    new_longint->digits = calloc(size, sizeof(digit));
    if (new_longint->digits == NULL)
    {
        fprintf(stderr, "%s\n", "Память для объекта типа longint не была выделена");
        return NULL;
    }
    new_longint->size = size;
    return new_longint;
}

void
longint_normalize(longint_t *v)
{
    signed_size_t j = v->size < 0 ? -v->size : v->size;
    signed_size_t i = j;

    while (i > 1 && v->digits[i - 1] == 0)
        i--;

    if (i != j)
        v->size = v->size < 0 ? -i : i;
}

void
free_longint(longint_t **z)
{
    free((*z)->digits);
    free(*z);
    *z = NULL;
}

longint_t *
long_long_mul(longint_t *a, longint_t *b)
{
    longint_t *z;
    signed_size_t size_a = ABS(a->size);
    signed_size_t size_b = ABS(b->size);
    signed_size_t i;

    z = new_longint(size_a + size_b);
    if (z == NULL)
        return NULL;

    memset(z->digits, 0, z->size * sizeof(digit));
    for (i = 0; i < size_a; ++i)
    {
        twodigits carry = 0;
        twodigits f = a->digits[i];
        digit *pz = z->digits + i;
        digit *pb = b->digits;
        digit *pbend = b->digits + size_b;

        while (pb < pbend)
        {
            carry += *pz + *pb++ * f;
            *pz++ = (digit)(carry & MASK);
            carry >>= SHIFT;
            assert(carry <= MASK);
        }
        if (carry)
            *pz += (digit)(carry & MASK);
        assert((carry >> SHIFT) == 0);
    }

    z->size *= a->size < 0 ? -1 : 1;
    z->size *= b->size < 0 ? -1 : 1;

    longint_normalize(z);

    return z;
}

void
set_longint_to_digit(longint_t *z,
                     digit number)
{
    signed_size_t size = ABS(z->size);
    z->digits[0] = number;
    for (signed_size_t i = 1; i < size; i++)
        z->digits[i] = 0;
    z->size = 1;
}

int
is_longint_zero(longint_t *z)
{
    return (ABS(z->size) == 1) && (z->digits[0] == 0);
}

int
longint_digit_add(longint_t **a,
                  digit b)
{
    signed_size_t size_a = ABS((*a)->size);
    int sign = (*a)->size / size_a;
    longint_t *z;
    signed_size_t i;
    digit carry = 0;

    z = new_longint(size_a + 1);
    if (z == NULL)
        return ERR_MEMORY_ALLOCATION;

    carry += (*a)->digits[0] + b;
    z->digits[0] = carry & MASK;
    carry >>= SHIFT;
    for (i = 1; i < size_a; ++i)
    {
        carry += (*a)->digits[i];
        z->digits[i] = carry & MASK;
        carry >>= SHIFT;
    }
    z->digits[i] = carry;
    z->size *= sign;
    longint_normalize(z);

    free_longint(a);
    *a = z;

    return EXIT_SUCCESS;
}

size_t
get_decimal_longint_length(longint_t *a)
{
    longint_t *scratch;
    signed_size_t size_a;

    if (a == NULL)
    {
        fprintf(stderr, "%s\n", "Указатель на объект типа longint равен NULL");
        return ERR_LONGINT_POINTS_NULL;
    }

    size_a = ABS(a->size);

    int d = (33 * DECIMAL_SHIFT) / (10 * SHIFT - 33 * DECIMAL_SHIFT);
    signed_size_t size = 1 + size_a + size_a / d;
    scratch = new_longint(size);
    if (scratch == NULL)
    {
        fprintf(stderr, "%s\n", "Ошибка выделения памяти под десятичное длинное число");
        return ERR_MEMORY_ALLOCATION;
    }

    signed_size_t i, j;
    digit *pin = a->digits;
    digit *pout = scratch->digits;
    for (i = size_a - 1; i >= 0; i--)
    {
        digit hi = pin[i];
        for (j = 0; j < size; j++)
        {
            twodigits z = (twodigits)pout[j] << SHIFT | hi;
            hi = (digit)(z / DECIMAL_BASE);
            pout[j] = (digit)(z - (twodigits)hi * DECIMAL_BASE);
        }
        while (hi)
        {
            pout[size++] = hi % DECIMAL_BASE;
            hi /= DECIMAL_BASE;
        }
    }

    if (size == 0)
        pout[size++] = 0;

    size_t digit_num = 0;
    digit rem, tenpow;
    tenpow = 10;
    rem = pout[size - 1];
    while (rem >= tenpow)
        tenpow *= 10;

    size_t zero_counter = 0;
    for (i = 0; i < size - 1; i++)
    {
        rem = pout[i];
        for (j = 0; j < DECIMAL_SHIFT; j++)
        {
            if (rem % 10 == 0)
                zero_counter++;
            else
                zero_counter = 0;
            digit_num++;
            rem /= 10;
        }
    }
    digit_num -= zero_counter;

    free(scratch);

    return digit_num;
}

