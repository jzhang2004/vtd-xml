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
	/// <summary> Abstract Reader Class decodes underlying raw bytes.
	/// </summary>
	public interface IReader
	{
		/// <summary> Get next available Char from the buffer.
		/// </summary>
		/// <returns> boolean
		/// </returns>
		/// <exception cref="com.ximpleware.ParseException">Parse exception.
		/// </exception>
		/// <exception cref="com.ximpleware.EncodingException">Raw char to UCS conversion exception.
		/// </exception>
		/// <exception cref="com.ximpleware.EOFException">The end of file exception.
		/// </exception>
		int Char
		{
			get;
			
		}
		/// <summary> Skip the next char if it is a match.
		/// </summary>
		/// <returns> boolean
		/// </returns>
		/// <exception cref="com.ximpleware.ParseException">Parse exception.
		/// </exception>
		/// <exception cref="com.ximpleware.EncodingException">Raw char to UCS conversion exception.
		/// </exception>
		/// <exception cref="com.ximpleware.EOFException">The end of file exception.
		/// </exception>
		bool skipChar(int ch);
	}
}