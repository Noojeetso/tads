#include "table.h"

void
print_table_key(key_t *key,
                size_t index,
                int offset)
{
    printf("%3lu |", index);
    print_integer_centered(key->id + 1, 17, ' ');

    char *printing_string = key->theater_name;
    if (key->theater_name_long != NULL)
        printing_string = key->theater_name_long;
    unicode_format_print(printing_string, 24, offset);
    print_newline();
}

void
print_table_keys(record_table_t *record_table,
                 size_t start_index,
                 size_t max_rows,
                 int offset)
{
    unicode_format_print("№", 6, 0);
    unicode_format_print("Номер в таблице", 17, 0);
    unicode_format_print("Название театра", 24, 0);
    print_newline();

    for (size_t i = start_index; i < max_rows + start_index; i++)
    {
        print_hyphen_line(48);
        print_table_key(&record_table->keys[i], i + 1, offset);
    }
    print_hyphen_line(48);
    print_newline();
}

void
print_header()
{
    unicode_format_print("№", 6, 0);
    unicode_format_print("Название театра", 24, 0);
    unicode_format_print("Название постановки", 25, 0);
    unicode_format_print("Режиссёр-постановщик", 25, 0);
    unicode_format_print("Диапазон цен", 15, 0);
    unicode_format_print("Спектакль", 13, 0);
    unicode_format_print("Представление", 15, 0);
    unicode_format_print("Возраст ребёнка", 17, 0);
    unicode_format_print("Композитор", 25, 0);
    unicode_format_print("Страна", 8, 0);
    unicode_format_print("Возраст", 9, 0);
    unicode_format_print("Время", 7, 0);
    print_newline();
}

void
print_table(record_table_t *record_table,
            size_t start_index,
            size_t max_rows,
            int offset)
{
    print_header();

    for (size_t i = start_index; i < max_rows + start_index; i++)
    {
        print_hyphen_line(199);
        print_record(&record_table->records[i], i + 1, offset);
    }
    print_hyphen_line(199);
}

void
print_table_with_keys(record_table_t *record_table,
                      size_t start_index,
                      size_t max_rows,
                      int offset)
{
    print_header();

    for (size_t i = start_index; i < max_rows + start_index; i++)
    {
        print_hyphen_line(199);
        print_record(&record_table->records[record_table->keys[i].id], i + 1, offset);
    }
    print_hyphen_line(199);
}

int
print_musicals_filtered(print_arguments_t *print_arguments)
{
    int rc;
    int32_t min_age;
    int32_t max_duration;
    int printed_num = 0;
    record_t *record;
    record_table_t *record_table = print_arguments->record_table;
    record_table_t extra_record_table;
    print_arguments_t filter_print_arguments = {&extra_record_table, print_table, '\0'};
    size_t record_num = record_table->size;

    rc = init_record_table(&extra_record_table, MAIN_TABLE_SIZE);
    if (rc != EXIT_SUCCESS)
        return rc;

    printf("%s\n", "Введите возраст ребёнка (до 18):");
    rc = scan_non_negative_integer(stdin, &min_age, 100);
    if (rc != EXIT_SUCCESS)
        return rc;

    printf("Введите продолжительность спектакля в минутах (до %d)\n", INT_MAX);
    rc = scan_non_negative_integer(stdin, &max_duration, INT_MAX);
    if (rc != EXIT_SUCCESS)
        return rc;

    for (size_t i = 0; i < record_num; i++)
    {
        record = &record_table->records[i];
        if (record->show_kind != MUSICAL ||
            record->play_group.musical.min_age != min_age ||
            record->play_group.musical.duration >= max_duration)
            continue;

        if (!printed_num)
            print_header();

        memcpy(&extra_record_table.records[extra_record_table.size++], record, sizeof(record_t));
    }

    if (extra_record_table.size > 0)
        print_horizontal_scroll(&filter_print_arguments);
    else
    {
        fputs("\nНе найдено подходящих спектаклей\n\n", stderr);
        return ERR_NOT_FOUND;
    }

    return EXIT_SUCCESS;
}

// Инициализация настроек терминала
static struct termios old, new;
void initTermios(int echo)
{
    tcgetattr(0, &old); // Получение старых настроек терминала
    new = old; // Копирование старых настроек терминала
    new.c_lflag &= ~ICANON; // Отключение буфера ввода/вывода
    new.c_lflag &= echo ? ECHO : ~ECHO; // Отключение печати вводимых символов
    tcsetattr(0, TCSANOW, &new); // Установка новых настроек терминала
}

// Восстановление старых настроек терминала
void resetTermios(void)
{
    tcsetattr(0, TCSANOW, &old);
}

void
scan_key(char *current_char)
{
    initTermios(0);
    char ch = '\0';
    while (ch != 'q')
    {
        read(STDIN_FILENO, &ch, 1);
        *current_char = ch;
    }
    resetTermios();
}

