#include "menu.h"

void
print_menu()
{
    puts("Главное меню:");
    puts("0. Выход");
    puts("1. Загрузить числа из файла в АВЛ дерево и в ДДП");
    puts("2. Вывести ДДП");
    puts("3. Вывести АВЛ дерево");
    puts("4. Добавить элемент в АВЛ дерево, в ДДП и в файл");
    puts("5. Удалить элемент из АВЛ дерева, из ДДП и из файла");
    puts("6. Найти элемент в АВЛ дереве и в ДДП");
    puts("7. Определить количество узлов АВЛ дерева и ДДП на каждом уровне");
    puts("8. Сравнить эффективность алгоритмов сортировки и поиска с использованием ДДП, АВЛ дерева и файла\n");
}

int
flush_input()
{
    int amount = 0;
    int ch;
    do
    {
        ch = fgetc(stdin);
        if (ch == EOF)
            break;
        amount++;
    }
    while (ch != '\n');

    return amount;
}

int
scan_menu(int *key)
{
    int ch;

    printf("%s", "Номер меню: ");

    ch = fgetc(stdin);

    if (ch == '\n')
    {
        print_newline();
        fputs("Не был введён номер меню\n\n", stderr);
        return ERR_INPUT_NUMBER;
    }
    /*
    else
    {
    if (flush_input() != 1)
    {
        print_newline();
        fputs("Ошибка считывания номера меню\n\n", stderr);
        return EXIT_FAILURE;
    }*/

    *key = ch - '0';

    return EXIT_SUCCESS;
}


int
menu_loop()
{
    int rc;
    int key;
    int value;
    FILE *file;
    tree_node_t *bst = NULL;
    tree_node_t *avl = NULL;

    do
    {
        print_menu();

        rc = scan_menu(&key);
        if (rc != EXIT_SUCCESS)
            continue;

        print_newline();

        switch (key)
        {
            case 0:
                break;
            case 1:
                free_tree(&avl);
                free_tree(&bst);

                file = fopen(DATABASE_FILENAME, READ_MODE);
                if (file == NULL)
                {
                    fputs("Ошибка открытия файла на чтение\n", stderr);
                    return ERR_OPEN_FILE;
                }

                avl_fill(file, &avl);
                if (rc != EXIT_SUCCESS)
                {
                    fclose(file);
                    return rc;
                }

                fseek(file, 0, SEEK_SET);

                bst_fill(file, &bst);
                if (rc != EXIT_SUCCESS)
                {
                    fclose(file);
                    return rc;
                }

                fclose(file);

                break;
            case 2:
                rc = tree_to_dot("bst_tree.dot", bst);
                if (rc != EXIT_SUCCESS)
                    break;

                rc = dot_to_svg("bst_tree.dot", "bst_tree.svg");
                if (rc != EXIT_SUCCESS)
                    return rc;

                rc = open_svg("bst_tree.svg");
                if (rc != EXIT_SUCCESS)
                    break;

                break;
            case 3:
                rc = tree_to_dot("avl_tree.dot", avl);
                if (rc != EXIT_SUCCESS)
                    break;

                rc = dot_to_svg("avl_tree.dot", "avl_tree.svg");
                if (rc != EXIT_SUCCESS)
                    break;

                rc = open_svg("avl_tree.svg");
                if (rc != EXIT_SUCCESS)
                    break;

                break;
            case 4:
                rc = scan_int(&value, -10000, 10000);
                if (rc != EXIT_SUCCESS)
                    break;
                print_newline();

                rc = avl_push(&avl, value);
                if (rc != EXIT_SUCCESS)
                    break;

                rc = bst_push(&bst, value);
                if (rc != EXIT_SUCCESS)
                    break;

                rc = file_push(DATABASE_FILENAME, value);
                if (rc != EXIT_SUCCESS)
                    break;

                break;
            case 5:
                rc = scan_int(&value, -10000, 10000);
                if (rc != EXIT_SUCCESS)
                    break;
                print_newline();

                rc = avl_pop(&avl, value);
                if (rc != EXIT_SUCCESS)
                    break;

                rc = bst_pop(&bst, value);
                if (rc != EXIT_SUCCESS)
                    break;

                rc = file_pop(DATABASE_FILENAME, value);
                if (rc != EXIT_SUCCESS)
                    break;

                break;
            case 7:
                print_heights(avl, bst);
                break;
            default:
                fprintf(stderr, "Ожидалась цифра в пределах от %d до %d\n\n", MIN_MENU_KEY, MAX_MENU_KEY);
        }
        print_newline();

        flush_input();
    }
    while (key);

    return EXIT_SUCCESS;
}

