/* 
 * Copyright (C) 2002-2006 XimpleWare, info@ximpleware.com
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
#ifndef VTDNAV_H
#define VTDNAV_H
#include "customTypes.h"
#include "fastIntBuffer.h"
#include "fastLongBuffer.h"
#include "contextBuffer.h"
#include "UTF8Char.h"
#include "XMLChar.h"
#include "decoder.h"
#include <math.h>

#if BIG_ENDIAN
#define MASK_TOKEN_FULL_LEN 0x000fffff00000000L
#define MASK_TOKEN_PRE_LEN 0x000ff80000000000L
#define MASK_TOKEN_QN_LEN  0x000007ff00000000L
#define MASK_TOKEN_OFFSET1  0x000000003fffffffL
#define MASK_TOKEN_OFFSET2  0x000000007fffffffL

#define MASK_TOKEN_TYPE  0xf000000000000000L
#define MASK_TOKEN_DEPTH  0x0ff0000000000000L
#define MASK_TOKEN_NS_MARK 0x00000000c0000000L
#else
#define MASK_TOKEN_OFFSET3  0xffffff3f00000000L
#define MASK_TOKEN_OFFSET4  0xffffff7f00000000L
//define MASKS for small endians
#define MASK_TOKEN_NS_MARK_LE 0x000000c000000000L
#endif
//#define ROOT 0
//#define PARENT 1
//#define FIRST_CHILD 2
//#define LAST_CHILD 3
//#define NEXT_SIBLING 4
//#define PREV_SIBLING 5

#define R 0
#define P 1
#define FC 2
#define LC 3
#define NS 4
#define PS 5


typedef struct vTDNav{
	int rootIndex;
	int nestingLevel;
	int* context; // context object
	Boolean atTerminal; // Add this model to be compatible with XPath data model, 
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

	int encoding;

	//int currentOffset;
	//int currentOffset2;

	int ns;
	int* stackTemp;
	int docOffset;	 // starting offset of the XML doc wrt XMLDoc
	int docLen;  // size of XML document
	int vtdSize; // # of entries in vtdBuffer equvalent 
	             // to calling size(FastLongBuffer *flb) defined in fastLongBuffer.h
	int bufLen; // size of XMLDoc in bytes
	Boolean br; // buffer reuse flag
} VTDNav;


//functions
//Create VTDNav object

VTDNav *createVTDNav(int r, encoding enc, Boolean ns, int depth,
					 UByte *x, int xLen, FastLongBuffer *vtd, FastLongBuffer *l1,
					 FastLongBuffer *l2, FastIntBuffer *l3, int so, int len,Boolean br);

//Free VTDNav object
void freeVTDNav(VTDNav *vn);

//Return the attribute count of the element at the cursor position.
int getAttrCount(VTDNav *vn);

//Get the token index of the attribute value given an attribute name.     
int getAttrVal(VTDNav *vn, UCSChar *attrName);

//Get the token index of the attribute value of given URL and local name.
//If ns is not enabled, the lookup will return -1, indicating a no-found.
//Also namespace nodes are invisible using this method.
int getAttrValNS(VTDNav *vn, UCSChar* URL, UCSChar *localName);


//Get the depth (>=0) of the current element.
extern inline int getCurrentDepth(VTDNav *vn);

// Get the index value of the current element.
extern inline int getCurrentIndex(VTDNav *vn);
extern inline int getCurrentIndex2(VTDNav *vn);
// Get the starting offset and length of an element
// encoded in a long, upper 32 bit is length; lower 32 bit is offset
Long getElementFragment(VTDNav *vn);

/**
 * Get the encoding of the XML document.
 * <pre>   0  ASCII       </pre>
 * <pre>   1  ISO-8859-1  </pre>
 * <pre>   2  UTF-8       </pre>
 * <pre>   3  UTF-16BE    </pre>
 * <pre>   4  UTF-16LE    </pre>
 */
extern inline encoding getEncoding(VTDNav *vn);

// Get the maximum nesting depth of the XML document (>0).
// max depth is nestingLevel -1

// max depth is nestingLevel -1
extern inline int getNestingLevel(VTDNav *vn);

// Get root index value.
extern inline int getRootIndex(VTDNav *vn);

// This function returns of the token index of the type character data or CDATA.
// Notice that it is intended to support data orient XML (not mixed-content XML).
int getText(VTDNav *vn);

//Get total number of VTD tokens for the current XML document.
extern inline int getTokenCount(VTDNav *vn);

//Get the depth value of a token (>=0)
int getTokenDepth(VTDNav *vn, int index);

//Get the token length at the given index value
//please refer to VTD spec for more details
int getTokenLength(VTDNav *vn, int index);

//Get the starting offset of the token at the given index.
extern inline int getTokenOffset(VTDNav *vn, int index);

