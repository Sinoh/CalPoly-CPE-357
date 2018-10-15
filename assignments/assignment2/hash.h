#ifndef HEADER_FILE
#define HEADER_FILE

struct HashTable{
   int elements;
   int size;
   struct Bucket *item;
};

struct Bucket{
   int count;
   char *key;
};

int search(struct HashTable* hashArray, char* key);
struct HashTable* insert(struct HashTable* hashArray, char* key);
void display(struct HashTable* hashArray);

#endif
