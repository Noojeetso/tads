#ifndef __STACK_H__
#define __STACK_H__

#include <stdlib.h>
#include <stdio.h>
#include "presets.h"
#include "errors.h"
#include "scan.h"

typedef struct array_stack
{
    int *data;
    size_t max_size;
    size_t size;
    int (*push)(struct array_stack *list_stack, int value);
    int (*pop)(struct array_stack *list_stack, int *value);
} array_stack_t;

typedef struct node node_t;

struct node
{
    int value;
    node_t *prev;
};

typedef struct list_stack
{
    size_t max_size;
    size_t size;
    node_t *tail;
    int (*push)(struct list_stack *list_stack, int value);
    int (*pop)(struct list_stack *list_stack, int *value);
} list_stack_t;

typedef struct adress_array
{
    size_t max_size;
    size_t size;
    node_t **data;
} adress_array_t;

// ARRAY_STACK
array_stack_t *
array_stack_create(size_t max_size);

void
array_stack_free(array_stack_t *stack);

int
is_array_stack_full(array_stack_t *stack);

int
check_array_stack_full(array_stack_t *stack);

int
is_array_stack_empty(array_stack_t *stack);

int
check_array_stack_empty(array_stack_t *stack);

/*
int
array_stack_push_safe(array_stack_t *stack,
                      int value);

void
array_stack_push(array_stack_t *stack,
                 int value);

int
array_stack_pop_safe(array_stack_t *stack,
                     int *value);

int
array_stack_pop(array_stack_t *stack);
*/

void
array_stack_print(array_stack_t *stack);

int *
get_array_stack_reversed_sequence(int *input_array,
                                  size_t array_size);

// LIST_STACK
adress_array_t *
adress_array_create(size_t max_size);

void
adress_array_free(adress_array_t *adress_array);

list_stack_t *
list_stack_create(size_t max_size);

void
list_stack_clear(list_stack_t *stack);

void
list_stack_free(list_stack_t *stack);

int
is_list_stack_empty(list_stack_t *stack);

int
check_list_stack_empty(list_stack_t *stack);

int
is_list_stack_full(list_stack_t *stack);

int
check_list_stack_full(list_stack_t *stack);

/*
int
list_stack_push(list_stack_t *stack,
                int value);

int
list_stack_pop(list_stack_t *stack);

int
list_stack_pop_safe(list_stack_t *stack,
                     int *value);
*/

void
list_stack_print(list_stack_t *stack);

int *
get_list_stack_reversed_sequence(int *input_array,
                                 size_t array_size);

int
add_tail_adress(adress_array_t *adress_array,
                list_stack_t *list_stack);

void
remove_tail_adress(adress_array_t *adress_array,
                   list_stack_t *list_stack);

void
print_adress_array(adress_array_t *adress_array);

#endif  // __STACK_H__
