#ifndef __STACK_H__
#define __STACK_H__

#include <stdlib.h>
#include <stdio.h>
#include "presets.h"
#include "errors.h"

typedef struct struct_array
{
    int *data;
    size_t max_size;
    size_t top;
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
    node_t *head;
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

void
array_stack_print(array_stack_t *stack);

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

int
list_stack_push(list_stack_t *stack,
                int value);

int
list_stack_pop(list_stack_t *stack);

int
list_stack_pop_safe(list_stack_t *stack,
                     int *value);

int
add_head_adress(list_stack_t *list_stack,
                adress_array_t *adress_array);

void
list_stack_print(list_stack_t *stack);

#endif  // __STACK_H__

