/* 
* Copyright (C) 2002-2011 XimpleWare, info@ximpleware.com
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
	
	/// <summary> A fast, unsynchronized, chunk-based long buffer for storing LCs and VTD.
	/// Creation date: (7/17/03 6:07:46 PM)
	/// </summary>
	public class FastLongBuffer : ILongBuffer
	{
		/// <summary> Get the capacity of the buffer.</summary>
		/// <returns> int
		/// </returns>
		public int getCapacity()
		{
				return capacity;			
		}
		/// <summary> Get the buffer page size.</summary>
		/// <returns> int
		/// </returns>
		public int getPageSize()
		{
		    	return pageSize;			
		}
		/* bufferArrayList is a resizable array list of int buffers
		*
		*/
		private com.ximpleware.ArrayList bufferArrayList;
		
		/// <summary> Total capacity of the IntBuffer</summary>
		private int capacity;
		
		/// <summary> Page size of the incremental growth of the Int Buffer</summary>
		private int pageSize;
		/// <summary> Total number of integers in the IntBuffer</summary>
		protected internal int size_Renamed_Field;
		private int exp;
		private int r;
		
		/// <summary> FastLongBuffer constructor comment.</summary>
		public FastLongBuffer()
		{
			size_Renamed_Field = 0;
			capacity = 0;
			pageSize = 1024;
			exp = 10;
			r = 1023;
			bufferArrayList = new ArrayList();
		}
		/// <summary> Construct a FastLongBuffer instance with specified page size</summary>
		/// <param name="e">int (so that pageSize = (1<<e)) 
		/// </param>
        public FastLongBuffer(int e)
		{
			if (e <= 0)
			{
				throw new System.ArgumentException();
			}
			capacity = size_Renamed_Field = 0;
			pageSize = (1 << e);
			exp = e;
			r = pageSize - 1;
			bufferArrayList = new ArrayList();
		}
		
		/// <summary> Construct a FastLongBuffer instance with specified page size</summary>
		/// <param name="e">int (so that pageSize = (1<<e))
		/// </param>
		/// <param name="c">int (suggest initial capacity of  ArrayList
		/// </param>
        public FastLongBuffer(int e, int c)
		{
			if (e <= 0)
			{
				throw new System.ArgumentException();
			}
			capacity = size_Renamed_Field = 0;
			pageSize = (1 << e);
			exp = e;
			r = pageSize - 1;
			bufferArrayList = new ArrayList(c);
		}
		/// <summary> Append single long to the end of array buffer.</summary>
		/// <param name="long_array">long[]
		/// </param>
		public void  append(long[] long_array)
		{
			if (long_array == null)
			{
				throw new System.NullReferenceException();
			}
			// no additional buffer space needed
			int lastBufferIndex =0;
			long[] lastBuffer = null;
			if (bufferArrayList.size == 0)
			{
				lastBuffer = new long[pageSize];
				bufferArrayList.Add(lastBuffer);
				lastBufferIndex = 0;
				capacity = pageSize;
			}
			else
			{
				lastBufferIndex = System.Math.Min((size_Renamed_Field >> exp), bufferArrayList.size - 1);
				lastBuffer = (long[]) bufferArrayList.oa[lastBufferIndex];
			}
			
			if ((this.size_Renamed_Field + long_array.Length) < this.capacity)
			{
				//get the last buffer from the bufferListArray
				//obtain the starting offset in that buffer to which the data is to be copied
				//update length
				if (this.size_Renamed_Field + long_array.Length < ((lastBufferIndex + 1) << exp))
				{
					Array.Copy(long_array, 0, lastBuffer, size_Renamed_Field & r, long_array.Length);
				}
				else
				{
					int offset = pageSize - (size_Renamed_Field & r);
					// copy the first part
					Array.Copy(long_array, 0, lastBuffer, size_Renamed_Field & r, offset);
					// copy the middle part
					int l = long_array.Length - offset;
					int k = (l) >> exp;
					int z;
					for (z = 1; z <= k; z++)
					{
						Array.Copy(long_array, offset,(long[]) bufferArrayList.oa[lastBufferIndex + z], 0, pageSize);
						offset += pageSize;
					}
					// copy the last part
					Array.Copy(long_array, offset, (long[])bufferArrayList.oa[lastBufferIndex + z], 0, l & r);
				}
				size_Renamed_Field += long_array.Length;
				return ;
			}
			// new buffers needed
			else
			{
				
				// compute the number of additional buffers needed
				//        int n =
				//            ((int) ((long_array.length + size) / pageSize))
				//                + (((long_array.length + size) % pageSize) > 0 ? 1 : 0)
				//                - (int) (capacity / pageSize);
				int n = ((long_array.Length + size_Renamed_Field) >> exp) + (((long_array.Length + size_Renamed_Field) & r) > 0?1:0) - (capacity >> exp);
				// create these buffers
				// add to bufferArrayList
				//System.arraycopy(long_array, 0, lastBuffer, size % pageSize, capacity - size);
				Array.Copy(long_array, 0, lastBuffer, size_Renamed_Field & r, capacity - size_Renamed_Field);
				
				for (int i = 0; i < n; i++)
				{
					long[] newBuffer = new long[pageSize];
					if (i < n - 1)
					{
						// full copy 
						Array.Copy(long_array, pageSize * i + capacity - size_Renamed_Field, newBuffer, 0, pageSize);
					}
					else
					{
						// last page
						Array.Copy(long_array, pageSize * i + capacity - size_Renamed_Field, newBuffer, 0, long_array.Length + size_Renamed_Field - pageSize * i - capacity);
					}
					bufferArrayList.Add(newBuffer);
				}
				// update length
				size_Renamed_Field += long_array.Length;
				// update capacity
				capacity += n * pageSize;
				// update
			}
		}
		/// <summary> Append an integer to the end of this array buffer</summary>
		/// <param name="a">long
		/// </param>
		public void  append(long i)
		{
            if (this.size_Renamed_Field < this.capacity)
            {
                //get the last buffer from the bufferListArray
                //obtain the starting offset in that buffer to which the data is to be copied
                //update length
                //System.arraycopy(long_array, 0, lastBuffer, size % pageSize, long_array.length);
                //lastBuffer[size % pageSize] = i;
                ((long[])bufferArrayList.oa[bufferArrayList.size - 1])[size_Renamed_Field & r] = i;
                size_Renamed_Field += 1;
            }
            else // new buffers needed
            {
                long[] newBuffer = new long[pageSize];
                size_Renamed_Field++;
                capacity += pageSize;
                bufferArrayList.Add(newBuffer);
                newBuffer[0] = i;
            }
		}
		/// <summary> Return a selected chuck of long buffer as a long array.</summary>
		/// <returns> long[]
		/// </returns>
		/// <param name="startingOffset">int
		/// </param>
		/// <param name="len">int
		/// </param>
		public long[] getLongArray(int startingOffset, int len)
		{
			if (size_Renamed_Field <= 0 || startingOffset < 0)
			{
				throw (new System.ArgumentException());
			}
			if ((startingOffset + len) > size_Renamed_Field)
			{
				throw (new System.IndexOutOfRangeException());
			}
			
			long[] result = new long[len]; // allocate result array
			
			int first_index = (startingOffset >> exp);
			int last_index = ((startingOffset + len) >> exp);
			
			//if ((startingOffset + len) % pageSize == 0) {
			if (((startingOffset + len) & r) == 0)
			{
				last_index--;
			}
			
			if (first_index == last_index)
			{
				// to see if there is a need to go across buffer boundry
				Array.Copy(((long[])bufferArrayList.oa[first_index]), startingOffset & r, result, 0, len);
			}
			else
			{
				int long_array_offset = 0;
				for (int i = first_index; i <= last_index; i++)
				{
					long[] currentChunk = (long[]) bufferArrayList.oa[i];
					if (i == first_index)
					// first section
					{
						Array.Copy(currentChunk, startingOffset & r, result, 0, pageSize - (startingOffset & r));
						long_array_offset += pageSize - (startingOffset & r);
					}
					else if (i == last_index)
					// last sections
					{
						Array.Copy(currentChunk, 0, result, long_array_offset, len - long_array_offset);
					}
					else
					{
						Array.Copy(currentChunk, 0, result, long_array_offset, pageSize);
						long_array_offset += pageSize;
					}
				}
			}
			
			return result;
		}
		/// <summary> Get the long val at given index value.</summary>
		/// <returns> long
		/// </returns>
		/// <param name="index">int
		/// </param>
		public long longAt(int index)
		{
			//if (index < 0 || index > size()) {
			// throw new ArgumentException("index out of bound");
			//}
			int pageNum = (index >> exp);
			// int offset = index % r;
			int offset = index & r;
			return ((long[]) bufferArrayList.oa[pageNum])[offset];
		}
		/// <summary> Get the lower 32 bit of the integer at the given index.</summary>
		/// <returns> int
		/// </returns>
		/// <param name="i">int
		/// </param>
		public int lower32At(int index)
		{
            //if (index < 0 || index > size())
            //{
            //    throw new System.IndexOutOfRangeException();
            //}
			int pageNum = (index >> exp);
			// int offset = index % pageSize;
			int offset = index & r;
			return (int) ((long[]) bufferArrayList.oa[pageNum])[offset];
		}
		/// <summary> Modify the value at the index to a new val.</summary>
		/// <param name="index">int
		/// </param>
		/// <param name="value">long
		/// </param>
		public void  modifyEntry(int index, long newValue)
		{
			
			//if (index < 0 || index > size_Renamed_Field + 1)
			//{
			//	throw new System.IndexOutOfRangeException();
			//}
			//((long[]) bufferArrayList.get((int) (index / pageSize)))[index % pageSize] =
			//UPGRADE_TODO: Method 'java.util.ArrayList.get' was not converted. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1095'"
			((long[]) bufferArrayList.oa[index >> exp])[index & r] = newValue;
		}
		/// <summary> Get the total number of longs in the buffer.</summary>
		/// <returns> int
		/// </returns>
		public int size()
		{
			return size_Renamed_Field;
		}
		/// <summary> Convert all longs into a long array.</summary>
		/// <returns> long[]
		/// </returns>
		public long[] toLongArray()
		{
			if (size_Renamed_Field > 0)
			{
				int s = size_Renamed_Field;
				long[] resultArray = new long[size_Renamed_Field];
				//copy all the content int into the resultArray
				int array_offset = 0;
				for (int i = 0; s > 0; i++)
				{
					Array.Copy((long[])bufferArrayList.oa[i], 0, resultArray, array_offset, (s < pageSize)?s:pageSize);
					//(i == (bufferArrayList.size_Renamed_Field - 1)) ? size - ((size>>exp)<<exp) : pageSize);
					//(i == (bufferArrayList.size_Renamed_Field - 1)) ? (size & r) : pageSize);
					s = s - pageSize;
					array_offset += pageSize;
				}
				return resultArray;
			}
			return null;
		}
		/// <summary> Return the upper 32 bit of the long at the index.</summary>
		/// <returns> int
		/// </returns>
		/// <param name="index">int
		/// </param>
		public int upper32At(int index)
		{
			//if (index < 0 || index >= size())
			//{
			//	throw new System.IndexOutOfRangeException();
			//}
			int pageNum = (index >> exp);
			int offset = index & r;
			return (int) ((((long[]) bufferArrayList.oa[pageNum])[offset] & (0xffffffffL << 32)) >> 32);
		}
		
		
		/// <summary> set teh size of long buffer to zero, capacity
		/// untouched so long buffer can be reused without
		/// any unnecessary and additional allocation
		/// 
		/// </summary>
		public void  clear()
		{
			size_Renamed_Field = 0;
		}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="newSz"></param>
        /// <returns></returns>
        public bool resize(int newSz)
        {
            if (newSz <= capacity && newSz >= 0)
            {
                this.size_Renamed_Field = newSz;
                return true;
            }
            else
                return false;
        }
	}
}