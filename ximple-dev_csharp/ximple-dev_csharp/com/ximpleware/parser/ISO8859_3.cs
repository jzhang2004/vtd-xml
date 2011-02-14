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
	
	/// <summary> this class contains method to map a ISO-8859-3 char
	/// into a Unicode char
	/// 
	/// </summary>
	public class ISO8859_3
	{
		//UPGRADE_NOTE: Final was removed from the declaration of 'chars '. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1003'"
		internal static readonly char[] chars = new char[0x100];
		
		public static char decode(byte b)
		{
			return chars[b];
		}
		static ISO8859_3()
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
				chars[0xA0] = (char) (0x00A0); //	NO-BREAK SPACE
				chars[0xA1] = (char) (0x0126); //	LATIN CAPITAL LETTER H WITH STROKE
				chars[0xA2] = (char) (0x02D8); //	BREVE
				chars[0xA3] = (char) (0x00A3); //	POUND SIGN
				chars[0xA4] = (char) (0x00A4); //	CURRENCY SIGN
				chars[0xA6] = (char) (0x0124); //	LATIN CAPITAL LETTER H WITH CIRCUMFLEX
				chars[0xA7] = (char) (0x00A7); //	SECTION SIGN
				chars[0xA8] = (char) (0x00A8); //	DIAERESIS
				chars[0xA9] = (char) (0x0130); //	LATIN CAPITAL LETTER I WITH DOT ABOVE
				chars[0xAA] = (char) (0x015E); //	LATIN CAPITAL LETTER S WITH CEDILLA
				chars[0xAB] = (char) (0x011E); //	LATIN CAPITAL LETTER G WITH BREVE
				chars[0xAC] = (char) (0x0134); //	LATIN CAPITAL LETTER J WITH CIRCUMFLEX
				chars[0xAD] = (char) (0x00AD); //	SOFT HYPHEN
				chars[0xAF] = (char) (0x017B); //	LATIN CAPITAL LETTER Z WITH DOT ABOVE
				chars[0xB0] = (char) (0x00B0); //	DEGREE SIGN
				chars[0xB1] = (char) (0x0127); //	LATIN SMALL LETTER H WITH STROKE
				chars[0xB2] = (char) (0x00B2); //	SUPERSCRIPT TWO
				chars[0xB3] = (char) (0x00B3); //	SUPERSCRIPT THREE
				chars[0xB4] = (char) (0x00B4); //	ACUTE ACCENT
				chars[0xB5] = (char) (0x00B5); //	MICRO SIGN
				chars[0xB6] = (char) (0x0125); //	LATIN SMALL LETTER H WITH CIRCUMFLEX
				chars[0xB7] = (char) (0x00B7); //	MIDDLE DOT
				chars[0xB8] = (char) (0x00B8); //	CEDILLA
				chars[0xB9] = (char) (0x0131); //	LATIN SMALL LETTER DOTLESS I
				chars[0xBA] = (char) (0x015F); //	LATIN SMALL LETTER S WITH CEDILLA
				chars[0xBB] = (char) (0x011F); //	LATIN SMALL LETTER G WITH BREVE
				chars[0xBC] = (char) (0x0135); //	LATIN SMALL LETTER J WITH CIRCUMFLEX
				chars[0xBD] = (char) (0x00BD); //	VULGAR FRACTION ONE HALF
				chars[0xBF] = (char) (0x017C); //	LATIN SMALL LETTER Z WITH DOT ABOVE
				chars[0xC0] = (char) (0x00C0); //	LATIN CAPITAL LETTER A WITH GRAVE
				chars[0xC1] = (char) (0x00C1); //	LATIN CAPITAL LETTER A WITH ACUTE
				chars[0xC2] = (char) (0x00C2); //	LATIN CAPITAL LETTER A WITH CIRCUMFLEX
				chars[0xC4] = (char) (0x00C4); //	LATIN CAPITAL LETTER A WITH DIAERESIS
				chars[0xC5] = (char) (0x010A); //	LATIN CAPITAL LETTER C WITH DOT ABOVE
				chars[0xC6] = (char) (0x0108); //	LATIN CAPITAL LETTER C WITH CIRCUMFLEX
				chars[0xC7] = (char) (0x00C7); //	LATIN CAPITAL LETTER C WITH CEDILLA
				chars[0xC8] = (char) (0x00C8); //	LATIN CAPITAL LETTER E WITH GRAVE
				chars[0xC9] = (char) (0x00C9); //	LATIN CAPITAL LETTER E WITH ACUTE
				chars[0xCA] = (char) (0x00CA); //	LATIN CAPITAL LETTER E WITH CIRCUMFLEX
				chars[0xCB] = (char) (0x00CB); //	LATIN CAPITAL LETTER E WITH DIAERESIS
				chars[0xCC] = (char) (0x00CC); //	LATIN CAPITAL LETTER I WITH GRAVE
				chars[0xCD] = (char) (0x00CD); //	LATIN CAPITAL LETTER I WITH ACUTE
				chars[0xCE] = (char) (0x00CE); //	LATIN CAPITAL LETTER I WITH CIRCUMFLEX
				chars[0xCF] = (char) (0x00CF); //	LATIN CAPITAL LETTER I WITH DIAERESIS
				chars[0xD1] = (char) (0x00D1); //	LATIN CAPITAL LETTER N WITH TILDE
				chars[0xD2] = (char) (0x00D2); //	LATIN CAPITAL LETTER O WITH GRAVE
				chars[0xD3] = (char) (0x00D3); //	LATIN CAPITAL LETTER O WITH ACUTE
				chars[0xD4] = (char) (0x00D4); //	LATIN CAPITAL LETTER O WITH CIRCUMFLEX
				chars[0xD5] = (char) (0x0120); //	LATIN CAPITAL LETTER G WITH DOT ABOVE
				chars[0xD6] = (char) (0x00D6); //	LATIN CAPITAL LETTER O WITH DIAERESIS
				chars[0xD7] = (char) (0x00D7); //	MULTIPLICATION SIGN
				chars[0xD8] = (char) (0x011C); //	LATIN CAPITAL LETTER G WITH CIRCUMFLEX
				chars[0xD9] = (char) (0x00D9); //	LATIN CAPITAL LETTER U WITH GRAVE
				chars[0xDA] = (char) (0x00DA); //	LATIN CAPITAL LETTER U WITH ACUTE
				chars[0xDB] = (char) (0x00DB); //	LATIN CAPITAL LETTER U WITH CIRCUMFLEX
				chars[0xDC] = (char) (0x00DC); //	LATIN CAPITAL LETTER U WITH DIAERESIS
				chars[0xDD] = (char) (0x016C); //	LATIN CAPITAL LETTER U WITH BREVE
				chars[0xDE] = (char) (0x015C); //	LATIN CAPITAL LETTER S WITH CIRCUMFLEX
				chars[0xDF] = (char) (0x00DF); //	LATIN SMALL LETTER SHARP S
				chars[0xE0] = (char) (0x00E0); //	LATIN SMALL LETTER A WITH GRAVE
				chars[0xE1] = (char) (0x00E1); //	LATIN SMALL LETTER A WITH ACUTE
				chars[0xE2] = (char) (0x00E2); //	LATIN SMALL LETTER A WITH CIRCUMFLEX
				chars[0xE4] = (char) (0x00E4); //	LATIN SMALL LETTER A WITH DIAERESIS
				chars[0xE5] = (char) (0x010B); //	LATIN SMALL LETTER C WITH DOT ABOVE
				chars[0xE6] = (char) (0x0109); //	LATIN SMALL LETTER C WITH CIRCUMFLEX
				chars[0xE7] = (char) (0x00E7); //	LATIN SMALL LETTER C WITH CEDILLA
				chars[0xE8] = (char) (0x00E8); //	LATIN SMALL LETTER E WITH GRAVE
				chars[0xE9] = (char) (0x00E9); //	LATIN SMALL LETTER E WITH ACUTE
				chars[0xEA] = (char) (0x00EA); //	LATIN SMALL LETTER E WITH CIRCUMFLEX
				chars[0xEB] = (char) (0x00EB); //	LATIN SMALL LETTER E WITH DIAERESIS
				chars[0xEC] = (char) (0x00EC); //	LATIN SMALL LETTER I WITH GRAVE
				chars[0xED] = (char) (0x00ED); //	LATIN SMALL LETTER I WITH ACUTE
				chars[0xEE] = (char) (0x00EE); //	LATIN SMALL LETTER I WITH CIRCUMFLEX
				chars[0xEF] = (char) (0x00EF); //	LATIN SMALL LETTER I WITH DIAERESIS
				chars[0xF1] = (char) (0x00F1); //	LATIN SMALL LETTER N WITH TILDE
				chars[0xF2] = (char) (0x00F2); //	LATIN SMALL LETTER O WITH GRAVE
				chars[0xF3] = (char) (0x00F3); //	LATIN SMALL LETTER O WITH ACUTE
				chars[0xF4] = (char) (0x00F4); //	LATIN SMALL LETTER O WITH CIRCUMFLEX
				chars[0xF5] = (char) (0x0121); //	LATIN SMALL LETTER G WITH DOT ABOVE
				chars[0xF6] = (char) (0x00F6); //	LATIN SMALL LETTER O WITH DIAERESIS
				chars[0xF7] = (char) (0x00F7); //	DIVISION SIGN
				chars[0xF8] = (char) (0x011D); //	LATIN SMALL LETTER G WITH CIRCUMFLEX
				chars[0xF9] = (char) (0x00F9); //	LATIN SMALL LETTER U WITH GRAVE
				chars[0xFA] = (char) (0x00FA); //	LATIN SMALL LETTER U WITH ACUTE
				chars[0xFB] = (char) (0x00FB); //	LATIN SMALL LETTER U WITH CIRCUMFLEX
				chars[0xFC] = (char) (0x00FC); //	LATIN SMALL LETTER U WITH DIAERESIS
				chars[0xFD] = (char) (0x016D); //	LATIN SMALL LETTER U WITH BREVE
				chars[0xFE] = (char) (0x015D); //	LATIN SMALL LETTER S WITH CIRCUMFLEX
				chars[0xFF] = (char) (0x02D9); //
			}
		}
	}
}