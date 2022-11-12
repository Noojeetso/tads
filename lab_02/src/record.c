#include "record.h"

void
free_record_safe(record_t *record)
{
    if (record->play_name_long != NULL)
        free(record->theater_name_long);
    if (record->play_name_long != NULL)
        free(record->play_name_long);
    if (record->producer_long != NULL)
        free(record->producer_long);
    if (record->show_kind == MUSICAL)
    {
        if (record->play_group.musical.composer_long != NULL)
            free(record->play_group.musical.composer_long);
        if (record->play_group.musical.country_long != NULL)
            free(record->play_group.musical.country_long);
    }
}

int32_t
get_high_range(uint64_t range)
{
    return range >> range_shift;
}

int32_t
get_low_range(uint64_t range)
{
    return range & low_range_mask;
}

void
set_low_range(uint64_t *range,
              int32_t low_range)
{
    *range &= high_range_mask;
    *range += low_range;
}

void
set_high_range(uint64_t *range,
               int32_t high_range)
{
    *range &= low_range_mask;
    *range += (uint64_t)high_range << range_shift;
}

char*
get_play_type_name(play_type_t play_type)
{
    switch (play_type)
    {
        case FAIRY_TALE:
            return FAIRY_TALE_STR;
        case PLAY:
            return PLAY_STR;
        case DRAMA:
            return DRAMA_STR;
        case COMEDY:
            return COMEDY_STR;
        default:
            return UNDEFINED_STR;
    }
}

