#include "longint.h"

char *
longint_to_string(longint_t *a)
{

    char *bytes_str;
    longint_t *scratch;
    signed_size_t str_len, size_a;
    int negative;

    if (a == NULL)
    {
        fprintf(stderr, "%s\n", "Указатель на объект типа longint равен NULL");
        return NULL;
    }

    size_a = ABS(a->size);
    negative = a->size < 0;

    int d = (33 * DECIMAL_SHIFT) / (10 * SHIFT - 33 * DECIMAL_SHIFT);
    signed_size_t size = 1 + size_a + size_a / d;
    scratch = new_longint(size);
    if (scratch == NULL)
    {
        fprintf(stderr, "%s\n", "Память для объекта типа longint не была выделена");
        return NULL;
    }

    signed_size_t i, j;
    digit *pin = a->digits;
    digit *pout = scratch->digits;
    for (i = size_a; --i >= 0; )
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

    digit rem, tenpow;
    str_len = negative + 1 + (size - 1) * DECIMAL_SHIFT;
    tenpow = 10;
    rem = pout[size - 1];
    while (rem >= tenpow)
    {
        tenpow *= 10;
        str_len++;
    }

    bytes_str = calloc(str_len, sizeof(char));
    if (bytes_str == NULL)
    {
        free(scratch);
        return NULL;
    }

    char *p = bytes_str + str_len;
    for (i = 0; i < size - 1; i++)
    {
        rem = pout[i];
        for (j = 0; j < DECIMAL_SHIFT; j++)
        {
            *--p = '0' + rem % 10;
            rem /= 10;
        }
    }

    p = bytes_str + 2;
    while (*p == '0')
        p++;
    if (*p == '\0')
        p--;
    signed_size_t diff = p - bytes_str - negative;
    for (p = bytes_str + negative; p < bytes_str + str_len - 1; p++)
        *p = *(p + diff);

    if (negative)
        *bytes_str = '-';

    free(scratch);

    return bytes_str;
}

longint_t *
longint_from_string(const char *str)
{
    int sign = 1;
    const char *start = str;

    if (*str == '+') {
        ++str;
    }
    else if (*str == '-') {
        ++str;
        sign = -1;
    }

    double log_base_BASE = (log((double)10) / log((double)BASE));
    int convwidth = DECIMAL_SHIFT;
    twodigits convmultmax = DECIMAL_BASE;

    size_t digits = 0;
    const char *scan = str;
    while (*scan != '\0' && *scan - '0' < 10)
    {
        ++digits;
        ++scan;
    }

    signed_size_t size_z = (signed_size_t)((double)digits * log_base_BASE + 1.0);

    assert(size_z > 0);

    longint_t *z = NULL;
    z = new_longint(size_z);
    if (z == NULL)
    {
        fprintf(stderr, "%s\n", "Память для объекта типа longint не была выделена");
        return NULL;
    }

    int i;
    twodigits c;
    twodigits convmult;
    while (str < scan)
    {
        c = (digit)(*str++ - '0');
        for (i = 1; i < convwidth && str != scan; str++)
        {
            i++;
            c = (twodigits)(c * 10 + (int)(*str - '0'));
        }

        convmult = convmultmax;
        if (i != convwidth)
        {
            convmult = 10;
            for ( ; i > 1; i--)
                convmult *= 10;
        }

        digit *pz, *pzstop;
        pz = z->digits;
        pzstop = pz + z->size;
        for (; pz < pzstop; pz++)
        {
            c += (twodigits)*pz * convmult;
            *pz = (digit)(c & MASK);
            c >>= SHIFT;
        }

        if (c)
        {
            assert(c < BASE);
            if (z->size < size_z)
            {
                *pz = (digit)c;
                z->size += 1;
            }
            else
            {
                longint_t *tmp = NULL;

                assert(z->size == size_z);

                tmp = new_longint(size_z + 1);
                if (tmp == NULL)
                {
                    fprintf(stderr, "%s\n", "Память для объекта типа longint не была выделена");
                    free(z);
                    return NULL;
                }
                memcpy(tmp->digits,
                       z->digits,
                       sizeof(digit) * size_z);
                free(z);
                z = tmp;
                z->digits[size_z] = (digit)c;
                ++size_z;
            }
        }
    }

    if (str == start)
    {
        fprintf(stderr, "%s\n", "Не было считано ни одного символа");
        free(z);
        return NULL;
    }

    if (sign < 0)
        z->size = -z->size;

    longint_normalize(z);
    return (longint_t *) z;
}

