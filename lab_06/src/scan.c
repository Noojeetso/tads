#include "scan.h"

int
scan_int(int *value,
         int min_value,
         int max_value)
{
    printf("Введите целое число от %d до %d включительно\n", min_value, max_value);

    if (scanf("%d", value) != 1)
    {
        fputs("Ошибка ввода целого числа", stderr);
        return ERR_INPUT_NUMBER;
    }

    if (*value < min_value)
    {
        fputs("Введённое число меньше, чем минимально допустимое", stderr);
        return ERR_DOMAIN;
    }

    if (*value > max_value)
    {
        fputs("Введённое число больше, чем максимально допустимое", stderr);
        return ERR_DOMAIN;
    }

    return EXIT_SUCCESS;
}
