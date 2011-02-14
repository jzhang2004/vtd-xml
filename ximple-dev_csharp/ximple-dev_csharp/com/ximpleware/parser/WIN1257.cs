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
	
	/// <summary> this class contains method to map a windows-1257 char
	/// into a Unicode char
	/// 
	/// </summary>
	public class WIN1257
	{
		//UPGRADE_NOTE: Final was removed from the declaration of 'chars '. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1003'"
		internal static readonly char[] chars = new char[0x100];
		public static char decode(byte b)
		{
			return chars[b];
		}
		static WIN1257()
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
				chars[0x80] = (char) (0x20AC); // EURO SIGN
				chars[0x82] = (char) (0x201A); // SINGLE LOW-9 QUOTATION MARK
				chars[0x84] = (char) (0x201E); // DOUBLE LOW-9 QUOTATION MARK
				chars[0x85] = (char) (0x2026); // HORIZONTAL ELLIPSIS
				chars[0x86] = (char) (0x2020); // DAGGER
				chars[0x87] = (char) (0x2021); // DOUBLE DAGGER
				chars[0x89] = (char) (0x2030); // PER MILLE SIGN
				chars[0x8B] = (char) (0x2039); // SINGLE LEFT-POINTING ANGLE QUOTATION MARK
				chars[0x8D] = (char) (0x00A8); // DIAERESIS
				chars[0x8E] = (char) (0x02C7); // CARON
				chars[0x8F] = (char) (0x00B8); // CEDILLA
				chars[0x91] = (char) (0x2018); // LEFT SINGLE QUOTATION MARK
				chars[0x92] = (char) (0x2019); // RIGHT SINGLE QUOTATION MARK
				chars[0x93] = (char) (0x201C); // LEFT DOUBLE QUOTATION MARK
				chars[0x94] = (char) (0x201D); // RIGHT DOUBLE QUOTATION MARK
				chars[0x95] = (char) (0x2022); // BULLET
				chars[0x96] = (char) (0x2013); // EN DASH
				chars[0x97] = (char) (0x2014); // EM DASH
				chars[0x99] = (char) (0x2122); // TRADE MARK SIGN
				chars[0x9B] = (char) (0x203A); // SINGLE RIGHT-POINTING ANGLE QUOTATION MARK
				chars[0x9D] = (char) (0x00AF); // MACRON
				chars[0x9E] = (char) (0x02DB); // OGONEK
				chars[0xA0] = (char) (0x00A0); // NO-BREAK SPACE
				chars[0xA2] = (char) (0x00A2); // CENT SIGN
				chars[0xA3] = (char) (0x00A3); // POUND SIGN
				chars[0xA4] = (char) (0x00A4); // CURRENCY SIGN
				chars[0xA6] = (char) (0x00A6); // BROKEN BAR
				chars[0xA7] = (char) (0x00A7); // SECTION SIGN
				chars[0xA8] = (char) (0x00D8); // LATIN CAPITAL LETTER O WITH STROKE
				chars[0xA9] = (char) (0x00A9); // COPYRIGHT SIGN
				chars[0xAA] = (char) (0x0156); // LATIN CAPITAL LETTER R WITH CEDILLA
				chars[0xAB] = (char) (0x00AB); // LEFT-POINTING DOUBLE ANGLE QUOTATION MARK
				chars[0xAC] = (char) (0x00AC); // NOT SIGN
				chars[0xAD] = (char) (0x00AD); // SOFT HYPHEN
				chars[0xAE] = (char) (0x00AE); // REGISTERED SIGN
				chars[0xAF] = (char) (0x00C6); // LATIN CAPITAL LETTER AE
				chars[0xB0] = (char) (0x00B0); // DEGREE SIGN
				chars[0xB1] = (char) (0x00B1); // PLUS-MINUS SIGN
				chars[0xB2] = (char) (0x00B2); // SUPERSCRIPT TWO
				chars[0xB3] = (char) (0x00B3); // SUPERSCRIPT THREE
				chars[0xB4] = (char) (0x00B4); // ACUTE ACCENT
				chars[0xB5] = (char) (0x00B5); // MICRO SIGN
				chars[0xB6] = (char) (0x00B6); // PILCROW SIGN
				chars[0xB7] = (char) (0x00B7); // MIDDLE DOT
				chars[0xB8] = (char) (0x00F8); // LATIN SMALL LETTER O WITH STROKE
				chars[0xB9] = (char) (0x00B9); // SUPERSCRIPT ONE
				chars[0xBA] = (char) (0x0157); // LATIN SMALL LETTER R WITH CEDILLA
				chars[0xBB] = (char) (0x00BB); // RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK
				chars[0xBC] = (char) (0x00BC); // VULGAR FRACTION ONE QUARTER
				chars[0xBD] = (char) (0x00BD); // VULGAR FRACTION ONE HALF
				chars[0xBE] = (char) (0x00BE); // VULGAR FRACTION THREE QUARTERS
				chars[0xBF] = (char) (0x00E6); // LATIN SMALL LETTER AE
				chars[0xC0] = (char) (0x0104); // LATIN CAPITAL LETTER A WITH OGONEK
				chars[0xC1] = (char) (0x012E); // LATIN CAPITAL LETTER I WITH OGONEK
				chars[0xC2] = (char) (0x0100); // LATIN CAPITAL LETTER A WITH MACRON
				chars[0xC3] = (char) (0x0106); // LATIN CAPITAL LETTER C WITH ACUTE
				chars[0xC4] = (char) (0x00C4); // LATIN CAPITAL LETTER A WITH DIAERESIS
				chars[0xC5] = (char) (0x00C5); // LATIN CAPITAL LETTER A WITH RING ABOVE
				chars[0xC6] = (char) (0x0118); // LATIN CAPITAL LETTER E WITH OGONEK
				chars[0xC7] = (char) (0x0112); // LATIN CAPITAL LETTER E WITH MACRON
				chars[0xC8] = (char) (0x010C); // LATIN CAPITAL LETTER C WITH CARON
				chars[0xC9] = (char) (0x00C9); // LATIN CAPITAL LETTER E WITH ACUTE
				chars[0xCA] = (char) (0x0179); // LATIN CAPITAL LETTER Z WITH ACUTE
				chars[0xCB] = (char) (0x0116); // LATIN CAPITAL LETTER E WITH DOT ABOVE
				chars[0xCC] = (char) (0x0122); // LATIN CAPITAL LETTER G WITH CEDILLA
				chars[0xCD] = (char) (0x0136); // LATIN CAPITAL LETTER K WITH CEDILLA
				chars[0xCE] = (char) (0x012A); // LATIN CAPITAL LETTER I WITH MACRON
				chars[0xCF] = (char) (0x013B); // LATIN CAPITAL LETTER L WITH CEDILLA
				chars[0xD0] = (char) (0x0160); // LATIN CAPITAL LETTER S WITH CARON
				chars[0xD1] = (char) (0x0143); // LATIN CAPITAL LETTER N WITH ACUTE
				chars[0xD2] = (char) (0x0145); // LATIN CAPITAL LETTER N WITH CEDILLA
				chars[0xD3] = (char) (0x00D3); // LATIN CAPITAL LETTER O WITH ACUTE
				chars[0xD4] = (char) (0x014C); // LATIN CAPITAL LETTER O WITH MACRON
				chars[0xD5] = (char) (0x00D5); // LATIN CAPITAL LETTER O WITH TILDE
				chars[0xD6] = (char) (0x00D6); // LATIN CAPITAL LETTER O WITH DIAERESIS
				chars[0xD7] = (char) (0x00D7); // MULTIPLICATION SIGN
				chars[0xD8] = (char) (0x0172); // LATIN CAPITAL LETTER U WITH OGONEK
				chars[0xD9] = (char) (0x0141); // LATIN CAPITAL LETTER L WITH STROKE
				chars[0xDA] = (char) (0x015A); // LATIN CAPITAL LETTER S WITH ACUTE
				chars[0xDB] = (char) (0x016A); // LATIN CAPITAL LETTER U WITH MACRON
				chars[0xDC] = (char) (0x00DC); // LATIN CAPITAL LETTER U WITH DIAERESIS
				chars[0xDD] = (char) (0x017B); // LATIN CAPITAL LETTER Z WITH DOT ABOVE
				chars[0xDE] = (char) (0x017D); // LATIN CAPITAL LETTER Z WITH CARON
				chars[0xDF] = (char) (0x00DF); // LATIN SMALL LETTER SHARP S
				chars[0xE0] = (char) (0x0105); // LATIN SMALL LETTER A WITH OGONEK
				chars[0xE1] = (char) (0x012F); // LATIN SMALL LETTER I WITH OGONEK
				chars[0xE2] = (char) (0x0101); // LATIN SMALL LETTER A WITH MACRON
				chars[0xE3] = (char) (0x0107); // LATIN SMALL LETTER C WITH ACUTE
				chars[0xE4] = (char) (0x00E4); // LATIN SMALL LETTER A WITH DIAERESIS
				chars[0xE5] = (char) (0x00E5); // LATIN SMALL LETTER A WITH RING ABOVE
				chars[0xE6] = (char) (0x0119); // LATIN SMALL LETTER E WITH OGONEK
				chars[0xE7] = (char) (0x0113); // LATIN SMALL LETTER E WITH MACRON
				chars[0xE8] = (char) (0x010D); // LATIN SMALL LETTER C WITH CARON
				chars[0xE9] = (char) (0x00E9); // LATIN SMALL LETTER E WITH ACUTE
				chars[0xEA] = (char) (0x017A); // LATIN SMALL LETTER Z WITH ACUTE
				chars[0xEB] = (char) (0x0117); // LATIN SMALL LETTER E WITH DOT ABOVE
				chars[0xEC] = (char) (0x0123); // LATIN SMALL LETTER G WITH CEDILLA
				chars[0xED] = (char) (0x0137); // LATIN SMALL LETTER K WITH CEDILLA
				chars[0xEE] = (char) (0x012B); // LATIN SMALL LETTER I WITH MACRON
				chars[0xEF] = (char) (0x013C); // LATIN SMALL LETTER L WITH CEDILLA
				chars[0xF0] = (char) (0x0161); // LATIN SMALL LETTER S WITH CARON
				chars[0xF1] = (char) (0x0144); // LATIN SMALL LETTER N WITH ACUTE
				chars[0xF2] = (char) (0x0146); // LATIN SMALL LETTER N WITH CEDILLA
				chars[0xF3] = (char) (0x00F3); // LATIN SMALL LETTER O WITH ACUTE
				chars[0xF4] = (char) (0x014D); // LATIN SMALL LETTER O WITH MACRON
				chars[0xF5] = (char) (0x00F5); // LATIN SMALL LETTER O WITH TILDE
				chars[0xF6] = (char) (0x00F6); // LATIN SMALL LETTER O WITH DIAERESIS
				chars[0xF7] = (char) (0x00F7); // DIVISION SIGN
				chars[0xF8] = (char) (0x0173); // LATIN SMALL LETTER U WITH OGONEK
				chars[0xF9] = (char) (0x0142); // LATIN SMALL LETTER L WITH STROKE
				chars[0xFA] = (char) (0x015B); // LATIN SMALL LETTER S WITH ACUTE
				chars[0xFB] = (char) (0x016B); // LATIN SMALL LETTER U WITH MACRON
				chars[0xFC] = (char) (0x00FC); // LATIN SMALL LETTER U WITH DIAERESIS
				chars[0xFD] = (char) (0x017C); // LATIN SMALL LETTER Z WITH DOT ABOVE
				chars[0xFE] = (char) (0x017E); // LATIN SMALL LETTER Z WITH CARON
				chars[0xFF] = (char) (0x02D9); // DOT ABOVE
			}
		}
	}
}