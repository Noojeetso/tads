#include "menu.h"

int
main(void)
{
    int rc = 0;

    record_table_t main_record_table;
    rc = init_record_table(&main_record_table, MAIN_TABLE_SIZE);
    if (rc != EXIT_SUCCESS)
        return rc;

    rc = menu_loop(&main_record_table);
    free_table(&main_record_table);

    return rc;
}

