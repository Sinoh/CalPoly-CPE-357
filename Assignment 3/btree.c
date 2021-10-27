#include "btree.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* creates a new tree node from a character and frequency */
struct node *new_node(int character, int count) {
  struct node *node;
  if ((node = (struct node *)malloc(sizeof(struct node))) != NULL) {
    node->character = character;
    node->count = count;
    node->left = NULL;
    node->right = NULL;
  }
  return node;
}

/* Given two tree_nodes, create a parent tree node
    and with the two nodes as children. Lower frequency
    is left child. Higer frequency is right child. Parent
    node will be the character with a lower value */
struct node *create_tree(struct node *node_1, struct node *node_2) {
  struct node *combined_node;
  struct node *temp_node = node_1;
  if (node_1->character > node_2->character) {
    temp_node = node_2;
  }
  combined_node = new_node(temp_node->character, node_1->count + node_2->count);
  combined_node->left = node_1;
  combined_node->right = node_2;

  return combined_node;
}

/* Checks if a node is a leaf */
int is_leaf(struct node *node) { return !(node->left) && !(node->right); }

/*Given a string/array, appends another string/array to the end */
void append_string(char *original, char *to_append, char *buffer) {
  int len_original = 0;
  if (original != NULL) {
    strcpy(buffer, original);
    len_original = strlen(original);
  }
  buffer[len_original] = *to_append;
  buffer[len_original + 1] = '\0';
}

/* Traverse the tree and find the hcode of each leaf */
int get_code(struct node *htree, struct node *leafNode[], char *hcode_string,
             struct freq_table *ftable, int leaf_index) {
  int hcode_index;

  if (hcode_string != NULL) {
    hcode_index = strlen(hcode_string);
  }
  if (htree->left != NULL) {
    char *left_string = malloc(sizeof(left_string) * (hcode_index + 2));
    append_string(hcode_string, "0", left_string);
    leaf_index =
        get_code(htree->left, leafNode, left_string, ftable, leaf_index);
    if (is_leaf(htree->left)) {
      leaf_index++;
    }
  }
  if (htree->right != NULL) {
    char *right_string = malloc(sizeof(right_string) * (hcode_index + 2));
    append_string(hcode_string, "1", right_string);
    leaf_index =
        get_code(htree->right, leafNode, right_string, ftable, leaf_index);
    if (is_leaf(htree->right)) {
      leaf_index++;
    }
  }
  if (is_leaf(htree)) {
    char *full_hcode = NULL;
    full_hcode = malloc(sizeof(*full_hcode) * strlen(hcode_string) + 1);

    strcpy(full_hcode, hcode_string);
    htree->code_size = strlen(hcode_string);
    full_hcode[strlen(hcode_string)] = '\0';
    htree->hcode = full_hcode;
    leafNode[leaf_index] = htree;
    return leaf_index++;
  }
  return leaf_index;
}

/* Create a node of a linked list */
struct linked *create_linked_list(int character, int count) {
  struct linked *list;
  if ((list = (struct linked *)malloc(sizeof(struct linked))) != NULL) {
    list->character = character;
    list->count = count;
    list->node = NULL;
    list->next = NULL;
  }
  return list;
}

/* Insert a node into an existing linked list */
struct linked *insert(struct linked *link, struct linked *list) {
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

/* Remove a node from a linked list */
struct linked *remove_link(int character, struct linked *list) {
  if (list == NULL) {
    return NULL;
  } else if (character == list->character) {
    struct linked *new = list->next;
    free(list);
    return new;
  } else {
    list->next = remove_link(character, list->next);
  }
  return list;
}

/* Given a character and an existing linked list,
    increase the counter of the character's node */
void linked_incr_count(int character, struct linked *list) {
  if (list == NULL) {
    exit(0);
  } else if (character == list->character) {
    list->count++;
  } else {
    linked_incr_count(character, list->next);
  }
}

/* Traverse the tree for decode (WIP) */
struct node *tree_traversal(struct node *tree, char *code, int code_index) {
  if (tree == NULL) {
    return NULL;
  }

  if ((code_index == strlen(code)) || !(is_leaf(tree))) {
    return tree;
  }

  if (is_leaf(tree)) {
    return tree;
  }

  if (code[code_index] == 1) {
    return tree_traversal(tree->left, code, code_index++);
  }

  if (code[code_index] == 1) {
    return tree_traversal(tree->right, code, code_index++);
  }
  return tree;
}
