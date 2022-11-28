#include "stack.h"

int
list_stack_push_safe(list_stack_t *stack,
                     int value);

int
list_stack_pop_safe(list_stack_t *stack,
                    int *value);

list_stack_t *
list_stack_create(size_t max_size)
{
    list_stack_t *new_list_stack = malloc(sizeof(list_stack_t));
    if (new_list_stack == NULL)
    {
        fprintf(stderr, "%s\n", "Не была выделена память под структуру стека в виде списка");
        return NULL;
    }
    new_list_stack->max_size = max_size;
    new_list_stack->tail = NULL;
    new_list_stack->push = list_stack_push_safe;
    new_list_stack->pop = list_stack_pop_safe;
    return new_list_stack;
}

void
list_stack_clear(list_stack_t *stack)
{
    node_t *current;
    node_t *prev = stack->tail;

    if (stack->tail != NULL)
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
        fprintf(stderr, "%s\n", "Не была выделена память под структуру массива адресов");
        return NULL;
    }
    new_adress_array->data = malloc(max_size * sizeof(node_t*));
    if (new_adress_array->data == NULL)
    {
        fprintf(stderr, "%s\n", "Не была выделена память под элементы массива адресов");
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
        fputs("Стек пуст\n", stderr);
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
        fputs("Стек переполнен\n", stderr);
        return ERR_STACK_FULL;
    }
    return EXIT_SUCCESS;
}

int
list_stack_push_safe(list_stack_t *stack,
                     int value)
{
    node_t *new_node = malloc(sizeof(node_t));
    if (new_node == NULL)
    {
        fprintf(stderr, "%s\n", "Не была выделена память под новый узел стека");
        return ERR_NO_MEMORY;
    }
    new_node->value = value;
    new_node->prev = stack->tail;
    stack->tail = new_node;
    stack->size++;

    return EXIT_SUCCESS;
}

int
list_stack_pop(list_stack_t *stack)
{
    int value = stack->tail->value;
    node_t *tmp_node = stack->tail->prev;
    free(stack->tail);
    stack->tail = tmp_node;
    stack->size--;

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

    *value = stack->tail->value;
    node_t *tmp_node = stack->tail->prev;
    free(stack->tail);
    stack->tail = tmp_node;
    stack->size--;

    return EXIT_SUCCESS;
}

int
add_tail_adress(adress_array_t *adress_array,
                list_stack_t *list_stack)
{
    if (list_stack == NULL)
    {
        // fputs("Передан указатель на несуществующий стек", stderr);
        return ERR_NULL_POINTER;
    }

    if (list_stack->tail == NULL)
    {
        return ERR_NULL_POINTER;
    }

    adress_array->data[adress_array->size] = list_stack->tail;
    adress_array->size++;

    return EXIT_SUCCESS;
}

int
remove_adress_by_id(adress_array_t *adress_array,
                    size_t index)
{
    if (index >= adress_array->size)
    {
        fputs("Элемента с таким индексом не существует\n", stderr);
        return ERR_DOMAIN;
    }
    for (size_t i = index; i < adress_array->size - 1; i++)
        adress_array->data[i] = adress_array->data[i + 1];
    adress_array->size--;
    return EXIT_SUCCESS;
}

void
remove_tail_adress(adress_array_t *adress_array,
                   list_stack_t *list_stack)
{
    for (size_t i = 0; i < adress_array->size; i++)
    {
        if (adress_array->data[i] == list_stack->tail)
        {
            remove_adress_by_id(adress_array, i);
            return;
        }
    }
}

void
print_adress_array(adress_array_t *adress_array)
{
    size_t array_size = adress_array->size;

    if (array_size == 0)
    {
        puts("Массив указателей на освободившиеся адреса пуст");
        return;
    }

    puts("Массив указателей на освободившиеся адреса:");
    for (size_t i = 0; i < array_size; i++)
        printf("%lu.\t%p\n", i, (void *)adress_array->data[i]);
}

void
list_stack_print(list_stack_t *stack)
{
    int value;
    list_stack_t *tmp_stack;
    size_t stack_size = stack->size;

    if (stack_size == 0)
        puts("Стек пуст");
    else
    {
        tmp_stack = list_stack_create(stack_size);
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

        list_stack_free(tmp_stack);
    }
}
