#ifndef HTABLE_H
#define HTABLE_H
#include <stdint.h>

struct freq_table {
  unsigned int num_leaf;
  struct node **leaf_list;
  struct h_table *table;
  struct node *htree;
};

struct h_table {
  unsigned int character;
  unsigned int count;
  char *hcode;
  int code_size;
};

void create_freq_table(char buffer[], int file_size, struct freq_table *ftable);

#endif
