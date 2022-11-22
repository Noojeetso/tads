#include "tree.h"

int
compare_int(int first,
            int second)
{
    if (first == second)
        return 0;
    return first > second ? 1 : -1;
}

tree_node_t*
new_node(int value)
{
    tree_node_t *node = malloc(sizeof(tree_node_t));
    if (node != NULL)
    {
        node->value = value;
        node->height = 1;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}

void
free_tree_r(tree_node_t *tree)
{
    if (tree == NULL)
        return;

    free_tree_r(tree->left);

    free_tree_r(tree->right);

    free(tree);
}

void
free_tree(tree_node_t **tree)
{
    free_tree_r(*tree);
    *tree = NULL;
}

void
tree_apply(tree_node_t *tree,
           void (*func)(tree_node_t *tree, void *),
           void *arg)
{
    if (tree == NULL)
        return;

    func(tree, arg);

    tree_apply(tree->left, func, arg);

    tree_apply(tree->right, func, arg);
}

tree_node_t *
find_node(tree_node_t *tree,
          int value)
{
    tree_node_t *result_node;

    if (tree == NULL || tree->value == value)
        return tree;

    result_node = find_node(tree->left, value);
    if (result_node != NULL)
        return result_node;

    result_node = find_node(tree->right, value);
    if (result_node != NULL)
        return result_node;

    return NULL;
}

void
tree_search(tree_node_t *tree,
            int value)
{
    tree_node_t *node;
    clock_t start, end;
    start = clock();
    node = find_node(tree, value);
    end = clock();
    if (node == NULL)
        puts("Элемент не найден");
    else
        printf("Элемент найден: %d\n", node->value);
    printf("Затрачено времени: %lu мкс\n", (end - start) / (CLOCKS_PER_SEC / 1000000));
}


void
avl_count_heights(tree_node_t *tree,
                  void *count_array)
{
    ((int *)count_array)[tree->height]++;
}

void
count_nodes_amount(tree_node_t *tree,
                   void *count)
{
    *((int *)count) += 1;
}

int
bst_count_heights(tree_node_t *tree,
                  int *heights,
                  int height)
{
    int left_height;
    int right_height;

    if (tree == NULL)
        return 1;

    left_height = bst_count_heights(tree->left, heights, height);
    right_height = bst_count_heights(tree->right, heights, height);

    height = left_height > right_height ? left_height : right_height;
    heights[height]++;

    return height + 1;
}

void
print_heights(tree_node_t *avl,
              tree_node_t *bst)
{
    int bst_max_height = 0;
    int *avl_nodes;
    int *bst_nodes;
    int bst_height;

    avl_nodes = calloc(avl->height + 1, sizeof(int));
    tree_apply(bst, count_nodes_amount, &bst_max_height);
    bst_nodes = calloc(bst_max_height + 1, sizeof(int));

    tree_apply(avl, avl_count_heights, avl_nodes);
    bst_count_heights(bst, bst_nodes, 0);

    for (bst_height = bst_max_height; bst_height > 0; bst_height--)
        if (bst_nodes[bst_height] != 0)
            break;

    puts("АВЛ дерево:");
    puts("\tВысота\tКоличество узлов");
    for (size_t i = avl->height; i > 0; i--)
        printf("\t%lu\t%d\n", i, avl_nodes[i]);

    puts("\nДвоичное дерево поиска:");
    puts("\tВысота\tКоличество узлов");
    for (size_t i = bst_height; i > 0; i--)
        printf("\t%lu\t%d\n", i, bst_nodes[i]);

    free(avl_nodes);
    free(bst_nodes);
}

tree_node_t*
find_max_node(tree_node_t *tree)
{
    return tree->right != NULL ? find_max_node(tree->right) : tree;
}

tree_node_t *
find_min_node(tree_node_t *tree)
{
    return tree->left != NULL ? find_min_node(tree->left) : tree;
}

// Функции для ДДП
tree_node_t*
bst_insert(tree_node_t *tree,
           int value)
{
    int cmp;
    tree_node_t *node;

    if (tree == NULL)
    {
        node = new_node(value);
        if (node == NULL)
            fputs("Ошибка выделения памяти под новый узел ДДП\n", stderr);
        return node;
    }

    cmp = compare_int(value, tree->value);
    if (cmp == 0)
        return tree;
        // abort();
    else if (cmp > 0)
        tree->right = bst_insert(tree->right, value);
    else
        tree->left = bst_insert(tree->left, value);

    return tree;
}

int
bst_push(tree_node_t **tree,
         int value)
{
    tree_node_t *new_root;

    if (find_node(*tree, value) != NULL)
    {
        fputs("Узел с таким значением уже существует\n", stderr);
        return ERR_ALREADY_EXISTS;
    }

    new_root = bst_insert(*tree, value);
    if (new_root == NULL)
        return ERR_NULL_POINTER;
    *tree = new_root;
    return EXIT_SUCCESS;
}

int
bst_fill(FILE *input_file,
         tree_node_t **tree)
{
    char raw_str[100];
    char *end_pointer;
    int value;

    while (fgets(raw_str, 100, input_file) != NULL)
    {
        value = strtol(raw_str, &end_pointer, 10);
        if (*end_pointer != '\n')
        {
            fputs("Ошибка считывания значения нового узла АВЛ дерева\n", stderr);
            return ERR_READING_VALUE;
        }
        if (raw_str[0] == '\n')
            return EXIT_SUCCESS;
        *tree = bst_insert(*tree, value);
    }

    return tree != NULL;
}

tree_node_t *
remove_min_bst(tree_node_t *tree)
{
    if (tree->left == NULL)
        return tree->right;
    tree->left = remove_min_bst(tree->left);
    return tree;
}

tree_node_t *
bst_remove(tree_node_t *tree,
           int value)
{
    if (tree == NULL)
        return NULL;

    if (tree->value > value)
        tree->left = bst_remove(tree->left, value);
    else if (tree->value < value)
        tree->right = bst_remove(tree->right, value);
    else
    {
        tree_node_t *left_child = tree->left;
        tree_node_t *right_child = tree->right;
        free(tree);
        if (right_child == NULL)
            return left_child;
        tree_node_t *right_child_min = find_min_node(right_child);
        right_child_min->right = remove_min_bst(right_child);
        right_child_min->left = left_child;
        return right_child_min;
    }
    return tree;
}

int
bst_pop(tree_node_t **root,
        int value)
{
    tree_node_t *new_root;

    new_root = bst_remove(*root, value);
    if (new_root == NULL)
    {
        fputs("Ошибка удаления элемента в ДДП\n", stderr);
        return ERR_NULL_POINTER;
    }

    *root = new_root;
    return EXIT_SUCCESS;
}

// Функции для АВЛ деревьев
unsigned char
get_height(tree_node_t *node)
{
    return node != NULL ? node->height : 0;
}

int
get_balance_factor(tree_node_t *node)
{
    return get_height(node->right) - get_height(node->left);
}

void
fix_height(tree_node_t* node)
{
    unsigned char height_left = get_height(node->left);
    unsigned char height_right = get_height(node->right);
    node->height = (height_left > height_right ? height_left : height_right) + 1;
}

tree_node_t*
rotate_right(tree_node_t* pivot)
{
    tree_node_t* new_root = pivot->left;
    pivot->left = new_root->right;
    new_root->right = pivot;
    fix_height(pivot);
    fix_height(new_root);
    return new_root;
}

tree_node_t*
rotate_left(tree_node_t* pivot)
{
    tree_node_t* new_root = pivot->right;
    pivot->right = new_root->left;
    new_root->left = pivot;
    fix_height(pivot);
    fix_height(new_root);
    return new_root;
}

tree_node_t*
balance(tree_node_t* node)
{
    fix_height(node);

    if (get_balance_factor(node) == 2)
    {
        if (get_balance_factor(node->right) < 0)
            node->right = rotate_right(node->right);
        return rotate_left(node);
    }

    if (get_balance_factor(node) == -2)
    {
        if (get_balance_factor(node->left) > 0)
            node->left = rotate_left(node->left);
        return rotate_right(node);
    }

    return node;
}

tree_node_t*
avl_insert(tree_node_t *tree,
           int value)
{
    int cmp;
    tree_node_t *new_root;

    if (tree == NULL)
    {
        new_root = new_node(value);
        if (new_root == NULL)
            fputs("Ошибка выделения памяти под новый узел АВЛ дерева\n", stderr);
        return new_root;
    }

    cmp = compare_int(value, tree->value);
    if (cmp == 0)
        return tree;
        // abort();
    else if (cmp > 0)
        tree->right = avl_insert(tree->right, value);
    else
        tree->left = avl_insert(tree->left, value);

    new_root = balance(tree);

    return new_root;
}

int
avl_push(tree_node_t **tree,
         int value)
{
    tree_node_t *new_root;

    if (find_node(*tree, value) != NULL)
    {
        fputs("Узел с таким значением уже существует\n", stderr);
        return ERR_ALREADY_EXISTS;
    }

    new_root = avl_insert(*tree, value);
    if (new_root == NULL)
    {
        fputs("Ошибка заполнения АВЛ дерева\n", stderr);
        return ERR_NULL_POINTER;
    }
    *tree = new_root;
    return EXIT_SUCCESS;
}

int
avl_fill(FILE *input_file,
         tree_node_t **tree)
{
    char raw_str[100];
    char *end_pointer;
    int value;

    while (fgets(raw_str, 100, input_file) != NULL)
    {
        value = strtol(raw_str, &end_pointer, 10);
        if (*end_pointer != '\n')
        {
            fputs("Ошибка считывания значения нового узла АВЛ дерева\n", stderr);
            return ERR_READING_VALUE;
        }
        if (raw_str[0] == '\n')
            return EXIT_SUCCESS;
        *tree = avl_insert(*tree, value);
    }

    return tree != NULL;
}

tree_node_t *
remove_min(tree_node_t *tree)
{
    if (tree->left == NULL)
        return tree->right;
    tree->left = remove_min(tree->left);
    return balance(tree);
}

tree_node_t *
avl_remove(tree_node_t *tree,
           int value)
{
    if (tree == NULL)
        return NULL;

    if (tree->value > value)
        tree->left = avl_remove(tree->left, value);
    else if (tree->value < value)
        tree->right = avl_remove(tree->right, value);
    else
    {
        tree_node_t *left_child = tree->left;
        tree_node_t *right_child = tree->right;
        free(tree);
        if (right_child == NULL)
            return left_child;
        tree_node_t *min = find_min_node(right_child);
        min->right = remove_min(right_child);
        min->left = left_child;
        return balance(min);
    }
    return balance(tree);
}

int
avl_pop(tree_node_t **root,
        int value)
{
    tree_node_t *new_root;

    new_root = avl_remove(*root, value);
    if (new_root == NULL)
    {
        fputs("Ошибка удаления элемента в АВЛ дереве\n", stderr);
        return ERR_NULL_POINTER;
    }

    *root = new_root;
    return EXIT_SUCCESS;
}
