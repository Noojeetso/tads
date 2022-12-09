#include "menu.h"

void
print_menu()
{
    puts(MENU_TITLE);
    puts(MENU_ITEM_0);
    puts(MENU_ITEM_1);
    puts(MENU_ITEM_2);
    puts(MENU_ITEM_3);
    puts(MENU_ITEM_4);
    puts(MENU_ITEM_5);
    puts(MENU_ITEM_6);
    puts(MENU_ITEM_7);
    puts(MENU_ITEM_8);
}

int
menu_loop()
{
    int rc;
    int key;
    int value;
    double average_compares;
    tree_node_t *bst = NULL;
    tree_node_t *avl = NULL;
    FILE *file;
    array_t *array = file_to_array(DATABASE_FILENAME);
    hashtable_opened_t *hashtable_opened = new_hashtable_opened(array->size * HASH_OPENED_COEFFICIENT);
    hashtable_closed_t *hashtable_closed = new_hashtable_closed(array->size * HASH_CLOSED_COEFFICIENT);
    hashtable_opened_t *tmp_table_opened;
    hashtable_closed_t *tmp_table_closed;
    free_array(array);

    do
    {
        print_menu();

        rc = scan_menu(&key);
        print_newline();
        if (rc != EXIT_SUCCESS)
            continue;

        switch (key)
        {
            case 0:
                break;
            case 1:
                puts("Очистка АВЛ дерева...");
                free_tree(&avl);
                puts("Очистка ДДП...");
                free_tree(&bst);
                puts("Очистка открытой хэш-таблицы...");
                free_table_opened(hashtable_opened);
                puts("Очистка закрытой хэш-таблицы...");
                free_table_closed(hashtable_closed);

                array_t *array = file_to_array(DATABASE_FILENAME);
                hashtable_opened = new_hashtable_opened(array->size * HASH_OPENED_COEFFICIENT);
                hashtable_closed = new_hashtable_closed(array->size * HASH_CLOSED_COEFFICIENT);
                free_array(array);

                print_newline();
                puts("Открытие файла на чтение...");

                file = fopen(DATABASE_FILENAME, READ_MODE);
                if (file == NULL)
                {
                    fputs("Ошибка открытия файла на чтение\n", stderr);
                    return ERR_OPEN_FILE;
                }

                puts("Запись в АВЛ дерево...");

                rc = avl_fill(file, &avl);
                if (rc != EXIT_SUCCESS)
                {
                    printf("err: %d\n", rc);
                    puts("Закрытие файла...");
                    fclose(file);
                    return rc;
                }

                fseek(file, 0, SEEK_SET);

                puts("Запись в ДДП...");

                rc = bst_fill(file, &bst);
                if (rc != EXIT_SUCCESS)
                {
                    puts("Закрытие файла...");
                    fclose(file);
                    break;
                }

                fseek(file, 0, SEEK_SET);

                puts("Запись в открытую хэш-таблицу...");

                rc = fill_hashtable_opened(file, hashtable_opened);
                if (rc != EXIT_SUCCESS)
                {
                    puts("Закрытие файла...");
                    fclose(file);
                    break;
                }

                fseek(file, 0, SEEK_SET);

                puts("Запись в закрытую хэш-таблицу...");

                rc = fill_hashtable_closed(file, hashtable_closed);
                if (rc != EXIT_SUCCESS)
                {
                    puts("Закрытие файла...");
                    fclose(file);
                    break;
                }

                puts("Закрытие файла...");
                fclose(file);

                break;
            case 2:
                tree_visualize(bst, "bst");

                break;
            case 3:
                tree_visualize(avl, "avl");

                break;
            case 4:
                print_newline();
                print_hashtable_opened(hashtable_opened);
                print_newline();
                print_hashtable_closed(hashtable_closed);

                break;
            case 5:
                rc = scan_int(&value, INT_MIN, INT_MAX);
                if (rc != EXIT_SUCCESS)
                    break;

                print_newline();
                puts("Вставка элемента в АВЛ дерево...");
                rc = avl_push(&avl, value);
                if (rc == EXIT_SUCCESS)
                    puts("Успешно");

                print_newline();
                puts("Вставка элемента в ДДП...");
                rc = bst_push(&bst, value);
                if (rc == EXIT_SUCCESS)
                    puts("Успешно");

                print_newline();
                puts("Вставка элемента в открытую хэш-таблицу...");
                rc = push_table_opened(hashtable_opened, value);
                if (rc == EXIT_SUCCESS)
                    puts("Успешно");

                print_newline();
                puts("Вставка элемента в закрытую хэш-таблицу...");
                rc = push_table_closed(hashtable_closed, value);
                if (rc == EXIT_SUCCESS)
                    puts("Успешно");

                print_newline();
                puts("Вставка элемента в файл...");
                rc = file_push(DATABASE_FILENAME, value);
                if (rc == EXIT_SUCCESS)
                    puts("Успешно");

                break;
            case 6:
                rc = scan_int(&value, INT_MIN, INT_MAX);
                if (rc != EXIT_SUCCESS)
                    break;

                print_newline();
                puts("Удаление элемента из АВЛ дерева...");
                rc = avl_pop(&avl, value);
                if (rc == EXIT_SUCCESS)
                    puts("Успешно");

                print_newline();
                puts("Удаление элемента из ДДП...");
                rc = bst_pop(&bst, value);
                if (rc == EXIT_SUCCESS)
                    puts("Успешно");

                print_newline();
                puts("Удаление элемента из открытой хэш-таблицы...");
                rc = hashtable_opened_pop(hashtable_opened, value);
                if (rc == EXIT_SUCCESS)
                    puts("Успешно");

                print_newline();
                puts("Удаление элемента из закрытой хэш-таблицы...");
                rc = hashtable_closed_pop(hashtable_closed, value);
                if (rc == EXIT_SUCCESS)
                    puts("Успешно");

                print_newline();
                puts("Удаление элемента из файла...");
                rc = file_pop(DATABASE_FILENAME, value);
                if (rc == EXIT_SUCCESS)
                    puts("Успешно");

                break;
            case 7:
                puts("Ввод числа для поиска");
                rc = scan_int(&value, INT_MIN, INT_MAX);
                if (rc != EXIT_SUCCESS)
                    break;
                print_newline();

                puts("Ввод максимального среднего количества сравнений");
                rc = scan_double(&average_compares, MIN_COMPARES, MAX_COMPARES);
                if (rc != EXIT_SUCCESS)
                    break;
                print_newline();

                puts("Поиск элемента в файле...");
                set_file_comparisons(0);
                array = file_to_array(DATABASE_FILENAME);
                file_search(DATABASE_FILENAME, value);
                printf("Количество сравнений: %lu\n", get_file_comparisons());
                free_array(array);
                print_newline();

                puts("Поиск элемента в АВЛ дереве...");
                set_tree_comparisons(0);
                tree_search(avl, value);
                printf("Количество сравнений: %lu\n", get_tree_comparisons());
                print_newline();

                puts("Поиск элемента в двоичном дереве поиска...");
                set_tree_comparisons(0);
                tree_search(bst, value);
                printf("Количество сравнений: %lu\n", get_tree_comparisons());
                print_newline();

                puts("Поиск элемента в открытой хэш-таблице...");
                set_hashtable_comparisons(0);
                hashtable_opened_search(hashtable_opened, value);
                printf("Количество сравнений: %lu\n", get_hashtable_comparisons());
                print_newline();

                if (get_hashtable_comparisons() > average_compares)
                {
                    file = fopen(DATABASE_FILENAME, READ_MODE);
                    if (file == NULL)
                    {
                        fputs("Ошибка открытия файла на чтение\n", stderr);
                        return ERR_OPEN_FILE;
                    }
                    tmp_table_opened = new_hashtable_opened(get_hashtable_opened_size(hashtable_opened) * EXPAND_FACTOR);
                    fill_hashtable_opened(file, tmp_table_opened);
                    free_table_opened(hashtable_opened);
                    hashtable_opened = tmp_table_opened;
                    fclose(file);
                    puts("Произошла реструктуризация открытой хэш-таблицы");

                    set_hashtable_comparisons(0);
                    hashtable_opened_find(hashtable_opened, value);
                    printf("Количество сравнений в реструктуризированной хэш-таблице: %lu\n", get_hashtable_comparisons());
                    print_newline();
                }

                puts("Поиск элемента в закрытой хэш-таблице...");
                set_hashtable_comparisons(0);
                hashtable_closed_search(hashtable_closed, value);
                printf("Количество сравнений: %lu\n", get_hashtable_comparisons());
                print_newline();

                if (get_hashtable_comparisons() > average_compares)
                {
                    file = fopen(DATABASE_FILENAME, READ_MODE);
                    if (file == NULL)
                    {
                        fputs("Ошибка открытия файла на чтение\n", stderr);
                        return ERR_OPEN_FILE;
                    }
                    tmp_table_closed = new_hashtable_closed(get_hashtable_closed_size(hashtable_closed) * EXPAND_FACTOR);
                    free_table_closed(hashtable_closed);
                    fill_hashtable_closed(file, tmp_table_closed);
                    hashtable_closed = tmp_table_closed;
                    fclose(file);
                    puts("Произошла реструктуризация закрытой хэш-таблицы");

                    set_hashtable_comparisons(0);
                    hashtable_closed_find(hashtable_closed, value);
                    printf("Количество сравнений в реструктуризированной хэш-таблице: %lu\n", get_hashtable_comparisons());
                    print_newline();
                }

                break;
            case 8:
                print_efficiency();
                break;
            default:
                fprintf(stderr, "Ожидалась цифра в пределах от %d до %d\n", MIN_MENU_KEY, MAX_MENU_KEY);
        }
        print_newline();
    }
    while (key);

    free_tree(&avl);
    free_tree(&bst);
    free_table_opened(hashtable_opened);
    free_table_closed(hashtable_closed);

    return EXIT_SUCCESS;
}
