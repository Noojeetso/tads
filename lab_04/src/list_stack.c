#include "stack.h"

list_stack_t *
list_stack_create(size_t max_size)
{
    list_stack_t *new_list_stack = malloc(sizeof(list_stack_t));
    if (new_list_stack == NULL)
    {
        fprintf(stderr, "%s\n", "No memory for the new list stack");
        return NULL;
    }
    new_list_stack->max_size = max_size;
    new_list_stack->head = NULL;

    return new_list_stack;
}

void
list_stack_clear(list_stack_t *stack)
{
    node_t *current;
    node_t *prev = stack->head;

    if (stack->head != NULL)
        while (prev->prev != NULL)
        {
            current = current->prev;
            free(prev);
        }

    free(prev);
}

void
list_stack_free(list_stack_t *stack)
{
    list_stack_clear(stack);
    free(stack);
}

adress_array_t *
adress_array_create(size_t max_size)
{
    adress_array_t *new_adress_array = malloc(sizeof(adress_array_t));
    if (new_adress_array == NULL)
    {
        fprintf(stderr, "%s\n", "No memory for the new adress array struct");
        return NULL;
    }
    new_adress_array->data = malloc(max_size * sizeof(node_t*));
    if (new_adress_array->data == NULL)
    {
        fprintf(stderr, "%s\n", "No memory for the new adress array's data");
        free(new_adress_array);
        return NULL;
    }
    return new_adress_array;
}

void
adress_array_free(adress_array_t *adress_array)
{
    free(adress_array->data);
    free(adress_array);
}

int
is_list_stack_empty(list_stack_t *stack)
{
    return stack->size == 0;
}

int
check_list_stack_empty(list_stack_t *stack)
{
    if (stack->size == 0)
    {
        fputs("Стек пуст", stderr);
        return ERR_STACK_EMPTY;
    }
    return EXIT_SUCCESS;
}

int
is_list_stack_full(list_stack_t *stack)
{
    return stack->size == stack->max_size;
}

int
check_list_stack_full(list_stack_t *stack)
{
    if (stack->size == stack->max_size)
    {
        fputs("Стек переполнен", stderr);
        return ERR_STACK_FULL;
    }
    return EXIT_SUCCESS;
}

int
list_stack_push(list_stack_t *stack,
                int value)
{
    node_t *new_node = malloc(sizeof(node_t));
    if (new_node != NULL)
    {
        fprintf(stderr, "%s\n", "No memory for the new node");
        return ERR_NO_MEMORY;
    }
    new_node->prev = stack->head;
    stack->head = new_node;
    return EXIT_SUCCESS;
}

int
list_stack_pop(list_stack_t *stack)
{
    int value = stack->head->value;
    // add_head_adress???
    node_t *tmp_node = stack->head->prev;
    free(stack->head);
    stack->head = tmp_node;

    return value;
}

int
list_stack_pop_safe(list_stack_t *stack,
                    int *value)
{
    int rc;
    rc = check_list_stack_empty(stack);
    if (rc != EXIT_SUCCESS)
        return rc;

    *value = stack->head->value;
    // add_head_adress???
    node_t *tmp_node = stack->head->prev;
    free(stack->head);
    stack->head = tmp_node;

    return EXIT_SUCCESS;
}

int
add_head_adress(list_stack_t *list_stack,
                adress_array_t *adress_array)
{
    if (list_stack == NULL)
    {
        fputs("Передан указатель на несуществующий стек", stderr);
        return ERR_NULL_POINTER;
    }

    adress_array->data[adress_array->size] = list_stack->head;
    adress_array->size++;

    return EXIT_SUCCESS;
}

/// ??? tmp_stack ???
void
list_stack_print(list_stack_t *stack)
{
    node_t *prev = stack->head;

    while (prev != NULL)
    {
        printf("element %d\n", prev->value);
        prev = prev->prev;
    }
}

