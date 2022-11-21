#ifndef __GRAPHVIZ_H__
#define __GRAPHVIZ_H__

#include "files.h"
#include "tree.h"

int
tree_to_dot(char *file_name,
            tree_node_t *tree);
int
dot_to_svg(char *input_file_name,
           char *output_file_name);

int
open_svg(char *input_file_name);

#endif  // __GRAPHVIZ_H__
