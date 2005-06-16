/* 
 * Copyright (C) 2002-2004 XimpleWare, info@ximpleware.com
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
#include "fastIntBuffer.h"

// Create FastIntBuffer with initial page size of 1024 ints
FastIntBuffer *createFastIntBuffer(){
	exception e;
	FastIntBuffer *fib = NULL;
	ArrayList *al= createArrayList();
	if (al==NULL){
		e.et = out_of_mem;
		e.msg = "FastIntBuffer allocation failed ";
		Throw e;
	}

	fib = (FastIntBuffer *)malloc(sizeof(FastIntBuffer));
	if (fib==NULL) {
		freeArrayList(al); 
		e.et = out_of_mem;
		e.msg = "FastIntBuffer allocation failed ";
		Throw e;	}

	fib->size = 0;
	fib->capacity = 0;
	fib->pageSize = 1<<10;
	fib->exp = 10;
	fib->r = 1023;
	fib->al = al;
	return fib;
}

// Create FastIntBuffer with initial page size of (1<<e) ints
FastIntBuffer *createFastIntBuffer2(int exp){
	exception e;
	FastIntBuffer *fib = NULL;
	ArrayList *al= createArrayList();
	if (al==NULL){
		e.et = out_of_mem;
		e.msg = "FastIntBuffer allocation failed ";
		Throw e;
	}
	fib = (FastIntBuffer *)malloc(sizeof(FastIntBuffer));
	if (fib==NULL) {
		freeArrayList(al); 
		e.et = out_of_mem;
		e.msg = "FastIntBuffer allocation failed ";
		Throw e;
	}

	fib->size = 0;
	fib->capacity = 0;
	fib->pageSize = 1<<exp;
	fib->exp = exp;
	fib->r = (1<<exp) - 1;
	fib->al = al;
	return fib;
}

// Free FastIntBuffer
void freeFastIntBuffer(FastIntBuffer *fib){
	if (fib != NULL) 
		freeArrayList(fib->al);
	free(fib);
}

// Append i to the end of FastIntBuffer
void appendInt(FastIntBuffer *fib, int i){
	exception e;
	int* lastBuffer = NULL;
    //if (bufferArrayList.size() == 0) {
    if (fib->al->size == 0) {
		//lastBuffer = new int[pageSize];
        //bufferArrayList.add(lastBuffer);
        //capacity = pageSize;

		lastBuffer = (int *)malloc(sizeof(int)<<fib->exp);
		if (lastBuffer == NULL){
			e.et = out_of_mem;
			e.msg = " append int failed in FastIntBuffer";
			Throw e;
		}
        add(fib->al,lastBuffer);
        fib->capacity = fib->pageSize;
    } else {
        //lastBuffer = (int[]) bufferArrayList.get(bufferArrayList.size() - 1);
        lastBuffer = (int *) get(fib->al,fib->al->size-1);

    }
    if ((fib->size + 1) <= fib->capacity) {
        //get the last buffer from the bufferListArray
        //obtain the starting offset in that buffer to which the data is to be copied
        //update length
        //System.arraycopy(long_array, 0, lastBuffer, size % pageSize, long_array.length);
        lastBuffer[fib->size & fib->r] = i;
//        lastBuffer[size % pageSize] = i;
        fib->size += 1;
    } else // new buffers needed
        {
        //int[] newBuffer = new int[pageSize];
        //size++;
        //capacity += pageSize;
        //bufferArrayList.add(newBuffer);
        //newBuffer[0] = i;
		
		int *newBuffer = (int *)malloc(sizeof(int)<<fib->exp);
		if (newBuffer == NULL){
			e.et = out_of_mem;
			e.msg = " append int failed in FastIntBuffer";
			Throw e;
		}
		fib->size++;
		fib->capacity += fib->pageSize;
		add(fib->al,newBuffer);
        newBuffer[0] = i;		
	}
}

// Append int array of length "len" to the end of fastIntBuffer
void appendIntArray(FastIntBuffer *fib, int* int_array, int len){
	exception e;
	int *lastBuffer=NULL;
	if (int_array == NULL || len <0) {
		e.et = invalid_argument;
		e.msg = "invalid argument for appendIntArray in FastIntBuffer";
        //throw new NullPointerException();
		Throw e;
    }
    // no additional buffer space needed

    //if (bufferArrayList.size() == 0) {
	if (fib->al->size == 0){
        //lastBuffer = new int[pageSize];
		lastBuffer = (int *)malloc(sizeof(int)<<fib->exp);
		if(lastBuffer==NULL){
			e.et = invalid_argument;
			e.msg = "invalid argument for appendIntArray in FastIntBuffer";
			Throw e;
		}       
		//bufferArrayList.add(lastBuffer);
        //capacity = pageSize;
		add(fib->al,lastBuffer);
		fib->capacity = fib->pageSize;
    } else {
        //lastBuffer = (int[]) bufferArrayList.get(bufferArrayList.size() - 1);
		lastBuffer = (int *)get(fib->al,fib->al->size -1);
    }

    if ((fib->size + len) < fib->capacity) {
        //get the last buffer from the bufferListArray
        //obtain the starting offset in that buffer to which the data is to be copied
        //update length

        /*System.arraycopy(
            int_array,
            0,
            lastBuffer,
            //size % pageSize,
            size & r,
            int_array.length);*/
		memcpy(lastBuffer+(fib->size&fib->r), int_array, len<<2);
        fib->size += len;
    } else // new buffers needed
        {
		int i;
        // compute the number of additional buffers needed
//        int n =
//            ((int) ((int_array.length + size) / pageSize))
//                + (((int_array.length + size) % pageSize) > 0 ? 1 : 0)
//                - (int) (capacity / pageSize);
        /*int n =
              ((int_array.length + size) >> exp)
                + (((int_array.length + size) &r) > 0 ? 1 : 0)
                -  (capacity >> exp);*/

		int n = ((len + fib->size)>>fib->exp)
			+(((len + fib->size) & fib->r) > 0 ? 1:0)
			- (fib->capacity >> fib->exp);
        // create these buffers
        // add to bufferArrayList

        //System.arraycopy(int_array, 0, lastBuffer, size % pageSize, capacity - size);
        //System.arraycopy(int_array, 0, lastBuffer, size& r, capacity - size);
		memcpy(lastBuffer+(fib->size & fib->r), 
			int_array,
			fib->capacity - fib->size);

        for (i = 0; i < n; i++) {
            //int[] newBuffer = new int[pageSize];
			int *newBuffer = (int *)malloc(sizeof(int)<<fib->exp);
            if (i < n - 1) {
                // full copy 
                /*System.arraycopy(
                    int_array,
                    pageSize * i + capacity - size,
                    newBuffer,
                    0,
                    pageSize);*/
				memcpy(newBuffer,
					int_array + (i<<fib->exp) + fib->capacity - fib->size,
					fib->pageSize<<2);
            } else {
                // last page
                /*System.arraycopy(
                    int_array,
                    pageSize * i + capacity - size,
                    newBuffer,
                    0,
                    int_array.length + this.size - capacity - pageSize*i);*/
				memcpy(newBuffer,
					int_array + (i<<fib->exp) + fib->capacity - fib->size,
					(len + fib->size - fib->capacity - (i<<fib->exp))<<2);
            }
            //bufferArrayList.add(newBuffer);
			add(fib->al,newBuffer);
        }
        // update length
        fib->size += len;
        // update capacity
        fib->capacity += (n << fib->exp);
        // update
    }
}

