#include "record.h"

play_type_t
scan_play_type(FILE *input_file)
{
    int rc;
    char type_name[MAX_TYPE_NAME_LENGTH + 2];

    if (fgets(type_name, MAX_TYPE_NAME_LENGTH + 2, input_file) == NULL)
    {
        fprintf(stderr, "%s\n", "Ошибка считывания типа спектакля");
        return ERR_INPUT_STRING;
    }

    size_t str_length = strlen(type_name);
    if (type_name[str_length - 1] != '\n')
    {
        fprintf(stderr, "%s\n", "Введена слишком длинная строка");
        return ERR_STRING_TOO_LONG;
    }
    type_name[str_length - 1] = '\0';

    rc = unicode_str_tolower(type_name);
    if (rc != EXIT_SUCCESS)
        return ERR_TYPE;

    if (!strcmp(type_name, FAIRY_TALE_STR))
        return FAIRY_TALE;
    if (!strcmp(type_name, PLAY_STR))
        return PLAY;
    if (!strcmp(type_name, DRAMA_STR))
        return DRAMA;
    if (!strcmp(type_name, COMEDY_STR))
        return COMEDY;
    return ERR_TYPE;
}

show_kind_t
scan_show_kind(FILE *input_file)
{
    int rc;
    char kind_name[MAX_TYPE_NAME_LENGTH + 2];

    if (fgets(kind_name, MAX_TYPE_NAME_LENGTH + 2, input_file) == NULL)
    {
        fprintf(stderr, "%s\n", "Ошибка считывания типа спектакля");
        return ERR_INPUT_STRING;
    }

    size_t str_length = strlen(kind_name);
    if (kind_name[str_length - 1] != '\n')
    {
        fprintf(stderr, "%s\n", "Введена слишком длинная строка");
        return ERR_STRING_TOO_LONG;
    }
    kind_name[str_length - 1] = '\0';

    rc = unicode_str_tolower(kind_name);
    if (rc != EXIT_SUCCESS)
        return ERR_KIND;

    if (!strcmp(kind_name, CHILD_STR))
        return CHILD;
    if (!strcmp(kind_name, ADULT_STR))
        return ADULT;
    if (!strcmp(kind_name, MUSICAL_STR))
        return MUSICAL;

    return ERR_KIND;
}

int
scan_record(record_t *new_record,
           FILE *input_file,
           int (*prompt)(const char *const format, ...))
{
    int rc;

    new_record->theater_name_long = NULL;
    new_record->play_name_long = NULL;
    new_record->producer_long = NULL;
    new_record->play_group.musical.composer_long = NULL;
    new_record->play_group.musical.country_long = NULL;
    new_record->show_kind = ERR_KIND;

    prompt("%s\n", "Введите название театра:");
    rc = scan_string(new_record->theater_name, SHORT_STRING_MAX_LENGTH, &new_record->theater_name_long, input_file);
    if (rc != EXIT_SUCCESS)
    {
        if (feof(input_file))
        {
            free_record_safe(new_record);
            return rc;
        }
        free_record_safe(new_record);
        return rc;
    }

    prompt("%s\n", "Введите название постановки:");
    rc = scan_string(new_record->play_name, SHORT_STRING_MAX_LENGTH,  &new_record->play_name_long, input_file);
    if (rc != EXIT_SUCCESS)
    {
        if (rc == ERR_EMPTY_STRING)
            fprintf(stderr, "%s\n", "Строка не должна быть пустой");
        free_record_safe(new_record);
        return rc;
    }

    prompt("%s\n", "Введите фамилию и имя режиссёра-постановщика:");
    rc = scan_string(new_record->producer, SHORT_STRING_MAX_LENGTH, &new_record->producer_long, input_file);
    if (rc != EXIT_SUCCESS)
    {
        if (rc == ERR_EMPTY_STRING)
            fprintf(stderr, "%s\n", "Строка не должна быть пустой");
        free_record_safe(new_record);
        return rc;
    }

    prompt("Введите минимальную цену за билет (до %lu):\n", low_range_mask);
    int32_t low_range;
    rc = scan_non_negative_integer(input_file, &low_range, low_range_mask);
    if (rc != EXIT_SUCCESS)
    {
        free_record_safe(new_record);
        return rc;
    }
    set_low_range(&new_record->price_range, low_range);

    prompt("Введите максимальную цену за билет (до %lu):\n", low_range_mask);
    int32_t high_range;
    rc = scan_non_negative_integer(input_file, &high_range, low_range_mask);
    if (rc != EXIT_SUCCESS)
    {
        free_record_safe(new_record);
        return rc;
    }
    if (high_range < low_range)
    {
        fprintf(stderr, "%s\n", "Минимальная цена не может превышать максимальную");
        free_record_safe(new_record);
        return ERR_PRICE_RANGE;
    }
    set_high_range(&new_record->price_range, high_range);

    prompt("%s\n", "Введите тип спектакля (ДЕТСКИЙ, ВЗРОСЛЫЙ или МУЗЫКАЛЬНЫЙ):");
    new_record->show_kind = scan_show_kind(input_file);
    if (new_record->show_kind == ERR_KIND)
    {
        fprintf(stderr, "%s\n", "Некорректный тип спектакля");
        free_record_safe(new_record);
        return ERR_INPUT_STRING;
    }

    int tmp_int;
    if (new_record->show_kind == CHILD)
    {
        prompt("%s\n", "Введите тип представления (СКАЗКА, ПЬЕСА):");
        play_type_t play_type = scan_play_type(input_file);
        if (play_type != FAIRY_TALE && play_type != PLAY)
        {
            fprintf(stderr, "%s\n", "Некорректный тип представления");
            free_record_safe(new_record);
            return ERR_INPUT_STRING;
        }
        new_record->play_type = play_type;

        prompt("%s\n", "Введите минимальный возраст ребёнка (до 18):");
        rc = scan_non_negative_integer(input_file, &tmp_int, 18);
        if (rc != EXIT_SUCCESS)
        {
            free_record_safe(new_record);
            return rc;
        }
        new_record->play_group.child.age = tmp_int;
    }
    else if (new_record->show_kind == ADULT)
    {
        prompt("%s\n", "Введите тип представления (ПЬЕСА, ДРАМА, КОМЕДИЯ):");
        play_type_t play_type = scan_play_type(input_file);
        if (play_type == ERR_TYPE || play_type == FAIRY_TALE)
        {
            fprintf(stderr, "%s\n", "Некорректный тип представления");
            free_record_safe(new_record);
            return ERR_INPUT_STRING;
        }
        new_record->play_type = play_type;
    }
    else if (new_record->show_kind == MUSICAL)
    {
        prompt("%s\n", "Введите имя композитора:");
        rc = scan_string(new_record->play_group.musical.composer, SHORT_STRING_MAX_LENGTH, &new_record->play_group.musical.composer_long, input_file);
        if (rc != EXIT_SUCCESS)
        {
            if (rc == ERR_EMPTY_STRING)
                fprintf(stderr, "%s\n", "Строка не должна быть пустой");
            free_record_safe(new_record);
            return rc;
        }

        prompt("%s\n", "Введите название страны происхождения:");
        rc = scan_string(new_record->play_group.musical.country, SHORT_STRING_MAX_LENGTH, &new_record->play_group.musical.country_long, input_file);
        if (rc != EXIT_SUCCESS)
        {
            if (rc == ERR_EMPTY_STRING)
                fprintf(stderr, "%s\n", "Строка не должна быть пустой");
            free_record_safe(new_record);
            return rc;
        }

        prompt("%s\n", "Введите минимальный возраст (до 100):");
        rc = scan_non_negative_integer(input_file, &tmp_int, 100);
        if (rc != EXIT_SUCCESS)
        {
            free_record_safe(new_record);
            return rc;
        }
        new_record->play_group.musical.min_age = tmp_int;

        prompt("Введите продолжительность представления в минутах (до %d)\n", INT_MAX);
        rc = scan_non_negative_integer(input_file, &tmp_int, INT_MAX);
        if (rc != EXIT_SUCCESS)
        {
            free_record_safe(new_record);
            return rc;
        }
        new_record->play_group.musical.duration = tmp_int;
    }

    return EXIT_SUCCESS;
}

