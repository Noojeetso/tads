#include "efficiency.h"

int
compare_int_pointers(const void *a_ptr,
                     const void *b_ptr)
{
    int a = *(int *)a_ptr;
    int b = *(int *)b_ptr;
    if (a == b)
        return 0;
    return a > b ? 1 : -1;
}

void
print_comparisons(results_t *results,
                  int index)
{
    printf("АВЛ дерево:\t\t%.1lf\n", (double)(results->avl[index]) / ITERATIONS);
    printf("Двоичное дерево поиска:\t%.1lf\n", (double)(results->bst[index]) / ITERATIONS);
    printf("Массив на основе файла:\t%.1lf\n", (double)(results->file[index]) / ITERATIONS);
    printf("Открытая хэш-таблица:\t%.1lf\n", (double)(results->hashtable_opened[index]) / ITERATIONS);
    printf("Закрытая хэш-таблица:\t%.1lf\n", (double)(results->hashtable_closed[index]) / ITERATIONS);
    if (results->hashtable_opened_increased_size[index] != 0)
        printf("Открытая хэш-таблица (реструктурированная):\t%.1lf\n", ((double)results->hashtable_opened_increased[index]) / ITERATIONS);
    else
        printf("Открытая хэш-таблица (реструктурированная):\t%c\n", '-');
    if (results->hashtable_closed_increased_size[index])
        printf("Закрытая хэш-таблица (реструктурированная):\t%.1lf\n", ((double)results->hashtable_closed_increased[index]) / ITERATIONS);
    else
        printf("Закрытая хэш-таблица (реструктурированная):\t%c\n", '-');
}

void
print_sizes_in_bytes(results_t *results,
                     int index)
{
    printf("АВЛ дерево:\t\t%lu\n", results->avl_size[index]);
    printf("Двоичное дерево поиска:\t%lu\n", results->bst_size[index]);
    printf("Массив на основе файла:\t%lu\n", results->file_size[index]);
    printf("Открытая хэш-таблица:\t%lu\n", results->hashtable_opened_size[index]);
    printf("Закрытая хэш-таблица:\t%lu\n", results->hashtable_closed_size[index]);
    if (results->hashtable_opened_increased_size[index] != 0)
        printf("Открытая хэш-таблица (реструктурированная):\t%lu\n", results->hashtable_opened_increased_size[index]);
    else
        printf("Открытая хэш-таблица (реструктурированная):\t%c\n", '-');
    if (results->hashtable_closed_increased_size[index])
        printf("Закрытая хэш-таблица (реструктурированная):\t%lu\n", results->hashtable_closed_increased_size[index]);
    else
        printf("Закрытая хэш-таблица (реструктурированная):\t%c\n", '-');
}

void
print_results(results_t *results)
{
    char *file_names[] = {"Маленький файл", "Средний файл",
                          "Большой файл", "Отсортированный файл",
                          "Файл, отсортированный в обратном порядке"};
    print_newline();
    print_newline();
    print_newline();
    puts("\tПоиск (количество сравнений)");
    for (size_t i = 0; i < sizeof(file_names) / sizeof(file_names[0]); i++)
    {
        puts(file_names[i]);
        print_comparisons(results, i);
        print_newline();
    }
    print_newline();
    print_newline();
    print_newline();
    puts("\tРазмер структур (в байтах)");
    for (size_t i = 0; i < sizeof(file_names) / sizeof(file_names[0]); i++)
    {
        puts(file_names[i]);
        print_sizes_in_bytes(results, i);
        print_newline();
    }
    print_newline();
}
/*
void
ignore(tree_node_t *node)
{
    return;
}
*/

