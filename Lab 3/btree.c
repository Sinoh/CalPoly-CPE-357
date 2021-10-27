#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "btree.h"

struct node* new_node(int character, int count) {
    struct node* node;
    if ((node = (struct node*) malloc(sizeof(struct node))) != NULL) {
        node->character = character;
        node->count = count;
        node->left = NULL;
        node->right = NULL;
    }
    return node;
}

struct node *create_tree(struct node *node_1, struct node *node_2) {
    struct node* combined_node;
    struct node* temp_node = node_1;
    if (node_1->character > node_2->character) {
        temp_node = node_2;
    }
    combined_node = new_node(temp_node->character,
            node_1->count + node_2->count);
    combined_node->left = node_1;
    combined_node->right = node_2;

    return combined_node;
}

int is_leaf(struct node *node) {
    return !(node->left) && !(node->right);
}

void get_code(struct node *tree, struct freq_table *table, 
        int index, int array[]) {
        int i;

        if (tree->left) {
            array[index] = 0;
            get_code(tree->left,table, index + 1, array);
        }
        if (tree->right) {
            array[index] = 1;
            get_code(tree->right, table, index + 1, array);
        }
        
        if (is_leaf(tree)) {
            for (i = 0; i < index; i++) {
                table[tree->character].code[i] = array[i];
            }
            table[tree->character].code_size = index;
        }
}

struct linked* create_linked_list(int character, int count) {
    struct linked* list;
   if ((list = (struct linked*) malloc(sizeof(struct linked))) != NULL) {
    list->character = character;
    list->count = count;
    list->node = NULL;
    list->next = NULL;
   }
    return list;
}

struct linked* insert(struct linked* link, struct linked* list) {
    if (link == NULL && list == NULL) {
        return NULL;
    } else if (link == NULL || list == NULL) {
        if (link == NULL) {
            return list;
        } else {
            return link;
        }
    } else if (link->count == list->count) {
        if (link->character > list->character) {
            list->next = insert(link, list->next);
            return list;
        } else {
            link->next = list;
            return link;
        }
    } else if (link->count > list->count) {
        list->next = insert(link, list->next);
        return list;
    } else {
        link->next = list;
        return link;
    }
    return list;
}

struct linked* remove_link(int character, struct linked* list) {
    if (list == NULL) {
        return NULL;
    } else if (character == list->character) {
        struct linked* new = list->next;
        free(list);
        return new;
    } else {
        list->next = remove_link(character, list->next);
    }
    return list;
}

void linked_incr_count(int character, struct linked* list) {
    if (list == NULL) {
        exit(0);
    } else if (character == list->character) {
        list->count++;
    } else {
        linked_incr_count(character, list->next);
    }
}

