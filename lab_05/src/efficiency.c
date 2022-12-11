#include "efficiency.h"

void
print_insertion_efficiency(int elements_amount)
{
    clock_t array_queue_start = 0;
    clock_t array_queue_end = 0;
    clock_t list_queue_start = 0;
    clock_t list_queue_end = 0;
    array_queue_t *array_queue;
    list_queue_t *list_queue;
    int value;

    array_queue = new_array_queue(elements_amount);
    list_queue = new_list_queue();

    for (size_t i = 0; i < ITERATIONS; i++)
    {
        for (size_t j = 0; j < elements_amount; j++)
        {
            value = rand();
            array_queue_start += clock();
            array_queue_push(array_queue, value);
            array_queue_end += clock();
        }

        for (size_t j = 0; j < elements_amount; j++)
        {
            value = rand();
            list_queue_start += clock();
            list_queue_push(list_queue, value);
            list_queue_end += clock();
        }
    }

    printf("Времени затрачено на вставку %d элемент(а, ов) в тиках (на основе %d итераций):\n", elements_amount, ITERATIONS);
    printf("Очередь в виде массива: %.3lf\n", (double)(array_queue_end - array_queue_start) / ITERATIONS);
    printf("Очередь в виде списка: %.3lf\n", (double)(list_queue_end - list_queue_start) / ITERATIONS);

    free_array_queue(array_queue);
    free_list_queue(list_queue);
}

void
print_deletion_efficiency(int elements_amount)
{
    clock_t array_queue_start = 0;
    clock_t array_queue_end = 0;
    clock_t list_queue_start = 0;
    clock_t list_queue_end = 0;
    array_queue_t *array_queue;
    list_queue_t *list_queue;
    int value;

    array_queue = new_array_queue(elements_amount);
    list_queue = new_list_queue();

    for (size_t i = 0; i < ITERATIONS; i++)
    {
        for (size_t j = 0; j < elements_amount; j++)
        {
            value = rand();
            array_queue_push(array_queue, value);
            list_queue_push(list_queue, value);
        }

        array_queue_start += clock();
        for (size_t j = 0; j < elements_amount; j++)
        {
            array_queue_pop(array_queue, &value);
        }
        array_queue_end += clock();

        list_queue_start += clock();
        for (size_t j = 0; j < elements_amount; j++)
        {
            list_queue_pop(list_queue, &value);
        }
        list_queue_end += clock();
    }

    printf("Времени затрачено на удаление %d элемент(а, ов) в тиках (на основе %d итераций):\n", elements_amount, ITERATIONS);
    printf("Очередь в виде массива: %.3lf\n", (double)(array_queue_end - array_queue_start) / ITERATIONS);
    printf("Очередь в виде списка: %.3lf\n", (double)(list_queue_end - list_queue_start) / ITERATIONS);

    free_array_queue(array_queue);
    free_list_queue(list_queue);
}

void
print_time_efficiency(int elements_amount)
{
    print_insertion_efficiency(elements_amount);
    print_newline();
    print_deletion_efficiency(elements_amount);
    print_newline();
}

void
print_memory_efficiency(int elements_amount)
{
    puts("Размер занимаемой памяти (в байтах):");
    printf("Очередь в виде массива: %lu\n", get_array_queue_size_bytes(elements_amount));
    printf("Очередь в виде списка: %lu\n", get_list_queue_size_bytes(elements_amount));
}

void
compare_efficiency(void)
{
    int rc;
    int elements_amount;

    printf("Введите количество элементов от %d до %d\n", 0, MAX_DEMO_VALUE);
    rc = scan_integer(&elements_amount, 0, MAX_DEMO_VALUE);
    if (rc != EXIT_SUCCESS)
        return;

    print_newline();
    print_time_efficiency(elements_amount);
    print_memory_efficiency(elements_amount);
}
