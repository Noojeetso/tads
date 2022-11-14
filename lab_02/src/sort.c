#include "sort.h"

int
compare_records_by_theater_name(const void *void_record_1,
                                const void *void_record_2)
{
    const record_t *record_1 = void_record_1;
    const record_t *record_2 = void_record_2;

    const char *name_1 = record_1->theater_name;
    if (record_1->theater_name_long != NULL)
        name_1 = record_1->theater_name_long;

    const char *name_2 = record_2->theater_name;
    if (record_2->theater_name_long != NULL)
        name_2 = record_2->theater_name_long;

    return strcmp(name_1, name_2);
}

int
compare_keys_by_theater_name(const void *void_key_1,
                             const void *void_key_2)
{
    const key_t *key_1 = void_key_1;
    const key_t *key_2 = void_key_2;

    const char *name_1 = key_1->theater_name;
    if (key_1->theater_name_long != NULL)
        name_1 = key_1->theater_name_long;

    const char *name_2 = key_2->theater_name;
    if (key_2->theater_name_long != NULL)
        name_2 = key_2->theater_name_long;

    return strcmp(name_1, name_2);
}

void
bubble_sort(void *base,
            size_t num,
            size_t size,
            int (*compare)(const void*, const void*))
{
    size_t type_size;
    char tmp;
    for (char *i = base; i < (char*)base + num * size; i += size)
        for (char *j = base; j < (char*)base + (num - 1) * size - (i - (char*)base); j += size)
            if (compare(j, j + size) > 0)
            {
                type_size = size;
                char *a = j;
                char *b = j + size;
                while (type_size--)
                {
                    tmp = *a;
                    *(a++) = *b;
                    *(b++) = tmp;
                }
            }
}

void
print_efficiency(size_t *first_measurements,
                 size_t *second_measurements,
                 size_t *amounts,
                 size_t size,
                 char *main_title,
                 char *first_title,
                 char *second_title,
                 char *relative_title)
{
    printf("%s\n", main_title);
    unicode_format_print("Количество элементов", 24, 0);
    unicode_format_print(first_title, multibyte_strlen(first_title) + 2, 0);
    unicode_format_print(second_title, multibyte_strlen(second_title) + 2, 0);
    unicode_format_print(relative_title, multibyte_strlen(relative_title) + 2, 0);
    print_newline();

    for (size_t i = 0; i < size; i++)
    {
        print_integer_centered(amounts[i], 24, ' ');
        print_integer_centered(first_measurements[i], multibyte_strlen(first_title) + 2, ' ');
        print_integer_centered(second_measurements[i], multibyte_strlen(second_title) + 2, ' ');
        print_integer_centered(100 * first_measurements[i] / second_measurements[i], multibyte_strlen(relative_title) + 2, '%');
        print_newline();
    }
}

