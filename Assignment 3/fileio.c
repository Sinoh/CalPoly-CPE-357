#include "fileio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NUM_ASCII 256
#define ONE_BYTE 1
#define FOUR_BYTE 4
#define EIGHT_BITS 8

/* Write the head to an output file */
void write_header(int fd, struct freq_table *ftable) {
  uint32_t leaf_count = ftable->num_leaf;
  uint8_t character;
  uint32_t freq;
  int i;

  write(fd, &leaf_count, FOUR_BYTE);

  for (i = 0; i < NUM_ASCII; i++) {
    if (ftable->table[i].count > 0) {
      character = (uint8_t)ftable->table[i].character;
      freq = (uint32_t)ftable->table[i].count;
      write(fd, &character, ONE_BYTE);
      write(fd, &freq, FOUR_BYTE);
    }
  }
}

/* Print the header to stdout */
void print_header(struct freq_table *ftable) {
  uint32_t leaf_count = ftable->num_leaf;
  uint8_t character;
  uint32_t freq;
  int i;

  printf("%02x", leaf_count);

  for (i = 0; i < NUM_ASCII; i++) {
    if (ftable->table[i].count > 0) {
      character = (uint8_t)ftable->table[i].character;
      freq = (uint32_t)ftable->table[i].count;
      printf("%02x", character);
      printf("%02x", freq);
    }
  }
}

/* Given an array of 8, appends to the array, if it fills,
    it will store the rest of the data in another buffer to
    appended later */
void make_body(char *original, char *to_append, char *buffer_padding) {
  int i;
  int j = 0;
  int len_original = 0;
  if (strlen(original) > 0) {
    len_original = strlen(original);
  }
  if (len_original < (len_original + strlen(to_append))) {
    if ((len_original + strlen(to_append) >= EIGHT_BITS)) {
      for (i = 0; i < (EIGHT_BITS - len_original); i++) {
        original[len_original] = to_append[i];
      }
      for (i = (EIGHT_BITS - len_original); i < strlen(to_append); i++) {
        buffer_padding[j] = to_append[i];
        j++;
      }
    } else {
      for (i = 0; i < strlen(to_append); i++) {
        original[len_original] = to_append[i];
        len_original++;
      }
    }
  }
}

/* Converts an array of binary numbers to hex */
uint8_t buffer_to_uint(char *buffer) {
  char *out = buffer;
  int num = 0;
  do {
    int b = *out == '1' ? 1 : 0;
    num = (num << 1) | b;
    out++;
  } while (*out);
  return num;
}

/* Create the buffer to be used for writing to output file */
int make_out_buffer(char buffer[], int file_size, struct freq_table *ftable,
                    uint8_t out_buffer[]) {
  int buffer_index = 0;
  int out_buffer_size = 0;
  char padding[1] = {'0'};
  int c;
  int i;
  int j;

  for (i = 0; i < NUM_ASCII; i++) {
    out_buffer_size += (ftable->table[i].code_size * ftable->table[i].count);
  }

  char *original_buffer = malloc(sizeof(original_buffer) * 8);
  for (i = 0; i < file_size; i++) {
    c = buffer[i];
    char *temp_buffer = (malloc(sizeof(temp_buffer) * (8)));
    if (strlen(original_buffer) != EIGHT_BITS) {
      make_body(original_buffer, ftable->table[c].hcode, temp_buffer);
    }
    if (strlen(original_buffer) == EIGHT_BITS || i == file_size - 1) {
      if (i == file_size - 1) {
        while (strlen(original_buffer) < EIGHT_BITS) {
          make_body(original_buffer, padding, temp_buffer);
        }
      }
      out_buffer[buffer_index] = buffer_to_uint(original_buffer);
      buffer_index++;
      free(original_buffer);
      char *original_buffer = malloc(sizeof(original_buffer) * EIGHT_BITS);
      for (j = 0; j < ftable->table[c].code_size; j++) {
        original_buffer[j] = temp_buffer[j];
      }
      free(temp_buffer);
    }
  }

  return buffer_index;
}
