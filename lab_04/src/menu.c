#include "stack.h"
#include "print.h"

void
print_menu()
{
    puts("Главное меню:");
    puts("0. Выход\n");
    puts("Методы стека на основе массива");
    puts("1. Вывести состояние стека");
    puts("2. Добавить элемент в стек");
    puts("3. Удалить элемент из стека");
    puts("4. Запись последовательности в стек");
    puts("5. Вывести массив освободившихся адресов");
    puts("6. Вывести убывающие серии последовательности целых чисел в обратном порядке\n");
    puts("Методы стека на основе списка");
    puts("7. Вывести состояние стека");
    puts("8. Добавить элемент в стек");
    puts("9. Удалить элемент из стека");
    puts("10. Запись последовательности в стек");
    puts("11. Вывести убывающие серии последовательности целых чисел в обратном порядке\n\n");
}

int
flush_input()
{
    int amount = 0;
    int ch;
    do
    {
        ch = fgetc(stdin);
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
    int ch;

    printf("%s", "Номер меню: ");

    ch = fgetc(stdin);

    if (ch == '\n')
    {
        print_newline();
        fputs("Не был введён номер меню\n\n", stderr);
        return ERR_INPUT_NUMBER;
    }

    if (flush_input() != 1)
    {
        print_newline();
        fputs("Ошибка считывания номера меню\n\n", stderr);
        return EXIT_FAILURE;
    }
    print_newline();

    *key = ch - '0';

    return EXIT_SUCCESS;
}

int
menu_loop(array_stack_t *array_stack, list_stack_t * list_stack)
{
    int rc;
    int key;

    do
    {
        print_menu();

        rc = scan_menu(&key);
        if (rc != EXIT_SUCCESS)
            continue;

        switch (key)
        {
            case 0:
                break;
            default:
                fprintf(stderr, "Ожидалась цифра в пределах от %d до %d\n\n", MIN_MENU_KEY, MAX_MENU_KEY);
        }
    }
    while (key);

    return EXIT_SUCCESS;
}