// Get the capacity of FastIntBuffer
/*int getCapacityFIB(FastIntBuffer *fib){
	return fib->capacity;
}*/

// Get the int array corresponding to content of FastIntBuffer 
// with the starting offset and len
int *getIntArray(FastIntBuffer *fib, int offset, int len){
	int *result;
	exception e;
	int first_index, last_index;
	if (fib->size <= 0 || 
		offset < 0 || 
		(offset+len)>fib->size) {
			e.et = invalid_argument;
			e.msg = "getIntArray in FastIntBuffer failed ";
			Throw e;
        //throw (new IllegalArgumentException());
    }
    /*if ((startingOffset + len) > size()) {
        throw (new IndexOutOfBoundsException());
    }*/
    result = (int *)malloc(len*sizeof(int)); // allocate result array
	if (result == NULL){
		e.et = out_of_mem;
		e.msg = "getIntArray in FastIntBuffer failed ";
		Throw e;
	}
//    int first_index = (int) (startingOffset / pageSize);
//    int last_index = (int) ((startingOffset + len) / pageSize);
//    if ((startingOffset + len) % pageSize == 0) {
//        last_index--;
//    }
    /*int first_index = startingOffset >> exp;
    int last_index = (startingOffset + len)>> exp;
    if (((startingOffset + len) & r) == 0) {
        last_index--;
    }*/

	first_index = offset >> fib->exp;
	last_index = (offset + len)>>fib->exp;
	if (((offset + len) & fib->r) == 0){
		last_index--;
	}

    if (first_index == last_index) {
        // to see if there is a need to go across buffer boundry
        /*System.arraycopy(
            (int[]) (bufferArrayList.get(first_index)),
//            startingOffset % pageSize,
            startingOffset & r,
            result,
            0,
            len);*/
		memcpy(result,
			(int *)get(fib->al,first_index)+ (offset & fib->r),
			len<<2);
    } else {
        int int_array_offset = 0;
		int i;
		int *currentChunk;
        for (i = first_index; i <= last_index; i++) {
            //int[] currentChunk = (int[]) bufferArrayList.get(i);
            currentChunk = (int *)get(fib->al,i);
			if (i == first_index) // first section
                {
                /*System.arraycopy(
                    currentChunk,
//                  startingOffset % pageSize,
                    startingOffset & r,
                    result,
                    0,
//                  pageSize - (startingOffset % pageSize));
                    pageSize - (startingOffset & r));*/

				memcpy(result,
					currentChunk + (offset & fib->r),
					(fib->pageSize - (offset & fib->r))<<2);
//              int_array_offset += pageSize - (startingOffset) % pageSize;
                //int_array_offset += pageSize - (startingOffset & r);
				int_array_offset += fib->pageSize  - (offset & fib->r);
            } else if (i == last_index) // last sections
                {
                /*System.arraycopy(
                    currentChunk,
                    0,
                    result,
                    int_array_offset,
                    len - int_array_offset);*/
					memcpy(result + int_array_offset,
						currentChunk,
						(len - int_array_offset)<<2);

            } else {
                //System.arraycopy(currentChunk, 0, result, int_array_offset, pageSize);
				memcpy(result + int_array_offset,
					currentChunk,
					fib->pageSize <<2);
                int_array_offset += fib->pageSize;
            }
        }
    }
    return result;
	//return NULL;
}

