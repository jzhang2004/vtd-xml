package com.ximpleware;

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
//import java.lang.NullPointerException;
import java.lang.IllegalArgumentException;
import java.util.ArrayList;
//import com.ximpleware.NavException;

/**
 * This class is used as the global stack in VTDNav class.
 * The VTDNav object instantiate a context Buffer object, then everytime
 * load/store is called, current context information gets pushed/popped to/from the 
 * ContextBuffer.
 * Creation date: (11/16/03 4:30:45 PM)
 */
public class ContextBuffer {
    private java.util.ArrayList bufferArrayList;
    private int capacity;
    private int pageSize;
    private int size;
    private int incSize;
/**
 * ContextBuffer constructor comment.
 * inc is the # of int to be pushed/pop to/from the underlying storage
 * @param pageSize int
 */
public ContextBuffer(int i) {
	super();
	pageSize =1024;
	incSize = i;
	capacity =0;
	if (incSize<0)
	  throw new IllegalArgumentException();
	bufferArrayList = new ArrayList();
	
}
/**
 * ContextBuffer constructor comment.
 * incSize is the # of int to be pushed/pop to/from the underlying storage
 * Creation date: (11/16/03 8:02:21 PM)
 * @param pageSize int
 * @param incSize int
 */
public ContextBuffer(int p, int i) {
    pageSize = p;
    incSize = i;
    capacity = 0;
    if (incSize < 0)
        throw new IllegalArgumentException("context buffer's incremental size must be greater than zero");
    bufferArrayList = new ArrayList();
}
/**
 * Pop the content value back into an integer array.
 * Creation date: (11/17/03 1:07:42 AM)
 * @param output int[]
 * @return boolean   If load is successful.
 **/
public boolean load(int[] output){

    if (size < incSize) {
        return false;
    }

    int startingOffset = size - incSize;
    int len = incSize;

    if ((startingOffset + len) > size) {
        throw (new IndexOutOfBoundsException());
    }
    //int[] result = new int[len]; // allocate result array
    //if (pageSize != 1) {
    int first_index = (int) (startingOffset / pageSize);
    int last_index = (int) ((startingOffset + len) / pageSize);
    if ((startingOffset + len) % pageSize == 0) {
        last_index--;
    }

    if (first_index == last_index) {
        // to see if there is a need to go across buffer boundry
        System.arraycopy(
            (int[]) (bufferArrayList.get(first_index)),
            startingOffset % pageSize,
            output,
            0,
            len);
    } else {
        int int_array_offset = 0;
        for (int i = first_index; i <= last_index; i++) {
            int[] currentChunk = (int[]) bufferArrayList.get(i);
            if (i == first_index) // first section
                {
                System.arraycopy(
                    currentChunk,
                    startingOffset % pageSize,
                    output,
                    0,
                    pageSize - (startingOffset % pageSize));
                int_array_offset += pageSize - (startingOffset) % pageSize;
            } else if (i == last_index) // last sections
                {
                System.arraycopy(
                    currentChunk,
                    0,
                    output,
                    int_array_offset,
                    len - int_array_offset);

            } else {
                System.arraycopy(currentChunk, 0, output, int_array_offset, pageSize);
                int_array_offset += pageSize;
            }
        }
    }

    size -= incSize;
    return true;
}
/**
 * Test the load and store functions.
 * Creation date: (11/19/03 3:51:17 PM)
 * @param args java.lang.String[]
 */
public static void main(String[] args) {
    try {
        int[] ia = new int[10];

        ContextBuffer cb = new ContextBuffer(16, 10);
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                ia[j] = i;
            }
            cb.store(ia);
        }
        //cb.store(ia);
        for (int i = 9; i >= 0; i--) {
            cb.load(ia);
            //System.out.println(""+ia);
            for (int j = 9; j >= 0; j--) {
            	System.out.println("j  ==>"+j);
                if (ia[j] != i) {
                    System.out.println(" store error " + i + " " + j + " " + ia[j]);
                }
            }
        }
        System.out.println("success");
    } catch (Exception e) {
        System.out.println(" exception caught ");
    }
}
/**
 * Manage the buffer size to reduce unused spaces
 * unimplemented at this point
 * Creation date: (11/19/03 3:39:15 PM)
 */
public void resize() {}
/**
 * Push the array content on to the stack.
 * Creation date: (11/17/03 1:06:43 AM)
 * @param input int[]
 */
public void store(int[] input){

    if (input == null) {
        throw new IllegalArgumentException("context buffer can't store a null pointer");
    }

    //if (input.length != incSize) {
        // throw a derivative of runtime exceptions
    //    throw new NavException("Store: Input size doesn't match increment size");
    //}

    // no additional buffer space needed

    int[] lastBuffer;
    if (bufferArrayList.size() == 0) {
        lastBuffer = new int[pageSize];
        bufferArrayList.add(lastBuffer);
        capacity = pageSize;
    } else {
        lastBuffer = (int[]) bufferArrayList.get(bufferArrayList.size() - 1);
    }

    if ((this.size + input.length) < this.capacity) {
        //get the last buffer from the bufferListArray
        //obtain the starting offset in that buffer to which the data is to be copied
        //update length

        System.arraycopy(input, 0, lastBuffer, size % pageSize, input.length);
        size += input.length;
    } else // new buffers needed
        {

        // compute the number of additional buffers needed
        int n =
            ((int) ((input.length + size) / pageSize))
                + (((input.length + size) % pageSize) > 0 ? 1 : 0)
                - (int) (capacity / pageSize);
        // create these buffers
        // add to bufferArrayList
        System.arraycopy(input, 0, lastBuffer, size % pageSize, capacity - size);

        for (int i = 0; i < n; i++) {
            int[] newBuffer = new int[pageSize];
            if (i < n - 1) {
                // full copy 
                System.arraycopy(input, pageSize * i + capacity - size, newBuffer, 0, pageSize);
            } else {
                // last page
                System.arraycopy(
                    input,
                    pageSize * i + capacity - size,
                    newBuffer,
                    0,
                    (input.length + size) - pageSize * i - capacity);
            }
            bufferArrayList.add(newBuffer);
        }
        // update length
        size += input.length;
        // update capacity
        capacity += n * pageSize;
        // update
    }
}
}
