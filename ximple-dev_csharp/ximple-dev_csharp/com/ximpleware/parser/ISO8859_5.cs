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
	
	/// <summary> this class contains method to map a ISO-8859-5 char
	/// into a Unicode char
	/// 
	/// </summary>
	public class ISO8859_5
	{
		//UPGRADE_NOTE: Final was removed from the declaration of 'chars '. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1003'"
		internal static readonly char[] chars = new char[0x100];
		
		public static char decode(byte b)
		{
			return chars[b];
		}
		static ISO8859_5()
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
				chars[0xA1] = (char) (0x0401);
				chars[0xA2] = (char) (0x0402);
				chars[0xA3] = (char) (0x0403);
				chars[0xA4] = (char) (0x0404);
				chars[0xA5] = (char) (0x0405);
				chars[0xA6] = (char) (0x0406);
				chars[0xA7] = (char) (0x0407);
				chars[0xA8] = (char) (0x0408);
				chars[0xA9] = (char) (0x0409);
				chars[0xAA] = (char) (0x040A);
				chars[0xAB] = (char) (0x040B);
				chars[0xAC] = (char) (0x040C);
				chars[0xAD] = (char) (0x00AD);
				chars[0xAE] = (char) (0x040E);
				chars[0xAF] = (char) (0x040F);
				chars[0xB0] = (char) (0x0410);
				chars[0xB1] = (char) (0x0411);
				chars[0xB2] = (char) (0x0412);
				chars[0xB3] = (char) (0x0413);
				chars[0xB4] = (char) (0x0414);
				chars[0xB5] = (char) (0x0415);
				chars[0xB6] = (char) (0x0416);
				chars[0xB7] = (char) (0x0417);
				chars[0xB8] = (char) (0x0418);
				chars[0xB9] = (char) (0x0419);
				chars[0xBA] = (char) (0x041A);
				chars[0xBB] = (char) (0x041B);
				chars[0xBC] = (char) (0x041C);
				chars[0xBD] = (char) (0x041D);
				chars[0xBE] = (char) (0x041E);
				chars[0xBF] = (char) (0x041F);
				chars[0xC0] = (char) (0x0420);
				chars[0xC1] = (char) (0x0421);
				chars[0xC2] = (char) (0x0422);
				chars[0xC3] = (char) (0x0423);
				chars[0xC4] = (char) (0x0424);
				chars[0xC5] = (char) (0x0425);
				chars[0xC6] = (char) (0x0426);
				chars[0xC7] = (char) (0x0427);
				chars[0xC8] = (char) (0x0428);
				chars[0xC9] = (char) (0x0429);
				chars[0xCA] = (char) (0x042A);
				chars[0xCB] = (char) (0x042B);
				chars[0xCC] = (char) (0x042C);
				chars[0xCD] = (char) (0x042D);
				chars[0xCE] = (char) (0x042E);
				chars[0xCF] = (char) (0x042F);
				chars[0xD0] = (char) (0x0430);
				chars[0xD1] = (char) (0x0431);
				chars[0xD2] = (char) (0x0432);
				chars[0xD3] = (char) (0x0433);
				chars[0xD4] = (char) (0x0434);
				chars[0xD5] = (char) (0x0435);
				chars[0xD6] = (char) (0x0436);
				chars[0xD7] = (char) (0x0437);
				chars[0xD8] = (char) (0x0438);
				chars[0xD9] = (char) (0x0439);
				chars[0xDA] = (char) (0x043A);
				chars[0xDB] = (char) (0x043B);
				chars[0xDC] = (char) (0x043C);
				chars[0xDD] = (char) (0x043D);
				chars[0xDE] = (char) (0x043E);
				chars[0xDF] = (char) (0x043F);
				chars[0xE0] = (char) (0x0440);
				chars[0xE1] = (char) (0x0441);
				chars[0xE2] = (char) (0x0442);
				chars[0xE3] = (char) (0x0443);
				chars[0xE4] = (char) (0x0444);
				chars[0xE5] = (char) (0x0445);
				chars[0xE6] = (char) (0x0446);
				chars[0xE7] = (char) (0x0447);
				chars[0xE8] = (char) (0x0448);
				chars[0xE9] = (char) (0x0449);
				chars[0xEA] = (char) (0x044A);
				chars[0xEB] = (char) (0x044B);
				chars[0xEC] = (char) (0x044C);
				chars[0xED] = (char) (0x044D);
				chars[0xEE] = (char) (0x044E);
				chars[0xEF] = (char) (0x044F);
				chars[0xF0] = (char) (0x2116);
				chars[0xF1] = (char) (0x0451);
				chars[0xF2] = (char) (0x0452);
				chars[0xF3] = (char) (0x0453);
				chars[0xF4] = (char) (0x0454);
				chars[0xF5] = (char) (0x0455);
				chars[0xF6] = (char) (0x0456);
				chars[0xF7] = (char) (0x0457);
				chars[0xF8] = (char) (0x0458);
				chars[0xF9] = (char) (0x0459);
				chars[0xFA] = (char) (0x045A);
				chars[0xFB] = (char) (0x045B);
				chars[0xFC] = (char) (0x045C);
				chars[0xFD] = (char) (0x00A7);
				chars[0xFE] = (char) (0x045E);
				chars[0xFF] = (char) (0x045F);
			}
		}
	}
}