/* 
 * Copyright (C) 2002-2004 XimpleWare, info@ximpleware.com
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
#ifndef XMLCHAR_H
#define XMLCHAR_H
#include "customTypes.h"

#define LENGTH(x,y) (sizeof(x)/sizeof(y))


 int MASK_VALID;//= 0x01;
 int MASK_SPACE; //= 0x01<<1;
 int MASK_NAME_START; //= 0x01<<2;
 int MASK_NAME; //= 0x01<<3;
 int MASK_PUBID; //= 0x01<<4;
 int MASK_CONTENT; //= 0x01<<5;
 int MASK_NCNAME_START; //= 0x01 << 6;
 int MASK_NCNAME;// = 0x01 << 7;

unsigned char CHARS[1<<16];// = unsigned Byte[1<<16];

Boolean isReady;// = FALSE;


void XMLChar_init();
   /**
     * Returns true if the specified character is a supplemental character.
     *
     * @param c The character to check.
     */
    inline Boolean XMLChar_isSupplemental(int c) {
       return (c >= 0x10000 && c <= 0x10FFFF);
    }

    /**
     * Returns true the supplemental character corresponding to the given
     * surrogates.
     *
     * @param h The high surrogate.
     * @param l The low surrogate.
     */
    inline int XMLChar_isSupplementalChar(char h, char l) {
        return (h - 0xD800) * 0x400 + (l - 0xDC00) + 0x10000;
    }

    /**
     * Returns the high surrogate of a supplemental character
     *
     * @param c The supplemental character to "split".
     */
    inline  unsigned short XMLChar_highSurrogate(int c) {
        return (unsigned short) (((c - 0x00010000) >> 10) + 0xD800);
    }

    /**
     * Returns the low surrogate of a supplemental character
     *
     * @param c The supplemental character to "split".
     */
    inline  unsigned short XMLChar_lowSurrogate(int c) {
        return (unsigned short) (((c - 0x00010000) & 0x3FF) + 0xDC00);
    }

    /**
     * Returns whether the given character is a high surrogate
     *
     * @param c The character to check.
     */
    inline  Boolean XMLChar_isHighSurrogate(int c) {
        return (0xD800 <= c && c <= 0xDBFF);
    }

    /**
     * Returns whether the given character is a low surrogate
     *
     * @param c The character to check.
     */
    inline  Boolean XMLChar_isLowSurrogate(int c) {
        return (0xDC00 <= c && c <= 0xDFFF);
    }


    /**
     * Returns true if the specified character is valid. This method
     * also checks the surrogate character range from 0x10000 to 0x10FFFF.
     * <p>
     * If the program chooses to apply the mask directly to the
     * <code>CHARS</code> array, then they are responsible for checking
     * the surrogate character range.
     *
     * @param c The character to check.
     */
    inline  Boolean XMLChar_isValidChar(int c) {
        return (c < 0x10000 && (CHARS[c] & MASK_VALID) != 0) ||
               (0x10000 <= c && c <= 0x10FFFF);
    } // isValid(int):boolean

    /**
     * Returns true if the specified character is invalid.
     *
     * @param c The character to check.
     */
    inline  Boolean XMLChar_isInvalidChar(int c) {
        return !XMLChar_isValidChar(c);
    } // isInvalid(int):boolean

    /**
     * Returns true if the specified character can be considered content.
     *
     * @param c The character to check.
     */
    inline  Boolean XMLChar_isContentChar(int c) {
        return (c < 0x10000 && (CHARS[c] & MASK_CONTENT) != 0) ||
               (0x10000 <= c && c <= 0x10FFFF);
    } // isContent(int):boolean

    /**
     * Returns true if the specified character can be considered markup.
     * Markup characters include '&lt;', '&amp;', and '%'.
     *
     * @param c The character to check.
     */
    inline  Boolean XMLChar_isMarkupChar(int c) {
        return c == '<' || c == '&' || c == '%';
    } // isMarkup(int):boolean

    /**
     * Returns true if the specified character is a space character
     * as defined by production [3] in the XML 1.0 specification.
     *
     * @param c The character to check.
     */
    inline  Boolean XMLChar_isSpaceChar(int c) {
        return c < 0x10000 && (CHARS[c] & MASK_SPACE) != 0;
    } // isSpace(int):boolean

    /**
     * Returns true if the specified character is a valid name start
     * character as defined by production [5] in the XML 1.0
     * specification.
     *
     * @param c The character to check.
     */
    inline  Boolean XMLChar_isNameStartChar(int c) {
        return c < 0x10000 && (CHARS[c] & MASK_NAME_START) != 0;
    } // isNameStart(int):boolean

    /**
     * Returns true if the specified character is a valid name
     * character as defined by production [4] in the XML 1.0
     * specification.
     *
     * @param c The character to check.
     */
    inline  Boolean XMLChar_isNameChar(int c) {
        return c < 0x10000 && (CHARS[c] & MASK_NAME) != 0;
    } // isName(int):boolean

    /**
     * Returns true if the specified character is a valid NCName start
     * character as defined by production [4] in Namespaces in XML
     * recommendation.
     *
     * @param c The character to check.
     */
    inline  Boolean XMLChar_isNCNameStart(int c) {
        return c < 0x10000 && (CHARS[c] & MASK_NCNAME_START) != 0;
    } // isNCNameStart(int):boolean

    /**
     * Returns true if the specified character is a valid NCName
     * character as defined by production [5] in Namespaces in XML
     * recommendation.
     *
     * @param c The character to check.
     */
    inline  Boolean XMLChar_isNCName(int c) {
        return c < 0x10000 && (CHARS[c] & MASK_NCNAME) != 0;
    } // isNCName(int):boolean

    /**
     * Returns true if the specified character is a valid Pubid
     * character as defined by production [13] in the XML 1.0
     * specification.
     *
     * @param c The character to check.
     */
    inline Boolean XMLChar_isPubid(int c) {
        return c < 0x10000 && (CHARS[c] & MASK_PUBID) != 0;
    } // isPubid(int):boolean


    /*
     * [5] Name ::= (Letter | '_' | ':') (NameChar)*
     */
    /**
     * Check to see if a string is a valid Name according to [5]
     * in the XML 1.0 Recommendation
     *
     * @param name string to check
     * @return true if name is a valid Name
     */
