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

#include "vtdNav.h"

static int getChar(VTDNav *vn);
static int getCharResolved(VTDNav *vn);
static int getCharUnit(VTDNav *vn, int index);
static inline Boolean isElement(VTDNav  *vn, int index);
static inline Boolean isWS(int ch);
static Boolean matchRawTokenString1(VTDNav *vn, int offset, int len, UCS2Char *s);
static Boolean matchRawTokenString2(VTDNav *vn, Long l, UCS2Char *s);
static Boolean matchTokenString1(VTDNav *vn, int offset, int len, UCS2Char *s);
static Boolean matchTokenString2(VTDNav *vn, Long l, UCS2Char *s);
static inline int NSval(VTDNav *vn, int i);
static int parseInt2(VTDNav *vn, int index, int radix);
static Long parseLong2(VTDNav *vn, int index, int radix);
static void resolveLC(VTDNav *vn);
static Boolean resolveNS(VTDNav *vn, UCS2Char *URL);
static Boolean resolveNS2(VTDNav *vn, UCS2Char *URL, int offset, int len); //UCS2Char *ln);



//Create VTDNav object

VTDNav *createVTDNav(int r, encoding enc, Boolean ns, int depth,
					 Byte *x, int xLen, FastLongBuffer *vtd, FastLongBuffer *l1,
					 FastLongBuffer *l2, FastIntBuffer *l3, int so, int len){
                         						
						 exception e;
						 VTDNav* vn = NULL;
						 int i;

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
							 e.et = invalid_argument;
							 e.msg = " invalid argument when creating VTDGen failed ";
							 Throw e;							
						 }

						 vn = (VTDNav *) malloc(sizeof(VTDNav));
						 if (vn==NULL){
							 e.et = out_of_mem;
							 e.msg = "VTDNav allocation failed ";
							 Throw e;
						 }
						 vn->l1Buffer = l1;
						 vn->l2Buffer = l2;
						 vn->l3Buffer = l3;
						 vn->vtdBuffer= vtd;
						 vn->XMLDoc = x;

						 vn->encoding = enc;
						 vn->nestingLevel = depth +1;

						 vn->ns = ns;
						 vn->context = (int *)malloc(vn->nestingLevel*sizeof(int));
						 if (vn->context == NULL){
							 free(vn);
							 e.et = out_of_mem;
							 e.msg = "VTDNav allocation failed ";
							 Throw e;
						 }
						 vn->context[0] = 0;
						 for (i=0;i<vn->nestingLevel;i++){
							 vn->context[i] = -1;
						 }
						 vn->currentOffset = 0;

						 vn->contextBuf = createContextBuffer2(10, vn->nestingLevel+7);
						 vn->stackTemp = (int *)malloc((vn->nestingLevel+7)*sizeof(int));

						 if (vn->contextBuf == NULL 
							 || vn->stackTemp == NULL){
								 freeContextBuffer(vn->contextBuf);
								 free(vn->stackTemp);
								 free(vn->context);
								 free(vn);
								 e.et = out_of_mem;
								 e.msg = "VTDNav allocation failed ";
								 Throw e;
							 }
							 vn->l1index = vn->l2index = vn->l3index = -1;
							 vn->l2lower = vn->l2upper = -1;
							 vn->l3lower = vn->l3upper = -1;
							 vn->offset = so;
							 vn->docLen = len;
							 vn->vtdSize = vtd->size;
							 vn->bufLen = xLen;

							 return vn;
					 }

					 //Free VTDNav object
					 void freeVTDNav(VTDNav *vn)
					 {	
						 if (vn!=NULL){
							 freeContextBuffer(vn->contextBuf);
							 freeFastLongBuffer(vn->vtdBuffer);
							 freeFastLongBuffer(vn->l1Buffer);
							 freeFastLongBuffer(vn->l2Buffer);
							 freeFastIntBuffer(vn->l3Buffer);
							 free(vn->context);
							 free(vn->stackTemp);
							 free(vn->XMLDoc);
						 }
						 free(vn);
					 }

					 //Return the attribute count of the element at the cursor position.
					 int getAttrCount(VTDNav *vn){
						 int count = 0;
						 int index = getCurrentIndex(vn) + 1;
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
					 int getAttrVal(VTDNav *vn, UCS2Char *an){
						 int size = vn->vtdBuffer->size;
						 int index = (vn->context[0] != 0) ? vn->context[vn->context[0]] + 1 : vn->rootIndex + 1;

						 int type = getTokenType(vn,index);
						 if (vn->ns == FALSE) {
							 while (index < size
								 && (type == TOKEN_ATTR_NAME || type == TOKEN_ATTR_NS)) {
									 if (matchRawTokenString(vn,index,
										 an)) { // ns node visible only ns is disabled
											 return index + 1;
										 }
										 index += 2;
										 type = getTokenType(vn,index);
								 }
						 } else {
							 while (index < size
								 && (type == TOKEN_ATTR_NAME || type == TOKEN_ATTR_NS)) {
									 if (type == TOKEN_ATTR_NAME
										 && matchRawTokenString(vn,
										 index,
										 an)) { // ns node visible only ns is disabled
											 return index + 1;
										 }
										 index += 2;
										 type = getTokenType(vn,index);
								 }
						 }
						 return -1;
					 }

					 //Get the token index of the attribute value of given URL and local name.
					 //If ns is not enabled, the lookup will return -1, indicating a no-found.
					 //Also namespace nodes are invisible using this method.
					 int getAttrValNS(VTDNav *vn, UCS2Char* URL, UCS2Char *ln){

						 int size, index;
						 tokenType type;
						 if (vn->ns == FALSE)
							 return -1;
						 if (URL == NULL)
							 return getAttrVal(vn,ln);
						 size = vn->vtdBuffer->size;
						 index = (vn->context[0] != 0) ? vn->context[vn->context[0]] + 1 : vn->rootIndex + 1;
						 // point to the token next to the element tag
						 type = getTokenType(vn,index);
						 while (index < size
							 && (type == TOKEN_ATTR_NAME || type == TOKEN_ATTR_NS)) {
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
									 }
									 index += 2;
									 type = getTokenType(vn,index);
							 }
							 return -1;
					 }
					 //This function decodes the underlying byte array into corresponding 
					 //UCS2 char representation .
					 //It doesn't resolves built-in entity and character references.
					 //Length will never be zero
					 static int getChar(VTDNav *vn){
						 exception e;
						 int temp = 0;
						 int a, c, d;
						 int val,i;
						 //int ch;
						 a = c = d = val = 0;

						 switch (vn->encoding) {
			case FORMAT_ASCII : // ascii is compatible with UTF-8, the offset value is bytes
			case FORMAT_UTF8 :
				temp = vn->XMLDoc[vn->currentOffset] & 0xff;

				switch (UTF8Char_byteCount(temp)) {
			case 1 :
				vn->currentOffset++;
				return temp;
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
			default :
				e.et = nav_exception;
				e.msg = "navigation exception during getChar";
				e.sub_msg = "UTF 8 encoding error: should never happen";
				Throw e;
				//throw new NavException("UTF 8 encoding error: should never happen");
				}

				val = (temp & c) << d;
				i = a - 1;
				while (i >= 0) {
					temp = vn->XMLDoc[vn->currentOffset + a - i];
					if ((temp & 0xc0) != 0x80){
						e.et = nav_exception;
						e.msg = "navigation exception during getChar";
						e.sub_msg = "UTF 8 encoding error: should never happen";
						Throw e;
					}
					//throw new NavException("UTF 8 encoding error: should never happen");
					val = val | ((temp & 0x3f) << i);
					i--;
				}
				vn->currentOffset += a + 1;
				return val;

			case FORMAT_ISO_8859 :
				temp = vn->XMLDoc[vn->currentOffset];
				vn->currentOffset++;
				return temp & 0xff;

			case FORMAT_UTF_16BE :
				// implement UTF-16BE to UCS4 conversion
				temp =
					vn->XMLDoc[vn->currentOffset << 1]
					<< 8 | vn->XMLDoc[(vn->currentOffset << 1) + 1];
					if ((temp < 0xd800)
						|| (temp >= 0xdc00)) { // not a high surrogate
							vn->currentOffset += 1;
							return temp;
						} else {
							val = temp;
							temp =
								vn->XMLDoc[(vn->currentOffset << 1) + 2]
								<< 8 | vn->XMLDoc[(vn->currentOffset << 1) + 3];
								if (temp < 0xdc00 || temp > 0xdfff) {
									// has to be a low surrogate here
									e.et = nav_exception;
									e.msg = "navigation exception during getChar";
									e.sub_msg = "UTF 16 BE encoding error: should never happen";
									Throw e;
									//throw new NavException("UTF 16 BE encoding error: should never happen");
								}
								val = (val - 0xd800) * 0x400 + (temp - 0xdc00) + 0x10000;
								vn->currentOffset += 2;
								return val;
						}

			case FORMAT_UTF_16LE :
				// implement UTF-16LE to UCS4 conversion
				temp =
					vn->XMLDoc[(vn->currentOffset << 1) + 1]
					<< 8 | vn->XMLDoc[vn->currentOffset << 1];
					if (temp < 0xdc00 || temp > 0xdfff) { // check for low surrogate
						vn->currentOffset += 1;
						return temp;
					} else {
						val = temp;
						temp =
							(vn->XMLDoc[(vn->currentOffset << 1) + 3]
							<< 8) | vn->XMLDoc[(vn->currentOffset << 1) + 2];
							if (temp < 0xd800 || temp > 0xdc00) {
								// has to be high surrogate
								e.et = nav_exception;
								e.msg = "navigation exception during getChar";
								e.sub_msg = "UTF 16 LE encoding error: should never happen";
								Throw e;
								//throw new NavException("UTF 16 BE encoding error: should never happen");
							}
							val = (temp - 0xd800) * 0x400 + (val - 0xdc00) + 0x10000;
							vn->currentOffset += 2;
							return val;
					}
					//System.out.println("UTF 16 LE unimplemented for now");

			default :
				e.et = nav_exception;
				e.msg = "navigation exception during getChar";
				e.sub_msg = "Unknown Encoding";
				Throw e;
				//throw new NavException("Unknown Encoding");
						 }


					 }

					 //This method decodes the underlying byte array into corresponding 
					 //UCS2 char representation .
					 //Also it resolves built-in entity and character references.
					 static int getCharResolved(VTDNav *vn){
						 exception e;
						 int ch = 0;
						 int val = 0;
						 ch = getChar(vn);
						 if (ch != '&')
							 return ch;

						 // let us handle references here
						 //currentOffset++;
						 ch = getCharUnit(vn,vn->currentOffset);
						 vn->currentOffset++;
						 switch (ch) {
			case '#' :

				ch = getCharUnit(vn,vn->currentOffset);

				if (ch == 'x') {
					while (TRUE) {
						vn->currentOffset++;
						ch = getCharUnit(vn,vn->currentOffset);

						if (ch >= '0' && ch <= '9') {
							val = (val << 4) + (ch - '0');
						} else if (ch >= 'a' && ch <= 'f') {
							val = (val << 4) + (ch - 'a' + 10);
						} else if (ch >= 'A' && ch <= 'F') {
							val = (val << 4) + (ch - 'A' + 10);
						} else if (ch == ';') {
							vn->currentOffset++;
							break;
						} else{
							e.et = nav_exception;
							e.msg = "navigation exception during getCharResolved";
							e.sub_msg = "Illegal char in a char reference";
							Throw e;
						}
						//throw new NavException("Illegal char in a char reference");
					}
				} else {
					while (TRUE) {

						ch = getCharUnit(vn,vn->currentOffset);

						if (ch >= '0' && ch <= '9') {
							val = val * 10 + (ch - '0');
						} else if (ch == ';') {
							vn->currentOffset++;
							break;
						} else{
							e.et = nav_exception;
							e.msg = "navigation exception during getCharResolved";
							e.sub_msg = "Illegal char in a char reference";
							Throw e;
						}
						//throw new NavException("Illegal char in char reference");
						vn->currentOffset++;
					}
				}
				break;

			case 'a' :
				ch = getCharUnit(vn, vn->currentOffset);
				if (ch == 'm') {
					if (getCharUnit(vn, vn->currentOffset + 1) == 'p'
						&& getCharUnit(vn, vn->currentOffset + 2) == ';') {
							vn->currentOffset += 3;
							val = '&';
						} else{
							e.et = nav_exception;
							e.msg = "navigation exception during getCharResolved";
							e.sub_msg = "illegal builtin reference";
							Throw e;
						}
						//	throw new NavException("illegal builtin reference");
				} else if (ch == 'p') {
					if (getCharUnit(vn,vn->currentOffset + 1) == 'o'
						&& getCharUnit(vn,vn->currentOffset + 2) == 's'
						&& getCharUnit(vn,vn->currentOffset + 3) == ';') {
							vn->currentOffset += 4;
							val = '\'';
						} else{
							e.et = nav_exception;
							e.msg = "navigation exception during getCharResolved";
							e.sub_msg = "illegal builtin reference";
							Throw e;
						}
						//throw new NavException("illegal builtin reference");
				} else{
					e.et = nav_exception;
					e.msg = "navigation exception during getCharResolved";
					e.sub_msg = "illegal builtin reference";
					Throw e;
				}
				//	throw new NavException("illegal builtin reference");
				break;

			case 'q' :

				if (getCharUnit(vn,vn->currentOffset) == 'u'
					&& getCharUnit(vn,vn->currentOffset + 1) == 'o'
					&& getCharUnit(vn,vn->currentOffset + 2) == 't'
					&& getCharUnit(vn,vn->currentOffset + 3) == ';') {
						vn->currentOffset += 4;
						val = '\"';
					} else{
						e.et = nav_exception;
						e.msg = "navigation exception during getCharResolved";
						e.sub_msg = "illegal builtin reference";
						Throw e;
					}
					//throw new NavException("illegal builtin reference");
					break;
			case 'l' :
				if (getCharUnit(vn,vn->currentOffset) == 't'
					&& getCharUnit(vn,vn->currentOffset + 1) == ';') {
						vn->currentOffset += 2;
						val = '<';
					} else{
						e.et = nav_exception;
						e.msg = "navigation exception during getCharResolved";
						e.sub_msg = "illegal builtin reference";
						Throw e;
					}
					//throw new NavException("illegal builtin reference");
					break;
			case 'g' :
				if (getCharUnit(vn,vn->currentOffset) == 't'
					&& getCharUnit(vn,vn->currentOffset + 1) == ';') {
						vn->currentOffset += 2;
						val = '>';
					} else{
						e.et = nav_exception;
						e.msg = "navigation exception during getCharResolved";
						e.sub_msg = "illegal builtin reference";
						Throw e;
					}
					//throw new NavException("illegal builtin reference");
					break;

			default :

				e.et = nav_exception;
				e.msg = "navigation exception during getCharResolved";
				e.sub_msg = "Invalid entity char";
				Throw e;

				//throw new NavException("Invalid entity char");

						 }

						 //currentOffset++;
						 return val;
					 }

					 //Get the next char unit which gets decoded automatically
					 static int getCharUnit(VTDNav *vn, int offset){
						 return (vn->encoding < 3)
							 ? vn->XMLDoc[offset] & 0xff
							 : (vn->encoding == FORMAT_UTF_16BE)
							 ? (vn->XMLDoc[offset << 1]
							 << 8 | vn->XMLDoc[(offset << 1) + 1])
								 : (vn->XMLDoc[(offset << 1) + 1]
								 << 8 | vn->XMLDoc[offset << 1]);
					 }

					 //Get the depth (>=0) of the current element.
					 /*inline int getCurrentDepth(VTDNav *vn){
						 return vn->context[0];
					 }*/

					 // Get the index value of the current element.
					 /* int getCurrentIndex(VTDNav *vn){
					 return (vn->context[0] == 0) ? vn->rootIndex : vn->context[vn->context[0]];
					 }*/

					 // Get the starting offset and length of an element
					 // encoded in a long, upper 32 bit is length; lower 32 bit is offset
					 Long getElementFragment(VTDNav *vn){
						 // a little scanning is needed
						 // has next sibling case
						 // if not
						 int depth = getCurrentDepth(vn);
						 int so = getTokenOffset(vn,getCurrentIndex(vn)) - 1;
						 int length = 0;
						 int temp;
						 int size, so2, d, i;

						 // for an element with next sibling
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
							 if (vn->encoding < 3)
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
								 (vn->encoding < 3)
								 ? (vn->offset + vn->docLen - 1)
								 : ((vn->offset + vn->docLen) << 1) - 1;
							 else
								 so2 = getTokenOffset(vn,temp + 1);
							 while (getCharUnit(vn,so2) != '>') {
								 so2--;
							 }
							 length = so2 - so + 1;
							 if (vn->encoding < 3)
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
								 if (vn->encoding < 3)
									 return ((Long) length) << 32 | so;
								 else
									 return ((Long) length) << 33 | (so << 1);
							 }
						 }
						 // temp is the last entry
						 // scan forward search for /> or </cc>
						 so2 =
							 (vn->encoding < 3)
							 ? (vn->offset + vn->docLen - 1)
							 : ((vn->offset + vn->docLen) << 1) - 1;
						 d = depth + 1;
						 i = 0;
						 while (i < d) {
							 if (getCharUnit(vn,so2) == '>') {
								 i++;
							 }
							 so2--;
						 }

						 length = so2 - so + 2;

						 if (vn->encoding < 3)
							 return ((Long) length) << 32 | so;
						 else
							 return ((Long) length) << 33 | (so << 1);
						 }

					 /**
					 * Get the encoding of the XML document.
					 * <pre>   0  ASCII       </pre>
					 * <pre>   1  ISO-8859-1  </pre>
					 * <pre>   2  UTF-8       </pre>
					 * <pre>   3  UTF-16BE    </pre>
					 * <pre>   4  UTF-16LE    </pre>
					 */
					/* inline encoding getEncoding(VTDNav *vn){
						 return vn->encoding;
					 }*/

					 // Get the maximum nesting depth of the XML document (>0).
					 // max depth is nestingLevel -1
					 /*inline int getNestingLevel(VTDNav *vn){
						 return vn->nestingLevel;
					 }*/

					 // Get root index value.
					/* inline int getRootIndex(VTDNav *vn){
						 return vn->rootIndex;
					 }*/


					 // This function returns of the token index of the type character data or CDATA.
					 // Notice that it is intended to support data orient XML (not mixed-content XML).
					 int getText(VTDNav *vn){
						 int index = (vn->context[0] != 0) ? 
							 vn->context[vn->context[0]] + 1 : vn->rootIndex + 1;
						 int depth = getCurrentDepth(vn);
						 tokenType type = getTokenType(vn,index);

						 while (index < vn->vtdSize) {
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
								 type = getTokenType(vn, index);
						 }
						 return -1;
					 }

					 //Get total number of VTD tokens for the current XML document.
					 /*inline int getTokenCount(VTDNav *vn){
						 return vn->vtdSize;
					 }*/

					 //Get the depth value of a token (>=0)
					 int getTokenDepth(VTDNav *vn, int index){
#if BIG_ENDIAN
						 int i = (int) ((longAt(vn->vtdBuffer,index) & MASK_TOKEN_DEPTH) >> 52);
#else
#endif
						 if (i != 255)
							 return i;
						 return -1;
					 }

					 //Get the token length at the given index value
					 //please refer to VTD spec for more details
					 int getTokenLength(VTDNav *vn, int index){
						 int type = getTokenType(vn,index);
						 //int val;
						 switch (type) {
			case TOKEN_ATTR_NAME :
			case TOKEN_ATTR_NS :
			case TOKEN_STARTING_TAG :
				return (vn->ns == FALSE)
#if BIG_ENDIAN
					? (int) ((longAt(vn->vtdBuffer,index) & MASK_TOKEN_QN_LEN) >> 32)
					: ((int) ((longAt(vn->vtdBuffer, index) & MASK_TOKEN_QN_LEN)
					>> 32)
					| ((int) ((longAt(vn->vtdBuffer,index) & MASK_TOKEN_PRE_LEN)
					>> 32)
					<< 5));
#else
#endif
			default :
#if BIG_ENDIAN
				return (int)
					((longAt(vn->vtdBuffer,index) & MASK_TOKEN_FULL_LEN) >> 32);
#else
#endif
						 }
					 }

					 //Get the starting offset of the token at the given index.
