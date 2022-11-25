#ifndef __TREE_H__
#define __TREE_H__

#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include "presets.h"
#include "errors.h"

typedef struct tree_node
{
    int value;
    unsigned char height;
    struct tree_node *left;
    struct tree_node *right;
} tree_node_t;

tree_node_t*
new_node(int value);

void
free_tree(tree_node_t **tree);

void
print_heights(tree_node_t *avl,
              tree_node_t *bst);

tree_node_t *
find_node(tree_node_t *tree,
          int value);

void
tree_search(tree_node_t *tree,
            int value);

int
bst_push(tree_node_t **tree,
         int value);

tree_node_t*
bst_insert(tree_node_t *tree,
           int value);

int
bst_fill(FILE *input_file,
         tree_node_t **tree);

int
bst_pop(tree_node_t **root,
        int value);

void
tree_sort(tree_node_t *tree,
          int *array,
          int *index);

tree_node_t*
avl_insert(tree_node_t *tree,
           int value);

int
avl_push(tree_node_t **tree,
         int value);

int
avl_fill(FILE *input_file,
         tree_node_t **tree);


/*
tree_node_t *
avl_remove(tree_node_t *root,
           int value);
*/
int
avl_pop(tree_node_t **root,
        int value);

#endif  // __TREE_H__

