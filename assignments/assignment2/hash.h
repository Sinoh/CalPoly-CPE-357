#ifndef HEADER_FILE
#define HEADER_FILE

struct DataItem {
   int count;
   char* key;
};

struct HashTable* insert(struct HashTable* hashArray, char *key);


#endif
