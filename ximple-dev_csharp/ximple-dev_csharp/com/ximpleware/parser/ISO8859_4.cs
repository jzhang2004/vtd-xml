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
	
	/// <summary> this class contains method to map a ISO-8859-4 char
	/// into a Unicode char
	/// 
	/// </summary>
	public class ISO8859_4
	{
		//UPGRADE_NOTE: Final was removed from the declaration of 'chars '. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1003'"
		internal static readonly char[] chars = new char[0x100];
		
		public static char decode(byte b)
		{
			return chars[b];
		}
		static ISO8859_4()
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
				chars[0xA0] = (char) (0x00A0);
				chars[0xA1] = (char) (0x0104);
				chars[0xA2] = (char) (0x0138);
				chars[0xA3] = (char) (0x0156);
				chars[0xA4] = (char) (0x00A4);
				chars[0xA5] = (char) (0x0128);
				chars[0xA6] = (char) (0x013B);
				chars[0xA7] = (char) (0x00A7);
				chars[0xA8] = (char) (0x00A8);
				chars[0xA9] = (char) (0x0160);
				chars[0xAA] = (char) (0x0112);
				chars[0xAB] = (char) (0x0122);
				chars[0xAC] = (char) (0x0166);
				chars[0xAD] = (char) (0x00AD);
				chars[0xAE] = (char) (0x017D);
				chars[0xAF] = (char) (0x00AF);
				chars[0xB0] = (char) (0x00B0);
				chars[0xB1] = (char) (0x0105);
				chars[0xB2] = (char) (0x02DB);
				chars[0xB3] = (char) (0x0157);
				chars[0xB4] = (char) (0x00B4);
				chars[0xB5] = (char) (0x0129);
				chars[0xB6] = (char) (0x013C);
				chars[0xB7] = (char) (0x02C7);
				chars[0xB8] = (char) (0x00B8);
				chars[0xB9] = (char) (0x0161);
				chars[0xBA] = (char) (0x0113);
				chars[0xBB] = (char) (0x0123);
				chars[0xBC] = (char) (0x0167);
				chars[0xBD] = (char) (0x014A);
				chars[0xBE] = (char) (0x017E);
				chars[0xBF] = (char) (0x014B);
				chars[0xC0] = (char) (0x0100);
				chars[0xC1] = (char) (0x00C1);
				chars[0xC2] = (char) (0x00C2);
				chars[0xC3] = (char) (0x00C3);
				chars[0xC4] = (char) (0x00C4);
				chars[0xC5] = (char) (0x00C5);
				chars[0xC6] = (char) (0x00C6);
				chars[0xC7] = (char) (0x012E);
				chars[0xC8] = (char) (0x010C);
				chars[0xC9] = (char) (0x00C9);
				chars[0xCA] = (char) (0x0118);
				chars[0xCB] = (char) (0x00CB);
				chars[0xCC] = (char) (0x0116);
				chars[0xCD] = (char) (0x00CD);
				chars[0xCE] = (char) (0x00CE);
				chars[0xCF] = (char) (0x012A);
				chars[0xD0] = (char) (0x0110);
				chars[0xD1] = (char) (0x0145);
				chars[0xD2] = (char) (0x014C);
				chars[0xD3] = (char) (0x0136);
				chars[0xD4] = (char) (0x00D4);
				chars[0xD5] = (char) (0x00D5);
				chars[0xD6] = (char) (0x00D6);
				chars[0xD7] = (char) (0x00D7);
				chars[0xD8] = (char) (0x00D8);
				chars[0xD9] = (char) (0x0172);
				chars[0xDA] = (char) (0x00DA);
				chars[0xDB] = (char) (0x00DB);
				chars[0xDC] = (char) (0x00DC);
				chars[0xDD] = (char) (0x0168);
				chars[0xDE] = (char) (0x016A);
				chars[0xDF] = (char) (0x00DF);
				chars[0xE0] = (char) (0x0101);
				chars[0xE1] = (char) (0x00E1);
				chars[0xE2] = (char) (0x00E2);
				chars[0xE3] = (char) (0x00E3);
				chars[0xE4] = (char) (0x00E4);
				chars[0xE5] = (char) (0x00E5);
				chars[0xE6] = (char) (0x00E6);
				chars[0xE7] = (char) (0x012F);
				chars[0xE8] = (char) (0x010D);
				chars[0xE9] = (char) (0x00E9);
				chars[0xEA] = (char) (0x0119);
				chars[0xEB] = (char) (0x00EB);
				chars[0xEC] = (char) (0x0117);
				chars[0xED] = (char) (0x00ED);
				chars[0xEE] = (char) (0x00EE);
				chars[0xEF] = (char) (0x012B);
				chars[0xF0] = (char) (0x0111);
				chars[0xF1] = (char) (0x0146);
				chars[0xF2] = (char) (0x014D);
				chars[0xF3] = (char) (0x0137);
				chars[0xF4] = (char) (0x00F4);
				chars[0xF5] = (char) (0x00F5);
				chars[0xF6] = (char) (0x00F6);
				chars[0xF7] = (char) (0x00F7);
				chars[0xF8] = (char) (0x00F8);
				chars[0xF9] = (char) (0x0173);
				chars[0xFA] = (char) (0x00FA);
				chars[0xFB] = (char) (0x00FB);
				chars[0xFC] = (char) (0x00FC);
				chars[0xFD] = (char) (0x0169);
				chars[0xFE] = (char) (0x016B);
				chars[0xFF] = (char) (0x02D9);
			}
		}
	}
}