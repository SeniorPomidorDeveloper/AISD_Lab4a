#include "program.h"

#include <stdio.h>
#include <stdlib.h>

#include "./libs/bin_tree/bin_tree.h"
#include "./libs/compare/compare.h"
#include "./libs/import/import.h"
#include "./libs/interface/menu.h"
#include "./libs/interface/libs/input/input.h"
#include "./libs/interface/libs/gets/gets.h"

int import_menu(Bin_tree **tree)
{
    char *path;
    if (input(&path, "\t> Введите путь до файла: ") == EOF) return EOF;
    int result = import_tree(tree, path);
    free(path);
    if (result > WARNING_FILE_READ) return result;
    else if (result == WARNING_FILE_READ) printf("[WARNING] Файл был считан не до конца!");
    return SUCCESS;
}

int new_tree_menu(Bin_tree **tree)
{
    return init_Bin_tree(tree, compare);
}

bool check_option_input_menu(long long int num)
{
    return num >= 0 && num <= 2;
}

bool check_result_input_menu(int result)
{
    return result <= WARNING_FILE_READ;
}

void print_input_menu()
{
    printf("1. Импортировать дерево из текстового файла\n");
    printf("2. Создать новое дерево\n");
    printf("0. Выход\n");
}

int input_tree(Bin_tree **tree)
{
    long long int option;
    int (*function[]) (Bin_tree **table) = {import_menu, new_tree_menu};
    print_input_menu();
    if (GetLLInt(&option, "Введите пункт меню: ", check_option_input_menu) == EOF) return EOF; 
    if (option == 0) return SUCCESS;
    int result = function[option - 1](tree);
    if (!check_result_input_menu(result)) return result;
    return SUCCESS;
}

int program()
{
    Bin_tree *tree;
    int result = input_tree(&tree);
    if (result > WARNING_FILE_READ || result == EOF) return result;
    result = main_menu(tree);
    if (result != SUCCESS)
    { 
        free_Bin_tree(tree);
        return result;
    } 
    free_Bin_tree(tree);
    return SUCCESS;
}