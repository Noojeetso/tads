#include "bigfloat.h"
#include "longint.h"
#include <stdlib.h>

bigfloat_t *
new_bigfloat(void)
{
    bigfloat_t *z = NULL;
    z = calloc(1, sizeof(bigfloat_t));
    if (z == NULL)
    {
        fprintf(stderr, "%s\n", "Ошибка выделения памяти для объекта типа bigfloat");
        return NULL;
    }

    z->mantissa = NULL;
    z->exponent = 1;

    return z;
}

void
free_bigfloat(bigfloat_t **z)
{
    if ((*z)->mantissa != NULL)
       free_longint(&(*z)->mantissa);
    free(*z);
    *z = NULL;
}

void
print_bigfloat(bigfloat_t *v)
{
    if (v->exponent < MIN_EXPONENT)
    {
        printf("%s\n", "+0.0E+0");
        return;
    }
    if (v->exponent > MAX_EXPONENT)
    {
        printf("%cinf\n", v->mantissa->size < 0 ? '-' : '+');
        return;
    }
    char *mantissa_str = longint_to_string(v->mantissa);
    int mantissa_negative = 0;
    if (mantissa_str[0] == '-')
        mantissa_negative = 1;
    printf("%s.%sE%c%d\n", mantissa_negative ? "-0" : "+0",
                           mantissa_str + mantissa_negative,
                           v->exponent < 0 ? '-' : '+',
                           ABS(v->exponent));
}

int
round_mantissa(longint_t **v)
{
    int overflow = 0;
    int sign = (*v)->size < 0 ? -1 : 1;
    char *mantissa_str = longint_to_string(*v);

    char *mantissa_str_p = mantissa_str;
    if (*mantissa_str_p == '-')
        mantissa_str_p++;

    if (strlen(mantissa_str_p) > MAX_MANTISSA_DIGITS)
    {
        char max_char = mantissa_str_p[MAX_MANTISSA_DIGITS];

        mantissa_str_p[MAX_MANTISSA_DIGITS] = '\0';

        longint_t *rounded_mantissa = longint_from_string(mantissa_str);
        free(mantissa_str);

        if (max_char >= '5')
        {
            longint_digit_add(&rounded_mantissa, 1);
            if (get_decimal_longint_length(rounded_mantissa) > MAX_MANTISSA_DIGITS)
            {
                set_longint_to_digit(rounded_mantissa, 1);
                rounded_mantissa->size *= sign;
            }
            overflow = 1;
        }

        free(*v);
        *v = rounded_mantissa;
    }
    return overflow;
}

bigfloat_t *
bigfloat_mul(bigfloat_t *a,
             bigfloat_t *b)
{
    bigfloat_t *v = new_bigfloat();
    longint_t *result_mantissa;

    if (is_longint_zero(a->mantissa) || is_longint_zero(b->mantissa))
    {
        v->mantissa = new_longint(1);
        set_longint_to_digit(v->mantissa, 0);
        v->exponent = 0;
        return v;
    }

    result_mantissa = long_long_mul(a->mantissa, b->mantissa);

    v->exponent = a->exponent + b->exponent;

    // Уменьшаем разряд на 1, если количество знаков результриующей мантиссы больше максимального или меньше суммы количества знаков исходных мантисс
    size_t mantissa_length = get_decimal_longint_length(result_mantissa);
    if (mantissa_length < get_decimal_longint_length(a->mantissa) + get_decimal_longint_length(b->mantissa) ||
        mantissa_length > MAX_MANTISSA_DIGITS)
        v->exponent--;

    if (v->exponent < MIN_EXPONENT)
    {
        set_longint_to_digit(result_mantissa, 0);
        v->exponent = 0;
    }

    v->mantissa = result_mantissa;

    v->exponent += round_mantissa(&v->mantissa);

    return v;
}

