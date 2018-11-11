#ifndef BTREE_H
#include "btree.h"
#endif

#ifndef FILEIO_H
#define FILEIO_H

#include <stdint.h>

struct header {
  uint32_t size;
  struct character *characters;
};

struct character {
  uint8_t letter;
  uint32_t count;
};

void write_header(int fd, struct freq_table *ftable);
void make_body(char *original, char *to_append, char *buffer_padding);
uint8_t buffer_to_uint(char *buffer);
int make_out_buffer(char buffer[], int file_size, struct freq_table *ftable,
                    uint8_t out_buffer[]);
void print_header(struct freq_table *ftable);

#endif
