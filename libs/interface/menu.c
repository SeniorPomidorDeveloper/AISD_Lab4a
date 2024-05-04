#include "menu.h"

#include <stdlib.h>
#include <stdio.h>

#include "./libs/gets/gets.h"
#include "./libs/input/input.h"
#include "./libs/visualization/visualization.h"

int find_menu(Bin_tree *tree)
{
    long long int LLI_key;
    if (GetLLInt(&LLI_key, "\t> Введите ключ элемента, который хотите найти: ", check_no_negative) == EOF) return EOF;
    ULLI key = LLI_key;
    Iter_Bin_tree iter = find_Bin_tree(tree, key);
    if (iter == NULL)
    {
        printf("\t> Элемент с ключом %lld не найден.\n", key);
        return NOT_FOUND;
    }
    else printf("\t> tree[%lld] = \"%s\"\n", key, iter->info);
    return SUCCESS;
}

int add_menu(Bin_tree *tree)
{
    long long int LLI_key;
    if (GetLLInt(&LLI_key, "\t> Введите ключ элемента: ", check_no_negative) == EOF) return EOF;
    ULLI key = LLI_key;
    char *info;
    if (input(&info, "\t> Введите хранимые данные: ") == EOF) return EOF;
    int result = add_Bin_tree(tree, key, info, NULL);
    if (result == FOUND)
    {
        printf("\t> Элемент с ключом %lld уже существует.\n", key);
        free(info);
        return FOUND;
    } 
    else if (result == ERROR_ALLOCATION)
    {
        free(info);
        return ERROR_ALLOCATION;
    }
    else if (result == SUCCESS) printf("\t> Элемент с ключом %lld был успешно добавлен.\n", key);
    free(info);
    return SUCCESS;
}

int del_menu(Bin_tree *tree)
{
    long long int LLI_key;
    if (GetLLInt(&LLI_key, "\t> Введите ключ элемента, который хотите удалить: ", check_no_negative) == EOF) return EOF;
    ULLI key = LLI_key;
    if (del_Bin_tree(tree, key) == NULL)
    {
        printf("\t> Элемент с ключом %lld не найден.\n", key);
        return NOT_FOUND;
    }
    else printf("\t> Элемент с ключом %lld был успешно удалён.\n", key);
    return SUCCESS;
}

int individual_menu(Bin_tree *tree)
{
    Iter_Bin_tree iter = max_Bin_tree(tree);
    if (iter == NULL)
    {
        printf("\t> Дерево пустое.\n");
        return NOT_FOUND;
    }
    else printf("\t> MAX: tree[%lld] = \"%s\"\n", iter->key, iter->info);
    return SUCCESS;
}

int bypass_menu(Bin_tree * tree)
{
    Iter_Bin_tree_arr *arr;
    if (reverse_bypass(tree, &arr) == ERROR_ALLOCATION) return ERROR_ALLOCATION;
    printf("\n");
    for (size_t i = 0; i < arr->len; i++) 
    {
        printf("key: %lld; ", arr->arr[i]->key);
        if (arr->arr[i]->prev != NULL) printf("prev: %lld; ", arr->arr[i]->prev->key);
        if (arr->arr[i]->next != NULL) printf("next: %lld; ", arr->arr[i]->next->key);
        if (arr->arr[i]->left != NULL) printf("left: %lld; ", arr->arr[i]->left->key);
        if (arr->arr[i]->right != NULL) printf("right: %lld; ", arr->arr[i]->right->key);
        if (arr->arr[i]->parent != NULL) printf("parent: %lld; ", arr->arr[i]->parent->key);
        if (arr->arr[i]->info != NULL) printf("info: %s; ", arr->arr[i]->info);
        printf("\n");
    }
    printf("\n");
    free(arr->arr);
    free(arr);
    return SUCCESS;
}

bool check_option(long long int option)
{
    return option >= 0 && option <= 5;
}

bool check_result_menu(int result)
{
    return result == SUCCESS || result == NOT_FOUND || result == FOUND;
}

void print_menu()
{
    printf("1. Найти узел\n");
    printf("2. Добавить узел\n");
    printf("3. Удаление узла\n");
    printf("4. Нахождение узла с максимальным значением ключа\n");
    printf("5. Обход дерева в обратном порядке\n");
    printf("0. Выход\n");
}

int main_menu(Bin_tree *tree)
{
    long long int option;
    int (*function[]) (Bin_tree *table) = {find_menu, add_menu, del_menu, individual_menu, bypass_menu};
    do 
    {
        consol_print_tree(tree);
        print_menu();
        if (GetLLInt(&option, "Введите пункт меню: ", check_option) == EOF) return EOF; 
        if (option == 0) break;
        int result = function[option - 1](tree);
        if (!check_result_menu(result)) return result;
    } while (option != 0);
    return SUCCESS;
}