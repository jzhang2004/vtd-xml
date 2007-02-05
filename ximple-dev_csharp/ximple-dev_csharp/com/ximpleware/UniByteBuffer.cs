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
using System;
namespace com.ximpleware
{
	/// <summary> A simple wrapper around monolithic byte array implementing IByteBuffer interface.
	/// </summary>
	/// <author> : 
	/// </author>
	public class UniByteBuffer : IByteBuffer
	{
		private byte[] ba;
		/// <summary> UniByteBuffer constructor comment.</summary>
		/// <exception cref="IllegalArguement"> when b is a null pointer
		/// </exception>
		public UniByteBuffer(byte[] b):base()
		{
			if (b == null)
				throw new System.ArgumentException();
			ba = b;
		}
		/// <summary> Get the byte at the index.
		/// </summary>
		/// <returns> byte
		/// </returns>
		/// <param name="index">int
		/// </param>
		public byte byteAt(int index)
		{
			return ba[index];
		}
		/// <summary> Return the token in its original encoding format.
		/// Creation date: (11/28/03 7:02:07 PM)
		/// </summary>
		/// <returns> byte[]
		/// </returns>
		/// <param name="offset">int
		/// </param>
		/// <param name="len">int
		/// </param>
		public byte[] getBytes(int offset, int len)
		{
			byte[] b = new byte[len];
			Array.Copy(ba, offset, b, 0, len);
			return b;
		}
		/// <summary> Get total size in terms of # of bytes.
		/// Creation date: (11/25/03 5:13:10 PM)
		/// </summary>
		/// <returns> int
		/// </returns>
		public int length()
		{
			return ba.Length;
		}
        /// <summary>
        ///  get the whole XML
        /// </summary>
        /// <returns></returns>

        public byte[] getBytes()
        {
            return ba;
        }
	}
}