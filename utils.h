#pragma once




#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linked-list.h"
#include "red-black-tree.h"
#include "globals.h"

int menu();

int start_tree(rb_tree *tree, char* arg);
int insert_data(rb_tree *tree, char *arg);

void estadistics(rb_tree *tree, char*  arg);
void max_destinations_tree(rb_tree *tree);

void write_data(rb_tree* tree, char* filename);
void read_data(rb_tree* tree, char* filename);

void free_memory(rb_tree* tree);



