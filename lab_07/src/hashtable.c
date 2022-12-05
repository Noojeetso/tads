#include "hashtable.h"

size_t hashtable_comparisons;

enum record_states {RECORD_EMPTY, RECORD_EXISTS, RECORD_DELETED};

typedef struct hash_node
{
    int value;
    struct hash_node *next;
} hash_node_t;

struct hashtable_opened
{
    hash_node_t **data;
    bool *exists;
    size_t records_amount;
    size_t max_size;
};

struct hashtable_closed
{
    int *data;
    unsigned char *states;
    size_t records_amount;
    size_t max_size;
};

size_t
get_next_prime(size_t number)
{
    int div_count;

    do
    {
        div_count = 0;
        number++;
        for(int i = 2; i < number - 1; i++)
            if ((number % i) == 0)
                div_count++;
    }
    while (div_count != 0);

    return number;
}

hashtable_opened_t *
new_hashtable_opened(size_t max_size)
{
    hash_node_t *new_node;
    hashtable_opened_t *hashtable;

    hashtable = malloc(sizeof(hashtable_opened_t));
    if (hashtable == NULL)
    {
        fputs("Ошибка выделения памяти под структуру открытой хэш-таблицы\n", stderr);
        return NULL;
    }

    max_size = get_next_prime(max_size);

    hashtable->max_size = 0;

    hashtable->data = malloc(max_size * sizeof(hash_node_t *));
    if (hashtable->data == NULL)
    {
        fputs("Ошибка выделения памяти под массив записей открытой хэш-таблицы\n", stderr);
        free_table_opened(hashtable);
        return NULL;
    }

    hashtable->exists = malloc(max_size * sizeof(bool));
    if (hashtable->data == NULL)
    {
        fputs("Ошибка выделения памяти под ключи открытой хэш-таблицы\n", stderr);
        free_table_opened(hashtable);
        return NULL;
    }

    for (size_t i = 0; i < max_size; i++)
    {
        new_node = malloc(sizeof(hash_node_t));
        if (new_node == NULL)
        {
            fputs("Ошибка выделения памяти под новую запись открытой хэш-таблицы\n", stderr);
            hashtable->max_size = i;
            free_table_opened(hashtable);
            return NULL;
        }
        new_node->value = 0;
        new_node->next = NULL;
        hashtable->data[i] = new_node;
        hashtable->exists[i] = false;
    }

    hashtable->records_amount = 0;
    hashtable->max_size = max_size;

    return hashtable;
}

void
clear_table_opened(hashtable_opened_t *hashtable)
{
    hash_node_t *curr_node;
    hash_node_t *del_node;
    for (size_t i = 0; i < hashtable->max_size; i++)
    {
        curr_node = hashtable->data[i];
        curr_node = curr_node->next;
        while (curr_node != NULL)
        {
            del_node = curr_node;
            curr_node = curr_node->next;
            free(del_node);
        }
    }
}

void
free_table_opened(hashtable_opened_t *hashtable)
{
    clear_table_opened(hashtable);
    for (size_t i = 0; i < hashtable->max_size; i++)
    {
        free(hashtable->data[i]);
    }
    free(hashtable->data);
    free(hashtable->exists);
    free(hashtable);
}

hashtable_closed_t *
new_hashtable_closed(size_t max_size)
{
    hashtable_closed_t *hashtable = malloc(sizeof(hashtable_closed_t));
    if (hashtable == NULL)
    {
        fputs("Ошибка выделения памяти под структуру закрытой хэш-таблицы\n", stderr);
        return NULL;
    }

    max_size = get_next_prime(max_size);

    hashtable->data = malloc(max_size * sizeof(int));
    if (hashtable->data == NULL)
    {
        fputs("Ошибка выделения памяти под массив записей закрытой хэш-таблицы\n", stderr);
        free_table_closed(hashtable);
        return NULL;
    }

    hashtable->states = malloc(max_size * sizeof(unsigned char));
    if (hashtable->data == NULL)
    {
        fputs("Ошибка выделения памяти под ключи закрытой хэш-таблицы\n", stderr);
        free_table_closed(hashtable);
        return NULL;
    }

    for (size_t i = 0; i < max_size; i++)
    {
        hashtable->states[i] = RECORD_EMPTY;
    }

    hashtable->records_amount = 0;
    hashtable->max_size = max_size;

    return hashtable;
}

