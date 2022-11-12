#ifndef __RECORD_H__
#define __RECORD_H__

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include "errors.h"
#include "presets.h"
#include "unicode_print.h"

#define INIT_SIZE 20
#define MAX_TYPE_NAME_LENGTH 30
#define FIELD_NUM 12
#define SHORT_STRING_MAX_LENGTH 40

#define CHILD_STR "детский"
#define ADULT_STR "взрослый"
#define MUSICAL_STR "музыкальный"
#define FAIRY_TALE_STR "сказка"
#define PLAY_STR "пьеса"
#define DRAMA_STR "драма"
#define COMEDY_STR "комедия"
#define UNDEFINED_STR "не определено"

const static int range_shift = CHAR_BIT * sizeof(uint64_t) / 2;
const static uint32_t low_range_mask = UINT32_MAX;
const static uint64_t high_range_mask = UINT64_MAX - low_range_mask;

typedef enum {ERR_TYPE, FAIRY_TALE, PLAY, DRAMA, COMEDY} play_type_t;

typedef enum {ERR_KIND, CHILD, ADULT, MUSICAL} show_kind_t;

typedef struct record
{
    char theater_name[SHORT_STRING_MAX_LENGTH + 2];
    char *theater_name_long;
    char play_name[SHORT_STRING_MAX_LENGTH + 2];
    char *play_name_long;
    char producer[SHORT_STRING_MAX_LENGTH + 2];
    char *producer_long;
    uint64_t price_range;
    show_kind_t show_kind;
    play_type_t play_type;
    union
    {
        struct child
        {
            char age;
        } child;
        struct musical
        {
            char composer[SHORT_STRING_MAX_LENGTH + 2];
            char *composer_long;
            char country[SHORT_STRING_MAX_LENGTH + 2];
            char *country_long;
            char min_age;
            int duration;
        } musical;
    } play_group;
} record_t;

typedef struct key
{
    char theater_name[SHORT_STRING_MAX_LENGTH + 2];
    char *theater_name_long;
    size_t id;
} key_t;

void
free_record_safe(record_t *record);

int32_t
get_high_range(uint64_t range);

int32_t
get_low_range(uint64_t range);

void
set_low_range(uint64_t *range,
              int32_t low_range);

void
set_high_range(uint64_t *range,
               int32_t high_range);

char*
get_play_type_name(play_type_t play_type);

void
print_record(record_t *record,
             size_t index,
             int offset);

int
scan_string(char *short_str,
            size_t short_str_max_length,
            char **long_str,
            FILE *input_file);

int
scan_non_negative_integer(FILE *input_file,
                          int32_t *out_int,
                          int32_t max_int);

int
scan_record(record_t *new_record,
           FILE *input_file,
           int (*prompt)(const char *const format, ...));

#endif // __RECORD_H__

