#ifndef HTABLE_H
#include "htable.h"
#endif

#ifndef BTREE_H
#define BTREE_H

#include <stdint.h>
#define MAX_HUFF_CODE 256


struct node {
    int character;
    int count;
    int code_size;
    char *hcode;
    struct node *left;
    struct node *right;
};

struct linked {
    int character;
    int count;
    struct node *node;
    struct linked *next;
};

struct node* new_node(int character, int count);
struct node *create_tree(struct node *node_1, struct node *node_2);
int get_code(struct node *htree, struct node *leafNode[], 
char *hcode_string, struct freq_table *ftable, int leaf_index);
int is_leaf(struct node *node);
void append_string(char *original, char *to_append, char *buffer);
struct linked* create_linked_list(int character, int count);
struct linked* insert(struct linked* link, struct linked* list);
struct linked* remove_link(int character, struct linked* list);
void linked_incr_count(int character, struct linked* list);
struct node* tree_traversal(struct node *tree, char *code, int code_index);

#endif
