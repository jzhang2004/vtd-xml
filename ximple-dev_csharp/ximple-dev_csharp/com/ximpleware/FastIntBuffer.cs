/* 
* Copyright (C) 2002-2010 XimpleWare, info@ximpleware.com
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
using System;
namespace com.ximpleware
{
	
	/// <summary> A fast, unsynchronized, chunk-based int buffer 
	/// 
	/// </summary>
	
	public class FastIntBuffer : IIntBuffer
	{
		/// <summary> Returns the total allocated capacity of this buffer instance.</summary>
		/// <returns> int
		/// </returns>
		public int getCapacity()
		{
			return capacity;			
		}
		/// <summary> Returns the page size of this buffer instance.
		/// </summary>
		/// <returns> int
		/// </returns>
		public int getPageSize()
		{
			return pageSize;
		}
		/* bufferArrayList is a resizable array list of int buffers
		*
		*/
       	public const int ASCENDING = 0;
	    public const int DESCENDING = 1;
		private com.ximpleware.ArrayList bufferArrayList;
		
		/// <summary> Total capacity of the IntBuffer</summary>
		private int capacity;
		
		/// <summary> Page size of the incremental growth of the Int Buffer</summary>
		private int pageSize;
		
		/// <summary> Total number of integers in the IntBuffer</summary>
		protected internal int size_Renamed_Field;
		private int exp;
		private int r;
		/// <summary> FastIntBuffer constructor comment.</summary>
		public FastIntBuffer()
		{
			size_Renamed_Field = 0;
			capacity = 0;
			pageSize = 1024;
			exp = 10;
			r = 1023;
			bufferArrayList = new ArrayList();
		}
		/// <summary> Constructor with adjustable buffer page size of the value bfz</summary>
		/// <param name="bfz">int  is the size of the internal buffer
		/// </param>
		public FastIntBuffer(int e)
		{
			if (e < 0)
			{
				throw new System.ArgumentException();
			}
			capacity = size_Renamed_Field = 0;
			pageSize = 1 << e;
			exp = e;
			r = pageSize - 1;
			bufferArrayList = new ArrayList();
		}
		/// <summary> Append an int array to the end of this buffer instance</summary>
		/// <param name="int_array">int[]
		/// </param>
		public void  append(int[] int_array)
		{
			/*if (int_array == null)
			{
				throw new System.NullReferenceException();
			}*/
			// no additional buffer space needed
			int lastBufferIndex = 0;
			int[] lastBuffer = null;
			
			if (bufferArrayList.size == 0)
			{
				lastBuffer = new int[pageSize];
				bufferArrayList.Add(lastBuffer);
				lastBufferIndex = 0;
				capacity = pageSize;
			}
			else
			{
				lastBufferIndex = System.Math.Min((size_Renamed_Field >> exp), bufferArrayList.size - 1);
				lastBuffer = (int[]) bufferArrayList.Get(lastBufferIndex);
			}
			
			if ((this.size_Renamed_Field + int_array.Length) < this.capacity)
			{
				//get the last buffer from the bufferListArray
				//obtain the starting offset in that buffer to which the data is to be copied
				//update length        
				
				//System.arraycopy(input, 0, lastBuffer, size % pageSize, input.length);
				if (this.size_Renamed_Field + int_array.Length < ((lastBufferIndex + 1) << exp))
				{
					Array.Copy(int_array, 0, lastBuffer, size_Renamed_Field & r, int_array.Length);
				}
				else
				{
					int offset = pageSize - (size_Renamed_Field & r);
					// copy the first part
					Array.Copy(int_array, 0, lastBuffer, size_Renamed_Field & r, offset);
					// copy the middle part
					
					int l = int_array.Length - offset;
					int k = (l) >> exp;
					int z;
					for (z = 1; z <= k; z++)
					{
						Array.Copy(int_array, offset, (int[])bufferArrayList.oa[lastBufferIndex + z], 0, pageSize);
						offset += pageSize;
					}
					// copy the last part
                    Array.Copy(int_array, offset, (int[])bufferArrayList.oa[lastBufferIndex + z], 0, l & r);
				}
				size_Renamed_Field += int_array.Length;
				return ;
			}
			// new buffers needed
			else
			{
				
				// compute the number of additional buffers needed
				//        int n =
				//            ((int) ((int_array.length + size) / pageSize))
				//                + (((int_array.length + size) % pageSize) > 0 ? 1 : 0)
				//                - (int) (capacity / pageSize);
				int n = ((int_array.Length + size_Renamed_Field) >> exp) + (((int_array.Length + size_Renamed_Field) & r) > 0?1:0) - (capacity >> exp);
				// create these buffers
				// add to bufferArrayList
				
				//System.arraycopy(int_array, 0, lastBuffer, size % pageSize, capacity - size);
				Array.Copy(int_array, 0, lastBuffer, size_Renamed_Field & r, capacity - size_Renamed_Field);
				
				for (int i = 0; i < n; i++)
				{
					int[] newBuffer = new int[pageSize];
					if (i < n - 1)
					{
						// full copy 
						Array.Copy(int_array, pageSize * i + capacity - size_Renamed_Field, newBuffer, 0, pageSize);
					}
					else
					{
						// last page
						Array.Copy(int_array, pageSize * i + capacity - size_Renamed_Field, newBuffer, 0, int_array.Length + this.size_Renamed_Field - capacity - pageSize * i);
					}
					bufferArrayList.Add(newBuffer);
				}
				// update length
				size_Renamed_Field += int_array.Length;
				// update capacity
				capacity += n * pageSize;
				// update
			}
		}
		/// <summary> Append a single int to the end of this buffer Instance</summary>
		/// <param name="i">int
		/// </param>
		public void  append(int i)
		{

            if (this.size_Renamed_Field < this.capacity)
            {
                //get the last buffer from the bufferListArray
                //obtain the starting offset in that buffer to which the data is to be copied
                //update length
                //System.arraycopy(long_array, 0, lastBuffer, size % pageSize, long_array.length);
                ((int[])bufferArrayList.oa[bufferArrayList.size - 1])[size_Renamed_Field & r] = i;
                //        lastBuffer[size % pageSize] = i;
                size_Renamed_Field += 1;
            }
            else // new buffers needed
            {
                int[] newBuffer = new int[pageSize];
                size_Renamed_Field++;
                capacity += pageSize;
                bufferArrayList.Add(newBuffer);
                newBuffer[0] = i;
            }
		}
		/// <summary> Returns a single int array representing every int in this buffer instance</summary>
		/// <returns> int[]  (null if there isn't anything left in the buffer   
		/// </returns>
		/// <param name="startingOffset">int
		/// </param>
		/// <param name="length">int
		/// </param>
		public int[] getIntArray(int startingOffset, int len)
		{
			if (size_Renamed_Field <= 0 || startingOffset < 0)
			{
				throw (new System.ArgumentException());
			}
			if ((startingOffset + len) > size_Renamed_Field)
			{
				throw (new System.IndexOutOfRangeException());
			}
			int[] result = new int[len]; // allocate result array
			
			//    int first_index = (int) (startingOffset / pageSize);
			//    int last_index = (int) ((startingOffset + len) / pageSize);
			//    if ((startingOffset + len) % pageSize == 0) {
			//        last_index--;
			//    }
			int first_index = startingOffset >> exp;
			int last_index = (startingOffset + len) >> exp;
			if (((startingOffset + len) & r) == 0)
			{
				last_index--;
			}
			
			if (first_index == last_index)
			{
				// to see if there is a need to go across buffer boundry
				Array.Copy(((int[])bufferArrayList.oa[first_index]), startingOffset & r, result, 0, len);
			}
			else
			{
				int int_array_offset = 0;
				for (int i = first_index; i <= last_index; i++)
				{
					int[] currentChunk = (int[]) bufferArrayList.oa[i];
					if (i == first_index)
					// first section
					{
						Array.Copy(currentChunk, startingOffset & r, result, 0, pageSize - (startingOffset & r));
						//                int_array_offset += pageSize - (startingOffset) % pageSize;
						int_array_offset += pageSize - (startingOffset & r);
					}
					else if (i == last_index)
					// last sections
					{
						Array.Copy(currentChunk, 0, result, int_array_offset, len - int_array_offset);
					}
					else
					{
						Array.Copy(currentChunk, 0, result, int_array_offset, pageSize);
						int_array_offset += pageSize;
					}
				}
			}
			return result;
		}
		/// <summary> Get the int at the location specified by index.</summary>
		/// <returns> int
		/// </returns>
		/// <param name="index">int
		/// </param>
		public int intAt(int index)
		{
			/*if (index < 0 || index > size() - 1)
			{
				throw new System.IndexOutOfRangeException();
			}*/
			//    int pageNum = (int) index / pageSize;
			int pageNum = index >> exp;
			//System.out.println("page Number is "+pageNum); 
			//    int offset = index % pageSize;
			int offset = index & r;
			return ((int[]) bufferArrayList.oa[pageNum])[offset];
		}
		/// <summary> Assigns a new int value to location index of the buffer instance.</summary>
		/// <param name="index">int
		/// </param>
		/// <param name="newValue">int
		/// </param>
		/// <Exception>   Throws IndexOutOfBoundsException </Exception>
		public void  modifyEntry(int index, int newValue)
		{
			
			if (index < 0 || index > size_Renamed_Field - 1)
			{
				throw new System.IndexOutOfRangeException();
			}
			
			//        ((int[]) bufferArrayList.get((int) (index / pageSize)))[index % pageSize] =
			//UPGRADE_TODO: Method 'java.util.ArrayList.get' was not converted. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1095'"
			((int[]) bufferArrayList.oa[(index >> exp)])[index & r] = newValue;
		}
		/// <summary> Returns the total number of int values in the buffer instance</summary>
		/// <returns> int
		/// </returns>
		public int size()
		{
			return size_Renamed_Field;
		}
		/// <summary> Returns the int array corresponding to all int values in this buffer instance</summary>
		/// <returns> int[] (null if the buffer is empty)
		/// </returns>
		public int[] toIntArray()
		{
			if (size_Renamed_Field > 0)
			{
				int s = size_Renamed_Field;
				int[] resultArray = new int[size_Renamed_Field];
				//copy all the content int into the resultArray
				int array_offset = 0;
				for (int i = 0; s > 0; i++)
				{
					Array.Copy((int[])bufferArrayList.oa[i], 0, resultArray, array_offset, (s < pageSize)?s:pageSize);
					//            (i == (bufferArrayList.size() - 1)) ? size() % pageSize : pageSize);
					s = s - pageSize;
					array_offset += pageSize;
				}
				return resultArray;
			}
			return null;
		}
		
		
		/// <summary> set the size of int buffer to zero, capacity
		/// untouched so int buffer can be reused without
		/// any unnecessary and additional allocation
		/// 
		/// </summary>
		public  void  clear()
		{
			size_Renamed_Field = 0;
		}
        /// <summary>
        /// 
        /// </summary>
        /// <param name="sz"></param>
        /// <returns></returns>
        public bool resize(int sz)
        {
            if (sz <= capacity && sz >= 0)
            {
                size_Renamed_Field = sz;
                return true;
            }
            else
                return false;
        }

        /**
  * Sort the integers in the buffer 
  * @param order (as of version 2.9) 
  * it can be either ASCENDING or DESCENDING
  */
        public void sort(int order)
        {
            switch (order)
            {
                case ASCENDING:
                    if (size_Renamed_Field > 0)
                        quickSort_ascending(0, size_Renamed_Field - 1);
                    break;
                case DESCENDING:
                    if (size_Renamed_Field > 0)
                        quickSort_descending(0, size_Renamed_Field - 1);
                    break;
                default:
                    throw new System.ArgumentException("Sort type undefined");
            }

        }

        void quickSort_ascending(int lo, int hi)
        {
            //   lo is the lower index, hi is the upper index
            //   of the region of array a that is to be sorted
            //System.out.println("lo ==>"+lo);
            //System.out.println("hi ==>"+hi);
            int i = lo, j = hi;
            int h;
            //Object o;
            int x = this.intAt((lo + hi) / 2);
            //  partition
            do
            {
                while (intAt(i) < x) i++;
                while (intAt(j) > x) j--;
                if (i <= j)
                {
                    h = this.intAt(i);
                    modifyEntry(i, this.intAt(j));
                    modifyEntry(j, h);
                    i++;
                    j--;
                }
            } while (i <= j);
            //  recursion
            if (lo < j) quickSort_ascending(lo, j);
            if (i < hi) quickSort_ascending(i, hi);
        }

        void quickSort_descending(int lo, int hi)
        {
            //   lo is the lower index, hi is the upper index
            //   of the region of array a that is to be sorted
            //System.out.println("lo ==>"+lo);
            //System.out.println("hi ==>"+hi);
            int i = lo, j = hi;
            int h;
            //Object o;
            int x = this.intAt((lo + hi) / 2);
            //  partition
            do
            {
                while (intAt(i) > x) i++;
                while (intAt(j) < x) j--;
                if (i <= j)
                {
                    h = this.intAt(i);
                    modifyEntry(i, this.intAt(j));
                    modifyEntry(j, h);
                    i++;
                    j--;
                }
            } while (i <= j);
            //  recursion
            if (lo < j) quickSort_descending(lo, j);
            if (i < hi) quickSort_descending(i, hi);
        }
	}
}