int
compare_efficiency()
{
    int rc;
    clock_t start_clock;
    clock_t end_clock;
    long long unsigned cpu_time_used_total;

    char *file_names[] = {"database/in_10.txt", "database/in_50.txt", "database/in_100.txt",
                          "database/in_500.txt", "database/in_1000.txt", "database/in_5000.txt"};
    size_t sizes[SORT_AMOUNT] = {10, 50, 100,
                                 500, 1000, 5000};

    size_t speed_bubble[SORT_AMOUNT];
    size_t speed_bubble_with_key[SORT_AMOUNT];
    size_t speed_quick[SORT_AMOUNT];
    size_t speed_quick_with_key[SORT_AMOUNT];
    size_t records_size_bytes[SORT_AMOUNT];
    size_t keys_size_bytes[SORT_AMOUNT];

    record_table_t record_table;

    rc = init_record_table(&record_table, MAX_SORT_TABLE_SIZE);
    if (rc != EXIT_SUCCESS)
        return rc;

    puts("Вычисление времени работы быстрой сортировки с ключами");

    for (int size_index = 0; size_index < sizeof(sizes) / sizeof(sizes[0]); size_index++)
    {
        cpu_time_used_total = 0;
        for (long i = 0; i < SORT_ITERATIONS; i++)
        {
            clear_table(&record_table);
            rc = add_records_from_file(&record_table, file_names[size_index]);
            if (rc != EXIT_SUCCESS)
                return rc;
            start_clock = clock();
            qsort(record_table.keys, record_table.size, sizeof(key_t), compare_keys_by_theater_name);
            end_clock = clock();
            cpu_time_used_total += (end_clock - start_clock) / (CLOCKS_PER_SEC / 1000000LLU);
        }
        speed_quick_with_key[size_index] = cpu_time_used_total / SORT_ITERATIONS;
        if (speed_quick_with_key[size_index] == 0)
            speed_quick_with_key[size_index] = 1;
    }

    puts("Выполнено");

    puts("Вычисление времени работы быстрой сортировки без ключей");

    for (int size_index = 0; size_index < sizeof(sizes) / sizeof(sizes[0]); size_index++)
    {
        cpu_time_used_total = 0;
        for (long i = 0; i < SORT_ITERATIONS; i++)
        {
            clear_table(&record_table);
            add_records_from_file(&record_table, file_names[size_index]);
            if (rc != EXIT_SUCCESS)
                return rc;
            start_clock = clock();
            qsort(record_table.records, record_table.size, sizeof(record_t), compare_records_by_theater_name);
            end_clock = clock();
            cpu_time_used_total += (end_clock - start_clock) / (CLOCKS_PER_SEC / 1000000LLU);
        }
        speed_quick[size_index] = cpu_time_used_total / SORT_ITERATIONS;
        if (speed_quick[size_index] == 0)
            speed_quick[size_index] = 1;
    }

    puts("Выполнено");

    puts("Вычисление времени работы сортировки пузырьком с ключами");

    for (int size_index = 0; size_index < sizeof(sizes) / sizeof(sizes[0]); size_index++)
    {
        cpu_time_used_total = 0;
        for (long i = 0; i < SORT_ITERATIONS; i++)
        {
            clear_table(&record_table);
            add_records_from_file(&record_table, file_names[size_index]);
            if (rc != EXIT_SUCCESS)
                return rc;
            start_clock = clock();
            bubble_sort(record_table.keys, record_table.size, sizeof(key_t), compare_keys_by_theater_name);
            end_clock = clock();
            cpu_time_used_total += (end_clock - start_clock) / (CLOCKS_PER_SEC / 1000000LLU);
        }
        speed_bubble_with_key[size_index] = cpu_time_used_total / SORT_ITERATIONS;
        if (speed_bubble_with_key[size_index] == 0)
            speed_bubble_with_key[size_index] = 1;
    }

    puts("Выполнено");

    puts("Вычисление времени работы сортировки пузырьком без ключей");

    for (int size_index = 0; size_index < sizeof(sizes) / sizeof(sizes[0]); size_index++)
    {
        cpu_time_used_total = 0;
        for (long i = 0; i < SORT_ITERATIONS; i++)
        {
            clear_table(&record_table);
            add_records_from_file(&record_table, file_names[size_index]);
            if (rc != EXIT_SUCCESS)
                return rc;
            start_clock = clock();
            bubble_sort(record_table.records, record_table.size, sizeof(record_t), compare_records_by_theater_name);
            end_clock = clock();
            cpu_time_used_total += (end_clock - start_clock) / (CLOCKS_PER_SEC / 1000000LLU);
        }
        speed_bubble[size_index] = cpu_time_used_total / SORT_ITERATIONS;
        if (speed_bubble[size_index] == 0)
            speed_bubble[size_index] = 1;
    }

    puts("Выполнено");

    puts("Вычисление занимаемой памяти");

    char *long_str;
    for (int size_index = 0; size_index < sizeof(sizes) / sizeof(sizes[0]); size_index++)
    {
        clear_table(&record_table);
        add_records_from_file(&record_table, file_names[size_index]);
        if (rc != EXIT_SUCCESS)
            return rc;

        records_size_bytes[size_index] = sizeof(record_t) * sizes[size_index];
        for (size_t i = 0; i < record_table.size; i++)
        {
            long_str = record_table.records[i].theater_name_long;
            if (long_str != NULL)
                records_size_bytes[size_index] += strlen(long_str);
            long_str = record_table.records[i].play_name_long;
            if (long_str != NULL)
                records_size_bytes[size_index] += strlen(long_str);
            long_str = record_table.records[i].producer_long;
            if (long_str != NULL)
                records_size_bytes[size_index] += strlen(long_str);
            if (record_table.records[i].show_kind == MUSICAL)
            {
                long_str = record_table.records[i].play_group.musical.composer_long;
                if (long_str != NULL)
                    records_size_bytes[size_index] += strlen(long_str);
                long_str = record_table.records[i].play_group.musical.country_long;
                if (long_str != NULL)
                    records_size_bytes[size_index] += strlen(long_str);
            }
        }

        keys_size_bytes[size_index] = sizeof(key_t) * sizes[size_index];
    }

    free_table(&record_table);

    puts("Выполнено");
    print_newline();

    print_efficiency(speed_bubble,
                     speed_bubble_with_key,
                     sizes,
                     sizeof(sizes) / sizeof(sizes[0]),
                     "Пузырьковая сортировка",
                     "Время без использования ключей в микросекундах",
                     "Время с использованием ключей в микросекундах",
                     "Отношение времени работы с ключами к времени работы без ключей");
    print_newline();
    print_efficiency(speed_quick,
                     speed_quick_with_key,
                     sizes,
                     sizeof(sizes) / sizeof(sizes[0]),
                     "Быстрая сортировка",
                     "Скорость без использования ключей в микросекундах",
                     "Скорость с использованием ключей в микросекундах",
                     "Отношение скорости работы с ключами к скорости работы без ключей");
    print_newline();
    print_efficiency(keys_size_bytes,
                     records_size_bytes,
                     sizes,
                     sizeof(sizes) / sizeof(sizes[0]),
                     "Занимаемая память",
                     "Размер массива ключей в байтах",
                     "Размер таблицы записей в байтах",
                     "Отношение размера массива ключей к размеру таблицы");

    return EXIT_SUCCESS;
}

