#include "efficiency.h"

int
compare_integer(const void *a_ptr,
                const void *b_ptr)
{
    int a = *(int *)a_ptr;
    int b = *(int *)b_ptr;
    if (a == b)
        return 0;
    return a > b ? 1 : -1;
}

void
print_table(size_t *avl_results,
            size_t *bst_results,
            size_t *file_results,
            int offset)
{
    puts("Затрачено времени в микросекундах:");
    puts("Маленький файл");
    printf("АВЛ дерево:\t\t%lu\n", avl_results[offset]);
    printf("Двоичное дерево поиска:\t%lu\n", bst_results[offset]);
    printf("Файл:\t\t\t%lu\n", file_results[offset]);
    print_newline();
    puts("Средний файл");
    printf("АВЛ дерево:\t\t%lu\n", avl_results[2 + offset]);
    printf("Двоичное дерево поиска:\t%lu\n", bst_results[2 + offset]);
    printf("Файл:\t\t\t%lu\n", file_results[2 + offset]);
    print_newline();
    puts("Большой файл");
    printf("АВЛ дерево:\t\t%lu\n", avl_results[4 + offset]);
    printf("Двоичное дерево поиска:\t%lu\n", bst_results[4 + offset]);
    printf("Файл:\t\t\t%lu\n", file_results[4 + offset]);
    print_newline();
    puts("Отсортированный файл");
    printf("АВЛ дерево:\t\t%lu\n", avl_results[6 + offset]);
    printf("Двоичное дерево поиска:\t%lu\n", bst_results[6 + offset]);
    printf("Файл:\t\t\t%lu\n", file_results[6 + offset]);
    print_newline();
    puts("Файл, отсортированный в обратном порядке");
    printf("АВЛ дерево:\t\t%lu\n", avl_results[8 + offset]);
    printf("Двоичное дерево поиска:\t%lu\n", bst_results[8 + offset]);
    printf("Файл:\t\t\t%lu\n", file_results[8 + offset]);
}

void
print_results(size_t *avl_results,
              size_t *bst_results,
              size_t *file_results)
{
    print_newline();
    puts("Поиск");
    print_newline();
    print_table(avl_results, bst_results, file_results, 0);
    print_newline();
    puts("Сортировка");
    print_newline();
    print_table(avl_results, bst_results, file_results, 1);
}

void
print_efficiency(void)
{
    clock_t start, end;
    tree_node_t *avl = NULL;
    tree_node_t *bst = NULL;
    tree_node_t *node;
    FILE *file;
    size_t found_index;
    int value;
    array_t *array;
    size_t avl_results[10] = {0};
    size_t bst_results[10] = {0};
    size_t file_results[10] = {0};
    const char *file_names[] = {SMALL_FILE_NAME, MEDIUM_FILE_NAME,
                                BIG_FILE_NAME, SORTED_FILE_NAME,
                                SORTED_REVERSE_FILE_NAME};

    for (size_t name_id = 0; name_id < 5; name_id++)
    {
        printf("Файл: \"%s\"\n", file_names[name_id]);
        file = fopen(file_names[name_id], READ_MODE);
        for (size_t i = 0; i < ITERATIONS; i++)
        {
            free_tree(&avl);
            free_tree(&bst);

            fseek(file, 0, SEEK_SET);

            start = clock();
            while (fscanf(file, "%d", &value) == 1)
                avl = avl_insert(avl, value);
            end = clock();
            avl_results[2 * name_id] += (end - start) / (CLOCKS_PER_SEC / 1000000);

            start = clock();
            find_node(avl, INT_MAX);
            end = clock();
            avl_results[2 * name_id + 1] += (end - start) / (CLOCKS_PER_SEC / 1000000);

            fseek(file, 0, SEEK_SET);

            start = clock();
            while (fscanf(file, "%d", &value) == 1)
                bst = bst_insert(bst, value);
            end = clock();
            bst_results[2 * name_id] += (end - start) / (CLOCKS_PER_SEC / 1000000);
            if (bst == NULL)
                puts("NULL");
            else
                puts("NOT NULL");

            start = clock();
            node = find_node(bst, INT_MAX);
            end = clock();
            if (node == NULL)
                puts("Not found");
            else
                puts("found");
            bst_results[2 * name_id + 1] += (end - start) / (CLOCKS_PER_SEC / 1000000);

            fseek(file, 0, SEEK_SET);

            start = clock();
            array = file_to_array(file_names[name_id]);
            qsort(array->data, array->size, sizeof(int), compare_integer);
            end = clock();
            file_results[2 * name_id] += (end - start) / (CLOCKS_PER_SEC / 1000000);

            free_array(array);
            fseek(file, 0, SEEK_SET);

            start = clock();
            array = file_to_array(file_names[name_id]);
            array_search_number(array, INT_MAX, &found_index);
            end = clock();
            file_results[2 * name_id + 1] += (end - start) / (CLOCKS_PER_SEC / 1000000);

            if (i == 0)
                printf("Элементов: %lu\n", array->size);
            free_array(array);
        }
        fclose(file);
    }

    for (size_t i = 0; i < sizeof(avl_results) / sizeof(avl_results[0]); i++)
    {
        avl_results[i] /= ITERATIONS;
        bst_results[i] /= ITERATIONS;
        file_results[i] /= ITERATIONS;
    }

    print_results(avl_results, bst_results, file_results);
}
