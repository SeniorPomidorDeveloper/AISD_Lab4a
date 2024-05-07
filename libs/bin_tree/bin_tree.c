#include "bin_tree.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

Error_code init_Bin_tree(Bin_tree **tree, bool (*compare) (const ULLI, const ULLI))
{
    *tree = (Bin_tree *) malloc(sizeof(Bin_tree));
    if (*tree == NULL)
    {
        return ERROR_ALLOCATION;
    }
    (*tree)->root = NULL;
    (*tree)->height = 0;
    (*tree)->compare = compare;
    return 0;
}

Iter_Bin_tree get_root_Bin_tree(const Bin_tree *tree)
{
    return tree->root;
}

Iter_Bin_tree next_Bin_tree(const Iter_Bin_tree iter, const Side side)
{
    if (side == LEFT)
    {
        return iter->left;
    }
    return iter->right;
}

Iter_Bin_tree prev_Bin_tree(const Iter_Bin_tree iter)
{
    return iter->parent;
}

Iter_Bin_tree find_Bin_tree(const Bin_tree *tree, const ULLI key)
{
    Iter_Bin_tree iter = tree->root;
    while (iter != NULL)
    {
        if (iter->key == key)
        {
            return iter;
        }
        if (tree->compare(key, iter->key))
        {
            iter = next_Bin_tree(iter, RIGHT);
        }
        else
        {
            iter = next_Bin_tree(iter, LEFT);
        }
    }
    return NULL;
}

int get_height(Bin_tree *tree, Iter_Bin_tree iter, size_t *height)
{
    Iter_Bin_tree root = get_root_Bin_tree(tree);
    *height = 0;
    int result = NOT_FOUND;
    while (root != NULL)
    {
        if (root == iter)
        {
            result = FOUND;
            break;
        }
        if (tree->compare(iter->key, root->key)) root = next_Bin_tree(root, RIGHT);
        else root = next_Bin_tree(root, LEFT);
        *height += 1;
    }
    if (*height > tree->height) tree->height = *height;
    return result;
}

Iter_Bin_tree find_new_parent_Bin_tree(Bin_tree *tree, const ULLI key)
{
    Iter_Bin_tree iter = get_root_Bin_tree(tree);
    Iter_Bin_tree prev = iter;
    size_t height = 0;
    while (iter != NULL)
    {
        prev = iter;
        if (tree->compare(key, iter->key))
        {
            iter = next_Bin_tree(iter, RIGHT);
        }
        else
        {
            iter = next_Bin_tree(iter, LEFT);
        }
        height++;
    }
    if (prev != NULL && tree->height < height)
    {
        tree->height = height;
    }
    return prev; 
}

int add_Bin_tree(Bin_tree *tree, const ULLI key, const char *info, Iter_Bin_tree *iter)
{
    if (find_Bin_tree(tree, key) != NULL)
    {
        return FOUND;
    }
    Node_Bin_tree *ptr = (Node_Bin_tree *) malloc(sizeof(Node_Bin_tree));
    if (ptr == NULL)
    {
        return ERROR_ALLOCATION;
    }
    ptr->key = key;
    ptr->info = (char *) malloc((strlen(info) + 1) * sizeof(char));
    if (ptr->info == NULL)
    {
        free(ptr);
        return ERROR_ALLOCATION;
    }
    strcpy(ptr->info, info);
    ptr->left = NULL;
    ptr->right = NULL;
    ptr->parent = find_new_parent_Bin_tree(tree, key);
    if (iter != NULL) *iter = ptr;
    if (ptr->parent == NULL)
    {
        tree->root = ptr;
        ptr->next = NULL;
        ptr->prev = NULL;
        return SUCCESS;
    }
    if (tree->compare(key, ptr->parent->key))
    {
        ptr->parent->right = ptr;
        ptr->next = ptr->parent;
        ptr->prev = ptr->parent->prev;
        if (ptr->parent->prev != NULL) ptr->parent->prev->next = ptr;
        ptr->parent->prev = ptr;
    }
    else 
    {
        ptr->parent->left = ptr;
        ptr->next = ptr->parent->next;
        if (ptr->next != NULL) ptr->next->prev = ptr;
        ptr->parent->next = ptr;
        ptr->prev = ptr->parent;
    }
    return SUCCESS;
}

