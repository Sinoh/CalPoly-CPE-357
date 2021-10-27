#include "fileio.h"
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define NUM_ASCII 256
#define BUF_SIZE 4096

int main(int argc, char *argv[]) {
  FILE *file;
  int in_fd, out_fd;
  int i, file_size;
  int out_flag = 0;
  char buffer[BUF_SIZE];
  uint8_t hcode;
  uint8_t out_buffer[BUF_SIZE];

  /* Check the appropriate number of agrumens are passed */
  if (argc < 2 || argc > 3) {
    printf("hencode infile [ outfile ]");
    exit(0);
  }

  /* opens and creates a file descriptor for the input */
  in_fd = open(argv[1], O_RDONLY);
  if (in_fd == -1) {
    perror("Infile Open Error");
    exit(0);
  }

  /* Opens and creates a file descriptor for the output */
  if (argc == 3) {
    out_fd = open(argv[2], O_WRONLY | O_CREAT, 0644);
    if (out_fd == -1) {
      perror("Outfile Open Error");
      exit(0);
    }
    /* Checks a output flag, will write to output */
    out_flag = 1;
  }

  /* Finds the file size, number of characters */
  file = fopen(argv[1], "r");
  fseek(file, 0L, SEEK_END);
  file_size = ftell(file);
  fclose(file);

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

  struct header *head = ((struct header *)malloc(sizeof(struct header)));
  if (ftable == NULL) {
    perror("Header Malloc Error");
    exit(1);
  }

  ftable->table = htable;

  /* read in to a buffer */
  read(in_fd, &buffer, BUF_SIZE);

  /* Create a table of counts, leaves, and hcodes */
  create_freq_table(buffer, file_size, ftable);

  /* If out flag is checked, write to output file
      else, write to stdout */
  if (out_flag == 1) {
    /* write the header to output file */
    write_header(out_fd, ftable);
    /* Create a buffer for the body */
    int buffer_size = make_out_buffer(buffer, file_size, ftable, out_buffer);
    for (i = 0; i < buffer_size; i++) {
      hcode = out_buffer[i];
      write(out_fd, &hcode, 1);
    }

  } else {
    /* Print header */
    print_header(ftable);
    /* Create body buffer to print */
    int buffer_size = make_out_buffer(buffer, file_size, ftable, out_buffer);
    for (i = 0; i < buffer_size; i++) {
      hcode = out_buffer[i];
      printf("%02x", hcode);
    }
  }

  /* Close files and free memory */
  close(in_fd);
  if (out_flag) {
    close(out_fd);
  }
  free(ftable->htree);
  free(ftable);
  free(htable);
  free(head);

  return 0;
}
