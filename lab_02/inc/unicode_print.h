#ifndef __UNICODE_PRINT_H__
#define __UNICODE_PRINT_H__

#include <stdlib.h>
#include <stdio.h>
#include "presets.h"

#define FIVE_HIGH_BITS_MASK 248
#define FOUR_HIGH_BITS_MASK 240
#define THREE_HIGH_BITS_MASK 224
#define TWO_HIGH_BITS_MASK 192
#define ONE_HIGH_BIT_MASK 128
#define SHIFT_RUSSIAN 32

#define CYRILLIC_START_CAPITAL_LOWER_BYTE 144
#define CYRILLIC_END_CAPITAL_LOWER_BYTE 175
#define CYRILLIC_START_CAPITAL_UPPER_BYTE 208
#define CYRILLIC_END_CAPITAL_UPPER_BYTE 208

#define CYRILLIC_START_SMALL_LOWER_BYTE 176
#define CYRILLIC_END_SMALL_LOWER_BYTE 143
#define CYRILLIC_START_SMALL_UPPER_BYTE 208
#define CYRILLIC_END_SMALL_UPPER_BYTE 209

#define SCROLL_WAIT_SECONDS 15
#define SPACES 7

void
print_newline();

void
print_hyphen_line(int repeat);

void
print_integer_centered(size_t number,
                       int target_width,
                       char after_sign);

int
multibyte_strlen(char *str);

int
unicode_str_tolower(char *str);

void
unicode_format_print(char *str,
                     int target_width,
                     int shift_offset);

#endif // __UNICODE__PRINT_H__

