#include "visualization.h"

#include <stdio.h>

size_t len_num(ULLI num)
{
    size_t len = 0;
    if (num == 0) return 1;
    while (num > 0)
    {
        num /= 10;
        len++;
    }
    return len;
}

void consol_print_tree(Bin_tree *tree)
{
    Iter_Bin_tree max = max_Bin_tree(tree);
    if (max == NULL) return;
    size_t max_len_num = len_num(max->key);
    printf("\n");
    do
    {
        size_t height;
        get_height(tree, max, &height);
        height = height * (1 + max_len_num);
        // for (size_t i = 0; i < height - 3 * (height >= 3); i++) printf(" ");
        // if (max->parent != NULL)
        // {
        //     if (max->parent->right == max) printf(" / ");
        //     else printf(" \\ ");
        // }
        // printf("%llu\n", max->key);
        // max = max->next;
        if (max->parent != NULL)
        {
            if (max->parent->right == max) 
            {
                for (size_t i = 0; i < height; i++) printf(" ");
                printf("%llu\n", max->key);
                for (size_t i = 0; i < height - (height >= 1); i++) printf(" ");
                printf("/\n");
            }
            else 
            {
                for (size_t i = 0; i < height - 1 * (height >= 1); i++) printf(" ");
                printf("\\\n");
                for (size_t i = 0; i < height; i++) printf(" ");
                printf("%llu\n", max->key);
            }
        }
        else printf("%llu\n", max->key);
        max = max->next;
    } while (max != NULL);
    printf("\n");
}