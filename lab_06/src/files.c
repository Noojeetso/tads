#include "files.h"

size_t
get_records_amount(FILE *file)
{
    size_t count = 0;
    int tmp_num;
    long old_pos = ftell(file);
    fseek(file, 0, SEEK_SET);
    printf("pos: %lu\n", ftell(file));
    while (fscanf(file, "%d", &tmp_num) == 1)
    {
        printf("num %d\n", tmp_num);
        count++;
    }
    fseek(file, old_pos, SEEK_SET);
    return count;
}

int
array_search_number(int *array, int arr_size, int value, size_t *index)
{
    for (size_t i = 0; i < arr_size; i++)
    {
        printf("array[%lu] = %d\n", i, array[i]);
        if (array[i] == value)
        {
            *index = i;
            return EXIT_SUCCESS;
        }
    }

    return EXIT_FAILURE;
}

int
file_push(char *file_name,
          int value)
{
    int rc;
    int tmp_num;
    size_t found_index;
    size_t arr_size;
    size_t fill_index;
    FILE *file;
    char buffer[12];

    file = fopen(file_name, READ_WRITE_MODE);
    if (file == NULL)
    {
        fputs("Не удалось открыть файл на чтение и запись\n", stderr);
        return ERR_OPEN_FILE;
    }

    arr_size = get_records_amount(file);
    fill_index = 0;
    int *array = malloc(arr_size * sizeof(int));

    while (fscanf(file, "%d", &tmp_num) == 1)
        array[fill_index++] = tmp_num;

    rc = array_search_number(array, arr_size, value, &found_index);
    free(array);
    if (rc == EXIT_SUCCESS)
    {
        fputs("Число уже есть в файле\n", stderr);
        return ERR_NOT_FOUND;
    }

    sprintf(buffer, "%d", value);
    fwrite(buffer, strlen(buffer), 1, file);

    fclose(file);

    return EXIT_SUCCESS;
}

void
rewrite_file(FILE *file,
             int *array,
             int arr_size)
{
    char buffer[12];
    fseek(file, 0, SEEK_SET);
    for (size_t i = 0; i < arr_size; i++)
    {
        sprintf(buffer, "%d\n", array[i]);
        fwrite(buffer, strlen(buffer), 1, file);
    }
}

int
file_pop(char *file_name,
         int value)
{
    int rc;
    int tmp_num;
    size_t found_index;
    size_t arr_size;
    size_t fill_index;
    FILE *file;

    file = fopen(file_name, READ_WRITE_MODE);
    if (file == NULL)
    {
        fputs("Не удалось открыть файл на чтение и запись\n", stderr);
        return ERR_OPEN_FILE;
    }

    arr_size = get_records_amount(file);
    printf("arr size: %lu\n", arr_size);
    fill_index = 0;
    int *array = malloc((arr_size + 1) * sizeof(int));

    while (fscanf(file, "%d", &tmp_num) == 1)
        array[fill_index++] = tmp_num;

    rc = array_search_number(array, arr_size, value, &found_index);
    if (rc != EXIT_SUCCESS)
    {
        fputs("Число не найдено в файле\n", stderr);
        return ERR_NOT_FOUND;
    }

    puts("");
    for (size_t i = 0; i < arr_size; i++)
        printf("array[%lu] = %d\n", i, array[i]);
    puts("");

    for (size_t i = found_index; i < arr_size - 1; i++)
        array[i] = array[i + 1];
    arr_size--;

    puts("");
    for (size_t i = 0; i < arr_size; i++)
        printf("array[%lu] = %d\n", i, array[i]);
    puts("");

    rewrite_file(file, array, arr_size);

    fclose(file);

    return EXIT_SUCCESS;
}
