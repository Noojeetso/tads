#include "list_graph.h"

typedef struct vertex
{
    int number;
    struct vertex *next;
} vertex_t;

struct list_graph
{
    size_t size;
    vertex_t **vertices;
};

list_graph_t *
new_list_graph(size_t size)
{
    list_graph_t *new_graph = malloc(sizeof(list_graph_t));
    if (new_graph == NULL)
    {
        fputs("Ошибка выделения памяти под структуру графа в виде списка\n", stderr);
        return NULL;
    }

    new_graph->vertices = malloc(size * sizeof(vertex_t *));
    if (new_graph->vertices == NULL)
    {
        fputs("Ошибка выделения памяти под массив указателей на вершины графа в виде списка\n", stderr);
        free(new_graph);
        return NULL;
    }

    new_graph->size = size;

    for (size_t i = 0; i < size; i++)
        new_graph->vertices[i] = NULL;

    return new_graph;
}

void
clear_list_graph(list_graph_t *list_graph)
{
    vertex_t *curr_vertex;
    vertex_t *del_vertex;

    for (size_t i = 0; i < list_graph->size; i++)
    {
        curr_vertex = list_graph->vertices[i];
        while (curr_vertex != NULL)
        {
            del_vertex = curr_vertex;
            curr_vertex = curr_vertex->next;
            free(del_vertex);
        }

        list_graph->vertices[i] = NULL;
    }
}

void
free_list_graph(list_graph_t *list_graph)
{
    clear_list_graph(list_graph);
    free(list_graph->vertices);
    free(list_graph);
}

int
scan_list_graph_vertex(FILE *file,
                       list_graph_t *list_graph)
{
    int rc;
    int number_from;
    int number_to;
    int index_from;
    int index_to;
    char ch;
    vertex_t *head_vertex_to;
    vertex_t *vertex_from;
    vertex_t *vertex_to;

    rc = scan_integer_file(file, &number_from, 1, list_graph->size);
    if (rc != EXIT_SUCCESS)
        return rc;
    index_from = number_from - 1;

    if (list_graph->vertices[index_from] == NULL)
    {
        vertex_from = malloc(sizeof(vertex_t));
        if (vertex_from == NULL)
        {
            fputs("Ошибка выделения памяти под новую вершину\n", stderr);
            return ERR_NO_MEMORY;
        }
        vertex_from->number = number_from;
        vertex_from->next = NULL;
        list_graph->vertices[index_from] = vertex_from;
    }

    ch = fgetc(file);
    if (ch == '\n')
    {
        return EXIT_SUCCESS;
    }
    else if (ch != ' ')
    {
        fputs("Файл повреждён\n", stderr);
        return ERR_FILE_BROKEN;
    }

    rc = scan_integer_file(file, &number_to, 1, list_graph->size);
    if (rc != EXIT_SUCCESS)
        return rc;
    index_to = number_to - 1;

    if (list_graph->vertices[index_to] == NULL)
    {
        head_vertex_to = malloc(sizeof(vertex_t));
        if (head_vertex_to == NULL)
        {
            fputs("Ошибка выделения памяти под новую вершину\n", stderr);
            return ERR_NO_MEMORY;
        }
        head_vertex_to->number = number_to;
        head_vertex_to->next = NULL;
        list_graph->vertices[index_to] = head_vertex_to;
    }

    vertex_to = malloc(sizeof(vertex_t));
    if (vertex_to == NULL)
    {
        fputs("Ошибка выделения памяти под новую вершину\n", stderr);
        return ERR_NO_MEMORY;
    }

    vertex_to->number = number_to;
    vertex_to->next = list_graph->vertices[index_from]->next;
    list_graph->vertices[index_from]->next = vertex_to;

    return EXIT_SUCCESS;
}

