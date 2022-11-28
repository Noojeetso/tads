#include "menu.h"
#include "print.h"

void
print_menu()
{
    puts("Главное меню:");
    puts("0. Выход");
    puts("1. Вывод эффективности");
    puts("2. Вывести убывающие серии последовательности целых чисел в обратном порядке\n");
    puts("Методы стека на основе массива");
    puts("3. Вывести состояние стека");
    puts("4. Добавить элемент в стек");
    puts("5. Удалить элемент из стека\n");
    puts("Методы стека на основе списка");
    puts("6. Вывести состояние стека");
    puts("7. Добавить элемент в стек");
    puts("8. Удалить элемент из стека");
    puts("9. Вывести массив освободившихся адресов\n\n");
}

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

    printf("Введите число от %d до %d: ", min, max);
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

int
menu_loop(array_stack_t *array_stack, list_stack_t * list_stack, adress_array_t *adress_array)
{
    int rc;
    int key = -1;
    int value;

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
            case 1:
                break;
            case 2:
                array_stack_print_reversed_task();
                break;
            case 3:
                array_stack_print(array_stack);
                break;
            case 4:
                rc = scan_integer(&value, INT_MIN, INT_MAX);
                if (rc != EXIT_SUCCESS)
                    break;
                puts("Вставка нового значения в стек...");
                rc = array_stack->push(array_stack, value);
                if (rc != EXIT_SUCCESS)
                    break;
                puts("Успешно вставлено новое значение в стек");
                add_tail_adress(adress_array, list_stack);
                break;
            case 5:
                puts("Удаление элемента из стека...");
                rc = array_stack->pop(array_stack, &value);
                if (rc != EXIT_SUCCESS)
                    break;
                printf("Уcпешно удалён элемент: %d\n", value);
                break;
            case 6:
                list_stack_print(list_stack);
                break;
            case 7:
                rc = scan_integer(&value, INT_MIN, INT_MAX);
                if (rc != EXIT_SUCCESS)
                    break;
                puts("Вставка нового значения в стек...");
                rc = list_stack->push(list_stack, value);
                if (rc != EXIT_SUCCESS)
                    break;
                puts("Успешно вставлено новое значение в стек");
                remove_tail_adress(adress_array, list_stack);
                break;
            case 8:
                add_tail_adress(adress_array, list_stack);
                puts("Удаление элемента из стека...");
                rc = list_stack->pop(list_stack, &value);
                if (rc != EXIT_SUCCESS)
                    break;
                printf("Успешно удалён элемент: %d\n", value);
                break;
            case 9:
                print_adress_array(adress_array);
                break;
            default:
                fprintf(stderr, "Ожидалась цифра в пределах от %d до %d\n\n", MIN_MENU_KEY, MAX_MENU_KEY);
        }
        print_newline();
    }
    while (key);

    return EXIT_SUCCESS;
}