void
clear_table_closed(hashtable_closed_t *hashtable)
{
    for (size_t i = 0; i < hashtable->max_size; i++)
    {
        hashtable->states[i] = RECORD_EMPTY;
    }
}

void
free_table_closed(hashtable_closed_t *hashtable)
{
    clear_table_closed(hashtable);
    free(hashtable->data);
    free(hashtable->states);
    free(hashtable);
}

size_t
get_hash_opened(int key,
                size_t max_size)
{
    return key % max_size;
}

size_t
get_hash_closed(int key,
                size_t max_size,
                size_t step,
                size_t iterations)
{
    return (key + iterations * step) % max_size;
}

int
push_table_opened(hashtable_opened_t *hashtable,
                  int value)
{
    hash_node_t *new_node;
    hash_node_t *curr_node;
    size_t hash = get_hash_opened(value, hashtable->max_size);

    if (hashtable_opened_find(hashtable, value) == EXIT_SUCCESS)
    {
        fputs("Элемент уже находится в открытой хеш-таблице\n", stderr);
        return ERR_ALREADY_EXISTS;
    }

    if (hashtable->exists[hash])
    {
        new_node = malloc(sizeof(hash_node_t));
        if (new_node == NULL)
        {
            fputs("Ошибка выделения памяти под узлы открытой хэш-таблицы\n", stderr);
            return ERR_NO_MEMORY;
        }
        new_node->value = value;
        new_node->next = NULL;

        curr_node = hashtable->data[hash];
        while (curr_node->next != NULL)
        {
            curr_node = curr_node->next;
        }
        curr_node->next = new_node;
    }
    else
    {
        hashtable->data[hash]->value = value;
        hashtable->exists[hash] = true;
    }
    hashtable->records_amount++;

    return EXIT_SUCCESS;
}

int
push_table_closed(hashtable_closed_t *hashtable,
                  int value)
{

    size_t hash;
    int iterations = 0;
    int step = hashtable->max_size / HASHTABLE_STEP_DIVISION;

    if (hashtable_closed_find(hashtable, value) == EXIT_SUCCESS)
    {
        fputs("Элемент уже находится в закрытой хеш-таблице\n", stderr);
        return ERR_ALREADY_EXISTS;
    }

    if (hashtable->records_amount == hashtable->max_size)
    {
        fputs("Закрытая хэш-таблица переполнена\n", stderr);
        return ERR_HASHTABLE_OVERFLOW;
    }

    do
    {
        hash = get_hash_closed(value, hashtable->max_size, step, iterations++);
    }
    while (hashtable->states[hash] == RECORD_EXISTS);

    hashtable->data[hash] = value;
    hashtable->states[hash] = RECORD_EXISTS;
    hashtable->records_amount++;

    return EXIT_SUCCESS;
}

int
fill_hashtable_opened(FILE *input_file,
                      hashtable_opened_t *hashtable)
{
    int rc;
    char raw_str[100];
    char *end_pointer;
    int value;

    while (fgets(raw_str, 100, input_file) != NULL)
    {
        value = strtol(raw_str, &end_pointer, 10);
        if (*end_pointer != '\n')
        {
            fputs("Ошибка считывания значения новой записи открытой хэш-таблицы\n", stderr);
            return ERR_READING_VALUE;
        }
        if (raw_str[0] == '\n')
            return EXIT_SUCCESS;

        rc = push_table_opened(hashtable, value);
        if (rc != EXIT_SUCCESS)
            return rc;
    }

    return EXIT_SUCCESS;
}

