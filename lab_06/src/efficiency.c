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
    puts("Затрачено времени в микросекундах:");
    print_newline();
    print_table(avl_results, bst_results, file_results, 0);
    print_newline();
    puts("Поиск");
    puts("Произведено сравнений:");
    print_newline();
    print_table(avl_results, bst_results, file_results, 1);
    print_newline();
    printf("Размер узла дерева в байтах: %lu\n", sizeof(tree_node_t));
    printf("Размер элемента массива в байтах: %lu\n", sizeof(int));
    printf("Размер структуры массива в байтах: %lu\n", sizeof(array_t));
}

void
print_efficiency(void)
{
    clock_t start, end;
    tree_node_t *avl = NULL;
    tree_node_t *bst = NULL;
    FILE *file;
    array_t *array;
    size_t avl_results[10] = {0};
    size_t bst_results[10] = {0};
    size_t file_results[10] = {0};
    const char *file_names[] = {SMALL_FILE_NAME, MEDIUM_FILE_NAME,
                                BIG_FILE_NAME, SORTED_FILE_NAME,
                                SORTED_REVERSE_FILE_NAME};
    tree_node_t *result_node;
    size_t search_compares;
    int *sorted_array;
    int sort_index;
    int value;
    size_t elements_amount;
    int values[ITERATIONS];

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
            qsort(array->data, array->size, sizeof(int), compare_integer);
            end = clock();
            file_results[2 * name_id] += (end - start) / (CLOCKS_PER_SEC / 1000000);
            fseek(file, 0, SEEK_SET);
            free_array(array);

            // Подсчёт количества сравнений при поиске элемента в массиве
            array = file_to_array(file_names[name_id]);
            for (search_compares = 0; search_compares < array->size; search_compares++)
                if (array->data[search_compares] == value)
                    break;
            file_results[2 * name_id + 1] += search_compares;
            elements_amount = array->size;
            fseek(file, 0, SEEK_SET);
            free_array(array);

            if (i == 0)
                printf("Элементов в файле: %lu\n", elements_amount);
            values[i] %= elements_amount / 2;

            // Замер времени создания АВЛ дерева в мкс
            start = clock();
            while (fscanf(file, "%d", &value) == 1)
                avl = avl_insert(avl, value);
            end = clock();
            avl_results[2 * name_id] += (end - start) / (CLOCKS_PER_SEC / 1000000);
            fseek(file, 0, SEEK_SET);

            // Замер времени создания ДДП в мкс
            start = clock();
            sorted_array = malloc(elements_amount * sizeof(int));
            while (fscanf(file, "%d", &value) == 1)
                bst = bst_insert(bst, value);
            sort_index = 0;
            tree_sort(bst, sorted_array, &sort_index);
            end = clock();
            bst_results[2 * name_id] += (end - start) / (CLOCKS_PER_SEC / 1000000);
            fseek(file, 0, SEEK_SET);
            free(sorted_array);

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
            bst_results[2 * name_id + 1] += search_compares;
            free_tree(&bst);

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
            avl_results[2 * name_id + 1] += search_compares;
            free_tree(&avl);
        }
        fclose(file);
    }

    // Нахождение среднего всех значений
    for (size_t i = 0; i < sizeof(avl_results) / sizeof(avl_results[0]); i++)
    {
        avl_results[i] /= ITERATIONS;
        bst_results[i] /= ITERATIONS;
        file_results[i] /= ITERATIONS;
    }

    print_results(avl_results, bst_results, file_results);
}
