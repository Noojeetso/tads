#ifndef __LIST_GRAPH_H__
#define __LIST_GRAPH_H__

#include <stdlib.h>
#include <stdio.h>
#include "scan.h"
#include "files.h"

typedef struct list_graph list_graph_t;

void
free_list_graph(list_graph_t *list_graph);

list_graph_t *
list_graph_from_file(char *filename);

int
write_list_graph_connections(FILE *file,
                             list_graph_t *list_graph);

#endif  // __LIST_GRAPH_H__
