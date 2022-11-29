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
print_structs(size_t *avl_results,
              size_t *bst_results,
              size_t *file_results,
              int index)
{
    printf("АВЛ дерево:\t\t%lu\n", avl_results[index]);
    printf("Двоичное дерево поиска:\t%lu\n", bst_results[index]);
    printf("Массив на основе файла:\t%lu\n", file_results[index]);
}

void
print_results(results_t *results)
{
    char *file_names[] = {"Маленький файл", "Средний файл",
                          "Большой файл", "Отсортированный файл",
                          "Файл, отсортированный в обратном порядке"};
    print_newline();
    print_newline();
    puts("Сортировка (время в микросекундах)");
    for (size_t i = 0; i < sizeof(file_names) / sizeof(file_names[0]); i++)
    {
        puts(file_names[i]);
        print_structs(results->avl_sort, results->bst_sort, results->file_sort, i);
        print_newline();
    }
    print_newline();
    puts("Поиск (количество сравнений)");
    for (size_t i = 0; i < sizeof(file_names) / sizeof(file_names[0]); i++)
    {
        puts(file_names[i]);
        print_structs(results->avl_search, results->bst_search, results->file_search, i);
        print_newline();
    }
    print_newline();
    puts("Средние коэффициенты ветвления деревьев:");
    for (size_t i = 0; i < sizeof(file_names) / sizeof(file_names[0]); i++)
    {
        puts(file_names[i]);
        printf("АВЛ дерево: %.3lf\n", results->avl_factors[i]);
        printf("ДДП: %.3lf\n", results->bst_factors[i]);
        print_newline();
    }
    print_newline();
    printf("Размер узла дерева в байтах: %lu\n", sizeof(tree_node_t));
    printf("Размер структуры массива в байтах: %lu\n", sizeof(array_t));
    printf("Размер элемента массива в байтах: %lu\n", sizeof(int));
}

void
print_efficiency(void)
{
    clock_t start, end;
    tree_node_t *avl = NULL;
    tree_node_t *bst = NULL;
    FILE *file;
    array_t *array;
    results_t results = {0};
    const char *file_names[] = {SMALL_FILE_NAME, MEDIUM_FILE_NAME,
                                BIG_FILE_NAME, SORTED_FILE_NAME,
                                SORTED_REVERSE_FILE_NAME};
    tree_node_t *result_node;
    size_t search_compares;
    int *sorted_array;
    int sort_index;
    size_t elements_amount;
    int values[ITERATIONS];

    // Генерация чисел для поиска
    for (int i = 0; i < ITERATIONS; i++)
        values[i] = rand();

    for (size_t name_id = 0; name_id < sizeof(file_names) / sizeof(file_names[0]); name_id++)
    {
        printf("Файл: \"%s\"\n", file_names[name_id]);
        file = fopen(file_names[name_id], READ_MODE);
        for (size_t i = 0; i < ITERATIONS; i++)
        {
            // Замер времени создания отсортированного массива в мкс
            start = clock();
            array = file_to_array(file_names[name_id]);
            qsort(array->data, array->size, sizeof(int), compare_int_pointers);
            end = clock();
            results.file_sort[name_id] += (end - start) / (CLOCKS_PER_SEC / 1000000);
            fseek(file, 0, SEEK_SET);
            elements_amount = array->size;
            if (i == 0)
                printf("Элементов в файле: %lu\n", elements_amount);
            // Нормализация числа для поиска
            values[i] %= elements_amount / 2;
            free_array(array);

            // Подсчёт количества сравнений при поиске элемента в массиве
            array = file_to_array(file_names[name_id]);
            for (search_compares = 0; search_compares < array->size; search_compares++)
                if (array->data[search_compares] == values[i])
                    break;
            results.file_search[name_id] += search_compares;
            fseek(file, 0, SEEK_SET);
            free_array(array);

            // Замер времени сортировки массива с помощью АВЛ дерева в мкс
            start = clock();
            sorted_array = malloc(elements_amount * sizeof(int));
            sort_index = 0;
            avl_fill(file, &avl);
            tree_sort(avl, sorted_array, &sort_index);
            end = clock();
            results.avl_sort[name_id] += (end - start) / (CLOCKS_PER_SEC / 1000000);
            results.avl_factors[name_id] += get_tree_factor(avl);
            fseek(file, 0, SEEK_SET);
            free(sorted_array);

            // Замер времени сортировки массива с помощью ДДП в мкс
            start = clock();
            sorted_array = malloc(elements_amount * sizeof(int));
            sort_index = 0;
            bst_fill(file, &bst);
            tree_sort(bst, sorted_array, &sort_index);
            end = clock();
            results.bst_sort[name_id] += (end - start) / (CLOCKS_PER_SEC / 1000000);
            results.bst_factors[name_id] += get_tree_factor(bst);
            fseek(file, 0, SEEK_SET);
            free(sorted_array);

            // Подсчёт количества сравнений при поиске элемента в АВЛ дереве
            search_compares = 0;
            result_node = avl;
            while (result_node != NULL)
            {
                if (result_node->value > values[i])
                    result_node = result_node->left;
                else if (result_node->value < values[i])
                    result_node = result_node->right;
                else
                    break;
                search_compares++;
            }
            results.avl_search[name_id] += search_compares;
            free_tree(&avl);

            // Подсчёт количества сравнений при поиске элемента в ДДП
            search_compares = 0;
            result_node = bst;
            while (result_node != NULL)
            {
                if (result_node->value > values[i])
                    result_node = result_node->left;
                else if (result_node->value < values[i])
                    result_node = result_node->right;
                else
                    break;
                search_compares++;
            }
            results.bst_search[name_id] += search_compares;
            free_tree(&bst);
        }
        fclose(file);
    }

    // Нахождение среднего всех значений
    for (size_t i = 0; i < sizeof(file_names) / sizeof(file_names[0]); i++)
    {
        results.avl_sort[i] /= ITERATIONS;
        results.bst_sort[i] /= ITERATIONS;
        results.file_sort[i] /= ITERATIONS;
        results.avl_factors[i] /= ITERATIONS;
        results.bst_factors[i] /= ITERATIONS;
    }

    print_results(&results);
}
