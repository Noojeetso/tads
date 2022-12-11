#include "scan.h"

int
flush_input()
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
scan_integer(int *value,
             int min,
             int max)
{
    int rc;

    rc = scanf("%d", value);
    if (rc != 1)
    {
        flush_input();
        print_newline();
        fputs("Ошибка считывания целого числа\n\n", stderr);
        return ERR_INPUT_NUMBER;
    }
    if (flush_input() != 1)
    {
        print_newline();
        fputs("Необходимо ввести целое число\n\n", stderr);
        return EXIT_FAILURE;
    }

    if (*value < min || *value > max)
    {
        print_newline();
        fputs("Число выходит за допустимые пределы\n\n", stderr);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
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
        fputs("Ошибка считывания номера меню\n\n", stderr);
        return EXIT_FAILURE;
    }

    if (rc != 1)
    {
        print_newline();
        fputs("Не был введён номер меню\n\n", stderr);
        return ERR_INPUT_NUMBER;
    }

    print_newline();

    return EXIT_SUCCESS;
}
