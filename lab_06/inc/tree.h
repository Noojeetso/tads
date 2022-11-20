#ifndef __TREE_H__
#define __TREE_H__

#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
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

int
print_heights(tree_node_t *avl,
              tree_node_t *bst);

tree_node_t*
insert_bst(tree_node_t *tree,
           int value);

int
fill_bst(FILE *input_file,
         tree_node_t **tree);

int
bst_pop(tree_node_t **root,
        int value);

tree_node_t*
insert_avl(tree_node_t *tree,
           int value);

int
fill_avl(FILE *input_file,
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