int
fill_hashtable_closed(FILE *input_file,
                      hashtable_closed_t *hashtable)
{
    int rc;
    char raw_str[100];
    char *end_pointer;
    int value;

    while (fgets(raw_str, 100, input_file) != NULL)
    {
        value = strtol(raw_str, &end_pointer, 10);
        if (*end_pointer != '\n')
        {
            fputs("Ошибка считывания значения новой записи закрытой хэш-таблицы\n", stderr);
            return ERR_READING_VALUE;
        }
        if (raw_str[0] == '\n')
            return EXIT_SUCCESS;

        rc = push_table_closed(hashtable, value);
        if (rc != EXIT_SUCCESS)
            return rc;
    }

    return EXIT_SUCCESS;
}

int
hashtable_opened_pop(hashtable_opened_t *hashtable,
                     int value)
{
    size_t hash;
    hash_node_t *prev_node;
    hash_node_t *curr_node;

    hash = get_hash_opened(value, hashtable->max_size);
    if (hashtable->exists[hash] == false)
    {
        puts("Элемент не найден в открытой хэш-таблице");
        return ERR_NOT_FOUND;
    }

    curr_node = hashtable->data[hash];
    if (curr_node->value == value)
    {
        if (curr_node->next == NULL)
            hashtable->exists[hash] = false;
        else
        {
            hashtable->data[hash] = curr_node->next;
            free(curr_node);
        }

        hashtable->records_amount--;

        return EXIT_SUCCESS;
    }

    prev_node = curr_node;
    curr_node = curr_node->next;
    while (curr_node != NULL)
    {
        if (curr_node->value == value)
        {
            prev_node->next = curr_node->next;
            free(curr_node);

            hashtable->records_amount--;

            return EXIT_SUCCESS;
        }

        prev_node = curr_node;
        curr_node = curr_node->next;
    }

    puts("Элемент не найден в открытой хэш-таблице");
    return ERR_NOT_FOUND;
}

int
hashtable_closed_pop(hashtable_closed_t *hashtable,
                     int value)
{
    size_t hash;
    int iterations = 0;
    int step = hashtable->max_size / HASHTABLE_STEP_DIVISION;

    do
    {
        hash = get_hash_closed(value, hashtable->max_size, step, iterations++);
        if (hashtable->states[hash] == RECORD_EMPTY)
            break;
        if (hashtable->states[hash] == RECORD_EXISTS && hashtable->data[hash] == value)
        {
            hashtable->states[hash] = RECORD_DELETED;

            hashtable->records_amount--;

            return EXIT_SUCCESS;
        }
    }
    while (iterations < hashtable->max_size);

    puts("Элемент не найден в закрытой хэш-таблице");
    return ERR_NOT_FOUND;
}

int
hashtable_opened_find(hashtable_opened_t *hashtable,
                      int value)
{
    size_t hash;
    hash_node_t *curr_node;

    hash = get_hash_opened(value, hashtable->max_size);

    if (hashtable->exists[hash] == false)
        return ERR_NOT_FOUND;

    curr_node = hashtable->data[hash];
    while (curr_node != NULL)
    {
        hashtable_comparisons++;
        if (curr_node->value == value)
        {
            return EXIT_SUCCESS;
        }
        curr_node = curr_node->next;
    }

    return ERR_NOT_FOUND;
}

int
hashtable_closed_find(hashtable_closed_t *hashtable,
                      int value)
{
    size_t hash;
    int iterations = 0;
    int step = hashtable->max_size / HASHTABLE_STEP_DIVISION;

    do
    {
        hash = get_hash_closed(value, hashtable->max_size, step, iterations++);
        if (hashtable->states[hash] == RECORD_EMPTY)
        {
            return ERR_NOT_FOUND;
        }
        hashtable_comparisons++;
        if (hashtable->states[hash] == RECORD_EXISTS && hashtable->data[hash] == value)
        {
            return EXIT_SUCCESS;
        }
    }
    while (iterations < hashtable->max_size);

    return ERR_NOT_FOUND;
}

