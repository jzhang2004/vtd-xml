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
#include "contextBuffer.h"


// Create ContextBuffer with incSize of i
ContextBuffer *createContextBuffer(int i){
	exception e;
	ArrayList *al = createArrayList();
	ContextBuffer *cb = NULL;
	if (al==NULL) {
		e.et = out_of_mem;
		e.msg = "ContextBuffer allocation failed ";
		Throw e;
	}
	cb = (ContextBuffer *)malloc(sizeof(ContextBuffer));
	if (cb == NULL || i<0){
		freeArrayList(al);
		e.et = out_of_mem;
		e.msg = "ContextBuffer allocation failed ";
		Throw e;
	}
	cb->capacity = 0;
	cb->pageSize = 1024;
	cb->n = 10;
	cb->r = cb->pageSize -1;
	cb->incSize = i;

	cb->al = al;
	return cb;
}


// Create ContextBuffer with page size (1<<e) and increment Size of i
ContextBuffer *createContextBuffer2(int exp, int i){
	exception e;
	ArrayList *al = createArrayList();
	ContextBuffer *cb = NULL;
	if (al==NULL){
		e.et = out_of_mem;
		e.msg = "ContextBuffer allocation failed ";
		Throw e;
	}
	cb = (ContextBuffer *)malloc(sizeof(ContextBuffer));
	if (cb == NULL || exp<0 || i<0){
		freeArrayList(al);
		e.et = out_of_mem;
		e.msg = "ContextBuffer allocation failed ";
		Throw e;
	}

	cb->capacity = 0;
	cb->pageSize = (1<<exp);
	cb->n = exp;
	cb->r = cb->pageSize -1;
	cb->incSize = i;

	cb->al = al;
	return cb;
}


// Free ContextBuffer
void freeContextBuffer(ContextBuffer *cb){

	// no way to guard against a garbage pointer
	if (cb!=NULL) 
		freeArrayList(cb->al);
	free(cb);
}


// Pop the content value back into an integer array.
Boolean load(ContextBuffer *cb, int* output){
	int startingOffset, len, first_index, last_index,i;
	//exception e;
	if (cb->size < cb->incSize) {
        return FALSE;
    }

    startingOffset = cb->size - cb->incSize;
    len = cb->incSize;


    first_index = (startingOffset >> cb->n);
    last_index = ((startingOffset + len) >>cb->n);
    if (((startingOffset + len)& cb->r)== 0) {
        last_index--;
    }

    if (first_index == last_index) {
        // to see if there is a need to go across buffer boundry

        /*System.arraycopy(
            (int*) (bufferArrayList.get(first_index)),
			//startingOffset % pageSize,
            startingOffset & cb->r,
            output,
            0,
            len);*/
		memcpy(output,
			((int *)get(cb->al,first_index))+(startingOffset & cb->r),
			len<<2);
    } else {
        int int_array_offset = 0;
        for (i = first_index; i <= last_index; i++) {
            int* currentChunk = (int *) get(cb->al,i);
            if (i == first_index) // first section
                {
					memcpy(output,
						currentChunk+ (startingOffset+cb->r),
						(cb->pageSize - (startingOffset & cb->r))<<2);
                /*System.arraycopy(
                    currentChunk,
                    //startingOffset % pageSize
                    startingOffset & cb->r,
                    output,
                    0,
                  //  pageSize - (startingOffset % pageSize));
                    pageSize - (startingOffset & cb->r)); */
                //int_array_offset += pageSize - (startingOffset) % pageSize;
                int_array_offset += cb->pageSize - (startingOffset &cb->r);
            } else if (i == last_index) // last sections
                {
					memcpy(output+int_array_offset,
						currentChunk, 
						(len-int_array_offset)<<2);
               /* System.arraycopy(
                    currentChunk,
                    0,
                    output,
                    int_array_offset,
                    len - int_array_offset); */

            } else {
				memcpy(output+int_array_offset, currentChunk, (cb->pageSize)<<2);
                //System.arraycopy(currentChunk, 0, output, int_array_offset, cb->pageSize);
                int_array_offset += cb->pageSize;
            }
        }
    }

    cb->size -= cb->incSize;
    return TRUE;
}


