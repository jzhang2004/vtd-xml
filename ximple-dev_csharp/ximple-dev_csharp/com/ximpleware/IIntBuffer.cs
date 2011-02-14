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
	/// <summary> Abstract int buffer storage interface.
	/// </summary>
	public interface IIntBuffer
	{
		/// <summary> Get the int at the index value.
		/// </summary>
		/// <param name="i">int
		/// </param>
		int intAt(int i);
		/// <summary> Modify the int at index to value val.
		/// Creation date: (11/23/03 6:16:56 PM)
		/// </summary>
		/// <param name="a">int
		/// </param>
		/// <param name="b">int
		/// </param>
		void  modifyEntry(int index, int val);
		/// <summary> Get the total number of int in the buffer.
		/// </summary>
		/// <returns> int
		/// </returns>
		int size();
	}
}
