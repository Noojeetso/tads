#include "bigfloat.h"
#include "presets.h"

void
print_info()
{
    int rc;
    char buff[MAX_BUFFER_SIZE];
    FILE *info_file = fopen("./txt/info.txt", "r");
    if (info_file == NULL)
    {
        perror(NULL);
        return;
    }

    while (fgets(buff, MAX_BUFFER_SIZE, info_file) != NULL)
        printf("%s", buff);
    printf("%c", '\n');

    rc = fclose(info_file);
    if (rc != EXIT_SUCCESS)
        perror(NULL);
}

int
main(void)
{
    int rc;
    bigfloat_t *a, *b;

    PRINT_INFO();

    a = new_bigfloat();
    b = new_bigfloat();
    if (a == NULL || b == NULL)
        return EXIT_FAILURE;

    printf("%s\n", "Введите первый множитель:");
    printf("%s\n", "---------|---------|---------|");
    rc = input_bigfloat(a);
    if (rc != EXIT_SUCCESS)
        return rc;
    // print_bigfloat(a);

    printf("%s\n", "Введите второй множитель:");
    printf("%s\n", "---------|---------|---------|");
    rc = input_bigfloat(b);
    if (rc != EXIT_SUCCESS)
    {
        free_bigfloat(&a);
        return rc;
    }
    // print_bigfloat(b);

    bigfloat_t *multi = bigfloat_mul(a, b);
    free_bigfloat(&a);
    free_bigfloat(&b);
    if (multi == NULL)
        return EXIT_FAILURE;

    printf("Результат умножения:\n");
    print_bigfloat(multi);

    free_bigfloat(&multi);

    return EXIT_SUCCESS;
}

