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
 
#include "vtdNav.h"
#include "indexHandler.h"
#include "elementFragmentNs.h"
#include "fastIntBuffer.h"
#include "bookMark.h"

static Long getChar(VTDNav *vn,int offset);
static Long getCharResolved(VTDNav *vn,int offset);
static int getCharUnit(VTDNav *vn, int index);
static Long handle_utf8(VTDNav *vn, Long temp, int offset);
static Long handle_utf16le(VTDNav *vn, int offset);
static Long handle_utf16be(VTDNav *vn, int offset);
static inline Boolean isElement(VTDNav  *vn, int index);
static inline Boolean isElementOrDocument(VTDNav *vn, int index);
static inline Boolean isWS(int ch);
static Boolean matchRawTokenString1(VTDNav *vn, int offset, int len, UCSChar *s);
static Boolean matchRawTokenString2(VTDNav *vn, Long l, UCSChar *s);
static Boolean matchTokenString1(VTDNav *vn, int offset, int len, UCSChar *s);
static Boolean matchTokenString2(VTDNav *vn, Long l, UCSChar *s);
static inline int NSval(VTDNav *vn, int i);
static int parseInt2(VTDNav *vn, int index, int radix);
static Long parseLong2(VTDNav *vn, int index, int radix);
static void resolveLC(VTDNav *vn);
static Boolean resolveNS(VTDNav *vn, UCSChar *URL);
static Boolean resolveNS2(VTDNav *vn, UCSChar *URL, int offset, int len); //UCSChar *ln);
static int lookupNS2(VTDNav *vn, int offset, int len);
static Long getChar4OtherEncoding(VTDNav *vn, int offset);
static int decode(VTDNav *vn,int offset);
static int compareRawTokenString2(VTDNav *vn, int offset, int len, UCSChar *s);
static int compareTokenString2(VTDNav *vn, int offset, int len, UCSChar *s);
static UCSChar *toStringUpperCase2(VTDNav *vn, int os, int len);
static UCSChar *toStringLowerCase2(VTDNav *vn, int os, int len);
//static UCSChar *toRawStringUpperCase(VTDNav *vn, int index);
//static UCSChar *toRawStringLowerCase(VTDNav *vn, int index);
static UCSChar *toRawStringUpperCase2(VTDNav *vn, int os, int len);
static UCSChar *toRawStringLowerCase2(VTDNav *vn, int os, int len);
static void resolveLC_l1(VTDNav *vn);
static void resolveLC_l2(VTDNav *vn);
static void resolveLC_l3(VTDNav *vn);
static void recoverNode_l1(VTDNav *vn,int index);
static void recoverNode_l2(VTDNav *vn,int index);
static void recoverNode_l3(VTDNav *vn,int index);
static int compareNormalizedTokenString2(VTDNav *vn,int offset, int len, UCSChar *s); 
/*Create VTDNav object*/
static Long handle_utf8(VTDNav *vn, Long temp, int offset){
	int c,d,a,i;
	Long val;
	//temp = vn->XMLDoc[vn->currentOffset];

	switch (UTF8Char_byteCount(temp)) {

			case 2 :
				c = 0x1f;
				d = 6;
				a = 1;
				break;
			case 3 :
				c = 0x0f;
				d = 12;
				a = 2;
				break;
			case 4 :
				c = 0x07;
				d = 18;
				a = 3;
				break;
			case 5 :
				c = 0x03;
				d = 24;
				a = 4;
				break;
			case 6 :
				c = 0x01;
				d = 30;
				a = 5;
				break;
			default :{
				throwException(nav_exception,0,
					"navigation exception during getChar",
					"UTF 8 encoding error: should never happen");
					 }
	}

	val = (temp & c) << d;
	i = a - 1;
	while (i >= 0) {
		temp = vn->XMLDoc[offset + a - i];
		if ((temp & 0xc0) != 0x80){
			throwException(nav_exception,0,
				"navigation exception during getChar",
				"UTF 8 encoding error: should never happen");
		}
		val = val | ((temp & 0x3f) << ((i<<2)+ (i<<1)));
		i--;
	}

	return val | ((Long)(a+1)<<32);
}

static Long handle_utf16le(VTDNav *vn, int offset){	
	int val,temp =
		vn->XMLDoc[(offset << 1) + 1]
	<< 8 | vn->XMLDoc[offset << 1];
	if (temp < 0xdc00 || temp > 0xdfff) {
		if (temp == '\r') {
			if (vn->XMLDoc[(offset << 1) + 2] == '\n'
				&& vn->XMLDoc[(offset << 1) + 3] == 0) {
					return '\n' | (2LL<<32) ;
			} else {
				return '\n' | (1LL<<32);
			}
		}
		return temp | (1LL<<32);
	} else {
		val = temp;
		temp =
			(vn->XMLDoc[(offset << 1) + 3]
		<< 8) | vn->XMLDoc[(offset << 1) + 2];
		if (temp < 0xd800 || temp > 0xdc00) {
			throwException(nav_exception,0,
				"navigation exception during getChar",
				"UTF 16 LE encoding error: should never happen");
		}
		val = ((temp - 0xd800) << 10) + (val - 0xdc00) + 0x10000;

		return val | (2LL<<32);
	}
}
static Long handle_utf16be(VTDNav *vn, int offset){
	int val, temp =
		vn->XMLDoc[offset << 1]
	<< 8 | vn->XMLDoc[(offset << 1) + 1];
	if ((temp < 0xd800)
		|| (temp >= 0xdc00)) { // not a high surrogate
			if (temp == '\r') {
				if (vn->XMLDoc[(offset << 1) + 3] == '\n'
					&& vn->XMLDoc[(offset << 1) + 2] == 0) {  
						return '\n'|(2LL<<32);
				} else {
					return '\n'|(1LL<<32);
				}
			}
			//currentOffset++;
			return temp| (1LL<<32);
	} else {
		val = temp;
		temp =
			vn->XMLDoc[(offset << 1) + 2]
		<< 8 | vn->XMLDoc[(offset << 1) + 3];
		if (temp < 0xdc00 || temp > 0xdfff) {
			throwException(nav_exception,0,
				"navigation exception during getChar",
				"UTF 16 BE encoding error: should never happen");
		}
		val = ((val - 0xd800) <<10) + (temp - 0xdc00) + 0x10000;
		return val | (2LL<<32);
	}
}

VTDNav *createVTDNav(int r, encoding_t enc, Boolean ns, int depth,
					 UByte *x, int xLen, FastLongBuffer *vtd, FastLongBuffer *l1,
					 FastLongBuffer *l2, FastIntBuffer *l3, int so, int len, Boolean br){
						 VTDNav* vn = NULL;
						 int i;
						 exception e;

						 if (l1 == NULL ||
							 l2 == NULL ||
							 l3 == NULL ||
							 vtd == NULL||
							 x == NULL ||
							 so<0 ||
							 len < 0 ||
							 xLen < 0 || // size of x
							 r < 0 ||
							 depth < 0 ||
							 (enc <FORMAT_ASCII || 
							 enc>FORMAT_UTF_16LE) 
							 )
						 {
							 throwException2(invalid_argument,
								 "Invalid argument when creating VTDGen failed ");
							 return NULL;
						 }

						 vn = (VTDNav *) malloc(sizeof(VTDNav));
						 if (vn==NULL){
							 throwException2(out_of_mem,							 
								 "VTDNav allocation failed ");
							 return NULL;
						 }
						 vn->l1Buffer = l1;
						 vn->l2Buffer = l2;
						 vn->l3Buffer = l3;
						 vn->vtdBuffer= vtd;
						 vn->XMLDoc = x;

						 vn->encoding = enc;
						 vn->rootIndex = r;
						 vn->nestingLevel = depth +1;

						 vn->ns = ns;

						 if (ns == TRUE)
							 vn->offsetMask = MASK_TOKEN_OFFSET1;
						 else 
							 vn->offsetMask = MASK_TOKEN_OFFSET2;



						 vn->atTerminal = FALSE;
						 vn->context = (int *)malloc(vn->nestingLevel*sizeof(int));
						 if (vn->context == NULL){
							 throwException2(out_of_mem,							 
								 "VTDNav allocation failed ");
							 return NULL;
						 }
						 vn->context[0] = 0;
						 for (i=1;i<vn->nestingLevel;i++){
							 vn->context[i] = -1;
						 }
						 //vn->currentOffset = 0;
						 Try{
							 vn->contextBuf = createContextBuffer2(10, vn->nestingLevel+9);
							 vn->contextBuf2 = createContextBuffer2(10,vn->nestingLevel+9);
						 }Catch(e){
							 freeContextBuffer(vn->contextBuf);
							 freeContextBuffer(vn->contextBuf2);
							 //free(vn->stackTemp);
							 free(vn->context);
							 free(vn);
							 throwException2(out_of_mem,							 
								 "VTDNav allocation failed ");
							 return NULL;
						 }

						 vn->stackTemp = (int *)malloc((vn->nestingLevel+9)*sizeof(int));

						 if (vn->contextBuf == NULL 
							 || vn->stackTemp == NULL){
								 freeContextBuffer(vn->contextBuf);
								 freeContextBuffer(vn->contextBuf2);
								 free(vn->stackTemp);
								 free(vn->context);
								 free(vn);
								 throwException2(out_of_mem,							 
									 "VTDNav allocation failed ");
								 return NULL;
						 }
						 vn->l1index = vn->l2index = vn->l3index = -1;
						 vn->l2lower = vn->l2upper = -1;
						 vn->l3lower = vn->l3upper = -1;
						 vn->docOffset = so;
						 vn->docLen = len;
						 vn->vtdSize = vtd->size;
						 vn->bufLen = xLen;
						 vn->br = br;
						 vn->LN = 0;
						 return vn;
}

//Free VTDNav object
//it doesn't free the memory block containing XML doc
void freeVTDNav(VTDNav *vn)
{	
	if (vn!=NULL){
		freeContextBuffer(vn->contextBuf);
		freeContextBuffer(vn->contextBuf2);
		if (vn->br == FALSE && vn->master){
			freeFastLongBuffer(vn->vtdBuffer);
			freeFastLongBuffer(vn->l1Buffer);
			freeFastLongBuffer(vn->l2Buffer);
			freeFastIntBuffer(vn->l3Buffer);
		}
		free(vn->context);
		free(vn->stackTemp);
		//free(vn->XMLDoc);
		free(vn);
	}						 
}

//Return the attribute count of the element at the cursor position.
int getAttrCount(VTDNav *vn){
	int count = 0;
	int index;
	if(vn->context[0]==-1) return 0;
	index= getCurrentIndex(vn) + 1;
	while (index < vn->vtdSize) {
		int type = getTokenType(vn,index);
		if (type == TOKEN_ATTR_NAME
			|| type == TOKEN_ATTR_VAL
			|| type == TOKEN_ATTR_NS) {
				if (type == TOKEN_ATTR_NAME
					|| (!vn->ns && (type == TOKEN_ATTR_NS))) {
						count++;
				}
		} else
			break;
		index++;
	}
	return count;
}

//Get the token index of the attribute value given an attribute name.     
int getAttrVal(VTDNav *vn, UCSChar *an){
	//int size = vn->vtdBuffer->size;
	int index;
	tokenType type;
	if (vn->context[0] ==-1)
		return -1;

	index = (vn->context[0] != 0) ? vn->context[vn->context[0]] + 1 : vn->rootIndex + 1;


	if(index<vn->vtdSize)
		type = getTokenType(vn,index);
	else 
		return -1;

	if (vn->ns == FALSE) {
		while ((type == TOKEN_ATTR_NAME || type == TOKEN_ATTR_NS)) {
			if (matchRawTokenString(vn,index,
				an)) { // ns node visible only ns is disabled
					return index + 1;
			}
			index += 2;
			if (index>=vn->vtdSize)
				return -1;
			type = getTokenType(vn,index);
		}
	} else {
		while ((type == TOKEN_ATTR_NAME || type == TOKEN_ATTR_NS)) {
			if (type == TOKEN_ATTR_NAME
				&& matchRawTokenString(vn,
				index,
				an)) { // ns node visible only ns is disabled
					return index + 1;
			}
			index += 2;
			if (index>=vn->vtdSize)
				return -1;
			type = getTokenType(vn,index);
		}
	}
	return -1;
}

//Get the token index of the attribute value of given URL and local name.
//If ns is not enabled, the lookup will return -1, indicating a no-found.
//Also namespace nodes are invisible using this method.
int getAttrValNS(VTDNav *vn, UCSChar* URL, UCSChar *ln){

	int size, index;
	tokenType type;
	if (vn->context[0]==-1)
		return -1;
	if (URL!=NULL && wcslen(URL)==0)
        	URL = NULL;
	if (vn->ns == FALSE)
		return -1;
	if (URL == NULL)
		return getAttrVal(vn,ln);
	size = vn->vtdBuffer->size;
	index = (vn->context[0] != 0) ? vn->context[vn->context[0]] + 1 : vn->rootIndex + 1;
	// point to the token next to the element tag

	if(index<vn->vtdSize)
		type = getTokenType(vn,index);
	else 
		return -1;

	while ((type == TOKEN_ATTR_NAME || type == TOKEN_ATTR_NS)) {
		int i = getTokenLength(vn, index);
		int offset = getTokenOffset(vn, index);
		int preLen = (i >> 16) & 0xffff;
		int fullLen = i & 0xffff;
		if (preLen != 0
			// attribute name without a prefix is not bound to any namespaces
			&& matchRawTokenString1(vn,
			offset + preLen + 1,
			fullLen - preLen - 1,
			ln)
			&& resolveNS2(vn, URL, offset, preLen)) {
				return index + 1;
		}else if (	preLen==3 
    				&& matchRawTokenString1(vn,offset, 3,L"xml")
    				&& wcscmp(URL,L"http://www.w3.org/XML/1998/namespace")){
    			// prefix matches "xml"
    			return index+1;
    		}
		index += 2;
		if (index >=vn->vtdSize)
			return -1;
		type = getTokenType(vn,index);
	}
	return -1;
}
//This function decodes the underlying byte array into corresponding 
//UCS2 char representation .
//It doesn't resolves built-in entity and character references.
//Length will never be zero
static Long getChar(VTDNav *vn,int offset){
	Long temp = 0;
	switch (vn->encoding) {
			case FORMAT_ASCII : 
			case FORMAT_ISO_8859_1 :			
				temp = vn->XMLDoc[offset];
				if (temp == '\r') {
					if (vn->XMLDoc[offset + 1] == '\n') {
						return '\n'|(2LL<<32);
					} else {
						return '\n'|(1LL<<32);
					}
				}   
				return temp|(1LL<<32);

			case FORMAT_UTF8 :
				temp = vn->XMLDoc[offset];
				if (temp<=127){
					if (temp == '\r') {
						if (vn->XMLDoc[offset + 1] == '\n') {
							return '\n'|(2LL<<32);
						} else {
							return '\n'|(1LL<<32);
						}
					}
					return (temp|(1LL<<32));
				}
				return handle_utf8(vn,temp,offset);

			case FORMAT_UTF_16BE :
				return handle_utf16be(vn, offset);

			case FORMAT_UTF_16LE :
				return handle_utf16le(vn,offset);
				// implement UTF-16LE to UCS4 conversion

				//System.out.println("UTF 16 LE unimplemented for now");

			default :
				return getChar4OtherEncoding(vn,offset);

	}


}


//This method decodes the underlying byte array into corresponding 
//UCS2 char representation .
//Also it resolves built-in entity and character references.
static Long getCharResolved(VTDNav *vn,int offset){
	int ch = 0;
	int val = 0;
	Long inc =2;
	Long l = getChar(vn,offset);
	ch = (int)l;

	if (ch != '&')
		return l;


	// let us handle references here
	//currentOffset++;
	offset++;
	ch = getCharUnit(vn,offset);
	offset++;
	switch (ch) {
			case '#' :

				ch = getCharUnit(vn,offset);

				if (ch == 'x') {
					while (TRUE) {
						offset++;
						inc++;
						ch = getCharUnit(vn,offset);

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
							throwException(nav_exception,0,
								"navigation exception during getCharResolved",
								"Illegal char in a char reference");
						}
						//throw new NavException("Illegal char in a char reference");
					}
				} else {
					while (TRUE) {

						ch = getCharUnit(vn,offset);
						offset++;
						inc++;
						if (ch >= '0' && ch <= '9') {
							val = val * 10 + (ch - '0');
						} else if (ch == ';') {
							break;
						} else{
							throwException(nav_exception,0,
								"navigation exception during getCharResolved",
								"Illegal char in a char reference");
						}
					}
				}
				break;

			case 'a' :
				ch = getCharUnit(vn, offset);
				if (ch == 'm') {
					if (getCharUnit(vn, offset + 1) == 'p'
						&& getCharUnit(vn, offset + 2) == ';') {
							inc = 5;
							val = '&';
					} else{
						throwException(nav_exception,0,
							"navigation exception during getCharResolved",
							"illegal builtin reference");
					}
					//	throw new NavException("illegal builtin reference");
				} else if (ch == 'p') {
					if (getCharUnit(vn,offset + 1) == 'o'
						&& getCharUnit(vn,offset + 2) == 's'
						&& getCharUnit(vn,offset + 3) == ';') {
							inc = 6;
							val = '\'';
					} else{
						throwException(nav_exception,0,
							"navigation exception during getCharResolved",
							"illegal builtin reference");
					}
					//throw new NavException("illegal builtin reference");
				} else{
					throwException(nav_exception,0,
						"navigation exception during getCharResolved",
						"illegal builtin reference");
				}
				//	throw new NavException("illegal builtin reference");
				break;

			case 'q' :

				if (getCharUnit(vn,offset) == 'u'
					&& getCharUnit(vn,offset + 1) == 'o'
					&& getCharUnit(vn,offset + 2) == 't'
					&& getCharUnit(vn,offset + 3) == ';') {
						inc = 6;
						val = '\"';
				} else{
					throwException(nav_exception,0,
						"navigation exception during getCharResolved",
						"illegal builtin reference");
				}
				//throw new NavException("illegal builtin reference");
				break;
			case 'l' :
				if (getCharUnit(vn,offset) == 't'
					&& getCharUnit(vn,offset + 1) == ';') {
						inc = 4;
						val = '<';
				} else{
					throwException(nav_exception,0,
						"navigation exception during getCharResolved",
						"illegal builtin reference");
				}
				//throw new NavException("illegal builtin reference");
				break;
			case 'g' :
				if (getCharUnit(vn,offset) == 't'
					&& getCharUnit(vn,offset + 1) == ';') {
						inc = 4;
						val = '>';
				} else{
					throwException(nav_exception,0,
						"navigation exception during getCharResolved",
						"illegal builtin reference");
				}
				break;

			default :

				throwException(nav_exception,0,
					"navigation exception during getCharResolved",
					"Invalid entity char");
	}
	return val | (inc << 32);
}



