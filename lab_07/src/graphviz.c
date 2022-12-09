#include "graphviz.h"

char *
create_tree_file_name(char *tree_name,
                     char *EXTENSION)
{
    char *file_name = malloc(strlen(OUT_FOLDER) + strlen(tree_name) + sizeof('.') + strlen(IMAGE_EXTENSION) + sizeof('\0'));
    if (file_name == NULL)
        return NULL;
    file_name = memcpy(file_name, OUT_FOLDER, strlen(OUT_FOLDER) + 1);
    file_name = strcat(file_name, tree_name);
    file_name = strcat(file_name, ".");
    file_name = strcat(file_name, EXTENSION);
    return file_name;
}

int
write_dot_records(FILE *output_file,
                  tree_node_t *tree)
{
    tree_apply(tree, write_node_connection, output_file);

    return EXIT_SUCCESS;
}

int
tree_to_dot(tree_node_t *tree,
            char *tree_name)
{
    int rc;
    FILE *output_file;
    char *file_name;

    puts("Преобразование дерева в .dot файл...");

    if (tree == NULL)
    {
        fputs("Дерево не имеет ни одного узла\n", stderr);
        return ERR_NULL_POINTER;
    }

    file_name = create_tree_file_name(tree_name, DOT_EXTENSION);
    if (file_name == NULL)
    {
        fputs("Ошибка генерации имени файла\n", stderr);
        return ERR_NO_MEMORY;
    }

    output_file = fopen(file_name, WRITE_MODE);
    free(file_name);
    if (output_file == NULL)
    {
        fputs("Ошибка при открытии файла на запись\n", stderr);
        return ERR_OPEN_FILE;
    }

    rc = fputs("digraph tree {\n", output_file);
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
dot_to_svg(char *tree_name)
{
    int rc;
    pid_t postprocess_id;
    char *input_file_name;
    char *output_file_name;

    puts("Преобразование .dot в .svg файл...");

    postprocess_id = fork();

    if (postprocess_id == -1)
    {
        fputs("Ошибка создания процесса для генерации изображения", stderr);
        return EXIT_FAILURE;
    }

    if (postprocess_id == 0)
    {
        input_file_name = create_tree_file_name(tree_name, DOT_EXTENSION);
        if (input_file_name == NULL)
        {
            fputs("Ошибка генерации имени файла\n", stderr);
            return ERR_NO_MEMORY;
        }

        output_file_name = create_tree_file_name(tree_name, IMAGE_EXTENSION);
        if (output_file_name == NULL)
        {
            fputs("Ошибка генерации имени файла\n", stderr);
            free(input_file_name);
            return ERR_NO_MEMORY;
        }

        execlp("dot", "dot", "-T"IMAGE_EXTENSION, input_file_name, "-o", output_file_name, NULL);
    }

    waitpid(postprocess_id, &rc, 0);

    if (rc != EXIT_SUCCESS)
    {
        fputs("Ошибка при создании изображения графика\n", stderr);
    }

    return rc;
}

int
open_svg(char *tree_name)
{
    int rc;
    FILE *dev_null;
    char *input_file_name;

    puts("Открытие .svg файла...");

    if (fork() == 0)
    {
        input_file_name = create_tree_file_name(tree_name, IMAGE_EXTENSION);
        if (input_file_name == NULL)
        {
            fputs("Ошибка генерации имени файла\n", stderr);
            return ERR_NO_MEMORY;
        }
        dev_null = fopen("/dev/null", READ_MODE);
        dup2(fileno(dev_null), 1);
        dup2(fileno(dev_null), 2);
        rc = execlp("xdg-open", "xdg-open", input_file_name, NULL);
        free(input_file_name);
        if (rc != EXIT_SUCCESS)
        {
            fputs("Ошибка при открытии изображении графика\n", stderr);
            return rc;
        }
        fclose(dev_null);
    }

    return EXIT_SUCCESS;
}

int
tree_visualize(tree_node_t *tree,
               char *tree_name)
{
    int rc;

    rc = tree_to_dot(tree, tree_name);
    if (rc != EXIT_SUCCESS)
        return rc;

    rc = dot_to_svg(tree_name);
    if (rc != EXIT_SUCCESS)
        return rc;

    rc = open_svg(tree_name);
    if (rc != EXIT_SUCCESS)
        return rc;

    return EXIT_SUCCESS;
}
