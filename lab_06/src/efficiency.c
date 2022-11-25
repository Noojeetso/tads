#include "efficiency.h"
#include "graphviz.h"

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
    puts("Сортировка");
    print_newline();
    print_table(avl_results, bst_results, file_results, 0);
    print_newline();
    puts("Поиск");
    print_newline();
    print_table(avl_results, bst_results, file_results, 1);
    print_newline();
    printf("Размер узла дерева в байтах: %lu\n", sizeof(tree_node_t));
    printf("Размер элемента массива в байтах: %lu\n", sizeof(int));
    printf("Размер структуры массива в байтах: %lu\n", sizeof(array_t));
}

extern int search_compares;

void
print_efficiency(void)
{
    // int rc;
    clock_t start, end;
    tree_node_t *avl = NULL;
    tree_node_t *bst = NULL;
    FILE *file;
    size_t found_index;
    int value;
    array_t *array;
    size_t avl_results[12] = {0};
    size_t bst_results[12] = {0};
    size_t file_results[12] = {0};
    const char *file_names[] = {SMALL_FILE_NAME, MEDIUM_FILE_NAME,
                                BIG_FILE_NAME, "database/numbers_big_2.txt", SORTED_FILE_NAME,
                                SORTED_REVERSE_FILE_NAME};
    int values[ITERATIONS];

    for (int i = 0; i < ITERATIONS; i++)
        values[i] = rand();

    for (size_t name_id = 0; name_id < 5; name_id++)
    {
        printf("Файл: \"%s\"\n", file_names[name_id]);
        file = fopen(file_names[name_id], READ_MODE);
        for (size_t i = 0; i < ITERATIONS; i++)
        {
            free_tree(&avl);
            free_tree(&bst);

            //fseek(file, 0, SEEK_SET);

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
                printf("Элементов в файле: %lu\n", array->size);
            values[i] %= array->size / 2;

            free_array(array);

            fseek(file, 0, SEEK_SET);

            start = clock();
            while (fscanf(file, "%d", &value) == 1)
                avl = avl_insert(avl, value);
            end = clock();
            avl_results[2 * name_id] += (end - start) / (CLOCKS_PER_SEC / 1000000);
            puts("avl created");

            printf("search value: %d\n", values[i]);
            fseek(file, 0, SEEK_SET);

            start = clock();
            while (fscanf(file, "%d", &value) == 1)
                bst = bst_insert(bst, value);
            end = clock();
            bst_results[2 * name_id] += (end - start) / (CLOCKS_PER_SEC / 1000000);
            puts("bst created");

            tree_node_t *result_node;

            search_compares = 0;
            start = clock();
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
            // find_node(bst, values[i]);
            end = clock();
            printf("bst compares: %d, time: %ld\n", search_compares, (end - start) / (CLOCKS_PER_SEC / 1000000));
            bst_results[2 * name_id + 1] += (end - start) / (CLOCKS_PER_SEC / 1000000);

            search_compares = 0;
            start = clock();
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
            // find_node(avl, values[i]);
            end = clock();
            printf("avl compares: %d, time: %ld\n", search_compares, (end - start) / (CLOCKS_PER_SEC / 1000000));
            avl_results[2 * name_id + 1] += (end - start) / (CLOCKS_PER_SEC / 1000000);

            /*
            int rc;
            rc = tree_to_dot("out/avl_tree.dot", avl);
            if (rc != EXIT_SUCCESS)
                break;

            rc = dot_to_svg("out/avl_tree.dot", "out/avl_tree.svg");
            if (rc != EXIT_SUCCESS)
                break;

            rc = open_svg("out/avl_tree.svg");
            if (rc != EXIT_SUCCESS)
                break;
            fgetc(stdin);
            */
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
