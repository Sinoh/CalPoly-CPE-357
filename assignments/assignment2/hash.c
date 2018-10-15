#include "hash.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int hashCode(char *key) {
  int i;
  int hashNumber = 0;
  int prime[5] = {2, 3, 5, 7, 9};

  for (i = 0; key[i] != '\0'; i++) {
    hashNumber += (int)key[i] * prime[i % 5];
  }

  return hashNumber;
}

struct HashTable *resize(struct HashTable *oldTable) {
  int i;

  struct HashTable *newTable =
      (struct HashTable *)malloc(sizeof(struct HashTable));
  struct Bucket *newBuckets =
      ((struct Bucket *)malloc(sizeof(struct Bucket) * oldTable->size * 2));
  newTable->item = newBuckets;
  newTable->elements = oldTable->elements;
  newTable->size = oldTable->size * 2;

  if (newTable == NULL) {
    perror("Malloc FAILED");
  }

  for (i = 0; i < oldTable->size; i++) {
    if (oldTable->item[i].key != NULL) {
      newTable = insert(newTable, oldTable->item[i].key);
      newTable->elements--;
      newTable->item[search(newTable, oldTable->item[i].key)].count =
          oldTable->item[i].count;
    }
  }
  return newTable;
}

int search(struct HashTable *hashArray, char *key) {
  // get the hash
  int item = hashCode(key) % hashArray->size;
  int hashIndex = item;

  // move in array until an empty
  while (hashArray->item[hashIndex].key != NULL) {

    // compares the key within the struct to the key passed, if the same
    // return
    if (!strcmp(hashArray->item[hashIndex].key, key)) {
      return hashIndex;
    }

    // else go to next cell
    hashIndex++;

    // wrap around the table
    hashIndex %= hashArray->size;
  }
  return 0;
}

struct HashTable *insert(struct HashTable *hashArray, char *key) {
  // get hash
  int hashIndex = hashCode(key) % hashArray->size;

  if (hashArray->item[hashIndex].key == NULL) {
  } else {
    while (hashArray->item[hashIndex].key != NULL) {
      if (!strcmp(hashArray->item[hashIndex].key, key)) {
        hashArray->item[hashIndex].count++;
        return hashArray;
      } else {
        hashIndex++;
      }
    }
  }

  hashArray->item[hashIndex].key = malloc(strlen(key) + 1);
  hashArray->item[hashIndex].key = strdup(key);
  hashArray->item[hashIndex].count = 1;
  hashArray->elements++;

  if (hashArray->elements > hashArray->size / 2) {
    hashArray->item = resize(hashArray)->item;
    hashArray->size = hashArray->size * 2;
  }

  return hashArray;
}

struct Bucket *delete (struct HashTable *hashArray, char *key) {

  // get the has
  int hashIndex = hashCode(key) % hashArray->size;
  struct Bucket *temp =
      (struct Bucket *)malloc(sizeof(struct Bucket) * hashArray->size);
  // move in array until an empty
  while (hashArray->item[hashIndex].key != NULL) {

    if (!strcmp(hashArray->item[hashIndex].key, key)) {

      temp[hashIndex].key = hashArray->item[hashIndex].key;
      temp[hashIndex].count = hashArray->item[hashIndex].count;
      // assign a summy item at deleted postion
      hashArray->item[hashIndex].key = NULL;

      hashArray->elements--;
      return temp;
    }

    // go to next cell
    ++hashIndex;
    // wrap around the table
    hashIndex %= hashArray->size;
  }

  return NULL;
}

/*int sort(struct HashTable* hashArray){
    int newArray[hashArray->elements];
    int index;
    int i;

    for (i = 0; i < hashArray->size; i++) {
        if (hashArray->item[i].key != NULL) {
            newArray[index] = hashArray->item[i];
            index++;
        }
    }
    return newArray;
}*/

void display(struct HashTable *hashArray) {
  int i;

  for (i = 0; i < hashArray->size; i++) {

    if (hashArray->item[i].key != NULL)
      printf(" (%s,%d)", hashArray->item[i].key, hashArray->item[i].count);
    else
      printf(" ~~ ");
  }

  printf("\n");
}

/* int main() {
    //Pointer to array
    struct HashTable* hashArray = ((struct HashTable*) malloc(sizeof(struct
HashTable)));
    struct Bucket* buckets = ((struct Bucket*) malloc(sizeof(struct Bucket) *
20));
    hashArray->item = buckets;
    hashArray->elements = 0;
    hashArray->size = 20;

    hashArray = insert(hashArray, "this");
    hashArray = insert(hashArray,"is");
    hashArray = insert(hashArray,"very");
    hashArray = insert(hashArray,"stupid");
    hashArray = insert(hashArray,"hello");
    hashArray = insert(hashArray,"world");
    hashArray = insert(hashArray,"i");
    hashArray = insert(hashArray,"I");
    hashArray = insert(hashArray,"sleep");

   // display(hashArray);
    delete(hashArray,"sleep");
   // display(hashArray);
    hashArray = insert(hashArray,"sleep");
    hashArray = insert(hashArray,"sleep");
   // display(hashArray);
    hashArray = insert(hashArray,"we");
   // display(hashArray);
    hashArray = insert(hashArray,"are");
    display(hashArray);
    hashArray = insert(hashArray,"sleep");
    hashArray = insert(hashArray,"are");
    display(hashArray);
}*/
