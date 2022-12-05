#ifndef __MENU_H__
#define __MENU_H__

#include "scan.h"
#include "graphviz.h"
#include "hashtable.h"
#include "efficiency.h"

#define MENU_TITLE "Главное меню:"
#define MENU_ITEM_0 "0. Выход"
#define MENU_ITEM_1 "1. Загрузить числа из файла в АВЛ дерево, в ДДП и в хэш-таблицы"
#define MENU_ITEM_2 "2. Вывести ДДП"
#define MENU_ITEM_3 "3. Вывести АВЛ дерево"
#define MENU_ITEM_4 "4. Вывести хэш-таблицы"
#define MENU_ITEM_5 "5. Добавить элемент в ДДП, АВЛ дерево, хэш-таблицу и в файл"
#define MENU_ITEM_6 "6. Удалить элемент из ДДП, АВЛ дерева, хэш-таблицы и из файла"
#define MENU_ITEM_7 "7. Найти элемент в АВЛ дереве, в ДДП, в хэш-таблицах и в файле"
#define MENU_ITEM_8 "8. Сравнить эффективность алгоритмов сортировки и поиска с использованием ДДП, АВЛ дерева и хэш-таблиц\n"

#define MIN_MENU_KEY 0
#define MAX_MENU_KEY 6

int
menu_loop();

#endif  // __MENU_H__
