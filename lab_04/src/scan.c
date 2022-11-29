#include "scan.h"

int
get_value(char *buffer,
          size_t buffer_size,
          int *value)
{
    size_t str_length;
    char *endptr;

    if (fgets(buffer, buffer_size, stdin) == NULL)
        return EXIT_FAILURE;
    str_length = strlen(buffer);

    if (str_length == 1)
        return EOF;

    if (buffer[str_length - 1] != '\n')
    {
        fputs("Ошибка считывания числа\n", stderr);
        return EXIT_FAILURE;
    }
    buffer[str_length - 1] = '\0';

    *value = strtol(buffer, &endptr, 10);

    if (endptr - buffer != strlen(buffer))
    {
        fputs("Ошибка считывания числа\n", stderr);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int *
scan_array(int *array_size)
{
    int rc;

    size_t max_array_size = 10;
    *array_size = 0;
    int *output_array = malloc(max_array_size * sizeof(int));
    int value;

    size_t buffer_size = log(INT_MAX) / log(10) + 4;
    char *buffer = malloc(buffer_size);

    rc = get_value(buffer, buffer_size, &value);
    while (rc == EXIT_SUCCESS)
    {
        if (*array_size == max_array_size)
        {
            output_array = realloc(output_array, max_array_size * 2 * sizeof(int));
            max_array_size *= 2;
        }
        output_array[(*array_size)++] = value;
        rc = get_value(buffer, buffer_size, &value);
    }
    free(buffer);

    return output_array;
}
