#ifndef CONTEXTBUFFER_H
#define CONTEXTBUFFER_H

#include <stdlib.h>
#include "arrayList.h"
#include "customTypes.h"

//data structure for the global stack used in VTDNav
typedef struct contextBuffer{
	ArrayList *al;
	int capacity;
	int incSize;
	int n;
	int pageSize;
	int r;
	int size;
} ContextBuffer;

// Create ContextBuffer with incSize of i
ContextBuffer *createContextBuffer(int i);
// Create ContextBuffer with page size (1<<e) and increment Size of i
ContextBuffer *createContextBuffer2(int e, int i);
// Free ContextBuffer
void freeContextBuffer(ContextBuffer *cb);
// Pop the content value back into an integer array.
Boolean load(ContextBuffer *cb, int* output);
// Push the array content on to the stack.
void store(ContextBuffer *cb, int *input);
// Manage the buffer size to reduce unused spaces
void resize(ContextBuffer *cb);

#endif