// Get the XML document 
extern inline UByte* getXML(VTDNav *vn);

//Get the token type of the token at the given index value.
extern inline	tokenType getTokenType(VTDNav *vn, int index);

//Test whether current element has an attribute with the matching name.
Boolean hasAttr(VTDNav *vn, UCSChar *attrName);

//Test whether the current element has an attribute with 
//matching namespace URL and localname.
Boolean hasAttrNS(VTDNav *vn, UCSChar *URL, UCSChar *localName);

//This method is similar to getElementByName in DOM except it doesn't
//return the nodeset, instead it iterates over those nodes.
int iterate(VTDNav *vn, int dp, UCSChar *en, Boolean special);

//This method is similar to getElementByName in DOM except it doesn't
//return the nodeset, instead it iterates over those nodes .
//When URL is "*" it will match any namespace
//if ns is false, return false immediately
int iterateNS(VTDNav *vn, int dp, UCSChar *URL, UCSChar *ln);

// This function is called by selectElement_P in autoPilot
Boolean iterate_preceding(VTDNav *vn,UCSChar *en, int* a, Boolean special);

// This function is called by selectElementNS_P in autoPilot
Boolean iterate_precedingNS(VTDNav *vn,UCSChar *URL, UCSChar *ln, int* a);

// This function is called by selectElement_F in autoPilot
Boolean iterate_following(VTDNav *vn,UCSChar *en, Boolean special);


// This function is called by selectElementNS_F in autoPilot
Boolean iterate_followingNS(VTDNav *vn, UCSChar *URL, UCSChar *ln);


//Test if the current element matches the given name.
Boolean matchElement(VTDNav *vn, UCSChar *en);

//Test whether the current element matches the given namespace URL and localname.
//URL, when set to "*", matches any namespace (including null), when set to null, defines a "always-no-match"
//ln is the localname that, when set to *, matches any localname
Boolean matchElementNS(VTDNav *vn, UCSChar *URL, UCSChar *ln);

//Match the string against the token at the given index value. When a token
//is an attribute name or starting tag, qualified name is what gets matched against
Boolean matchRawTokenString(VTDNav *vn, int index, UCSChar *s);
//This method matches two VTD tokens of 2 VTDNavs
Boolean matchTokens(VTDNav *vn, int i1, VTDNav *vn2, int i2);

//Match the string against the token at the given index value. When a token
//is an attribute name or starting tag, qualified name is what gets matched against
Boolean matchTokenString(VTDNav *vn, int index, UCSChar *s);

//Convert a vtd token into a double.
double parseDouble(VTDNav *vn, int index);

//Convert a vtd token into a float.
float parseFloat(VTDNav *vn, int index);

//Convert a vtd token into an int
int parseInt(VTDNav *vn, int index);

//Convert a vtd token into a long
Long parseLong(VTDNav *vn, int index);

//Load the context info from ContextBuffer.
//Info saved including LC and current state of the context 
Boolean pop(VTDNav *vn);
Boolean pop2(VTDNav *vn);
//Store the context info into the ContextBuffer.
//Info saved including LC and current state of the context 
Boolean push(VTDNav *vn);
Boolean push2(VTDNav *vn);
void sampleState(VTDNav *vn, FastIntBuffer *fib);

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
Boolean toElement(VTDNav *vn, navDir direction);

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
Boolean toElement2(VTDNav *vn, navDir direction, UCSChar *en);
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
Boolean toElementNS(VTDNav *vn, navDir direction, UCSChar *URL, UCSChar *ln);

//This method normalizes a token into a string in a way that resembles DOM.
//The leading and trailing white space characters will be stripped.
//The entity and character references will be resolved
//Multiple whitespaces char will be collapsed into one.
UCSChar *toNormalizedString(VTDNav *vn, int index);

//Convert a token at the given index to a String, 
//(built-in entity and char references not resolved)
//(entities and char references not expanded).
UCSChar *toRawString(VTDNav *vn, int index);
UCSChar *toRawString2(VTDNav *vn, int os, int len);

//Convert a token at the given index to a String, (entities and char 
//references resolved).
// An attribute name or an element name will get the UCS2 string of qualified name 
UCSChar *toString(VTDNav *vn, int index);
UCSChar *toString2(VTDNav *vn, int os, int len);

/**
 * Set the value of atTerminal
 * This function only gets called in XPath eval
 * when a step calls for @* or child::text()
 */

void setAtTerminal(VTDNav* vn, Boolean b);

/**
 * Get the value of atTerminal
 * This function only gets called in XPath eval
 */
Boolean getAtTerminal(VTDNav *vn);

extern inline int swap_bytes(int i);
int lookupNS(VTDNav *vn);

#endif
