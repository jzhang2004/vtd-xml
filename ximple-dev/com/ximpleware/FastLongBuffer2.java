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
import java.lang.IndexOutOfBoundsException;
import java.lang.NullPointerException;
import java.lang.IllegalArgumentException;
import java.util.ArrayList;

/**
 * A fast, unsynchronized, chunk-based long buffer for storing LCs and VTD.
 * Creation date: (7/17/03 6:07:46 PM)
 */
public class FastLongBuffer2 implements ILongBuffer {
    /* bufferArrayList is a resizable array list of int buffers
     *
     */
    private ArrayList bufferArrayList;

    /**
    * Total capacity of the IntBuffer
    */
    private int capacity;

    /**
    * Page size of the incremental growth of the Int Buffer
    */
    private int pageSize;
    /**
    	 * Total number of integers in the IntBuffer
    */
    private int size;
    private int exp;
    private int r;

    /**
     * FastLongBuffer constructor comment.
     */
    public FastLongBuffer2() {
        size = 0;
        capacity = 0;
        pageSize = 1024;
        exp = 10;
        r = 1023;
        bufferArrayList = new ArrayList();
    }
/**
 * Construct a FastLongBuffer instance with specified page size
 * @param e int (so that pageSize = (1<<e)) 
 */
public FastLongBuffer2(int e) {
    if (e <= 0) {
        throw new IllegalArgumentException();
    }
    capacity = size = 0;
    pageSize = (1<<e);
    exp = e;
    r = pageSize -1;
    bufferArrayList = new ArrayList();
}

/**
 * Construct a FastLongBuffer instance with specified page size
 * @param e int (so that pageSize = (1<<e))
 * @param c int (suggest initial capacity of  ArrayList
 */
public FastLongBuffer2(int e,int c) {
    if (e <= 0) {
        throw new IllegalArgumentException();
    }
    capacity = size = 0;
    pageSize = (1<<e);
    exp = e;
    r = pageSize -1;
    bufferArrayList = new ArrayList(c);
}
/**
 * Append single long to the end of array buffer.
 * @param long_array long[]
 */
public void append(long[] long_array) {
  if (long_array == null) {
        throw new NullPointerException();
    }
    // no additional buffer space needed

    long[] lastBuffer;
    if (bufferArrayList.size() == 0) {
        lastBuffer = new long[pageSize];
        bufferArrayList.add(lastBuffer);
        capacity = pageSize;
    } else {
        lastBuffer = (long[]) bufferArrayList.get(bufferArrayList.size() - 1);
    }

    if ((this.size + long_array.length) < this.capacity) {
        //get the last buffer from the bufferListArray
        //obtain the starting offset in that buffer to which the data is to be copied
        //update length

        System.arraycopy(
            long_array,
            0,
            lastBuffer,
			// size % pageSize,
            size& r,
            long_array.length);
        size += long_array.length;
    } else // new buffers needed
        {

        // compute the number of additional buffers needed
//        int n =
//            ((int) ((long_array.length + size) / pageSize))
//                + (((long_array.length + size) % pageSize) > 0 ? 1 : 0)
//                - (int) (capacity / pageSize);
    	int n =
    		    ((long_array.length + size) >> exp)
                + (((long_array.length + size)&r) > 0 ? 1 : 0)
                -  (capacity >> exp);
        // create these buffers
        // add to bufferArrayList
        //System.arraycopy(long_array, 0, lastBuffer, size % pageSize, capacity - size);
        System.arraycopy(long_array, 0, lastBuffer, size & r, capacity - size);

        for (int i = 0; i < n; i++) {
            long[] newBuffer = new long[pageSize];
            if (i < n - 1) {
                // full copy 
                System.arraycopy(
                    long_array,
                    pageSize * i + capacity - size,
                    newBuffer,
                    0,
                    pageSize);
            } else {
                // last page
                System.arraycopy(
                    long_array,
                    pageSize * i + capacity - size,
                    newBuffer,
                    0,
                    long_array.length+ size - pageSize*i - capacity);
            }
            bufferArrayList.add(newBuffer);
        }
        // update length
        size += long_array.length;
        // update capacity
        capacity += n * pageSize;
        // update
    }
}
/**
 * Append an integer to the end of this array buffer
 * @param a long
 */
public void append(long i) {
   long[] lastBuffer;
    if (bufferArrayList.size() == 0) {
        lastBuffer = new long[pageSize];
        bufferArrayList.add(lastBuffer);
        capacity = pageSize;
    } else {
        lastBuffer = (long[]) bufferArrayList.get(bufferArrayList.size() - 1);
    }
    if (this.size  < this.capacity) {
        //get the last buffer from the bufferListArray
        //obtain the starting offset in that buffer to which the data is to be copied
        //update length
        //System.arraycopy(long_array, 0, lastBuffer, size % pageSize, long_array.length);
        //lastBuffer[size % pageSize] = i;
        lastBuffer[size & r] = i;
        size += 1;
    } else // new buffers needed
        {
        long[] newBuffer = new long[pageSize];
        size++;
        capacity += pageSize;
        bufferArrayList.add(newBuffer);
        newBuffer[0] = i;
    }
}
/**
 * Get the capacity of the buffer.
 * @return int
 */
public int getCapacity() {
	return capacity;
}
/**
 * Return a selected chuck of long buffer as a long array.
 * @return long[]
 * @param startingOffset int
 * @param length int
 */
public long[] getLongArray(int startingOffset, int len) {
    if (size <= 0 || startingOffset < 0) {
        throw (new IllegalArgumentException());
    }
    if ((startingOffset + len) > size()) {
        throw (new IndexOutOfBoundsException());
    }

    long[] result = new long[len]; // allocate result array

    int first_index =  (startingOffset >> exp);
    int last_index = ((startingOffset + len) >>exp);

    //if ((startingOffset + len) % pageSize == 0) {
    if (((startingOffset + len) & r) == 0) {
        last_index--;
    }

    if (first_index == last_index) {
        // to see if there is a need to go across buffer boundry
        System.arraycopy(
            (long[]) (bufferArrayList.get(first_index)),
//            startingOffset % pageSize,
			startingOffset & r,
            result,
            0,
            len);
    } else {
        int long_array_offset = 0;
        for (int i = first_index; i <= last_index; i++) {
            long[] currentChunk = (long[]) bufferArrayList.get(i);
            if (i == first_index) // first section
                {
                System.arraycopy(
                    currentChunk,
//                  startingOffset % pageSize,
        			startingOffset & r,
                    result,
                    0,
//                  pageSize - (startingOffset % r));
					pageSize - (startingOffset & r));
                long_array_offset += pageSize - (startingOffset & r);
            } else if (i == last_index) // last sections
                {
                System.arraycopy(
                    currentChunk,
                    0,
                    result,
                    long_array_offset,
                    len - long_array_offset);

            } else {
                System.arraycopy(currentChunk, 0, result, long_array_offset, pageSize);
                long_array_offset += pageSize;
            }
        }
    }

    return result;
}
/**
 * Get the buffer page size.
 * @return int
 */
public int getPageSize() {
	return pageSize;
}
/**
 * Get the long val at given index value.
 * @return long
 * @param index int
 */
public long longAt(int index) {
    /*if (index < 0 || index > size()) {
        throw new IndexOutOfBoundsException();
    }*/
    int pageNum = (index >> exp);
    // int offset = index % r;
    int offset = index &r;
    return ((long[]) bufferArrayList.get(pageNum))[offset];
}
/**
 * Get the lower 32 bit of the integer at the given index.
 * @return int
 * @param i int
 */
 public int lower32At(int index) {
    if (index < 0 || index > size()) {
        throw new IndexOutOfBoundsException();
    }
    int pageNum =  (index >> exp);
    // int offset = index % pageSize;
    int offset = index & r;
    return (int) ((long[]) bufferArrayList.get(pageNum))[offset];
}
/**
 * Modify the value at the index to a new val.
 * @param index int
 * @param value long
 */
public void modifyEntry(int index, long newValue) {

    if (index < 0 || index > size + 1) {
        throw new IndexOutOfBoundsException();
    }
    //((long[]) bufferArrayList.get((int) (index / pageSize)))[index % pageSize] =
    ((long[]) bufferArrayList.get(index >> exp))[index & r] =
        newValue;
}
/**
 * Get the total number of longs in the buffer.
 * @return int
 */
public int size() {
	return size;
}
/**
 * Convert all longs into a long array.
 * @return long[]
 */
public long[] toLongArray() {
    if (size > 0) {
        long[] resultArray = new long[size];
        //copy all the content int into the resultArray
        int array_offset = 0;
        for (int i = 0; i < bufferArrayList.size(); i++) {
            System.arraycopy(
                (int[]) bufferArrayList.get(i),
                0,
                resultArray,
                array_offset,
                //(i == (bufferArrayList.size() - 1)) ? size - ((size>>exp)<<exp) : pageSize);
                (i == (bufferArrayList.size() - 1)) ? (size & r) : pageSize);
            array_offset += pageSize;
        }
        return resultArray;
    }
    return null;
}
/**
 * Return the upper 32 bit of the long at the index.
 * @return int
 * @param index int
 */
public int upper32At(int index) {
    if (index < 0 || index > size()) {
        throw new IndexOutOfBoundsException();
    }
    int pageNum = (index >>exp);
    int offset = index & r;
    return (int)
        ((((long[]) bufferArrayList.get(pageNum))[offset] & (0xffffffffL << 32)) >> 32);

}
}

