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
namespace com.ximpleware.parser
{
	public class ISO8859_15
	{
		//UPGRADE_NOTE: Final was removed from the declaration of 'chars '. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1003'"
		internal static readonly char[] chars = new char[0x100];
		public static char decode(byte b)
		{
			return chars[b];
		}
		static ISO8859_15()
		{
			{
				for (int i = 0; i < 256; i++)
				{
					chars[i] = (char) i;
				}
				chars[0xA4] = (char) (0x20AC);
				chars[0xA6] = (char) (0x0160);
				chars[0xA8] = (char) (0x0161);
				chars[0xB4] = (char) (0x017D);
				chars[0xB8] = (char) (0x017E);
				chars[0xBC] = (char) (0x0152);
				chars[0xBD] = (char) (0x0153);
				chars[0xBE] = (char) (0x0178);
			}
		}
	}
}