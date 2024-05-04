#include "import.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../error_codes.h"
#include "../compare/compare.h"

int import_tree(Bin_tree **tree, const char *path_file)
{
    FILE *file = fopen(path_file, "r");
    if (file == NULL) return ERROR_FILE_OPEN;
    if (init_Bin_tree(tree, compare) == ERROR_ALLOCATION)
    {
        fclose(file);
        return ERROR_ALLOCATION;
    }
    size_t len;
    int result = fscanf(file, "%zu", &len);
    if (result == EOF)
    {
        free_Bin_tree(*tree);
        fclose(file);
        return EOF;
    }
    else if (result == 0)
    {
        free_Bin_tree(*tree);
        fclose(file);
        return ERROR_FILE_READ;
    }
    for (size_t i = 0; i < len; ++i)
    {
        ULLI key;
        result = fscanf(file, "%llu", &key);
        if (result == EOF)
        {
            free_Bin_tree(*tree);
            fclose(file);
            return EOF;
        } 
        else if (result == 0)
        {
            fclose(file);
            return WARNING_FILE_READ;
        } 
        char symbol;
        do 
        {
            symbol = fgetc(file);
        } while (symbol != 10 && symbol != EOF);
        size_t BUF = 10;
        size_t str_len = 0;
        char *str = (char *) malloc((BUF + 1) * sizeof(char));
        if (str == NULL)
        {
            free_Bin_tree(*tree);
            fclose(file);
            return ERROR_ALLOCATION;
        }
        do 
        {
            symbol = fgetc(file);
            if (symbol != 10 && symbol != EOF && symbol != 13) 
            {
                str[str_len] = symbol;
                str_len++;
            } 
            if (str_len % BUF == 0 && str_len != 0) str = (char *) realloc(str, (str_len + BUF + 1) * sizeof(char));
            if (str == NULL)
            {
                free_Bin_tree(*tree);
                fclose(file);
                return ERROR_ALLOCATION;
            }
        } while (symbol != 10 && symbol != EOF);
        str[str_len] = '\0';
        str = (char *) realloc(str, (str_len + 1) * sizeof(char));
        if (str == NULL)
        {
            free_Bin_tree(*tree);
            fclose(file);
            return ERROR_ALLOCATION;
        }
        if (add_Bin_tree(*tree, key, str, NULL) == ERROR_ALLOCATION)
        {
            free(str);
            free_Bin_tree(*tree);
            fclose(file);
            return ERROR_ALLOCATION;
        }
        free(str);
    }
    fclose(file);
    return SUCCESS;
}