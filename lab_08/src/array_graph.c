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

    rc = scan_integer_file(file, &number_from, 0, array_graph->size);
    if (rc != EXIT_SUCCESS)
        return rc;

    ch = fgetc(file);
    if (ch == '\n')
    {
        array_graph->matrix[number_from - 1][number_from - 1] = 1;
        return EXIT_SUCCESS;
    }
    else if (ch != ' ')
    {
        fputs("Файл повреждён\n", stderr);
        return ERR_FILE_BROKEN;
    }

    rc = scan_integer_file(file, &number_to, 0, array_graph->size);
    if (rc != EXIT_SUCCESS)
        return rc;

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

    puts("\nВводите связи между вершинами.\n"
         "В каждой строке должно располагаться от одного до двух индексов вершин\n");
    counter = 0;
    do
    {
        rc = scan_array_graph_vertex(file, array_graph);
        puts("gere");
        counter++;
    }
    while (rc == EXIT_SUCCESS);

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
                              array_graph_t *array_graph)
{
    int rc;

    for (size_t i = 0; i < array_graph->size; i++)
        for (size_t j = i + 1; j < array_graph->size; j++)
        {
            if (array_graph->matrix[i][j] == 0)
                continue;

            rc = fprintf(file, "%lu -> %lu;\n", j + 1, i + 1);
            if (rc == EOF)
                return rc;
        }

    for (size_t i = 0; i < array_graph->size; i++)
        if (array_graph->matrix[i][i] == 1)
        {
            rc = fprintf(file, "%lu;\n", i + 1);
            if (rc == EOF)
                return rc;
        }

    for (size_t i = 0; i < array_graph->size; i++)
        for (size_t j = i + 1; j < array_graph->size; j++)
        {
            if (array_graph->matrix[j][i] == 0)
                continue;

            rc = fprintf(file, "%lu -> %lu;\n", i + 1, j  + 1);
            if (rc == EOF)
                return rc;
        }

    return EXIT_SUCCESS;
}