int
input_bigfloat(bigfloat_t *v)
{
    char raw_str[MAX_MANTISSA_DIGITS + 1 + MAX_EXPONENT_DIGITS + 1 + 1] = "";
    signed char ch;
    size_t str_length = 0;
    size_t max_str_length = MAX_MANTISSA_DIGITS + 1 + MAX_EXPONENT_DIGITS + 1 + 1;
    char *exponent_ptr = NULL;
    char *dot_ptr = NULL;
    int mantissa_sign = 1;
    int exponent_sign = 1;
    char last_ch = '\0';
    char *str_p = raw_str;
    size_t end_zero_counter = 0;
    int exponent = 0;
    while (str_length < max_str_length)
    {
        ch = fgetc(stdin);

        if (ch == EOF || ch == '\n')
            break;

        if ((*raw_str == '\0') && (last_ch == '0' && ch == '0' && dot_ptr == NULL && exponent_ptr == NULL))
            continue;

        // Проверка на ввод знака
        if (ch == '+' || ch == '-')
        {
            if (*raw_str == '\0')
                mantissa_sign = (ch == '-' ? -1 : 1);
            else if (last_ch == 'e' || last_ch == 'E')
                exponent_sign = (ch == '-' ? -1 : 1);
            else
            {
                fprintf(stderr, "%s\n", "Некорректный символ знака");
                return EXIT_FAILURE;
            }
        }
        // Проверка на начало ввода порядка
        else if ((ch == 'e' || ch == 'E') && *raw_str != '\0' && exponent_ptr == NULL)
            exponent_ptr = str_p;
        // Проверка на начало ввода дробной части
        else if (ch == '.' && dot_ptr == NULL && exponent_ptr == NULL)
            dot_ptr = str_p;
        // Проверка на ввод цифры
        else if (isdigit(ch))
        {
            // Заполнение мантиссы
            if (exponent_ptr == NULL)
            {
                if (str_length >= MAX_MANTISSA_DIGITS)
                {
                    printf("%s\n", "Переполнение при вводе мантиссы");
                    return EXIT_FAILURE;
                }
                // Пропуск нулей мантиссы слева
                if (*raw_str == '0' && *(raw_str + 1) == '\0')
                {
                    if (ch == '0')
                    {
                        if (dot_ptr != NULL)
                            exponent--;
                    }
                    // Перезапись старшего разряда целой части, если он равен нулю
                    else
                    {
                        *raw_str = ch;
                        if (dot_ptr != NULL)
                            dot_ptr = raw_str;
                    }
                    continue;
                }
                // Пропуск разрядов мантиссы, выходящих за точность, кроме первого
                if (str_length >= MAX_MANTISSA_DIGITS + 1)
                {
                    if (dot_ptr == NULL)
                        exponent++;
                    continue;
                }
                // Учёт незначащих нулей справа
                if (*raw_str != '\0' && ch == '0')
                    end_zero_counter++;
                else
                    end_zero_counter = 0;
            }
            // Заполнение порядка
            else
            {
                // Пропуск незначащих нулей порядка
                if (exponent_ptr == str_p && ch == '0')
                    continue;
                // Пропуск разрядов порядка, выходящих за точность, кроме первого
                if (str_p - exponent_ptr >= MAX_EXPONENT_DIGITS + 1)
                    continue;
            }
            // Запись нового разряда
            *(str_p++) = ch;
            str_length++;
        }
        else
        {
            fprintf(stderr, "Неожиданный символ: '%c' после '%c'\n", ch, last_ch);
            return EXIT_FAILURE;
        }
        last_ch = ch;
    }

    if (str_length == 0)
    {
        fprintf(stderr, "%s\n", "Не было считано ни одного символа");
        return ERR_NO_CHARACTERS_READ;
    }

    // Обработка введённого порядка
    if (exponent_ptr != NULL)
    {
        exponent += exponent_sign * (int)(strtol(exponent_ptr, NULL, 10));
        *exponent_ptr = '\0';
        if (exponent > MAX_EXPONENT)
        {
            printf("%s\n", "Порядок введённого числа превышает допустимый");
            return EXIT_FAILURE;
        }
    }

    // Прибавление к порядку количества разрядов целой части
    char *raw_str_p = raw_str;
    while (*raw_str_p == '0')
        raw_str_p++;
    if (dot_ptr != NULL)
        exponent += dot_ptr - raw_str_p;
    else
        exponent += strlen(raw_str);

    // Удаление незначащих нулей
    if (end_zero_counter)
        raw_str[strlen(raw_str) - end_zero_counter] = '\0';
    longint_t *mantissa = longint_from_string(raw_str);

    // Проверка на ноль и присвоение знака мантиссы
    if (is_longint_zero(mantissa))
    {
        mantissa->size = 1;
        exponent = 0;
    }
    else
        mantissa->size *= mantissa_sign;

    // Проверка на переполнение порядка снизу
    /*
    if (exponent < MIN_EXPONENT)
    {
        set_longint_to_digit(mantissa, 0);
        exponent = 0;
    }
    */

    // Округление мантиссы и обновление порядка
    //exponent += round_mantissa(&mantissa);

    set_bigfloat(v, mantissa, exponent);

    return EXIT_SUCCESS;
}

void
set_bigfloat(bigfloat_t *z,
             longint_t *new_mantissa,
             sdigit new_exponent)
{
    z->mantissa = new_mantissa;
    z->exponent = new_exponent;
}

