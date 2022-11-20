#include "unicode_print.h"

void
print_newline()
{
    putc('\n', stdout);
}

void
print_hyphen_line(int repeat)
{
    while (repeat-- > 0)
        putc('-', stdout);
    putc('\n', stdout);
}

void
print_integer_centered(size_t number,
                       int target_width,
                       char after_sign)
{
    int tmp = number;
    int length = 2;
    while (tmp /= 10)
        length++;
    target_width -= length;
    printf("%*c%lu%c%*c|", target_width - target_width / 2, ' ', number, after_sign, target_width / 2, ' ');
}

int
unicode_str_tolower(char *str)
{
    char *str_p = str;
    unsigned char code = *str_p;
    unsigned char prev_code;

    if ((code & TWO_HIGH_BITS_MASK) == ONE_HIGH_BIT_MASK)
    {
        fprintf(stderr, "%s\n", "Некорректный первый байт в utf-8 строке");
        return EXIT_FAILURE;
    }

    while(*str_p != '\0')
    {
        code = *str_p;
        // Стандартная ASCII таблица
        if ((code & ONE_HIGH_BIT_MASK) == 0)
        {
            if (code >= 'A' && code <= 'Z')
                *str_p += ('a' - 'A');
        }
        // Первый из двух байтов
        else if ((code & THREE_HIGH_BITS_MASK) == TWO_HIGH_BITS_MASK)
        {
            str_p++;
            continue;
        }
        // Второй из двух байтов
        else if ((code & TWO_HIGH_BITS_MASK) == ONE_HIGH_BIT_MASK)
        {
            int rem = SHIFT_RUSSIAN;
            prev_code = *(str_p - 1);
            if (code >= CYRILLIC_START_CAPITAL_LOWER_BYTE &&
                code <= CYRILLIC_END_CAPITAL_LOWER_BYTE &&
                prev_code >= CYRILLIC_START_CAPITAL_UPPER_BYTE &&
                prev_code <= CYRILLIC_END_CAPITAL_UPPER_BYTE)
            {
                *str_p += rem;
                if (*str_p & 0x40)
                {
                    *str_p -= 0x40;
                    *(str_p - 1) += 1;
                }
            }
        }
        str_p++;
    }

    return EXIT_SUCCESS;
}

int
multibyte_safe_byte_amount(char *str,
                           int *max_symbols_amount)
{
    int safe_byte_amount = 0;
    int byte_amount = 0;
    int symbols_amount = 0;
    unsigned char current_char;

    while (*str != '\0')
    {
        current_char = *str;
        symbols_amount++;
        byte_amount++;
        if ((current_char & THREE_HIGH_BITS_MASK) == TWO_HIGH_BITS_MASK)
        {
            byte_amount++;
            str++;
        }
        else if (current_char && FOUR_HIGH_BITS_MASK == THREE_HIGH_BITS_MASK)
        {
            byte_amount += 2;
            str += 2;
        }
        else if (current_char && FIVE_HIGH_BITS_MASK == FOUR_HIGH_BITS_MASK)
        {
            byte_amount += 3;
            str += 3;
        }
        str++;

        if (symbols_amount > *max_symbols_amount)
            break;

        safe_byte_amount = byte_amount;
    }

    *max_symbols_amount -= symbols_amount;

    return safe_byte_amount;
}

int
multibyte_strlen(char *str)
{
    int count = 0;
    unsigned char current_char;

    while (*str != '\0')
    {
        current_char = *str;
        count++;
        if ((current_char & THREE_HIGH_BITS_MASK) == TWO_HIGH_BITS_MASK)
            str++;
        else if (current_char && FOUR_HIGH_BITS_MASK == THREE_HIGH_BITS_MASK)
            str += 2;
        else if (current_char && FIVE_HIGH_BITS_MASK == FOUR_HIGH_BITS_MASK)
            str += 3;
        str++;
    }

    return count;
}

void
unicode_format_print(char *str,
                     int target_width,
                     int shift_offset)
{
    target_width -= 2;

    int str_length = multibyte_strlen(str);

    shift_offset %= str_length + 1 + SCROLL_WAIT_SECONDS + SPACES;
    if (shift_offset <= SCROLL_WAIT_SECONDS)
        shift_offset = 0;
    shift_offset -= SCROLL_WAIT_SECONDS;

    int offset_width = target_width - str_length;
    offset_width = MAX(offset_width, 0);

    int remaining_width = target_width;
    int remaining_offset = shift_offset;
    remaining_offset = MIN(remaining_offset, str_length);

    int spaces_after_end = 0;

    int left_shift_bytes;
    int left_bytes;
    int right_bytes;
    int offset_delta;

    if (str_length <= target_width)
    {
        printf(" %*s", offset_width - offset_width / 2, "");
        printf("%s", str);
        printf(" %*s|", offset_width / 2, "");
    }
    else
    {
        left_shift_bytes = multibyte_safe_byte_amount(str, &remaining_offset);
        left_bytes = multibyte_safe_byte_amount(str + left_shift_bytes, &remaining_width);
        printf(" %.*s", left_bytes, str + left_shift_bytes);

        offset_delta = shift_offset - str_length;
        spaces_after_end = offset_delta + target_width;
        offset_delta = MAX(offset_delta, 0);
        spaces_after_end = MAX(spaces_after_end, 0);
        spaces_after_end = MIN(spaces_after_end, SPACES);
        spaces_after_end -= offset_delta;
        printf("%*s", spaces_after_end, "");

        remaining_width -= spaces_after_end;
        remaining_width = MAX(remaining_width, 0);
        right_bytes = multibyte_safe_byte_amount(str, &remaining_width);
        printf("%.*s |", right_bytes, str);
    }
}

