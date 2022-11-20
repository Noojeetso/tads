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

    result_node = find_node(tree->left, value);
    if (result_node != NULL)
        return result_node;

    return NULL;
}

void
count_avl_nodes(tree_node_t *tree,
                void *count_array)
{
    ((int *)count_array)[tree->height]++;
}

int
count_height(tree_node_t *tree)
{
    return 0;
}

int
print_heights(tree_node_t *avl,
              tree_node_t *bst)
{
    // printf("avl height: %d\n", avl->right->height);
    int *avl_nodes = calloc(avl->height + 1, sizeof(int));
    int *bst_nodes = calloc(avl->height + 1, sizeof(int));

    tree_apply(avl, count_avl_nodes, avl_nodes);
    tree_apply(bst, count_avl_nodes, bst_nodes);

    puts("АВЛ дерево:");
    puts("\tВысота\tКоличество");
    for (size_t i = avl->height; i > 0; i--)
        printf("\t%lu\t%d\n", i, avl_nodes[i]);

    puts("ДДП:");
    puts("\tВысота\tКоличество");
    for (size_t i = bst->height; i > 0; i--)
        printf("\t%lu\t%d\n", i, bst_nodes[i]);

    return EXIT_SUCCESS;
}

// Функции для ДДП
tree_node_t*
insert_bst(tree_node_t *tree,
           int value)
{
    int cmp;
    tree_node_t *node;

    if (tree == NULL)
    {
        node = new_node(value);
        if (node == NULL)
            fputs("Ошибка выделения памяти под новый узел ДДП", stderr);
        return node;
    }

    cmp = compare_int(value, tree->value);
    if (cmp == 0)
        abort();
    else if (cmp > 0)
        tree->right = insert_bst(tree->right, value);
    else
        tree->left = insert_bst(tree->left, value);

    return tree;
}

int
fill_bst(FILE *input_file,
         tree_node_t **tree)
{
    int value;

    while (fscanf(input_file, "%d", &value) == 1)
        *tree = insert_bst(*tree, value);

    return *tree != NULL;
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
        fputs("Ошибка удаления элемента", stderr);
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
rotate_right(tree_node_t* pivot) // поворот вправо вокруг pivot
{
    tree_node_t* new_root = pivot->left;
    pivot->left = new_root->right;
    new_root->right = pivot;
    fix_height(pivot);
    fix_height(new_root);
    return new_root;
}

tree_node_t*
rotate_left(tree_node_t* pivot) // поворот влево вокруг pivot
{
    tree_node_t* new_root = pivot->right;
    pivot->right = new_root->left;
    new_root->left = pivot;
    fix_height(pivot);
    fix_height(new_root);
    return new_root;
}

tree_node_t*
balance(tree_node_t* node) // балансировка узла node
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

    return node; // балансировка не нужна
}

tree_node_t*
insert_avl(tree_node_t *tree,
           int value)
{
    int cmp;
    tree_node_t *new_root;

    if (tree == NULL)
    {
        new_root = new_node(value);
        if (new_root == NULL)
            fputs("Ошибка выделения памяти под новый узел АВЛ дерева", stderr);
        return new_root;
    }

    cmp = compare_int(value, tree->value);
    if (cmp == 0)
        abort();
    else if (cmp > 0)
        tree->right = insert_avl(tree->right, value);
    else
        tree->left = insert_avl(tree->left, value);

    new_root = balance(tree);

    printf("val2: %d\n", new_root->value);

    return new_root;
}

int
fill_avl(FILE *input_file,
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
            fputs("Ошибка считывания значения нового узла дерева", stderr);
            return ERR_READING_VALUE;
        }
        if (raw_str[0] == '\n')
            return EXIT_SUCCESS;
        // sscanf(raw_str, "%d", value);
        printf("scanned: %d\n", value);
        *tree = insert_avl(*tree, value);
    }

    return tree != NULL;
}

tree_node_t *
remove_min(tree_node_t *tree) // удаление узла с минимальным ключом из дерева p
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
        fputs("Ошибка удаления элемента", stderr);
        return ERR_NULL_POINTER;
    }

    *root = new_root;
    return EXIT_SUCCESS;
}
