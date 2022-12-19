#include "menu.h"

void
print_menu()
{
    puts("Главное меню:");
    puts("0. Выход");
    puts("1. Загрузить файл в графы на основе списка и матрицы");
    puts("2. Вывести граф на основе списка");
    puts("3. Вывести граф на основе матрицы");
    puts("4. В графе на основе списка для заданной вершины отобразить все недостижимые вершины");
    puts("5. В графе на основе массива для заданной вершины отобразить все недостижимые вершины");
    puts("6. Сравнить эффективность использования графов на основе списка и матрицы\n");
}

#define MAX_STR_LEN 100
#define DATABASES "databases/"

int
menu_loop()
{
    int rc;
    int key;
    int value;
    list_graph_t *list_graph = NULL;
    array_graph_t *array_graph = NULL;
    char file_name[MAX_STR_LEN];
    char file_path[MAX_STR_LEN + sizeof(DATABASES)];

    do
    {
        print_menu();

        rc = scan_menu(&key);
        print_newline();
        if (rc != EXIT_SUCCESS)
            continue;

        switch (key)
        {
            case 0:
                break;
            case 1:
                puts("Введите имя файла: ");
                rc = scan_string(file_name, MAX_STR_LEN);
                if (rc != EXIT_SUCCESS)
                {
                    flush_input();
                    break;
                }

                strcpy(file_path, DATABASES);
                strcat(file_path, file_name);

                puts("Очистка графа в виде списка...");
                if (list_graph != NULL)
                    free_list_graph(list_graph);
                list_graph = NULL;
                puts("Очистка графа в виде матрицы...");
                if (array_graph != NULL)
                    free_array_graph(array_graph);
                array_graph = NULL;

                print_newline();
                puts("Запись в граф на основе списка...");

                list_graph = list_graph_from_file(file_path);
                if (list_graph == NULL)
                    break;

                puts("Запись в граф на основе массива...");

                array_graph = array_graph_from_file(file_path);
                if (array_graph == NULL)
                    break;

                break;
            case 2:
                list_graph_visualize(list_graph, "list_graph");
                break;
            case 3:
                array_graph_visualize(array_graph, "array_graph");
                break;
            case 4:
                if (list_graph == NULL)
                {
                    fputs("Граф не существует\n", stderr);
                    break;
                }
                puts("Ввод номера исходной вершины");
                rc = scan_int(&value, 1, INT_MAX - 1);
                if (rc != EXIT_SUCCESS)
                    return rc;
                list_graph_visualize_unreachables(list_graph, value, "list_graph_unreachables");
                break;
            case 5:
                if (array_graph == NULL)
                {
                    fputs("Граф не существует\n", stderr);
                    break;
                }
                puts("Ввод номера исходной вершины");
                rc = scan_int(&value, 1, INT_MAX - 1);
                if (rc != EXIT_SUCCESS)
                    return rc;
                array_graph_visualize_unreachables(array_graph, value, "array_graph_unreachables");
                break;
            case 6:
                // print_efficiency();
                break;
            default:
                fprintf(stderr, "Ожидалась цифра в пределах от %d до %d\n", MIN_MENU_KEY, MAX_MENU_KEY);
        }
        print_newline();
    }
    while (key);

    if (list_graph != NULL)
        free_list_graph(list_graph);
    if (array_graph != NULL)
        free_array_graph(array_graph);

    return EXIT_SUCCESS;
}