// Push the array content on to the stack .
void store(ContextBuffer *cb, int *input){
	exception e;
	int *lastBuffer; 
	int i;
    // no additional buffer space needed

    //if (bufferArrayList.size() == 0) {
	if (cb->al->size == 0){
        //lastBuffer = new int[pageSize];
		lastBuffer = (int *)malloc(sizeof(int)*cb->pageSize);
		if (lastBuffer == NULL){
			e.et = out_of_mem;
			e.msg = "Fail to increase the size of ArrayList of ContextBuffer";
			Throw e;
		}
        //bufferArrayList.add(lastBuffer);
		add(cb->al,lastBuffer);
        cb->capacity = cb->pageSize;
    } else {
        //lastBuffer = (int[]) bufferArrayList.get(bufferArrayList.size() - 1);
		lastBuffer = (int *)get(cb->al,cb->al->size -1);
    }

    if ((cb->size + cb->incSize) < cb->capacity) {
        //get the last buffer from the bufferListArray
        //obtain the starting offset in that buffer to which the data is to be copied
        //update length

        //System.arraycopy(input, 0, lastBuffer, size % pageSize, input.length);
        //System.arraycopy(input, 0, lastBuffer, size & r, input.length);
		memcpy(lastBuffer+(cb->size& cb->r),input, cb->incSize <<2);
        cb->size += cb->incSize;
    } else // new buffers needed
        {
        // compute the number of additional buffers needed
        int k =
            /*//((int) ((input.length + size) / pageSize))
                ((input.length + size) >>n)
                //+ (((input.length + size) % pageSize) > 0 ? 1 : 0)
                + (((input.length + size) & r ) > 0 ? 1 : 0)
                -  (capacity >> n);*/
			((cb->incSize + cb->size)>>cb->n)+
			(((cb->incSize + cb->size)&cb->r)>0? 1 : 0)-
			(cb->capacity>>cb->n);
        // create these buffers
        // add to bufferArrayList
        //System.arraycopy(input, 0, lastBuffer, size % pageSize, capacity - size);
        //System.arraycopy(input, 0, lastBuffer, size & r, capacity - size);
		memcpy(lastBuffer+(cb->size&cb->r),input, (cb->capacity-cb->size)<<2);

        for (i = 0; i < k; i++) {
            //int[] newBuffer = new int[pageSize];
            int *newBuffer = (int *)malloc(cb->pageSize*sizeof(int));
			if (newBuffer == NULL){
				e.et = out_of_mem;
				e.msg = "Fail to increase the size of ArrayList of ContextBuffer";
				Throw e;
			}
            if (i < k - 1) {
                // full copy 
                //System.arraycopy(input, pageSize * i + capacity - size, newBuffer, 0, pageSize);
				memcpy(newBuffer, 
					input + cb->pageSize*i + cb->capacity - cb->size,
					cb->pageSize);
					
            } else {
                // last page
                //System.arraycopy(
                //    input,
                //    pageSize * i + capacity - size,
                //    newBuffer,
                //    0,
                //    (input.length + size) - pageSize * i - capacity);
				memcpy(newBuffer,
					input + cb->pageSize * i + cb->capacity - cb->size, 
					(cb->incSize+cb->size - cb->pageSize*i - cb->capacity)<<2);
            }
            //bufferArrayList.add(newBuffer);
			add(cb->al,newBuffer);
        }
        // update length
        cb->size += cb->incSize;
        // update capacity
        cb->capacity += (k << cb->n);
        // update 
		}
}


// Manage the buffer size to reduce unused spaces
void resize(ContextBuffer *cb){}