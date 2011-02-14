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
	
	/// <summary> This class contains method to map a windows-1251 char
	/// into a Unicode char
	/// 
	/// </summary>
	public class WIN1251
	{
		//UPGRADE_NOTE: Final was removed from the declaration of 'chars '. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1003'"
		internal static readonly char[] chars = new char[0x100];
		public static char decode(byte b)
		{
			return chars[b];
		}
		static WIN1251()
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
				chars[0x80] = (char) (0x0402); // CYRILLIC CAPITAL LETTER DJE
				chars[0x81] = (char) (0x0403); // CYRILLIC CAPITAL LETTER GJE
				chars[0x82] = (char) (0x201A); // SINGLE LOW-9 QUOTATION MARK
				chars[0x83] = (char) (0x0453); // CYRILLIC SMALL LETTER GJE
				chars[0x84] = (char) (0x201E); // DOUBLE LOW-9 QUOTATION MARK
				chars[0x85] = (char) (0x2026); // HORIZONTAL ELLIPSIS
				chars[0x86] = (char) (0x2020); // DAGGER
				chars[0x87] = (char) (0x2021); // DOUBLE DAGGER
				chars[0x88] = (char) (0x20AC); // EURO SIGN
				chars[0x89] = (char) (0x2030); // PER MILLE SIGN
				chars[0x8A] = (char) (0x0409); // CYRILLIC CAPITAL LETTER LJE
				chars[0x8B] = (char) (0x2039); // SINGLE LEFT-POINTING ANGLE QUOTATION MARK
				chars[0x8C] = (char) (0x040A); // CYRILLIC CAPITAL LETTER NJE
				chars[0x8D] = (char) (0x040C); // CYRILLIC CAPITAL LETTER KJE
				chars[0x8E] = (char) (0x040B); // CYRILLIC CAPITAL LETTER TSHE
				chars[0x8F] = (char) (0x040F); // CYRILLIC CAPITAL LETTER DZHE
				chars[0x90] = (char) (0x0452); // CYRILLIC SMALL LETTER DJE
				chars[0x91] = (char) (0x2018); // LEFT SINGLE QUOTATION MARK
				chars[0x92] = (char) (0x2019); // RIGHT SINGLE QUOTATION MARK
				chars[0x93] = (char) (0x201C); // LEFT DOUBLE QUOTATION MARK
				chars[0x94] = (char) (0x201D); // RIGHT DOUBLE QUOTATION MARK
				chars[0x95] = (char) (0x2022); // BULLET
				chars[0x96] = (char) (0x2013); // EN DASH
				chars[0x97] = (char) (0x2014); // EM DASH
				chars[0x99] = (char) (0x2122); // TRADE MARK SIGN
				chars[0x9A] = (char) (0x0459); // CYRILLIC SMALL LETTER LJE
				chars[0x9B] = (char) (0x203A); // SINGLE RIGHT-POINTING ANGLE QUOTATION MARK
				chars[0x9C] = (char) (0x045A); // CYRILLIC SMALL LETTER NJE
				chars[0x9D] = (char) (0x045C); // CYRILLIC SMALL LETTER KJE
				chars[0x9E] = (char) (0x045B); // CYRILLIC SMALL LETTER TSHE
				chars[0x9F] = (char) (0x045F); // CYRILLIC SMALL LETTER DZHE
				chars[0xA0] = (char) (0x00A0); // NO-BREAK SPACE
				chars[0xA1] = (char) (0x040E); // CYRILLIC CAPITAL LETTER SHORT U
				chars[0xA2] = (char) (0x045E); // CYRILLIC SMALL LETTER SHORT U
				chars[0xA3] = (char) (0x0408); // CYRILLIC CAPITAL LETTER JE
				chars[0xA4] = (char) (0x00A4); // CURRENCY SIGN
				chars[0xA5] = (char) (0x0490); // CYRILLIC CAPITAL LETTER GHE WITH UPTURN
				chars[0xA6] = (char) (0x00A6); // BROKEN BAR
				chars[0xA7] = (char) (0x00A7); // SECTION SIGN
				chars[0xA8] = (char) (0x0401); // CYRILLIC CAPITAL LETTER IO
				chars[0xA9] = (char) (0x00A9); // COPYRIGHT SIGN
				chars[0xAA] = (char) (0x0404); // CYRILLIC CAPITAL LETTER UKRAINIAN IE
				chars[0xAB] = (char) (0x00AB); // LEFT-POINTING DOUBLE ANGLE QUOTATION MARK
				chars[0xAC] = (char) (0x00AC); // NOT SIGN
				chars[0xAD] = (char) (0x00AD); // SOFT HYPHEN
				chars[0xAE] = (char) (0x00AE); // REGISTERED SIGN
				chars[0xAF] = (char) (0x0407); // CYRILLIC CAPITAL LETTER YI
				chars[0xB0] = (char) (0x00B0); // DEGREE SIGN
				chars[0xB1] = (char) (0x00B1); // PLUS-MINUS SIGN
				chars[0xB2] = (char) (0x0406); // CYRILLIC CAPITAL LETTER BYELORUSSIAN-UKRAINIAN I
				chars[0xB3] = (char) (0x0456); // CYRILLIC SMALL LETTER BYELORUSSIAN-UKRAINIAN I
				chars[0xB4] = (char) (0x0491); // CYRILLIC SMALL LETTER GHE WITH UPTURN
				chars[0xB5] = (char) (0x00B5); // MICRO SIGN
				chars[0xB6] = (char) (0x00B6); // PILCROW SIGN
				chars[0xB7] = (char) (0x00B7); // MIDDLE DOT
				chars[0xB8] = (char) (0x0451); // CYRILLIC SMALL LETTER IO
				chars[0xB9] = (char) (0x2116); // NUMERO SIGN
				chars[0xBA] = (char) (0x0454); // CYRILLIC SMALL LETTER UKRAINIAN IE
				chars[0xBB] = (char) (0x00BB); // RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK
				chars[0xBC] = (char) (0x0458); // CYRILLIC SMALL LETTER JE
				chars[0xBD] = (char) (0x0405); // CYRILLIC CAPITAL LETTER DZE
				chars[0xBE] = (char) (0x0455); // CYRILLIC SMALL LETTER DZE
				chars[0xBF] = (char) (0x0457); // CYRILLIC SMALL LETTER YI
				chars[0xC0] = (char) (0x0410); // CYRILLIC CAPITAL LETTER A
				chars[0xC1] = (char) (0x0411); // CYRILLIC CAPITAL LETTER BE
				chars[0xC2] = (char) (0x0412); // CYRILLIC CAPITAL LETTER VE
				chars[0xC3] = (char) (0x0413); // CYRILLIC CAPITAL LETTER GHE
				chars[0xC4] = (char) (0x0414); // CYRILLIC CAPITAL LETTER DE
				chars[0xC5] = (char) (0x0415); // CYRILLIC CAPITAL LETTER IE
				chars[0xC6] = (char) (0x0416); // CYRILLIC CAPITAL LETTER ZHE
				chars[0xC7] = (char) (0x0417); // CYRILLIC CAPITAL LETTER ZE
				chars[0xC8] = (char) (0x0418); // CYRILLIC CAPITAL LETTER I
				chars[0xC9] = (char) (0x0419); // CYRILLIC CAPITAL LETTER SHORT I
				chars[0xCA] = (char) (0x041A); // CYRILLIC CAPITAL LETTER KA
				chars[0xCB] = (char) (0x041B); // CYRILLIC CAPITAL LETTER EL
				chars[0xCC] = (char) (0x041C); // CYRILLIC CAPITAL LETTER EM
				chars[0xCD] = (char) (0x041D); // CYRILLIC CAPITAL LETTER EN
				chars[0xCE] = (char) (0x041E); // CYRILLIC CAPITAL LETTER O
				chars[0xCF] = (char) (0x041F); // CYRILLIC CAPITAL LETTER PE
				chars[0xD0] = (char) (0x0420); // CYRILLIC CAPITAL LETTER ER
				chars[0xD1] = (char) (0x0421); // CYRILLIC CAPITAL LETTER ES
				chars[0xD2] = (char) (0x0422); // CYRILLIC CAPITAL LETTER TE
				chars[0xD3] = (char) (0x0423); // CYRILLIC CAPITAL LETTER U
				chars[0xD4] = (char) (0x0424); // CYRILLIC CAPITAL LETTER EF
				chars[0xD5] = (char) (0x0425); // CYRILLIC CAPITAL LETTER HA
				chars[0xD6] = (char) (0x0426); // CYRILLIC CAPITAL LETTER TSE
				chars[0xD7] = (char) (0x0427); // CYRILLIC CAPITAL LETTER CHE
				chars[0xD8] = (char) (0x0428); // CYRILLIC CAPITAL LETTER SHA
				chars[0xD9] = (char) (0x0429); // CYRILLIC CAPITAL LETTER SHCHA
				chars[0xDA] = (char) (0x042A); // CYRILLIC CAPITAL LETTER HARD SIGN
				chars[0xDB] = (char) (0x042B); // CYRILLIC CAPITAL LETTER YERU
				chars[0xDC] = (char) (0x042C); // CYRILLIC CAPITAL LETTER SOFT SIGN
				chars[0xDD] = (char) (0x042D); // CYRILLIC CAPITAL LETTER E
				chars[0xDE] = (char) (0x042E); // CYRILLIC CAPITAL LETTER YU
				chars[0xDF] = (char) (0x042F); // CYRILLIC CAPITAL LETTER YA
				chars[0xE0] = (char) (0x0430); // CYRILLIC SMALL LETTER A
				chars[0xE1] = (char) (0x0431); // CYRILLIC SMALL LETTER BE
				chars[0xE2] = (char) (0x0432); // CYRILLIC SMALL LETTER VE
				chars[0xE3] = (char) (0x0433); // CYRILLIC SMALL LETTER GHE
				chars[0xE4] = (char) (0x0434); // CYRILLIC SMALL LETTER DE
				chars[0xE5] = (char) (0x0435); // CYRILLIC SMALL LETTER IE
				chars[0xE6] = (char) (0x0436); // CYRILLIC SMALL LETTER ZHE
				chars[0xE7] = (char) (0x0437); // CYRILLIC SMALL LETTER ZE
				chars[0xE8] = (char) (0x0438); // CYRILLIC SMALL LETTER I
				chars[0xE9] = (char) (0x0439); // CYRILLIC SMALL LETTER SHORT I
				chars[0xEA] = (char) (0x043A); // CYRILLIC SMALL LETTER KA
				chars[0xEB] = (char) (0x043B); // CYRILLIC SMALL LETTER EL
				chars[0xEC] = (char) (0x043C); // CYRILLIC SMALL LETTER EM
				chars[0xED] = (char) (0x043D); // CYRILLIC SMALL LETTER EN
				chars[0xEE] = (char) (0x043E); // CYRILLIC SMALL LETTER O
				chars[0xEF] = (char) (0x043F); // CYRILLIC SMALL LETTER PE
				chars[0xF0] = (char) (0x0440); // CYRILLIC SMALL LETTER ER
				chars[0xF1] = (char) (0x0441); // CYRILLIC SMALL LETTER ES
				chars[0xF2] = (char) (0x0442); // CYRILLIC SMALL LETTER TE
				chars[0xF3] = (char) (0x0443); // CYRILLIC SMALL LETTER U
				chars[0xF4] = (char) (0x0444); // CYRILLIC SMALL LETTER EF
				chars[0xF5] = (char) (0x0445); // CYRILLIC SMALL LETTER HA
				chars[0xF6] = (char) (0x0446); // CYRILLIC SMALL LETTER TSE
				chars[0xF7] = (char) (0x0447); // CYRILLIC SMALL LETTER CHE
				chars[0xF8] = (char) (0x0448); // CYRILLIC SMALL LETTER SHA
				chars[0xF9] = (char) (0x0449); // CYRILLIC SMALL LETTER SHCHA
				chars[0xFA] = (char) (0x044A); // CYRILLIC SMALL LETTER HARD SIGN
				chars[0xFB] = (char) (0x044B); // CYRILLIC SMALL LETTER YERU
				chars[0xFC] = (char) (0x044C); // CYRILLIC SMALL LETTER SOFT SIGN
				chars[0xFD] = (char) (0x044D); // CYRILLIC SMALL LETTER E
				chars[0xFE] = (char) (0x044E); // CYRILLIC SMALL LETTER YU
				chars[0xFF] = (char) (0x044F); // CYRILLIC SMALL LETTER YA
			}
		}
	}
}