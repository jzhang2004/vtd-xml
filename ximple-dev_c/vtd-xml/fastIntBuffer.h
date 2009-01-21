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
//int getCapacityFIB(FastIntBuffer *fib);
#define getCapacityFIB(fib) fib->capacity
// Get the int array corresponding to content of FastIntBuffer 
// with the starting offset and len
int *getIntArray(FastIntBuffer *fib, int offset, int len);

// Get the page size of FastIntBuffer
//int getPageSizeFIB(FastIntBuffer *fib);
#define getPageSizeFIB(fib) fib->pageSize

// Get the int at the index position of FastIntBuffer
extern inline int intAt();

// Replace the value at the index position of FastIntBuffer 
// with newVal
extern inline void modifyEntryFIB(FastIntBuffer *fib, int index, int newVal);

 // Get the size of the FastIntBuffer
//int sizeFIB(FastIntBuffer *fib);
//#define sizeFIB(fib) fib->size

// convert the content of FastIntBuffer to int *
int* toIntArray(FastIntBuffer *fib);

// set the buffer size to zero, capacity untouched,
void clearFastIntBuffer (FastIntBuffer *fib);

// reset the size of fastIntBuffer
Boolean resizeFIB(FastIntBuffer *fib, int newSz);

#endif