// Get the page size of FastIntBuffer
/*int getPageSizeFIB(FastIntBuffer *fib){
	return fib->pageSize;
}*/

// Get the int at the index position of FastIntBuffer
// inlined in .h
//int intAt(FastIntBuffer *fib, int index){
//	exception e;	    
//	if (index < 0 || index > fib->size - 1) {
//        e.et = invalid_argument;
//		e.msg = "index out of range for modifyEntryFIB in FastIntBuffer";
//		Throw e;
//    }
//	return ((int *) get(fib->al,index>>fib->exp))[index & fib->r];
//}

// Replace the value at the index position of FastIntBuffer 
// with newVal
// inlined in .h
//void modifyEntryFIB(FastIntBuffer *fib, int index, int newVal){
//	exception e;	    
//	if (index < 0 || index > fib->size - 1) {
//        e.et = invalid_argument;
//		e.msg = "index out of range for modifyEntryFIB in FastIntBuffer";
//		Throw e;
//    }
////      ((int[]) bufferArrayList.get((int) (index / pageSize)))[index % pageSize] =
//    //((int[]) bufferArrayList.get((index >> exp)))[index & r] =  newValue;
//	((int *) get(fib->al,index>>fib->exp))[index & fib->r] = newVal;
//}

// Get the size of the FastIntBuffer
/*int sizeFIB(FastIntBuffer *fib){
	return fib->size;
}*/
// convert the content of FastIntBuffer to int *
int* toIntArray(FastIntBuffer *fib){
	exception e;
	if (fib->size > 0) {
        //int[] resultArray = new int[size];
		int i;
		int array_offset = 0;
		int *resultArray = (int *)malloc(sizeof(int)*fib->size);
		if (resultArray == NULL){
			e.et = out_of_mem;
			e.msg = "toIntArray in FastIntBuffer failed ";
			Throw e;
		}
        //copy all the content int into the resultArray
        //int array_offset = 0;
        //for (int i = 0; i < bufferArrayList.size(); i++) {
        for (i = 0; i<fib->al->size;i++)
		{
			/*System.arraycopy(
                (int[]) bufferArrayList.get(i),
                0,
                resultArray,
                array_offset,
                (i == (bufferArrayList.size() - 1)) ? (size& r) : pageSize);*/
			memcpy(resultArray + array_offset, 
				(int *) get(fib->al,i),
				(i == fib->al->size -1)? (fib->size & fib->r) : fib->pageSize);
//            (i == (bufferArrayList.size() - 1)) ? size() % pageSize : pageSize);
            array_offset += fib->pageSize;
        }
        return resultArray;
    }
	return NULL;
}

// Get the int at the index position of FastIntBuffer
int intAt(FastIntBuffer *fib, int index){
	exception e;	    
	if (index < 0 || index > fib->size - 1) {
        e.et = invalid_argument;
		e.msg = "index out of range for modifyEntryFIB in FastIntBuffer";
		Throw e;
    }
	return ((int *) get(fib->al,index>>fib->exp))[index & fib->r];
}

// Replace the value at the index position of FastIntBuffer 
// with newVal
void modifyEntryFIB(FastIntBuffer *fib, int index, int newVal){
	exception e;	    
	if (index < 0 || index > fib->size - 1) {
        e.et = invalid_argument;
		e.msg = "index out of range for modifyEntryFIB in FastIntBuffer";
		Throw e;
    }
	((int *) get(fib->al,index>>fib->exp))[index & fib->r] = newVal;
}
