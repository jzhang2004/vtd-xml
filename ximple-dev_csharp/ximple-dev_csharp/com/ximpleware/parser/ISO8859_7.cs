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
	
	/// <summary> this class contains method to map a ISO-8859-7 char
	/// into a Unicode char
	/// 
	/// </summary>
	public class ISO8859_7
	{
		//UPGRADE_NOTE: Final was removed from the declaration of 'chars '. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1003'"
		internal static readonly char[] chars = new char[0x100];
		
		public static char decode(byte b)
		{
			return chars[b];
		}
		static ISO8859_7()
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
				chars[0xA1] = (char) (0x02BD);
				chars[0xA2] = (char) (0x02BC);
				chars[0xA3] = (char) (0x00A3);
				chars[0xA6] = (char) (0x00A6);
				chars[0xA7] = (char) (0x00A7);
				chars[0xA8] = (char) (0x00A8);
				chars[0xA9] = (char) (0x00A9);
				chars[0xAB] = (char) (0x00AB);
				chars[0xAC] = (char) (0x00AC);
				chars[0xAD] = (char) (0x00AD);
				chars[0xAF] = (char) (0x2015);
				chars[0xB0] = (char) (0x00B0);
				chars[0xB1] = (char) (0x00B1);
				chars[0xB2] = (char) (0x00B2);
				chars[0xB3] = (char) (0x00B3);
				chars[0xB4] = (char) (0x0384);
				chars[0xB5] = (char) (0x0385);
				chars[0xB6] = (char) (0x0386);
				chars[0xB7] = (char) (0x00B7);
				chars[0xB8] = (char) (0x0388);
				chars[0xB9] = (char) (0x0389);
				chars[0xBA] = (char) (0x038A);
				chars[0xBB] = (char) (0x00BB);
				chars[0xBC] = (char) (0x038C);
				chars[0xBD] = (char) (0x00BD);
				chars[0xBE] = (char) (0x038E);
				chars[0xBF] = (char) (0x038F);
				chars[0xC0] = (char) (0x0390);
				chars[0xC1] = (char) (0x0391);
				chars[0xC2] = (char) (0x0392);
				chars[0xC3] = (char) (0x0393);
				chars[0xC4] = (char) (0x0394);
				chars[0xC5] = (char) (0x0395);
				chars[0xC6] = (char) (0x0396);
				chars[0xC7] = (char) (0x0397);
				chars[0xC8] = (char) (0x0398);
				chars[0xC9] = (char) (0x0399);
				chars[0xCA] = (char) (0x039A);
				chars[0xCB] = (char) (0x039B);
				chars[0xCC] = (char) (0x039C);
				chars[0xCD] = (char) (0x039D);
				chars[0xCE] = (char) (0x039E);
				chars[0xCF] = (char) (0x039F);
				chars[0xD0] = (char) (0x03A0);
				chars[0xD1] = (char) (0x03A1);
				chars[0xD3] = (char) (0x03A3);
				chars[0xD4] = (char) (0x03A4);
				chars[0xD5] = (char) (0x03A5);
				chars[0xD6] = (char) (0x03A6);
				chars[0xD7] = (char) (0x03A7);
				chars[0xD8] = (char) (0x03A8);
				chars[0xD9] = (char) (0x03A9);
				chars[0xDA] = (char) (0x03AA);
				chars[0xDB] = (char) (0x03AB);
				chars[0xDC] = (char) (0x03AC);
				chars[0xDD] = (char) (0x03AD);
				chars[0xDE] = (char) (0x03AE);
				chars[0xDF] = (char) (0x03AF);
				chars[0xE0] = (char) (0x03B0);
				chars[0xE1] = (char) (0x03B1);
				chars[0xE2] = (char) (0x03B2);
				chars[0xE3] = (char) (0x03B3);
				chars[0xE4] = (char) (0x03B4);
				chars[0xE5] = (char) (0x03B5);
				chars[0xE6] = (char) (0x03B6);
				chars[0xE7] = (char) (0x03B7);
				chars[0xE8] = (char) (0x03B8);
				chars[0xE9] = (char) (0x03B9);
				chars[0xEA] = (char) (0x03BA);
				chars[0xEB] = (char) (0x03BB);
				chars[0xEC] = (char) (0x03BC);
				chars[0xED] = (char) (0x03BD);
				chars[0xEE] = (char) (0x03BE);
				chars[0xEF] = (char) (0x03BF);
				chars[0xF0] = (char) (0x03C0);
				chars[0xF1] = (char) (0x03C1);
				chars[0xF2] = (char) (0x03C2);
				chars[0xF3] = (char) (0x03C3);
				chars[0xF4] = (char) (0x03C4);
				chars[0xF5] = (char) (0x03C5);
				chars[0xF6] = (char) (0x03C6);
				chars[0xF7] = (char) (0x03C7);
				chars[0xF8] = (char) (0x03C8);
				chars[0xF9] = (char) (0x03C9);
				chars[0xFA] = (char) (0x03CA);
				chars[0xFB] = (char) (0x03CB);
				chars[0xFC] = (char) (0x03CC);
				chars[0xFD] = (char) (0x03CD);
				chars[0xFE] = (char) (0x03CE);
			}
		}
	}
}