/*Get the next char unit which gets decoded automatically*/
static int getCharUnit(VTDNav *vn, int offset){
	return (vn->encoding <=FORMAT_UTF8)
		? vn->XMLDoc[offset]:
	(vn->encoding <= FORMAT_WIN_1258)
		? decode(vn,offset)
		: ((vn->encoding == FORMAT_UTF_16BE)
		? (vn->XMLDoc[offset << 1]
	<< 8 | vn->XMLDoc[(offset << 1) + 1])
		: (vn->XMLDoc[(offset << 1) + 1]
	<< 8 | vn->XMLDoc[offset << 1]));
}

// Get the starting offset and length of an element fragment
// encoded in a long, upper 32 bit is length; lower 32 bit is offset
Long getElementFragment(VTDNav *vn){
	// a little scanning is needed
	// has next sibling case
	// if not
	int depth = getCurrentDepth(vn);
	int so; 
	int length = 0;
	int temp;
	int size, so2, d, i;

	if (depth == -1)
		return ((Long)vn->docLen)<<32 | vn->docOffset;

	if (depth == -1){
		    int i=lower32At(vn->vtdBuffer,0);
		    if (i==0)
		        return ((Long)vn->docLen)<<32| vn->docOffset;
		    else
		        return ((Long)(vn->docLen-32))| 32;
	}
	// for an element with next sibling
	so = getTokenOffset(vn,getCurrentIndex2(vn)) - 1;
	if (toElement(vn,NEXT_SIBLING)) {

		int temp = getCurrentIndex(vn);
		int so2;
		// rewind 
		while (getTokenDepth(vn,temp) < depth) {
			temp--;
		}
		//temp++;
		so2 = getTokenOffset(vn,temp) - 1;
		// look for the first '>'
		while (getCharUnit(vn,so2) != '>') {
			so2--;
		}
		length = so2 - so + 1;
		toElement(vn, PREV_SIBLING);
		if (vn->encoding <= FORMAT_WIN_1258)
			return ((Long) length) << 32 | so;
		else
			return ((Long) length) << 33 | (so << 1);
	}

	// for root element
	if (depth == 0) {
		int temp = vn->vtdBuffer->size - 1;
		Boolean b = FALSE;
		int so2 = 0;
		while (getTokenDepth(vn,temp) == -1) {
			temp--; // backward scan
			b = TRUE;
		}
		if (b == FALSE)
			so2 =
			(vn->encoding <= FORMAT_WIN_1258)
			? (vn->docOffset + vn->docLen - 1)
			: ((vn->docOffset + vn->docLen) >> 1) - 1;
		else
			so2 = getTokenOffset(vn,temp + 1);
		while (getCharUnit(vn,so2) != '>') {
			so2--;
		}
		length = so2 - so + 1;
		if (vn->encoding <= FORMAT_WIN_1258)
			return ((Long) length) << 32 | so;
		else
			return ((Long) length) << 33 | (so << 1);
	}
	// for a non-root element with no next sibling
	temp = getCurrentIndex(vn) + 1;
	size = vn->vtdBuffer->size;
	// temp is not the last entry in VTD buffer
	if (temp < size - 1) {
		while (temp < size && getTokenDepth(vn,temp) >= depth) {
			temp++;
		}
		if (temp != size) {
			int d =
				depth
				- getTokenDepth(vn,temp)
				+ ((getTokenType(vn, temp) == TOKEN_STARTING_TAG) ? 1 : 0);
			int so2 = getTokenOffset(vn,temp) - 1;
			int i = 0;
			// scan backward
			while (i < d) {
				if (getCharUnit(vn,so2) == '>')
					i++;
				so2--;
			}
			length = so2 - so + 2;
			if (vn->encoding <= FORMAT_WIN_1258)
				return ((Long) length) << 32 | so;
			else
				return ((Long) length) << 33 | (so << 1);
		}
	}
	// temp is the last entry
	// scan forward search for /> or </cc>
	so2 =
		(vn->encoding <= FORMAT_WIN_1258)
		? (vn->docOffset + vn->docLen - 1)
		: ((vn->docOffset + vn->docLen) >> 1) - 1;
	d = depth + 1;
	i = 0;
	while (i < d) {
		if (getCharUnit(vn,so2) == '>') {
			i++;
		}
		so2--;
	}

	length = so2 - so + 2;

	if (vn->encoding <= FORMAT_WIN_1258)
		return ((Long) length) << 32 | so;
	else
		return ((Long) length) << 33 | (so << 1);
}


// Get the starting offset and length of an element content 
// ie. everything warpped around the starting and ending tags of the 
// cursor element, return -1 if the cursor element is an empty element
// 
Long getContentFragment(VTDNav *vn){
	int so,length,so2,temp,size,d,i;
		int depth = getCurrentDepth(vn);
//		 document length and offset returned if depth == -1
		if (depth == -1){
		    int i=lower32At(vn->vtdBuffer,0);
		    if (i==0)
		        return ((Long)vn->docLen)<<32| vn->docOffset;
		    else
		        return ((Long)(vn->docLen-32))| 32;
		}

		
		so = getOffsetAfterHead(vn);
		if (so==-1)
			return -1L;
		length = 0;
		

		// for an element with next sibling
		if (toElement(vn,NEXT_SIBLING)) {

			int temp = getCurrentIndex(vn);
			// rewind
			while (getTokenDepth(vn,temp) < depth) {
				temp--;
			}
			//temp++;
			so2 = getTokenOffset(vn,temp) - 1;
			// look for the first '>'
			while (getCharUnit(vn,so2) != '>') {
				so2--;
			}
			while (getCharUnit(vn,so2) != '/') {
				so2--;
			}
			while (getCharUnit(vn,so2) != '<') {
				so2--;
			}
			length = so2 - so;
			toElement(vn,PREV_SIBLING);
			if (vn->encoding <= FORMAT_WIN_1258)
				return ((Long) length) << 32 | so;
			else
				return ((Long) length) << 33 | (so << 1);
		}

		// for root element
		if (depth == 0) {
			int temp = vn->vtdBuffer->size - 1;
			Boolean b = FALSE;
			int so2 = 0;
			while (getTokenDepth(vn,temp) == -1) {
				temp--; // backward scan
				b = TRUE;
			}
			if (b == FALSE)
				so2 =
					(vn->encoding <= FORMAT_WIN_1258 )
						? (vn->docOffset + vn->docLen - 1)
						: ((vn->docOffset + vn->docLen) >> 1) - 1;
			else
				so2 = getTokenOffset(vn,temp + 1);
			while (getCharUnit(vn,so2) != '>') {
				so2--;
			}
			while (getCharUnit(vn,so2) != '/') {
				so2--;
			}
			while (getCharUnit(vn,so2) != '<') {
				so2--;
			}
			length = so2 - so;
			if (vn->encoding <= FORMAT_WIN_1258)
				return ((Long) length) << 32 | so;
			else
				return ((Long) length) << 33 | (so << 1);
		}
		// for a non-root element with no next sibling
		temp = getCurrentIndex(vn) + 1;
		size = vn->vtdBuffer->size;
		// temp is not the last entry in VTD buffer
		if (temp < size) {
			while (temp < size && getTokenDepth(vn,temp) >= depth) {
				temp++;
			}
			if (temp != size) {
				int d =
					depth
						- getTokenDepth(vn,temp)
						+ ((getTokenType(vn,temp) == TOKEN_STARTING_TAG) ? 1 : 0);
				int so2 = getTokenOffset(vn,temp) - 1;
				int i = 0;
				// scan backward
				while (i < d) {
					if (getCharUnit(vn,so2) == '>')
						i++;
					so2--;
				}
				while (getCharUnit(vn,so2) != '/') {
					so2--;
				}
				while (getCharUnit(vn,so2) != '<') {
					so2--;
				}
				length = so2 - so;
				if (vn->encoding <= FORMAT_WIN_1258)
					return ((Long) length) << 32 | so;
				else
					return ((Long) length) << 33 | (so << 1);
			}
			/*
             * int so2 = getTokenOffset(temp - 1) - 1; int d = depth -
             * getTokenDepth(temp - 1); int i = 0; while (i < d) { if
             * (getCharUnit(so2) == '>') { i++; } so2--; } length = so2 - so +
             * 2; if (encoding < 3) return ((long) length) < < 32 | so; else
             * return ((long) length) < < 33 | (so < < 1);
             */
		}
		// temp is the last entry
		// scan forward search for /> or </cc>
		
		so2 =
			(vn->encoding <= FORMAT_WIN_1258)
				? (vn->docOffset + vn->docLen - 1)
				: ((vn->docOffset + vn->docLen) >> 1) - 1;
			   
	    d = depth + 1;
	    
	    i = 0;
        while (i < d) {
            if (getCharUnit(vn,so2) == '>') {
                i++;
            }
            so2--;
        }
        while (getCharUnit(vn,so2) != '/') {
			so2--;
		}
		while (getCharUnit(vn,so2) != '<') {
			so2--;
		}

		length = so2 - so;

		if (vn->encoding <= FORMAT_WIN_1258)
			return ((Long) length) << 32 | so;
		else
			return ((Long) length) << 33 | (so << 1);
}
// This function returns of the token index of the type character data or CDATA.
// Notice that it is intended to support data orient XML (not mixed-content XML).
int getText(VTDNav *vn){
	if (vn->context[0]==-1)
		return -1;
	else {
		int index = (vn->context[0] != 0) ? 
			vn->context[vn->context[0]] + 1 : vn->rootIndex + 1;
		int depth = getCurrentDepth(vn);
		tokenType type;
		if (index<vn->vtdSize)
			type= getTokenType(vn,index);
		else
			return -1;

		while (TRUE) {
			if (type == TOKEN_CHARACTER_DATA || type == TOKEN_CDATA_VAL) {
				if (depth == getTokenDepth(vn,index))
					return index;
				else
					return -1;
			} else if (type == TOKEN_ATTR_NS || type == TOKEN_ATTR_NAME) {
				index += 2; // assuming a single token for attr val
			} else if (
				type == TOKEN_PI_NAME
				|| type == TOKEN_PI_VAL
				|| type == TOKEN_COMMENT) {
					if (depth == getTokenDepth(vn,index)) {
						index += 1;
					} else
						return -1;
			} else
				return -1;
			if (index >= vn->vtdSize) 
				break;
			type = getTokenType(vn, index);
		}
		return -1;
	}
}

//Get the depth value of a token (>=0)
int getTokenDepth(VTDNav *vn, int index){
	int i;
	i = (int) ((longAt(vn->vtdBuffer,index) & MASK_TOKEN_DEPTH) >> 52);

	if (i != 255)
		return i;
	return -1;
}

//Get the token length at the given index value
//please refer to VTD spec for more details
int getTokenLength(VTDNav *vn, int index){
	Long i = 0;
	//int j=0;
	int depth;
	int len = 0;
	int type = getTokenType(vn,index), temp=0;
	//int val;
	switch (type) {
			case TOKEN_ATTR_NAME :
			case TOKEN_ATTR_NS :
			case TOKEN_STARTING_TAG :
				i = longAt(vn->vtdBuffer, index);

				return (vn->ns == FALSE)
					? (int) ((i & MASK_TOKEN_QN_LEN) >> 32)
					: ((int) ((i & MASK_TOKEN_QN_LEN)
					>> 32)
					| ((int) ((i & MASK_TOKEN_PRE_LEN)
					>> 32)
					<< 5));

				break;
			case TOKEN_CHARACTER_DATA:
			case TOKEN_CDATA_VAL:
			case TOKEN_COMMENT: // make sure this is total length
				depth = getTokenDepth(vn,index);
				do{

					len = len +  (int)
						((longAt(vn->vtdBuffer, index) 
						& MASK_TOKEN_FULL_LEN) >> 32);
					temp =  getTokenOffset(vn, index)+(int)
						((longAt(vn->vtdBuffer,index)& MASK_TOKEN_FULL_LEN) >> 32);
					index++;						
				}
				while(index < vn->vtdSize && depth == getTokenDepth(vn,index) 
					&& type == getTokenType(vn,index) && temp == getTokenOffset(vn,index));
				//if (int k=0)
				return len;
			default :

				return (int)
					((longAt(vn->vtdBuffer,index) & MASK_TOKEN_FULL_LEN) >> 32);

				break;
	}
}



//Test whether current element has an attribute with the matching name.
Boolean hasAttr(VTDNav *vn, UCSChar *an){
	tokenType type;

	int size = vn->vtdBuffer->size;
	int index = (vn->context[0] != 0) ? vn->context[vn->context[0]] + 1 : vn->rootIndex + 1;

	if (vn->context[0]==-1) 
		return FALSE;

	if (index >= size)
		return FALSE;

	type = getTokenType(vn,index);
	if (vn->ns == FALSE) {
		if (wcscmp(an,L"*")==0) {
			if (type == TOKEN_ATTR_NAME || type == TOKEN_ATTR_NS)
				return TRUE;
			else
				return FALSE;
		} else {
			while (index < size
				&& (type == TOKEN_ATTR_NAME
				|| type == TOKEN_ATTR_NS)) { // ns tokens becomes visible
					if (matchRawTokenString(vn,index, an))
						return TRUE;
					index += 2;
					type = getTokenType(vn,index);
			}
			return FALSE;
		}
	} else {
		if (wcscmp(an,L"*")==0) {
			while (index < size
				&& (getTokenType(vn,index) == TOKEN_ATTR_NAME
				|| getTokenType(vn,index) == TOKEN_ATTR_NS)) {
					if (type == TOKEN_ATTR_NAME)
						// TOKEN_ATTR_NS is invisible when ns == true
						return TRUE;
					index += 2;
					type = getTokenType(vn,index);
			}
			return FALSE;

		} else {
			while (index < size
				&& (type == TOKEN_ATTR_NAME || type == TOKEN_ATTR_NS)) {
					if (type == TOKEN_ATTR_NAME
						&& matchRawTokenString(vn,index, an))
						return TRUE;
					index += 2;
					type = getTokenType(vn,index);
			}
			return FALSE;
		}
	}
}

//Test whether the current element has an attribute with 
//matching namespace URL and localname.
Boolean hasAttrNS(VTDNav *vn, UCSChar *URL, UCSChar *localName){
	if (vn->context[0]==-1)
		return FALSE;
	return (getAttrValNS(vn,URL, localName) != -1);
}

//Test the token type, to see if it is a starting tag.
static inline Boolean isElement(VTDNav  *vn, int index){
	return (((longAt(vn->vtdBuffer,index) & MASK_TOKEN_TYPE) >> 60) & 0xf)
		== TOKEN_STARTING_TAG;
}

// Test the token type, to see if it is a starting tag or document token
static inline Boolean isElementOrDocument(VTDNav  *vn, int index){
	int i = 0;
 	i= (int)(((longAt(vn->vtdBuffer,index) & MASK_TOKEN_TYPE) >> 60) & 0xf);
	return (i == TOKEN_STARTING_TAG || i == TOKEN_DOCUMENT); 
}

