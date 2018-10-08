#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 20

struct DataItem {
    int count;
    char* key;
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

/*
struct DataItem* resize(struct DataItem hashArray, int arraySize) {
    int i;
    int newSize = arraySize * 2;
    struct DataItem* temp[newSize];
    

    for (i = 0; i < arraySize; i ++) {
        *item = hashArray;
        

        if (item != NULL) {
            int hashIndex = hashCode(item->key) % SIZE;

            while (temp[hashIndex] != NULL) {

                ++hashIndex;

                hashIndex %= newSize;
            }

        temp[hashIndex] = item;
        }
    }
    return *temp;

}

*/

struct DataItem *search(char* key) {
    // get the hash
    int hashIndex = hashCode(key) % SIZE;

    // move in array until an empty
    while(hashArray[hashIndex] != NULL) {
       
       // compares the key within the struct to the key passed, if the same
       // return 
        if (!strcmp(hashArray[hashIndex]->key, key)) {
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

void insert(char* key, int count, int elementsCount) {
    int i;


    struct DataItem *item = (struct DataItem*) malloc(sizeof(struct DataItem));
    if (item == NULL) {
        perror("Malloc Failed");
    }
    
    // get hash
    int hashIndex = hashCode(key) % SIZE;

    item->key = malloc(strlen(key)+1);

    item->count = count;
    item->key = strdup(key);

    // move in array until empty or deleted cell
    while (hashArray[hashIndex] != NULL) {
        // got tonext cell
        ++hashIndex;

        //wrap around the table
        hashIndex %= SIZE;
    }

    hashArray[hashIndex] = item;
    elementsCount++;

    if (elementsCount > sizeof(hashArray) / 2) {
       // hashArray = resize(hashArray, sizeof(hashArray) * 2);
        
        struct DataItem* temp[sizeof(hashArray) * 2];

        for (i = 0; i < sizeof(hashArray); i ++) {
            item = hashArray[i];
            
            if (item != NULL) {

                int hashIndex = hashCode(item->key) % sizeof(temp);
                
                while (temp[hashIndex] != NULL) {
                    ++hashIndex;
                    hashIndex %= sizeof(temp);
                }
            }
            temp[hashIndex] = item;
        }
        hashArray = struct DataItem* hashArray[sizeof(temp)];
        hashArray = temp;
    }

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

struct DataItem* delete(char* key, int elementsCount) {

    // get the hash
    int hashIndex = hashCode(key) % SIZE;

    // move in array until an empty
    while(hashArray[hashIndex] != NULL) {
    
        if(!strcmp(hashArray[hashIndex]->key, key)) {
            struct DataItem* temp = hashArray[hashIndex];

            // assign a summy item at delted postion
            hashArray[hashIndex] = NULL;
            elementsCount --;
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
         printf(" (%s,%d)",hashArray[i]->key,hashArray[i]->count);
      else
         printf(" ~~ ");
   }
	
   printf("\n");
}

int main() {
    int elementsCount = 0;

   insert("this", 20, elementsCount);
   insert("is", 70, elementsCount);
   insert("very", 80,elementsCount);
   insert("stupid", 25,elementsCount);
   insert("hello", 44, elementsCount);
   insert("world", 32, elementsCount);
   insert("i", 11, elementsCount);
   insert("I", 78, elementsCount);
   insert("sleep", 97, elementsCount);

   display();
   item = search("sleep");

   if(item != NULL) {
      printf("Element found: %d\n", item->count);
   } else {
      printf("Element not found\n");
   }

   delete("sleep", elementsCount);
   display();
   item = search("sleep");

   if(item != NULL) {
      printf("Element found: %d\n", item->count);
   } else {
      printf("Element not found\n");
   }

   insert("sleep", 97, elementsCount);
   display();
   insert("we", 10, elementsCount);
   display();
   insert("are", 20, elementsCount);
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
