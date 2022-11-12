#include "table.h"

int
init_record_table(record_table_t *record_table, size_t max_size)
{
    record_table->records = malloc(max_size * sizeof(record_t));
    if (record_table->records == NULL)
    {
        fputs("Ошибка выделения памяти под записи таблицы", stderr);
        return ERR_NO_MEMORY;
    }

    record_table->keys = malloc(max_size * sizeof(key_t));
    if (record_table->keys == NULL)
    {
        fputs("Ошибка выделения памяти под массив ключей таблицы", stderr);
        free(record_table->records);
        return ERR_NO_MEMORY;
    }

    record_table->max_size = max_size;
    record_table->size = 0;
    return EXIT_SUCCESS;
}

void clear_keys(record_table_t *record_table)
{
    for (size_t i = 0; i < record_table->size; i++)
        if (record_table->keys[i].theater_name_long != NULL)
            free(record_table->keys[i].theater_name_long);
}

void
clear_table(record_table_t *record_table)
{
    // clear_keys(record_table);
    for (size_t i = 0; i < record_table->size; i++)
        free_record_safe(&record_table->records[i]);
    record_table->size = 0;
}

void
free_table(record_table_t *record_table)
{
    clear_table(record_table);
    free(record_table->records);
    free(record_table->keys);
}

int
is_table_empty(record_table_t *record_table)
{
    return record_table->size == 0;
}

int
check_table_empty(record_table_t *record_table)
{
    if (is_table_empty(record_table))
    {
        fputs("Таблица пуста\n\n", stderr);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int
is_table_full(record_table_t *record_table)
{
    return record_table->size == record_table->max_size;
}

int
check_table_full(record_table_t *record_table)
{
    if (is_table_full(record_table))
    {
        fputs("Таблица переполнена\n\n", stderr);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

int
delete_by_id(record_table_t *record_table, size_t id)
{
    size_t key_table_id = 0;

    if (id > record_table->size)
    {
        fputs("Не существует записи с таким номером", stderr);
        return ERR_OUT_OF_RANGE;
    }

    id--;

    for (size_t i = 0; i < record_table->size; i++)
        if (record_table->keys[i].id == id)
        {
            key_table_id = i;
            break;
        }

    for (size_t i = key_table_id; i < record_table->size - 1; i++)
        record_table->keys[i] = record_table->keys[i + 1];

    for (size_t i = id; i < record_table->size - 1; i++)
        record_table->records[i] = record_table->records[i + 1];

    record_table->size--;

    puts("Удаление записи завершено");

    return EXIT_SUCCESS;
}

int
delete_by_position(record_table_t *record_table)
{
    long long number;

    printf("%s", "Введите номер удаляемой записи: ");

    if (scanf("%lld", &number) != 1)
    {
        fputs("\nОшибка ввода числа\n", stderr);
        return ERR_INPUT_NUMBER;
    }
    getc(stdin);
    print_newline();

    if (number < 1)
    {
        fputs("Номер записи не может быть меньше единицы\n", stderr);
        return ERR_DOMAIN;
    }

    if (number > record_table->size)
    {
        fputs("Не существует записи с таким номером\n", stderr);
        return ERR_DOMAIN;
    }

    puts("Удаление записи...");

    return delete_by_id(record_table, number);
}

int
delete_by_theater_name(record_table_t *record_table)
{
    int rc;
    char theater_name[SHORT_STRING_MAX_LENGTH + 2];
    char *theater_name_long = NULL;

    puts("Введите название театра:\n");
    rc = scan_string(theater_name, SHORT_STRING_MAX_LENGTH, &theater_name_long, stdin);
    if (rc != EXIT_SUCCESS)
    {
        if (feof(stdin))
        {
            if (theater_name_long != NULL)
                free(theater_name_long);
            return rc;
        }
        if (rc == ERR_EMPTY_STRING)
            fprintf(stderr, "%s\n", "Строка не должна быть пустой");
        if (theater_name_long != NULL)
            free(theater_name_long);
        return rc;
    }

    for (size_t i = 0; i < record_table->size; i++)
    {
        if (theater_name_long == NULL)
        {
            if (strcmp(theater_name, record_table->records[i].theater_name))
                continue;
        }
        else
        {
            if (record_table->records[i].theater_name_long == NULL ||
                strcmp(theater_name_long, record_table->records[i].theater_name_long))
                continue;
        }
        delete_by_id(record_table, i);
        i--;
    }

    return EXIT_SUCCESS;
}