//Test whether ch is a white space character or not.
static inline Boolean isWS(int ch){
	return (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r');
}
//This method is similar to getElementByName in DOM except it doesn't
//return the nodeset, instead it iterates over those nodes.
int iterate(VTDNav *vn, int dp, UCSChar *en,Boolean special){

	int index = getCurrentIndex(vn) + 1;
	tokenType tt;
	//int size = vtdBuffer.size();
	while (index < vn->vtdSize) {
		tt = getTokenType(vn,index);
		if (tt == TOKEN_ATTR_NAME ||
			tt == TOKEN_ATTR_NS ){
				index +=2;
				continue;
		}

		if (isElementOrDocument(vn,index)) {
			int depth = getTokenDepth(vn,index);
			if (depth > dp) {
				vn->context[0] = depth;
				if (depth > 0)
					vn->context[depth] = index;

				if (special || matchElement(vn, en)) {
					if (dp < 4)
						resolveLC(vn);
					return TRUE;
				}
			} else {
				return FALSE;
			}
		}
		index++;
	}
	return FALSE;
}

//This method is similar to getElementByName in DOM except it doesn't
//return the nodeset, instead it iterates over those nodes .
//When URL is "*" it will match any namespace
//if ns is false, return false immediately
int iterateNS(VTDNav *vn, int dp, UCSChar *URL, UCSChar *ln){
	int index;
	tokenType tt;
	if (vn->ns == FALSE)
		return FALSE;

	index = getCurrentIndex(vn) + 1;
	while (index < vn->vtdSize) {
		tt = getTokenType(vn,index);
		if (tt == TOKEN_ATTR_NAME ||
			tt == TOKEN_ATTR_NS ){
				index +=2;
				continue;
		}
		if (isElement(vn,index)) {
			int depth = getTokenDepth(vn,index);
			if (depth > dp) {
				vn->context[0] = depth;
				if (depth>0)
					vn->context[depth] = index;
				if (matchElementNS(vn,URL, ln)) {
					if (dp < 4)
						resolveLC(vn);
					return TRUE;
				}
			} else {
				return FALSE;
			}
		}
		index++;

	}
	return FALSE;
}


// This function is called by selectElement_P in autoPilot
Boolean iterate_preceding(VTDNav *vn,UCSChar *en, int* a, Boolean special){
	int index = getCurrentIndex(vn) - 1;
	int t,d,i;
	//int depth = getTokenDepth(index);
	//int size = vtdBuffer.size();
	while (index >  0) {
		if (isElementOrDocument(vn,index)) {
			int depth = getTokenDepth(vn,index);
			vn->context[0] = depth;
			//context[depth]=index;
			if (depth>0){
				vn->context[depth] = index;
				t = index -1;
				for (i=depth-1;i>0;i--){
					if (vn->context[i]>index || vn->context[i] == -1){
						while(t>0){
							d = getTokenDepth(vn,t);
							if ( d == i && isElement(vn,t)){
								vn->context[i] = t;
								break;
							}
							t--;
						}							
					}else
						break;
				}
			}
			//dumpContext();
			if (index!= a[depth] && (special || matchElement(vn,en))) {					
				if (depth <4)				
					resolveLC(vn);
				return TRUE;
			}
		} 
		index--;
	}
	return FALSE;	
}

// This function is called by selectElementNS_P in autoPilot
Boolean iterate_precedingNS(VTDNav *vn,UCSChar *URL, UCSChar *ln, int* a){
	int index = getCurrentIndex(vn) - 1;
	int t,d,i;
	//int depth = getTokenDepth(index);
	//int size = vtdBuffer.size();
	while (index > 0 ) {
		if (isElementOrDocument(vn,index)) {
			int depth = getTokenDepth(vn,index);
			vn->context[0] = depth;
			//context[depth]=index;
			if (depth>0){
				vn->context[depth] = index;
				t = index -1;
				for (i=depth-1;i>0;i--){
					if (vn->context[i]>index || vn->context[i]==-1){
						while(t>0){
							d = getTokenDepth(vn,t);
							if ( d == i && isElement(vn,t)){
								vn->context[i] = t;
								break;
							}
							t--;
						}							
					}else
						break;
				}
			}
			//dumpContext();
			if (index != a[depth] && matchElementNS(vn,URL,ln)) {					
				if (depth <4)
					resolveLC(vn);
				return TRUE;
			}
		} 
		index--;
	}
	return FALSE;	
}


// This function is called by selectElement_F in autoPilot

Boolean iterate_following(VTDNav *vn,UCSChar *en, Boolean special){
	int index = getCurrentIndex(vn) + 1;
	//int size = vtdBuffer.size();
	while (index < vn->vtdSize) {
		if (isElementOrDocument(vn,index)) {
			int depth = getTokenDepth(vn,index);
			vn->context[0] = depth;
			if (depth>0)
				vn->context[depth] = index;
			if (special || matchElement(vn,en)) {					
				if (depth <4)
					resolveLC(vn);
				return TRUE;
			}
		} 
		index++;
	}
	return FALSE;
}


// This function is called by selectElementNS_F in autoPilot

Boolean iterate_followingNS(VTDNav *vn, UCSChar *URL, UCSChar *ln){
	int index = getCurrentIndex(vn) + 1;
	//int size = vtdBuffer.size();
	while (index < vn->vtdSize) {
		if (isElementOrDocument(vn,index)) {
			int depth = getTokenDepth(vn,index);
			vn->context[0] = depth;
			if (depth>0)
				vn->context[depth] = index;
			if (matchElementNS(vn,URL,ln)) {					
				if (depth <4)
					resolveLC(vn);
				return TRUE;
			}
		} 
		index++;
	}
	return FALSE;
}



//Test if the current element matches the given name.
Boolean matchElement(VTDNav *vn, UCSChar *en){
	/*if (en == NULL){
	throwException2(invalid_argument,
	"matchElement's element name can't be null");
	}*/

	// throw new IllegalArgumentException(" Element name can't be null ");
	if (wcscmp(en,L"*") == 0 && vn->context[0] !=-1)
		return TRUE;
	if (vn->context[0]==-1)
		return FALSE;
	return matchRawTokenString(vn,
		(vn->context[0] == 0) ? vn->rootIndex : vn->context[vn->context[0]],
		en);
}

//Test whether the current element matches the given namespace URL and localname.
//URL, when set to "*", matches any namespace (including null), when set to null, defines a "always-no-match"
//ln is the localname that, when set to *, matches any localname
Boolean matchElementNS(VTDNav *vn, UCSChar *URL, UCSChar *ln){
	if (vn->context[0]==-1)
		return FALSE;
	else {
		int i =
			getTokenLength(vn, (vn->context[0] != 0) ? vn->context[vn->context[0]] : vn->rootIndex);
		int offset =
			getTokenOffset(vn, (vn->context[0] != 0) ? vn->context[vn->context[0]] : vn->rootIndex);
		int preLen = (i >> 16) & 0xffff;
		int fullLen = i & 0xffff;
		if (URL!=NULL && wcslen(URL)==0)
        	URL = NULL;
		if (wcscmp(ln, L"*")== 0
			|| ((preLen != 0)
			? matchRawTokenString1(vn,
			offset + preLen + 1,
			fullLen - preLen - 1,
			ln)
			: matchRawTokenString1(vn,
			offset,
			fullLen,
			ln))) { // no prefix, search for xmlns
				if (((URL != NULL) ? wcscmp(URL,L"*")==0 : FALSE)
					|| (resolveNS2(vn, URL, offset, preLen) == TRUE))
					return TRUE;

				if ( preLen==3 
    			&& matchRawTokenString1(vn,offset, preLen,L"xml")
    			&& wcscmp(URL,L"http://www.w3.org/XML/1998/namespace"))
    			return TRUE;   
		}
		return FALSE;
	}
}

//Match a string against a token with given offset and len, entities 
//doesn't get resolved.
static Boolean matchRawTokenString1(VTDNav *vn, int offset, int len, UCSChar *s){
  return  compareRawTokenString2(vn,offset,len,s)==0;
}

//Match a string with a token represented by a long (upper 32 len, lower 32 offset).
static Boolean matchRawTokenString2(VTDNav *vn, Long l, UCSChar *s){
	int len;
	int offset;
	/*if (s == NULL){
	throwException2(invalid_argument,
	" invalid argument for matchRawTokenString2, s can't be NULL");
	}*/
	//throw new IllegalArgumentException("string can't be null");
	len = (int) ((l & MASK_TOKEN_FULL_LEN) >> 32);
	// a little hardcode is always bad
	offset = (int) l;
	return compareRawTokenString2(vn, offset, len, s)==0;
}

//Match the string against the token at the given index value. When a token
//is an attribute name or starting tag, qualified name is what gets matched against
Boolean matchRawTokenString(VTDNav *vn, int index, UCSChar *s){	
	tokenType type;
	int len;
	int offset;
	/*if (s == NULL){
	//	 throwException2(invalid_argument,
	//		 " invalid argument for matchRawTokenString, s can't be NULL");
	}*/
	type = getTokenType(vn,index);
	len =
		(type == TOKEN_STARTING_TAG
		|| type == TOKEN_ATTR_NAME
		|| type == TOKEN_ATTR_NS)
		? getTokenLength(vn, index) & 0xffff
		: getTokenLength(vn, index);
	// upper 16 bit is zero or for prefix

	offset = getTokenOffset(vn, index);
	// point currentOffset to the beginning of the token
	// for UTF 8 and ISO, the performance is a little better by avoid calling getChar() everytime
	return compareRawTokenString2(vn, offset, len, s)==0;
}

//Match a string against a token with given offset and len, entities get 
//resolved properly.
static Boolean matchTokenString1(VTDNav *vn, int offset, int len, UCSChar *s){
	int endOffset;
	int l;
	Long l1;
	if (s == NULL){ 
		throwException2(invalid_argument,
			" invalid argument for matchRawTokenString1, s can't be NULL");
	}
	//throw new IllegalArgumentException("string can't be null");

	//vn->currentOffset = offset;
	endOffset = offset + len;

	if (vn->encoding < FORMAT_UTF8) {
		int i = 0;
		l = (int)wcslen(s);
		for (i = 0; i < l && offset < endOffset; i++) {
			if ((vn->XMLDoc[offset] & 0xff) != '&') {
				if (s[i] != (vn->XMLDoc[offset] & 0xff))
					return FALSE;
				offset++;
			} else {
				l1 = getCharResolved(vn,offset);
				if (s[i] != (int)l1) {
					return FALSE;
				}
				offset += (int)(l1>>32);
			}
		}
		if (i == l && offset == endOffset)
			return TRUE;
		else
			return FALSE;
	} else {
		int i = 0;
		l = (int)wcslen(s);
		for (i = 0; i < l && offset < endOffset; i++) {
			l1 = getCharResolved(vn,offset);
			offset += (int)(l1>>32);
			if (s[i] != (int)l1) {
				return FALSE;
			}
		}
		if (i == l && offset == endOffset)
			return TRUE;
		else
			return FALSE;
	} //return TRUE;
}

//Match a string against a "non-extractive" token represented by a 
//long (upper 32 len, lower 32 offset).
static Boolean matchTokenString2(VTDNav *vn, Long l, UCSChar *s){
	int len,offset;
	if (s == NULL){
		throwException2(invalid_argument,
			" invalid argument for matchTokenString2, s can't be NULL");
	}
	//	 throw new IllegalArgumentException("string can't be null");

	len = (int) ((l & MASK_TOKEN_FULL_LEN) >> 32);
						 // a little hardcode is always bad
	offset = (int) l;
	return matchRawTokenString1(vn,offset, len, s);
}


Boolean matchTokens(VTDNav *vn1, int i1, VTDNav *vn2, int i2){
	return compareTokens(vn1,i1,vn2,i2)==0;
}
//Match the string against the token at the given index value. When a token
//is an attribute name or starting tag, qualified name is what gets matched against
Boolean matchTokenString(VTDNav *vn, int index, UCSChar *s){
	tokenType type;
	int offset;
	int len;
	if (s == NULL){
		throwException2(invalid_argument,
			" invalid argument for matchTokenString, s can't be NULL");
	}
	type = getTokenType(vn,index);
	len =
		(type == TOKEN_STARTING_TAG
		|| type == TOKEN_ATTR_NAME
		|| type == TOKEN_ATTR_NS)
		? getTokenLength(vn,index) & 0xffff
		: getTokenLength(vn,index);
	// upper 16 bit is zero or for prefix

	offset = getTokenOffset(vn,index);
	// point currentOffset to the beginning of the token
	// for UTF 8 and ISO, the performance is a little better by avoid calling getChar() everytime
	return matchTokenString1(vn,offset, len, s);
}

//Evaluate the namespace indicator in bit 31 and bit 30.
static inline int NSval(VTDNav *vn, int i){
	return (int) (longAt(vn->vtdBuffer,i) & MASK_TOKEN_NS_MARK);
}

//Convert a vtd token into a double.
double parseDouble(VTDNav *vn, int index){
	int ch;
	int end;
	Long l;
	int t = getTokenType(vn,index);
	Boolean b = (t==TOKEN_CHARACTER_DATA) || (t== TOKEN_ATTR_VAL);
	Long left, right, scale, exp;
	double v;
	Boolean neg;
	int offset = getTokenOffset(vn,index);
	end = offset + getTokenLength(vn,index);
	//past the last one by one

	//ch = b?getCharResolved(vn):getChar(vn);
	{
		l = b? getCharResolved(vn,offset):getChar(vn,offset);
		ch = (int)l;
		offset += (int)(l>>32);
	}
	while (offset <= end) { /* trim leading whitespaces*/
		if (!isWS(ch))
			break;
		l = b? getCharResolved(vn,offset):getChar(vn,offset);
		ch = (int)l;
		offset += (int)(l>>32);
	}

	if (offset > end) {/* all whitespace */
		double d1 = 0.0;
		return d1/d1;
	}

	neg = (ch == '-');

	if (ch == '-' || ch == '+')
	{
		l = b? getCharResolved(vn,offset):getChar(vn,offset);
		ch = (int)l;
		offset += (int)(l>>32);
	}

	/*left part of decimal*/
	left = 0;
	while (offset <= end) {
		/*must be <= since we get the next one at last.*/

		int dig = Character_digit((char) ch, 10); /*only consider decimal*/
		if (dig < 0)
			break;

		left = left * 10 + dig;

		{
			l = b? getCharResolved(vn,offset):getChar(vn,offset);
			ch = (int)l;
			offset += (int)(l>>32);
		}
	}

	/*right part of decimal*/
	right = 0;
	scale = 1;
	if (ch == '.') {
		{
			l = b? getCharResolved(vn,offset):getChar(vn,offset);
			ch = (int)l;
			offset += (int)(l>>32);
		}

		while (offset <= end) {
			/*must be <= since we get the next one at last.*/

			int dig = Character_digit((char) ch, 10);
			/*only consider decimal */
			if (dig < 0)
				break;

			right = right * 10 + dig;
			scale *= 10;

			{
				l = b? getCharResolved(vn,offset):getChar(vn,offset);
				ch = (int)l;
				offset += (int)(l>>32);
			}
		}
	}

	/*exponent*/
	exp = 0;
	if (ch == 'E' || ch == 'e') {
		Boolean expneg;
		int cur;
		{
			l = b? getCharResolved(vn,offset):getChar(vn,offset);
			ch = (int)l;
			offset += (int)(l>>32);
		}
		expneg = (ch == '-'); /*sign for exp*/
		if (ch == '+' || ch == '-')
		{
			l = b? getCharResolved(vn,offset):getChar(vn,offset);
			ch = (int)l;
			offset += (int)(l>>32);
		} /*skip the +/- sign*/

		cur = offset;
		/*remember the indx, used to find a invalid number like 1.23E*/

		while (offset <= end) {
			/*must be <= since we get the next one at last.*/

			int dig = Character_digit((char) ch, 10);
			/*only consider decimal*/
			if (dig < 0)
				break;
			exp = exp * 10 + dig;
			{
				l = b? getCharResolved(vn,offset):getChar(vn,offset);
				ch = (int)l;
				offset += (int)(l>>32);
			}
		}
		if (cur == offset){// all whitespace
			double d1 = 0.0;
			return d1/d1;
			//throw new NavException("Empty string");
		}
		if (expneg)
			exp = (-exp);
	}

	//anything left must be space
	while (offset <= end) {
		if (!isWS(ch)){// all whitespace
			double d1 = 0.0;
			return d1/d1;
		}
		// throw new NavException(toString(index));

		{
			l = b? getCharResolved(vn,offset):getChar(vn,offset);
			ch = (int)l;
			offset += (int)(l>>32);
		}
	}

	v = (double) left;
	if (right != 0)
		v += ((double) right) / (double) scale;

	if (exp != 0)
		v = v* pow(10,(double) exp);

	return ((neg) ? (-v) : v);
}

//Convert a vtd token into a float.
float parseFloat(VTDNav *vn, int index){

	Long exp,left, right, scale;
	float f;
	int end, ch;
	int t = getTokenType(vn,index);
	Boolean b = (t==TOKEN_CHARACTER_DATA) || (t== TOKEN_ATTR_VAL);
	Boolean neg;
	double v;
	Long l;
	int offset = getTokenOffset(vn,index);
	end = offset + getTokenLength(vn, index);
	//past the last one by one

	{
		l = b? getCharResolved(vn,offset):getChar(vn,offset);
		ch = (int)l;
		offset += (int)(l>>32);
	}

	while (offset <= end) { // trim leading whitespaces
		if (!isWS(ch))
			break;
		{
			l = b? getCharResolved(vn,offset):getChar(vn,offset);
			ch = (int)l;
			offset += (int)(l>>32);
		}
	}

	if (offset > end){// all whitespace
		float d1 = 0.0;
		return d1/d1;
	}
	//throw new NavException("Empty string");

	neg = (ch == '-');

	if (ch == '-' || ch == '+')
	{
		l = b? getCharResolved(vn,offset):getChar(vn,offset);
		ch = (int)l;
		offset += (int)(l>>32);
	} //get another one if it is sign.

	//left part of decimal
	left = 0;
	while (offset<= end) {
		//must be <= since we get the next one at last.

		int dig = Character_digit((char) ch, 10); //only consider decimal
		if (dig < 0)
			break;

		left = left * 10 + dig;

		{
			l = b? getCharResolved(vn,offset):getChar(vn,offset);
			ch = (int)l;
			offset += (int)(l>>32);
		}
	}

	//right part of decimal
	right = 0;
	scale = 1;
	if (ch == '.') {
		{
			l = b? getCharResolved(vn,offset):getChar(vn,offset);
			ch = (int)l;
			offset += (int)(l>>32);
		}

		while (offset <= end) {
			//must be <= since we get the next one at last.

			int dig = Character_digit((char) ch, 10);
			//only consider decimal
			if (dig < 0)
				break;

			right = right * 10 + dig;
			scale *= 10;

			{
				l = b? getCharResolved(vn,offset):getChar(vn,offset);
				ch = (int)l;
				offset += (int)(l>>32);
			}
		}
	}

	//exponent
	exp = 0;
	if (ch == 'E' || ch == 'e') {
		Boolean expneg;
		int cur;
		{
			l = b? getCharResolved(vn,offset):getChar(vn,offset);
			ch = (int)l;
			offset += (int)(l>>32);
		}
		expneg = (ch == '-'); //sign for exp
		if (ch == '+' || ch == '-')
		{
			l = b? getCharResolved(vn,offset):getChar(vn,offset);
			ch = (int)l;
			offset += (int)(l>>32);
		}//skip the +/- sign

		cur =offset;
		//remember the indx, used to find a invalid number like 1.23E

		while (offset <= end) {
			//must be <= since we get the next one at last.

			int dig = Character_digit((char) ch, 10);
			//only consider decimal
			if (dig < 0)
				break;

			exp = exp * 10 + dig;

			{
				l = b? getCharResolved(vn,offset):getChar(vn,offset);
				ch = (int)l;
				offset += (int)(l>>32);
			}
		}

		if (cur == offset){// all whitespace
			float f = 0;
			return f/f;
		}
		//	 throw new NavException(toString(index));
		//found a invalid number like 1.23E

		if (expneg)
			exp = (-exp);
	}

	/*anything left must be space*/
	while (offset <= end) {
		if (!isWS(ch)){/* all whitespace */
			float f = 0.0;
			return f/f;
		}
		{
			l = b? getCharResolved(vn,offset):getChar(vn,offset);
			ch = (int)l;
			offset += (int)(l>>32);
		}
	}

	v = (double) left;
	if (right != 0)
		v += ((double) right) / (double) scale;

	if (exp != 0)
		v = v * pow(10, (double)exp);


	f = (float) v;

	/*try to handle overflow/underflow */
	if (v >= MAXFLOAT)
		f = MAXFLOAT;
	else if (v <= MINFLOAT)
		f = MINFLOAT;

	if (neg)
		f = -f;
	return f;
}

/*Convert a vtd token into an int*/
int parseInt(VTDNav *vn, int index){
	return parseInt2(vn,index,10);
}

/*Convert a vtd token into an Int according to given radix.*/
static int parseInt2(VTDNav *vn, int index, int radix){
	Long pos, result;
	int endOffset, c;
	Boolean neg;

	Long l;
	int t = getTokenType(vn,index);
	Boolean b = (t==TOKEN_CHARACTER_DATA) || (t== TOKEN_ATTR_VAL);
	int offset = getTokenOffset(vn,index);
	endOffset = offset + getTokenLength(vn,index);

	{
		l = b? getCharResolved(vn,offset):getChar(vn,offset);
		c = (int)l;
		offset += (int)(l>>32);
	}

	// trim leading whitespaces
	while ((c == ' ' || c == '\n' || c == '\t' || c == '\r')
		&& (offset<= endOffset))
	{
		l = b? getCharResolved(vn,offset):getChar(vn,offset);
		c = (int)l;
		offset += (int)(l>>32);
	}

	if (offset > endOffset) {// all whitespace
		throwException2(number_format_exception,
			" empty string for parseInt2");
	}

	neg = (c == '-');
	if (neg || c == '+')
	{
		l = b? getCharResolved(vn,offset):getChar(vn,offset);
		c = (int)l;
		offset += (int)(l>>32);
	}//skip sign

	result = 0;
	pos = 1;
	while (offset <= endOffset) {
		int digit = Character_digit((char) c, radix);
		if (digit < 0)
			break;

		//Note: for binary we can simply shift to left to improve performance
		result = result * radix + digit;
		//pos *= radix;

		{
			l = b? getCharResolved(vn,offset):getChar(vn,offset);
			c = (int)l;
			offset += (int)(l>>32);
		}
	}

	if (result > MAXINT) {// all whitespace
		throwException2(number_format_exception,
			" integer value over(under) flow");
	}
	// throw new NumberFormatException("Overflow: " + toString(index));

	// take care of the trailing
	while (offset <= endOffset && isWS(c)) {
		l = b? getCharResolved(vn,offset):getChar(vn,offset);
		c = (int)l;
		offset += (int)(l>>32);
	}
	if (offset == (endOffset + 1))
		return (int) ((neg) ? (-result) : result);
	else{// all whitespace
		throwException2(number_format_exception,
			" invalid char during parseInt2");
		return -1;
	}
	// throw new NumberFormatException(toString(index));
}

//Convert a vtd token into a long
Long parseLong(VTDNav *vn, int index){
	return parseLong2(vn, index, 10);
}

//Convert a vtd token into a long according to given radix.
static Long parseLong2(VTDNav *vn, int index, int radix){
	int endOffset, c;
	Long result, pos;
	Boolean neg;
	Long l;
	int t = getTokenType(vn,index);
	Boolean b = (t==TOKEN_CHARACTER_DATA) || (t== TOKEN_ATTR_VAL);

	int offset = getTokenOffset(vn, index);
	endOffset = offset + getTokenLength(vn, index);

	{
		l = b? getCharResolved(vn,offset):getChar(vn,offset);
		c = (int)l;
		offset += (int)(l>>32);
	}
	// trim leading whitespaces
	while ((c == ' ' || c == '\n' || c == '\t' || c == '\r')
		&& (offset <= endOffset))
	{
		l = b? getCharResolved(vn,offset):getChar(vn,offset);
		c = (int)l;
		offset += (int)(l>>32);
	}

	if (offset > endOffset) {// all whitespace
		throwException2(number_format_exception,
			" empty string for parseLong2");
	}

	neg = (c == '-');
	if (neg || c == '+')
	{
		l = b? getCharResolved(vn,offset):getChar(vn,offset);
		c = (int)l;
		offset += (int)(l>>32);
	}//skip sign

	result = 0;
	pos = 1;
	while (offset <= endOffset) {
		int digit = Character_digit((char) c, radix);
		if (digit < 0)
			break;

		//Note: for binary we can simply shift to left to improve performance
		result = result * radix + digit;
		//pos *= radix;

		{
			l = b? getCharResolved(vn,offset):getChar(vn,offset);
			c = (int)l;
			offset += (int)(l>>32);
		}
	}

	if (result > MAXLONG) {// all whitespace
		throwException2(number_format_exception,
			" long value over(under) flow");
	}
	// throw new NumberFormatException("Overflow: " + toString(index));

	// take care of the trailing
	while (offset <= endOffset && isWS(c)) {
		{
			l = b? getCharResolved(vn,offset):getChar(vn,offset);
			c = (int)l;
			offset += (int)(l>>32);
		}
	}
	if (offset == (endOffset + 1))
		return  ((neg) ? (-result) : result);
	else{// all whitespace
		throwException2(number_format_exception,
			" invalid char during parseLong2");
		return -1;
	}
	//throw new NumberFormatException(toString(index));
}

//Load the context info from contextBuf.
//Info saved including LC and current state of the context 
Boolean pop(VTDNav *vn){
	Boolean b = load(vn->contextBuf,vn->stackTemp);
	int i;
	if (b == FALSE)
		return FALSE;
	for ( i = 0; i < vn->nestingLevel; i++) {
		vn->context[i] = vn->stackTemp[i];
	}
	vn->l1index = vn->stackTemp[vn->nestingLevel];
	vn->l2index = vn->stackTemp[vn->nestingLevel + 1];
	vn->l3index = vn->stackTemp[vn->nestingLevel + 2];
	vn->l2lower = vn->stackTemp[vn->nestingLevel + 3];
	vn->l2upper = vn->stackTemp[vn->nestingLevel + 4];
	vn->l3lower = vn->stackTemp[vn->nestingLevel + 5];
	vn->l3upper = vn->stackTemp[vn->nestingLevel + 6];
	vn->atTerminal = (vn->stackTemp[vn->nestingLevel + 7] == 1);
	vn->LN  = vn->stackTemp[vn->nestingLevel + 8];
	return TRUE;
}


//Load the context info from contextBuf2.
//Info saved including LC and current state of the context 
// this function is for XPath evaluation only
Boolean pop2(VTDNav *vn){
	Boolean b = load(vn->contextBuf2,vn->stackTemp);
	int i;
	if (b == FALSE)
		return FALSE;
	for ( i = 0; i < vn->nestingLevel; i++) {
		vn->context[i] = vn->stackTemp[i];
	}
	vn->l1index = vn->stackTemp[vn->nestingLevel];
	vn->l2index = vn->stackTemp[vn->nestingLevel + 1];
	vn->l3index = vn->stackTemp[vn->nestingLevel + 2];
	vn->l2lower = vn->stackTemp[vn->nestingLevel + 3];
	vn->l2upper = vn->stackTemp[vn->nestingLevel + 4];
	vn->l3lower = vn->stackTemp[vn->nestingLevel + 5];
	vn->l3upper = vn->stackTemp[vn->nestingLevel + 6];
	vn->atTerminal = (vn->stackTemp[vn->nestingLevel + 7] == 1);
	vn->LN  = vn->stackTemp[vn->nestingLevel + 8];
	return TRUE;
}

//Store the context info into the contextBuf.
//Info saved including LC and current state of the context 
Boolean push(VTDNav *vn){
	int i;
	for (i = 0; i < vn->nestingLevel; i++) {
		vn->stackTemp[i] = vn->context[i];
	}
	vn->stackTemp[vn->nestingLevel] = vn->l1index;
	vn->stackTemp[vn->nestingLevel + 1] = vn->l2index;
	vn->stackTemp[vn->nestingLevel + 2] = vn->l3index;
	vn->stackTemp[vn->nestingLevel + 3] = vn->l2lower;
	vn->stackTemp[vn->nestingLevel + 4] = vn->l2upper;
	vn->stackTemp[vn->nestingLevel + 5] = vn->l3lower;
	vn->stackTemp[vn->nestingLevel + 6] = vn->l3upper;
	if (vn->atTerminal)
		vn->stackTemp[vn->nestingLevel + 7]=1;
	else 
		vn->stackTemp[vn->nestingLevel + 7]=0;
	vn->stackTemp[vn->nestingLevel + 8] = vn->LN;
	store(vn->contextBuf,vn->stackTemp);
	return TRUE;
}

//Store the context info into the contextBuf2.
//Info saved including LC and current state of the context 
// This function is for XPath evaluation only
Boolean push2(VTDNav *vn){
	int i;
	for (i = 0; i < vn->nestingLevel; i++) {
		vn->stackTemp[i] = vn->context[i];
	}
	vn->stackTemp[vn->nestingLevel] = vn->l1index;
	vn->stackTemp[vn->nestingLevel + 1] = vn->l2index;
	vn->stackTemp[vn->nestingLevel + 2] = vn->l3index;
	vn->stackTemp[vn->nestingLevel + 3] = vn->l2lower;
	vn->stackTemp[vn->nestingLevel + 4] = vn->l2upper;
	vn->stackTemp[vn->nestingLevel + 5] = vn->l3lower;
	vn->stackTemp[vn->nestingLevel + 6] = vn->l3upper;
	if (vn->atTerminal)
		vn->stackTemp[vn->nestingLevel + 7]=1;
	else 
		vn->stackTemp[vn->nestingLevel + 7]=0;
	vn->stackTemp[vn->nestingLevel + 8] = vn->LN;
	store(vn->contextBuf2,vn->stackTemp);
	return TRUE;
}


//Sync up the current context with location cache.
static void resolveLC(VTDNav *vn){	
	if (vn->context[0]<=0)
		return;
	resolveLC_l1(vn);
	if (vn->context[0] == 1)
		return;
	resolveLC_l2(vn);	
	if (vn->context[0] == 2)
		return;
	resolveLC_l3(vn);

	/*if (vn->context[0] == 3)
	break;*/
}

//Test whether the URL is defined in the document.
static Boolean resolveNS(VTDNav *vn, UCSChar *URL){
	int i, offset, preLen;

	if (vn->context[0]==-1)
		return FALSE;
	i =
		getTokenLength(vn,(vn->context[0] != 0) ? vn->context[vn->context[0]] : vn->rootIndex);
	offset =
		getTokenOffset(vn,(vn->context[0] != 0) ? vn->context[vn->context[0]] : vn->rootIndex);
	preLen = (i >> 16) & 0xffff;

	i = lookupNS2(vn, offset, preLen);
	switch(i){
							 case 0: if (URL== NULL){
								 if (getTokenLength(vn,i)==0)
								 return TRUE;
									 } else {
										 return FALSE;
									 }
							 default:
								 if (URL == NULL){
									 return FALSE;
								 }
								 else {
									 return matchTokenString2(vn,i,URL);
								 }
	}
}

//Test whether the URL is defined in the document.
//Null is allowed to indicate the name space should be undefined.
static Boolean resolveNS2(VTDNav *vn, UCSChar *URL, int offset, int len){
	int i;
	i = lookupNS2(vn, offset, len);
	switch(i){
							 case 0: if (URL== NULL){
								 return TRUE;
									 } else {
										 return FALSE;
									 }
							 default:
								 if (URL == NULL){
									 return FALSE;
								 }
								 else {
									 return matchTokenString(vn,i,URL);
								 }
	}
}

void sampleState(VTDNav *vn, FastIntBuffer *fib){
	if (vn->context[0]>=1)							
		appendInt(fib, vn->l1index);

	if (vn->context[0]>=2){
		appendInt(fib,vn->l2index);
		appendInt(fib,vn->l2upper);
		appendInt(fib,vn->l2lower);
	}

	if (vn->context[0]>=3){
		appendInt(fib,vn->l3index);
		appendInt(fib,vn->l3upper);
		appendInt(fib,vn->l3lower);
	}

}
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
Boolean toElement(VTDNav *vn, navDir direction){
	int size,i;
	switch (direction) {
			case ROOT :
				if (vn->context[0] != 0) {
					/*for (i = 1; i <= vn->context[0]; i++) {
					vn->context[i] = 0xffffffff;
					}*/
					vn->context[0] = 0;
				}
				vn->atTerminal = FALSE;
				vn->l1index = vn->l2index = vn->l3index = -1;
				return TRUE;
			case PARENT :
				if (vn->atTerminal == TRUE){
					vn->atTerminal = FALSE;
					return TRUE;
				}
				if (vn->context[0] > 0) {
					//vn->context[vn->context[0]] = vn->context[vn->context[0] + 1] = 0xffffffff;
					//vn->context[vn->context[0]] = -1;
					vn->context[0]--;
					return TRUE;
				} else if (vn->context[0]==0){
					vn->context[0]=-1;
					return TRUE;
				}else {
					return FALSE;
				}
			case FIRST_CHILD :
			case LAST_CHILD :
				if (vn->atTerminal) return FALSE;
				switch (vn->context[0]) {
			case -1:
				vn->context[0] = 0;
				return TRUE;
			case 0 :
				if (vn->l1Buffer->size > 0) {
					vn->context[0] = 1;
					vn->l1index =
						(direction == FIRST_CHILD)
						? 0
						: (vn->l1Buffer->size - 1);
					vn->context[1] = upper32At(vn->l1Buffer,vn->l1index);
					//(int) (vtdToken >> 32);
					return TRUE;
				} else
					return FALSE;
			case 1 :
				vn->l2lower = lower32At(vn->l1Buffer,vn->l1index);
				if (vn->l2lower == -1) {
					return FALSE;
				}
				vn->context[0] = 2;
				vn->l2upper = vn->l2Buffer->size - 1;
				size = vn->l1Buffer->size;
				for (i = vn->l1index + 1; i < size; i++) {
					int temp = lower32At(vn->l1Buffer,i);
					if (temp != 0xffffffff) {
						vn->l2upper = temp - 1;
						break;
					}
				}
				//System.out.println(" l2 upper: " + l2upper + " l2 lower : " + l2lower);
				vn->l2index =
					(direction == FIRST_CHILD) ? vn->l2lower : vn->l2upper;
				vn->context[2] = upper32At(vn->l2Buffer,vn->l2index);
				return TRUE;

			case 2 :
				vn->l3lower = lower32At(vn->l2Buffer,vn->l2index);
				if (vn->l3lower == -1) {
					return FALSE;
				}
				vn->context[0] = 3;
				size = vn->l2Buffer->size;
				vn->l3upper = vn->l3Buffer->size - 1;

				for (i = vn->l2index + 1; i < size; i++) {
					int temp = lower32At(vn->l2Buffer,i);
					if (temp != 0xffffffff) {
						vn->l3upper = temp - 1;
						break;
					}
				}
				//System.out.println(" l3 upper : " + l3upper + " l3 lower : " + l3lower);
				vn->l3index =
					(direction == FIRST_CHILD) ? vn->l3lower : vn->l3upper;
				vn->context[3] = intAt(vn->l3Buffer,vn->l3index);

				return TRUE;

			default :
				if (direction == FIRST_CHILD) {
					int index;
					size = vn->vtdBuffer->size;
					index = vn->context[vn->context[0]] + 1;
					while (index < size) {
						Long temp = longAt(vn->vtdBuffer,index);
						int token_type;

						token_type =
							(int) ((MASK_TOKEN_TYPE & temp) >> 60)
							& 0xf;


						if (token_type == TOKEN_STARTING_TAG) {

							int depth =
								(int) ((MASK_TOKEN_DEPTH & temp) >> 52);

							if (depth <= vn->context[0]) {
								return FALSE;
							} else if (depth == (vn->context[0] + 1)) {
								vn->context[0] += 1;
								vn->context[vn->context[0]] = index;
								return TRUE;
							}
						}

						index++;
					} // what condition  
					return FALSE;
				} else {
					int index = vn->context[vn->context[0]] + 1;
					int last_index = -1;
					size = vn->vtdBuffer->size;
					while (index < size) {
						Long temp = longAt(vn->vtdBuffer,index);
						int token_type;
						int depth;

						depth =
							(int) ((MASK_TOKEN_DEPTH & temp) >> 52);



						token_type =
							(int) ((MASK_TOKEN_TYPE & temp) >> 60)
							& 0xf;

						if (token_type == TOKEN_STARTING_TAG) {
							if (depth <= vn->context[0]) {
								break;
							} else if (depth == (vn->context[0] + 1)) {
								last_index = index;
							}
						}
						index++;
					}

					if (last_index == -1) {
						return FALSE;
					} else {
						vn->context[0] += 1;
						vn->context[vn->context[0]] = last_index;
						return TRUE;
					}
				}
				}

			case NEXT_SIBLING :
			case PREV_SIBLING :
				if (vn->atTerminal) return FALSE;
				switch (vn->context[0]) {
			case 0 :
				return FALSE;
			case 1 :
				if (direction == NEXT_SIBLING) {
					if (vn->l1index + 1 >= vn->l1Buffer->size) {
						return FALSE;
					}

					vn->l1index++; // global incremental
				} else {
					if (vn->l1index - 1 < 0) {
						return FALSE;
					}
					vn->l1index--; // global incremental
				}
				vn->context[1] = upper32At(vn->l1Buffer,vn->l1index);
				return TRUE;
			case 2 :
				if (direction == NEXT_SIBLING) {
					if (vn->l2index + 1 > vn->l2upper) {
						return FALSE;
					}
					vn->l2index++;
				} else {
					if (vn->l2index - 1 < vn->l2lower) {
						return FALSE;
					}
					vn->l2index--;
				}
				vn->context[2] = upper32At(vn->l2Buffer,vn->l2index);
				return TRUE;
			case 3 :
				if (direction == NEXT_SIBLING) {
					if (vn->l3index + 1 > vn->l3upper) {
						return FALSE;
					}
					vn->l3index++;
				} else {
					if (vn->l3index - 1 < vn->l3lower) {
						return FALSE;
					}
					vn->l3index--;
				}
				vn->context[3] = intAt(vn->l3Buffer,vn->l3index);
				return TRUE;
			default :
				//int index = context[context[0]] + 1;

				if (direction == NEXT_SIBLING) {
					int index = vn->context[vn->context[0]] + 1;
					int size = vn->vtdBuffer->size;
					while (index < size) {
						Long temp = longAt(vn->vtdBuffer,index);
						int token_type;

						token_type =
							(int) ((MASK_TOKEN_TYPE & temp) >> 60)
							& 0xf;

						if (token_type == TOKEN_STARTING_TAG) {
							int depth =
								(int) ((MASK_TOKEN_DEPTH & temp) >> 52);

							if (depth < vn->context[0]) {
								return FALSE;
							} else if (depth == (vn->context[0])) {
								vn->context[vn->context[0]] = index;
								return TRUE;
							}
						}
						index++;
					}
					return FALSE;
				} else {
					int index = vn->context[vn->context[0]] - 1;
					while (index > vn->context[vn->context[0] - 1]) {
						// scan backforward
						Long temp =longAt(vn->vtdBuffer,index);

						tokenType token_type =
							(int) ((MASK_TOKEN_TYPE & temp) >> 60)
							& 0xf;

						if (token_type == TOKEN_STARTING_TAG) {

							int depth =
								(int) ((MASK_TOKEN_DEPTH & temp) >> 52);

							/*if (depth < vn->context[0]) {
							return false;
							} else */
							if (depth == (vn->context[0])) {
								vn->context[vn->context[0]] = index;
								return TRUE;
							}
						}
						index--;
					} // what condition          	    
					return FALSE;
				}
				}

			default :
				return FALSE;
	}

}

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
Boolean toElement2(VTDNav *vn, navDir direction, UCSChar *en){
	//int size;
	int temp;
	int d;
	int val = 0;
	if (en == NULL){
		throwException2(invalid_argument,
			"inavlid argument for toElement2");
	}
	if (wcscmp(en,L"*") == 0)
		return toElement(vn,direction);
	switch (direction) {
			case ROOT :
				return toElement(vn,ROOT);

			case PARENT :
				return toElement(vn,PARENT);

			case FIRST_CHILD :
				if (vn->atTerminal)return FALSE;
				if (toElement(vn,FIRST_CHILD) == FALSE)
					return FALSE;
				// check current element name
				if (matchElement(vn,en) == FALSE) {
					if (toElement2(vn,NEXT_SIBLING, en) == TRUE)
						return TRUE;
					else {
						//toParentElement();
						//vn->context[vn->context[0]] = 0xffffffff;
						vn->context[0]--;
						return FALSE;
					}
				} else
					return TRUE;

			case LAST_CHILD :
				if (vn->atTerminal)return FALSE;
				if (toElement(vn,LAST_CHILD) == FALSE)
					return FALSE;
				if (matchElement(vn,en) == FALSE){
					if (toElement2(vn,PREV_SIBLING, en) == TRUE)
						return TRUE;
					else {
						//vn->context[vn->context[0]] = 0xffffffff;
						vn->context[0]--;
						//toParentElement();
						return FALSE;
					}
				} else
					return TRUE;

			case NEXT_SIBLING :
				if (vn->atTerminal)return FALSE;
				d = vn->context[0];
				temp = vn->context[d]; // store the current position

				switch(d)
				{
				case -1:
				case 0: return FALSE;
				case 1: val = vn->l1index; break;
				case 2: val = vn->l2index; break;
				case 3: val = vn->l3index; break;
				default:
					break;
				}
				//if (d == 0)
				//	return FALSE;
				while (toElement(vn,NEXT_SIBLING)) {
					if (matchElement(vn,en)) {
						return TRUE;
					}
				}
				switch(d)
				{
				case 1: vn->l1index = val; break;
				case 2: vn->l2index = val; break;
				case 3: vn->l3index = val; break;
				default:
					break;
				}
				vn->context[d] = temp;
				return FALSE;

			case PREV_SIBLING :
				if (vn->atTerminal)return FALSE;
				d = vn->context[0];
				temp = vn->context[d]; // store the current position
				switch(d)
				{
				case -1:
				case 0: return FALSE;
				case 1: val = vn->l1index; break;
				case 2: val = vn->l2index; break;
				case 3: val = vn->l3index; break;
				default:
					break;
				}
				//if (d == 0)
				//	return FALSE;
				while (toElement(vn,PREV_SIBLING)) {
					if (matchElement(vn,en)) {
						return TRUE;
					}
				}
				switch(d)
				{
				case 1: vn->l1index = val; break;
				case 2: vn->l2index = val; break;
				case 3: vn->l3index = val; break;
				default:
					break;
				}
				vn->context[d] = temp;
				return FALSE;

			default :
				return FALSE;
				//throw new NavException("illegal navigation options");
	}
}
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
Boolean toElementNS(VTDNav *vn, navDir direction, UCSChar *URL, UCSChar *ln){
	//int size;
	int temp;
	int d;
	int val = 0;
	if (vn->ns == FALSE)
		return FALSE;
	switch (direction) {
			case ROOT :
				return toElement(vn,ROOT);

			case PARENT :
				return toElement(vn,PARENT);

			case FIRST_CHILD :
				if (vn->atTerminal)return FALSE;
				if (toElement(vn,FIRST_CHILD) == FALSE)
					return FALSE;
				// check current element name
				if (matchElementNS(vn,URL, ln) == FALSE) {
					if (toElementNS(vn,NEXT_SIBLING, URL, ln) == TRUE )
						return TRUE;
					else {
						//toParentElement();
						//vn->context[vn->context[0]] = 0xffffffff;
						vn->context[0]--;
						return FALSE;
					}
				} else
					return TRUE;

			case LAST_CHILD :
				if (vn->atTerminal)return FALSE;
				if (toElement(vn,LAST_CHILD) == FALSE)
					return FALSE;
				if (matchElementNS(vn, URL, ln) == FALSE) {
					if (toElementNS(vn, PREV_SIBLING, URL, ln) == TRUE)
						return TRUE;
					else {
						//vn->context[vn->context[0]] = 0xffffffff;
						vn->context[0]--;
						//toParentElement();
						return FALSE;
					}
				} else
					return TRUE;

			case NEXT_SIBLING :
				if (vn->atTerminal)return FALSE;
				d = vn->context[0];
				temp = vn->context[d]; // store the current position
				switch(d)
				{
				case -1:
				case 0: return FALSE;
				case 1: val = vn->l1index; break;
				case 2: val = vn->l2index; break;
				case 3: val = vn->l3index; break;
				default:
					break;
				}
				//if (d == 0)
				//	return FALSE;
				while (toElement(vn,NEXT_SIBLING)) {
					if (matchElementNS(vn,URL, ln)) {
						return TRUE;
					}
				}
				switch(d)
				{
				case 1: vn->l1index = val; break;
				case 2: vn->l2index = val; break;
				case 3: vn->l3index = val; break;
				default:
					break;
				}
				vn->context[d] = temp;
				return FALSE;

			case PREV_SIBLING :
				if (vn->atTerminal)return FALSE;
				d = vn->context[0];
				temp = vn->context[d]; // store the current position
				switch(d)
				{
				case -1: 
				case  0: return FALSE;
				case 1: val = vn->l1index; break;
				case 2: val = vn->l2index; break;
				case 3: val = vn->l3index; break;
				default:
					break;
				}
				//if (d == 0)
				//	return FALSE;
				while (toElement(vn,PREV_SIBLING)) {
					if (matchElementNS(vn,URL, ln)) {
						return TRUE;
					}
				}
				switch(d)
				{
				case 1: vn->l1index = val; break;
				case 2: vn->l2index = val; break;
				case 3: vn->l3index = val; break;
				default:
					break;
				}
				vn->context[d] = temp;
				return FALSE;

			default :
				return FALSE;
				//throw new NavException("illegal navigation options");
	}

}

/*This method normalizes a token into a string in a way that resembles DOM.
The leading and trailing white space characters will be stripped.
The entity and character references will be resolved
Multiple whitespaces char will be collapsed into one.*/

UCSChar *toNormalizedString(VTDNav *vn, int index){
	tokenType type = getTokenType(vn,index);

	if (type!=TOKEN_CHARACTER_DATA &&
		type!= TOKEN_ATTR_VAL)
		return toRawString(vn, index);
	else {
		int len, endOffset;
		int ch,k,offset;
		Long l;
		Boolean d;
		UCSChar *s = NULL;


		len = getTokenLength(vn,index);

		s = (UCSChar *)malloc(sizeof(UCSChar)*(len+1));
		if (s == NULL)
		{
			throwException2(out_of_mem,
				" string allocation failed in toNormalizedString ");
		}

		offset = getTokenOffset(vn ,index);
		endOffset = len + offset - 1; // point to the last character


		// trim off the leading whitespaces

		while (TRUE) {
			int temp = offset;
			l = getChar(vn,offset);
			ch = (int)l;
			offset += (UCSChar)(l>>32);

			if (!isWS(ch)) {
				offset = temp;
				break;
			}
		}

		d = FALSE;
		k = 0;
		while (offset <= endOffset) {
			l= getCharResolved(vn,offset);
			ch = (int)l;
			offset += (int)(l>>32);

			if (isWS(ch)&& getCharUnit(vn,offset - 1) != ';') {
				d = TRUE;
			} else {
				if (d == FALSE)
					//sb.append((char) ch); // java only supports 16 bit unicode
					s[k++] = ch;
				else {
					//sb.append(' ');
					s[k++] = (UCSChar) ' ';
					//sb.append((char) ch);
					s[k++] = (UCSChar) ch;
					d = FALSE;
				}
			}
		}
		s[k] = 0;
		return s;
	}
}

/*Convert a token at the given index to a String, 
(built-in entity and char references not resolved)
(entities and char references not expanded).*/
UCSChar *toRawString(VTDNav *vn, int index){						
	int offset;
	tokenType type = getTokenType(vn,index);
	int len;						 
	/*UCSChar *s = NULL;*/

	if (type == TOKEN_STARTING_TAG
		|| type == TOKEN_ATTR_NAME
		|| type == TOKEN_ATTR_NS)
		len = getTokenLength(vn,index) & 0xffff;
	else
		len = getTokenLength(vn,index);

	offset = getTokenOffset(vn,index);
	return toRawString2(vn, offset, len);
}

UCSChar *toRawString2(VTDNav *vn, int os, int len){
	int offset = os, endOffset=os+len,k=0;
	Long l;
	UCSChar *s = NULL;
	s = (UCSChar *)malloc(sizeof(UCSChar)*(len+1));
	if (s == NULL)
	{
		throwException2(out_of_mem,
			" string allocation failed in toRawString2 ");
	}
	/*if (vn->encoding > FORMAT_WIN_1258){
		offset = offset>>1;
		endOffset = endOffset>>1;
	}*/
	while (offset < endOffset) {
		l = getChar(vn,offset);
		offset += (int)(l>>32);
		s[k++] = (UCSChar)l; // java only support 16 bit unit code
	}
	s[k] = 0;
	return s;
}


//Convert a token at the given index to a String, (entities and char 
//references resolved).
// An attribute name or an element name will get the UCS2 string of qualified name 
UCSChar *toString(VTDNav *vn, int index){
	int offset;
	tokenType type = getTokenType(vn,index);
	int len;

	/*UCSChar *s = NULL;*/
	if (type!=TOKEN_CHARACTER_DATA &&
		type!= TOKEN_ATTR_VAL)
		return toRawString(vn,index);

	len = getTokenLength(vn,index);

	offset = getTokenOffset(vn,index);
	return toString2(vn,offset,len);
}

UCSChar *toString2(VTDNav *vn, int os, int len){
	int offset = os, endOffset=os+len,k=0;
	Long l;
	UCSChar *s = NULL;
	s = (UCSChar *)malloc(sizeof(UCSChar)*(len+1));
	if (s == NULL)
	{
		throwException2(out_of_mem,
			" string allocation failed in toString2 ");
	}
	/*if (vn->encoding > FORMAT_WIN_1258){
		offset = offset>>1;
		endOffset = endOffset>>1;
	}*/
	while (offset < endOffset) {
		l = getCharResolved(vn,offset);
		offset += (int)(l>>32);
		s[k++] = (UCSChar)l; // java only support 16 bit unit code
	}
	s[k] = 0;
	return s;
}

//Get the starting offset of the token at the given index.

int getTokenOffset(VTDNav *vn, int index){

	return (int) (longAt(vn->vtdBuffer,index) & vn->offsetMask);

}
// Get the XML document 
UByte* getXML(VTDNav *vn){
	return vn->XMLDoc;
}

//Get the token type of the token at the given index value.
tokenType getTokenType(VTDNav *vn, int index){
	return (tokenType) ((longAt(vn->vtdBuffer,index) & MASK_TOKEN_TYPE) >> 60) & 0xf;
}

//Get the depth (>=0) of the current element.
int getCurrentDepth(VTDNav *vn){
	return vn->context[0];
}
// Get the index value of the current element/text/attribute.
int getCurrentIndex(VTDNav *vn){
	if (vn->atTerminal)
		return vn->LN;
	switch(vn->context[0]){
		case -1: return 0;
		case 0: return vn->rootIndex;
		default: return vn->context[vn->context[0]];
	}
	//return (vn->context[0] == 0) ? vn->rootIndex : vn->context[vn->context[0]];
}

int getCurrentIndex2(VTDNav *vn){
	switch(vn->context[0]){
		case -1: return 0;
		case 0: return vn->rootIndex;
		default: return vn->context[vn->context[0]];
	}
	//return (vn->context[0] == 0) ? vn->rootIndex : vn->context[vn->context[0]];
}

/**
* Get the encoding of the XML document.
* <pre>   0  ASCII       </pre>
* <pre>   1  ISO-8859-1  </pre>
* <pre>   2  UTF-8       </pre>
* <pre>   3  UTF-16BE    </pre>
* <pre>   4  UTF-16LE    </pre>
*/
inline encoding_t getEncoding(VTDNav *vn){
	return vn->encoding;
}

// Get the maximum nesting depth of the XML document (>0).
// max depth is nestingLevel -1

// max depth is nestingLevel -1
inline int getNestingLevel(VTDNav *vn){
	return vn->nestingLevel;
}
// Get root index value.
inline int getRootIndex(VTDNav *vn){
	return vn->rootIndex;
}

//Get total number of VTD tokens for the current XML document.
inline int getTokenCount(VTDNav *vn){
	return vn->vtdSize;
}


/**
* Set the value of atTerminal
* This function only gets called in XPath eval
* when a step calls for @* or child::text()
*/

inline void setAtTerminal(VTDNav* vn, Boolean b){
	vn->atTerminal = b;
}

/**
* Get the value of atTerminal
* This function only gets called in XPath eval
*/
inline Boolean getAtTerminal(VTDNav *vn){
	return vn->atTerminal;
}


inline int swap_bytes(int i){
	return (((i & 0xff) << 24) |
		((i & 0xff00) <<8) |
		((i & 0xff0000) >> 8) |
		((i & 0xff000000) >> 24)&0xff);
}


static int lookupNS2(VTDNav *vn, int offset, int len){
	Long l; 
	int i,k;
	tokenType type;
	Boolean hasNS = FALSE;
	int size = vn->vtdBuffer->size;
	// look for a match in the current hiearchy and return true
	for (i = vn->context[0]; i >= 0; i--) {
		int s = (i != 0) ? vn->context[i] : vn->rootIndex;
		switch (NSval(vn,s)) { // checked the ns marking
				case 0xc0000000 :
					s = s + 1;
					if (s>=size)
						break;
					type = getTokenType(vn,s);

					while (s < size
						&& (type == TOKEN_ATTR_NAME || type == TOKEN_ATTR_NS)) {
							if (type == TOKEN_ATTR_NS) {
								// Get the token length
								int temp = getTokenLength(vn, s);
								int preLen = ((temp >> 16) & 0xffff);
								int fullLen = temp & 0xffff;
								int os = getTokenOffset(vn, s);
								// xmlns found
								if (temp == 5 && len == 0) {
									return s+1;
								} else if ((fullLen - preLen - 1) == len) {
									// prefix length identical to local part of ns declaration
									Boolean a = TRUE;
									int j;
									for (j = 0; j < len; j++) {
										if (getCharUnit(vn,os + preLen + 1 + j)
											!= getCharUnit(vn,offset + j)) {
												a = FALSE;
												break;
										}
									}
									if (a == TRUE) {
										return s+1;
									}
								}
							}
							//return (URL != null) ? true : false;
							s += 2;
							type = getTokenType(vn,s);
					}
					break;
				case 0x80000000 :
					break;
				default : // check the ns existence, mark bit 31:30 to 11 or 10
					k = s + 1;
					if (k>=size)
						break;
					type = getTokenType(vn,k);

					while ((type == TOKEN_ATTR_NAME || type == TOKEN_ATTR_NS)) {
						if (type == TOKEN_ATTR_NS) {
							// Get the token length

							int temp = getTokenLength(vn, k);
							int preLen = ((temp >> 16) & 0xffff);
							int fullLen = temp & 0xffff;
							int os = getTokenOffset(vn, k);
							hasNS = TRUE;
							// xmlns found
							if (temp == 5 && len == 0) {
								l = longAt(vn->vtdBuffer,s);
								hasNS = FALSE;

								modifyEntryFLB(vn->vtdBuffer,
									s,
									l | 0x00000000c0000000L);
								return k+1;
							} else if ((fullLen - preLen - 1) == len) {
								// prefix length identical to local part of ns declaration
								Boolean a = TRUE;
								int j;
								for (j = 0; j < len; j++) {
									if (getCharUnit(vn, os + preLen + 1 + j)
										!= getCharUnit(vn, offset + j)) {
											a = FALSE;
											break;
									}
								}
								if (a == TRUE) {
									l = longAt(vn->vtdBuffer,s);
									//hasNS = false;

									modifyEntryFLB(vn->vtdBuffer,
										s,
										l | 0x00000000c0000000L);

									return k+1;
								}
							}
						}
						//return (URL != null) ? true : false;
						k += 2;
						if (k>=size)
							break;
						type = getTokenType(vn,k);
					}
					l = longAt(vn->vtdBuffer, s);

					if (hasNS) {
						hasNS = FALSE;
						modifyEntryFLB(vn->vtdBuffer, s, l | 0x00000000c0000000L);
					} else {
						modifyEntryFLB(vn->vtdBuffer, s, l | 0x0000000080000000L);
					}

					break;
		}
	}
	return 0;

}

int lookupNS(VTDNav *vn){
	int i, offset, preLen;

	if (vn->context[0]==-1)
		return FALSE;
	i =
		getTokenLength(vn,(vn->context[0] != 0) ? vn->context[vn->context[0]] : vn->rootIndex);
	offset =
		getTokenOffset(vn,(vn->context[0] != 0) ? vn->context[vn->context[0]] : vn->rootIndex);
	preLen = (i >> 16) & 0xffff;

	return lookupNS2(vn,offset, preLen);
}

static Long getChar4OtherEncoding(VTDNav *vn, int offset){

	if (vn->encoding <= FORMAT_WIN_1258) 
	{ 
		int temp = decode(vn,offset); 
		if (temp == '\r') 
		{ 
			if (vn->XMLDoc[offset + 1] == '\n') 
			{ 
				return '\n' | (2LL << 32); 
			} 
			else 
			{ 
				return '\n' | (1LL << 32); 
			} 
		} 
		return temp | (1LL << 32); 
	} 
	{
		throwException(nav_exception,0,
			"navigation exception during getChar4OtherEncoding",	
			"Unknown encoding error: should never happen");
		return 0;
	}
}

static int decode(VTDNav *vn,int offset)
{
	char ch = vn->XMLDoc[offset];
	switch (vn->encoding)
	{
	case FORMAT_ISO_8859_2:
		return iso_8859_2_decode(ch);
	case FORMAT_ISO_8859_3:
		return iso_8859_3_decode(ch);
	case FORMAT_ISO_8859_4:
		return iso_8859_4_decode(ch);
	case FORMAT_ISO_8859_5:
		return iso_8859_5_decode(ch);
	case FORMAT_ISO_8859_6:
		return iso_8859_6_decode(ch);
	case FORMAT_ISO_8859_7:
		return iso_8859_7_decode(ch);
	case FORMAT_ISO_8859_8:
		return iso_8859_8_decode(ch);
	case FORMAT_ISO_8859_9:
		return iso_8859_9_decode(ch);
	case FORMAT_ISO_8859_10:
		return iso_8859_10_decode(ch);
	case FORMAT_ISO_8859_11:
		return iso_8859_11_decode(ch);
	case FORMAT_ISO_8859_13:
		return iso_8859_13_decode(ch);
	case FORMAT_ISO_8859_14:
		return iso_8859_14_decode(ch);
	case FORMAT_ISO_8859_15:
		return iso_8859_15_decode(ch);
	case FORMAT_WIN_1250:
		return windows_1250_decode(ch);
	case FORMAT_WIN_1251:
		return windows_1251_decode(ch);
	case FORMAT_WIN_1252:
		return windows_1252_decode(ch);
	case FORMAT_WIN_1253:
		return windows_1253_decode(ch);
	case FORMAT_WIN_1254:
		return windows_1254_decode(ch);
	case FORMAT_WIN_1255:
		return windows_1255_decode(ch);
	case FORMAT_WIN_1256:
		return windows_1256_decode(ch);
	case FORMAT_WIN_1257:
		return windows_1257_decode(ch);
	default:
		return windows_1258_decode(ch);
	}
}

void throwException(enum exception_type et, int sub_type, char* msg, char* submsg){
	exception e;
	e.et = et;
	e.subtype = sub_type;
	e.msg = msg;
	e.sub_msg = submsg;
	Throw e;
}

void throwException2 (enum exception_type et, char *msg){
	exception e;
	e.et = et;
	e.msg = msg;
	Throw e;
}

/* overwrite */
Boolean overWrite(VTDNav *vn, int index, UByte* ba, int offset, int len){
	int t;
	if (ba == NULL
		|| index >= vn->vtdSize
		|| offset < 0){
			throwException2(invalid_argument,"Illegal argument for overwrite");
			return FALSE;
	}
	if (vn->encoding >= FORMAT_UTF_16BE
		&& (((len & 1) == 1)
		|| ((offset & 1) == 1)))
	{
		// for UTF 16, len and offset must be integer multiple
		// of 2
		return FALSE;
	}
	t = getTokenType(vn,index);
	if (t == TOKEN_CHARACTER_DATA
		|| t == TOKEN_ATTR_VAL
		|| t == TOKEN_CDATA_VAL)
	{
		int os, length,temp,k;
		length = getTokenLength(vn,index);
		if (length < len)
			return FALSE;
		os = getTokenOffset(vn,index);
		temp = length - len;
		// get XML doc
		//Array.Copy(ba, offset, XMLDoc.getBytes(), os, len);
		memcpy(vn->XMLDoc+os,ba+offset,len);
		for (k = 0; k < temp; )
		{
			if (vn->encoding < FORMAT_UTF_16BE)
			{
				// write white spaces
				vn->XMLDoc[os + len + k] = ' ';
				k++;
			}
			else
			{
				if (vn->encoding == FORMAT_UTF_16BE)
				{
					vn->XMLDoc[os + len + k] = 0;
					vn->XMLDoc[os + len + k+1] = ' ';
				}
				else
				{
					vn->XMLDoc[os + len + k] = ' ';
					vn->XMLDoc[os + len + k + 1] = 0;                            
				}
				k += 2;
			}
		}
		return TRUE;
	}
	return FALSE;
}

int compareRawTokenString2(VTDNav *vn, int offset, int len, UCSChar *s){
	int endOffset;
	int l,i;
	Long l1;
	//throw new IllegalArgumentException("string can't be null");

	//vn->currentOffset = offset;
	endOffset = offset + len;

	l = (int)wcslen(s);
	for (i = 0; i < l && offset < endOffset; i++) {
		l1 = getChar(vn,offset);
		offset += (int)(l1>>32);
		if (s[i] < (int) l1)
            return 1;
        if (s[i] > (int) l1)
            return -1;
	}

	if (i == l && offset < endOffset)
		return 1;
	if (i < l && offset == endOffset)
		return -1;
	return 0;
}

int compareTokenString2(VTDNav *vn, int offset, int len, UCSChar *s){
	int endOffset;
	int l,i;
	Long l1;

	//throw new IllegalArgumentException("string can't be null");

	//vn->currentOffset = offset;
	endOffset = offset + len;

	l = (int)wcslen(s);
	for (i = 0; i < l && offset < endOffset; i++) {
		l1 = getCharResolved(vn,offset);
		offset += (int)(l1>>32);
		if (s[i] < (int) l1)
            return 1;
        if (s[i] > (int) l1)
            return -1;
	}

	if (i == l && offset < endOffset)
		return 1;
	if (i < l && offset == endOffset)
		return -1;
	return 0;

}

int compareTokenString(VTDNav *vn,int index, UCSChar *s){
		tokenType type;
	int offset;
	int len;

	type = getTokenType(vn,index);
	len =
		(type == TOKEN_STARTING_TAG
		|| type == TOKEN_ATTR_NAME
		|| type == TOKEN_ATTR_NS)
		? getTokenLength(vn,index) & 0xffff
		: getTokenLength(vn,index);
	// upper 16 bit is zero or for prefix

	offset = getTokenOffset(vn,index);
	// point currentOffset to the beginning of the token
	// for UTF 8 and ISO, the performance is a little better by avoid calling getChar() everytime
	return compareTokenString2(vn,offset, len, s);
}

int compareRawTokenString(VTDNav *vn, int index, UCSChar *s){
	tokenType type;
	int len;
	int offset;
	/*if (s == NULL){
	//	 throwException2(invalid_argument,
	//		 " invalid argument for matchRawTokenString, s can't be NULL");
	}*/
	type = getTokenType(vn,index);
	len =
		(type == TOKEN_STARTING_TAG
		|| type == TOKEN_ATTR_NAME
		|| type == TOKEN_ATTR_NS)
		? getTokenLength(vn, index) & 0xffff
		: getTokenLength(vn, index);
	// upper 16 bit is zero or for prefix

	offset = getTokenOffset(vn, index);
	// point currentOffset to the beginning of the token
	// for UTF 8 and ISO, the performance is a little better by avoid calling getChar() everytime
	return compareRawTokenString2(vn, offset, len, s);
}

int compareTokens(VTDNav *vn1, int i1, VTDNav *vn2, int i2){
	int t1, t2;
	int ch1, ch2;
	int len1, len2;
	int endOffset1, endOffset2;
	Long l;
	int offset1, offset2;
	/*if (vn2 ==null){
	throw new NavException(" One of VTD objects is null!!");
	}*/

	if ( i1 ==i2 && vn1 == vn2)
		return 0;

	t1 = getTokenType(vn1,i1);
	t2 = getTokenType(vn2,i2);

	offset1 = getTokenOffset(vn1,i1);
	offset2 = getTokenOffset(vn2,i2);

	len1 =
		(t1 == TOKEN_STARTING_TAG
		|| t1 == TOKEN_ATTR_NAME
		|| t1 == TOKEN_ATTR_NS)
		? getTokenLength(vn1,i1) & 0xffff
		: getTokenLength(vn1,i1);

	len2 = 
		(t2 == TOKEN_STARTING_TAG
		|| t2 == TOKEN_ATTR_NAME
		|| t2 == TOKEN_ATTR_NS)
		? getTokenLength(vn2,i2) & 0xffff
		: getTokenLength(vn2,i2);

	endOffset1 = len1 + offset1;
	endOffset2 = len2 + offset2;

	for(;offset1<endOffset1 && offset2< endOffset2;){
		if(t1 == TOKEN_CHARACTER_DATA
			|| t1== TOKEN_ATTR_VAL){
				l = getCharResolved(vn1,offset1);
		} else{ 
			l = getChar(vn1,offset1);
		}
		ch1 = (int)l;
		offset1 += (int)(l>>32);

		if(t2 == TOKEN_CHARACTER_DATA
			|| t2== TOKEN_ATTR_VAL){
				l = getCharResolved(vn2,offset2);
		} else{ 
			l = getChar(vn2,offset2);
		}
		ch2 = (int)l;
		offset2 += (int)(l>>32);
		if (ch1 > ch2)
			return 1;
		if (ch1 < ch2)
			return -1;
	}

	if (offset1 == endOffset1 
		&& offset2 < endOffset2)
		return -1;
	else if (offset1 < endOffset1 
		&& offset2 == endOffset2)
		return 1;
	else
		return 0;
}

/* Write VTD+XML into a FILE pointer */
Boolean writeIndex_VTDNav(VTDNav *vn, FILE *f){
	return _writeIndex(1, 
                vn->encoding, 
                vn->ns, 
                TRUE, 
				vn->nestingLevel-1, 
                3, 
                vn->rootIndex, 
                vn->XMLDoc, 
                vn->docOffset, 
                vn->docLen, 
				vn->vtdBuffer, 
                vn->l1Buffer, 
                vn->l2Buffer, 
                vn->l3Buffer, 
                f);
}
Boolean writeIndex2_VTDNav(VTDNav *vn, char *fileName){
	FILE *f = NULL;
	Boolean b = FALSE;
	f = fopen(fileName,"wb");
	
	if (f==NULL){
		throwException2(invalid_argument,"fileName not valid");
		return FALSE;
	}
	b = writeIndex_VTDNav(vn,f);
	fclose(f);
	return b;
}
/* pre-calculate the VTD+XML index size without generating the actual index */
Long getIndexSize2(VTDNav *vn){
		int size;
	    if ( (vn->docLen & 7)==0)
	       size = vn->docLen;
	    else
	       size = ((vn->docLen >>3)+1)<<3;
	    
	    size += (vn->vtdBuffer->size<<3)+
	            (vn->l1Buffer->size<<3)+
	            (vn->l2Buffer->size<<3);
	    
		if ((vn->l3Buffer->size & 1) == 0){ //even
	        size += vn->l3Buffer->size<<2;
	    } else {
	        size += (vn->l3Buffer->size+1)<<2; //odd
	    }
	    return size+64;
}

// Get the element fragment object corresponding to a ns 
// compensated element 
ElementFragmentNs *getElementFragmentNs(VTDNav *vn){

	if (vn->ns == FALSE){  
		throwException2(nav_exception,"getElementFragmentNS can only be called when ns is tured on");
		return NULL;
	}else {
		//fill the fib with integer 
		// first get the list of name space nodes 
		FastIntBuffer *fib = NULL;
		int i = 0;	    
		int count=0;
		int type;

		int* ia = vn->context;
		int d =ia[0]; // -1 for document node, 0 for root element;
		int c = getCurrentIndex2(vn);
		int len = (c == 0 || c == vn->rootIndex )? 0: 
			(getTokenLength(vn,c) & 0xffff); // get the length of qualified node
		int newSz; Long l;
		fib = createFastIntBuffer2(3); // init size 8


		// put the neighboring ATTR_NS nodes into the array
		// and record the total # of them	     

		if (d > 0){ // depth > 0 every node except document and root element
			int k=getCurrentIndex2(vn)+1;
			if (k<vn->vtdSize){				
				while(k<vn->vtdSize) {
					type = getTokenType(vn,k);
					if(type==TOKEN_ATTR_NAME || type==TOKEN_ATTR_NS){
						if (type == TOKEN_ATTR_NS){    
							appendInt(fib,k);
							//System.out.println(" ns name ==>" + toString(k));
						}
					}
					k+=2;					
					//type = getTokenType(vn,k);
				}

			}
			count = fib->size;
			d--; 
			while (d >= 0) {                
				// then search for ns node in the vinicity of the ancestor nodes
				if (d > 0) {
					// starting point
					k = ia[d]+1;
				} else {
					// starting point
					k = vn->rootIndex+1;
				}
				if (k<vn->vtdSize){
					
					while(k<vn->vtdSize){ 
						type = getTokenType(vn,k);
						if (type==TOKEN_ATTR_NAME || type==TOKEN_ATTR_NS){
							Boolean unique = TRUE;
							if (type == TOKEN_ATTR_NS){
								int z = 0;
								for (z=0;z<fib->size;z++){
									//System.out.println("fib size ==> "+fib.size());
									//if (fib->size==4);
									if (matchTokens(vn, intAt(fib,z),vn,k)){
										unique = FALSE;
										break;
									} 

								}            
								if (unique)
									appendInt(fib,k);
							}
						}
							k+=2;
							//type = getTokenType(vn,k);
					}
				}
				d--;
			}
			// System.out.println("count ===> "+count);
			// then restore the name space node by shifting the array
			newSz= fib->size-count;
			for (i= 0; i<newSz; i++ ){
				modifyEntryFIB(fib,i,intAt(fib,i+count));                
			}
			resizeFIB(fib,newSz);
		}

		l= getElementFragment(vn);
		return createElementFragmentNs(vn,l,fib,len);
	}
}

/* dump XML text into a given file name */
void dumpXML(VTDNav *vn, char *fileName){
	FILE *f=fopen(fileName,"w");
	if (f!=NULL){
		dumpXML2(vn,f);
		fclose(f);
	} else {
		throwException2(io_exception,"can't open file");
	}	
}

/* dump XML text into a given file descriptor */
void dumpXML2(VTDNav *vn, FILE *f){	
	size_t i = fwrite(vn->XMLDoc+vn->docOffset,1,vn->docLen,f);
	if (i < (size_t) vn->docLen)
		throwException2(io_exception, "can't complete the write");	
}

/* Get the string length of a token as if it is converted into a normalized UCS string*/
int getNormalizedStringLength(VTDNav *vn, int index){	
	tokenType type = getTokenType(vn,index);
	if (type!=TOKEN_CHARACTER_DATA &&
		type!= TOKEN_ATTR_VAL)
		return getRawStringLength(vn,index); 
	else{
		int len, endOffset, len1=0;
		int ch,k,offset;
		Long l;
		Boolean d;

		len = getTokenLength(vn,index);
		offset = getTokenOffset(vn ,index);
		endOffset = len + offset - 1; // point to the last character

		// trim off the leading whitespaces
		while (TRUE) {
			int temp = offset;
			l = getChar(vn,offset);
			ch = (int)l;
			offset += (UCSChar)(l>>32);

			if (!isWS(ch)) {
				offset = temp;
				break;
			}
		}

		d = FALSE;
		k = 0;
		while (offset <= endOffset) {
			l= getCharResolved(vn,offset);
			ch = (int)l;
			offset += (int)(l>>32);

			if (isWS(ch)&& getCharUnit(vn,offset - 1) != ';') {
				d = TRUE;
			} else {
				if (d == FALSE)
					//sb.append((char) ch); // java only supports 16 bit unicode
					len1++;
				else {
					//sb.append(' ');
					len1 = len1+2;
					d = FALSE;
				}
			}
		}
		return len1;
	}
}

/* getStringLength return the string length of a token as if the token is converted into 
   a string (entity resolved)*/
int getStringLength(VTDNav *vn, int index){
	    tokenType type = getTokenType(vn,index);
        if (type != TOKEN_CHARACTER_DATA && type != TOKEN_ATTR_VAL)
            return getRawStringLength(vn, index);
		else {
			int len = 0, len1 = 0, offset, endOffset;
			Long l;
			len = getTokenLength(vn, index);
			offset = getTokenOffset(vn, index);
			endOffset = offset + len;			

			while (offset < endOffset) {
				l = getCharResolved(vn,offset);
				offset += (int) (l >> 32);
				len1++;
			}
			return len1;
		}
}

/*Get the string length as if the token is converted into a UCS string (entity not resolved) */
int getRawStringLength(VTDNav *vn, int index){
	    tokenType type = getTokenType(vn,index);
        int len = 0, len1 = 0, offset, endOffset;
		Long l;
        if (type == TOKEN_STARTING_TAG || type == TOKEN_ATTR_NAME
                || type == TOKEN_ATTR_NS)
            len = getTokenLength(vn, index) & 0xffff;
        else
            len = getTokenLength(vn, index);
		if (vn->encoding!=FORMAT_UTF8 &&
			vn->encoding!=FORMAT_UTF_16BE &&
        	vn->encoding!=FORMAT_UTF_16LE) {
        	return len;
        }
        offset = getTokenOffset(vn,index);
        endOffset = offset + len;
        
        while (offset < endOffset) {
            l = getChar(vn, offset);
            offset += (int) (l >> 32);
            len1++;
        }
        return len1;
}

/* Get the offset value right after head (e.g. <a b='b' c='c'> ) */
int getOffsetAfterHead(VTDNav *vn){
	    int i = getCurrentIndex(vn),j,offset;
		//encoding_t enc;
 	    if (getTokenType(vn,i)!= TOKEN_STARTING_TAG){
	        return -1;
 	    }
	    j=i+1;
 	    while (j<vn->vtdSize && (getTokenType(vn,j)==TOKEN_ATTR_NAME
	            || getTokenType(vn,j)== TOKEN_ATTR_NS)){
	        j += 2;
 	    }

		if (i+1==j)
	    {
			offset = getTokenOffset(vn,i)+((vn->ns==FALSE)?getTokenLength(vn,i):(getTokenLength(vn,i)&0xff));
 	    }else {
			offset = getTokenOffset(vn,j-1)+((vn->ns==FALSE)?getTokenLength(vn,j-1):(getTokenLength(vn,j-1)&0xff))+1;
	    }

 	    while(getCharUnit(vn,offset)!='>'){
	        offset++;
	    }
 
	    if (getCharUnit(vn,offset-1)=='/')
 	        return -1;
	    else
 	        return  offset+1;

}


/* Write the VTDs and LCs into an Separate index file*/
void writeSeparateIndex_VTDNav(VTDNav *vn, char *VTDIndexFile){
	FILE *f = NULL;
	//Boolean b = FALSE;
	f = fopen(VTDIndexFile,"wb");
	
	if (f==NULL){
		throwException2(invalid_argument,"fileName not valid");
		//return FALSE;
	}

	_writeSeparateIndex( (Byte)2, 
                vn->encoding, 
                vn->ns, 
                TRUE, 
				vn->nestingLevel-1, 
                3, 
                vn->rootIndex, 
                vn->docOffset, 
                vn->docLen, 
				vn->vtdBuffer, 
                vn->l1Buffer, 
                vn->l2Buffer, 
                vn->l3Buffer, 
                f);
	
	fclose(f);
	//return b;
}


/* Test the start of token content at index i matches the content 
of s, notice that this is to save the string allocation cost of
using String's built-in startsWidth */
Boolean startsWith(VTDNav *vn, int index, UCSChar *s){
	tokenType type = getTokenType(vn, index);
	int len =
		(type == TOKEN_STARTING_TAG
		|| type == TOKEN_ATTR_NAME
		|| type == TOKEN_ATTR_NS)
		? getTokenLength(vn,index) & 0xffff
		: getTokenLength(vn,index);
	int offset = getTokenOffset(vn, index);
	Long l1; 
	size_t l,i;
	int i1;
	int endOffset = offset + len;
	Boolean b = (type == TOKEN_CHARACTER_DATA 
		|| type == TOKEN_ATTR_VAL);
	//       System.out.print("currentOffset :" + currentOffset);
	l = wcslen(s);
	if (l> (size_t)len)
		return FALSE;
	//System.out.println(s);
	for (i = 0; i < l && offset < endOffset; i++) {
		if (b)
			l1 = getCharResolved(vn, offset);
		else
			l1 = getChar(vn, offset);
		i1 = s[i];
		if (i1 != (int) l1)
			return FALSE;
		offset += (int) (l1 >> 32);
	}	    
	return TRUE;

}

/*Test the end of token content at index i matches the content 
of s, notice that this is to save the string allocation cost of
using String's built-in endsWidth */
Boolean endsWith(VTDNav *vn, int index, UCSChar *s){
	tokenType type = getTokenType(vn,index);
	size_t len =
		(type == TOKEN_STARTING_TAG
		|| type == TOKEN_ATTR_NAME
		|| type == TOKEN_ATTR_NS)
		? (size_t)getTokenLength(vn,index) & 0xffff
		: (size_t)getTokenLength(vn,index);
	int offset = getTokenOffset(vn, index);
	Long l1; 
	size_t l,i,i2;
	int i1;
	Boolean b = (type == TOKEN_CHARACTER_DATA 
		|| type == TOKEN_ATTR_VAL);
	//int endOffset = offset + len;

	//       System.out.print("currentOffset :" + currentOffset);
	l = wcslen(s);
	if (l>len)
		return FALSE;
	i2 = (size_t)getStringLength(vn,index);
	if (l> i2)
		return FALSE;
	i2 = i2 - l; // calculate the # of chars to be skipped
	// eat away first several chars
	for (i = 0; i < i2; i++) {
		if (b)
			l1 = getCharResolved(vn,offset);
		else 
			l1 = getChar(vn,offset);
		offset += (int) (l1 >> 32);
	}
	//System.out.println(s);
	for (i = 0; i < l; i++) {
		if (b)
			l1 = getCharResolved(vn,offset);
		else
			l1 = getChar(vn,offset);
		i1 = s[i];
		if (i1 != (int) l1)
			return FALSE;
		offset += (int) (l1 >> 32);
	}	    
	return TRUE;
}

/*Test whether a given token contains s. notie that this function
directly operates on the byte content of the token to avoid string creation */

Boolean contains(VTDNav *vn, int index, UCSChar *s){
	tokenType type = getTokenType(vn,index);
	size_t len =
		(type == TOKEN_STARTING_TAG
		|| type == TOKEN_ATTR_NAME
		|| type == TOKEN_ATTR_NS)
		? getTokenLength(vn,index) & 0xffff
		: getTokenLength(vn,index);
	int offset = getTokenOffset(vn,index);
	Long l1;
	size_t l,i;
	int i1;
	int endOffset = offset + len;

	Boolean b = (type == TOKEN_CHARACTER_DATA 
		|| type == TOKEN_ATTR_VAL);
	//       System.out.print("currentOffset :" + currentOffset);
	int gOffset = offset;
	l = wcslen(s);
	if (l> len)
		return FALSE;
	//System.out.println(s);
	while( offset<endOffset){
		gOffset = offset;
		for (i = 0; i < l && gOffset < endOffset; i++) {
			if (b)
				l1 = getCharResolved(vn,gOffset);
			else
				l1 = getChar(vn,gOffset);
			i1 = s[i];
			gOffset += (int) (l1 >> 32);
			if (i ==0)
				offset = gOffset;
			if (i1 != (int) l1)
				break;				
		}
		if (i==l)
			return TRUE;
	}
	return FALSE;
}


UCSChar *toStringUpperCase2(VTDNav *vn, int os, int len){
	int offset = os, endOffset=os+len,k=0;
	Long l;
	UCSChar *s = (UCSChar *)malloc(sizeof(UCSChar)*(len+1));
	if (s == NULL)
	{
		throwException2(out_of_mem,
			" string allocation failed in toString ");
	}
	while (offset < endOffset) {
		l = getCharResolved(vn,offset);
		offset += (int)(l>>32);
		if ((int)l>96 && (int)l<123)
			s[k++] = (UCSChar)(l-32); // java only support 16 bit unit code
		else 
			s[k++] = (UCSChar)l;
	}
	s[k] = 0;
	return s;
}

UCSChar *toStringLowerCase2(VTDNav *vn, int os, int len){
	int offset = os, endOffset=os+len,k=0;
	Long l;
	UCSChar *s = (UCSChar *)malloc(sizeof(UCSChar)*(len+1));
	if (s == NULL)
	{
		throwException2(out_of_mem,
			" string allocation failed in toString ");
	}
	while (offset < endOffset) {
		l = getCharResolved(vn,offset);
		offset += (int)(l>>32);
		if ((int)l>64 && (int)l<91)
			s[k++] = (UCSChar)(l+32); // java only support 16 bit unit code
		else 
			s[k++] = (UCSChar)l; // java only support 16 bit unit code
	}
	s[k] = 0;
	return s;
}

/* Convert a token at the given index to a String and any lower case
   character will be converted to upper case, (entities and char
   references resolved).*/
UCSChar *toStringUpperCase(VTDNav *vn, int index){
	int offset,len;
	tokenType type = getTokenType(vn,index);

	/*UCSChar *s = NULL;*/
	if (type!=TOKEN_CHARACTER_DATA &&
		type!= TOKEN_ATTR_VAL)
		return toRawStringUpperCase(vn,index);

	len = getTokenLength(vn,index);

	offset = getTokenOffset(vn,index);
	return toStringUpperCase2(vn,offset,len);
}

/* Convert a token at the given index to a String and any upper case
   character will be converted to lower case, (entities and char
   references resolved).*/
UCSChar *toStringLowerCase(VTDNav *vn, int index){
	int offset,len;
	tokenType type = getTokenType(vn,index);

	/*UCSChar *s = NULL;*/
	if (type!=TOKEN_CHARACTER_DATA &&
		type!= TOKEN_ATTR_VAL)
		return toRawStringLowerCase(vn,index);

	len = getTokenLength(vn,index);

	offset = getTokenOffset(vn,index);
	return toStringLowerCase2(vn,offset,len);
}

UCSChar *toRawStringUpperCase(VTDNav *vn, int index){
	int offset,len;
	tokenType type = getTokenType(vn,index);					 
	/*UCSChar *s = NULL;*/

	if (type == TOKEN_STARTING_TAG
		|| type == TOKEN_ATTR_NAME
		|| type == TOKEN_ATTR_NS)
		len = getTokenLength(vn,index) & 0xffff;
	else
		len = getTokenLength(vn,index);

	offset = getTokenOffset(vn,index);
	return toRawStringUpperCase2(vn, offset, len);
}
UCSChar *toRawStringLowerCase(VTDNav *vn, int index){
	int offset,len;
	tokenType type = getTokenType(vn,index);						 
	/*UCSChar *s = NULL;*/

	if (type == TOKEN_STARTING_TAG
		|| type == TOKEN_ATTR_NAME
		|| type == TOKEN_ATTR_NS)
		len = getTokenLength(vn,index) & 0xffff;
	else
		len = getTokenLength(vn,index);

	offset = getTokenOffset(vn,index);
	return toRawStringLowerCase2(vn, offset, len);
}
static UCSChar *toRawStringUpperCase2(VTDNav *vn, int os, int len){
	int offset = os, endOffset=os+len,k=0;
	Long l;
	UCSChar *s = (UCSChar *)malloc(sizeof(UCSChar)*(len+1));
	if (s == NULL)
	{
		throwException2(out_of_mem,
			" string allocation failed in toString ");
	}
	while (offset < endOffset) {
		l = getChar(vn,offset);
		offset += (int)(l>>32);
		if ((int)l>96 && (int)l<123)
			s[k++] = (UCSChar)(l-32); // java only support 16 bit unit code
		else 
			s[k++] = (UCSChar)l;
	}
	s[k] = 0;
	return s;
}

static UCSChar *toRawStringLowerCase2(VTDNav *vn, int os, int len){
	int offset = os, endOffset=os+len,k=0;
	Long l;
	UCSChar *s = (UCSChar *)malloc(sizeof(UCSChar)*(len+1));
	if (s == NULL)
	{
		throwException2(out_of_mem,
			" string allocation failed in toString ");
	}
	while (offset < endOffset) {
		l = getChar(vn,offset);
		offset += (int)(l>>32);
		if ((int)l>64 && (int)l<91)
			s[k++] = (UCSChar)(l+32); // java only support 16 bit unit code
		else 
			s[k++] = (UCSChar)l; // java only support 16 bit unit code
	}
	s[k] = 0;
	return s;
}

static void resolveLC_l1(VTDNav *vn){
		int k;
		if (vn->l1index < 0
		|| vn->l1index >= vn->l1Buffer->size
		|| vn->context[1] != upper32At(vn->l1Buffer, vn->l1index)) {
			if (vn->l1index >= vn->l1Buffer->size || vn->l1index <0) {
				vn->l1index = 0;
			}
			if (vn->l1index+1< vn->l1Buffer->size 
				&& vn->context[1] != upper32At(vn->l1Buffer,vn->l1index + 1)) {
					int init_guess;
					k = vn->context[1];
					init_guess =
						(int) (vn->l1Buffer->size
						* ((float) /*vn->context[1]*/k / vn->vtdBuffer->size));
					if (upper32At(vn->l1Buffer,init_guess) > k /*vn->context[1]*/) {
						while (upper32At(vn->l1Buffer,init_guess)
							!= k /*vn->context[1]*/) {
								init_guess--;
						}
					} else if (
						upper32At(vn->l1Buffer,init_guess) < k /*vn->context[1]*/) {
							while (upper32At(vn->l1Buffer,init_guess)
								!= k /*vn->context[1]*/) {
									init_guess++;
							}
					}
					vn->l1index = init_guess;
			} else{
				if (vn->context[1]>=upper32At(vn->l1Buffer,vn->l1index)){
					while(vn->context[1]!=upper32At(vn->l1Buffer,vn->l1index)
						&& vn->l1index<vn->l1Buffer->size){
							vn->l1index++;								
					}
				} else {
					while(vn->context[1]!=upper32At(vn->l1Buffer,vn->l1index)
						&& vn->l1index >= 0){
							vn->l1index--;								
					}
				}

			}
			// for iterations, l1index+1 is the logical next value for l1index
	}
}
static void resolveLC_l2(VTDNav *vn){
	int temp = lower32At(vn->l1Buffer,vn->l1index),i,k;
	if (vn->l2lower != temp) {
		vn->l2lower = temp;
		// l2lower shouldn't be -1 !!!!  l2lower and l2upper always get resolved simultaneously
		vn->l2index = vn->l2lower;
		vn->l2upper = vn->l2Buffer->size - 1;
		k = vn->l1Buffer->size;
		for (i = vn->l1index + 1; i < k; i++) {
			temp = lower32At(vn->l1Buffer,i);
			if (temp != 0xffffffff) {
				vn->l2upper = temp - 1;
				break;
			}
		}
	} // intelligent guess again ??

	if (vn->l2index < 0
		|| vn->l2index >= vn->l2Buffer->size
		|| vn->context[2] != upper32At(vn->l2Buffer,vn->l2index)) {
			if (vn->l2index >= vn->l2Buffer->size || vn->l2index<0)
				vn->l2index = vn->l2lower;
			if (vn->l2index+1< vn->l2Buffer->size 
				&& vn->context[2] == upper32At(vn->l2Buffer,vn->l2index + 1))
				vn->l2index = vn->l2index + 1;
			else if (vn->l2upper - vn->l2lower >= 16) {
				int init_guess =
					vn->l2lower
					+ (int) ((vn->l2upper - vn->l2lower)
					* ((float) vn->context[2]
				- upper32At(vn->l2Buffer,vn->l2lower))
					/ (upper32At(vn->l2Buffer,vn->l2upper)
					- upper32At(vn->l2Buffer,vn->l2lower)));
				if (upper32At(vn->l2Buffer,init_guess) > vn->context[2]) {
					while (vn->context[2]
					!= upper32At(vn->l2Buffer,init_guess))
						init_guess--;
				} else if (
					upper32At(vn->l2Buffer,init_guess) < vn->context[2]) {
						while (vn->context[2]
						!= upper32At(vn->l2Buffer,init_guess))
							init_guess++;
				}
				vn->l2index = init_guess;
			} else if (vn->context[2]<upper32At(vn->l2Buffer,vn->l2index)){
				while (vn->context[2] != upper32At(vn->l2Buffer,vn->l2index)) {
					vn->l2index--;
				}
			}
			else {
				while (vn->context[2] != upper32At(vn->l2Buffer,vn->l2index)) {
					vn->l2index++;
				}
			}
	}
}

static void resolveLC_l3(VTDNav *vn){
	int i,k,temp = lower32At(vn->l2Buffer,vn->l2index);
	k = vn->l2Buffer->size;
	if (vn->l3lower != temp) {
		//l3lower and l3upper are always together
		vn->l3lower = temp;
		// l3lower shouldn't be -1
		vn->l3index = vn->l3lower;
		vn->l3upper = vn->l3Buffer->size - 1;
		for (i = vn->l2index + 1; i < k; i++) {
			temp = lower32At(vn->l2Buffer,i);
			if (temp != 0xffffffff) {
				vn->l3upper = temp - 1;
				break;
			}
		}
	}

	if (vn->l3index < 0
		|| vn->l3index >= vn->l3Buffer->size
		|| vn->context[3] != intAt(vn->l3Buffer,vn->l3index)) {
			if (vn->l3index >= vn->l3Buffer->size || vn->l3index<0)
				vn->l3index = vn->l3lower;
			if (vn->l3index+1 < vn->l3Buffer->size  
				&& vn->context[3] == intAt(vn->l3Buffer,vn->l3index + 1))
				vn->l3index = vn->l3index + 1;
			else if (vn->l3upper - vn->l3lower >= 16) {
				int init_guess =
					vn->l3lower
					+ (int) ((vn->l3upper - vn->l3lower)
					* ((float) (vn->context[3]
				- intAt(vn->l3Buffer,vn->l3lower))
					/ (intAt(vn->l3Buffer,vn->l3upper)
					- intAt(vn->l3Buffer,vn->l3lower))));
				if (intAt(vn->l3Buffer,init_guess) > vn->context[3]) {
					while (vn->context[3] != intAt(vn->l3Buffer, init_guess))
						init_guess--;
				} else if (intAt(vn->l3Buffer,init_guess) < vn->context[3]) {
					while (vn->context[3] != intAt(vn->l3Buffer,init_guess))
						init_guess++;
				}
				vn->l3index = init_guess;
			} else if (vn->context[3] < intAt(vn->l3Buffer, vn->l3index)){
				while (vn->context[3] != intAt(vn->l3Buffer,vn->l3index)) {
					vn->l3index--;
				}
			} else {
				while (vn->context[3] != intAt(vn->l3Buffer,vn->l3index)) {
					vn->l3index++;
				}
			}
	}
}


/* DupliateNav duplicates an instance of VTDNav but doesn't retain the original node position*/
VTDNav* duplicateNav(VTDNav *vn){
	VTDNav* vn1 = createVTDNav( vn->rootIndex,
		vn->encoding,
		vn->ns,
		vn->nestingLevel-1,
		vn->XMLDoc,
		vn->bufLen,
		vn->vtdBuffer,
		vn->l1Buffer,
		vn->l2Buffer,
		vn->l3Buffer,
		vn->docOffset,
		vn->docLen,
		vn->br);	
	vn1->master = FALSE;
	return vn1;
}


/* ClineNav duplicates an instance of VTDNav, also copies node position over */
VTDNav *cloneNav(VTDNav *vn){
	VTDNav* vn1 = createVTDNav( vn->rootIndex,
		vn->encoding,
		vn->ns,
		vn->nestingLevel-1,
		vn->XMLDoc,
		vn->bufLen,
		vn->vtdBuffer,
		vn->l1Buffer,
		vn->l2Buffer,
		vn->l3Buffer,
		vn->docOffset,
		vn->docLen,
		vn->br);	
	vn1->master = FALSE;
	vn1->atTerminal = vn->atTerminal;
	vn1->LN = vn->LN;
	if (vn->context[0]!=-1)
		memcpy(vn1->context,vn->context, vn->context[0]);
		//System.arraycopy(vn.context, 0, vn.context, 0, this.context[0] );
	else 
		vn1->context[0]=-1;
	vn1->l1index = vn->l1index; 
	if (getCurrentDepth(vn)>1){
		vn1->l2index = vn->l2index;
		vn1->l2upper = vn->l2upper;
		vn1->l2lower = vn->l2lower;
	}
	if (getCurrentDepth(vn) > 2) {
		vn1->l3lower = vn->l3lower;
		vn1->l3index = vn->l3index;
		vn1->l3upper = vn->l3upper;
	}
	return vn1;
}

/* This method takes a vtd index, and recover its correspondin
 * node position, the index can only be of node type element,
 * document, attribute name, attribute value or character data,
 * or CDATA  */
void recoverNode(VTDNav *vn, int index){
	int d, type,t;
	//exception e;
	if (index <0 || index>=vn->vtdSize ){
		throwException2(invalid_argument,"Invalid VTD index");
		//throw new NavException("Invalid VTD index");
	}

	type = getTokenType(vn,index);

	if (//type == VTDNav.TOKEN_COMMENT ||
		//	type == VTDNav.TOKEN_PI_NAME ||
		type == TOKEN_PI_VAL ||
		type == TOKEN_DEC_ATTR_NAME ||
		type == TOKEN_DEC_ATTR_VAL ||
		type == TOKEN_ATTR_VAL){
			throwException2( nav_exception, "Invalid VTD index");
			//throw new NavException("Token type not yet supported");
	}
	// get depth
	d = getTokenDepth(vn,index);
	// handle document node;	
	switch (d){
		case -1:
			vn->context[0]=-1;
			if (index != 0){
				vn->LN = index;
				vn->atTerminal = TRUE;
			}			
			return;
		case 0:
			vn->context[0]=0;
			if (index != vn->rootIndex){
				vn->LN = index;
				vn->atTerminal = TRUE;
			}
			return;		
	}
	vn->context[0]=d;
	if (type != TOKEN_STARTING_TAG){
		vn->LN = index;
		vn->atTerminal = TRUE;
	}
	// search LC level 1
	recoverNode_l1(vn,index);

	if (d==1)
		return;
	// search LC level 2
	recoverNode_l2(vn,index);
	if (d==2){
		//resolveLC();
		return;
	}
	// search LC level 3
	recoverNode_l3(vn,index);
	if (d==3){
		//resolveLC();
		return;
	}
	// scan backward
	if ( type == TOKEN_STARTING_TAG ){
		vn->context[d] = index;
	} else{
		int t = index-1;
		while( !(getTokenType(vn,t)==TOKEN_STARTING_TAG && 
			getTokenDepth(vn,t)==d)){
				t--;
		}
		vn->context[d] = t;
	}
	t = vn->context[d]-1;
	d--;
	while(d>3){
		while( !(getTokenType(vn,t)==TOKEN_STARTING_TAG && 
			getTokenDepth(vn,t)==d)){
				t--;
		}
		vn->context[d] = t;
		d--;
	}
}

static void recoverNode_l1(VTDNav *vn,int index){
	int i;
	if(vn->context[1]==index){

	}
	else if (vn->context[1]>index 
		&& vn->l1index+1<vn->l1Buffer->size
		&& upper32At(vn->l1Buffer,vn->l1index+1) < index){

	}
	else {
		i= (index/vn->vtdSize)*vn->l1Buffer->size;
		if (i>=vn->l1Buffer->size)
			i=vn->l1Buffer->size-1;

		if (upper32At(vn->l1Buffer,i)< index) {
			while(i<vn->l1Buffer->size-1 && 
				upper32At(vn->l1Buffer,i)<index){
					i++;
			}
			if (upper32At(vn->l1Buffer,i)>index)
				i--;
		} else {
			while(upper32At(vn->l1Buffer,i)>index){
				i--;
			}
		}	
		vn->context[1] = upper32At(vn->l1Buffer,i);
		vn->l1index = i;
	}
}

static void recoverNode_l2(VTDNav *vn,int index){
	int i = lower32At(vn->l1Buffer,vn->l1index),k,t1,t2;

	if (vn->l2lower != i) {
		vn->l2lower = i;
		// l2lower shouldn't be -1 !!!! l2lower and l2upper always get
		// resolved simultaneously
		//l2index = l2lower;
		vn->l2upper = vn->l2Buffer->size - 1;
		for (k = vn->l1index + 1; k < vn->l1Buffer->size; k++) {
			i = lower32At(vn->l1Buffer,k);
			if (i != 0xffffffff) {
				vn->l2upper = i - 1;
				break;
			}
		}
	}
	// guess what i would be in l2 cache
	t1=upper32At(vn->l2Buffer, vn->l2lower);
	t2=upper32At(vn->l2Buffer, vn->l2upper);
	//System.out.print("   t2  ==>"+t2+"   t1  ==>"+t1);
	i= min(vn->l2lower+ 
		(int)(((float)(index-t1)/(t2-t1+1))*(vn->l2upper-vn->l2lower))
		,vn->l2upper) ;
	//System.out.print("  i1  "+i);
	while(i<vn->l2Buffer->size-1 && upper32At(vn->l2Buffer,i)<index){
		i++;	
	}
	//System.out.println(" ==== i2    "+i+"    index  ==>  "+index);

	while (upper32At(vn->l2Buffer,i)>index && i>0)
		i--;
	vn->context[2] = upper32At(vn->l2Buffer,i);
	vn->l2index = i;
}

static void recoverNode_l3(VTDNav *vn,int index){
int i = lower32At(vn->l2Buffer,vn->l2index),k,t1,t2;
		
		if (vn->l3lower != i) {
			//l3lower and l3upper are always together
			vn->l3lower = i;
			// l3lower shouldn't be -1
			//l3index = l3lower;
			vn->l3upper = vn->l3Buffer->size - 1;
			for (k = vn->l2index + 1; k < vn->l2Buffer->size; k++) {
				i = lower32At(vn->l2Buffer,k);
				if (i != 0xffffffff) {
					vn->l3upper = i - 1;
					break;
				}
			}
		}
		t1=intAt(vn->l3Buffer,vn->l3lower);
		t2=intAt(vn->l3Buffer,vn->l3upper);

		i= min(vn->l3lower+ (int)(((float)(index-t1)/(t2-t1+1))*(vn->l3upper-vn->l3lower)),vn->l3upper) ;
		while(i<vn->l3Buffer->size-1 && intAt(vn->l3Buffer,i)<index){
			i++;	
		}
		while (intAt(vn->l3Buffer,i)>index && i>0)
			i--;
		//System.out.println(" i ===> "+i);
		vn->context[3] = intAt(vn->l3Buffer,i);
		vn->l3index = i;
}

/**
	 * Match the string against the token at the given index value. The token will be
     * interpreted as if it is normalized (i.e. all white space char (\r\n\a ) is replaced
     * by a white space, char entities and entity references will be replaced by their correspondin
     * char see xml 1.0 spec interpretation of attribute value normalization) */

Boolean matchNormalizedTokenString2(VTDNav *vn,int index, UCSChar *s){
		tokenType type = getTokenType(vn,index);
		int len =
			(type == TOKEN_STARTING_TAG
				|| type == TOKEN_ATTR_NAME
				|| type == TOKEN_ATTR_NS)
				? getTokenLength(vn,index) & 0xffff
				: getTokenLength(vn,index);

		return compareNormalizedTokenString2(vn,getTokenOffset(vn,index), len, s)==0;
}

int compareNormalizedTokenString2(VTDNav *vn,int offset, int len,
			UCSChar *s) {
		int i,i1, l, temp;
		Long l1,l2;
		//boolean b = false;
		// this.currentOffset = offset;
		int endOffset = offset + len;

		// System.out.print("currentOffset :" + currentOffset);
		l = wcslen(s);
		// System.out.println(s);
		for (i = 0; i < l && offset < endOffset;) {
			l1 = getCharResolved(vn,offset);
			temp = (int) l1;
			l2 = (l1>>32);
			if (l2<=2 && isWS(temp))
				temp = ' ';
			i1 = s[i];
			if (i1 < temp)
				return 1;
			if (i1 > temp)
				return -1;
			i++;			
			offset += (int) (l1 >> 32);
		}

		if (i == l && offset < endOffset)
			return 1;
		if (i < l && offset == endOffset)
			return -1;
		return 0;
		// return -1;
	}

/**
	 * Return the byte offset and length of up to i sibling fragments. If 
	 * there is a i+1 sibling element, the cursor element would 
	 * move to it; otherwise, there is no cursor movement. If the cursor isn't 
	 * positioned at an element (due to XPath evaluation), then -1 will be 
	 * returned
	 * @param i number of silbing elements including the cursor element
	 * @return a long encoding byte offset (bit 31 to bit 0), length (bit 62 
	 * to bit 32) of those fragments 
	 * @throws NavException
	 */

Long getSiblingElementFragments(VTDNav *vn,int i){
	int so, len;
	Long l;
	BookMark *bm;
		if (i<=0)
			throwException2(invalid_argument," # of sibling can be less or equal to 0");
		// get starting char offset
		if(vn->atTerminal==TRUE)
			return -1L;
		// so is the char offset
		so = getTokenOffset(vn,getCurrentIndex(vn))-1;
		// char offset to byte offset conversion
		if (vn->encoding>=FORMAT_UTF_16BE)
			so = so<<1;
		bm = createBookMark(vn);
		recordCursorPosition(bm,vn);
		while(i>1 && toElement(vn,NEXT_SIBLING)){
			i--;
		}
		l= getElementFragment(vn);
		len = (int)l+(int)(l>>32)-so;
		if (i==1 && toElement(vn,NEXT_SIBLING)){
		}else
			setCursorPosition(bm,vn);
		freeBookMark(bm);
		return (((Long)len)<<32)|so;
}

/**
	 * Match the string against the token at the given index value. The token will be
     * interpreted as if it is normalized (i.e. all white space char (\r\n\a ) is replaced
     * by a white space, char entities and entity references will be replaced by their correspondin
     * char see xml 1.0 spec interpretation of attribute value normalization) 
	 */
//Boolean matchNormalizedTokenString2(VTDNav *vn, int index, UCSChar *s){

//}

/**
	 * (New since version 2.9)
	 * Shallow Normalization follows the rules below to normalize a token into
	 * a string
	 * *#xD#xA gets converted to #xA
	 * *For a character reference, append the referenced character to the normalized value.
	 * *For an entity reference, recursively apply step 3 of this algorithm to the replacement text of the entity.
	 * *For a white space character (#x20, #xD, #xA, #x9), append a space character (#x20) to the normalized value.
	 * *For another character, append the character to the normalized value.*/
UCSChar* toNormalizedString2(VTDNav *vn, int index){
		tokenType type = getTokenType(vn,index);
		Long l;
		UCSChar *s;int ch,k;
		int len,offset, endOffset;
		if (type!=TOKEN_CHARACTER_DATA &&
				type!= TOKEN_ATTR_VAL)
			return toRawString(vn,index); 
		
		len = getTokenLength(vn,index);
		if (len == 0)
			return wcsdup(L"");
		offset = getTokenOffset(vn,index);
		endOffset = len + offset - 1; // point to the last character

		s = (UCSChar *)malloc(sizeof(UCSChar)*(len+1));
		if (s == NULL)
		{
			throwException2(out_of_mem,
				" string allocation failed in toNormalizedString2 ");
		}
		
		
		

		//boolean d = false;
		while (offset <= endOffset) {
			l = getCharResolved(vn,offset);
			ch = (int)l;
			offset += (int)(l>>32);
			if (isWS(ch) && (l>>32)<=2) {
				//d = true;
				s[k++] = (UCSChar)' ';
				//sb.append(' ');
			} else{
				s[k++] = (UCSChar)ch;
				//sb.append((char) ch);
			}
		}
		return s;
}


UCSChar *getPrefixString(VTDNav *vn, int i){
	if (vn->ns == FALSE)
			return NULL;
	{
		tokenType type = getTokenType(vn,i);
		if (type != TOKEN_ATTR_NAME && type != TOKEN_STARTING_TAG)
			return NULL;
		else {
			int offset = getTokenOffset(vn,i);
			int preLen = getTokenLength(vn,i) >> 16;
			if (preLen !=0)
				return toRawString2(vn,offset,preLen);
		}
		return NULL;
	}
}
