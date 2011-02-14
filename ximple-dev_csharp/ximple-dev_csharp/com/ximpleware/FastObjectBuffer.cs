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
	
	/// <summary> Fast object array implementation
	/// 
	/// </summary>
	public class FastObjectBuffer : IObjectBuffer
	{
		/// <summary> Returns the total allocated capacity of this buffer instance.</summary>
		/// <returns> int
		/// </returns>
		virtual public int Capacity
		{
			get
			{
				return capacity;
			}
			
		}
		/// <summary> Returns the page size of this buffer instance.
		/// Creation date: (7/17/03 6:38:02 PM)
		/// </summary>
		/// <returns> int
		/// </returns>
		virtual public int PageSize
		{
			get
			{
				return pageSize;
			}
			
		}
		/* bufferArrayList is a resizable array list of int buffers
		*
		*/
		private com.ximpleware.ArrayList bufferArrayList;
		
		/// <summary> Total capacity of the ObjectBuffer</summary>
		private int capacity;
		
		/// <summary> Page size of the incremental growth of the object Buffer</summary>
		private int pageSize;
		
		/// <summary> Total number of objects in the IntBuffer</summary>
		protected internal int size_Renamed_Field;
		private int exp;
		private int r;
		/// <summary> FastIntBuffer constructor comment.</summary>
		public FastObjectBuffer()
		{
			size_Renamed_Field = 0;
			capacity = 0;
			pageSize = 1024;
			exp = 10;
			r = 1023;
			bufferArrayList = new com.ximpleware.ArrayList();
		}
		/// <summary> Constructor with adjustable buffer page size of the value bfz</summary>
		/// <param name="bfz">int  is the size of the internal buffer
		/// </param>
		public FastObjectBuffer(int e)
		{
			if (e < 0)
			{
				throw new System.ArgumentException();
			}
			capacity = size_Renamed_Field = 0;
			pageSize = 1 << e;
			exp = e;
			r = pageSize - 1;
			bufferArrayList = new com.ximpleware.ArrayList();
		}
		/// <summary> Append an object array to the end of this buffer instance</summary>
		/// <param name="obj_array">Object[]
		/// </param>
		public virtual void  append(System.Object[] obj_array)
		{
			if (obj_array == null)
			{
				throw new System.NullReferenceException();
			}
			// no additional buffer space needed
			int lastBufferIndex;
			System.Object[] lastBuffer;
			
			if (bufferArrayList.size == 0)
			{
				lastBuffer = new System.Object[pageSize];
				bufferArrayList.Add(lastBuffer);
				lastBufferIndex = 0;
				capacity = pageSize;
			}
			else
			{
				lastBufferIndex = System.Math.Min((size_Renamed_Field >> exp), bufferArrayList.size - 1);
				lastBuffer = (System.Object[]) bufferArrayList.oa[lastBufferIndex];
			}
			
			if ((this.size_Renamed_Field + obj_array.Length) < this.capacity)
			{
				//get the last buffer from the bufferListArray
				//obtain the starting offset in that buffer to which the data is to be copied
				//update length        
				
				//System.arraycopy(input, 0, lastBuffer, size % pageSize, input.length);
				if (this.size_Renamed_Field + obj_array.Length < ((lastBufferIndex + 1) << exp))
				{
					Array.Copy(obj_array, 0, lastBuffer, size_Renamed_Field & r, obj_array.Length);
				}
				else
				{
					int offset = pageSize - (size_Renamed_Field & r);
					// copy the first part
					Array.Copy(obj_array, 0, lastBuffer, size_Renamed_Field & r, offset);
					// copy the middle part
					
					int l = obj_array.Length - offset;
					int k = (l) >> exp;
					int z;
					for (z = 1; z <= k; z++)
					{
						Array.Copy(obj_array, offset, (Object[])bufferArrayList.oa[lastBufferIndex + z], 0, pageSize);
						offset += pageSize;
					}
					// copy the last part
					Array.Copy(obj_array, offset, (Object[])bufferArrayList.oa[lastBufferIndex + z], 0, l & r);
				}
				size_Renamed_Field += obj_array.Length;
				return ;
			}
			// new buffers needed
			else
			{
				
				// compute the number of additional buffers needed
				//       int n =
				//           ((int) ((int_array.length + size) / pageSize))
				//               + (((int_array.length + size) % pageSize) > 0 ? 1 : 0)
				//               - (int) (capacity / pageSize);
				int n = ((obj_array.Length + size_Renamed_Field) >> exp) + (((obj_array.Length + size_Renamed_Field) & r) > 0?1:0) - (capacity >> exp);
				// create these buffers
				// add to bufferArrayList
				
				//System.arraycopy(int_array, 0, lastBuffer, size % pageSize, capacity - size);
				Array.Copy(obj_array, 0, lastBuffer, size_Renamed_Field & r, capacity - size_Renamed_Field);
				
				for (int i = 0; i < n; i++)
				{
					System.Object[] newBuffer = new System.Object[pageSize];
					if (i < n - 1)
					{
						// full copy 
						Array.Copy(obj_array, pageSize * i + capacity - size_Renamed_Field, newBuffer, 0, pageSize);
					}
					else
					{
						// last page
						Array.Copy(obj_array, pageSize * i + capacity - size_Renamed_Field, newBuffer, 0, obj_array.Length + this.size_Renamed_Field - capacity - pageSize * i);
					}
					bufferArrayList.Add(newBuffer);
				}
				// update length
				size_Renamed_Field += obj_array.Length;
				// update capacity
				capacity += n * pageSize;
				// update
			}
		}
		/// <summary> Append a single object to the end of this buffer Instance</summary>
		/// <param name="i">int
		/// </param>
		public virtual void  append(System.Object obj)
		{

            if (this.size_Renamed_Field < this.capacity)
            {
                //get the last buffer from the bufferListArray
                //obtain the starting offset in that buffer to which the data is to be copied
                //update length
                //System.arraycopy(long_array, 0, lastBuffer, size % pageSize, long_array.length);
                ((Object[])bufferArrayList.oa[bufferArrayList.size - 1])[size_Renamed_Field & r] = obj;
                //       lastBuffer[size % pageSize] = i;
                size_Renamed_Field += 1;
            }
            else // new buffers needed
            {
                Object[] newBuffer = new Object[pageSize];
                size_Renamed_Field++;
                capacity += pageSize;
                bufferArrayList.Add(newBuffer);
                newBuffer[0] = obj;
            }
		}
		/// <summary> Returns a single object array representing every object in this buffer instance</summary>
		/// <returns> Object[]  (null if there isn't anything left in the buffer   
		/// </returns>
		/// <param name="startingOffset">int
		/// </param>
		/// <param name="length">int
		/// </param>
		public virtual System.Object[] getObjectArray(int startingOffset, int len)
		{
			if (size_Renamed_Field <= 0 || startingOffset < 0)
			{
				throw (new System.ArgumentException());
			}
			if ((startingOffset + len) > this.size_Renamed_Field)
			{
				throw (new System.IndexOutOfRangeException());
			}
			System.Object[] result = new System.Object[len]; // allocate result array
			
			//   int first_index = (int) (startingOffset / pageSize);
			//   int last_index = (int) ((startingOffset + len) / pageSize);
			//   if ((startingOffset + len) % pageSize == 0) {
			//       last_index--;
			//   }
			int first_index = startingOffset >> exp;
			int last_index = (startingOffset + len) >> exp;
			if (((startingOffset + len) & r) == 0)
			{
				last_index--;
			}
			
			if (first_index == last_index)
			{
				// to see if there is a need to go across buffer boundry
				Array.Copy((Object[])(bufferArrayList.oa[first_index]), startingOffset & r, result, 0, len);
			}
			else
			{
				int obj_array_offset = 0;
				for (int i = first_index; i <= last_index; i++)
				{
					System.Object[] currentChunk = (System.Object[]) bufferArrayList.oa[i];
					if (i == first_index)
					// first section
					{
						Array.Copy(currentChunk, startingOffset & r, result, 0, pageSize - (startingOffset & r));
						//               int_array_offset += pageSize - (startingOffset) % pageSize;
						obj_array_offset += pageSize - (startingOffset & r);
					}
					else if (i == last_index)
					// last sections
					{
						Array.Copy(currentChunk, 0, result, obj_array_offset, len - obj_array_offset);
					}
					else
					{
						Array.Copy(currentChunk, 0, result, obj_array_offset, pageSize);
						obj_array_offset += pageSize;
					}
				}
			}
			return result;
		}
		/// <summary> Get the object at the location specified by index.</summary>
		/// <returns> int
		/// </returns>
		/// <param name="index">int
		/// </param>
		public virtual System.Object objectAt(int index)
		{
			if (index < 0 || index > size() - 1)
			{
				throw new System.IndexOutOfRangeException();
			}
			//   int pageNum = (int) index / pageSize;
			int pageNum = index >> exp;
			//System.out.println("page Number is "+pageNum); 
			//   int offset = index % pageSize;
			int offset = index & r;
			return ((System.Object[]) bufferArrayList.oa[pageNum])[offset];
		}
		/// <summary> Assigns a new int value to location index of the buffer instance.</summary>
		/// <param name="index">int
		/// </param>
		/// <param name="newValue">int
		/// </param>
		/// <Exception>   Throws IndexOutOfBoundsException </Exception>
		public virtual void  modifyEntry(int index, System.Object newValue)
		{
			
			if (index < 0 || index > size_Renamed_Field - 1)
			{
				throw new System.IndexOutOfRangeException();
			}
			
			//       ((int[]) bufferArrayList.get((int) (index / pageSize)))[index % pageSize] =
			//UPGRADE_TODO: Method 'java.util.ArrayList.get' was not converted. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1095'"
			((System.Object[]) bufferArrayList.oa[(index >> exp)])[index & r] = newValue;
		}
		/// <summary> Returns the total number of objects in the buffer instance</summary>
		/// <returns> int
		/// </returns>
		public virtual int size()
		{
			return size_Renamed_Field;
		}
		/// <summary> Returns the object array corresponding to all objects in this buffer instance</summary>
		/// <returns> Object[] (null if the buffer is empty)
		/// </returns>
		public virtual System.Object[] toObjectArray()
		{
			if (size_Renamed_Field > 0)
			{
				int s = size_Renamed_Field;
				System.Object[] resultArray = new System.Object[size_Renamed_Field];
				//copy all the content int into the resultArray
				int array_offset = 0;
				for (int i = 0; s > 0; i++)
				{
					Array.Copy((Object[])bufferArrayList.oa[i], 0, resultArray, array_offset, (s < pageSize)?s:pageSize);
					//           (i == (bufferArrayList.size_Renamed_Field - 1)) ? size() % pageSize : pageSize);
					s = s - pageSize;
					array_offset += pageSize;
				}
				return resultArray;
			}
			return null;
		}
		
		
		/// <summary> set the size of object buffer to zero, capacity
		/// untouched so object buffer can be reused without
		/// any unnecessary and additional allocation
		/// 
		/// </summary>
		public virtual void  clear()
		{
			size_Renamed_Field = 0;
		}
	}
}