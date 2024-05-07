#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#include "./libs/bin_tree/bin_tree.h"
#include "./libs/compare/compare.h"

int timing(const size_t step, const size_t num)
{
    Bin_tree *tree;
    int result = init_Bin_tree(&tree, compare);
    if (result != SUCCESS) return result;
    srand(time(NULL));
    ULLI keys[step];
    for (size_t i = 0; i < num; i++) {
        for (size_t j = 0; j < step; j++) keys[j] = rand() * rand();
        for (size_t j = 0; j < 1000000;)
        {
            ULLI key = rand() * rand();
            result = add_Bin_tree(tree, key, "\0", NULL);
            if (result == ERROR_ALLOCATION)
            {
                free_Bin_tree(tree);
                return ERROR_ALLOCATION;
            }
            else if (result == SUCCESS) j++;
        }
        size_t count_finds = 0;
        clock_t first_time = clock();
        for (size_t j = 0; j < step; ++j)
        {
            if (find_Bin_tree(tree, keys[j]) != NULL) count_finds++;
        }
        clock_t second_time = clock();
        printf("Было найдено %zu эллементов.\n", count_finds);
        printf("Тест №%zu: Колличество узлов - %zu; Время - %ld\n", i + 1, (i + 1) * 1000000, (second_time - first_time) / 1000);
    }
    free_Bin_tree(tree);
    return SUCCESS;
}

int main()
{
    return timing(10000, 15);
}