void
hashtable_opened_search(hashtable_opened_t *hashtable,
                        int value)
{
    int rc;
    rc = hashtable_opened_find(hashtable, value);
    if (rc == EXIT_SUCCESS)
        printf("Элемент найден: %d\n", value);
    else
        puts("Элемент не найден");
}

void
hashtable_closed_search(hashtable_closed_t *hashtable,
                        int value)
{
    int rc;
    rc = hashtable_closed_find(hashtable, value);
    if (rc == EXIT_SUCCESS)
        printf("Элемент найден: %d\n", value);
    else
        puts("Элемент не найден");
}

void
print_hashtable_opened(hashtable_opened_t *hashtable)
{
    hash_node_t *curr_node;

    if (hashtable->records_amount == 0)
    {
        puts("Открытая хэш-таблица пуста");
        return;
    }

    puts("Открытая хэш-таблица:");
    printf("Размер: %lu\n", hashtable->max_size);
    puts("Ключ\tЗначения");
    for (size_t i = 0; i < hashtable->max_size; i++)
    {
        if (hashtable->exists[i] == false)
            continue;

        curr_node = hashtable->data[i];

        printf("%lu\t|%d", i, curr_node->value);

        curr_node = curr_node->next;
        while (curr_node != NULL)
        {
            printf(",\t%d", curr_node->value);
            curr_node = curr_node->next;
        }
        putc('\n', stdout);
    }
}

void
print_hashtable_closed(hashtable_closed_t *hashtable)
{
    size_t deleted_counter = 0;
    if (hashtable->records_amount == 0)
    {
        puts("Закрытая хэш-таблица пуста");
        return;
    }

    puts("Закрытая хэш-таблица:");
    printf("Размер: %lu\n", hashtable->max_size);
    puts("Ключ\tЗначение");
    for (size_t i = 0; i < hashtable->max_size; i++)
    {
        deleted_counter += hashtable->states[i] == RECORD_DELETED;
        if (hashtable->states[i] != RECORD_EXISTS)
            continue;
        printf("%lu\t|%d\n", i, hashtable->data[i]);
    }

    if (deleted_counter != 0)
    {
        puts("Помеченные как удалённые:");
        for (size_t i = 0; i < hashtable->max_size; i++)
        {
            if (hashtable->states[i] != RECORD_DELETED)
                continue;
            printf("%lu\t|%d\n", i, hashtable->data[i]);
        }
    }
}

size_t
get_hashtable_opened_size(hashtable_opened_t *hashtable)
{
    return hashtable->max_size;
}

size_t
get_hashtable_closed_size(hashtable_closed_t *hashtable)
{
    return hashtable->max_size;
}

size_t
get_hashtable_opened_size_bytes(hashtable_opened_t *hashtable)
{
    hash_node_t *curr_node;
    size_t size = sizeof(hashtable_opened_t);
    size += hashtable->max_size * sizeof(hash_node_t *);
    size += hashtable->max_size * sizeof(hash_node_t);

    for (size_t i = 0; i < hashtable->max_size; i++)
    {
        if (hashtable->exists[i] == false)
            continue;

        curr_node = hashtable->data[i];
        curr_node = curr_node->next;
        while (curr_node != NULL)
        {
            size += sizeof(hash_node_t);
            curr_node = curr_node->next;
        }
    }

    size += hashtable->max_size * sizeof(bool);

    return size;
}

size_t
get_hashtable_closed_size_bytes(hashtable_closed_t *hashtable)
{
    size_t size = sizeof(hashtable_closed_t);
    size += hashtable->max_size * sizeof(int);
    size += hashtable->max_size * sizeof(unsigned char);
    return size;
}

size_t
get_hashtable_comparisons(void)
{
    return hashtable_comparisons;
}

void
set_hashtable_comparisons(size_t new_comparisons)
{
    hashtable_comparisons = new_comparisons;
}