Iter_Bin_tree del_Bin_tree(Bin_tree *tree, const ULLI key)
{
    if (tree->root == NULL) return NULL;
    Iter_Bin_tree iter = find_Bin_tree(tree, key);
    if (iter == NULL) return NULL;
    Iter_Bin_tree result;
    free(iter->info);
    // 4 Варинта событий ----------------------------------------------------
    if (iter->left == NULL && iter->right == NULL) 
    {
        result = NULL;
        // Изменяем указатели для прошивки при удалении листа
        if (iter->prev != NULL) iter->prev->next = iter->next;
        if (iter->next != NULL) iter->next->prev = iter->prev;
        // ---------------------------------------------------
    }
    else if (iter->left != NULL && iter->right == NULL)
    {
        result = iter->left;
        if (iter->prev != NULL) iter->prev->next = result;
        if (iter->next != NULL) iter->next->prev = iter->prev;
    }
    else if (iter->left == NULL && iter->right != NULL) 
    {
        result = iter->right;
        if (iter->next != NULL) iter->next->prev = result;
        result->next = iter->next;
    }
    else 
    {
        result = iter->prev;
        Iter_Bin_tree res_par = result->parent;
        Iter_Bin_tree res_left_child = result->left;
        Iter_Bin_tree res_right_child = result->right;
        Iter_Bin_tree inter_left_child = iter->left;
        Iter_Bin_tree inter_right_child = iter->right;
        if (result->parent != NULL && result->parent != iter)
        {
            // Изменям родительский элемент у которого мы забираем ребёнка -----------
            if (res_par->right == result) res_par->right = res_left_child;
            else res_par->left = res_right_child;
            // -----------------------------------------------------------------------
        }
        // Изменяем детей переносимого узла ----------------------------
        if (res_left_child != NULL && res_par != iter) res_left_child->parent = res_par;
        else if (res_right_child != NULL && res_par != iter) res_right_child->parent = res_par;
        // -------------------------------------------------------------
        // Изменяем родственные связи переносимого узла ------
        result->left = inter_left_child;
        if (res_par != iter) result->right = inter_right_child;
        // ---------------------------------------------------
        // Изменяем детей удаляемого узла --------------------
        if (inter_left_child != NULL) inter_left_child->parent = result;
        if (inter_right_child != NULL) inter_right_child->parent = result;
        // ---------------------------------------------------

        // Изменяем указатели для прошивки при удалении узла
        if (iter->prev != NULL) iter->prev->next = result;
        if (iter->next != NULL) iter->next->prev = result;
        result->next = result != iter->next ? iter->next : NULL;
        // -------------------------------------------------

    }
    // ----------------------------------------------------------------------
    if (result != NULL)
    {
        result->parent = iter->parent;
    }
    if (iter->parent != NULL)
    {
        // Изменяем нового родителя ----------------------------------
        if (iter->parent->right == iter) iter->parent->right = result;
        else iter->parent->left = result;
        // -----------------------------------------------------------
    }
    if (iter == tree->root) tree->root = result;
    free(iter);
    if (result == NULL) result = tree->root;
    return result;
}

Error_code reverse_bypass(const Bin_tree *tree, Iter_Bin_tree_arr **arr)
{
    *arr = (Iter_Bin_tree_arr *) malloc(sizeof(Iter_Bin_tree_arr));
    if (arr == NULL) return ERROR_ALLOCATION;
    (*arr)->len = 0;
    size_t capacity = 1;
    for (size_t i = 0; i < tree->height + 1; i++) capacity *= 2;
    (*arr)->arr = (Iter_Bin_tree *) malloc((capacity - 1) * sizeof(Iter_Bin_tree));
    if ((*arr)->arr == NULL) return ERROR_ALLOCATION;
    Iter_Bin_tree iter = get_root_Bin_tree(tree);
    if (iter == NULL) 
    {
        free((*arr)->arr);
        (*arr)->arr = NULL;
        return SUCCESS;
    }
    while (iter->right != NULL) iter = iter->right;
    do
    {
        ((*arr)->arr)[(*arr)->len] = iter;
        (*arr)->len++;
        iter = iter->next;
    } while(iter != NULL);
    (*arr)->arr = realloc((*arr)->arr, (*arr)->len * sizeof(Iter_Bin_tree));
    return SUCCESS;
}

Iter_Bin_tree max_Bin_tree(const Bin_tree *tree)
{
    Iter_Bin_tree iter = get_root_Bin_tree(tree);
    if (iter == NULL) return NULL;
    while (iter->right != NULL)
    {
        iter = next_Bin_tree(iter, RIGHT);
    }
    return iter;
}

void free_Bin_tree(Bin_tree *tree)
{
    Iter_Bin_tree iter = max_Bin_tree(tree);
    while (iter != NULL)
    {
        free(iter->info);
        Iter_Bin_tree prev = iter;
        iter = iter->next;
        free(prev);
    }
    free(tree);
}