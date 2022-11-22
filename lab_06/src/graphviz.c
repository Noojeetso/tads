#include "graphviz.h"

int
write_dot_record(FILE *output_file,
                 tree_node_t *left,
                 tree_node_t *right)
{
    int rc;
    if ( left == NULL || right == NULL)
        return EXIT_SUCCESS;
    rc = fprintf(output_file, "%d -> %d;\n", left->value, right->value);
    if (rc <= 0)
        return ERR_WRITING_FILE;
    return EXIT_SUCCESS;
}

int
write_dot_records(FILE *output_file,
                  tree_node_t *tree)
{
    int rc;
    if (tree == NULL)
        return EXIT_SUCCESS;

    rc = write_dot_record(output_file, tree, tree->left);
    if (rc != EXIT_SUCCESS)
        return rc;

    rc = write_dot_record(output_file, tree, tree->right);
    if (rc != EXIT_SUCCESS)
        return rc;

    write_dot_records(output_file, tree->left);

    write_dot_records(output_file, tree->right);

    return EXIT_SUCCESS;
}

int
tree_to_dot(char *file_name,
            tree_node_t *tree)
{
    int rc;
    FILE *output_file;

    puts("Преобразование дерева в .dot файл...");

    if (tree == NULL)
    {
        fputs("Дерево не имеет ни одного узла\n", stderr);
        return ERR_NULL_POINTER;
    }

    output_file = fopen(file_name, WRITE_MODE);
    if (output_file == NULL)
    {
        fputs("Ошибка при открытии файла на запись\n", stderr);
        return ERR_OPEN_FILE;
    }

    // rc = fputs("{{{\n", output_file);
    // rc = fputs("#!graphviz\n", output_file);
    rc = fputs("digraph test_tree {\n", output_file);
    if (rc == EOF)
    {
        fputs("Ошибка при записи в файл\n", stderr);
        fclose(output_file);
        return ERR_WRITING_FILE;
    }

    rc = write_dot_records(output_file, tree);
    if (rc != EXIT_SUCCESS)
    {
        fputs("Ошибка при записи в файл\n", stderr);
        fclose(output_file);
        return rc;
    }

    rc = fputs("}\n", output_file);
    // fputs("}}}\n", output_file);

    if (rc == EOF)
    {
        fclose(output_file);
        return ERR_WRITING_FILE;
    }

    if (fclose(output_file) == EOF)
    {
        fputs("Ошибка при закрытии файла\n", stderr);
        return ERR_CLOSE_FILE;
    }

    return EXIT_SUCCESS;
}

int
dot_to_svg(char *input_file_name,
           char *output_file_name)
{
    int rc;

    puts("Преобразование .dot в .svg файл...");

    if (fork() == 0)
    {
        rc = execlp("dot", "dot", "-Tsvg", input_file_name, "-o", output_file_name, NULL);
        if (rc != EXIT_SUCCESS)
        {
            fputs("Ошибка при создании изображения графика\n", stderr);
            return rc;
        }
    }

    return EXIT_SUCCESS;
}

int
open_svg(char *input_file_name)
{
    int rc;
    FILE *dev_null;

    puts("Открытие .svg файла...");

    if (fork() == 0)
    {
        dev_null = fopen("/dev/null", READ_MODE);
        dup2(fileno(dev_null), 1);
        dup2(fileno(dev_null), 2);
        rc = execlp("eog", "eog", input_file_name, NULL);
        if (rc != EXIT_SUCCESS)
        {
            fputs("Ошибка при открытии изображении графика\n", stderr);
            return rc;
        }
        fclose(dev_null);
    }

    return EXIT_SUCCESS;
}