//					 int getTokenOffset(VTDNav *vn, int index){
//#if BIG_ENDIAN
//                         return (int) (longAt(vn->vtdBuffer,index) & MASK_TOKEN_OFFSET);
//#else
//
//#endif
//					 }

					 //Get the XML document 
					 /*inline Byte* getXML(VTDNav *vn){
						 return vn->XMLDoc;
					 }*/

//					 //Get the token type of the token at the given index value.
//					 tokenType getTokenType(VTDNav *vn, int index){
//#if BIG_ENDIAN
//						 return (tokenType) ((longAt(vn->vtdBuffer,index) & MASK_TOKEN_TYPE) >> 60) & 0xf;
//#else
//
//#endif
//					 }

					 //Test whether current element has an attribute with the matching name.
					 Boolean hasAttr(VTDNav *vn, UCS2Char *an){
						 tokenType type;
						 int size = vn->vtdBuffer->size;
						 int index = (vn->context[0] != 0) ? vn->context[vn->context[0]] + 1 : vn->rootIndex + 1;
						 if (index >= size)
							 return FALSE;

						 type = getTokenType(vn,index);
						 if (vn->ns == FALSE) {
							 if (strcmp(an,"*")==0) {
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
							 if (strcmp(an,"*")==0) {
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
					 Boolean hasAttrNS(VTDNav *vn, UCS2Char *URL, UCS2Char *localName){
						 return (getAttrValNS(vn,URL, localName) != -1);
					 }

					 //Test the token type, to see if it is a starting tag.
					 static inline Boolean isElement(VTDNav  *vn, int index){
#if BIG_ENDIAN
						 return (((longAt(vn->vtdBuffer,index) & MASK_TOKEN_TYPE) >> 60) & 0xf)
							 == TOKEN_STARTING_TAG;
#else
#endif
					 }

					 //Test whether ch is a white space character or not.
					 static inline Boolean isWS(int ch){
						 return (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r');
					 }
					 //This method is similar to getElementByName in DOM except it doesn't
					 //return the nodeset, instead it iterates over those nodes.
					 int iterate(VTDNav *vn, int dp, UCS2Char *en){

						 int index = getCurrentIndex(vn) + 1;
						 //int size = vtdBuffer.size();
						 while (index < vn->vtdSize) {
							 if (isElement(vn,index)) {
								 int depth = getTokenDepth(vn,index);
								 if (depth > dp) {
									 vn->context[0] = depth;
									 vn->context[depth] = index;
									 if (matchElement(vn, en)) {
										 if (depth < 4)
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
					 int iterateNS(VTDNav *vn, int dp, UCS2Char *URL, UCS2Char *ln){
						 int index;
						 exception e;
						 if (vn->ns == FALSE)
							 return FALSE;
						 if (ln == NULL){
							 e.et = invalid_argument;
							 e.msg = " invalid argument for iterateNS, ln can't be NULL";
							 Throw e;
						 }

						 index = getCurrentIndex(vn) + 1;
						 while (index < vn->vtdSize) {
							 if (isElement(vn,index)) {
								 int depth = getTokenDepth(vn,index);
								 if (depth > dp) {
									 vn->context[0] = depth;
									 vn->context[depth] = index;
									 if (matchElementNS(vn,URL, ln)) {
										 if (depth < 4)
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

					 //Test if the current element matches the given name.
					 Boolean matchElement(VTDNav *vn, UCS2Char *en){
						 exception e;
						 if (en == NULL){
							 e.et = invalid_argument;
							 e.msg = "matchElement's element name can't be null";
							 Throw e;
						 }
						 // throw new IllegalArgumentException(" Element name can't be null ");
						 if (strcmp(en,"*") == 0)
							 return TRUE;
						 return matchRawTokenString(vn,
							 (vn->context[0] == 0) ? vn->rootIndex : vn->context[vn->context[0]],
							 en);
					 }

					 //Test whether the current element matches the given namespace URL and localname.
					 //URL, when set to "*", matches any namespace (including null), when set to null, defines a "always-no-match"
					 //ln is the localname that, when set to *, matches any localname
					 Boolean matchElementNS(VTDNav *vn, UCS2Char *URL, UCS2Char *ln){
						 int i =
							 getTokenLength(vn, (vn->context[0] != 0) ? vn->context[vn->context[0]] : vn->rootIndex);
						 int offset =
							 getTokenOffset(vn, (vn->context[0] != 0) ? vn->context[vn->context[0]] : vn->rootIndex);
						 int preLen = (i >> 16) & 0xffff;
						 int fullLen = i & 0xffff;

						 if (strcmp(ln, "*")== 0
							 || ((preLen != 0)
							 ? matchRawTokenString1(vn,
							 offset + preLen + 1,
							 fullLen - preLen - 1,
							 ln)
							 : matchRawTokenString1(vn,
							 offset,
							 fullLen,
							 ln))) { // no prefix, search for xmlns
								 if (((URL != NULL) ? strcmp(URL,"*")==0 : FALSE)
									 || (resolveNS2(vn, URL, offset, preLen) == TRUE))
									 return TRUE;
							 }
							 return FALSE;
					 }

					 //Match a string against a token with given offset and len, entities 
					 //doesn't get resolved.
					 static Boolean matchRawTokenString1(VTDNav *vn, int offset, int len, UCS2Char *s){
						 int i,l, endOffset;
						 exception e;
						 if (s == NULL){
							 e.et = invalid_argument;
							 e.msg = " invalid argument for matchRawTokenString2, s can't be NULL";
							 Throw e;
						 }

						 vn->currentOffset = offset;
						 endOffset = offset + len;
						 if (vn->encoding < 2) {
							 if (strlen(s) != len)
								 return FALSE;
							 l = (int)strlen(s);
							 for (i = 0; i < l && vn->currentOffset < endOffset; i++) {
								 if (s[i] != (vn->XMLDoc[vn->currentOffset] & 0xff))
									 return FALSE ;
								 vn->currentOffset++;
							 }
							 return TRUE;
						 } else {
							 //       System.out.print("currentOffset :" + currentOffset);
							 l = (int)strlen(s);
							 //System.out.println(s);
							 for (i = 0; i < l && vn->currentOffset < endOffset; i++) {
								 if (s[i] != getChar(vn)) {
									 return FALSE;
								 }
							 }
						 }
						 if (i == l && vn->currentOffset == endOffset)
							 return TRUE;
						 return FALSE;
					 }

					 //Match a string with a token represented by a long (upper 32 len, lower 32 offset).
					 static Boolean matchRawTokenString2(VTDNav *vn, Long l, UCS2Char *s){
						 exception e;
						 int len;
						 if (s == NULL){
							 e.et = invalid_argument;
							 e.msg = " invalid argument for matchRawTokenString2, s can't be NULL";
							 Throw e;
						 }
						 //throw new IllegalArgumentException("string can't be null");
						 len = (int) ((l & MASK_TOKEN_FULL_LEN) >> 32);
						 // a little hardcode is always bad
						 vn->currentOffset = (int) l;
						 return matchRawTokenString1(vn, vn->currentOffset, len, s);
					 }

					 //Match the string against the token at the given index value. When a token
					 //is an attribute name or starting tag, qualified name is what gets matched against
					 Boolean matchRawTokenString(VTDNav *vn, int index, UCS2Char *s){	
						 exception e;
						 tokenType type;
						 int len;
						 if (s == NULL){
							 e.et = invalid_argument;
							 e.msg = " invalid argument for matchRawTokenString, s can't be NULL";
							 Throw e;
						 }
						 type = getTokenType(vn,index);
						 len =
							 (type == TOKEN_STARTING_TAG
							 || type == TOKEN_ATTR_NAME
							 || type == TOKEN_ATTR_NS)
							 ? getTokenLength(vn, index) & 0xffff
							 : getTokenLength(vn, index);
						 // upper 16 bit is zero or for prefix

						 vn->currentOffset = getTokenOffset(vn, index);
						 // point currentOffset to the beginning of the token
						 // for UTF 8 and ISO, the performance is a little better by avoid calling getChar() everytime
						 return matchRawTokenString1(vn, vn->currentOffset, len, s);
					 }

					 //Match a string against a token with given offset and len, entities get 
					 //resolved properly.
					 static Boolean matchTokenString1(VTDNav *vn, int offset, int len, UCS2Char *s){
						 exception e;
						 int endOffset;
						 int l;
						 if (s == NULL){ 
							 e.et = invalid_argument;
							 e.msg = " invalid argument for matchRawTokenString1, s can't be NULL";
							 Throw e;
						 }
						 //throw new IllegalArgumentException("string can't be null");

						 vn->currentOffset = offset;
						 endOffset = offset + len;

						 if (vn->encoding < FORMAT_UTF8) {
							 int i = 0;
							 l = (int)strlen(s);
							 for (i = 0; i < l && vn->currentOffset < endOffset; i++) {
								 if ((vn->XMLDoc[vn->currentOffset] & 0xff) != '&') {
									 if (s[i] != (vn->XMLDoc[vn->currentOffset] & 0xff))
										 return FALSE;
									 vn->currentOffset++;
								 } else {
									 if (s[i] != getCharResolved(vn)) {
										 return FALSE;
									 }
								 }
							 }
							 if (i == l && vn->currentOffset == endOffset)
								 return TRUE;
							 else
								 return FALSE;
						 } else {
							 int i = 0;
							 l = (int)strlen(s);
							 for (i = 0; i < l && vn->currentOffset < endOffset; i++) {
								 if (s[i] != getCharResolved(vn)) {
									 return FALSE;
								 }
							 }
							 if (i == l && vn->currentOffset == endOffset)
								 return TRUE;
							 else
								 return FALSE;
						 } //return TRUE;
					 }

					 //Match a string against a "non-extractive" token represented by a 
					 //long (upper 32 len, lower 32 offset).
					 static Boolean matchTokenString2(VTDNav *vn, Long l, UCS2Char *s){
						 exception e;
						 int len;
						 if (s == NULL){
							 e.et = invalid_argument;
							 e.msg = " invalid argument for matchTokenString2, s can't be NULL";
							 Throw e;
						 }
						 //	 throw new IllegalArgumentException("string can't be null");
						 len = (int) ((l & MASK_TOKEN_FULL_LEN) >> 32);
						 // a little hardcode is always bad
						 vn->currentOffset = (int) l;
						 return matchRawTokenString1(vn,vn->currentOffset, len, s);
					 }

					 //Match the string against the token at the given index value. When a token
					 //is an attribute name or starting tag, qualified name is what gets matched against
					 Boolean matchTokenString(VTDNav *vn, int index, UCS2Char *s){
						 exception e;
						 tokenType type;
						 int len;
						 if (s == NULL){
							 e.et = invalid_argument;
							 e.msg = " invalid argument for matchTokenString, s can't be NULL";
							 Throw e;
						 }
						 type = getTokenType(vn,index);
						 len =
							 (type == TOKEN_STARTING_TAG
							 || type == TOKEN_ATTR_NAME
							 || type == TOKEN_ATTR_NS)
							 ? getTokenLength(vn,index) & 0xffff
							 : getTokenLength(vn,index);
						 // upper 16 bit is zero or for prefix

						 vn->currentOffset = getTokenOffset(vn,index);
						 // point currentOffset to the beginning of the token
						 // for UTF 8 and ISO, the performance is a little better by avoid calling getChar() everytime
						 return matchTokenString1(vn,vn->currentOffset, len, s);
					 }

					 //Evaluate the namespace indicator in bit 31 and bit 30.
					 static inline int NSval(VTDNav *vn, int i){		
						 return (int) (longAt(vn->vtdBuffer,i) & MASK_TOKEN_NS_MARK);
					 }

					 //Convert a vtd token into a double.
					 double parseDouble(VTDNav *vn, int index){
						 int ch;
						 int end;
						 exception e;
						 Long left, right, scale, exp;
						 double v;
						 Boolean neg;
						 vn->currentOffset = getTokenOffset(vn,index);
						 end = vn->currentOffset + getTokenLength(vn,index);
						 //past the last one by one

						 ch = getCharResolved(vn);

						 while (vn->currentOffset <= end) { // trim leading whitespaces
							 if (!isWS(ch))
								 break;
							 ch = getCharResolved(vn);
						 }

						 if (vn->currentOffset > end) {// all whitespace
							 e.et = nav_exception;
							 e.msg = " empty string for parseDouble";
							 Throw e;
							 //throw new NavException("Empty string");
						 }

						 neg = (ch == '-');

						 if (ch == '-' || ch == '+')
							 ch = getCharResolved(vn); //get another one if it is sign.

						 //left part of decimal
						 left = 0;
						 while (vn->currentOffset <= end) {
							 //must be <= since we get the next one at last.

							 int dig = Character_digit((char) ch, 10); //only consider decimal
							 if (dig < 0)
								 break;

							 left = left * 10 + dig;

							 ch = getCharResolved(vn);
						 }

						 //right part of decimal
						 right = 0;
						 scale = 1;
						 if (ch == '.') {
							 ch = getCharResolved(vn);

							 while (vn->currentOffset <= end) {
								 //must be <= since we get the next one at last.

								 int dig = Character_digit((char) ch, 10);
								 //only consider decimal
								 if (dig < 0)
									 break;

								 right = right * 10 + dig;
								 scale *= 10;

								 ch = getCharResolved(vn);
							 }
						 }

						 //exponent
						 exp = 0;
						 if (ch == 'E' || ch == 'e') {
							 Boolean expneg;
							 int cur;
							 ch = getCharResolved(vn);
							 expneg = (ch == '-'); //sign for exp
							 if (ch == '+' || ch == '-')
								 ch = getCharResolved(vn); //skip the +/- sign

							 cur = vn->currentOffset;
							 //remember the indx, used to find a invalid number like 1.23E

							 while (vn->currentOffset <= end) {
								 //must be <= since we get the next one at last.

								 int dig = Character_digit((char) ch, 10);
								 //only consider decimal
								 if (dig < 0)
									 break;

								 exp = exp * 10 + dig;

								 ch = getCharResolved(vn);
							 }
							 if (cur == vn->currentOffset){// all whitespace
								 e.et = nav_exception;
								 e.msg = " parseDouble failed, invalid format, no digits after +/-";
								 Throw e;
								 //throw new NavException("Empty string");
							 }
							 //throw new NavException(toString(index));
							 //found a invalid number like 1.23E

							 if (expneg)
								 exp = (-exp);
						 }

						 //anything left must be space
						 while (vn->currentOffset <= end) {
							 if (!isWS(ch)){// all whitespace
								 e.et = nav_exception;
								 e.msg = " parseDouble failed, invalid char found";
								 Throw e;
								 //throw new NavException("Empty string");
							 }
							// throw new NavException(toString(index));

							 ch = getCharResolved(vn);
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
						 Boolean neg;
						 exception e;
						 //Long exp;
						 double v;
						 vn->currentOffset = getTokenOffset(vn,index);
						 end = vn->currentOffset + getTokenLength(vn, index);
						 //past the last one by one

						 ch = getCharResolved(vn);

						 while (vn->currentOffset <= end) { // trim leading whitespaces
							 if (!isWS(ch))
								 break;
							 ch = getCharResolved(vn);
						 }

						 if (vn->currentOffset > end){// all whitespace
							 e.et = number_format_exception;
							 e.msg = " empty string for parseFloat";
							 Throw e;
						 }
							 //throw new NavException("Empty string");

						 neg = (ch == '-');

						 if (ch == '-' || ch == '+')
							 ch = getCharResolved(vn); //get another one if it is sign.

						 //left part of decimal
						 left = 0;
						 while (vn->currentOffset <= end) {
							 //must be <= since we get the next one at last.

							 int dig = Character_digit((char) ch, 10); //only consider decimal
							 if (dig < 0)
								 break;

							 left = left * 10 + dig;

							 ch = getCharResolved(vn);
						 }

						 //right part of decimal
						 right = 0;
						 scale = 1;
						 if (ch == '.') {
							 ch = getCharResolved(vn);

							 while (vn->currentOffset <= end) {
								 //must be <= since we get the next one at last.

								 int dig = Character_digit((char) ch, 10);
								 //only consider decimal
								 if (dig < 0)
									 break;

								 right = right * 10 + dig;
								 scale *= 10;

								 ch = getCharResolved(vn);
							 }
						 }

						 //exponent
						 exp = 0;
						 if (ch == 'E' || ch == 'e') {
							 Boolean expneg;
							 int cur;
							 ch = getCharResolved(vn);
							 expneg = (ch == '-'); //sign for exp
							 if (ch == '+' || ch == '-')
								 ch = getCharResolved(vn); //skip the +/- sign

							 cur = vn->currentOffset;
							 //remember the indx, used to find a invalid number like 1.23E

							 while (vn->currentOffset <= end) {
								 //must be <= since we get the next one at last.

								 int dig = Character_digit((char) ch, 10);
								 //only consider decimal
								 if (dig < 0)
									 break;

								 exp = exp * 10 + dig;

								 ch = getCharResolved(vn);
							 }

							 if (cur == vn->currentOffset){// all whitespace
								 e.et = number_format_exception;
								 e.msg = " parseFloat failed, invalid format, no digits after +/-";
								 Throw e;
							 }
							//	 throw new NavException(toString(index));
							 //found a invalid number like 1.23E

							 if (expneg)
								 exp = (-exp);
						 }

						 //anything left must be space
						 while (vn->currentOffset <= end) {
							 if (!isWS(ch)){// all whitespace
								 e.et = number_format_exception;
								 e.msg = " parseFloat failed, invalid format, invalid char encountered";
								 Throw e;
							 }
							// throw new NavException(toString(index));

							 ch = getCharResolved(vn);
						 }

						 v = (double) left;
						 if (right != 0)
							 v += ((double) right) / (double) scale;

						 if (exp != 0)
							 v = v * pow(10, (double)exp);
						 if (neg)
							 v = -v;

						 f = (float) v;

						 //try to handle overflow/underflow
						 if (v >= MAXFLOAT)
							 f = MAXFLOAT;
						 else if (v <= MINFLOAT)
							 f = MINFLOAT;

						 return f;
						 //return 1.0;
					 }

					 //Convert a vtd token into an int
					 int parseInt(VTDNav *vn, int index){
						 return parseInt2(vn,index,10);
					 }

					 //Convert a vtd token into an Int according to given radix.
					 static int parseInt2(VTDNav *vn, int index, int radix){
						 Long pos, result;
						 int endOffset, c;
						 Boolean neg;
						 exception e;

						 vn->currentOffset = getTokenOffset(vn,index);
						 endOffset = vn->currentOffset + getTokenLength(vn,index);

						 c = getCharResolved(vn);

						 // trim leading whitespaces
						 while ((c == ' ' || c == '\n' || c == '\t' || c == '\r')
							 && (vn->currentOffset <= endOffset))
							 c = getCharResolved(vn);

						 if (vn->currentOffset > endOffset) {// all whitespace
							 e.et = number_format_exception;
							 e.msg = " empty string for parseInt2";
							 Throw e;
						 }

						 neg = (c == '-');
						 if (neg || c == '+')
							 c = getCharResolved(vn); //skip sign

						 result = 0;
						 pos = 1;
						 while (vn->currentOffset <= endOffset) {
							 int digit = Character_digit((char) c, radix);
							 if (digit < 0)
								 break;

							 //Note: for binary we can simply shift to left to improve performance
							 result = result * radix + digit;
							 //pos *= radix;

							 c = getCharResolved(vn);
						 }

						 if (result > MAXINT) {// all whitespace
							 e.et = number_format_exception;
							 e.msg = " integer value over(under) flow";
							 Throw e;
						 }
							// throw new NumberFormatException("Overflow: " + toString(index));

						 // take care of the trailing
						 while (vn->currentOffset <= endOffset && isWS(c)) {
							 c = getCharResolved(vn);
						 }
						 if (vn->currentOffset == (endOffset + 1))
							 return (int) ((neg) ? (-result) : result);
						 else{// all whitespace
							 e.et = number_format_exception;
							 e.msg = " invalid char during parseInt2";
							 Throw e;
						 }
						 // throw new NumberFormatException(toString(index));
					 }

					 //Convert a vtd token into a long
					 Long parseLong(VTDNav *vn, int index){
						return parseLong2(vn, index, 10);
					 }

					 //Convert a vtd token into a long according to given radix.
					 static Long parseLong2(VTDNav *vn, int index, int radix){
						 exception e;
						 int endOffset, c;
						 Long result, pos;
						 Boolean neg;
						 vn->currentOffset = getTokenOffset(vn, index);
						 endOffset = vn->currentOffset + getTokenLength(vn, index);

						 c = getCharResolved(vn);

						 // trim leading whitespaces
						 while ((c == ' ' || c == '\n' || c == '\t' || c == '\r')
							 && (vn->currentOffset <= endOffset))
							 c = getCharResolved(vn);

						 if (vn->currentOffset > endOffset) {// all whitespace
							 e.et = number_format_exception;
							 e.msg = " empty string for parseLong2";
							 Throw e;
						 }

						 neg = (c == '-');
						 if (neg || c == '+')
							 c = getCharResolved(vn); //skip sign

						 result = 0;
						 pos = 1;
						 while (vn->currentOffset <= endOffset) {
							 int digit = Character_digit((char) c, radix);
							 if (digit < 0)
								 break;

							 //Note: for binary we can simply shift to left to improve performance
							 result = result * radix + digit;
							 //pos *= radix;

							 c = getCharResolved(vn);
						 }

						 if (result > MAXLONG) {// all whitespace
							 e.et = number_format_exception;
							 e.msg = " long value over(under) flow";
							 Throw e;
						 }
						// throw new NumberFormatException("Overflow: " + toString(index));

						 // take care of the trailing
						 while (vn->currentOffset <= endOffset && isWS(c)) {
							 c = getCharResolved(vn);
						 }
						 if (vn->currentOffset == (endOffset + 1))
							 return (int) ((neg) ? (-result) : result);
						 else{// all whitespace
							 e.et = number_format_exception;
							 e.msg = " invalid char during parseLong2";
							 Throw e;
						 }
						 //throw new NumberFormatException(toString(index));
					 }

					 //Load the context info from ContextBuffer.
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

						 return TRUE;
					 }

					 //Store the context info into the ContextBuffer.
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

						 store(vn->contextBuf,vn->stackTemp);
						 return TRUE;
					 }



					 //Sync up the current context with location cache.
					 static void resolveLC(VTDNav *vn){	
						 int temp;
						 int i,k;
						 switch (vn->context[0]) {
			case 1 :
			case 2 :
			case 3 :
				if (vn->l1index < 0
					|| vn->l1index >= vn->l1Buffer->size
					|| vn->context[1] != upper32At(vn->l1Buffer, vn->l1index)) {
						if (vn->l1index >= vn->l1Buffer->size) {
							vn->l1index = 0;
						}
						if (vn->context[1] != upper32At(vn->l1Buffer,vn->l1index + 1)) {
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
						} else
							vn->l1index = vn->l1index + 1;
						// for iterations, l1index+1 is the logical next value for l1index
					}
					if (vn->context[0] == 1)
						break;

					temp = lower32At(vn->l1Buffer,vn->l1index);
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
							if (vn->l2index >= vn->l2Buffer->size)
								vn->l2index = vn->l2lower;
							if (vn->context[2] == upper32At(vn->l2Buffer,vn->l2index + 1))
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
							} else
								while (vn->context[2] != upper32At(vn->l2Buffer,vn->l2index)) {
									vn->l2index++;
								}
						}

						if (vn->context[0] == 2)
							break;
						temp = lower32At(vn->l2Buffer,vn->l2index);
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
								if (vn->l3index >= vn->l3Buffer->size)
									vn->l3index = vn->l3lower;
								if (vn->context[3] == intAt(vn->l3Buffer,vn->l3index + 1))
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
								} else
									while (vn->context[3] != intAt(vn->l3Buffer,vn->l3index)) {
										vn->l3index++;
									}
							}

							/*if (vn->context[0] == 3)
							break;*/
			default : // do nothing here
				return;
						 }
					 }

					 //Test whether the URL is defined in the document.
					 static Boolean resolveNS(VTDNav *vn, UCS2Char *URL){
						 int i, offset, preLen;
						 i =
							 getTokenLength(vn,(vn->context[0] != 0) ? vn->context[vn->context[0]] : vn->rootIndex);
						 offset =
							 getTokenOffset(vn,(vn->context[0] != 0) ? vn->context[vn->context[0]] : vn->rootIndex);
						 preLen = (i >> 16) & 0xffff;

						 return resolveNS2(vn,URL, offset, preLen);
					 }

					 //Test whether the URL is defined in the document.
					 //Null is allowed to indicate the name space should be undefined.
					 static Boolean resolveNS2(VTDNav *vn, UCS2Char *URL, int offset, int len){
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
									if (URL != NULL) {
										return matchTokenString(vn, s + 1, URL);
									} else { //xmlns is found but shouldn't be
										return FALSE;
									}
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
										return (URL != NULL)
											? matchTokenString(vn, s + 1, URL)
											: FALSE;
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
					type = getTokenType(vn,k);

					while (k < size
						&& (type == TOKEN_ATTR_NAME || type == TOKEN_ATTR_NS)) {
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
									if (URL != NULL) {
										return matchRawTokenString(vn,s + 1, URL);
									} else { //xmlns is found but shouldn't be
										return FALSE;
									}
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
										return (URL != NULL)
											? matchTokenString(vn, k + 1, URL)
											: FALSE;
									}
								}
							}
							//return (URL != null) ? true : false;
							k += 2;
							type = getTokenType(vn,k);
						}
						l = longAt(vn->vtdBuffer, s);
						if (hasNS) {
							hasNS = FALSE;
							modifyEntryFLB(vn->vtdBuffer,s, l | 0x00000000c0000000L);
						} else {
							modifyEntryFLB(vn->vtdBuffer, s, l | 0x0000000080000000L);
						}
						break;
							 }
						 }
						 return (URL != NULL) ? FALSE : TRUE;
						 //return FALSE;
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
					for (i = 1; i <= vn->context[0]; i++) {
						vn->context[i] = 0xffffffff;
					}
					vn->context[0] = 0;
				}
				vn->l1index = vn->l2index = vn->l3index = -1;
			case PARENT :
				if (vn->context[0] != 0) {
					//vn->context[vn->context[0]] = vn->context[vn->context[0] + 1] = 0xffffffff;
					vn->context[vn->context[0]] = -1;
					vn->context[0]--;
					return TRUE;
				} else {
					return FALSE;
				}
			case FIRST_CHILD :
			case LAST_CHILD :

				switch (vn->context[0]) {
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
				vn->l3upper = size - 1;

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
						int token_type =
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
						int depth =
							(int) ((MASK_TOKEN_DEPTH & temp) >> 52);
						int token_type =
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
						int token_type =
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
						int token_type =
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
					 Boolean toElement2(VTDNav *vn, navDir direction, UCS2Char *en){
						 //int size;
						 int temp;
						 exception e;
						 if (en == NULL){
							 e.et = invalid_argument;
							 e.msg = "inavlid argument for toElement2";
							 Throw e;
							 //throw new IllegalArgumentException(" Element name can't be null ");
						 }
						 if (strcmp(en,"*") == 0)
							 return toElement(vn,direction);
						 switch (direction) {
			case ROOT :
				return toElement(vn,ROOT);

			case PARENT :
				return toElement(vn,PARENT);

			case FIRST_CHILD :
				if (toElement(vn,FIRST_CHILD) == FALSE)
					return FALSE;
				// check current element name
				if (matchElement(vn,en) == FALSE) {
					if (toElement2(vn,NEXT_SIBLING, en) == TRUE)
						return TRUE;
					else {
						//toParentElement();
						vn->context[vn->context[0]] = 0xffffffff;
						vn->context[0]--;
						return FALSE;
					}
				} else
					return TRUE;

			case LAST_CHILD :
				if (toElement(vn,LAST_CHILD) == FALSE)
					return FALSE;
				if (matchElement(vn,en) == FALSE){
					if (toElement2(vn,PREV_SIBLING, en) == TRUE)
						return TRUE;
					else {
						vn->context[vn->context[0]] = 0xffffffff;
						vn->context[0]--;
						//toParentElement();
						return FALSE;
					}
				} else
					return TRUE;

			case NEXT_SIBLING :
				temp = vn->context[vn->context[0]]; // store the current position
				if (vn->context[0] == 0)
					return FALSE;
				while (toElement(vn,NEXT_SIBLING)) {
					if (matchElement(vn,en)) {
						return TRUE;
					}
				}
				vn->context[vn->context[0]] = temp;
				return FALSE;

			case PREV_SIBLING :
				temp = vn->context[vn->context[0]]; // store the current position
				if (vn->context[0] == 0)
					return FALSE;
				while (toElement(vn,PREV_SIBLING)) {
					if (matchElement(vn,en)) {
						return TRUE;
					}
				}
				vn->context[vn->context[0]] = temp;
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
					 Boolean toElementNS(VTDNav *vn, navDir direction, UCS2Char *URL, UCS2Char *ln){
						 //int size;
						 int temp;
						 if (vn->ns == FALSE)
							 return FALSE;
						 switch (direction) {
			case ROOT :
				return toElement(vn,ROOT);

			case PARENT :
				return toElement(vn,PARENT);

			case FIRST_CHILD :
				if (toElement(vn,FIRST_CHILD) == FALSE)
					return FALSE;
				// check current element name
				if (matchElementNS(vn,URL, ln) == FALSE) {
					if (toElementNS(vn,NEXT_SIBLING, URL, ln) == TRUE )
						return TRUE;
					else {
						//toParentElement();
						vn->context[vn->context[0]] = 0xffffffff;
						vn->context[0]--;
						return FALSE;
					}
				} else
					return TRUE;

			case LAST_CHILD :
				if (toElement(vn,LAST_CHILD) == FALSE)
					return FALSE;
				if (matchElementNS(vn, URL, ln) == FALSE) {
					if (toElementNS(vn, PREV_SIBLING, URL, ln) == TRUE)
						return TRUE;
					else {
						vn->context[vn->context[0]] = 0xffffffff;
						vn->context[0]--;
						//toParentElement();
						return FALSE;
					}
				} else
					return TRUE;

			case NEXT_SIBLING :
				temp = vn->context[vn->context[0]]; // store the current position
				if (vn->context[0] == 0)
					return FALSE;
				while (toElement(vn,NEXT_SIBLING)) {
					if (matchElementNS(vn,URL, ln)) {
						return TRUE;
					}
				}
				vn->context[vn->context[0]] = temp;
				return FALSE;

			case PREV_SIBLING :
				temp = vn->context[vn->context[0]]; // store the current position
				if (vn->context[0] == 0)
					return FALSE;
				while (toElement(vn,PREV_SIBLING)) {
					if (matchElementNS(vn,URL, ln)) {
						return TRUE;
					}
				}
				vn->context[vn->context[0]] = temp;
				return FALSE;

			default :
				return FALSE;
				//throw new NavException("illegal navigation options");
							}

					 }

					 //This method normalizes a token into a string in a way that resembles DOM.
					 //The leading and trailing white space characters will be stripped.
					 //The entity and character references will be resolved
					 //Multiple whitespaces char will be collapsed into one.

					 UCS2Char *toNormalizedString(VTDNav *vn, int index){
						 exception e;
						 tokenType type = getTokenType(vn,index);
						 int len, endOffset;
						 int ch,k;
						 Boolean d;
						 UCS2Char *s = NULL;

						 len = getTokenLength(vn,index);
						 s = (UCS2Char *)malloc(sizeof(UCS2Char)*len);
						 if (s == NULL)
						 {
							 e.et = out_of_mem;
							 e.msg = " string allocation failed in toString ";
							 Throw e;							
						 }
						 if (len == 0)
							 return "";
						 vn->currentOffset = getTokenOffset(vn ,index);
						 endOffset = len + vn->currentOffset - 1; // point to the last character
						 
						 
						 // trim off the leading whitespaces

						 while (TRUE) {
							 int temp = vn->currentOffset;
							 ch = getCharResolved(vn);
							 if (!isWS(ch)) {
								 vn->currentOffset = temp;
								 break;
							 }
						 }

						 d = FALSE;
						 k = 0;
						 while (vn->currentOffset <= endOffset) {
							 ch = getCharResolved(vn);
							 if (isWS(ch)) {
								 d = TRUE;
							 } else {
								 if (d == FALSE)
									 //sb.append((char) ch); // java only supports 16 bit unicode
								     s[k++] = ch;
								 else {
									 //sb.append(' ');
									 s[k++] = (UCS2Char) ch;
									 //sb.append((char) ch);
									 s[k++] = (UCS2Char) ch;
									 d = FALSE;
								 }
							 }
						 }
						 return s;
					 }

					 //Convert a token at the given index to a String, 
					 //(built-in entity and char references not resolved)
					 //(entities and char references not expanded).
					 UCS2Char *toRawString(VTDNav *vn, int index){
						 exception e;
						 int k, offset, endOffset;
						 tokenType type = getTokenType(vn,index);
						 int len;
						 UCS2Char *s = NULL;
						 
						 len = getTokenLength(vn,index);
						 offset = getTokenOffset(vn,index);

						 vn->currentOffset = getTokenOffset(vn,index);
						 endOffset = len + vn->currentOffset;
						 //StringBuffer sb = new StringBuffer(len);
	
						 s = (UCS2Char *)malloc(sizeof(UCS2Char)*len);
						 if (s == NULL)
						 {
							 e.et = out_of_mem;
							 e.msg = " string allocation failed in toString ";
							 Throw e;							
						 }
						 k = 0;
						 while (vn->currentOffset < endOffset) {
							 UCS2Char c = (UCS2Char) getChar(vn);
							 s[k++] = c;; // java only support 16 bit unit code
						 }
						 return s;
					 }

					 //Convert a token at the given index to a String, (entities and char 
					 //references resolved).
					 // An attribute name or an element name will get the UCS2 string of qualified name 
					 UCS2Char *toString(VTDNav *vn, int index){
						 exception e;
						 int k, offset, endOffset;
						 tokenType type = getTokenType(vn,index);
						 int len = getTokenLength(vn,index);
						 UCS2Char *s = NULL;
						 offset = getTokenOffset(vn,index);

						 vn->currentOffset = getTokenOffset(vn,index);
						 endOffset = len + vn->currentOffset;
						 //StringBuffer sb = new StringBuffer(len);
	
						 s = (UCS2Char *)malloc(sizeof(UCS2Char)*len);
						 if (s == NULL)
						 {
							 e.et = out_of_mem;
							 e.msg = " string allocation failed in toString ";
							 Throw e;							
						 }
						 k = 0;
						 while (vn->currentOffset < endOffset) {
							 UCS2Char c = (UCS2Char) getCharResolved(vn);
							 s[k++] = c;; // java only support 16 bit unit code
						 }
						 return s;
					 }