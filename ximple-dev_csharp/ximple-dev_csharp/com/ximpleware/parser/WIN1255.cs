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
	
	/// <summary> this class contains method to map a windows-1255 char
	/// into a Unicode char
	/// 
	/// </summary>
	public class WIN1255
	{
		//UPGRADE_NOTE: Final was removed from the declaration of 'chars '. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1003'"
		internal static readonly char[] chars = new char[0x100];
		public static char decode(byte b)
		{
			return chars[b];
		}
		static WIN1255()
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
				chars[0x83] = (char) (0x0192); // LATIN SMALL LETTER F WITH HOOK
				chars[0x84] = (char) (0x201E); // DOUBLE LOW-9 QUOTATION MARK
				chars[0x85] = (char) (0x2026); // HORIZONTAL ELLIPSIS
				chars[0x86] = (char) (0x2020); // DAGGER
				chars[0x87] = (char) (0x2021); // DOUBLE DAGGER
				chars[0x88] = (char) (0x02C6); // MODIFIER LETTER CIRCUMFLEX ACCENT
				chars[0x89] = (char) (0x2030); // PER MILLE SIGN
				chars[0x8B] = (char) (0x2039); // SINGLE LEFT-POINTING ANGLE QUOTATION MARK
				chars[0x91] = (char) (0x2018); // LEFT SINGLE QUOTATION MARK
				chars[0x92] = (char) (0x2019); // RIGHT SINGLE QUOTATION MARK
				chars[0x93] = (char) (0x201C); // LEFT DOUBLE QUOTATION MARK
				chars[0x94] = (char) (0x201D); // RIGHT DOUBLE QUOTATION MARK
				chars[0x95] = (char) (0x2022); // BULLET
				chars[0x96] = (char) (0x2013); // EN DASH
				chars[0x97] = (char) (0x2014); // EM DASH
				chars[0x98] = (char) (0x02DC); // SMALL TILDE
				chars[0x99] = (char) (0x2122); // TRADE MARK SIGN
				chars[0x9B] = (char) (0x203A); // SINGLE RIGHT-POINTING ANGLE QUOTATION MARK
				chars[0xA0] = (char) (0x00A0); // NO-BREAK SPACE
				chars[0xA1] = (char) (0x00A1); // INVERTED EXCLAMATION MARK
				chars[0xA2] = (char) (0x00A2); // CENT SIGN
				chars[0xA3] = (char) (0x00A3); // POUND SIGN
				chars[0xA4] = (char) (0x20AA); // NEW SHEQEL SIGN
				chars[0xA5] = (char) (0x00A5); // YEN SIGN
				chars[0xA6] = (char) (0x00A6); // BROKEN BAR
				chars[0xA7] = (char) (0x00A7); // SECTION SIGN
				chars[0xA8] = (char) (0x00A8); // DIAERESIS
				chars[0xA9] = (char) (0x00A9); // COPYRIGHT SIGN
				chars[0xAA] = (char) (0x00D7); // MULTIPLICATION SIGN
				chars[0xAB] = (char) (0x00AB); // LEFT-POINTING DOUBLE ANGLE QUOTATION MARK
				chars[0xAC] = (char) (0x00AC); // NOT SIGN
				chars[0xAD] = (char) (0x00AD); // SOFT HYPHEN
				chars[0xAE] = (char) (0x00AE); // REGISTERED SIGN
				chars[0xAF] = (char) (0x00AF); // MACRON
				chars[0xB0] = (char) (0x00B0); // DEGREE SIGN
				chars[0xB1] = (char) (0x00B1); // PLUS-MINUS SIGN
				chars[0xB2] = (char) (0x00B2); // SUPERSCRIPT TWO
				chars[0xB3] = (char) (0x00B3); // SUPERSCRIPT THREE
				chars[0xB4] = (char) (0x00B4); // ACUTE ACCENT
				chars[0xB5] = (char) (0x00B5); // MICRO SIGN
				chars[0xB6] = (char) (0x00B6); // PILCROW SIGN
				chars[0xB7] = (char) (0x00B7); // MIDDLE DOT
				chars[0xB8] = (char) (0x00B8); // CEDILLA
				chars[0xB9] = (char) (0x00B9); // SUPERSCRIPT ONE
				chars[0xBA] = (char) (0x00F7); // DIVISION SIGN
				chars[0xBB] = (char) (0x00BB); // RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK
				chars[0xBC] = (char) (0x00BC); // VULGAR FRACTION ONE QUARTER
				chars[0xBD] = (char) (0x00BD); // VULGAR FRACTION ONE HALF
				chars[0xBE] = (char) (0x00BE); // VULGAR FRACTION THREE QUARTERS
				chars[0xBF] = (char) (0x00BF); // INVERTED QUESTION MARK
				chars[0xC0] = (char) (0x05B0); // HEBREW POINT SHEVA
				chars[0xC1] = (char) (0x05B1); // HEBREW POINT HATAF SEGOL
				chars[0xC2] = (char) (0x05B2); // HEBREW POINT HATAF PATAH
				chars[0xC3] = (char) (0x05B3); // HEBREW POINT HATAF QAMATS
				chars[0xC4] = (char) (0x05B4); // HEBREW POINT HIRIQ
				chars[0xC5] = (char) (0x05B5); // HEBREW POINT TSERE
				chars[0xC6] = (char) (0x05B6); // HEBREW POINT SEGOL
				chars[0xC7] = (char) (0x05B7); // HEBREW POINT PATAH
				chars[0xC8] = (char) (0x05B8); // HEBREW POINT QAMATS
				chars[0xC9] = (char) (0x05B9); // HEBREW POINT HOLAM
				chars[0xCB] = (char) (0x05BB); // HEBREW POINT QUBUTS
				chars[0xCC] = (char) (0x05BC); // HEBREW POINT DAGESH OR MAPIQ
				chars[0xCD] = (char) (0x05BD); // HEBREW POINT METEG
				chars[0xCE] = (char) (0x05BE); // HEBREW PUNCTUATION MAQAF
				chars[0xCF] = (char) (0x05BF); // HEBREW POINT RAFE
				chars[0xD0] = (char) (0x05C0); // HEBREW PUNCTUATION PASEQ
				chars[0xD1] = (char) (0x05C1); // HEBREW POINT SHIN DOT
				chars[0xD2] = (char) (0x05C2); // HEBREW POINT SIN DOT
				chars[0xD3] = (char) (0x05C3); // HEBREW PUNCTUATION SOF PASUQ
				chars[0xD4] = (char) (0x05F0); // HEBREW LIGATURE YIDDISH DOUBLE VAV
				chars[0xD5] = (char) (0x05F1); // HEBREW LIGATURE YIDDISH VAV YOD
				chars[0xD6] = (char) (0x05F2); // HEBREW LIGATURE YIDDISH DOUBLE YOD
				chars[0xD7] = (char) (0x05F3); // HEBREW PUNCTUATION GERESH
				chars[0xD8] = (char) (0x05F4); // HEBREW PUNCTUATION GERSHAYIM
				chars[0xE0] = (char) (0x05D0); // HEBREW LETTER ALEF
				chars[0xE1] = (char) (0x05D1); // HEBREW LETTER BET
				chars[0xE2] = (char) (0x05D2); // HEBREW LETTER GIMEL
				chars[0xE3] = (char) (0x05D3); // HEBREW LETTER DALET
				chars[0xE4] = (char) (0x05D4); // HEBREW LETTER HE
				chars[0xE5] = (char) (0x05D5); // HEBREW LETTER VAV
				chars[0xE6] = (char) (0x05D6); // HEBREW LETTER ZAYIN
				chars[0xE7] = (char) (0x05D7); // HEBREW LETTER HET
				chars[0xE8] = (char) (0x05D8); // HEBREW LETTER TET
				chars[0xE9] = (char) (0x05D9); // HEBREW LETTER YOD
				chars[0xEA] = (char) (0x05DA); // HEBREW LETTER FINAL KAF
				chars[0xEB] = (char) (0x05DB); // HEBREW LETTER KAF
				chars[0xEC] = (char) (0x05DC); // HEBREW LETTER LAMED
				chars[0xED] = (char) (0x05DD); // HEBREW LETTER FINAL MEM
				chars[0xEE] = (char) (0x05DE); // HEBREW LETTER MEM
				chars[0xEF] = (char) (0x05DF); // HEBREW LETTER FINAL NUN
				chars[0xF0] = (char) (0x05E0); // HEBREW LETTER NUN
				chars[0xF1] = (char) (0x05E1); // HEBREW LETTER SAMEKH
				chars[0xF2] = (char) (0x05E2); // HEBREW LETTER AYIN
				chars[0xF3] = (char) (0x05E3); // HEBREW LETTER FINAL PE
				chars[0xF4] = (char) (0x05E4); // HEBREW LETTER PE
				chars[0xF5] = (char) (0x05E5); // HEBREW LETTER FINAL TSADI
				chars[0xF6] = (char) (0x05E6); // HEBREW LETTER TSADI
				chars[0xF7] = (char) (0x05E7); // HEBREW LETTER QOF
				chars[0xF8] = (char) (0x05E8); // HEBREW LETTER RESH
				chars[0xF9] = (char) (0x05E9); // HEBREW LETTER SHIN
				chars[0xFA] = (char) (0x05EA); // HEBREW LETTER TAV
				chars[0xFD] = (char) (0x200E); // LEFT-TO-RIGHT MARK
				chars[0xFE] = (char) (0x200F); // RIGHT-TO-LEFT MARK
			}
		}
	}
}