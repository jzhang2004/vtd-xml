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
	
	/// <summary> this class contains method to map a ISO-8859-6 char
	/// into a Unicode char
	/// 
	/// </summary>
	public class ISO8859_6
	{
		//UPGRADE_NOTE: Final was removed from the declaration of 'chars '. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1003'"
		internal static readonly char[] chars = new char[0x100];
		
		public static char decode(byte b)
		{
			return chars[b];
		}
		static ISO8859_6()
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
				chars[0xA4] = (char) (0x00A4);
				chars[0xAC] = (char) (0x060C);
				chars[0xAD] = (char) (0x00AD);
				chars[0xBB] = (char) (0x061B);
				chars[0xBF] = (char) (0x061F);
				chars[0xC1] = (char) (0x0621);
				chars[0xC2] = (char) (0x0622);
				chars[0xC3] = (char) (0x0623);
				chars[0xC4] = (char) (0x0624);
				chars[0xC5] = (char) (0x0625);
				chars[0xC6] = (char) (0x0626);
				chars[0xC7] = (char) (0x0627);
				chars[0xC8] = (char) (0x0628);
				chars[0xC9] = (char) (0x0629);
				chars[0xCA] = (char) (0x062A);
				chars[0xCB] = (char) (0x062B);
				chars[0xCC] = (char) (0x062C);
				chars[0xCD] = (char) (0x062D);
				chars[0xCE] = (char) (0x062E);
				chars[0xCF] = (char) (0x062F);
				chars[0xD0] = (char) (0x0630);
				chars[0xD1] = (char) (0x0631);
				chars[0xD2] = (char) (0x0632);
				chars[0xD3] = (char) (0x0633);
				chars[0xD4] = (char) (0x0634);
				chars[0xD5] = (char) (0x0635);
				chars[0xD6] = (char) (0x0636);
				chars[0xD7] = (char) (0x0637);
				chars[0xD8] = (char) (0x0638);
				chars[0xD9] = (char) (0x0639);
				chars[0xDA] = (char) (0x063A);
				chars[0xE0] = (char) (0x0640);
				chars[0xE1] = (char) (0x0641);
				chars[0xE2] = (char) (0x0642);
				chars[0xE3] = (char) (0x0643);
				chars[0xE4] = (char) (0x0644);
				chars[0xE5] = (char) (0x0645);
				chars[0xE6] = (char) (0x0646);
				chars[0xE7] = (char) (0x0647);
				chars[0xE8] = (char) (0x0648);
				chars[0xE9] = (char) (0x0649);
				chars[0xEA] = (char) (0x064A);
				chars[0xEB] = (char) (0x064B);
				chars[0xEC] = (char) (0x064C);
				chars[0xED] = (char) (0x064D);
				chars[0xEE] = (char) (0x064E);
				chars[0xEF] = (char) (0x064F);
				chars[0xF0] = (char) (0x0650);
				chars[0xF1] = (char) (0x0651);
				chars[0xF2] = (char) (0x0652);
			}
		}
	}
}