list_graph_t *
list_graph_from_file(char *filename)
{
    list_graph_t *list_graph;
    FILE *file;
    long size;
    int rc;
    size_t counter;

    file = fopen(filename, READ_MODE);
    if (file == NULL)
    {
        fputs("Ошибка открытия файла\n", stderr);
        return NULL;
    }

    rc = fscanf(file, "%ld", &size);
    if (rc != 1)
    {
        fputs("Ошибка ввода количества вершин\n", stderr);
        fclose(file);
        return NULL;
    }

    if (size <= 0)
    {
        fputs("Граф должен содержать минимум одну вершину\n", stderr);
    }

    list_graph = new_list_graph(size);
    if (list_graph == NULL)
    {
        fclose(file);
        return NULL;
    }

    // puts("\nВводите связи между вершинами.\n"
    //      "В каждой строке должно располагаться от одного до двух индексов вершин\n");
    counter = 0;
    do
    {
        rc = scan_list_graph_vertex(file, list_graph);
        counter++;
    }
    while (rc == EXIT_SUCCESS);

    rc = fclose(file);
    if (rc == EOF)
    {
        fputs("Ошибка закрытия файла\n", stderr);
        free_list_graph(list_graph);
        return NULL;
    }

    if (counter == 1)
    {
        fputs("Не было введено ни одной вершины\n", stderr);
        free_list_graph(list_graph);
        return NULL;
    }

    return list_graph;
}

int
write_list_graph_connections(FILE *file,
                             void *graph,
                             void *arg)
{
    list_graph_t *list_graph = (list_graph_t *)graph;
    vertex_t *from_vertex;
    vertex_t *to_vertex;
    int rc;
    int *min_paths = (int *)arg;
    char *from_color;
    char *from_style;

    for (size_t i = 0; i < list_graph->size; i++)
    {
        from_vertex = list_graph->vertices[i];

        if (from_vertex == NULL)
            continue;

        to_vertex = from_vertex->next;

        if (min_paths != NULL)
        {
            from_style = "";
            from_color = "";
            if (min_paths[from_vertex->number - 1] == INT_MAX)
            {
                from_style = FILLED;
                from_color = ORANGE;
            }
            rc = fprintf(file, "%d [style=\"%s\", fillcolor=\"%s\"];\n",
                         from_vertex->number, from_style, from_color);
        }

        if (to_vertex == NULL)
        {
            rc = fprintf(file, "%d;\n", from_vertex->number);
            if (rc == EOF)
                return rc;
        }

        while (to_vertex != NULL)
        {
            rc = fprintf(file, "%d -> %d;\n",
                         from_vertex->number, to_vertex->number);
            if (rc == EOF)
                return rc;
            to_vertex = to_vertex->next;
        }
    }

    return EXIT_SUCCESS;
}

int
find_list_graph_min_paths(list_graph_t *list_graph,
                          int start_number,
                          int **min_paths)
{
    array_queue_t *array_queue;
    int number;
    int curr_number;
    int rc;
    vertex_t *curr_vertex;

    if (list_graph == NULL)
    {
        fputs("Граф не существует\n", stderr);
        return ERR_NULL_POINTER;
    }

    if (start_number > list_graph->size ||
        list_graph->vertices[start_number - 1] == NULL)
    {
        fputs("В графе нет вершины с таким номером\n", stderr);
        return ERR_NOT_FOUND;
    }

    (*min_paths) = malloc(list_graph->size * sizeof(int));
    if ((*min_paths) == NULL)
    {
        fputs("Ошибка выделения памяти под массив минимальных расстояний\n", stderr);
        return ERR_NO_MEMORY;
    }

    for (size_t i = 0; i < list_graph->size; i++)
        (*min_paths)[i] = INT_MAX;
    (*min_paths)[start_number - 1] = 0;

    array_queue = new_array_queue(INITIAL_QUEUE_SIZE);
    if (array_queue == NULL)
    {
        fputs("Ошибка выделения памяти под очередь\n", stderr);
        free(min_paths);
        return ERR_NO_MEMORY;
    }

    number = start_number;

    do
    {
        curr_vertex = list_graph->vertices[number - 1];
        while (curr_vertex != NULL)
        {
            curr_number = curr_vertex->number;
            if ((*min_paths)[curr_number - 1] == INT_MAX)
            {
                (*min_paths)[curr_number - 1] = (*min_paths)[number - 1] + 1;
                rc = array_queue_push(array_queue, curr_number);
                if (rc != EXIT_SUCCESS)
                {
                    free(min_paths);
                    free_array_queue(array_queue);
                    return ERR_NO_MEMORY;
                }
            }
            curr_vertex = curr_vertex->next;
        }
        rc = array_queue_pop(array_queue, &number);
    }
    while (rc == EXIT_SUCCESS);

    free_array_queue(array_queue);

    return EXIT_SUCCESS;
}
