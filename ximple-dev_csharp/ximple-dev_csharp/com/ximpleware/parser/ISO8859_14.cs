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
	public class ISO8859_14
	{
		//UPGRADE_NOTE: Final was removed from the declaration of 'chars '. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1003'"
		internal static readonly char[] chars = new char[0x100];
		
		public static char decode(byte b)
		{
			return chars[b];
		}
		static ISO8859_14()
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
				chars[0xA0] = (char) (0x00A0); //NO-BREAK SPACE
				chars[0xA1] = (char) (0x1E02); //LATIN CAPITAL LETTER B WITH DOT ABOVE
				chars[0xA2] = (char) (0x1E03); //LATIN SMALL LETTER B WITH DOT ABOVE
				chars[0xA3] = (char) (0x00A3); //POUND SIGN
				chars[0xA4] = (char) (0x010A); //LATIN CAPITAL LETTER C WITH DOT ABOVE
				chars[0xA5] = (char) (0x010B); //LATIN SMALL LETTER C WITH DOT ABOVE
				chars[0xA6] = (char) (0x1E0A); //LATIN CAPITAL LETTER D WITH DOT ABOVE
				chars[0xA7] = (char) (0x00A7); //SECTION SIGN
				chars[0xA8] = (char) (0x1E80); //LATIN CAPITAL LETTER W WITH GRAVE
				chars[0xA9] = (char) (0x00A9); //COPYRIGHT SIGN
				chars[0xAA] = (char) (0x1E82); //LATIN CAPITAL LETTER W WITH ACUTE
				chars[0xAB] = (char) (0x1E0B); //LATIN SMALL LETTER D WITH DOT ABOVE
				chars[0xAC] = (char) (0x1EF2); //LATIN CAPITAL LETTER Y WITH GRAVE
				chars[0xAD] = (char) (0x00AD); //SOFT HYPHEN
				chars[0xAE] = (char) (0x00AE); //REGISTERED SIGN
				chars[0xAF] = (char) (0x0178); //LATIN CAPITAL LETTER Y WITH DIAERESIS
				chars[0xB0] = (char) (0x1E1E); //LATIN CAPITAL LETTER F WITH DOT ABOVE
				chars[0xB1] = (char) (0x1E1F); //LATIN SMALL LETTER F WITH DOT ABOVE
				chars[0xB2] = (char) (0x0120); //LATIN CAPITAL LETTER G WITH DOT ABOVE
				chars[0xB3] = (char) (0x0121); //LATIN SMALL LETTER G WITH DOT ABOVE
				chars[0xB4] = (char) (0x1E40); //LATIN CAPITAL LETTER M WITH DOT ABOVE
				chars[0xB5] = (char) (0x1E41); //LATIN SMALL LETTER M WITH DOT ABOVE
				chars[0xB6] = (char) (0x00B6); //PILCROW SIGN
				chars[0xB7] = (char) (0x1E56); //LATIN CAPITAL LETTER P WITH DOT ABOVE
				chars[0xB8] = (char) (0x1E81); //LATIN SMALL LETTER W WITH GRAVE
				chars[0xB9] = (char) (0x1E57); //LATIN SMALL LETTER P WITH DOT ABOVE
				chars[0xBA] = (char) (0x1E83); //LATIN SMALL LETTER W WITH ACUTE
				chars[0xBB] = (char) (0x1E60); //LATIN CAPITAL LETTER S WITH DOT ABOVE
				chars[0xBC] = (char) (0x1EF3); //LATIN SMALL LETTER Y WITH GRAVE
				chars[0xBD] = (char) (0x1E84); //LATIN CAPITAL LETTER W WITH DIAERESIS
				chars[0xBE] = (char) (0x1E85); //LATIN SMALL LETTER W WITH DIAERESIS
				chars[0xBF] = (char) (0x1E61); //LATIN SMALL LETTER S WITH DOT ABOVE
				chars[0xC0] = (char) (0x00C0); //LATIN CAPITAL LETTER A WITH GRAVE
				chars[0xC1] = (char) (0x00C1); //LATIN CAPITAL LETTER A WITH ACUTE
				chars[0xC2] = (char) (0x00C2); //LATIN CAPITAL LETTER A WITH CIRCUMFLEX
				chars[0xC3] = (char) (0x00C3); //LATIN CAPITAL LETTER A WITH TILDE
				chars[0xC4] = (char) (0x00C4); //LATIN CAPITAL LETTER A WITH DIAERESIS
				chars[0xC5] = (char) (0x00C5); //LATIN CAPITAL LETTER A WITH RING ABOVE
				chars[0xC6] = (char) (0x00C6); //LATIN CAPITAL LETTER AE
				chars[0xC7] = (char) (0x00C7); //LATIN CAPITAL LETTER C WITH CEDILLA
				chars[0xC8] = (char) (0x00C8); //LATIN CAPITAL LETTER E WITH GRAVE
				chars[0xC9] = (char) (0x00C9); //LATIN CAPITAL LETTER E WITH ACUTE
				chars[0xCA] = (char) (0x00CA); //LATIN CAPITAL LETTER E WITH CIRCUMFLEX
				chars[0xCB] = (char) (0x00CB); //LATIN CAPITAL LETTER E WITH DIAERESIS
				chars[0xCC] = (char) (0x00CC); //LATIN CAPITAL LETTER I WITH GRAVE
				chars[0xCD] = (char) (0x00CD); //LATIN CAPITAL LETTER I WITH ACUTE
				chars[0xCE] = (char) (0x00CE); //LATIN CAPITAL LETTER I WITH CIRCUMFLEX
				chars[0xCF] = (char) (0x00CF); //LATIN CAPITAL LETTER I WITH DIAERESIS
				chars[0xD0] = (char) (0x0174); //LATIN CAPITAL LETTER W WITH CIRCUMFLEX
				chars[0xD1] = (char) (0x00D1); //LATIN CAPITAL LETTER N WITH TILDE
				chars[0xD2] = (char) (0x00D2); //LATIN CAPITAL LETTER O WITH GRAVE
				chars[0xD3] = (char) (0x00D3); //LATIN CAPITAL LETTER O WITH ACUTE
				chars[0xD4] = (char) (0x00D4); //LATIN CAPITAL LETTER O WITH CIRCUMFLEX
				chars[0xD5] = (char) (0x00D5); //LATIN CAPITAL LETTER O WITH TILDE
				chars[0xD6] = (char) (0x00D6); //LATIN CAPITAL LETTER O WITH DIAERESIS
				chars[0xD7] = (char) (0x1E6A); //LATIN CAPITAL LETTER T WITH DOT ABOVE
				chars[0xD8] = (char) (0x00D8); //LATIN CAPITAL LETTER O WITH STROKE
				chars[0xD9] = (char) (0x00D9); //LATIN CAPITAL LETTER U WITH GRAVE
				chars[0xDA] = (char) (0x00DA); //LATIN CAPITAL LETTER U WITH ACUTE
				chars[0xDB] = (char) (0x00DB); //LATIN CAPITAL LETTER U WITH CIRCUMFLEX
				chars[0xDC] = (char) (0x00DC); //LATIN CAPITAL LETTER U WITH DIAERESIS
				chars[0xDD] = (char) (0x00DD); //LATIN CAPITAL LETTER Y WITH ACUTE
				chars[0xDE] = (char) (0x0176); //LATIN CAPITAL LETTER Y WITH CIRCUMFLEX
				chars[0xDF] = (char) (0x00DF); //LATIN SMALL LETTER SHARP S
				chars[0xE0] = (char) (0x00E0); //LATIN SMALL LETTER A WITH GRAVE
				chars[0xE1] = (char) (0x00E1); //LATIN SMALL LETTER A WITH ACUTE
				chars[0xE2] = (char) (0x00E2); //LATIN SMALL LETTER A WITH CIRCUMFLEX
				chars[0xE3] = (char) (0x00E3); //LATIN SMALL LETTER A WITH TILDE
				chars[0xE4] = (char) (0x00E4); //LATIN SMALL LETTER A WITH DIAERESIS
				chars[0xE5] = (char) (0x00E5); //LATIN SMALL LETTER A WITH RING ABOVE
				chars[0xE6] = (char) (0x00E6); //LATIN SMALL LETTER AE
				chars[0xE7] = (char) (0x00E7); //LATIN SMALL LETTER C WITH CEDILLA
				chars[0xE8] = (char) (0x00E8); //LATIN SMALL LETTER E WITH GRAVE
				chars[0xE9] = (char) (0x00E9); //LATIN SMALL LETTER E WITH ACUTE
				chars[0xEA] = (char) (0x00EA); //LATIN SMALL LETTER E WITH CIRCUMFLEX
				chars[0xEB] = (char) (0x00EB); //LATIN SMALL LETTER E WITH DIAERESIS
				chars[0xEC] = (char) (0x00EC); //LATIN SMALL LETTER I WITH GRAVE
				chars[0xED] = (char) (0x00ED); //LATIN SMALL LETTER I WITH ACUTE
				chars[0xEE] = (char) (0x00EE); //LATIN SMALL LETTER I WITH CIRCUMFLEX
				chars[0xEF] = (char) (0x00EF); //LATIN SMALL LETTER I WITH DIAERESIS
				chars[0xF0] = (char) (0x0175); //LATIN SMALL LETTER W WITH CIRCUMFLEX
				chars[0xF1] = (char) (0x00F1); //LATIN SMALL LETTER N WITH TILDE
				chars[0xF2] = (char) (0x00F2); //LATIN SMALL LETTER O WITH GRAVE
				chars[0xF3] = (char) (0x00F3); //LATIN SMALL LETTER O WITH ACUTE
				chars[0xF4] = (char) (0x00F4); //LATIN SMALL LETTER O WITH CIRCUMFLEX
				chars[0xF5] = (char) (0x00F5); //LATIN SMALL LETTER O WITH TILDE
				chars[0xF6] = (char) (0x00F6); //LATIN SMALL LETTER O WITH DIAERESIS
				chars[0xF7] = (char) (0x1E6B); //LATIN SMALL LETTER T WITH DOT ABOVE
				chars[0xF8] = (char) (0x00F8); //LATIN SMALL LETTER O WITH STROKE
				chars[0xF9] = (char) (0x00F9); //LATIN SMALL LETTER U WITH GRAVE
				chars[0xFA] = (char) (0x00FA); //LATIN SMALL LETTER U WITH ACUTE
				chars[0xFB] = (char) (0x00FB); //LATIN SMALL LETTER U WITH CIRCUMFLEX
				chars[0xFC] = (char) (0x00FC); //LATIN SMALL LETTER U WITH DIAERESIS
				chars[0xFD] = (char) (0x00FD); //LATIN SMALL LETTER Y WITH ACUTE
				chars[0xFE] = (char) (0x0177); //LATIN SMALL LETTER Y WITH CIRCUMFLEX
				chars[0xFF] = (char) (0x00FF); //LATIN SMALL LETTER Y WITH DIAERESIS
			}
		}
	}
}