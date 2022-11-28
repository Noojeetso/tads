#include "stack.h"

int
array_stack_push_safe(array_stack_t *stack,
                      int value);

int
array_stack_pop_safe(array_stack_t *stack,
                     int *value);

array_stack_t *
array_stack_create(size_t max_size)
{
    array_stack_t *new_array_stack = malloc(sizeof(array_stack_t));
    if (new_array_stack == NULL)
        return NULL;
    new_array_stack->data = malloc(max_size * sizeof(int));
    if (new_array_stack->data == NULL)
    {
        free(new_array_stack);
        return NULL;
    }
    new_array_stack->max_size = max_size;
    new_array_stack->size = 0;
    new_array_stack->push = array_stack_push_safe;
    new_array_stack->pop = array_stack_pop_safe;
    return new_array_stack;
}

void
array_stack_free(array_stack_t *stack)
{
    free(stack->data);
    free(stack);
}

int
is_array_stack_full(array_stack_t *stack)
{
    return stack->size == stack->max_size;
}

int
check_array_stack_full(array_stack_t *stack)
{
    if (stack->size >= stack->max_size)
    {
        fputs("Стек переполнен\n", stderr);
        return ERR_STACK_FULL;
    }
    return EXIT_SUCCESS;
}

int
is_array_stack_empty(array_stack_t *stack)
{
    return stack->size == 0;
}

int
check_array_stack_empty(array_stack_t *stack)
{
    if (stack->size == 0)
    {
        fputs("Стек пуст\n", stderr);
        return ERR_STACK_EMPTY;
    }
    return EXIT_SUCCESS;
}

int
array_stack_push_safe(array_stack_t *stack,
                      int value)
{
    int rc;
    rc = check_array_stack_full(stack);
    if (rc != EXIT_SUCCESS)
        return rc;

    stack->data[stack->size++] = value;

    return EXIT_SUCCESS;
}

void
array_stack_push(array_stack_t *stack,
                 int value)
{
    stack->data[stack->size++] = value;
}

int
array_stack_pop_safe(array_stack_t *stack,
                     int *value)
{
    int rc;
    rc = check_array_stack_empty(stack);
    if (rc != EXIT_SUCCESS)
        return rc;

    *value = stack->data[--stack->size];

    return EXIT_SUCCESS;
}

int
array_stack_pop(array_stack_t *stack)
{
    return stack->data[--stack->size];
}

void
array_stack_print(array_stack_t *stack)
{
    int value;
    array_stack_t *tmp_stack;
    size_t stack_size = stack->size;

    if (stack_size == 0)
        puts("Стек пуст");
    else
    {
        tmp_stack = array_stack_create(stack_size);
        for (size_t i = stack_size; i > 0; i--)
        {
            stack->pop(stack, &value);
            tmp_stack->push(tmp_stack, value);
            printf("Элемент %lu: %d\n", stack_size - i + 1, value);
        }

        for (size_t i = stack_size; i > 0; i--)
        {
            tmp_stack->pop(tmp_stack, &value);
            stack->push(stack, value);
        }

        array_stack_free(tmp_stack);
    }
}

int
get_value(char *buffer,
          size_t buffer_size,
          int *value)
{
    size_t str_length;
    char *endptr;

    if (fgets(buffer, buffer_size, stdin) == NULL)
        return EXIT_FAILURE;
    str_length = strlen(buffer);

    if (str_length == 1)
        return EOF;

    if (buffer[str_length - 1] != '\n')
    {
        fputs("Ошибка считывания числа\n", stderr);
        return EXIT_FAILURE;
    }
    buffer[str_length - 1] = '\0';

    *value = strtol(buffer, &endptr, 10);

    if (endptr - buffer != strlen(buffer))
    {
        fputs("Ошибка считывания числа\n", stderr);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void
array_stack_print_reversed_task(void)
{
    int rc;
    int previous_value;
    int value;
    size_t buffer_size = log(INT_MAX) / log(10) + 4;
    char *buffer = malloc(buffer_size);

    size_t max_array_size = 10;
    size_t array_size = 0;
    int *output_array = malloc(max_array_size * sizeof(int));

    array_stack_t *stack = array_stack_create(100);
    size_t stack_size;
    int stack_value;

    printf("Введите последовательность чисел (от %d до %d), каждое в новой строке.\n", INT_MIN, INT_MAX);
    puts("По окончании ввода нажмите ENTER.");

    rc = get_value(buffer, buffer_size, &value);
    previous_value = value;
    while (rc == EXIT_SUCCESS)
    {
        if (value >= previous_value)
        {
            stack_size = stack->size;
            for (size_t i = 0; i < stack_size; i++)
            {
                if (array_size == max_array_size)
                {
                    output_array = realloc(output_array, max_array_size * 2 * sizeof(int));
                    max_array_size *= 2;
                }
                stack->pop(stack, &stack_value);
                output_array[array_size++] = stack_value;
            }
        }
        stack->push(stack, value);

        previous_value = value;
        rc = get_value(buffer, buffer_size, &value);
    }

    if (rc != EOF)
    {
        free(buffer);
        free(output_array);
        return;
    }

    stack_size = stack->size;
    for (size_t i = 0; i < stack_size; i++)
    {
        if (array_size == max_array_size)
        {
            output_array = realloc(output_array, max_array_size * 2 * sizeof(int));
            max_array_size *= 2;
        }
        stack->pop(stack, &stack_value);
        output_array[array_size++] = stack_value;
    }

    puts("Полученная строка:");
    for (size_t i = 0; i < array_size; i++)
        printf("%d%c", output_array[i], i == array_size - 1 ? '\n' : ' ');
}
