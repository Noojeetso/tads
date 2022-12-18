#ifndef __ARRAY_GRAPH_H__
#define __ARRAY_GRAPH_H__

#include <stdlib.h>
#include <stdio.h>
#include "scan.h"
#include "files.h"

typedef struct array_graph array_graph_t;

void
free_array_graph(array_graph_t *array_graph);

array_graph_t *
array_graph_from_file(char *filename);

int
write_array_graph_connections(FILE *file,
                              array_graph_t *array_graph);

#endif  // __ARRAY_GRAPH_H__
