#ifndef STRUCT_H
#define STRUCT_H

#include <stddef.h>
#include <stdbool.h>

#define ULLI unsigned long long int

typedef struct Node_Bin_tree // Структура узла бинарного дерева
{
    ULLI key; // - Ключ
	struct Node_Bin_tree *left; // - Указатель на левый дочерний элемент
	struct Node_Bin_tree *right; // - Указатель на правый дочерний элемент
	struct Node_Bin_tree *next; // - Указатель на следующи элемент при обходе
	struct Node_Bin_tree *prev; // - Указатель на предыдущий элемент при обходе
	struct Node_Bin_tree *parent; // - Указатель на родительский элемент
	char *info; // - Указатель на хранимую информацию
} Node_Bin_tree;

typedef Node_Bin_tree * Iter_Bin_tree; // Итератор, указывающий на узел бинарного дерева

typedef struct Bin_tree // Структура бинарного дерева
{
	Node_Bin_tree *root; // - Указатель на корневой элемент бинарного дерева
	size_t height; // - Высота бинарного дерева
	bool (*compare) (const ULLI, const ULLI);
} Bin_tree;

typedef struct Iter_Bin_tree_arr // Структура, содержащая информацию элемента дерева
{
	size_t len;
	Iter_Bin_tree *arr;
} Iter_Bin_tree_arr;

#endif // STRUCT_H