#ifndef __LIST_GRAPH_H__
#define __LIST_GRAPH_H__

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "presets.h"
#include "scan.h"
#include "array_queue.h"

typedef struct list_graph list_graph_t;

void
free_list_graph(list_graph_t *list_graph);

list_graph_t *
list_graph_from_file(char *filename);

int
write_list_graph_connections(FILE *file,
                             void *graph,
                             void *arg);

int
find_list_graph_min_paths(list_graph_t *list_graph,
                          int start_number,
                          int **min_paths);

#endif  // __LIST_GRAPH_H__
