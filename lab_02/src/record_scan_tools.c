#include "record.h"

int
dynamic_string_append(char **input_str,
                      size_t str_length,
                      FILE *input_file)
{
    size_t max_size = str_length * 2;
    int ch;

    while ((ch = fgetc(input_file)) != EOF)
    {
        if (ch == '\n')
            break;
        if (str_length + 1 == max_size)
        {
            char *tmp = realloc(*input_str, max_size * 2);
            if (tmp == NULL)
            {
                DEBUG_PRINT("Ошибка выделения памяти под расширенную строку");
                free(*input_str);
                *input_str = NULL;
                return ERR_NO_MEMORY;
            }
            *input_str = tmp;
            max_size *= 2;
        }
        (*input_str)[str_length++] = (char)ch;
    }

    (*input_str)[str_length] = '\0';

    return EXIT_SUCCESS;
}

int
scan_string(char *short_str,
            size_t short_str_max_length,
            char **long_str,
            FILE *input_file)
{
    if (fgets(short_str, short_str_max_length + 2, input_file) == NULL)
        return ERR_INPUT_STRING;

    size_t str_length = strlen(short_str);\
    if (short_str[str_length - 1] != '\n')
    {
        *long_str = malloc(str_length * 2 * sizeof(char));
        if (*long_str == NULL)
        {
            fprintf(stderr, "%s\n", "Ошибка выделения памяти под расширенную строку");
            return ERR_NO_MEMORY;
        }

        strncpy(*long_str, short_str, str_length);

        return dynamic_string_append(long_str, str_length, input_file);
    }

    short_str[str_length - 1] = '\0';

    return EXIT_SUCCESS;
}

int
scan_non_negative_integer(FILE *input_file,
                          int32_t *out_int,
                          int32_t max_int)
{
    char ch;
    int64_t input_num;

    if (fscanf(input_file, "%ld", &input_num) != 1)
    {
        fprintf(stderr, "%s\n", "Ожидалось целое число");
        return ERR_INPUT_NUMBER;
    }

    ch = fgetc(input_file);
    if (ch != '\n')
    {
        fprintf(stderr, "%s\n", "Ожидалось целое число\\n");
        return ERR_DOMAIN;
    }

    if (input_num < 0)
    {
        fprintf(stderr, "%s\n", "Введённое число меньше нуля");
        return ERR_DOMAIN;
    }

    if (input_num > (uint64_t)max_int)
    {
        fprintf(stderr, "%s: %lu/%u\n", "Введённое число превышает максимальное значение", input_num, max_int);
        return ERR_DOMAIN;
    }

    *out_int = input_num;

    return EXIT_SUCCESS;
}

