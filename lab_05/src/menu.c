#include "menu.h"
#include "print.h"

void
print_menu()
{
    puts("Главное меню:");
    puts("0. Выход");
    puts("Функции для очереди в виде массива");
    puts("1. Добавить элемент в очередь");
    puts("2. Удалить элемент из очереди");
    puts("3. Отобразить очередь на экран");
    puts("Функции для очереди в виде списка");
    puts("4. Добавить элемент в очередь");
    puts("5. Удалить элемент из очереди");
    puts("6. Отобразить очередь на экран");
    puts("7. Вывести массив освободившихся адресов");
    puts("Функции для моделирования и вычисления эффективности");
    puts("8. Смоделировать работу ОА с очередью на основе массива");
    puts("9. Смоделировать работу ОА с очередью на основе списка");
    puts("10. Сравнить эффективность работы очередей\n\n");
}

int
menu_loop(void)
{
    int rc;
    int key = -1;
    int value;
    list_queue_t *list_queue;
    array_queue_t *array_queue;
    freed_nodes_t freed_nodes = {.size = 0};

    list_queue = new_list_queue();
    if (list_queue == NULL)
        return ERR_NO_MEMORY;

    array_queue = new_array_queue(INITIAL_SIZE);
    if (array_queue == NULL)
    {
        free_list_queue(list_queue);
        return ERR_NO_MEMORY;
    }

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
                printf("Введите число от %d до %d: ", MIN_DEMO_VALUE, MAX_DEMO_VALUE);
                rc = scan_integer(&value, MIN_DEMO_VALUE, MAX_DEMO_VALUE);
                if (rc != EXIT_SUCCESS)
                    break;
                rc = array_queue_push(array_queue, value);
                if (rc != EXIT_SUCCESS)
                    break;
                puts("Значение успешно вставлено");
                break;
            case 2:
                rc = array_queue_pop(array_queue, &value);
                if (rc != EXIT_SUCCESS)
                    break;
                printf("Удалено значение: %d\n", value);
                break;
            case 3:
                array_queue_print(array_queue);
                break;
            case 4:
                printf("Введите число от %d до %d: ", MIN_DEMO_VALUE, MAX_DEMO_VALUE);
                rc = scan_integer(&value, MIN_DEMO_VALUE, MAX_DEMO_VALUE);
                if (rc != EXIT_SUCCESS)
                    break;
                rc = list_queue_push_address(list_queue, value, &freed_nodes);
                if (rc != EXIT_SUCCESS)
                    break;
                puts("Значение успешно вставлено");
                break;
            case 5:
                rc = list_queue_pop_address(list_queue, &value, &freed_nodes);
                if (rc != EXIT_SUCCESS)
                    break;
                printf("Удалено значение: %d\n", value);
                break;
            case 6:
                list_queue_print(list_queue);
                break;
            case 7:
                freed_nodes_print(&freed_nodes);
                break;
            case 8:
                simulate_array_queue();
                break;
            case 9:
                simulate_list_queue();
                break;
            case 10:
                compare_efficiency();
                break;
            default:
                fprintf(stderr, "Ожидалась цифра в пределах от %d до %d\n\n", MIN_MENU_KEY, MAX_MENU_KEY);
        }
        print_newline();
    }
    while (key);

    free_list_queue(list_queue);
    free_array_queue(array_queue);

    return EXIT_SUCCESS;
}
