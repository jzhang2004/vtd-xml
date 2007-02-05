/* 
* Copyright (C) 2002-2007 XimpleWare, info@ximpleware.com
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
using System;
namespace com.ximpleware
{
	//import com.ximpleware.NavException;
	
	/// <summary> This class is used as the global stack in VTDNav class.
	/// The VTDNav object instantiates a context Buffer object, then everytime
	/// load/store is called, current context information gets pushed/popped to/from the 
	/// ContextBuffer.
	/// Creation date: (11/16/03 4:30:45 PM)
	/// </summary>
	public class ContextBuffer
	{
		private System.Collections.ArrayList bufferArrayList;
		private int capacity;
		private int pageSize;
		protected internal int size;
		private int incSize;
		private int n; // for fast divide
		private int r; // mask for remainder
		/// <summary> ContextBuffer constructor comment.
		/// inc is the # of int to be pushed/pop to/from the underlying storage
		/// </summary>
		/// <param name="pageSize">int
		/// </param>
		public ContextBuffer(int i):base()
		{
			pageSize = 1024;
			n = 10; //1<<10 == 1024
			r = pageSize - 1;
			incSize = i;
			if (incSize < 0)
				throw new System.ArgumentException();
			bufferArrayList = new System.Collections.ArrayList();
		}
		/// <summary> ContextBuffer constructor comment.
		/// incSize is the # of int to be pushed/pop to/from the underlying storage
		/// Creation date: (11/16/03 8:02:21 PM)
		/// </summary>
		/// <param name="p">int (pageSize equals (1<<p)
		/// </param>
		/// <param name="i">int
		/// </param>
		public ContextBuffer(int p, int i)
		{
			if (p < 0)
				throw new System.ArgumentException("invalid Buffer size");
			pageSize = (1 << p);
			r = pageSize - 1;
			n = p;
			incSize = i;
			if (incSize < 0)
				throw new System.ArgumentException("context buffer's incremental size must be greater than zero");
			bufferArrayList = new System.Collections.ArrayList();
		}
		/// <summary> Pop the content value back into an integer array.
		/// Creation date: (11/17/03 1:07:42 AM)
		/// </summary>
		/// <param name="output">int[]
		/// </param>
		/// <returns> boolean   If load is successful.
		/// 
		/// </returns>
		public  bool load(int[] output)
		{
			
			if (size < incSize)
			{
				return false;
			}
			
			int startingOffset = size - incSize;
			int len = incSize;
			
			
			//int[] result = new int[len]; // allocate result array
			//if (pageSize != 1) {
			//int first_index = (int) (startingOffset / pageSize);
			//int last_index = (int) ((startingOffset + len) / pageSize);
			//if ((startingOffset + len) % pageSize == 0) {
			int first_index = (startingOffset >> n);
			int last_index = ((startingOffset + len) >> n);
			if (((startingOffset + len) & r) == 0)
			{
				last_index--;
			}
			
			if (first_index == last_index)
			{
				// to see if there is a need to go across buffer boundry
				Array.Copy(((int[])bufferArrayList[first_index]), startingOffset & r, output, 0, len);
			}
			else
			{
				int int_array_offset = 0;
				for (int i = first_index; i <= last_index; i++)
				{
					int[] currentChunk = (int[]) bufferArrayList[i];
					if (i == first_index)
					// first section
					{
						Array.Copy(currentChunk, startingOffset & r, output, 0, pageSize - (startingOffset & r));
						//int_array_offset += pageSize - (startingOffset) % pageSize;
						int_array_offset += pageSize - (startingOffset & r);
					}
					else if (i == last_index)
					// last sections
					{
						Array.Copy(currentChunk, 0, output, int_array_offset, len - int_array_offset);
					}
					else
					{
						Array.Copy(currentChunk, 0, output, int_array_offset, pageSize);
						int_array_offset += pageSize;
					}
				}
			}
			
			size -= incSize;
			return true;
		}		
		
		/// <summary> Set the context buffer size to zero
		/// capacity untouched
		/// </summary>
		public void  clear()
		{
			size = 0;
		}
		/// <summary> Push the array content on to the stack.
		/// Creation date: (11/17/03 1:06:43 AM)
		/// </summary>
		/// <param name="input">int[]
		/// </param>
		public void  store(int[] input)
		{
			
			if (input == null)
			{
				throw new System.ArgumentException("context buffer can't store a null pointer");
			}
			
			//if (input.length != incSize) {
			// throw a derivative of runtime exceptions
			//    throw new NavException("Store: Input size doesn't match increment size");
			//}
			
			// no additional buffer space needed
			int lastBufferIndex;
			int[] lastBuffer;
			
			if (bufferArrayList.Count == 0)
			{
				lastBuffer = new int[pageSize];
				bufferArrayList.Add(lastBuffer);
				lastBufferIndex = 0;
				capacity = pageSize;
			}
			else
			{
				lastBufferIndex = System.Math.Min((size >> n), bufferArrayList.Count - 1);
				lastBuffer = (int[]) bufferArrayList[lastBufferIndex];
			}
			
			
			if ((this.size + input.Length) < this.capacity)
			{
				//get the last buffer from the bufferListArray
				//obtain the starting offset in that buffer to which the data is to be copied
				//update length
				
				//System.arraycopy(input, 0, lastBuffer, size % pageSize, input.length);
				if (this.size + input.Length < ((lastBufferIndex + 1) << n))
				{
					Array.Copy(input, 0, lastBuffer, size & r, input.Length);
				}
				else
				{
					int offset = pageSize - (size & r);
					// copy the first part
					Array.Copy(input, 0, lastBuffer, size & r, offset);
					// copy the middle part
					
					int l = input.Length - (offset);
					int k = (l) >> n;
					int z;
					for (z = 1; z <= k; z++)
					{
						Array.Copy(input, offset, (int[])bufferArrayList[lastBufferIndex + z], 0, pageSize);
						offset += pageSize;
					}
					// copy the last part
					Array.Copy(input, offset, (int[])bufferArrayList[lastBufferIndex + z], 0, l & r);
				}
				size += input.Length;
				return ;
				//System.out.println("     --+++  buffer size "+size);
				//size += input.length;
			}
			// new buffers needed
			else
			{
				
				// compute the number of additional buffers needed
				int k = ((input.Length + size) >> n) + (((input.Length + size) & r) > 0?1:0) - (capacity >> n);
				// create these buffers
				// add to bufferArrayList
				//System.arraycopy(input, 0, lastBuffer, size % pageSize, capacity - size);
				Array.Copy(input, 0, lastBuffer, size & r, capacity - size);
				
				for (int i = 0; i < k; i++)
				{
					int[] newBuffer = new int[pageSize];
					if (i < k - 1)
					{
						// full copy 
						Array.Copy(input, pageSize * i + capacity - size, newBuffer, 0, pageSize);
					}
					else
					{
						// last page
						Array.Copy(input, pageSize * i + capacity - size, newBuffer, 0, (input.Length + size) - pageSize * i - capacity);
					}
					bufferArrayList.Add(newBuffer);
				}
				// update length
				size += input.Length;
				// update capacity
				capacity += (k << n);
				// update
			}
		}
	}
}