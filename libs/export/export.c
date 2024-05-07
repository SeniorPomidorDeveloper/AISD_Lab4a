#include "export.h"

#include <stdio.h>
#include <stdlib.h>

int export_graph(const Bin_tree *tree, const char *path)
{
    FILE *file = fopen(path, "w+");
    if (file == NULL)
    {
        return ERROR_FILE_OPEN;
    }
    Iter_Bin_tree_arr *arr;
    if (reverse_bypass(tree, &arr) == ERROR_ALLOCATION)
    {
        fclose(file);
        return ERROR_ALLOCATION;
    }
    if (fprintf(file, "digraph G {\n") != 12)
    {
        fclose(file);
        free(arr->arr);
        free(arr);
        return ERROR_FILE_WRITE;
    }
    for (size_t i = arr->len; i > 0; i--)
    {
        if (fprintf(file, "\tN%llu [label=\"%llu\"]\n", arr->arr[i - 1]->key, arr->arr[i - 1]->key) < 16)
        {
            fclose(file);
            free(arr->arr);
            free(arr);
            return ERROR_FILE_WRITE;
        }
    }
    if (fprintf(file, "\n") != 1)
    {
        fclose(file);
        free(arr->arr);
        free(arr);
        return ERROR_FILE_WRITE;
    }
    for (size_t i = 0; i < arr->len; i++)
    {
        Iter_Bin_tree node[2] = {arr->arr[i]->left, arr->arr[i]->right};
        for (size_t j = 0; j < 2; j++)
        {
            if (node[j] == NULL) continue;
            if (fprintf(file, "\tN%llu -> N%llu\n", arr->arr[i]->key, node[j]->key) < 10)
            {
                fclose(file);
                free(arr->arr);
                free(arr);
                return ERROR_FILE_WRITE;
            }
        }
    }
    if (fprintf(file, "}") != 1)
    {
        fclose(file);
        free(arr->arr);
        free(arr);
        return ERROR_FILE_WRITE;
    }
    free(arr->arr);
    free(arr);
    fclose(file);
    return SUCCESS;
}