#include "menu.h"
#include "print.h"

void
print_menu()
{
    puts("Главное меню:");
    puts("0. Выход");
    puts("1. Вывод эффективности\n");
    puts("Методы стека на основе массива");
    puts("2. Вывести состояние стека");
    puts("3. Добавить элемент в стек");
    puts("4. Удалить элемент из стека");
    puts("5. Вывести убывающие серии последовательности целых чисел в обратном порядке\n");
    puts("Методы стека на основе списка");
    puts("6. Вывести состояние стека");
    puts("7. Добавить элемент в стек");
    puts("8. Удалить элемент из стека");
    puts("9. Вывести массив освободившихся адресов");
    puts("10. Вывести убывающие серии последовательности целых чисел в обратном порядке\n\n");
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
menu_loop(array_stack_t *array_stack,
          list_stack_t * list_stack,
          adress_array_t *adress_array)
{
    int rc;
    int key = -1;
    int value;
    int *in_array;
    int *out_array;
    int array_size;

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
                print_results();
                break;
            case 2:
                array_stack_print(array_stack);
                break;
            case 3:
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
            case 4:
                puts("Удаление элемента из стека...");
                rc = array_stack->pop(array_stack, &value);
                if (rc != EXIT_SUCCESS)
                    break;
                printf("Уcпешно удалён элемент: %d\n", value);
                break;
            case 5:
                printf("Введите последовательность чисел (от %d до %d), каждое в новой строке.\n", INT_MIN, INT_MAX);
                puts("По окончании ввода нажмите ENTER.");
                in_array = scan_array(&array_size);
                out_array = get_array_stack_reversed_sequence(in_array, array_size);
                free(in_array);
                puts("Полученная строка:");
                for (size_t i = 0; i < array_size; i++)
                    printf("%d%c", out_array[i], i == array_size - 1 ? '\n' : ' ');
                free(out_array);
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
            case 10:
                printf("Введите последовательность чисел (от %d до %d), каждое в новой строке.\n", INT_MIN, INT_MAX);
                puts("По окончании ввода нажмите ENTER.");
                in_array = scan_array(&array_size);
                out_array = get_list_stack_reversed_sequence(in_array, array_size);
                free(in_array);
                puts("Полученная строка:");
                for (size_t i = 0; i < array_size; i++)
                    printf("%d%c", out_array[i], i == array_size - 1 ? '\n' : ' ');
                free(out_array);
                break;
            default:
                fprintf(stderr, "Ожидалась цифра в пределах от %d до %d\n\n", MIN_MENU_KEY, MAX_MENU_KEY);
        }
        print_newline();
    }
    while (key);

    return EXIT_SUCCESS;
}
