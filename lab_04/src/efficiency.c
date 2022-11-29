#include "efficiency.h"

void
fill_random_numbers(int *random_numbers,
                    int max_size)
{
    for (size_t i = 0; i < max_size; i++)
        random_numbers[i] = rand();
}

void
print_memory_results(unsigned int max_size)
{
    printf("Количество занимаемой памяти (в байтах) для %d элемент(а, ов)\n", max_size);
    printf("Стеком на основе массива: %lu\n", sizeof(array_stack_t) + max_size * sizeof(int));
    printf("Стеком на основе списка: %lu\n", sizeof(list_stack_t) + max_size * sizeof(node_t));
}

void
print_push_results(int max_size)
{
    array_stack_t *array_stack = array_stack_create(max_size);
    list_stack_t *list_stack = list_stack_create(max_size);
    size_t time_array = 0;
    size_t time_list = 0;
    clock_t start, end;
    int *random_numbers;

    if (array_stack == NULL || list_stack == NULL)
        return;

    random_numbers = malloc(max_size * sizeof(int));
    if (random_numbers == NULL)
        return;

    fill_random_numbers(random_numbers, max_size);

    for (size_t i = 0; i < ITERATIONS; i++)
    {
        start = clock();
        for (int j = 0; j < max_size; j++)
            array_stack->push(array_stack, random_numbers[j]);
        end = clock();
        time_array += (end - start) / (CLOCKS_PER_SEC / 1000000);

        for (int j = 0; j < max_size; j++)
            array_stack->pop(array_stack, &random_numbers[j]);

        start = clock();
        for (int j = 0; j < max_size; j++)
            list_stack->push(list_stack, random_numbers[j]);
        end = clock();
        time_list += (end - start) / (CLOCKS_PER_SEC / 1000000);

        for (int j = 0; j < max_size; j++)
            list_stack->pop(list_stack, &random_numbers[j]);
    }

    free(random_numbers);
    array_stack_free(array_stack);
    list_stack_free(list_stack);

    time_array /= ITERATIONS;
    time_list /= ITERATIONS;

    printf("Времени (в микросекундах) портачено на добавление в стек %d элемент(а, ов)\n", max_size);
    printf("Стеком на основе массива: %lu\n", time_array);
    printf("Стеком на основе списка: %lu\n", time_list);
}

void
print_pop_results(int max_size)
{
    array_stack_t *array_stack = array_stack_create(max_size);
    list_stack_t *list_stack = list_stack_create(max_size);
    size_t time_array = 0;
    size_t time_list = 0;
    clock_t start, end;
    int *random_numbers;

    if (array_stack == NULL || list_stack == NULL)
        return;

    random_numbers = malloc(max_size * sizeof(int));
    if (random_numbers == NULL)
        return;

    fill_random_numbers(random_numbers, max_size);

    for (size_t i = 0; i < ITERATIONS; i++)
    {
        for (int j = 0; j < max_size; j++)
            array_stack->push(array_stack, random_numbers[j]);

        start = clock();
        for (int j = 0; j < max_size; j++)
            array_stack->pop(array_stack, &random_numbers[j]);
        end = clock();
        time_array += (end - start) / (CLOCKS_PER_SEC / 1000000);

        for (int j = 0; j < max_size; j++)
            list_stack->push(list_stack, random_numbers[j]);

        start = clock();
        for (int j = 0; j < max_size; j++)
            list_stack->pop(list_stack, &random_numbers[j]);
        end = clock();
        time_list += (end - start) / (CLOCKS_PER_SEC / 1000000);
    }

    free(random_numbers);
    array_stack_free(array_stack);
    list_stack_free(list_stack);

    time_array /= ITERATIONS;
    time_list /= ITERATIONS;

    printf("Времени (в микросекундах) портачено на удаление из стека %d элемент(а, ов)\n", max_size);
    printf("Стеком на основе массива: %lu\n", time_array);
    printf("Стеком на основе списка: %lu\n", time_list);
}

void
print_task_results(int max_size)
{
    array_stack_t *array_stack = array_stack_create(max_size);
    list_stack_t *list_stack = list_stack_create(max_size);
    size_t time_array = 0;
    size_t time_list = 0;
    clock_t start, end;
    int *random_numbers;

    if (array_stack == NULL || list_stack == NULL)
        return;

    random_numbers = malloc(max_size * sizeof(int));
    if (random_numbers == NULL)
        return;

    fill_random_numbers(random_numbers, max_size);

    for (size_t i = 0; i < ITERATIONS; i++)
    {
        start = clock();
        get_array_stack_reversed_sequence(random_numbers, max_size);
        end = clock();
        time_array += (end - start) / (CLOCKS_PER_SEC / 1000000);

        start = clock();
        get_list_stack_reversed_sequence(random_numbers, max_size);
        end = clock();
        time_list += (end - start) / (CLOCKS_PER_SEC / 1000000);
    }

    free(random_numbers);
    array_stack_free(array_stack);
    list_stack_free(list_stack);

    time_array /= ITERATIONS;
    time_list /= ITERATIONS;

    printf("Времени (в микросекундах) портачено на решение задачи по варианту для %d элемент(а, ов)\n", max_size);
    printf("Стеком на основе массива: %lu\n", time_array);
    printf("Стеком на основе списка: %lu\n", time_list);
}

void
print_time_results(int max_size)
{
    print_push_results(max_size);
    print_newline();
    print_pop_results(max_size);
    print_newline();
    print_task_results(max_size);
}

void
print_results(void)
{
    int rc;
    int max_size;

    puts("Введите количество обрабатываемых элементов для оценки эффективности работы стеков (от 1 до 10000): ");

    rc = scanf("%d", &max_size);
    if (rc != 1)
    {
        fputs("Ошибка считывания количества элементов\n", stderr);
        return;
    }

    if (max_size < 1 || max_size > 10000)
    {
        fputs("Введённое число выходит из допустимого диапазона\n", stderr);
        return;
    }

    print_newline();
    print_memory_results(max_size);
    print_newline();
    print_time_results(max_size);
    print_newline();
}
