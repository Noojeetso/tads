#include "scan.h"

int
flush_input(void)
{
    int amount = 0;
    char ch;
    do
    {
        scanf("%c", &ch);
        if (ch == EOF)
            break;
        amount++;
    }
    while (ch != '\n');

    return amount;
}

int
scan_menu(int *key)
{
    int rc;

    printf("%s", "Номер меню: ");

    rc = scanf("%d", key);

    if (flush_input() != 1)
    {
        print_newline();
        fputs("Ошибка при считывании номера меню\n", stderr);
        return EXIT_FAILURE;
    }

    if (rc != 1)
    {
        print_newline();
        fputs("Не был введён номер меню\n", stderr);
        return ERR_INPUT_NUMBER;
    }

    return EXIT_SUCCESS;
}

int
scan_double(double *value,
            double min_value,
            double max_value)
{
    printf("Введите число от %.1lf до %.1lf включительно:\n", min_value, max_value);

    if (scanf("%lf", value) != 1 || flush_input() != 1)
    {
        print_newline();
        fputs("Ошибка при вводе целого числа\n", stderr);
        return ERR_INPUT_NUMBER;
    }

    if (*value < min_value)
    {
        print_newline();
        fputs("Введённое число меньше, чем минимально допустимое\n", stderr);
        return ERR_DOMAIN;
    }

    if (*value > max_value)
    {
        print_newline();
        fputs("Введённое число больше, чем максимально допустимое\n", stderr);
        return ERR_DOMAIN;
    }

    return EXIT_SUCCESS;
}

int
scan_int(int *value,
         int min_value,
         int max_value)
{
    printf("Введите целое число от %d до %d включительно:\n", min_value, max_value);

    if (scanf("%d", value) != 1 || flush_input() != 1)
    {
        print_newline();
        fputs("Ошибка при вводе целого числа\n", stderr);
        return ERR_INPUT_NUMBER;
    }

    if (*value < min_value)
    {
        print_newline();
        fputs("Введённое число меньше, чем минимально допустимое\n", stderr);
        return ERR_DOMAIN;
    }

    if (*value > max_value)
    {
        print_newline();
        fputs("Введённое число больше, чем максимально допустимое\n", stderr);
        return ERR_DOMAIN;
    }

    return EXIT_SUCCESS;
}
