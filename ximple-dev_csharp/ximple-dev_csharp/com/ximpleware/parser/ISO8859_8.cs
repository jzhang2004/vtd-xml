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
	
	/// <summary> this class contains method to map a ISO-8859-8 char
	/// into a Unicode char
	/// 
	/// </summary>
	public class ISO8859_8
	{
		//UPGRADE_NOTE: Final was removed from the declaration of 'chars '. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1003'"
		internal static readonly char[] chars = new char[0x100];
		
		public static char decode(byte b)
		{
			return chars[b];
		}
		static ISO8859_8()
		{
			{
				for (int i = 0; i < 128; i++)
				{
					chars[i] = (char) i;
				}
				for (int i = 128; i < 256; i++)
				{
					chars[i] = (char) (0xfffd);
				}
				chars[0xAA] = (char) (0x00D7);
				chars[0xAB] = (char) (0x00AB);
				chars[0xAC] = (char) (0x00AC);
				chars[0xAD] = (char) (0x00AD);
				chars[0xAE] = (char) (0x00AE);
				chars[0xAF] = (char) (0x203E);
				chars[0xB0] = (char) (0x00B0);
				chars[0xB1] = (char) (0x00B1);
				chars[0xB2] = (char) (0x00B2);
				chars[0xB3] = (char) (0x00B3);
				chars[0xB4] = (char) (0x00B4);
				chars[0xB5] = (char) (0x00B5);
				chars[0xB6] = (char) (0x00B6);
				chars[0xB7] = (char) (0x00B7);
				chars[0xB8] = (char) (0x00B8);
				chars[0xB9] = (char) (0x00B9);
				chars[0xBA] = (char) (0x00F7);
				chars[0xBB] = (char) (0x00BB);
				chars[0xBC] = (char) (0x00BC);
				chars[0xBD] = (char) (0x00BD);
				chars[0xBE] = (char) (0x00BE);
				chars[0xDF] = (char) (0x2017);
				chars[0xE0] = (char) (0x05D0);
				chars[0xE1] = (char) (0x05D1);
				chars[0xE2] = (char) (0x05D2);
				chars[0xE3] = (char) (0x05D3);
				chars[0xE4] = (char) (0x05D4);
				chars[0xE5] = (char) (0x05D5);
				chars[0xE6] = (char) (0x05D6);
				chars[0xE7] = (char) (0x05D7);
				chars[0xE8] = (char) (0x05D8);
				chars[0xE9] = (char) (0x05D9);
				chars[0xEA] = (char) (0x05DA);
				chars[0xEB] = (char) (0x05DB);
				chars[0xEC] = (char) (0x05DC);
				chars[0xED] = (char) (0x05DD);
				chars[0xEE] = (char) (0x05DE);
				chars[0xEF] = (char) (0x05DF);
				chars[0xF0] = (char) (0x05E0);
				chars[0xF1] = (char) (0x05E1);
				chars[0xF2] = (char) (0x05E2);
				chars[0xF3] = (char) (0x05E3);
				chars[0xF4] = (char) (0x05E4);
				chars[0xF5] = (char) (0x05E5);
				chars[0xF6] = (char) (0x05E6);
				chars[0xF7] = (char) (0x05E7);
				chars[0xF8] = (char) (0x05E8);
				chars[0xF9] = (char) (0x05E9);
				chars[0xFA] = (char) (0x05EA);
			}
		}
	}
}