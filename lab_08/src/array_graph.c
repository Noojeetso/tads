#include "array_graph.h"

struct array_graph
{
    int **matrix;
    size_t size;
};

array_graph_t *
new_array_graph(size_t size)
{
    array_graph_t *new_graph = malloc(sizeof(array_graph_t));
    if (new_graph == NULL)
    {
        fputs("Ошибка выделения памяти под структуру графа в виде списка\n", stderr);
        return NULL;
    }

    new_graph->matrix = malloc(size * sizeof(int *));
    if (new_graph->matrix == NULL)
    {
        fputs("Ошибка выделения памяти под массив указателей на строки матрицы графа в виде массива\n", stderr);
        free(new_graph);
        return NULL;
    }

    for (size_t i = 0; i < size; i++)
    {
        new_graph->matrix[i] = calloc(size, sizeof(int));
        if (new_graph->matrix[i] == NULL)
        {
            fputs("Ошибка выделения памяти под строку матрицы графа в виде массива\n", stderr);
            new_graph->size = i;
            free_array_graph(new_graph);
        }
    }

    new_graph->size = size;

    return new_graph;
}

void
free_array_graph(array_graph_t *array_graph)
{
    for (size_t i = 0; i < array_graph->size; i++)
        free(array_graph->matrix[i]);
    free(array_graph->matrix);
    free(array_graph);
}

int
scan_array_graph_vertex(FILE *file,
                        array_graph_t *array_graph)
{
    int rc;
    int number_from;
    int number_to;
    char ch;

    rc = scan_integer_file(file, &number_from, 1, array_graph->size);
    if (rc != EXIT_SUCCESS)
        return rc;

    array_graph->matrix[number_from - 1][number_from - 1] = 1;

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

    rc = scan_integer_file(file, &number_to, 1, array_graph->size);
    if (rc != EXIT_SUCCESS)
        return rc;

    array_graph->matrix[number_to - 1][number_to - 1] = 1;
    array_graph->matrix[number_from - 1][number_to - 1] = 1;

    return EXIT_SUCCESS;
}

array_graph_t *
array_graph_from_file(char *filename)
{
    array_graph_t *array_graph;
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
        return NULL;
    }

    if (size <= 0)
    {
        fputs("Граф должен содержать минимум одну вершину\n", stderr);
    }

    array_graph = new_array_graph(size);
    if (array_graph == NULL)
        return NULL;

    // puts("\nВводите связи между вершинами.\n"
    //      "В каждой строке должно располагаться от одного до двух индексов вершин\n");
    counter = 0;
    do
    {
        rc = scan_array_graph_vertex(file, array_graph);
        counter++;
    }
    while (rc == EXIT_SUCCESS);

    rc = fclose(file);
    if (rc == EOF)
    {
        fputs("Ошибка закрытия файла\n", stderr);
        free_array_graph(array_graph);
        return NULL;
    }

    if (counter == 1)
    {
        fputs("Не было введено ни одной вершины\n", stderr);
        free_array_graph(array_graph);
        return NULL;
    }

    return array_graph;
}

int
write_array_graph_connections(FILE *file,
                              void *graph,
                              void *arg)
{
    array_graph_t *array_graph = (array_graph_t *)graph;
    int *min_paths = (int *)arg;
    int rc;
    char *from_color;
    char *from_style;

    if (min_paths != NULL)
    {
        for (size_t i = 0; i < array_graph->size; i++)
        {
            if (array_graph->matrix[i][i] == 0)
                continue;
            from_style = "";
            from_color = "";
            if (min_paths[i] == INT_MAX)
            {
                from_style = FILLED;
                from_color = ORANGE;
            }
            rc = fprintf(file, "%lu [style=\"%s\", fillcolor=\"%s\"];\n",
                         i + 1, from_style, from_color);
            if (rc == EOF)
                return rc;
        }
    }

    for (size_t i = 0; i < array_graph->size; i++)
    {
        if (array_graph->matrix[i][i] == 0)
            continue;
        rc = fprintf(file, "%lu;\n", i + 1);
        if (rc == EOF)
            return rc;
    }

    for (size_t i = 0; i < array_graph->size; i++)
        for (size_t j = 0; j < array_graph->size; j++)
        {
            if (array_graph->matrix[i][j] == 0 || i == j)
                continue;

            rc = fprintf(file, "%lu -> %lu;\n", i + 1, j + 1);
            if (rc == EOF)
                return rc;
        }

    return EXIT_SUCCESS;
}

int
find_array_graph_min_paths(array_graph_t *array_graph,
                           int start_number,
                           int **min_paths)
{
    array_queue_t *array_queue;
    int number;
    int rc;

    if (array_graph == NULL)
    {
        fputs("Граф не существует\n", stderr);
        return ERR_NULL_POINTER;
    }

    if (start_number > array_graph->size ||
        array_graph->matrix[start_number - 1][start_number - 1] == 0)
    {
        fputs("В графе нет вершины с таким номером\n", stderr);
        return ERR_NOT_FOUND;
    }

    (*min_paths) = malloc(array_graph->size * sizeof(int));
    if ((*min_paths) == NULL)
    {
        fputs("Ошибка выделения памяти под массив минимальных расстояний\n", stderr);
        return ERR_NO_MEMORY;
    }

    for (size_t i = 0; i < array_graph->size; i++)
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
        for (size_t j = 0; j < array_graph->size; j++)
        {
            if (array_graph->matrix[number - 1][j] == 0)
                continue;

            if ((*min_paths)[j] == INT_MAX)
            {
                (*min_paths)[j] = (*min_paths)[number - 1] + 1;
                rc = array_queue_push(array_queue, j + 1);
                if (rc != EXIT_SUCCESS)
                {
                    free(min_paths);
                    free_array_queue(array_queue);
                    return ERR_NO_MEMORY;
                }
            }
        }
        rc = array_queue_pop(array_queue, &number);
    }
    while (rc == EXIT_SUCCESS);

    free_array_queue(array_queue);

    return EXIT_SUCCESS;
}
