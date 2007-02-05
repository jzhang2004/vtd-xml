/* 
* Copyright (C) 2002-2007 XimpleWare, info@ximpleware.com
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
	/// <summary> XimpleWare's XMLChar implementation.
	/// Creation date: (2/9/04 6:27:46 PM)
	/// </summary>
	public class XMLChar
	{
		/// <summary> XMLChar constructor comment.</summary>
		//
		// Constants
		//
		
		/// <summary>Character flags. </summary>
		//UPGRADE_NOTE: Final was removed from the declaration of 'UNI_CHARS '. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1003'"
		private static readonly sbyte[] UNI_CHARS = new sbyte[0x10000];
		
		/// <summary>Valid character mask. </summary>
		public const int XML_VALID = 0x01;
		
		/// <summary>Space character mask. </summary>
		public const int XML_SPACE = 0x02;
		
		/// <summary>Name start character mask. </summary>
		public const int XML_NAME_START = 0x04;
		
		/// <summary>Name character mask. </summary>
		public const int XML_NAME = 0x08;
		
		/// <summary>Pubid character mask. </summary>
		public const int XML_PUBID = 0x10;
		
		/// <summary> Content character mask. Special characters are those that can
		/// be considered the start of markup, such as '&lt;' and '&amp;'.
		/// The various newline characters are considered special as well.
		/// All other valid XML characters can be considered content.
		/// <p>
		/// This is an optimization for the inner loop of character scanning.
		/// </summary>
		public const int XML_CONTENT = 0x20;
		
		/// <summary>NCName start character mask. </summary>
		public const int XML_NCNAME_START = 0x40;
		
		/// <summary>NCName character mask. </summary>
		public const int XML_NCNAME = -0x80;
		
		/// <summary> Returns the high surrogate from a Unicode scalar value
		/// 
		/// </summary>
		/// <param name="c">The Unicode scalar value to "split".
		/// </param>
		public static char highSurrogate(int c)
		{
			//H = (S - 0x10000) / 0x400 + 0xD800
			return (char) ((c >> 10) + 0xd7c0);
		}
		/// <summary> Returns true if the specified character can be considered XML content.
		/// 
		/// </summary>
		/// <param name="c">The character to check.
		/// </param>
		public static bool isContentChar(int c)
		{
			return (c < 0x10000 && (UNI_CHARS[c] & XML_CONTENT) != 0) || (0x10000 <= c && c <= 0x10FFFF);
		}
		/// <summary> Returns whether the given Unicode scalar value is a high surrogate
		/// 
		/// </summary>
		/// <param name="c">The character to check.
		/// </param>
		public static bool isHighSurrogate(int c)
		{
			return (0xD800 <= c && c <= 0xDBFF);
		}
		/// <summary> Returns true if the specified character is invalid.
		/// 
		/// </summary>
		/// <param name="c">The character to check.
		/// </param>
		public static bool isInvalidChar(int c)
		{
			return ((c < 0x10000) && (UNI_CHARS[c] & XML_VALID) == 0) || (c >= 0x10ffff);
		}
		/// <summary> Returns whether the given Unicode scalar value is a low surrogate
		/// 
		/// </summary>
		/// <param name="c">The character to check.
		/// </param>
		public static bool isLowSurrogate(int c)
		{
			return (0xDC00 <= c && c <= 0xDFFF);
		}
		/// <summary> Returns true if the specified character can be considered markup.
		/// Markup characters include '&lt;', '&amp;', and '%'.
		/// 
		/// </summary>
		/// <param name="c">The character to check.
		/// </param>
		public static bool isMarkupChar(int c)
		{
			return c == '<' || c == '&' || c == '%';
		}
		/// <summary> Returns true if the specified character is a valid name
		/// character as defined by production [4] in the XML 1.0
		/// specification.
		/// 
		/// </summary>
		/// <param name="c">The character to check.
		/// </param>
		public static bool isNameChar(int c)
		{
			return c < 0x10000 && (UNI_CHARS[c] & XML_NAME) != 0;
		}
		/// <summary> Returns true if the specified character is a valid name start
		/// character as defined by production [5] in the XML 1.0
		/// specification.
		/// 
		/// </summary>
		/// <param name="c">The character to check.
		/// </param>
		public static bool isNameStartChar(int c)
		{
			return c < 0x10000 && (UNI_CHARS[c] & XML_NAME_START) != 0;
		}
		/// <summary> Returns true if the specified character is a valid NCName
		/// character as defined by production [5] in Namespaces in XML
		/// recommendation.
		/// 
		/// </summary>
		/// <param name="c">The character to check.
		/// </param>
		public static bool isNCNameChar(int c)
		{
			return c < 0x10000 && (UNI_CHARS[c] & XML_NCNAME) != 0;
		}
		/// <summary> Returns true if the specified character is a valid NCName start
		/// character as defined by production [4] in Namespaces in XML
		/// recommendation.
		/// 
		/// </summary>
		/// <param name="c">The character to check.
		/// </param>
		public static bool isNCNameStartChar(int c)
		{
			return c < 0x10000 && (UNI_CHARS[c] & XML_NCNAME_START) != 0;
		}
		/*
		* [7] Nmtoken ::= (NameChar)+
		*
		* Check to see if a string is a valid Nmtoken according to [7]
		* in the XML 1.0 Recommendation
		*
		* @param nmToken string to check
		* @return true if nmtoken is a valid Nmtoken
		*/
		public static bool isNmtoken(System.String nmToken)
		{
			if (nmToken.Length == 0)
				return false;
			
			for (int i = 0; i < nmToken.Length; i++)
				if (!isNameChar(nmToken[i]))
					return false;
			
			return true;
		}
		/// <summary> Returns true if the specified character is a valid Pubid
		/// character as defined by production [13] in the XML 1.0
		/// specification.
		/// 
		/// </summary>
		/// <param name="c">The character to check.
		/// </param>
		public static bool isPubidChar(int c)
		{
			return c < 0x10000 && (UNI_CHARS[c] & XML_PUBID) != 0;
		}
		/// <summary> Returns true if the specified character is a space character
		/// as defined by production [3] in the XML 1.0 specification.
		/// 
		/// </summary>
		/// <param name="c">The character to check.
		/// </param>
		public static bool isSpaceChar(int c)
		{
			return c < 0x10000 && (UNI_CHARS[c] & XML_SPACE) != 0;
		}
		/// <summary> Returns true if the specified character is a supplemental character.
		/// 
		/// </summary>
		/// <param name="c">The character to check.
		/// </param>
		public static bool isSupplementalChar(int c)
		{
			return (c >= 0x10000 && c <= 0x10FFFF);
		}
		/// <summary> Returns true if the specified character is valid. This method
		/// also checks the surrogate character range from 0x10000 to 0x10FFFF.
		/// <p>
		/// If the program chooses to apply the mask directly to the
		/// <code>UNI_CHARS</code> array, then they are responsible for checking
		/// the surrogate character range.
		/// 
		/// </summary>
		/// <param name="c">The character to check.
		/// </param>
		public static bool isValidChar(int c)
		{
			return (c < 0x10000 && (UNI_CHARS[c] & XML_VALID) != 0) || (0x10000 <= c && c <= 0x10FFFF);
		}
		/// <summary> Returns the low surrogate from a Unicode scalar value
		/// 
		/// </summary>
		/// <param name="c">The Unicode scalar value to "split".
		/// </param>
		public static char lowSurrogate(int c)
		{
			//L = (S - 0x10000) % 0x400 + 0xDC00
			return (char) (((c - 0x00010000) & 0x3FF) + 0xDC00);
		}
		/// <summary> Returns Unicode scalar value corresponding to the given
		/// surrogates.
		/// 
		/// </summary>
		/// <param name="hi">The high surrogate.
		/// </param>
		/// <param name="lo">The low surrogate.
		/// </param>
		public static int scalarValueSurrogate(char hi, char lo)
		{
			//N = (H - 0xD800) * 0x400 + (L - 0xDC00) + 0x10000
			return ((hi - 0xd800) << 10) + lo - 0x2400;
		}
		static XMLChar()
		{
			//
			// Static initialization
			//
			
			{
				
				//
				// [2] Char ::= #x9 | #xA | #xD | [#x20-#xD7FF] |
				//              [#xE000-#xFFFD] | [#x10000-#x10FFFF]
				//
				
				int[][] xmlCharRange = new int[][]{new int[]{0x0009, 0x000A}, new int[]{0x000D, 0x000D}, new int[]{0x0020, 0xD7FF}, new int[]{0xE000, 0xFFFD}};
				
				//
				// [3] S ::= (#x20 | #x9 | #xD | #xA)+
				//
				
				int[] xmlSpaceChar = new int[]{0x0020, 0x0009, 0x000D, 0x000A};
				
				//
				// [4] NameChar ::= Letter | Digit | '.' | '-' | '_' | ':' |
				//                  CombiningChar | Extender
				//
				
				int[] xmlNameChar = new int[]{0x002D, 0x002E};
				
				//
				// [5] Name ::= (Letter | '_' | ':') (NameChar)*
				//
				
				int[] xmlNameStartChar = new int[]{0x003A, 0x005F};
				
				//
				// [13] PubidChar ::= #x20 | 0xD | 0xA | [a-zA-Z0-9] | [-'()+,./:=?;!*#@$_%]
				//
				
				int[] xmlPubidChar = new int[]{0x000A, 0x000D, 0x0020, 0x0021, 0x0023, 0x0024, 0x0025, 0x003D, 0x005F};
				
				int[][] xmlPubidRange = new int[][]{new int[]{0x0027, 0x003B}, new int[]{0x003F, 0x005A}, new int[]{0x0061, 0x007A}};
				
				//
				// [84] Letter ::= BaseChar | Ideographic
				//
				
				int[][] xmlLetterRange = new int[][]{new int[]{0x0041, 0x005A}, new int[]{0x0061, 0x007A}, new int[]{0x00C0, 0x00D6}, new int[]{0x00D8, 0x00F6}, new int[]{0x00F8, 0x0131}, new int[]{0x0134, 0x013E}, new int[]{0x0141, 0x0148}, new int[]{0x014A, 0x017E}, new int[]{0x0180, 0x01C3}, new int[]{0x01CD, 0x01F0}, new int[]{0x01F4, 0x01F5}, new int[]{0x01FA, 0x0217}, new int[]{0x0250, 0x02A8}, new int[]{0x02BB, 0x02C1}, new int[]{0x0388, 0x038A}, new int[]{0x038E, 0x03A1}, new int[]{0x03A3, 0x03CE}, new int[]{0x03D0, 0x03D6}, new int[]{0x03E2, 0x03F3}, new int[]{0x0401, 0x040C}, new int[]{0x040E, 0x044F}, new int[]{0x0451, 0x045C}, new int[]{0x045E, 0x0481}, new int[]{0x0490, 0x04C4}, new int[]{0x04C7, 0x04C8}, new int[]{0x04CB, 0x04CC}, new int[]{0x04D0, 0x04EB}, new int[]{0x04EE, 0x04F5}, new int[]{0x04F8, 0x04F9}, new int[]{0x0531, 0x0556}, new int[]{0x0561, 0x0586}, new int[]{0x05D0, 0x05EA}, new int[]{0x05F0, 0x05F2}, new int[]{0x0621, 0x063A}, new int[]{0x0641, 0x064A}, new int[]{0x0671, 0x06B7}, new int[]{0x06BA, 0x06BE}, new int[]{0x06C0, 0x06CE}, new int[]{0x06D0, 0x06D3}, new int[]{0x06E5, 0x06E6}, new int[]{0x0905, 0x0939}, new int[]{0x0958, 0x0961}, new int[]{0x0985, 0x098C}, new int[]{0x098F, 0x0990}, new int[]{0x0993, 0x09A8}, new int[]{0x09AA, 0x09B0}, new int[]{0x09B6, 0x09B9}, new int[]{0x09DC, 0x09DD}, new int[]{0x09DF, 0x09E1}, new int[]{0x09F0, 0x09F1}, new int[]{0x0A05, 0x0A0A}, new int[]{0x0A0F, 0x0A10}, new int[]{0x0A13, 0x0A28}, new int[]{0x0A2A, 0x0A30}, new int[]{0x0A32, 0x0A33}, new int[]{0x0A35, 0x0A36}, new int[]{0x0A38, 0x0A39}, new int[]{0x0A59, 0x0A5C}, new int[]{0x0A72, 0x0A74}, new int[]{0x0A85, 0x0A8B}, new int[]{0x0A8F, 0x0A91}, new int[]{0x0A93, 0x0AA8}, new int[]{0x0AAA, 0x0AB0}, new int[]{0x0AB2, 0x0AB3}, new int[]{0x0AB5, 0x0AB9}, new int[]{0x0B05, 0x0B0C}, new int[]{0x0B0F, 0x0B10}, new int[]{0x0B13, 0x0B28}, new int[]{0x0B2A, 0x0B30}, new int[]{0x0B32, 0x0B33}, new int[]{0x0B36, 0x0B39}, new int[]{0x0B5C, 0x0B5D}, new int[]{0x0B5F, 0x0B61}, new int[]{0x0B85, 
					0x0B8A}, new int[]{0x0B8E, 0x0B90}, new int[]{0x0B92, 0x0B95}, new int[]{0x0B99, 0x0B9A}, new int[]{0x0B9E, 0x0B9F}, new int[]{0x0BA3, 0x0BA4}, new int[]{0x0BA8, 0x0BAA}, new int[]{0x0BAE, 0x0BB5}, new int[]{0x0BB7, 0x0BB9}, new int[]{0x0C05, 0x0C0C}, new int[]{0x0C0E, 0x0C10}, new int[]{0x0C12, 0x0C28}, new int[]{0x0C2A, 0x0C33}, new int[]{0x0C35, 0x0C39}, new int[]{0x0C60, 0x0C61}, new int[]{0x0C85, 0x0C8C}, new int[]{0x0C8E, 0x0C90}, new int[]{0x0C92, 0x0CA8}, new int[]{0x0CAA, 0x0CB3}, new int[]{0x0CB5, 0x0CB9}, new int[]{0x0CE0, 0x0CE1}, new int[]{0x0D05, 0x0D0C}, new int[]{0x0D0E, 0x0D10}, new int[]{0x0D12, 0x0D28}, new int[]{0x0D2A, 0x0D39}, new int[]{0x0D60, 0x0D61}, new int[]{0x0E01, 0x0E2E}, new int[]{0x0E32, 0x0E33}, new int[]{0x0E40, 0x0E45}, new int[]{0x0E81, 0x0E82}, new int[]{0x0E87, 0x0E88}, new int[]{0x0E94, 0x0E97}, new int[]{0x0E99, 0x0E9F}, new int[]{0x0EA1, 0x0EA3}, new int[]{0x0EAA, 0x0EAB}, new int[]{0x0EAD, 0x0EAE}, new int[]{0x0EB2, 0x0EB3}, new int[]{0x0EC0, 0x0EC4}, new int[]{0x0F40, 0x0F47}, new int[]{0x0F49, 0x0F69}, new int[]{0x10A0, 0x10C5}, new int[]{0x10D0, 0x10F6}, new int[]{0x1102, 0x1103}, new int[]{0x1105, 0x1107}, new int[]{0x110B, 0x110C}, new int[]{0x110E, 0x1112}, new int[]{0x1154, 0x1155}, new int[]{0x115F, 0x1161}, new int[]{0x116D, 0x116E}, new int[]{0x1172, 0x1173}, new int[]{0x11AE, 0x11AF}, new int[]{0x11B7, 0x11B8}, new int[]{0x11BC, 0x11C2}, new int[]{0x1E00, 0x1E9B}, new int[]{0x1EA0, 0x1EF9}, new int[]{0x1F00, 0x1F15}, new int[]{0x1F18, 0x1F1D}, new int[]{0x1F20, 0x1F45}, new int[]{0x1F48, 0x1F4D}, new int[]{0x1F50, 0x1F57}, new int[]{0x1F5F, 0x1F7D}, new int[]{0x1F80, 0x1FB4}, new int[]{0x1FB6, 0x1FBC}, new int[]{0x1FC2, 0x1FC4}, new int[]{0x1FC6, 0x1FCC}, new int[]{0x1FD0, 0x1FD3}, new int[]{0x1FD6, 0x1FDB}, new int[]{0x1FE0, 0x1FEC}, new int[]{0x1FF2, 0x1FF4}, new int[]{0x1FF6, 0x1FFC}, new int[]{0x212A, 0x212B}, new int[]{0x2180, 0x2182}, new int[]{0x3041, 0x3094}, new int[]{0x30A1, 0x30FA}, new int[]{0x3105, 0x312C}, new int[]{
					0xAC00, 0xD7A3}, new int[]{0x3021, 0x3029}, new int[]{0x4E00, 0x9FA5}};
				int[] xmlLetterChar = new int[]{0x0386, 0x038C, 0x03DA, 0x03DC, 0x03DE, 0x03E0, 0x0559, 0x06D5, 0x093D, 0x09B2, 0x0A5E, 0x0A8D, 0x0ABD, 0x0AE0, 0x0B3D, 0x0B9C, 0x0CDE, 0x0E30, 0x0E84, 0x0E8A, 0x0E8D, 0x0EA5, 0x0EA7, 0x0EB0, 0x0EBD, 0x1100, 0x1109, 0x113C, 0x113E, 0x1140, 0x114C, 0x114E, 0x1150, 0x1159, 0x1163, 0x1165, 0x1167, 0x1169, 0x1175, 0x119E, 0x11A8, 0x11AB, 0x11BA, 0x11EB, 0x11F0, 0x11F9, 0x1F59, 0x1F5B, 0x1F5D, 0x1FBE, 0x2126, 0x212E, 0x3007};
				
				//
				// [87] CombiningChar ::= ...
				//
				
				int[][] xmlCombiningCharRange = new int[][]{new int[]{0x0300, 0x0345}, new int[]{0x0360, 0x0361}, new int[]{0x0483, 0x0486}, new int[]{0x0591, 0x05A1}, new int[]{0x05A3, 0x05B9}, new int[]{0x05BB, 0x05BD}, new int[]{0x05C1, 0x05C2}, new int[]{0x064B, 0x0652}, new int[]{0x06D6, 0x06DC}, new int[]{0x06DD, 0x06DF}, new int[]{0x06E0, 0x06E4}, new int[]{0x06E7, 0x06E8}, new int[]{0x06EA, 0x06ED}, new int[]{0x0901, 0x0903}, new int[]{0x093E, 0x094C}, new int[]{0x0951, 0x0954}, new int[]{0x0962, 0x0963}, new int[]{0x0981, 0x0983}, new int[]{0x09C0, 0x09C4}, new int[]{0x09C7, 0x09C8}, new int[]{0x09CB, 0x09CD}, new int[]{0x09E2, 0x09E3}, new int[]{0x0A40, 0x0A42}, new int[]{0x0A47, 0x0A48}, new int[]{0x0A4B, 0x0A4D}, new int[]{0x0A70, 0x0A71}, new int[]{0x0A81, 0x0A83}, new int[]{0x0ABE, 0x0AC5}, new int[]{0x0AC7, 0x0AC9}, new int[]{0x0ACB, 0x0ACD}, new int[]{0x0B01, 0x0B03}, new int[]{0x0B3E, 0x0B43}, new int[]{0x0B47, 0x0B48}, new int[]{0x0B4B, 0x0B4D}, new int[]{0x0B56, 0x0B57}, new int[]{0x0B82, 0x0B83}, new int[]{0x0BBE, 0x0BC2}, new int[]{0x0BC6, 0x0BC8}, new int[]{0x0BCA, 0x0BCD}, new int[]{0x0C01, 0x0C03}, new int[]{0x0C3E, 0x0C44}, new int[]{0x0C46, 0x0C48}, new int[]{0x0C4A, 0x0C4D}, new int[]{0x0C55, 0x0C56}, new int[]{0x0C82, 0x0C83}, new int[]{0x0CBE, 0x0CC4}, new int[]{0x0CC6, 0x0CC8}, new int[]{0x0CCA, 0x0CCD}, new int[]{0x0CD5, 0x0CD6}, new int[]{0x0D02, 0x0D03}, new int[]{0x0D3E, 0x0D43}, new int[]{0x0D46, 0x0D48}, new int[]{0x0D4A, 0x0D4D}, new int[]{0x0E34, 0x0E3A}, new int[]{0x0E47, 0x0E4E}, new int[]{0x0EB4, 0x0EB9}, new int[]{0x0EBB, 0x0EBC}, new int[]{0x0EC8, 0x0ECD}, new int[]{0x0F18, 0x0F19}, new int[]{0x0F71, 0x0F84}, new int[]{0x0F86, 0x0F8B}, new int[]{0x0F90, 0x0F95}, new int[]{0x0F99, 0x0FAD}, new int[]{0x0FB1, 0x0FB7}, new int[]{0x20D0, 0x20DC}, new int[]{0x302A, 0x302F}};
				
				int[] xmlCombiningCharChar = new int[]{0x05BF, 0x05C4, 0x0670, 0x093C, 0x094D, 0x09BC, 0x09BE, 0x09BF, 0x09D7, 0x0A02, 0x0A3C, 0x0A3E, 0x0A3F, 0x0ABC, 0x0B3C, 0x0BD7, 0x0D57, 0x0E31, 0x0EB1, 0x0F35, 0x0F37, 0x0F39, 0x0F3E, 0x0F3F, 0x0F97, 0x0FB9, 0x20E1, 0x3099, 0x309A};
				
				//
				// [88] Digit ::= ...
				//
				
				int[][] xmlDigitRange = new int[][]{new int[]{0x0030, 0x0039}, new int[]{0x0660, 0x0669}, new int[]{0x06F0, 0x06F9}, new int[]{0x0966, 0x096F}, new int[]{0x09E6, 0x09EF}, new int[]{0x0A66, 0x0A6F}, new int[]{0x0AE6, 0x0AEF}, new int[]{0x0B66, 0x0B6F}, new int[]{0x0BE7, 0x0BEF}, new int[]{0x0C66, 0x0C6F}, new int[]{0x0CE6, 0x0CEF}, new int[]{0x0D66, 0x0D6F}, new int[]{0x0E50, 0x0E59}, new int[]{0x0ED0, 0x0ED9}, new int[]{0x0F20, 0x0F29}};
				
				//
				// [89] Extender ::= ...
				//
				
				int[][] xmlExtenderRange = new int[][]{new int[]{0x3031, 0x3035}, new int[]{0x309D, 0x309E}, new int[]{0x30FC, 0x30FE}};
				
				int[] xmlExtenderChar = new int[]{0x00B7, 0x02D0, 0x02D1, 0x0387, 0x0640, 0x0E46, 0x0EC6, 0x3005};
				
				//
				// SpecialChar ::= '<', '&', '\n', '\r', ']'
				//
				
				//int specialChar[] = {
				//    '<', '&', '\n', '\r', ']',
				//};
				
				int[] xmlSpecialChar = new int[]{'<', '&', ']'};
				
				//
				// Static Initialization of all chars and their masks
				//
				
				// set valid characters
				for (int i = 0; i < xmlCharRange.Length; i++)
				{
					for (int j = xmlCharRange[i][0]; j <= xmlCharRange[i][1]; j++)
					{
						UNI_CHARS[j] |= XML_VALID | XML_CONTENT;
					}
				}
				
				// remove special characters
				for (int i = 0; i < xmlSpecialChar.Length; i++)
				{
					UNI_CHARS[xmlSpecialChar[i]] = (sbyte) (UNI_CHARS[xmlSpecialChar[i]] & ~ XML_CONTENT);
				}
				
				// set space characters
				for (int i = 0; i < xmlSpaceChar.Length; i++)
				{
					UNI_CHARS[xmlSpaceChar[i]] |= (sbyte) (XML_SPACE);
				}
				
				// set name start characters
				for (int i = 0; i < xmlNameStartChar.Length; i++)
				{
					UNI_CHARS[xmlNameStartChar[i]] |= XML_NAME_START | XML_NAME | XML_NCNAME_START | XML_NCNAME;
				}
				for (int i = 0; i < xmlLetterRange.Length; i++)
				{
					for (int j = xmlLetterRange[i][0]; j <= xmlLetterRange[i][1]; j++)
					{
						UNI_CHARS[j] |= XML_NAME_START | XML_NAME | XML_NCNAME_START | XML_NCNAME;
					}
				}
				for (int i = 0; i < xmlLetterChar.Length; i++)
				{
					UNI_CHARS[xmlLetterChar[i]] |= XML_NAME_START | XML_NAME | XML_NCNAME_START | XML_NCNAME;
				}
				
				// set name characters
				for (int i = 0; i < xmlNameChar.Length; i++)
				{
					UNI_CHARS[xmlNameChar[i]] |= XML_NAME | XML_NCNAME;
				}
				for (int i = 0; i < xmlDigitRange.Length; i++)
				{
					for (int j = xmlDigitRange[i][0]; j <= xmlDigitRange[i][1]; j++)
					{
						UNI_CHARS[j] |= XML_NAME | XML_NCNAME;
					}
				}
				for (int i = 0; i < xmlCombiningCharRange.Length; i++)
				{
					for (int j = xmlCombiningCharRange[i][0]; j <= xmlCombiningCharRange[i][1]; j++)
					{
						UNI_CHARS[j] |= XML_NAME | XML_NCNAME;
					}
				}
				for (int i = 0; i < xmlCombiningCharChar.Length; i++)
				{
					UNI_CHARS[xmlCombiningCharChar[i]] |= XML_NAME | XML_NCNAME;
				}
				for (int i = 0; i < xmlExtenderRange.Length; i++)
				{
					for (int j = xmlExtenderRange[i][0]; j <= xmlExtenderRange[i][1]; j++)
					{
						UNI_CHARS[j] |= XML_NAME | XML_NCNAME;
					}
				}
				for (int i = 0; i < xmlExtenderChar.Length; i++)
				{
					UNI_CHARS[xmlExtenderChar[i]] |= XML_NAME | XML_NCNAME;
				}
				
				// remove ':' from allowable XML_NCNAME_START and XML_NCNAME chars
				UNI_CHARS[':'] &= ~ (XML_NCNAME_START | XML_NCNAME);
				
				// set Pubid characters
				for (int i = 0; i < xmlPubidChar.Length; i++)
				{
					UNI_CHARS[xmlPubidChar[i]] |= (sbyte) (XML_PUBID);
				}
				for (int i = 0; i < xmlPubidRange.Length; i++)
				{
					for (int j = xmlPubidRange[i][0]; j <= xmlPubidRange[i][1]; j++)
					{
						UNI_CHARS[j] |= (sbyte) (XML_PUBID);
					}
				}
			}
		}
	}
}