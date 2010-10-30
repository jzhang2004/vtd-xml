/* 
* Copyright (C) 2002-2010 XimpleWare, info@ximpleware.com
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
#pragma once
#ifndef VTDNAV_H
#define VTDNAV_H

//#include "customTypes.h"
//#include "fastIntBuffer.h"
//#include "fastLongBuffer.h"
//#include "contextBuffer.h"
//#include "elementFragmentNs.h"
//#include "XMLChar.h"
#include "indexHandler.h"
#include "fastIntBuffer.h"
#include "fastLongBuffer.h"
#include "contextBuffer.h"
#include "UTF8Char.h"
#include "XMLChar.h"
// Get declaration for f(int i, char c, float x)
#include "decoder.h"
#include <math.h>
//#include <new>
//#include <iostream>

namespace com_ximpleware {
	class FastLongBuffer;
	class VTDNav {
		friend class ElementFragmentNs;
		friend class FastIntBuffer;
		friend class FastLongBuffer;
		friend class Expr;
		friend class FilterExpr;
		friend class UnionExpr;
		friend class PathExpr;
		friend class LocationPathExpr;
		friend class BinaryExpr;
		friend class VTDGen;
		friend class TextIter;
		friend struct NodeTest;
		friend class BookMark;
		friend class AutoPilot;
		friend class FuncExpr;
		friend class NodeRecorder;
		friend class XMLModifier;
	private:
		int rootIndex;
		int nestingLevel;
		int* context; // context object
		bool atTerminal; // Add this model to be compatible with XPath data model, 
		// true if attribute axis or text()
		// location cache part
		int l2upper;
		int l2lower;
		int l3upper;
		int l3lower;
		int l2index;
		int l3index;
		int l1index;

		FastLongBuffer *vtdBuffer;
		FastLongBuffer *l1Buffer;
		FastLongBuffer *l2Buffer;
		FastIntBuffer *l3Buffer;
		UByte* XMLDoc;

		Long offsetMask;
		ContextBuffer *contextBuf;
		ContextBuffer *contextBuf2;// this is reserved for XPath

		int LN;// record txt and attrbute for XPath eval purposes

		encoding_t encoding;

		//int currentOffset;
		//int currentOffset2;

		bool ns;
		int* stackTemp;
		int docOffset;	 // starting offset of the XML doc wrt XMLDoc
		int docLen;  // size of XML document
		int vtdSize; // # of entries in vtdBuffer equvalent 
		// to calling size(FastLongBuffer *flb) defined in fastLongBuffer.h
		int bufLen; // size of XMLDoc in bytes
		bool br; // buffer reuse flag
		bool master; // true if vn is obtained by calling getNav(), otherwise false
		// useful for implementing dupliateNav() and cloneNav();

		Long getChar(int offset);
		Long getCharResolved(int offset);
		int getCharUnit( int offset);
		Long handle_utf8( Long temp, int offset);
		Long handle_utf16le( int offset);
		Long handle_utf16be( int offset);
		bool isElement(int index);
		bool isElementOrDocument( int index);
		bool isWS(int ch);
		bool matchRawTokenString1( int offset, int len, UCSChar *s);
		bool matchRawTokenString2( Long l, UCSChar *s);
		bool matchTokenString1( int offset, int len, UCSChar *s);
		bool matchTokenString2( Long l, UCSChar *s);
		inline int NSval( int i);
		int parseInt2( int index, int radix);
		Long parseLong2( int index, int radix);
		void resolveLC();
		bool resolveNS( UCSChar *URL);
		bool resolveNS2( UCSChar *URL, int offset, int len); //UCSChar *ln);
		int lookupNS2( int offset, int len);
		Long getChar4OtherEncoding( int offset);
		int decode(int offset);
		int compareRawTokenString2( int offset, int len, UCSChar *s);
		int compareTokenString2( int offset, int len, UCSChar *s);
		UCSChar *toStringUpperCase2( int os, int len);
		UCSChar *toStringLowerCase2( int os, int len);
		//UCSChar *toRawStringUpperCase( int index);
		//UCSChar *toRawStringLowerCase( int index);
		UCSChar *toRawStringUpperCase2( int os, int len);
		UCSChar *toRawStringLowerCase2( int os, int len);
		void resolveLC_l1();
		void resolveLC_l2();
		void resolveLC_l3();
		void recoverNode_l1(int index);
		void recoverNode_l2(int index);
		void recoverNode_l3(int index);

	protected:
		VTDNav(int r, 
			encoding_t enc, 
			bool ns, 
			int depth,
			UByte *x, 
			int xLen, 
			FastLongBuffer *vtd, 
			FastLongBuffer *l1,
			FastLongBuffer *l2, 
			FastIntBuffer *l3, 
			int so, 
			int len,
			bool br);

	public:
		const static Long MASK_TOKEN_FULL_LEN=0x000fffff00000000LL;
		const static Long MASK_TOKEN_PRE_LEN=0x000ff80000000000LL;
		const static Long MASK_TOKEN_QN_LEN=0x000007ff00000000LL;
		const static Long MASK_TOKEN_OFFSET1=0x000000003fffffffL;
		const static Long MASK_TOKEN_OFFSET2=0x000000007fffffffL;

		const static Long MASK_TOKEN_TYPE=0xf000000000000000LL;
		const static Long MASK_TOKEN_DEPTH=0x0ff0000000000000LL;
		const static Long MASK_TOKEN_NS_MARK=0x00000000c0000000LL;

		const static int R=0;
		const static int P=1;
		const static int FC=2;
		const static int LC=3;
		const static int NS=4;
		const static int PS=5;

		//

		virtual ~VTDNav();
		//Return the attribute count of the element at the cursor position.
		int getAttrCount();
		//Get the token index of the attribute value given an attribute name.     
		int getAttrVal(UCSChar *attrName);
		//Get the token index of the attribute value of given URL and local name.
		//If ns is not enabled, the lookup will return -1, indicating a no-found.
		//Also namespace nodes are invisible using this method.
		int getAttrValNS(UCSChar* URL, UCSChar *localName);
		//Get the depth (>=0) of the current element.
		int getCurrentDepth();

		// Get the index value of the current element.
		int getCurrentIndex();
		int getCurrentIndex2();
		// Get the starting offset and length of an element
		// encoded in a long, upper 32 bit is length; lower 32 bit is offset
		Long getElementFragment();
		Long getContentFragment();
		//ElementFragmentNs* getElementFragmentNs();
		int* getElementFragmentNs();
		/**
		* Get the encoding of the XML document.
		* <pre>   0  ASCII       </pre>
		* <pre>   1  ISO-8859-1  </pre>
		* <pre>   2  UTF-8       </pre>
		* <pre>   3  UTF-16BE    </pre>
		* <pre>   4  UTF-16LE    </pre>
		*/
		encoding_t getEncoding(){
			return encoding;
		}
		// Get the maximum nesting depth of the XML document (>0).
		// max depth is nestingLevel -1

		// max depth is nestingLevel -1
		int getNestingLevel();

		// Get root index value.
		int getRootIndex();
		// This function returns of the token index of the type character data or CDATA.
		// Notice that it is intended to support data orient XML (not mixed-content XML).
		int getText();

		//Get total number of VTD tokens for the current XML document.
		int getTokenCount();

		//Get the depth value of a token (>=0)
		int getTokenDepth(int index);

		//Get the token length at the given index value
		//please refer to VTD spec for more details
		int getTokenLength(int index);

		//Get the starting offset of the token at the given index.
		int getTokenOffset(int index); /*{
			return (int) ((vtdBuffer->longAt(index)) & offsetMask);
			return 1;
		}*/	

		/*void inlineTest(){
			printf("good %d \n",vtdBuffer);
		}*/

		// Get the XML document 
		UByte* getXML();

		//Get the token type of the token at the given index value.
		tokenType getTokenType(int index);/*{
			return (tokenType) (((vtdBuffer->longAt(index) & MASK_TOKEN_TYPE) >> 60) & 0xf);
		}*/

		//Test whether current element has an attribute with the matching name.
		bool hasAttr(UCSChar *attrName);

		//Test whether the current element has an attribute with 
		//matching namespace URL and localname.
		bool hasAttrNS(UCSChar *URL, UCSChar *localName);

		//This method is similar to getElementByName in DOM except it doesn't
		//return the nodeset, instead it iterates over those nodes.
		bool iterate(int dp, UCSChar *en, bool special);

		//This method is similar to getElementByName in DOM except it doesn't
		//return the nodeset, instead it iterates over those nodes .
		//When URL is "*" it will match any namespace
		//if ns is false, return false immediately
		bool iterateNS(int dp, UCSChar *URL, UCSChar *ln);

		// This function is called by selectElement_P in autoPilot
		bool iterate_preceding(UCSChar *en, int* a, bool special);

		// This function is called by selectElementNS_P in autoPilot
		bool iterate_precedingNS(UCSChar *URL, UCSChar *ln, int* a);

		// This function is called by selectElement_F in autoPilot
		bool iterate_following(UCSChar *en, bool special);


		// This function is called by selectElementNS_F in autoPilot
		bool iterate_followingNS( UCSChar *URL, UCSChar *ln);


		//Test if the current element matches the given name.
		bool matchElement( UCSChar *en);

		//Test whether the current element matches the given namespace URL and localname.
		//URL, when set to "*", matches any namespace (including null), when set to null, defines a "always-no-match"
		//ln is the localname that, when set to *, matches any localname
		bool matchElementNS( UCSChar *URL, UCSChar *ln);

		//Match the string against the token at the given index value. When a token
		//is an attribute name or starting tag, qualified name is what gets matched against
		bool matchRawTokenString( int index, UCSChar *s);
		//This method matches two VTD tokens of 2 VTDNavs
		bool matchTokens( int i1, VTDNav *vn2, int i2);

		//Match the string against the token at the given index value. When a token
		//is an attribute name or starting tag, qualified name is what gets matched against
		bool matchTokenString( int index, UCSChar *s);

		//Convert a vtd token into a double.
		double parseDouble( int index);

		//Convert a vtd token into a float.
		float parseFloat( int index);

		//Convert a vtd token into an int
		int parseInt( int index);

		//Convert a vtd token into a long
		Long parseLong( int index);

		//Load the context info from ContextBuffer.
		//Info saved including LC and current state of the context 
		bool pop();
		bool pop2();
		//Store the context info into the ContextBuffer.
		//Info saved including LC and current state of the context 
		bool push();
		bool push2();
		void sampleState( FastIntBuffer *fib);

		// A generic navigation method.
		// Move the current to the element according to the direction constants
		// If no such element, no position change and return false (0).
		/* Legal direction constants are 	<br>
		* <pre>		ROOT            0  </pre>
		* <pre>		PARENT          1  </pre>
		* <pre>		FIRST_CHILD     2  </pre>
		* <pre>		LAST_CHILD      3  </pre>
		* <pre>		NEXT_SIBLING    4  </pre>
		* <pre>		PREV_SIBLING    5  </pre>
		* <br>
		*/
		bool toElement( navDir direction);

		/**
		* A generic navigation method.
		* Move the current to the element according to the direction 
		* constants and the element name
		* If no such element, no position change and return false (0).
		* "*" matches any element
		* Legal direction constants are 	<br>
		* <pre>		ROOT            0  </pre>
		* <pre>		PARENT          1  </pre>
		* <pre>		FIRST_CHILD     2  </pre>
		* <pre>		LAST_CHILD      3  </pre>
		* <pre>		NEXT_SIBLING    4  </pre>
		* <pre>		PREV_SIBLING    5  </pre>
		* <br>
		* for ROOT and PARENT, element name will be ignored.
		*/
		bool toElement2( navDir direction, UCSChar *en);
		/*	
		* A generic navigation function with namespace support.
		* Move the current to the element according to the direction constants and the prefix and local names
		* If no such element, no position change and return false(0).
		* URL * matches any namespace, including undefined namespaces
		* a null URL means hte namespace prefix is undefined for the element
		* ln *  matches any localname
		* Legal direction constants are<br>
		* <pre>		ROOT            0  </pre>
		* <pre>		PARENT          1  </pre>
		* <pre>		FIRST_CHILD     2  </pre>
		* <pre>		LAST_CHILD      3  </pre>
		* <pre>		NEXT_SIBLING    4  </pre>
		* <pre>		PREV_SIBLING    5  </pre>
		* <br>
		* for ROOT and PARENT, element name will be ignored.
		* If not ns enabled, return false immediately with no position change.
		*/
		bool toElementNS( navDir direction, UCSChar *URL, UCSChar *ln);

		//This method normalizes a token into a string in a way that resembles DOM.
		//The leading and trailing white space characters will be stripped.
		//The entity and character references will be resolved
		//Multiple whitespaces char will be collapsed into one.
		UCSChar *toNormalizedString( int index);

		//Convert a token at the given index to a String, 
		//(built-in entity and char references not resolved)
		//(entities and char references not expanded).
		//os and len are in bytes
		UCSChar *toRawString( int index);
		UCSChar *toRawString2( int os, int len);

		//Convert a token at the given index to a String, (entities and char 
		//references resolved).
		// An attribute name or an element name will get the UCS2 string of qualified name 
		//os and len are in bytes
		UCSChar *toString( int index);
		UCSChar *toString2( int os, int len);

		/**
		* Set the value of atTerminal
		* This function only gets called in XPath eval
		* when a step calls for @* or child::text()
		*/

		void setAtTerminal(bool b){
			atTerminal = b;
		}

		/**
		* Get the value of atTerminal
		* This function only gets called in XPath eval
		*/
		//bool getAtTerminal();

		bool getAtTerminal(){
			return atTerminal;
		}
		//inline int swap_bytes(int i);
		int lookupNS();

		/* Write VTD+XML into a FILE pointer */
		//bool writeIndex_VTDNav(VTDNav * FILE *f);

		/* overwrite */
		bool overWrite( int index, UByte* ba, int offset, int len);

		int compareTokenString(int index, UCSChar *s);

		int compareRawTokenString( int index, UCSChar *s);

		int compareTokens( int i1, VTDNav *vn2, int i2);

		/* Write VTD+XML into a FILE pointer */
		bool writeIndex(FILE *f);

		/* Write VTD+XML into a file of given name */
		bool writeIndex2(char *fileName);

		/* Write the VTDs and LCs into an file*/
		void writeSeparateIndex_VTDNav( char *vtdIndex);

		/* pre-calculate the VTD+XML index size without generating the actual index */
		Long getIndexSize2();

		/* dump XML text into a given file name */
		void dumpXML( char *fileName);

		/* dump XML text into a given file descriptor */
		void dumpXML2( FILE *f);

		/*Get the string length as if the token is converted into a normalized UCS string */
		int getNormalizedStringLength( int index);
		/*Get the string length as if the token is converted into a UCS string (entity resolved) */
		int getStringLength( int index);
		/*Get the string length as if the token is converted into a UCS string (entity not resolved) */
		int getRawStringLength( int index);
		/* Get the offset value right after head (e.g. <a b='b' c='c'> ) */
		int getOffsetAfterHead();

		/* Test the start of token content at index i matches the content 
		of s, notice that this is to save the string allocation cost of
		using String's built-in startsWidth */
		bool startsWith( int index, UCSChar *s);

		/*Test the end of token content at index i matches the content 
		of s, notice that this is to save the string allocation cost of
		using String's built-in endsWidth */

		bool endsWith( int index, UCSChar *s);

		/*Test whether a given token contains s. notie that this function
		directly operates on the byte content of the token to avoid string creation */

		bool contains( int index, UCSChar *s);

		/* Convert a token at the given index to a String and any lower case
		character will be converted to upper case, (entities and char
		references resolved).*/
		UCSChar *toStringUpperCase( int index);

		/* Convert a token at the given index to a String and any upper case
		character will be converted to lower case, (entities and char
		references resolved).*/
		UCSChar *toStringLowerCase( int index);

		/* Convert a token at the given index to a String and any lower case
		character will be converted to upper case, (entities and char
		references resolved for character data and attr val).*/
		UCSChar *toRawStringUpperCase( int index);

		/* Convert a token at the given index to a String and any upper case
		character will be converted to lower case, (entities and char
		references resolved for character data and attr val).*/
		UCSChar *toRawStringLowerCase( int index);

		/* DupliateNav duplicates an instance of VTDNav but doesn't retain the original node position*/
		VTDNav *duplicateNav();
		/* ClineNav duplicates an instance of VTDNav, also copies node position over */
		VTDNav *cloneNav();

		/* This method takes a vtd index, and recover its correspondin
		* node position, the index can only be of node type element,
		* document, attribute name, attribute value or character data,
		* or CDATA  */
		void recoverNode( int index);

	};

	inline int VTDNav::getCurrentDepth(){
		return context[0];
	}

	// Get the index value of the current element.
	inline int VTDNav::getCurrentIndex(){	
		if (atTerminal)
			return LN;
		switch(context[0]){
		case -1: return 0;
		case 0: return rootIndex;
		default: return context[context[0]];
		}
	}
	inline int VTDNav::getCurrentIndex2(){
		switch(context[0]){
		case -1: return 0;
		case 0: return rootIndex;
		default: return context[context[0]];
		}
	}

	// max depth is nestingLevel -1
	inline int VTDNav::getNestingLevel(){
		return nestingLevel;
	}

	// Get root index value.
	inline int VTDNav::getRootIndex(){
		return rootIndex;
	}


	

	// Get the XML document 
	inline UByte* VTDNav::getXML(){
		return XMLDoc;
	}

	inline Long VTDNav::getChar(int offset){
			Long temp = 0;
			switch (encoding) {
			case FORMAT_ASCII : 
			case FORMAT_ISO_8859_1 :			
				temp = XMLDoc[offset];
				if (temp == '\r') {
					if (XMLDoc[offset + 1] == '\n') {
						return '\n'|(2LL<<32);
					} else {
						return '\n'|(1LL<<32);
					}
				}   
				return temp|(1LL<<32);

			case FORMAT_UTF8 :
				temp = XMLDoc[offset];
				if (temp<=127){
					if (temp == '\r') {
						if (XMLDoc[offset + 1] == '\n') {
							return '\n'|(2LL<<32);
						} else {
							return '\n'|(1LL<<32);
						}
					}
					return (temp|(1LL<<32));
				}
				return handle_utf8(temp,offset);

			case FORMAT_UTF_16BE :
				return handle_utf16be( offset);

			case FORMAT_UTF_16LE :
				return handle_utf16le(offset);
				// implement UTF-16LE to UCS4 conversion
				//System.out.println("UTF 16 LE unimplemented for now");
			default :
				return getChar4OtherEncoding(offset);
			}
		}

	inline int VTDNav::getCharUnit(int offset){
			return (encoding <=FORMAT_UTF8)
				? XMLDoc[offset]:
			(encoding <= FORMAT_WIN_1258)
				? decode(offset)
				: ((encoding == FORMAT_UTF_16BE)
				? (XMLDoc[offset << 1]
			<< 8 | XMLDoc[(offset << 1) + 1])
				: (XMLDoc[(offset << 1) + 1]
			<< 8 | XMLDoc[offset << 1]));
		}

	inline Long VTDNav::getCharResolved(int offset){
		int ch = 0;
		int val = 0;
		Long inc =2;
		Long l = getChar(offset);
		ch = (int)l;

		if (ch != '&')
			return l;


		// let us handle references here
		//currentOffset++;
		offset++;
		ch = getCharUnit(offset);
		offset++;
		switch (ch) {
			case '#' :

				ch = getCharUnit(offset);

				if (ch == 'x') {
					while (true) {
						offset++;
						inc++;
						ch = getCharUnit(offset);

						if (ch >= '0' && ch <= '9') {
							val = (val << 4) + (ch - '0');
						} else if (ch >= 'a' && ch <= 'f') {
							val = (val << 4) + (ch - 'a' + 10);
						} else if (ch >= 'A' && ch <= 'F') {
							val = (val << 4) + (ch - 'A' + 10);
						} else if (ch == ';') {
							inc++;
							break;
						} else{
							throw NavException(
								"navigation exception during getCharResolved \n"\
								"Illegal char in a char reference");
						}
						//throw new NavException("Illegal char in a char reference");
					}
				} else {
					while (true) {

						ch = getCharUnit(offset);
						offset++;
						inc++;
						if (ch >= '0' && ch <= '9') {
							val = val * 10 + (ch - '0');
						} else if (ch == ';') {
							break;
						} else{
							throw NavException(
								"navigation exception during getCharResolved \n"\
								"Illegal char in a char reference");
						}
					}
				}
				break;

			case 'a' :
				ch = getCharUnit( offset);
				if (ch == 'm') {
					if (getCharUnit( offset + 1) == 'p'
						&& getCharUnit( offset + 2) == ';') {
							inc = 5;
							val = '&';
					} else{
						throw NavException(
							"navigation exception during getCharResolved \n"\
							"illegal builtin reference");
					}
					//	throw new NavException("illegal builtin reference");
				} else if (ch == 'p') {
					if (getCharUnit(offset + 1) == 'o'
						&& getCharUnit(offset + 2) == 's'
						&& getCharUnit(offset + 3) == ';') {
							inc = 6;
							val = '\'';
					} else{
						throw NavException(
							"navigation exception during getCharResolved \n"\
							"illegal builtin reference");
					}
					//throw new NavException("illegal builtin reference");
				} else{
					throw NavException(
						"navigation exception during getCharResolved \n"\
						"illegal builtin reference");
				}
				//	throw new NavException("illegal builtin reference");
				break;

			case 'q' :

				if (getCharUnit(offset) == 'u'
					&& getCharUnit(offset + 1) == 'o'
					&& getCharUnit(offset + 2) == 't'
					&& getCharUnit(offset + 3) == ';') {
						inc = 6;
						val = '\"';
				} else{
					throw NavException(
						"navigation exception during getCharResolved \n"\
						"illegal builtin reference");
				}
				//throw new NavException("illegal builtin reference");
				break;
			case 'l' :
				if (getCharUnit(offset) == 't'
					&& getCharUnit(offset + 1) == ';') {
						inc = 4;
						val = '<';
				} else{
					throw NavException(
						"navigation exception during getCharResolved \n"\
						"illegal builtin reference");
				}
				//throw new NavException("illegal builtin reference");
				break;
			case 'g' :
				if (getCharUnit(offset) == 't'
					&& getCharUnit(offset + 1) == ';') {
						inc = 4;
						val = '>';
				} else{
					throw NavException(
						"navigation exception during getCharResolved \n"\
						"illegal builtin reference");
				}
				break;

			default :

				throw NavException(
					"navigation exception during getCharResolved \n"
					"Invalid entity char");
		}
		return val | (inc << 32);
	}


	inline bool VTDNav::isWS(int ch){
		return (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r');
	}
	inline bool VTDNav::matchRawTokenString1( int offset, int len, UCSChar *s){
		return  compareRawTokenString2(offset,len,s)==0;
	}
};

//Get the depth (>=0) of the current element.


#endif
