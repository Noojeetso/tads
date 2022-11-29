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

int *
get_array_stack_reversed_sequence(int *input_array,
                                  size_t array_size)
{
    int previous_value;
    int value;
    array_stack_t *stack = array_stack_create(100);
    size_t stack_size;
    int stack_value;
    size_t index = 0;

    int *output_array = malloc(array_size * sizeof(int));

    value = input_array[0];
    stack->push(stack, value);
    previous_value = value;
    for (size_t i = 1; i < array_size; i++)
    {
        value = input_array[i];
        if (value >= previous_value)
        {
            stack_size = stack->size;
            for (size_t j = 0; j < stack_size; j++)
            {
                stack->pop(stack, &stack_value);
                output_array[index++] = stack_value;
            }
        }
        stack->push(stack, value);
        previous_value = value;
    }

    stack_size = stack->size;
    for (size_t i = 0; i < stack_size; i++)
    {
        stack->pop(stack, &stack_value);
        output_array[index++] = stack_value;
    }

    array_stack_free(stack);

    return output_array;
}