void
*print_thread(void *print_arguments_p)
{
    char current_char;
    print_arguments_t *print_arguments = print_arguments_p;
    record_table_t *record_table = print_arguments->record_table;
    // Структура содержащая время задержки
    struct timespec tw = {0, 125000000};
    // Структура, в которую будет помещен остаток времени задержки, если задержка будет прервана досрочно
    struct timespec tr;
    struct winsize window;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);
    short terminal_rows = window.ws_row;
    int delta_rows;

    struct timeval stop, start;
    gettimeofday(&start, NULL);
    long unsigned start_seconds = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
    long unsigned delta_seconds;

    size_t start_index = 0;
    size_t max_rows = 0;
    int offset = 0;
    int run = 1;
    while (run)
    {
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &window);
        terminal_rows = window.ws_row;
        delta_rows = (start_index + terminal_rows / 2 - 2) - record_table->size;
        if (delta_rows > 0)
        {
            if (delta_rows > start_index)
                delta_rows = start_index;
            start_index -= delta_rows;
        }

        gettimeofday(&stop, NULL);
        delta_seconds = stop.tv_sec * 1000000 + stop.tv_usec - start_seconds;
        if (delta_seconds >= 120000)
        {
            start_seconds += delta_seconds;
            offset++;
        }

        current_char = print_arguments->current_char;
        if (current_char != '\0')
        {
            switch (current_char)
            {
                case 'k':
                    if (start_index + terminal_rows / 2 - 2 < record_table->size)
                        start_index++;
                    break;
                case 'j':
                    if (start_index > 0)
                        start_index--;
                    break;
                case 'q':
                    run = 0;
                    break;
            }
            print_arguments->current_char = '\0';
        }
        system("clear");

        max_rows = terminal_rows / 2 - 2;
        if (record_table->size < max_rows)
            max_rows = record_table->size;

        print_arguments->print_function(record_table, start_index, max_rows, offset);

        puts("q - Выйти      j - Переместиться на строку ввверх      k - Перместиться на строку вниз");

        nanosleep(&tw, &tr);
    }
    system("clear");

    return NULL;
}

void
print_horizontal_scroll(print_arguments_t *print_arguments)
{
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, print_thread, print_arguments);
    scan_key(&print_arguments->current_char);
    pthread_join(thread_id, NULL);
}

int
add_record(record_table_t *record_table,
           FILE *input_file,
           int (*prompt)(const char *const format, ...))
{
    int rc;
    record_t *new_record;

    rc = check_table_full(record_table);
    if (rc != EXIT_SUCCESS)
        return rc;

    new_record = &record_table->records[record_table->size];

    rc = scan_record(new_record, input_file, prompt);
    if (rc != EXIT_SUCCESS)
        return rc;

    record_table->size++;

    return EXIT_SUCCESS;
}

void
add_key(record_table_t *record_table)
{
    size_t key_index = record_table->size - 1;
    key_t *key = &record_table->keys[key_index];
    record_t *record = &record_table->records[key_index];

    key->id = key_index;

    *key->theater_name = '\0';
    key->theater_name_long = NULL;

    if (record_table->records[key_index].theater_name_long == NULL)
        memcpy(key->theater_name, record->theater_name, strlen(record->theater_name));
    else
        key->theater_name_long = record->theater_name_long;
}

int
dummy_prompt(const char *const format,
             ...)
{
    return format[0];
}

int
add_records_from_file(record_table_t *record_table,
                      char *file_name)
{
    int rc;
    int ch;
    FILE *file;

    file = fopen(file_name, "r");
    if (file == NULL)
    {
        fprintf(stderr, "%s\n", "Не удалось открыть файл");
        return ERR_OPEN_FILE;
    }

    do
    {
        rc = add_record(record_table, file, dummy_prompt);
        if (rc != EXIT_SUCCESS)
        {
            fclose(file);
            return rc;
        }

        add_key(record_table);

        ch = getc(file);
        if (ch == EOF)
            break;
        if (ch != '\n')
        {
            fprintf(stderr, "%s\n", "Структура файла повреждена");
            fclose(file);
            return ERR_FILE_BROKEN;
        }
    }
    while (rc == EXIT_SUCCESS);

    fgetc(file);
    if (!feof(file))
    {
        fprintf(stderr, "%s\n", "Структура файла повреждена 2");
        return ERR_FILE_BROKEN;
    }

    rc = fclose(file);
    if (rc == EOF)
    {
        fprintf(stderr, "%s\n", "Не удалось закрыть файл");
        return ERR_CLOSE_FILE;
    }

    return EXIT_SUCCESS;
}

int
add_record_from_console(record_table_t *record_table)
{
    int rc;

    if (check_table_full(record_table))
        return ERR_TABLE_FULL;

    rc = add_record(record_table, stdin, printf);
    if (rc == EXIT_SUCCESS)
        add_key(record_table);
    else if (rc == ERR_EMPTY_STRING)
        fprintf(stderr, "%s\n", "Строка не должна быть пустой");

    return rc;
}

