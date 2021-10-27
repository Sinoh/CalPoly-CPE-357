#include "btree.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_ASCII 256
#define ERROR_NUM -1

int main(int argc, char **argv) {
  FILE *file;
  int i;
  int j;
  int c;
  int array[MAX_HUFF_CODE];
  struct node *tree;
  struct node *temp_node;
  struct linked *list = NULL;
  struct linked *temp_list;
  struct freq_table h_table[NUM_ASCII];

  for (i = 0; i < NUM_ASCII; i++) {
      h_table[i].count = 0;
      h_table[i].code_size = 0;
  }

  file = fopen(argv[1], "r");

  if (file == NULL) {
    perror("cannot open file");
  } else {
    while (!feof(file)) {
      c = fgetc(file);
      if (c == ERROR_NUM) {
        break;
      }

      h_table[c].count++;
    }

    for (i = 0; i < NUM_ASCII; i++) {
        if (h_table[i].count != 0) {
            temp_list = create_linked_list(i, h_table[i].count);
            temp_list->node = new_node(i, temp_list->count);
            list = insert(temp_list, list);
        }
    }
  }

  fclose(file);

  if (list == NULL) {
    exit(0);
  }
  if (list != NULL && list->next == NULL) {
    for (i = 0; i < NUM_ASCII; i++) {
      if (h_table[i].count != 0) {
        printf("0x%02x: \n", i);
        exit(0);
      }
    }
  }

  while (list->next != NULL) {
    int min1 = list->character;
    int min2 = list->next->character;
    struct node* node1 = list->node;
    struct node* node2 = list->next->node;
    list = remove_link(min1, list);
    list = remove_link(min2, list);

    temp_node = create_tree(node1, node2);
    temp_list = create_linked_list(temp_node->character, temp_node->count);
    temp_list->node = temp_node;
    list = insert(temp_list, list);

       
  }

  tree = list->node;
  get_code(tree, h_table, 0, array);

  for (i = 0; i < NUM_ASCII; i++) {
    if (h_table[i].count != 0) {
      printf("0x%02x: ", i);
      for (j = 0; j < h_table[i].code_size; j++) {
        printf("%d", h_table[i].code[j]);
      }
      printf("\n"); 

    }
  }
  free(tree);
  free(list);
  free(temp_node);
  free(temp_list);
}
