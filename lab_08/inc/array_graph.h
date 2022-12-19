#ifndef __ARRAY_GRAPH_H__
#define __ARRAY_GRAPH_H__

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "presets.h"
#include "scan.h"
#include "array_queue.h"

typedef struct array_graph array_graph_t;

void
free_array_graph(array_graph_t *array_graph);

array_graph_t *
array_graph_from_file(char *filename);

int
write_array_graph_connections(FILE *file,
                              void *graph,
                              void *arg);

int
find_array_graph_min_paths(array_graph_t *array_graph,
                           int start_number,
                           int **min_paths);

#endif  // __ARRAY_GRAPH_H__
