#include "fileio.h"
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NUM_ASCII 256
#define BUF_SIZE 4096
#define ONE_BYTE 1
#define FOUR_BYTE 4
#define EIGHT_BITS 8

int main(int argc, char *argv[]) {

  int in_fd, out_fd;
  char buffer[BUF_SIZE];
  char *bit_value;
  int i, file_size;
  FILE *file;
  int out_flag = 0;
  struct node *temp_node;
  int char_count = 0;
  int char_number = 0;

  /* Check arguments given on the command line */
  if (argc > 3) {
    printf("hdecode [ ( infile | - ) [ outfile ] ]");
    exit(0);
  }

  if (strcmp(argv[1], "-") == 0) {
    /* implement stdin */
  } else {
    in_fd = open(argv[1], O_RDONLY);
    if (in_fd == -1) {
      perror("open");
      exit(0);
    }
  }

  /* Open and get file desriptor fro, output if present */
  if (argc == 3) {
    out_fd = open(argv[2], O_WRONLY | O_CREAT, 0644);
    if (out_fd == -1) {
      perror("open");
      exit(0);
    }
    out_flag = 1;
  }

  /* Create structures to be used in storing data */
  struct freq_table *ftable = calloc(1, sizeof(struct freq_table));
  if (ftable == NULL) {
    perror("Ftable Malloc Error");
    exit(1);
  }

  struct h_table *htable =
      ((struct h_table *)malloc(sizeof(struct h_table) * NUM_ASCII));
  if (ftable == NULL) {
    perror("Htable Malloc Error");
    exit(1);
  }

  ftable->table = htable;

  struct header *head = (struct header *)malloc(sizeof(struct header));
  struct character *char_arr =
      (struct character *)malloc(sizeof(struct character) * head->size);

  /* Read in and store respective data in head and char_arr */
  read(in_fd, &(head->size), FOUR_BYTE);
  for (i = 0; i < head->size; i++) {
    read(in_fd, &char_arr->letter, ONE_BYTE);
    read(in_fd, &char_arr->count, FOUR_BYTE);
  }

  /* Copy data to ftable for rebuilding htree */
  for (i = 0; i < head->size; i++) {
    ftable->table[char_arr[i].letter].count = char_arr[i].count;
    char_number += char_arr[i].count;
  }

  /* create htree */
  create_freq_table(buffer, 0, ftable);
  struct node *tree = ftable->htree;

  /* Read in the body to a buffer and decode it by traversing the tree
      then print or write out (WIP) */
  if (read(in_fd, &buffer, BUF_SIZE) != 0) {
    bit_value = malloc(sizeof(BUF_SIZE));
    temp_node = (struct node *)malloc(sizeof(struct node));
    for (i = 0; i < (BUF_SIZE); i++) {
      bit_value[i] = (buffer[i] >> 1) & 0x1;
    }
    for (i = 0; i < strlen(bit_value); i = i) {
      temp_node = tree_traversal(tree, bit_value, i);
      if (is_leaf(temp_node)) {
        char_count++;
        if (out_flag == 1) {
          write(out_fd, &tree->character, 1);
        }

        else {
          printf("%d", tree->character);
        }
        i += tree->code_size;
      }
      if (char_count == char_number) {
        return 0;
      }
    }
  }
  return 0;
}