#include "table.h"
#include "sort.h"

void
print_menu()
{
    puts("Главное меню:");
    puts("0. Выход");
    puts("1. Загрузить список театров из файла в таблицу");
    puts("2. Добавить запись в конец таблицы");
    puts("3. Вывести таблицу");
    puts("4. Удалить запись по номеру");
    puts("5. Вывести список всех музыкальных спектаклей для детей указанного возраста с продолжительностью меньше указанной");
    puts("6. Вывести массив ключей, отсортированный по названию театра");
    puts("7. Вывести таблицу, отсортированную по названию театра");
    puts("8. Вывести таблицу, отсортированную по названию театра, используя массив ключей");
    puts("9. Сравнить эффективность двух алгоритмов сортировки с использованием массива ключей и без\n");
}

int
menu_loop(record_table_t *main_record_table)
{
    int rc;
    char str[4];
    int key;
    print_arguments_t print_arguments;
    print_arguments.record_table = main_record_table;
    print_arguments.current_char = '\0';

    do
    {
        print_menu();

        printf("%s", "Номер меню: ");
        if (fgets(str, 4, stdin) == NULL || strlen(str) > 2 || str[1] != '\n')
        {
            print_newline();
            fputs("Ошибка считывания номера меню\n\n", stderr);
            continue;
        }
        print_newline();

        key = str[0] - '0';

        switch (key)
        {
            case 0:
                break;
            case 1:
                puts("Перенос записей из файла в таблицу...");
                clear_table(main_record_table);
                rc = add_records_from_file(main_record_table, DATABASE_FILE_NAME);
                if (rc == EXIT_SUCCESS)
                    puts("Записи из файла успешно перенесены в таблицу");
                print_newline();
                break;
            case 2:
                add_record_from_console(main_record_table);
                print_newline();
                break;
            case 3:
                if (check_table_empty(main_record_table) != EXIT_SUCCESS)
                    break;
                print_arguments.print_function = print_table;
                print_horizontal_scroll(&print_arguments);
                break;
            case 4:
                if (check_table_empty(main_record_table) != EXIT_SUCCESS)
                    break;
                delete_by_position(main_record_table);
                print_newline();
                break;
            case 5:
                if (check_table_empty(main_record_table) != EXIT_SUCCESS)
                    break;
                print_arguments.print_function = print_table_keys;
                print_musicals_filtered(&print_arguments);
                break;
            case 6:
                if (check_table_empty(main_record_table) != EXIT_SUCCESS)
                    break;
                qsort(main_record_table->keys, main_record_table->size, sizeof(key_t), compare_keys_by_theater_name);
                print_arguments.print_function = print_table_keys;
                print_horizontal_scroll(&print_arguments);
                break;
            case 7:
                if (check_table_empty(main_record_table) != EXIT_SUCCESS)
                    break;
                qsort(main_record_table->records, main_record_table->size, sizeof(record_t), compare_records_by_theater_name);
                print_arguments.print_function = print_table;
                print_horizontal_scroll(&print_arguments);
                break;
            case 8:
                if (check_table_empty(main_record_table) != EXIT_SUCCESS)
                    break;
                qsort(main_record_table->keys, main_record_table->size, sizeof(key_t), compare_keys_by_theater_name);
                print_arguments.print_function = print_table_with_keys;
                print_horizontal_scroll(&print_arguments);
                break;
            case 9:
                compare_efficiency();
                print_newline();
                break;
            default:
                fprintf(stderr, "Ожидалась цифра в пределах от %d до %d\n\n", MIN_MENU_KEY, MAX_MENU_KEY);
        }
    }
    while (key);

    return EXIT_SUCCESS;
}

int
main(void)
{
    int rc = 0;

    record_table_t main_record_table;
    rc = init_record_table(&main_record_table, MAIN_TABLE_SIZE);
    if (rc != EXIT_SUCCESS)
        return rc;

    rc = menu_loop(&main_record_table);
    free_table(&main_record_table);

    return rc;
}

