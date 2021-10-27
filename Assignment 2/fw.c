#include "hash.h"
include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WORD_OUTPUT 10
#define SIZE 20

void get_words(struct HashTable *array, FILE *file) {
  char c_in;
  int i;
  char word[SIZE] = { 0 };

  c_in = i = 0;

  while ((c_in = fgetc(file)) != EOF) { // grab each char from stdini
    if (feof(file)) {
      break;
    }
    if (isalpha(c_in)) {       // If a letter
      word[i] = tolower(c_in); // lower it
      i++;
    } else {          // if not char
      word[i] = '\0'; // set end of word to null character
      i = 0;
      if (!isalpha(word[0])) { // If not a word
        continue;              // Not a word and continue grabbing chars
      } else {                 // search for word
        array = insert(array, word);
      }
    }
  }
}

void print_out(struct HashTable *array, int num_out,
               int (*compar)(const void *, const void *)) {
  int i, idx;
  i = idx = 0;

  struct Bucket *newArray =
      ((struct Bucket *)malloc(sizeof(struct Bucket) * array->elements));

  for (i = 0; i < array->size; i++) {
    if (array->item[i].key == NULL) {
      continue;
    } else {
      newArray[idx] = array->item[i];
      idx++;
    }
  }

  qsort(newArray, array->elements, sizeof(struct Bucket), compar);

  printf("The top %d words (out of %d) are:\n", num_out, array->elements);
  for (i = 0; i < num_out; ++i) {
    if (i == array->elements) {
      break;
    }
    printf("%9d %s\n", newArray[i].count, newArray[i].key);
  }
}

int compare(const void *left, const void *right) {
  const struct Bucket *lefts = (const struct Bucket *)left;
  const struct Bucket *rights = (const struct Bucket *)right;

  if (lefts->count == rights->count) {
    return strcmp(rights->key, lefts->key);
  } else {
    return rights->count - lefts->count;
  }
}

int main(int argc, char *argv[]) {
  FILE *fp;
  int arg_files[argc - 1];
  int word_o = WORD_OUTPUT;
  int arg, arg_f_i, i, index;
  struct HashTable *hashArray =
      ((struct HashTable *)malloc(sizeof(struct HashTable)));
  struct Bucket *buckets =
      ((struct Bucket *)calloc(sizeof(struct Bucket), SIZE));

  hashArray->item = buckets;
  hashArray->elements = 0;
  hashArray->size = SIZE;

  fp = NULL;
  i = index = arg_f_i = 0;

  for (arg = 1; arg < argc; arg++) {
    if (!strcmp(argv[arg], "-n")) {   // check if there is a '-n' argument
      if (isdigit(*argv[arg + 1])) {  // check if num is a digit
        word_o = atoi(argv[arg + 1]); // change word output to num
        arg++;
      } else { // else give error usage
        fprintf(stderr, "usage: fw [-n num] [ file1 [ file 2 ...] ]\n");
        return 1;
      }
    } else {                    // if no '-n' argument then
      arg_files[arg_f_i] = arg; // set arg file index to argv index
      arg_f_i++;                // increment index for arg file index
    }
  }

  if (arg_f_i > 0) { // if num of files in argv is greater than 0
    for (arg = 0; arg < arg_f_i; arg++) {
      if ((fp = fopen(argv[arg_files[arg]], "r")) ==
          NULL) { // if file doesn't exist, give error
        perror(argv[arg_files[arg]]);
        continue;                 // continues loop
      } else {                    // if file exists
        get_words(hashArray, fp); // get words from files
        print_out(hashArray, word_o, compare);
        fclose(fp);
      }
    }
  } else if (arg_f_i == 0) {     // if no file names in arg
    get_words(hashArray, stdin); // get words from stdin
    print_out(hashArray, word_o, compare);
  }
  return 0;
}