/*    inline  static bool isValidName(const string& name)
	{
        if(name.empty() || (!isNameStart(name[0]))) return false;

        for (int i = 1; i < name.length(); i++ )
           if(!isName(name[i])) return false;

		return true;
    } // isValidName(String):boolean
*/

    /*
     * from the namespace rec
     * [4] NCName ::= (Letter | '_') (NCNameChar)*
     */
    /**
     * Check to see if a string is a valid NCName according to [4]
     * from the XML Namespaces 1.0 Recommendation
     *
     * @param name string to check
     * @return true if name is a valid NCName
     */
/*    inline  static bool isValidNCName(const string& ncName)
	{
        if (ncName.empty() ||(!isNCNameStart(ncName[0]))) return false;

        for (int i = 1; i < ncName.length(); i++ )
           if(!isNCName(ncName[i])) return false;

		return true;
    } // isValidNCName(String):boolean
*/
    /*
     * [7] Nmtoken ::= (NameChar)+
     */
    /**
     * Check to see if a string is a valid Nmtoken according to [7]
     * in the XML 1.0 Recommendation
     *
     * @param nmtoken string to check
     * @return true if nmtoken is a valid Nmtoken
     */
/*  inline  static bool isValidNmtoken(const string& nmtoken) {
        if(nmtoken.empty()) return false;

        for (int i = 0; i < nmtoken.length(); i++ )
           if(!isName(nmtoken[i])) return false;

		return true;
    } // isValidName(String):boolean
*/

#endif