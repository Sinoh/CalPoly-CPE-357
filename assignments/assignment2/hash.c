#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 20
#define half 0.5

struct DataItem {
    int count;
    char key;
};

/*
struct DataItem {
   int data;   
   int key;
};
*/

struct DataItem* hashArray[SIZE]; 
struct DataItem* dummyItem;
struct DataItem* item;


int hashCode(char* key) {
    int i;
    int hashNumber = 0;
    int prime[5] = {2, 3, 5, 7, 9};

    for (i = 0; key[i] != '\0'; i++) {
        hashNumber += (int)key[i] * prime[i % 5];
    }

    return hashNumber;
}


/*
int hashCode(int key) {
    return key % SIZE;
}
*/
 

struct DataItem *search(char* key) {
    // get the hash
    int hashNumber = hashCode(key);
    int hashIndex = hashNumber % SIZE;

    // move in array until an empty
    while(hashArray[hashIndex] != NULL) {
        
        if (hashArray[hashIndex]->key == hashIndex){
            return hashArray[hashIndex];
        }

        // else go to next cell
        ++hashIndex;

        //wrap around the table
        hashIndex %= SIZE;
    }
    return NULL;
}

/*
struct DataItem *search(int key) {
   //get the hash 
   int hashIndex = hashCode(key);  
	
   //move in array until an empty 
   while(hashArray[hashIndex] != NULL) {
	
      if(hashArray[hashIndex]->key == key)
         return hashArray[hashIndex]; 
			
      //go to next cell
      ++hashIndex;
		
      //wrap around the table
      hashIndex %= SIZE;
   }        
	
   return NULL;        
}

*/

void insert(char* key, int count) {
    struct DataItem *item = (struct DataItem*) malloc(sizeof(struct DataItem));
    if (item == NULL) {
        perror("Malloc Failed");
    }
    
    // get hash
    int hashNumber = hashCode(key);
    int hashIndex = hashNumber % SIZE;

    item->count = count;
    item->key = hashNumber;

    // move in array until empty or deleted cell
    while (hashArray[hashIndex] != NULL && hashArray[hashIndex]->key != -1) {
        // got tonext cell
        ++hashIndex;

        //wrap around the table
        hashIndex %= SIZE;
    }

    hashArray[hashIndex] = item;
/*
    if ((sizeof(DataItem) / sizeof(int)) > half) {
        
    }
*/
}

/*
void insert(int key,int data) {

   struct DataItem *item = (struct DataItem*) malloc(sizeof(struct DataItem));
   item->data = data;  
   item->key = key;

   //get the hash 
   int hashIndex = hashCode(key);

   //move in array until an empty or deleted cell
   while(hashArray[hashIndex] != NULL && hashArray[hashIndex]->key != -1) {
      //go to next cell
      ++hashIndex;
		
      //wrap around the table
      hashIndex %= SIZE;
   }
	
   hashArray[hashIndex] = item;
}
*/

struct DataItem* delete(char* key) {

    // get the hash
    int hashIndex = hashCode(key);

    // move in array until an empty
    while(hashArray[hashIndex] != NULL) {
    
        if(hashArray[hashIndex]-> key == *key) {
            struct DataItem* temp = hashArray[hashIndex];

            // assign a summy item at delted postion
            hashArray[hashIndex] = dummyItem;
            return temp;
        }

        // go to next cell
        ++hashIndex;

        //wrap around the table
        hashIndex %= SIZE;
    }
    
    return NULL;
}
/*
struct DataItem* delete(struct DataItem* item) {
   int key = item->key;

   //get the hash 
   int hashIndex = hashCode(key);

   //move in array until an empty
   while(hashArray[hashIndex] != NULL) {
	
      if(hashArray[hashIndex]->key == key) {
         struct DataItem* temp = hashArray[hashIndex]; 
			
         //assign a dummy item at deleted position
         hashArray[hashIndex] = dummyItem; 
         return temp;
      }
		
      //go to next cell
      ++hashIndex;
		
      //wrap around the table
      hashIndex %= SIZE;
   }      
	
   return NULL;        
}
*/

void display() {
   int i = 0;
	
   for(i = 0; i<SIZE; i++) {
	
      if(hashArray[i] != NULL)
         printf(" (%d,%d)",hashArray[i]->key,hashArray[i]->count);
      else
         printf(" ~~ ");
   }
	
   printf("\n");
}

int main() {
   dummyItem = (struct DataItem*) malloc(sizeof(struct DataItem));
   dummyItem->count = -1;  
   dummyItem->key = -1; 

   insert("this", 20);
  // insert("is", 70);
  // insert("very", 80);
  // insert("stupid", 25);
  // insert("hello", 44);
  // insert("world", 32);
  // insert("i", 11);
  // insert("I", 78);
  // insert("sleep", 97);

   display();
   item = search("cannot");

   if(item != NULL) {
      printf("Element found: %d\n", item->count);
   } else {
      printf("Element not found\n");
   }

   delete("cannot");
   item = search("acutally");

   if(item != NULL) {
      printf("Element found: %d\n", item->count);
   } else {
      printf("Element not found\n");
   }
}


/*
int main() {
   dummyItem = (struct DataItem*) malloc(sizeof(struct DataItem));
   dummyItem->data = -1;  
   dummyItem->key = -1; 

   insert(1, 20);
   insert(2, 70);
   insert(42, 80);
   insert(4, 25);
   insert(12, 44);
   insert(14, 32);
   insert(17, 11);
   insert(13, 78);
   insert(37, 97);

   display();
   item = search(37);

   if(item != NULL) {
      printf("Element found: %d\n", item->data);
   } else {
      printf("Element not found\n");
   }

   delete(item);
   item = search(37);

   if(item != NULL) {
      printf("Element found: %d\n", item->data);
   } else {
      printf("Element not found\n");
   }
}*/
