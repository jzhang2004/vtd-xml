#ifndef LONGBUFFER_H
#define LONGBUFFER_H
// Header file for FastLongBuffer
// Long is a 64 bit int, so in C it is really long long
#include "arrayList.h"
#include "customTypes.h"
#include <stdlib.h>

typedef struct fastLongBuffer{
	ArrayList *al;
	int capacity;
	int exp;
	int pageSize;
	int r;
	int size;
} FastLongBuffer;


// create FastLongBuffer with default page size of 1024 longs 
FastLongBuffer *createFastLongBuffer();

// create FastLongBuffer with page size of (1<<e) longs
FastLongBuffer *createFastLongBuffer2(int e);

// create FastLongBuffer with page size of (1<<e) longs and initial capciaty of c longs
FastLongBuffer *createFastLongBuffer3(int e, int c);

// free FastLongBuffer 
void freeFastLongBuffer(FastLongBuffer *flb);

// append a long array to the end of FastLongBuffer
void appendLongArray(FastLongBuffer *flb, Long *longArray, int len);

// append a long to the end of FastLongBuffer
void appendLong(FastLongBuffer *flb, Long i);

// get the capacity of FastLongBuffer
int getCapacityFLB(FastLongBuffer *flb);

// Return a selected chuck of long buffer as a long array.
Long *getLongArray(FastLongBuffer *flb, int offset, int len);

// get the page size of FastLongBuffer
int getPageSizeFLB(FastLongBuffer *flb);

// get the long at the index position from FastLongBuffer
//Long longAt(FastLongBuffer *flb, int index);
inline Long longAt(FastLongBuffer *flb, int index){
	int pageNum = (index >>flb->exp);
    // int offset = index % r;
    int offset = index & flb->r;
    //return ((Long[]) bufferArrayList.get(pageNum))[offset];
	return ((Long *)get(flb->al,pageNum))[offset];
}

// get the lower 32 bits from the index position from FastLongBuffer
int lower32At(FastLongBuffer *flb, int index);

// get the upper 32 bits from the index position from FastLongBuffer 
int upper32At(FastLongBuffer *flb, int index);

// replace the entry at the index position of FastLongBuffer with l
void modifyEntryFLB(FastLongBuffer *flb, int index, Long l);

// convert FastLongBuffer into a Long array 
Long* toLongArray(FastLongBuffer *flb);

#endif