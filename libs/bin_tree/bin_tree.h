#ifndef BIN_TREE_H
#define BIN_TREE_H

#include "../error_codes.h"

#include "struct.h"
#include "Instructions.h"

Error_code init_Bin_tree(Bin_tree **tree, bool (*compare) (const ULLI, const ULLI));

Iter_Bin_tree get_root_Bin_tree(const Bin_tree *tree);
Iter_Bin_tree next_Bin_tree(const Iter_Bin_tree iter, const Side side);
Iter_Bin_tree prev_Bin_tree(const Iter_Bin_tree iter);


int get_height(Bin_tree *tree, Iter_Bin_tree iter, size_t *height);
Iter_Bin_tree find_Bin_tree(const Bin_tree *tree, const ULLI key);
int add_Bin_tree(Bin_tree *tree, const ULLI key, const char *info, Iter_Bin_tree *iter);
Iter_Bin_tree del_Bin_tree(Bin_tree *tree, const ULLI key);

Error_code reverse_bypass(const Bin_tree *tree, Iter_Bin_tree_arr **arr);
Iter_Bin_tree max_Bin_tree(const Bin_tree *tree);

void free_Bin_tree(Bin_tree *tree);

#endif // BIN_TREE_H