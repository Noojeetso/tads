#ifndef __GRAPHVIZ_H__
#define __GRAPHVIZ_H__

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
// #include "files.h"
#include "array_graph.h"
#include "list_graph.h"

#define OUT_FOLDER "out/"
#define DOT_EXTENSION "dot"
#define IMAGE_EXTENSION "svg"

int
list_graph_visualize(list_graph_t *list_graph,
                     char *graph_name);

int
array_graph_visualize(array_graph_t *array_graph,
                      char *graph_name);

#endif  // __GRAPHVIZ_H__
