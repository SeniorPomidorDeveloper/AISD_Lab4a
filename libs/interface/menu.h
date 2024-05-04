#ifndef MENU_H
#define MENU_H

#include "../error_codes.h"
#include "../bin_tree/bin_tree.h"

int find_menu(Bin_tree *tree);
int add_menu(Bin_tree *tree);
int del_menu(Bin_tree *tree);
int bypass_menu(Bin_tree * tree);

int individual_menu(Bin_tree *tree);

int main_menu(Bin_tree *tree);

#endif // MENU_H