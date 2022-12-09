#ifndef __GRAPHVIZ_H__
#define __GRAPHVIZ_H__

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "files.h"
#include "tree.h"

#define OUT_FOLDER "out/"
#define DOT_EXTENSION "dot"
#define IMAGE_EXTENSION "svg"

int
tree_visualize(tree_node_t *tree,
               char *tree_name);

#endif  // __GRAPHVIZ_H__
