#include "menu.h"

int
main(void)
{
    int rc = 0;

    array_stack_t *array_stack = array_stack_create(MAIN_STACK_SIZE);
    if (array_stack == NULL)
        return ERR_NO_MEMORY;

    list_stack_t *list_stack = list_stack_create(MAIN_STACK_SIZE);
    if (list_stack == NULL)
        return ERR_NO_MEMORY;

    adress_array_t *adress_array = adress_array_create(MAIN_STACK_SIZE * 2);

    rc = menu_loop(array_stack, list_stack, adress_array);

    array_stack_free(array_stack);
    list_stack_free(list_stack);
    adress_array_free(adress_array);

    return rc;
}
