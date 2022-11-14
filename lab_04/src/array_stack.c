#include "stack.h"

array_stack_t *
array_stack_create(size_t max_size)
{
    array_stack_t *stack = malloc(sizeof(array_stack_t));
    if (stack == NULL)
        return NULL;
    stack->data = malloc(max_size * sizeof(int));
    if (stack->data == NULL)
    {
        free(stack);
        return NULL;
    }
    stack->max_size = max_size;
    stack->top = 0;
    return stack;
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
    return stack->top == stack->max_size;
}

int
check_array_stack_full(array_stack_t *stack)
{
    if (stack->top >= stack->max_size)
    {
        fputs("Стек переполнен", stderr);
        return ERR_STACK_FULL;
    }
    return EXIT_SUCCESS;
}

int
is_array_stack_empty(array_stack_t *stack)
{
    return stack->top == 0;
}

int
check_array_stack_empty(array_stack_t *stack)
{
    if (stack->top == 0)
    {
        fputs("Стек пуст", stderr);
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

    stack->data[stack->top++] = value;

    return EXIT_SUCCESS;
}

void
array_stack_push(array_stack_t *stack,
                 int value)
{
    stack->data[stack->top++] = value;
}

int
array_stack_pop_safe(array_stack_t *stack,
                     int *value)
{
    int rc;
    rc = check_array_stack_empty(stack);
    if (rc != EXIT_SUCCESS)
        return rc;

    *value = stack->data[--stack->top];

    return EXIT_SUCCESS;
}

int
array_stack_pop(array_stack_t *stack)
{
    return stack->data[--stack->top];
}

void
array_stack_print(array_stack_t *stack)
{
    int i = 0;
    for (i = 0; i < stack->top; i++)
        printf("element %d: %d\n", i, stack->data[i]);
    if (i == 0)
        printf("%s\n", "Stack is empty");
}