void
print_record(record_t *record,
             size_t index,
             int offset)
{
    printf("%3lu |", index);
    char *printing_string = record->theater_name;
    if (record->theater_name_long != NULL)
        printing_string = record->theater_name_long;
    unicode_format_print(printing_string, 24, offset);

    printing_string = record->play_name;
    if (record->play_name_long != NULL)
        printing_string = record->play_name_long;
    unicode_format_print(printing_string, 25, offset);

    printing_string = record->producer;
    if (record->producer_long != NULL)
        printing_string = record->producer_long;
    unicode_format_print(printing_string, 25, offset);

    printf(" %6d-%-6d |", get_low_range(record->price_range), get_high_range(record->price_range));

    switch (record->show_kind)
    {
        case CHILD:
            unicode_format_print(CHILD_STR, 13, 0);
            unicode_format_print(get_play_type_name(record->play_type), 15, offset);
            print_integer_centered(record->play_group.child.age, 17, '+');
            unicode_format_print("--", 25, 0);
            unicode_format_print("--", 8, 0);
            unicode_format_print("--", 9, 0);
            unicode_format_print("--", 7, 0);
            break;
        case ADULT:
            unicode_format_print(ADULT_STR, 13, 0);
            unicode_format_print(get_play_type_name(record->play_type), 15, offset);
            unicode_format_print("--", 17, 0);
            unicode_format_print("--", 25, 0);
            unicode_format_print("--", 8, 0);
            unicode_format_print("--", 9, 0);
            unicode_format_print("--", 7, 0);
            break;
        case MUSICAL:
            unicode_format_print(MUSICAL_STR, 13, offset);
            unicode_format_print("--", 15, offset);
            unicode_format_print("--", 17, 0);

            printing_string = record->play_group.musical.composer;
            if (record->play_group.musical.composer_long != NULL)
                printing_string = record->play_group.musical.composer_long;
            unicode_format_print(printing_string, 25, offset);

            printing_string = record->play_group.musical.country;
            if (record->play_group.musical.country_long != NULL)
                printing_string = record->play_group.musical.country_long;
            unicode_format_print(printing_string, 8, offset);

            print_integer_centered(record->play_group.musical.min_age, 9, '+');
            print_integer_centered(record->play_group.musical.duration, 7, ' ');
        case ERR_KIND:
            break;
    }
    print_newline();
}

