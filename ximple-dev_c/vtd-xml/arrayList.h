#ifndef AL_H
#define AL_H

#include "customTypes.h"
#include <stdlib.h>
// This define the arrayList struct and functions
// arrayList grows automatically when new buffers are added
// functions defined in this file allows one to create, de-allocate
// and add elements to the arrayList


#define AL_GROW_INC 16;
typedef struct arrayList {
	int capacity; // capacity of the arrayList
	int size;   // actual size
	void **storage; // where object pointers are stored
	               // storage needs to grow automatically
}ArrayList;

// that is all we need for now, more can be defined later

// allocate arrayList with initial capacity of 10, 
// return NULL if allocation failed
ArrayList* createArrayList(); 						  

// allocate arrayList with initial capacity, 
// return NULL if allocation failed
ArrayList* createArrayList2(int initialCapacity); 

// garbage collect arrayList
void freeArrayList(ArrayList* al); 

// add the element to the end of the storage, 
// return the status of add
// 1 if ok, 0 if failed
int add(ArrayList* al, void* element);

// get the object pointer at the index position
//(user program has to perform out-of-range checking)
void* get(ArrayList* al, int index);

#endif