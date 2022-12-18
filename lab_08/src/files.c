#include "files.h"

size_t file_comparisons;

size_t
get_file_comparisons(void)
{
    return file_comparisons;
}

void
set_file_comparisons(size_t new_comparisons)
{
    file_comparisons = new_comparisons;
}

size_t
get_array_size_bytes(array_t *array)
{
    size_t size = sizeof(array_t);
    size += array->max_size * sizeof(int);
    return size;
}

array_t *
new_array(size_t size)
{
    array_t *new_array = malloc(sizeof(array_t));
    if (new_array == NULL)
        return NULL;
    new_array->data = malloc(size * sizeof(int));
    if (new_array->data == NULL)
    {
        free(new_array);
        return NULL;
    }
    new_array->size = 0;
    new_array->max_size = size;
    return new_array;
}

void
free_array(array_t *array)
{
    free(array->data);
    free(array);
}

size_t
get_records_amount(FILE *file)
{
    size_t count = 0;
    int tmp_num;
    long old_pos = ftell(file);
    fseek(file, 0, SEEK_SET);
    while (fscanf(file, "%d", &tmp_num) == 1)
        count++;
    fseek(file, old_pos, SEEK_SET);
    return count;
}

int
array_search_number(array_t *array,
                    int value,
                    size_t *index)
{
    for (size_t i = 0; i < array->size; i++)
    {
        file_comparisons++;
        if (array->data[i] == value)
        {
            *index = i;
            return EXIT_SUCCESS;
        }
    }

    return EXIT_FAILURE;
}

array_t *
file_to_array(const char *file_name)
{
    FILE *file;
    size_t arr_size;
    array_t *array;
    int value;

    file = fopen(file_name, READ_MODE);
    if (file == NULL)
    {
        fputs("Не удалось открыть файл на чтение и запись\n", stderr);
        return NULL;
    }

    arr_size = get_records_amount(file);
    array = new_array(arr_size);
    if (array == NULL)
    {
        fputs("Ошибка выделения памяти под массив\n", stderr);
        return NULL;
    }

    fseek(file, 0, SEEK_SET);
    while (fscanf(file, "%d", &value) == 1)
        array->data[array->size++] = value;

    fclose(file);

    return array;
}

int
file_find(char *file_name,
          int value)
{
    int rc;
    size_t found_index;
    array_t *array;

    array = file_to_array(file_name);
    if (array == NULL)
        return ERR_NULL_POINTER;
    rc = array_search_number(array, value, &found_index);

    free_array(array);

    return rc;
}

void
file_search(char *file_name,
            int value)
{
    int rc;

    rc = file_find(file_name, value);
    if (rc != EXIT_SUCCESS)
        puts("Элемент не найден");
    else
        printf("Элемент найден: %d\n", value);
}

int
file_push(char *file_name,
          int value)
{
    int rc;
    size_t found_index;
    char buffer[100];
    FILE *file;

    array_t *array = file_to_array(file_name);
    if (array == NULL)
        return ERR_NULL_POINTER;

    rc = array_search_number(array, value, &found_index);
    free_array(array);
    if (rc == EXIT_SUCCESS)
    {
        fputs("Элемент уже находится в файле\n", stderr);
        return ERR_NOT_FOUND;
    }

    file = fopen(file_name, APPEND_MODE);

    sprintf(buffer, "%d\n", value);
    fwrite(buffer, strlen(buffer), 1, file);

    fclose(file);

    return EXIT_SUCCESS;
}

void
rewrite_file(FILE *file,
             array_t *array)
{
    char buffer[100];
    fseek(file, 0, SEEK_SET);
    for (size_t i = 0; i < array->size; i++)
    {
        sprintf(buffer, "%d\n", array->data[i]);
        fwrite(buffer, strlen(buffer), 1, file);
    }
}

int
file_pop(char *file_name,
         int value)
{
    int rc;
    size_t found_index;
    FILE *file;

    array_t *array = file_to_array(file_name);
    if (array == NULL)
        return ERR_NULL_POINTER;

    rc = array_search_number(array, value, &found_index);
    if (rc != EXIT_SUCCESS)
    {
        fputs("Элемент не найден в файле\n", stderr);
        free_array(array);
        return ERR_NOT_FOUND;
    }

    for (size_t i = found_index; i < array->size - 1; i++)
        array->data[i] = array->data[i + 1];
    array->size--;

    file = fopen(file_name, WRITE_MODE);

    rewrite_file(file, array);

    free_array(array);
    fclose(file);

    return EXIT_SUCCESS;
}
