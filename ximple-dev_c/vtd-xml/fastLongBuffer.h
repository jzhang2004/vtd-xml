/* 
 * Copyright (C) 2002-2009 XimpleWare, info@ximpleware.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
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
//int getCapacityFLB(FastLongBuffer *flb);
#define getCapacityFLB(flb) flb->capacity

// Return a selected chuck of long buffer as a long array.
Long *getLongArray(FastLongBuffer *flb, int offset, int len);

// get the page size of FastLongBuffer
//int getPageSizeFLB(FastLongBuffer *flb);
#define getPageSizeFLB(flb) flb->pageSize

// get the long at the index position from FastLongBuffer
//Long longAt(FastLongBuffer *flb, int index);
extern inline Long longAt(FastLongBuffer *flb, int index);

// get the lower 32 bits from the index position from FastLongBuffer
extern inline int lower32At(FastLongBuffer *flb, int index);


// get the upper 32 bits from the index position from FastLongBuffer 
extern inline int upper32At(FastLongBuffer *flb, int index);

// replace the entry at the index position of FastLongBuffer with l
extern inline void modifyEntryFLB(FastLongBuffer *flb, int index, Long l);

// convert FastLongBuffer into a Long array 
Long* toLongArray(FastLongBuffer *flb);

// set the buffer size to zero, capacity untouched,
void clearFastLongBuffer (FastLongBuffer *flb);

#endif
