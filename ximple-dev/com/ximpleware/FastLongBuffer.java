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
public class FastLongBuffer implements ILongBuffer {
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

    /**
     * FastLongBuffer constructor comment.
     */
    public FastLongBuffer() {
        size = 0;
        capacity = 1024;
        pageSize = 1024;
        bufferArrayList = new ArrayList();
    }
/**
 * Construct a FastLongBuffer instance with specified page size
 * Creation date: (7/17/03 7:35:56 PM)
 * @param bfz int
 */
public FastLongBuffer(int bfz) {
    if (bfz <= 0) {
        throw new IllegalArgumentException();
    }
    size = 0;
    capacity = pageSize = bfz;
    bufferArrayList = new ArrayList();
}
/**
 * Append single long to the end of array buffer.
 * Creation date: (7/17/03 7:37:34 PM)
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
            size % pageSize,
            long_array.length);
        size += long_array.length;
    } else // new buffers needed
        {

        // compute the number of additional buffers needed
        int n =
            ((int) ((long_array.length + size) / pageSize))
                + (((long_array.length + size) % pageSize) > 0 ? 1 : 0)
                - (int) (capacity / pageSize);
        // create these buffers
        // add to bufferArrayList
        System.arraycopy(long_array, 0, lastBuffer, size % pageSize, capacity - size);

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
 * Creation date: (7/17/03 7:37:54 PM)
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
    if ((this.size + 1) <= this.capacity) {
        //get the last buffer from the bufferListArray
        //obtain the starting offset in that buffer to which the data is to be copied
        //update length
        //System.arraycopy(long_array, 0, lastBuffer, size % pageSize, long_array.length);
        lastBuffer[size % pageSize] = i;
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
 * Creation date: (7/17/03 7:38:30 PM)
 * @return int
 */
public int getCapacity() {
	return capacity;
}
/**
 * Return a selected chuck of long buffer as a long array.
 * Creation date: (7/17/03 7:39:55 PM)
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

    int first_index = (int) (startingOffset / pageSize);
    int last_index = (int) ((startingOffset + len) / pageSize);

    if ((startingOffset + len) % pageSize == 0) {
        last_index--;
    }

    if (first_index == last_index) {
        // to see if there is a need to go across buffer boundry
        System.arraycopy(
            (long[]) (bufferArrayList.get(first_index)),
            startingOffset % pageSize,
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
                    startingOffset % pageSize,
                    result,
                    0,
                    pageSize - (startingOffset % pageSize));
                long_array_offset += pageSize - (startingOffset) % pageSize;
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
 * Creation date: (7/17/03 7:50:38 PM)
 * @return int
 */
public int getPageSize() {
	return pageSize;
}
/**
 * Get the long val at given index value.
 * Creation date: (7/17/03 7:41:21 PM)
 * @return long
 * @param index int
 */
public long longAt(int index) {
    /*if (index < 0 || index > size()) {
        throw new IndexOutOfBoundsException();
    }*/
    int pageNum = (int) index / pageSize;
    int offset = index % pageSize;
    return ((long[]) bufferArrayList.get(pageNum))[offset];
}
/**
 * Get the lower 32 bit of the integer at the given index.
 * Creation date: (7/17/03 7:46:48 PM)
 * @return int
 * @param i int
 */
 public int lower32At(int index) {
    if (index < 0 || index > size()) {
        throw new IndexOutOfBoundsException();
    }
    int pageNum = (int) index / pageSize;
    int offset = index % pageSize;
    return (int) ((long[]) bufferArrayList.get(pageNum))[offset];
}
/**
 * Modify the value at the index to a new val.
 * Creation date: (7/17/03 7:48:22 PM)
 * @param index int
 * @param value long
 */
public void modifyEntry(int index, long newValue) {

    if (index < 0 || index > size + 1) {
        throw new IndexOutOfBoundsException();
    }

    ((long[]) bufferArrayList.get((int) (index / pageSize)))[index % pageSize] =
        newValue;
}
/**
 * Get the total number of longs in the buffer.
 * Creation date: (7/17/03 7:38:57 PM)
 * @return int
 */
public int size() {
	return size;
}
/**
 * Convert all longs into a long array.
 * Creation date: (7/17/03 7:47:36 PM)
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
                (i == (bufferArrayList.size() - 1)) ? size() % pageSize : pageSize);
            array_offset += pageSize;
        }
        return resultArray;
    }
    return null;
}
/**
 * Return the upper 32 bit of the long at the index.
 * Creation date: (7/17/03 7:49:36 PM)
 * @return int
 * @param index int
 */
public int upper32At(int index) {
    if (index < 0 || index > size()) {
        throw new IndexOutOfBoundsException();
    }
    int pageNum = (int) index / pageSize;
    int offset = index % pageSize;
    return (int)
        ((((long[]) bufferArrayList.get(pageNum))[offset] & (0xffffffffL << 32)) >> 32);

}
}
