#include "arrayList.h"

//Implementation for functions defined in arrayList.h


// allocate arrayList with initial capacity of 10, 
// return NULL if allocation failed
 ArrayList *createArrayList(){
	exception e;
	int i=0;
	ArrayList *al = (ArrayList*) malloc(sizeof(ArrayList));
	if (al==NULL) {
		e.et = out_of_mem;
		e.msg = "ArrayList allocation failed ";
		Throw e;
	}
	
	al->storage = (void **)malloc(10 * sizeof(void *)); // initial capacity of 10;
	if (al->storage == NULL){
		free(al);
		e.et = out_of_mem;
		e.msg = "ArrayList allocation failed ";
		Throw e;
	}
	for ( i=0;i<10;i++){
		al->storage[i] = NULL;
	}
    al->capacity = 10;
    al->size = 0;
    return al;
}

 // create ArrayList with initialCapacity
 // return NULL if allocation failed

 ArrayList *createArrayList2(int initialCapacity){
	 exception e;
	 int i;
	ArrayList *al = (ArrayList*) malloc(sizeof(ArrayList));
	if (al==NULL) {
		e.et = out_of_mem;
		e.msg = "ArrayList allocation failed ";
		Throw e;
	}
	al->storage = (void **)malloc(initialCapacity * sizeof(void *)); // initial capacity of 10;
	if (al->storage == NULL){
		free(al);
		e.et = out_of_mem;
		e.msg = "ArrayList allocation failed ";
		Throw e;
	}

	for (i=0;i<initialCapacity;i++)
		al->storage[i] = NULL;
    al->capacity = 10;
    al->size = 0;
    return al;
}
// free ArrayList
 void freeArrayList(ArrayList *al){
	int i=0;
	if (al!=NULL){
	for (i=0;i<al->size;i++)
		free(al->storage[i]); // it is ok, only pointers to mem blocks are stored
		free(al->storage);
	}
	free(al);
}

// add the element to the end of the storage, 
// return the status of add
// 1 if ok, 0 if failed
 int add(ArrayList *al, void *element){
	exception e;
	int t = 0,k=0;
	void **v=NULL;
	if (al->size < al->capacity){
		al->storage[al->size] = element;
		al->size++;
		return al->size;
	}
	else{
		t = al->capacity + AL_GROW_INC;
		v = (void **)malloc(t*sizeof(void *));
		if (v==NULL) {
			e.et = out_of_mem;
			e.msg = "ArrayList add failed ";
			Throw e;
		} // add failed
	
		
		for (k=0;k<al->size;k++)
		{
               v[k] = al->storage[k]; // copy content
		}
		for (k=al->size;k<al->capacity;k++){
			v[k] = NULL;			// the remaining ones set to NULL
		}
		v[al->size]=element;
		al->capacity = t;
		al->size++;
		free(al->storage);
		al->storage = v;
		return al->size;
	}
}
// enforcing in-the-range index value is left to the application logic
// get the object pointer at the index position
//(user program has to perform out-of-range checking)

 void *get(ArrayList *al, int index){
	return al->storage[index];
}