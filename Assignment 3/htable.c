#include "btree.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_ASCII 256
#define ERROR_NUM -1

void create_freq_table(char buffer[], int file_size,
                       struct freq_table *ftable) {
  int i = 0;
  int array[MAX_HUFF_CODE];
  char *hcode_string = NULL;
  struct node *tree;
  struct node *temp_node;
  struct linked *list = NULL;
  struct linked *temp_list;

  while (i < file_size) {
    ftable->table[buffer[i]].count++;
    ftable->table[buffer[i]].character = buffer[i];
    i++;
  }

  // Creat a linked_list in asceending order of frequencey
  for (i = 0; i < NUM_ASCII; i++) {
    if (ftable->table[i].count != 0) {
      ftable->num_leaf++;
      temp_list = create_linked_list(i, ftable->table[i].count);
      temp_list->node = new_node(i, temp_list->count);
      list = insert(temp_list, list);
    }
  }
  /* If there are no characters */
  if (list == NULL) {
    exit(0);
  }

  /* If only one character is present */
  if (list != NULL && list->next == NULL) {
    struct node *leaf_nodes =
        ((struct node *)malloc(sizeof(struct node) * ftable->num_leaf));
        if (leaf_nodes == NULL) {
        perror("Leaf Nodes Error");
        exit(0);
      }
    ftable->leaf_list[0] = list->node;
    ftable->leaf_list[0]->hcode = 0;
    free(leaf_nodes);
    return;
  }

  /* Createsa htree */
  while (list->next != NULL) {
    int min1 = list->character;
    int min2 = list->next->character;
    struct node *node1 = list->node;
    struct node *node2 = list->next->node;
    list = remove_link(min1, list);
    list = remove_link(min2, list);

    temp_node = create_tree(node1, node2);
    temp_list = create_linked_list(temp_node->character, temp_node->count);
    temp_list->node = temp_node;
    list = insert(temp_list, list);
  }

  /* Setup */
  tree = list->node;
  struct node **leaf_nodes =
      ((struct node **)malloc(sizeof(struct node *) * ftable->num_leaf));
      if (leaf_nodes == NULL) {
        perror("Leaf Nodes Error");
        exit(0);
      }
  ftable->leaf_list = leaf_nodes;
  ftable->htree = tree;

  /* Traverse the tree to get each hcode */
  get_code(tree, leaf_nodes, hcode_string, ftable, 0);
  for (i = 0; i < ftable->num_leaf; i++) {
    ftable->table[leaf_nodes[i]->character].hcode = leaf_nodes[i]->hcode;
    ftable->table[leaf_nodes[i]->character].code_size =
        leaf_nodes[i]->code_size;
  }

  /* Free if need be */
  free(list);
  free(temp_node);
  free(temp_list);
}