void
print_efficiency(void)
{
    /*
    int rc;
    tree_node_t *avl = NULL;
    tree_node_t *bst = NULL;
    tree_node_t *result_node = NULL;
    hashtable_opened_t *hashtable_opened;
    hashtable_closed_t *hashtable_closed;
    FILE *file;
    array_t *array;
    results_t results = {0};
    const char *file_names[] = {SMALL_FILE_NAME, MEDIUM_FILE_NAME,
                                BIG_FILE_NAME, SORTED_FILE_NAME,
                                SORTED_REVERSE_FILE_NAME};
    size_t search_compares;
    size_t elements_amount;
    int values[ITERATIONS];

    // ignore(result_node);

    double max_comps;
    double average_compares;
    */

    /*
    puts("Ввод максимального среднего количества сравнений");
    rc = scan_double(&max_comps, MIN_COMPARES, MAX_COMPARES);
    if (rc != EXIT_SUCCESS)
        return;

    // Генерация чисел для поиска
    for (int i = 0; i < ITERATIONS; i++)
        values[i] = rand();

    print_newline();
    puts("\tКоличество элементов в файлах");
    for (size_t name_id = 0; name_id < 5; name_id++)
    {
        printf("Файл: \"%s\"\n", file_names[name_id]);
        file = fopen(file_names[name_id], READ_MODE);
        for (size_t i = 0; i < ITERATIONS; i++)
        {
            array = file_to_array(file_names[name_id]);
            elements_amount = array->size;
            // Вывод количества элементов в файле
            if (i == 0)
                printf("Элементов в файле: %lu\n", elements_amount);
            // Нормализация числа для поиска
            values[i] %= elements_amount / 2;
            free_array(array);

            fseek(file, 0, SEEK_SET);

            // Подсчёт количества сравнений при поиске элемента в массиве
            array = file_to_array(file_names[name_id]);
            for (search_compares = 0; search_compares < array->size; search_compares++)
                if (array->data[search_compares] == values[i])
                    break;
            results.file[name_id] += search_compares + 1;
            if (i == 0)
                results.file_size[name_id] = get_array_size_bytes(array);
            free_array(array);

            fseek(file, 0, SEEK_SET);

            // Подсчёт количества сравнений при поиске элемента в АВЛ дереве
            avl_fill(file, &avl);
            set_tree_comparisons(0);
            result_node = find_node(avl, values[i]);
            results.avl[name_id] += get_tree_comparisons();
            if (i == 0)
                results.avl_size[name_id] = get_tree_size_bytes(avl);
            free_tree(&avl);

            fseek(file, 0, SEEK_SET);

            // Подсчёт количества сравнений при поиске элемента в ДДП
            bst_fill(file, &bst);
            set_tree_comparisons(0);
            result_node = find_node(bst, values[i]);
            results.bst[name_id] += get_tree_comparisons();
            if (i == 0)
                results.bst_size[name_id] = get_tree_size_bytes(bst);
            free_tree(&bst);

            fseek(file, 0, SEEK_SET);

            // Подсчёт количества сравнений при поиске в открытой хэш-таблице и определение её размера
            hashtable_opened = new_hashtable_opened(elements_amount * HASH_OPENED_COEFFICIENT);
            fill_hashtable_opened(file, hashtable_opened);
            set_hashtable_comparisons(0);
            hashtable_opened_find(hashtable_opened, values[i]);
            results.hashtable_opened[name_id] += get_hashtable_comparisons();
            if (i == 0)
                results.hashtable_opened_size[name_id] = get_hashtable_opened_size_bytes(hashtable_opened);
            free_table_opened(hashtable_opened);

            fseek(file, 0, SEEK_SET);

            // Подсчёт количества сравнений при поиске в закрытой хэш-таблице и определение её размера
            hashtable_closed = new_hashtable_closed(elements_amount * HASH_CLOSED_COEFFICIENT);
            fill_hashtable_closed(file, hashtable_closed);
            set_hashtable_comparisons(0);
            hashtable_closed_find(hashtable_closed, values[i]);
            results.hashtable_closed[name_id] += get_hashtable_comparisons();
            if (i == 0)
                results.hashtable_closed_size[name_id] = get_hashtable_closed_size_bytes(hashtable_closed);
            free_table_closed(hashtable_closed);

        }
        fclose(file);
    }

    for (size_t name_id = 0; name_id < sizeof(file_names) / sizeof(file_names[0]); name_id++)
    {
        file = fopen(file_names[name_id], READ_MODE);
        for (size_t i = 0; i < ITERATIONS; i++)
        {
            array = file_to_array(file_names[name_id]);
            elements_amount = array->size;
            // Нормализация числа для поиска
            values[i] %= elements_amount / 2;
            free_array(array);

            fseek(file, 0, SEEK_SET);

            average_compares = ((double)results.hashtable_opened[name_id]) / ITERATIONS;
            if (average_compares > max_comps)
            {
                // Подсчёт количества сравнений при поиске в закрытой хэш-таблице (реструктуризированной)
                hashtable_opened = new_hashtable_opened(elements_amount * HASH_OPENED_COEFFICIENT * EXPAND_FACTOR);
                fill_hashtable_opened(file, hashtable_opened);
                set_hashtable_comparisons(0);
                hashtable_opened_find(hashtable_opened, values[i]);
                results.hashtable_opened_increased[name_id] += get_hashtable_comparisons();
                if (i == 0)
                    results.hashtable_opened_increased_size[name_id] = get_hashtable_opened_size_bytes(hashtable_opened);
                free_table_opened(hashtable_opened);
            }

            fseek(file, 0, SEEK_SET);

            average_compares = ((double)results.hashtable_closed[name_id]) / ITERATIONS;
            if (average_compares > max_comps)
            {
                // Подсчёт количества сравнений при поиске в открытой хэш-таблице (реструктуризированной)
                hashtable_closed = new_hashtable_closed(elements_amount * HASH_CLOSED_COEFFICIENT * EXPAND_FACTOR);
                fill_hashtable_closed(file, hashtable_closed);
                set_hashtable_comparisons(0);
                hashtable_closed_find(hashtable_closed, values[i]);
                results.hashtable_closed_increased[name_id] += get_hashtable_comparisons();
                if (i == 0)
                    results.hashtable_closed_increased_size[name_id] = get_hashtable_closed_size_bytes(hashtable_closed);
                free_table_closed(hashtable_closed);
            }
        }
        fclose(file);
    }
    */
    // print_results(&results);
}
