#ifndef INTBUFFER_H
#define INTBUFFER_H
#include "customTypes.h"
#include "arrayList.h"
#include <stdlib.h>

typedef struct fastIntBuffer{
	ArrayList *al;
	int capacity;
	int exp;
	int pageSize;
	int r;
	int size;
} FastIntBuffer;


// Create FastIntBuffer with initial page size of 1024 ints
FastIntBuffer *createFastIntBuffer();

// Create FastIntBuffer with initial page size of (1<<e) ints
FastIntBuffer *createFastIntBuffer2(int exp);

// Free FastIntBuffer
void freeFastIntBuffer(FastIntBuffer *fib);

// Append i to the end of FastIntBuffer
void appendInt(FastIntBuffer *fib, int i);

// Append int array of length "len" to the end of fastIntBuffer
void appendIntArray(FastIntBuffer *fib, int* i, int len);

// Get the capacity of FastIntBuffer
int getCapacityFIB(FastIntBuffer *fib);

// Get the int array corresponding to content of FastIntBuffer 
// with the starting offset and len
int *getIntArray(FastIntBuffer *fib, int offset, int len);

// Get the page size of FastIntBuffer
int getPageSizeFIB(FastIntBuffer *fib);

// Get the int at the index position of FastIntBuffer
int intAt(FastIntBuffer *fib, int index);

// Replace the value at the index position of FastIntBuffer 
// with newVal
 void modifyEntryFIB(FastIntBuffer *fib, int index, int newVal);

 // Get the size of the FastIntBuffer
int sizeFIB(FastIntBuffer *fib);

// convert the content of FastIntBuffer to int *
int* toIntArray(FastIntBuffer *fib);

#endif