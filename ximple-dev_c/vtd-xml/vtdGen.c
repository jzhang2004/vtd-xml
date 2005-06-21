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
#include "vtdGen.h"
// define internal parser state
//#define	STATE_DOC_START  0 // beginning of document
//#define	STATE_DOC_END  1 // end of document 
//#define	STATE_LT_SEEN  2 // encounter the first <
//#define	STATE_START_TAG  3
//#define	STATE_END_TAG  4
//#define	STATE_UNRECORDED_TEXT  5
//#define	STATE_TEXT  6
//#define	STATE_PI_TAG  7
//#define	STATE_PI_VAL  8
//#define	STATE_DEC_ATTR_NAME  9
//#define	STATE_ATTR_NAME  10
//#define	STATE_ATTR_VAL  11
//#define	STATE_COMMENT  12
//#define	STATE_CDATA  13
//#define	STATE_DOCTYPE  14
//#define	STATE_END_COMMENT  15
//#define	STATE_END_PI  16

typedef enum pState {STATE_DOC_START,
STATE_DOC_END,
STATE_LT_SEEN,
STATE_START_TAG,
STATE_END_TAG,
STATE_UNRECORDED_TEXT,
STATE_TEXT,
STATE_PI_TAG,
STATE_PI_VAL,
STATE_DEC_ATTR_NAME,
STATE_ATTR_NAME,
STATE_ATTR_VAL,
STATE_COMMENT,
STATE_CDATA, 
STATE_DOCTYPE,
STATE_END_COMMENT,
STATE_END_PI
} parseState;

// internal functions
static int  entityIdentifier(VTDGen *vg);
static void printLineNumber(VTDGen *vg);
static int getChar(VTDGen *vg);
static inline int getChar2(VTDGen *vg);
static int handle_16le(VTDGen *vg);
static int handle_16be(VTDGen *vg);
static int handle_utf8(VTDGen *vg,int temp);
static int getCharAfterSe(VTDGen *vg);
static int getCharAfterS(VTDGen *vg);
static int getCharAfterS2(VTDGen *vg, Boolean entityOK);
static Boolean skipChar(VTDGen *vg, int ch);
static void writeVTD(VTDGen *vg, int offset, int length, tokenType token_type, int depth);
static void finishUp(VTDGen *vg);

static void decide_encoding(VTDGen *vg);
static int process_end_pi(VTDGen *vg);
static int process_end_comment(VTDGen *vg);
static int process_comment(VTDGen *vg);
static int process_doc_type(VTDGen *vg);
static int process_cdata(VTDGen *vg);
static int process_pi_val(VTDGen *vg);
static int process_pi_tag(VTDGen *vg);
static int process_dec_attr(VTDGen *vg);

// create VTDGen
VTDGen *createVTDGen(){
	exception e;
	Long* l = NULL;
	Long* ts = NULL;
	VTDGen *vg = NULL;

	l = (Long*) malloc(ATTR_NAME_ARRAY_SIZE*sizeof(Long));
	if (l==NULL){
		e.et = out_of_mem;
		e.msg = "VTDGen allocation failed ";
		Throw e;
	}
	//vg->anaLen = ATTR_NAME_ARRAY_SIZE;

	ts = (Long*) malloc(TAG_STACK_SIZE * sizeof(Long));
	if (ts==NULL) {
		free(l);
		e.et = out_of_mem;
		e.msg = "VTDGen allocation failed ";
		Throw e;
	}

	vg = (VTDGen *)malloc(sizeof(VTDGen));
	if (vg==NULL){
		free(l);
		free(ts);
		e.et = out_of_mem;
		e.msg = "VTDGen allocation failed ";
		Throw e;
	}
	vg->anaLen = ATTR_NAME_ARRAY_SIZE;
	vg->attr_name_array = l;
	vg->tag_stack = ts;

	vg->VTDDepth = 0;

	vg->VTDBuffer =	vg->l1Buffer = 	vg->l2Buffer = NULL;
	vg->l3Buffer = NULL;
	vg->vtdSize = vg->l1Size = vg->l2Size = vg->l3Size = 0;
	vg->bufLen = vg->docLen = vg->docLen = vg->last_depth = 0;
	vg->last_i3_index = vg->last_l2_index = vg->last_l1_index = 0;
	vg->XMLDoc = NULL;
	vg->rootIndex = vg->endOffset= 0;
	vg->ns = vg->offset = vg->prev_offset =0;
	vg->stateTransfered = TRUE; // free VTDGen won't free all location cache and VTD buffer

	return vg;
}

// free VTDGen
void freeVTDGen(VTDGen *vg){
	if (vg != NULL){
		free(vg->attr_name_array);
		free(vg->tag_stack);
		if (vg->stateTransfered == FALSE){
			free(vg->XMLDoc);
			freeFastLongBuffer(vg->VTDBuffer);
			freeFastLongBuffer(vg->l1Buffer);
			freeFastLongBuffer(vg->l2Buffer);
			freeFastIntBuffer(vg->l3Buffer);
		}
	}
	free(vg);
}

// clear the internal state of VTDGen so it can process 
// the next XML file
void clear(VTDGen *vg){
	if (vg->stateTransfered == FALSE){
		free(vg->XMLDoc);
		freeFastIntBuffer(vg->l3Buffer);
		freeFastLongBuffer(vg->l1Buffer);
		freeFastLongBuffer(vg->l2Buffer);
		freeFastLongBuffer(vg->VTDBuffer);
	}

	vg->VTDBuffer = NULL;
	vg->l1Buffer = NULL;
	vg->l2Buffer = NULL;
	vg->l3Buffer = NULL;
	vg->XMLDoc = NULL;

	vg->stateTransfered = TRUE;
	vg->l1Size = vg->l2Size = vg->l3Size = vg->VTDDepth = 0;
	vg->last_depth = vg->last_l1_index = 
		vg->last_l2_index = vg->last_i3_index =0;
	vg->offset = vg->temp_offset = 0;
	vg->rootIndex = 0;
	
	vg->depth = -1;
	vg->increment = 1;
	vg->BOM_detected = FALSE;
	vg->must_utf_8 = FALSE;
	vg->ch = vg->ch_temp = 0;
	
}

//detect whether the entity is valid or not and increment offset.
static int  entityIdentifier(VTDGen *vg){
	exception e;
	int ch = getChar(vg);
	int val = 0;

	switch (ch) {
			case '#' :
				ch = getChar(vg);
				if (ch == 'x') {
					while (TRUE) {
						ch = getChar(vg);
						if (ch >= '0' && ch <= '9') {
							val = (val << 4) + (ch - '0');
						} else if (ch >= 'a' && ch <= 'f') {
							val = (val << 4) + (ch - 'a' + 10);
						} else if (ch >= 'A' && ch <= 'F') {
							val = (val << 4) + (ch - 'A' + 10);
						} else if (ch == ';') {
							return val;
						} else{
							e.et = parse_exception;
							e.subtype = 0;
							e.msg = "Parse exception in entityIdentifier";
							e.sub_msg = "Errors in char reference: Illegal char following &#x.";
							Throw e;
						}
						//throw new EntityException("Errors in char reference: Illegal char following &#x.");
					}
				} else {
					while (TRUE) {
						if (ch >= '0' && ch <= '9') {
							val = val * 10 + (ch - '0');
						} else if (ch == ';') {
							break;
						} else{
							e.et = parse_exception;
							e.subtype = 0;
							e.msg = "Parse exception in entityIdentifier";
							e.sub_msg = "Errors in char reference: Illegal char following &#.";
							Throw e;
						}
						//throw new EntityException("Errors in char reference: Illegal char following &#.");
						ch = getChar(vg);
					}
				}
				if (!XMLChar_isValidChar(val)) {
					e.et = parse_exception;
					e.subtype = 0;
					e.msg = "Parse exception in entityIdentifier";
					e.sub_msg = "Errors in entity reference: Invalid XML char.";
					Throw e;
					//throw new EntityException("Errors in entity reference: Invalid XML char.");
				}
				return val;
				//break;

			case 'a' :
				ch = getChar(vg);
				if (ch == 'm') {
					if (getChar(vg) == 'p' && getChar(vg) == ';') {
						//System.out.println(" entity for &");
						return '&';
					} else{
						e.et = parse_exception;
						e.subtype = 0;
						e.msg = "Parse exception in entityIdentifier";
						e.sub_msg = "Errors in Entity: Illegal builtin reference";
						Throw e;
					}
					//throw new EntityException("Errors in Entity: Illegal builtin reference");
				} else if (ch == 'p') {
					if (getChar(vg) == 'o'
						&& getChar(vg) == 's'
						&& getChar(vg) == ';') {
							//System.out.println(" entity for ' ");
							return '\'';
						} else{
							e.et = parse_exception;
							e.subtype = 0;
							e.msg = "Parse exception in entityIdentifier";
							e.sub_msg = "Errors in Entity: Illegal builtin reference";
							Throw e;
						}
						//throw new EntityException("Errors in Entity: Illegal builtin reference");
				} else{
					e.et = parse_exception;
					e.subtype = 0;
					e.msg = "Parse exception in entityIdentifier";
					e.sub_msg = "Errors in Entity: Illegal builtin reference";
					Throw e;
				}
				//throw new EntityException("Errors in Entity: Illegal builtin reference");

			case 'q' :
				if (getChar(vg) == 'u'
					&& getChar(vg) == 'o'
					&& getChar(vg) == 't'
					&& getChar(vg) == ';') {
						return '"';
					} else{
						e.et = parse_exception;
						e.subtype = 0;
						e.msg = "Parse exception in entityIdentifier";
						e.sub_msg = "Errors in Entity: Illegal builtin reference";
						Throw e;
					}
					//throw new EntityException("Errors in Entity: Illegal builtin reference");
			case 'l' :
				if (getChar(vg) == 't' && getChar(vg) == ';') {
					return '<';
				} else{
					e.et = parse_exception;
					e.subtype = 0;
					e.msg = "Parse exception in entityIdentifier";
					e.sub_msg = "Errors in Entity: Illegal builtin reference";
					Throw e;
				}
				//throw new EntityException("Errors in Entity: Illegal builtin reference");
				//break;
			case 'g' :
				if (getChar(vg) == 't' && getChar(vg) == ';') {
					return '>';
				} else{
					e.et = parse_exception;
					e.subtype = 0;
					e.msg = "Parse exception in entityIdentifier";
					e.sub_msg = "Errors in Entity: Illegal builtin reference";
					Throw e;
				}
				//throw new EntityException("Errors in Entity: Illegal builtin reference");
			default :
				e.et = parse_exception;
				e.subtype = 0;
				e.msg = "Parse exception in entityIdentifier";
				e.sub_msg = "Errors in Entity: Illegal builtin reference";
				Throw e;

				//throw new EntityException("Errors in Entity: Illegal entity char");
	}
}



// The string indicating the position (line number:offset) 
// of the offset if there is an exception.
static void printLineNumber(VTDGen *vg){
	int so = vg->docOffset;
	int lineNumber = 0;
	int lineOffset = 0;
	//int end = vg->offset;

	if (vg->encoding < FORMAT_UTF_16BE) {
		while (so <= vg->offset-1) {
			if (vg->XMLDoc[so] == '\n') {
				lineNumber++;
				lineOffset = so;
			}
			//lineOffset++;
			so++;
		}
		lineOffset = vg->offset - lineOffset;
	} else if (vg->encoding == FORMAT_UTF_16BE) {
		while (so <= vg->offset-2) {
			if (vg->XMLDoc[so + 1] == '\n' && vg->XMLDoc[so] == 0) {
				lineNumber++;
				lineOffset = so;
			}
			so += 2;
		}
		lineOffset = (vg->offset - lineOffset) >> 1;
	} else {
		while (so <= vg->offset-2) {
			if (vg->XMLDoc[so] == '\n' && vg->XMLDoc[so + 1] == 0) {
				lineNumber++;
				lineOffset = so;
			}
			so += 2;
		}
		lineOffset = (vg->offset - lineOffset) >> 1;
	}
	//return "\nLine Number: " + lineNumber + " Offset: " + lineOffset;
	printf("\nLine Number: %d  Offset: %d \n",lineNumber+1, lineOffset-1);
}

// This method automatically converts the underlying byte 
// representation character into the right UCS character format.
static int getChar(VTDGen *vg){
	exception e;
	int temp = 0;
	int a = 0, c = 0, d = 0, val = 0, i=0;
	if (vg->offset >= vg->endOffset){
		e.et = parse_exception;
		e.subtype = 0;
		e.msg = "Parse exception in getChar";
		e.sub_msg = "Premature EOF reached, XML document incomplete";
		Throw e;			
	}
	//throw new EOFException("permature EOF reached, XML document incomplete");
	switch (vg->encoding) {
			case FORMAT_ASCII :
				temp = vg->XMLDoc[vg->offset] & 0x7f;
				vg->offset++;
				return temp;
			case FORMAT_UTF8 :

				temp = vg->XMLDoc[vg->offset];
				if (temp <128) {
					vg->offset++;
					return temp;
				}
				//temp = temp & 0xff;
				switch (UTF8Char_byteCount(temp)) { // handle multi-byte code
			case 2 :
				c = 0x1f;
				// A mask determine the val portion of the first byte
				d = 6; // 
				a = 1; //
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
				e.et = parse_exception;
				e.subtype = 0;
				e.msg = "Parse exception in getChar";
				e.sub_msg = "UTF 8 encoding error: should never happen";
				Throw e;
				//throw new ParseException("UTF 8 encoding error: should never happen");
				}
				val = (temp & c) << d;
				i = a - 1;
				while (i >= 0) {
					temp = vg->XMLDoc[vg->offset + a - i];
					if ((temp & 0xc0) != 0x80){
						e.et = parse_exception;
						e.subtype = 0;
						e.msg = "Parse exception in getChar";
						e.sub_msg = "UTF 8 encoding error: should never happen";
						Throw e;
					}
					//throw new ParseException("UTF 8 encoding error: should never happen");
					val = val | ((temp & 0x3f) << ((i<<2)+(i<<1)));
					i--;
				}
				vg->offset += a + 1;
				return val;

			case FORMAT_UTF_16BE :
				// implement UTF-16BE to UCS4 conversion
				temp = vg->XMLDoc[vg->offset] << 8 | vg->XMLDoc[vg->offset + 1];
				if ((temp < 0xd800)
					|| (temp > 0xdfff)) { // not a high surrogate
						vg->offset += 2;
						return temp;
					} else {
						if(temp<0xd800 || temp>0xdbff){
							e.et = parse_exception;
							e.subtype = 0;
							e.msg = "Parse exception in getChar";
							e.sub_msg = "UTF 16 BE encoding error: should never happen";
							Throw e;
						}
						val = temp;
						temp = vg->XMLDoc[vg->offset + 2] << 8 | vg->XMLDoc[vg->offset + 3];
						if (temp < 0xdc00 || temp > 0xdfff) {
							// has to be a low surrogate here
							e.et = parse_exception;
							e.subtype = 0;
							e.msg = "Parse exception in getChar";
							e.sub_msg = "UTF 16 BE encoding error: should never happen";
							Throw e;
							//throw new EncodingException("UTF 16 BE encoding error: should never happen");
						}
						val = ((val - 0xd800) <<10) + (temp - 0xdc00) + 0x10000;
						vg->offset += 4;
						return val;
					}
			case FORMAT_UTF_16LE :
				temp = vg->XMLDoc[vg->offset + 1] << 8 | vg->XMLDoc[vg->offset];
				if (temp < 0xd800 || temp > 0xdfff) { // check for low surrogate
					vg->offset += 2;
					return temp;
				} else {
					if(temp<0xd800 || temp>0xdbff){
						e.et = parse_exception;
						e.subtype = 0;
						e.msg = "Parse exception in getChar";
						e.sub_msg = "UTF 16 LE encoding error: should never happen";
						Throw e;
					}
					val = temp;
					temp = vg->XMLDoc[vg->offset + 3] << 8 | vg->XMLDoc[vg->offset + 2];
					if (temp < 0xdc00 || temp > 0xdfff) {
						// has to be high surrogate
						e.et = parse_exception;
						e.subtype = 0;
						e.msg = "Parse exception in getChar";
						e.sub_msg = "UTF 16 LE encoding error: should never happen";
						Throw e;
						//throw new EncodingException("UTF 16 LE encoding error: should never happen");
					}
					val = ((temp - 0xd800) << 10) + (val - 0xdc00) + 0x10000;
					vg->offset += 4;
					return val;
				}
			case FORMAT_ISO_8859 :
				temp = vg->XMLDoc[vg->offset];
				vg->offset++;
				return temp;
			default :
				e.et = parse_exception;
				e.subtype = 0;
				e.msg = "Parse exception in getChar";
				e.sub_msg = "Unknown encoding";
				Throw e;
				//	throw new EncodingException("Unknown encoding");
	}
}


static int getChar2(VTDGen *vg){
	exception e;
	int temp = 0;
	//int a = 0, c = 0, d = 0, val = 0, i=0;
	if (vg->offset >= vg->endOffset){
		e.et = parse_exception;
		e.subtype = 0;
		e.msg = "Parse exception in getChar";
		e.sub_msg = "Premature EOF reached, XML document incomplete";
		Throw e;			
	}
	//throw new EOFException("permature EOF reached, XML document incomplete");
	switch (vg->encoding) {
			case FORMAT_ASCII :
				temp = vg->XMLDoc[vg->offset] & 0x7f;
				vg->offset++;
				return temp;
			case FORMAT_UTF8 :

				temp = vg->XMLDoc[vg->offset];
				if (temp <128) {
					vg->offset++;
					return temp;
				}
				//temp = temp & 0xff;
				return handle_utf8(vg,temp);
		

			case FORMAT_UTF_16BE :
				// implement UTF-16BE to UCS4 conversion
				return handle_16be(vg);
			
			case FORMAT_UTF_16LE :
				 return handle_16le(vg);
				
			case FORMAT_ISO_8859 :
				temp = vg->XMLDoc[vg->offset];
				vg->offset++;
				return temp;
			default :
				e.et = parse_exception;
				e.subtype = 0;
				e.msg = "Parse exception in getChar";
				e.sub_msg = "Unknown encoding";
				Throw e;
	}
}

static int handle_16le(VTDGen *vg){
	exception e;
	int temp,val;
	temp = vg->XMLDoc[vg->offset + 1] << 8 | vg->XMLDoc[vg->offset];
	if (temp < 0xd800 || temp > 0xdfff) { // check for low surrogate
		vg->offset += 2;
		return temp;
	} else {
		if(temp<0xd800 || temp>0xdbff){
			e.et = parse_exception;
			e.subtype = 0;
			e.msg = "Parse exception in getChar";
			e.sub_msg = "UTF 16 LE encoding error: should never happen";
			Throw e;
		}
		val = temp;
		temp = vg->XMLDoc[vg->offset + 3] << 8 | vg->XMLDoc[vg->offset + 2];
		if (temp < 0xdc00 || temp > 0xdfff) {
		// has to be high surrogate
			e.et = parse_exception;
			e.subtype = 0;
			e.msg = "Parse exception in getChar";
			e.sub_msg = "UTF 16 LE encoding error: should never happen";
			Throw e;
			//throw new EncodingException("UTF 16 LE encoding error: should never happen");
		}
		val = ((temp - 0xd800) << 10) + (val - 0xdc00) + 0x10000;
		vg->offset += 4;
		return val;
	}
}

static int handle_16be(VTDGen *vg){
	exception e;
	int temp,val;
	// implement UTF-16BE to UCS4 conversion
	// handle_16be(vg);
	temp = vg->XMLDoc[vg->offset] << 8 | vg->XMLDoc[vg->offset + 1];
	if ((temp < 0xd800)
		|| (temp > 0xdfff)) { // not a high surrogate
			vg->offset += 2;
			return temp;
	} else {
		if(temp<0xd800 || temp>0xdbff){
			e.et = parse_exception;
			e.subtype = 0;
			e.msg = "Parse exception in getChar";
			e.sub_msg = "UTF 16 BE encoding error: should never happen";
			Throw e;
		}
		val = temp;
		temp = vg->XMLDoc[vg->offset + 2] << 8 | vg->XMLDoc[vg->offset + 3];
		if (temp < 0xdc00 || temp > 0xdfff) {
			// has to be a low surrogate here
			e.et = parse_exception;
			e.subtype = 0;
			e.msg = "Parse exception in getChar";
			e.sub_msg = "UTF 16 BE encoding error: should never happen";
			Throw e;
			//throw new EncodingException("UTF 16 BE encoding error: should never happen");
		}
		val = ((val - 0xd800) <<10) + (temp - 0xdc00) + 0x10000;
		vg->offset += 4;
		return val;
	}
}

static int handle_utf8(VTDGen *vg, int temp){
	int c,d,a,i;
	exception e;
	int val;
		switch (UTF8Char_byteCount(temp)) { // handle multi-byte code
			case 2 :
				c = 0x1f;
				// A mask determine the val portion of the first byte
				d = 6; // 
				a = 1; //
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
				e.et = parse_exception;
				e.subtype = 0;
				e.msg = "Parse exception in getChar";
				e.sub_msg = "UTF 8 encoding error: should never happen";
				Throw e;
				//throw new ParseException("UTF 8 encoding error: should never happen");
				}
				val = (temp & c) << d;
				i = a - 1;
				while (i >= 0) {
					temp = vg->XMLDoc[vg->offset + a - i];
					if ((temp & 0xc0) != 0x80){
						e.et = parse_exception;
						e.subtype = 0;
						e.msg = "Parse exception in getChar";
						e.sub_msg = "UTF 8 encoding error: should never happen";
						Throw e;
					}
					//throw new ParseException("UTF 8 encoding error: should never happen");
					val = val | ((temp & 0x3f) << ((i<<2)+(i<<1)));
					i--;
				}
				vg->offset += a + 1;
				return val;
	
}


// The entity aware version of getCharAfterS
static int getCharAfterSe(VTDGen *vg){
	int n = 0;
	int temp; //offset saver
	while (TRUE) {
		n = getChar2(vg);
		if (!XMLChar_isSpaceChar(n)) {
			if (n != '&')
				return n;
			else {
				temp = vg->offset;
				if (!XMLChar_isSpaceChar(entityIdentifier(vg))) {
					vg->offset = temp; // rewind
					return '&';
				}
			}
		}
	}
}

// The entity ignorant version of getCharAfterS
static int getCharAfterS(VTDGen *vg){
	int n, k;
	n = k = 0;
	while (TRUE) {
		n = getChar2(vg);
		if (n == ' ' || n == '\t' || n == '\n' || n == '\r') {
		} else
			return n;
	}
}

static int getCharAfterS2(VTDGen *vg, Boolean entityOK){
	int n = 0;
	int temp; //offset saver
	if (entityOK == FALSE) {
		while (TRUE) {
			n = getChar(vg);
			if (!XMLChar_isSpaceChar(n))
				return n;
		}
	} else
		while (TRUE) {
			n = getChar(vg);
			if (!XMLChar_isSpaceChar(n)) {
				if (n != '&')
					return n;
				else {
					temp = vg->offset;
					if (!XMLChar_isSpaceChar(entityIdentifier(vg))) {
						vg->offset = temp; // rewind
						return '&';
					}
				}
			}
		}
}
// Returns the VTDNav object after parsing, it also cleans 
// internal state so VTDGen can process the next file.
VTDNav *getNav(VTDGen *vg){
	VTDNav *vn = createVTDNav( vg->rootIndex,
		vg->encoding,
		vg->ns,
		vg->VTDDepth,
		vg->XMLDoc,
		vg->bufLen,
		vg->VTDBuffer,
		vg->l1Buffer,
		vg->l2Buffer,
		vg->l3Buffer,
		vg->docOffset,
		vg->docLen);
	vg->stateTransfered = TRUE;
	clear(vg);
	return vn;

}

// Get the offset value of previous character.
static int getPrevOffset(VTDGen *vg){
	exception e;
	int prevOffset = vg->offset;
	switch (vg->encoding) {
			case FORMAT_UTF8 :
				do {
					prevOffset--;
				} while (vg->XMLDoc[prevOffset] >= 128);
				return prevOffset;
			case FORMAT_ASCII :
			case FORMAT_ISO_8859 :
				return vg->offset - 1;
			case FORMAT_UTF_16LE :
				if (vg->XMLDoc[vg->offset - 2] < 0xDC00
					|| vg->XMLDoc[vg->offset - 2] > 0xDFFFF) {
						return vg->offset - 2;
					} else
						return vg->offset - 4;
			case FORMAT_UTF_16BE :
				if (vg->XMLDoc[vg->offset - 1] < 0xDC00
					|| vg->XMLDoc[vg->offset - 1] > 0xDFFFF) {
						return vg->offset - 2;
					} else
						return vg->offset - 4;
			default :
				e.et = parse_exception;
				e.subtype = 0;
				e.msg = "Parse Exception in getPrevOffset";
				e.sub_msg = "Other Error: Should never happen";
				Throw e;
				//throw new ParseException("Other Error: Should never happen");
	}
}

// Generating VTD tokens and Location cache info.
// One specifies whether the parsing is namespace aware or not.
void parse(VTDGen *vg, Boolean ns){

	// define internal variables	
	exception e;

	int length1 = 0, length2 = 0;
	int attr_count = 0 /*, ch = 0, ch_temp = 0*/;
	//int prev_ch = 0, prev2_ch = 0;
	int i,j;
	parseState parser_state = STATE_DOC_START;
	//boolean has_amp = false; 
	Boolean is_ns = FALSE;
	Boolean unique;
	Boolean unequal;
	//Boolean BOM_detected = FALSE;
	//Boolean must_utf_8 = FALSE;
	Long x;
	Boolean main_loop = TRUE,
		hasDTD = FALSE,
		//hasDecl = FALSE,
		docEnd = FALSE
		//firstLT = TRUE
		;
	//char char_temp; //holds the ' or " indicating start of attr val
	int sos = 0, sl = 0;
	XMLChar_init();
	vg->ns = ns;
	vg->encoding = FORMAT_UTF8;

	//long[] tag_stack = new long[256];
	//long[] attr_name_array = new long[512]; // 512 attributes limit
	//ASCII UTF-8 UTF-16 UTF-16BE UTF-16LE ISO-8859-1
	//
	//int[] scratch_buffer = new int[10];

	// first check first 2 bytes BOM to determine if encoding is UTF16
	decide_encoding(vg);

	// enter the main finite state machine
	Try {
		while (main_loop) {
			switch (parser_state) {
					case STATE_DOC_START :
						if (getChar(vg) == '<') {
							vg->temp_offset = vg->offset;
							// xml decl has to be right after the start of the document
							if (skipChar(vg,'?')
								&& (skipChar(vg,'x') || skipChar(vg,'X'))
								&& (skipChar(vg,'m') || skipChar(vg,'M'))
								&& (skipChar(vg,'l') || skipChar(vg,'L'))) {
									if (skipChar(vg,' ')
										|| skipChar(vg,'\t')
										|| skipChar(vg,'\n')
										|| skipChar(vg,'\r')) {
											vg->ch = getCharAfterS(vg);
											vg->temp_offset = vg->offset;
											parser_state = STATE_DEC_ATTR_NAME;
											break;
										} else if (skipChar(vg,'?')){
											e.et = parse_exception;
											e.subtype = 0;
											e.msg = "Parse Exception in parse()";
											e.sub_msg = "Error in XML decl: premature ending";
											Throw e;
										}
										/*	throw new ParseException(
										"Error in XML decl: premature ending"
										+ formatLineNumber());*/
								}
								vg->offset = vg->temp_offset;
								parser_state = STATE_LT_SEEN;
								break;
						}
						e.et = parse_exception;
						e.subtype = 0;
						e.msg = "Parse Exception in parse()";
						e.sub_msg = "Other Error: XML not starting properly";
						Throw e;

						/*throw new ParseException(
						"Other Error: XML not starting properly"
						+ formatLineNumber()); */
					case STATE_DOC_END :
						docEnd = TRUE;
						vg->ch = getCharAfterS(vg);
						// eof exception should be thrown here for premature ending
						if (vg->ch == '<') {

							if (skipChar(vg,'?')) {
								// processing instruction after end tag of root element
								vg->temp_offset = vg->offset;
								parser_state = STATE_END_PI;
								break;
							} else if (
								skipChar(vg,'!')
								&& skipChar(vg,'-')
								&& skipChar(vg,'-')) {
									// comments allowed after the end tag of the root element
									vg->temp_offset = vg->offset;
									parser_state = STATE_END_COMMENT;
									break;
								}
						}
						e.et = parse_exception;
						e.subtype = -1;
						e.msg = "Parse Exception in parse()";
						e.sub_msg = "Other Error: XML not terminated properly";
						Throw e;
						/*throw new ParseException(
						"Other Error: XML not terminated properly"
						+ formatLineNumber());*/

					case STATE_LT_SEEN : //if (depth < -1)
						//    throw new ParseException("Other Errors: Invalid depth");
						vg->temp_offset = vg->offset;
						vg->ch = getChar(vg);
						if (XMLChar_isNameStartChar(vg->ch)) {
							//vg->temp_offset = offset;
							//length1++;
							vg->depth++;
							//if (ch == ':')
							//   length2 = 0;
							parser_state = STATE_START_TAG;
						} else {
							switch (vg->ch) {
					case '/' :
						parser_state = STATE_END_TAG;
						break;
					case '?' :
						vg->temp_offset = vg->offset;
						vg->ch = getChar(vg);
						if (XMLChar_isNameStartChar(vg->ch)) {
							//vg->temp_offset = offset;
							if ((vg->ch == 'x' || vg->ch == 'X')
								&& (skipChar(vg,'m') || skipChar(vg,'M'))
								&& (skipChar(vg,'l') || skipChar(vg,'L'))) {
									vg->ch = getChar(vg);
									if (vg->ch == '?'
										|| XMLChar_isSpaceChar(vg->ch)){
											e.et = parse_exception;
											e.subtype = 0;
											e.msg = "Parse Exception in parse()";
											e.sub_msg = "Error in PI: [xX][mM][lL] not a valid PI targetname";
											Throw e;
										}
										/*throw new ParseException(
										"Error in PI: [xX][mM][lL] not a valid PI targetname"
										+ formatLineNumber()); */
										vg->offset = getPrevOffset(vg);
								}

								parser_state = STATE_PI_TAG;
								break;
						}
						e.et = parse_exception;
						e.subtype = 0;
						e.msg = "Parse Exception in parse()";
						e.sub_msg = "Other Error: First char after <? invalid";
						Throw e;
						/*throw new ParseException(
						"Other Error: First char after <? invalid"
						+ formatLineNumber()); */

					case '!' : // three possibility (comment, CDATA, DOCTYPE)
						vg->ch = getChar(vg);
						switch (vg->ch) {
					case '-' :
						if (skipChar(vg,'-')) {
							vg->temp_offset = vg->offset;
							parser_state = STATE_COMMENT;
							break;
						} else
						{		
							e.et = parse_exception;
							e.subtype = 0;
							e.msg = "Parse Exception in parse()";
							e.sub_msg = "Error in comment: Invalid char sequence to start a comment";
							Throw e;
						}
						/*throw new ParseException(
						"Error in comment: Invalid char sequence to start a comment"
						+ formatLineNumber()); */
					case '[' :
						if (skipChar(vg,'C')
							&& skipChar(vg,'D')
							&& skipChar(vg,'A')
							&& skipChar(vg,'T')
							&& skipChar(vg,'A')
							&& skipChar(vg,'[')
							&& (vg->depth != -1)) {
								vg->temp_offset = vg->offset;
								parser_state = STATE_CDATA;
								break;
							} else {
								if (vg->depth == -1){
									e.et = parse_exception;
									e.subtype = 0;
									e.msg = "Parse Exception in parse()";
									e.sub_msg = "Error in CDATA: Invalid char sequence for CDATA";
									Throw e;
								}
								/*throw new ParseException(
								"Error in CDATA: Wrong place for CDATA"
								+ formatLineNumber());*/
								e.et = parse_exception;
								e.subtype = 0;
								e.msg = "Parse Exception in parse()";
								e.sub_msg = "Error in CDATA: Invalid char sequence for CDATA";
								Throw e;
								/*throw new ParseException(
								"Error in CDATA: Invalid char sequence for CDATA"
								+ formatLineNumber());*/
							}

					case 'D' :
						if (skipChar(vg,'O')
							&& skipChar(vg,'C')
							&& skipChar(vg,'T')
							&& skipChar(vg,'Y')
							&& skipChar(vg,'P')
							&& skipChar(vg,'E')
							&& (vg->depth == -1)
							&& !hasDTD) {
								hasDTD = TRUE;
								vg->temp_offset = vg->offset;
								parser_state = STATE_DOCTYPE;
								break;
							} else {
								if (hasDTD == TRUE){
									e.et = parse_exception;
									e.subtype = 0;
									e.msg = "Parse Exception in parse()";
									e.sub_msg = "Error for DOCTYPE: Only DOCTYPE allowed";
									Throw e;
								}
								/*	throw new ParseException(
								"Error for DOCTYPE: Only DOCTYPE allowed"
								+ formatLineNumber()); */
								if (vg->depth != -1){
									e.et = parse_exception;
									e.subtype = 0;
									e.msg = "Parse Exception in parse()";
									e.sub_msg = "Error for DOCTYPE: DTD at wrong place";
									Throw e;
								}
								/*	throw new ParseException(
								"Error for DOCTYPE: DTD at wrong place"
								+ formatLineNumber());*/

								e.et = parse_exception;
								e.subtype = 0;
								e.msg = "Parse Exception in parse()";
								e.sub_msg = "Error for DOCTYPE: Invalid char sequence for DOCTYPE";
								Throw e;
								/*throw new ParseException(
								"Error for DOCTYPE: Invalid char sequence for DOCTYPE"
								+ formatLineNumber());*/
							}
					default :

						e.et = parse_exception;
						e.subtype = 0;
						e.msg = "Parse Exception in parse()";
						e.sub_msg = "Other Error: Unrecognized char after <!";
						Throw e;
						/*	throw new ParseException(
						"Other Error: Unrecognized char after <!"
						+ formatLineNumber()); */
						}
						break;
					default :
						e.et = parse_exception;
						e.subtype = 0;
						e.msg = "Parse Exception in parse()";
						e.sub_msg = "Other Error: Invalid char after <";
						Throw e;
						/*throw new ParseException(
						"Other Error: Invalid char after <"
						+ formatLineNumber());*/
							}
						}
						break;

					case STATE_START_TAG : //name space is handled by
						while (TRUE) {
							vg->ch = getChar2(vg);
							if (XMLChar_isNameChar(vg->ch)) {
								if (vg->ch == ':') {
									length2 = vg->offset - vg->temp_offset - vg->increment;
								}
							} else
								break;
						}
						length1 = vg->offset - vg->temp_offset - vg->increment;
						//writeVTD(offset, TOKEN_STARTING_TAG, length2:length1, depth)
						x = ((Long) length1 << 32) + vg->temp_offset;
						vg->tag_stack[vg->depth] = x;
						if (vg->depth > MAX_DEPTH) {
							e.et = parse_exception;
							e.subtype = 0;
							e.msg = "Parse Exception in parse()";
							e.sub_msg = "Other Error: Depth exceeds MAX_DEPTH";
							Throw e;
							/*throw new ParseException(
							"Other Error: Depth exceeds MAX_DEPTH"
							+ formatLineNumber());*/
						}
						// System.out.println(
						//     " " + (vg->temp_offset) + " " + length2 + ":" + length1 + " startingTag " + depth);
						if (vg->depth > vg->VTDDepth)
							vg->VTDDepth = vg->depth;
						if (vg->encoding < FORMAT_UTF_16BE){
							/*if (length2>MAX_PREFIX_LENGTH
							|| length1 > MAX_QNAME_LENGTH)
							throw new ParseException(
							"Token Length Error: Starting tag prefix or qname length too long"
							+formatLineNumber()); */
							if (length2>MAX_PREFIX_LENGTH 
								|| length1 > MAX_QNAME_LENGTH){
									e.et = parse_exception;
									e.subtype = 0;
									e.msg = "Parse Exception in parse()";
									e.sub_msg="Token Length Error: Starting tag prefix or qname length too long";
									Throw e;
								}

								writeVTD(vg,
									(vg->temp_offset),
									(length2 << 11) | length1,
									TOKEN_STARTING_TAG,
									vg->depth);
						}
						else{
							/*if (length2>MAX_PREFIX_LENGTH<<1
							|| length1 > MAX_QNAME_LENGTH<<1)
							throw new ParseException(
							"Token Length Error: Starting tag prefix or qname length too long"
							+formatLineNumber()); */
							if ((length2>(MAX_PREFIX_LENGTH<<1)) 
								|| (length1 > (MAX_QNAME_LENGTH <<1))){
									e.et = parse_exception;
									e.subtype = 0;
									e.msg = "Parse Exception in parse()";
									e.sub_msg="Token Length Error: Starting tag prefix or qname length too long";
									Throw e;
								}

								writeVTD(vg,
									(vg->temp_offset) >> 1,
									(length2 << 10) | (length1 >> 1),
									TOKEN_STARTING_TAG,
									vg->depth);
						}
						//offset += length1;
						length2 = 0;
						if (XMLChar_isSpaceChar(vg->ch)) {
							vg->ch = getCharAfterS(vg);
							if (XMLChar_isNameStartChar(vg->ch)) {
								// seen an attribute here
								vg->temp_offset = getPrevOffset(vg);
								parser_state = STATE_ATTR_NAME;
								break;
							}
						}
						if (vg->ch == '/') {
							vg->depth--;
							vg->ch = getChar(vg);
						}
						if (vg->ch == '>') {
							if (vg->depth != -1) {
								vg->temp_offset = vg->offset;
								vg->ch = getCharAfterSe(vg); // consume WSs
								if (vg->ch == '<') {
									parser_state = STATE_LT_SEEN;
									if (skipChar(vg,'/')) {
										length1 =
											vg->offset
											- vg->temp_offset
											- (vg->increment<<1);
										if (length1 > 0) {
											if (vg->encoding < FORMAT_UTF_16BE)
												writeVTD(vg,
												(vg->temp_offset),
												length1,
												TOKEN_CHARACTER_DATA,
												vg->depth);
											else
												writeVTD(vg,
												(vg->temp_offset) >> 1,
												(length1 >> 1),
												TOKEN_CHARACTER_DATA,
												vg->depth);
										}
										//offset += length1;
										parser_state = STATE_END_TAG;
										break;
									}
								} else if (XMLChar_isContentChar(vg->ch)) {
									//vg->temp_offset = offset;
									parser_state = STATE_TEXT;
								} else if (vg->ch == '&') {
									//has_amp = true;
									//vg->temp_offset = offset;
									entityIdentifier(vg);
									parser_state = STATE_TEXT;
								} else if (vg->ch == ']') {
								if (skipChar(vg,']')) {
									while (skipChar(vg,']')) {
									}
									if (skipChar(vg,'>')){		
										e.et = parse_exception;
										e.subtype = 0;
										e.msg = "Parse Exception in parse()";
										e.sub_msg = "Error in text content: ]]> in text content";
										Throw e;
									}
									/*throw new ParseException(
									"Error in text content: ]]> in text content"
									+ formatLineNumber());*/
								}
								parser_state = STATE_TEXT;
							   } else{		
									e.et = parse_exception;
									e.subtype = 0;
									e.msg = "Parse Exception in parse()";
									e.sub_msg = "Error in text content: Invalid char";
									Throw e;
								}
								/*throw new ParseException(
								"Error in text content: Invalid char"
								+ formatLineNumber());*/
							} else {
								parser_state = STATE_DOC_END;
							}
							break;
						}

						e.et = parse_exception;
						e.subtype = 0;
						e.msg = "Parse Exception in parse()";
						e.sub_msg = "Starting tag Error: Invalid char in starting tag";
						Throw e;
						/*throw new ParseException(
						"Starting tag Error: Invalid char in starting tag"
						+ formatLineNumber());*/

					case STATE_END_TAG :
						vg->temp_offset = vg->offset;
						
						sos = (int) vg->tag_stack[vg->depth];
						sl = (int) (vg->tag_stack[vg->depth] >> 32);
						vg->offset = vg->temp_offset + sl;
						if (vg->offset >= vg->endOffset){
							e.et = parse_exception;
							e.subtype = 0;
							e.msg = "Parse exception in parse()";
							e.sub_msg = "Premature EOF reached, XML document incomplete";
							Throw e;
						}

						if (memcmp(vg->XMLDoc+vg->temp_offset, vg->XMLDoc+sos, sl)!=0){
							e.et = parse_exception;
							e.subtype = 0;
							e.msg = "Parse Exception in parse()";
							e.sub_msg = "Ending tag error: Start/ending tag mismatch";
							Throw e;
						}
						// replace this with memcmp 
						//for (i = 0; i < sl; i++) {
						//	if (vg->XMLDoc[sos + i] != vg->XMLDoc[vg->temp_offset + i]){
						//		e.et = parse_exception;
						//		e.subtype = 0;
						//		e.msg = "Parse Exception in parse()";
						//		e.sub_msg = "Ending tag error: Start/ending tag mismatch";
						//		Throw e;
						//	}
						//}
							
						vg->depth--;
						vg->ch = getCharAfterS(vg);
						
						if (vg->ch!='>'){
							e.et = parse_exception;
							e.subtype = 0;
							e.msg = "Parse Exception in parse()";
							e.sub_msg = "Ending tag error: Invalid char in end tag";
							Throw e;
						}

						
						if (vg->depth != -1) {
							vg->temp_offset = vg->offset;
							vg->ch = getCharAfterS(vg);
							if (vg->ch == '<')
								parser_state = STATE_LT_SEEN;
							else if (XMLChar_isContentChar(vg->ch)) {
								parser_state = STATE_TEXT;
							} else if (vg->ch == '&') {
								//has_amp = true;
								entityIdentifier(vg);
								parser_state = STATE_TEXT;
							} else if (vg->ch == ']') {
								if (skipChar(vg,']')) {
									while (skipChar(vg,']')) {
									}
									if (skipChar(vg,'>')){		
										e.et = parse_exception;
										e.subtype = 0;
										e.msg = "Parse Exception in parse()";
										e.sub_msg = "Error in text content: ]]> in text content";
										Throw e;
									}
									
								}
								parser_state = STATE_TEXT;
							}
							else
							{	e.et = parse_exception;
								e.subtype = 0;
								e.msg = "Parse Exception in parse()";
								e.sub_msg = "Other Error: Invalid char in xml";
								Throw e;
							}
							
						} else
							parser_state = STATE_DOC_END;
						break;

						/*	throw new ParseException(
						"Other Error: Invalid char in ending tag"
						+ formatLineNumber());*/
					case STATE_UNRECORDED_TEXT :
						break;

					case STATE_PI_TAG :
						parser_state = process_pi_tag(vg);
						break;

						//throw new ParseException("Error in PI: Invalid char");
					case STATE_PI_VAL :
						parser_state = process_pi_val(vg);
						break;


					case STATE_DEC_ATTR_NAME :
						parser_state = process_dec_attr(vg);
						break;
				
					case STATE_COMMENT :
						parser_state = process_comment(vg);
						break;
						/*throw new ParseException(
						"Error in comment: Invalid terminating sequence"
						+ formatLineNumber());*/
					case STATE_CDATA :
						parser_state = process_cdata(vg);
						break;
						
					case STATE_DOCTYPE :
						parser_state = process_doc_type(vg);
						break;
					
					case STATE_TEXT :
						if (vg->depth == -1){		
							e.et = parse_exception;
							e.subtype = 0;
							e.msg = "Parse Exception in parse()";
							e.sub_msg = "Error in text: Char data at the wrong place";
							Throw e;
						}
						/*throw new ParseException(
						"Error in text: Char data at the wrong place"
						+ formatLineNumber());*/
						while (TRUE) {
							vg->ch = getChar2(vg);
							if (XMLChar_isContentChar(vg->ch)) {
							} else if (vg->ch == '&') {
								//has_amp = true;
								if (!XMLChar_isValidChar(entityIdentifier(vg))){		
									e.et = parse_exception;
									e.subtype = 0;
									e.msg = "Parse Exception in parse()";
									e.sub_msg = "Error in text content: Invalid char in text content";
									Throw e;
								}
								/*throw new ParseException(
								"Error in text content: Invalid char in text content "
								+ formatLineNumber());*/

								//parser_state = STATE_TEXT;
							} else if (vg->ch == '<') {
								break;
							} else if (vg->ch == ']') {
								if (skipChar(vg,']')) {
									while (skipChar(vg,']')) {
									}
									if (skipChar(vg,'>')){		
										e.et = parse_exception;
										e.subtype = 0;
										e.msg = "Parse Exception in parse()";
										e.sub_msg = "Error in text content: ]]> in text content";
										Throw e;
									}
									/*throw new ParseException(
									"Error in text content: ]]> in text content"
									+ formatLineNumber());*/
								}
							} else{		
								e.et = parse_exception;
								e.subtype = 0;
								e.msg = "Parse Exception in parse()";
								e.sub_msg = "Error in text content: Invalid char in text content";
								Throw e;
							}
							/*throw new ParseException(
							"Error in text content: Invalid char in text content"
							+ formatLineNumber());*/
						}
						length1 = vg->offset - vg->increment - vg->temp_offset;
						//if (has_amp) {
						/*System.out.println(
						" " + vg->temp_offset + " " + length1 + " text with amp " + vg->depth);*/
						if (vg->encoding < FORMAT_UTF_16BE){
							writeVTD(vg,
								vg->temp_offset,
								length1,
								TOKEN_CHARACTER_DATA,
								vg->depth);
						}
						else{
							writeVTD(vg,
								vg->temp_offset >> 1,
								length1 >> 1,
								TOKEN_CHARACTER_DATA,
								vg->depth);
						}
						//} else {
						//System.out.println(" " + vg->temp_offset + " " + length1 + " text " + vg->depth);
						//  if (encoding < 3)
						//  writeVTD(vg->temp_offset, length1, TOKEN_CHARACTER_DATA_NO_ENTITY, vg->depth);
						//else
						//writeVTD(vg->temp_offset >> 1, length1 >> 1, TOKEN_CHARACTER_DATA_NO_ENTITY, vg->depth);
						//}
						//has_amp = true;
						parser_state = STATE_LT_SEEN;
						break;

					case STATE_ATTR_NAME :

						if (vg->ch == 'x') {
							if (skipChar(vg,'m')
								&& skipChar(vg,'l')
								&& skipChar(vg,'n')
								&& skipChar(vg,'s')) {
									vg->ch = getChar(vg);
									if (vg->ch == '='
										|| XMLChar_isSpaceChar(vg->ch)
										|| vg->ch == ':') {
											is_ns = TRUE; //break;
										}
								}
						}
						while (TRUE) {
							if (XMLChar_isNameChar(vg->ch)) {
								if (vg->ch == ':') {
									length2 = vg->offset - vg->temp_offset - vg->increment;
								}
								vg->ch = getChar2(vg);
							} else
								break;
						}
						length1 = getPrevOffset(vg) - vg->temp_offset;
						// check for uniqueness here
						/*
						boolean unique = true;
						boolean unequal;
						for (int i = 0; i < attr_count; i++) {
						unequal = false;
						int prevLen = (int) attr_name_array[i];
						if (length1 == prevLen) {
						int prevOffset = (int) (attr_name_array[i] >> 32);
						for (int j = 0; j < prevLen; j++) {
						if (XMLDoc[prevOffset + j] != XMLDoc[vg->temp_offset + j]) {
						unequal = true;
						break;
						}
						}
						} else
						unequal = true;
						unique = unique && unequal;
						}*/
						unique = TRUE;
						//unequal;
						for (i = 0; i < attr_count; i++) {
							int prevLen;
							unequal = FALSE;
							prevLen = (int) vg->attr_name_array[i];
							if (length1 == prevLen) {
								int prevOffset =
									(int) (vg->attr_name_array[i] >> 32);
								for (j = 0; j < prevLen; j++) {
									if (vg->XMLDoc[prevOffset + j]
									!= vg->XMLDoc[vg->temp_offset + j]) {
										unequal = TRUE;
										break;
									}
								}
							} else
								unequal = TRUE;
							unique = unique && unequal;
						}
						if (!unique && attr_count != 0){		
							e.et = parse_exception;
							e.subtype = 0;
							e.msg = "Parse Exception in parse()";
							e.sub_msg = "Error in attr: Attr name not unique";
							Throw e;
						}
						/*throw new ParseException(
						"Error in attr: Attr name not unique"
						+ formatLineNumber());*/
						unique = TRUE;
						if (attr_count < vg->anaLen) {
							vg->attr_name_array[attr_count] =
								((Long) (vg->temp_offset) << 32) + length1;
							attr_count++;
						} else // grow the attr_name_array by 16
						{
							Long* temp_array = vg->attr_name_array;
							/*System.out.println(
							"size increase from "
							+ temp_array.length
							+ "  to "
							+ (attr_count + 16));*/
							vg->attr_name_array = 
								(Long *)malloc(sizeof(Long)*
								(attr_count + ATTR_NAME_ARRAY_SIZE));
							/*new long[attr_count + ATTR_NAME_ARRAY_SIZE];*/

							if (vg->attr_name_array == NULL){
								e.et=out_of_mem;
								e.subtype = 0;
								e.msg = "alloc mem for attr_name_array_failed";
								Throw e;
							}
							vg->anaLen = attr_count + ATTR_NAME_ARRAY_SIZE;

							for (i = 0; i < attr_count; i++) {
								vg->attr_name_array[i] = temp_array[i];
							}
							vg->attr_name_array[attr_count] =
								((Long) (vg->temp_offset) << 32) + length1;
							attr_count++;
						}
						// after checking, write VTD
						if (is_ns) {
							if (vg->encoding < FORMAT_UTF_16BE){

								/*if (length2>MAX_PREFIX_LENGTH
								|| length1 > MAX_QNAME_LENGTH)
								throw new ParseException(
								"Token Length Error: Starting tag prefix or qname length too long"
								+formatLineNumber()); */	
								if (length2>MAX_PREFIX_LENGTH 
									|| length1 > MAX_QNAME_LENGTH){
										e.et = parse_exception;
										e.subtype = 0;
										e.msg = "Parse Exception in parse()";
										e.sub_msg="Token Length Error: Attr NS prefix or qname length too long";
										Throw e;
									}

									writeVTD(vg,
										vg->temp_offset,
										(length2 << 11) | length1,
										TOKEN_ATTR_NS,
										vg->depth);

							}
							else{
								/*if (length2>MAX_PREFIX_LENGTH<<1
								|| length1 > MAX_QNAME_LENGTH<<1 )
								throw new ParseException(
								"Token Length Error: Starting tag prefix or qname length too long"
								+formatLineNumber()); */

								if (length2>(MAX_PREFIX_LENGTH<<1) 
									|| length1 >(MAX_QNAME_LENGTH<<1)){
										e.et = parse_exception;
										e.subtype = 0;
										e.msg = "Parse Exception in parse()";
										e.sub_msg="Token Length Error: Attr NS prefix or qname length too long";
										Throw e;
									}

									writeVTD(vg,
										vg->temp_offset >> 1,
										(length2 << 10) | (length1 >> 1),
										TOKEN_ATTR_NS,
										vg->depth);

							}
							is_ns = FALSE;
						} else {
							if (vg->encoding < FORMAT_UTF_16BE){
								/*if (length2>MAX_PREFIX_LENGTH
								|| length1 > MAX_QNAME_LENGTH)
								throw new ParseException(
								"Token Length Error: Starting tag prefix or qname length too long"
								+formatLineNumber()); */
								if (length2>MAX_PREFIX_LENGTH 
									|| length1 > MAX_QNAME_LENGTH){
										e.et = parse_exception;
										e.subtype = 0;
										e.msg = "Parse Exception in parse()";
										e.sub_msg="Token Length Error: Attr name prefix or qname length too long";
										Throw e;
									}
									writeVTD(vg,
										vg->temp_offset,
										(length2 << 11) | length1,
										TOKEN_ATTR_NAME,
										vg->depth);

							}
							else {
								/*if (length2>MAX_PREFIX_LENGTH<<1
								|| length1 > MAX_QNAME_LENGTH <<1 )
								throw new ParseException(
								"Token Length Error: Starting tag prefix or qname length too long"
								+formatLineNumber()); */
								if (length2> (MAX_PREFIX_LENGTH <<1)
									|| length1 > (MAX_QNAME_LENGTH<<1)){
										e.et = parse_exception;
										e.subtype = 0;
										e.msg = "Parse Exception in parse()";
										e.sub_msg="Token Length Error: Attr name prefix or qname length too long";
										Throw e;
									}

									writeVTD(vg,
										vg->temp_offset >> 1,
										(length2 << 10) | (length1 >> 1),
										TOKEN_ATTR_NAME,
										vg->depth);

							}
						}
						/*System.out.println(
						" " + vg->temp_offset + " " + length2 + ":" + length1 + " attr name " + vg->depth);*/
						length2 = 0;
						if (XMLChar_isSpaceChar(vg->ch)) {
							vg->ch = getCharAfterS(vg);
						}
						if (vg->ch != '='){		
							e.et = parse_exception;
							e.subtype = 0;
							e.msg = "Parse Exception in parse()";
							e.sub_msg = "Error in attr: invalid char";
							Throw e;
						}
						/*throw new ParseException(
						"Error in attr: invalid char"
						+ formatLineNumber());*/
						vg->ch_temp = getCharAfterS(vg);
						if (vg->ch_temp != '"' && vg->ch_temp != '\''){		
							e.et = parse_exception;
							e.subtype = 0;
							e.msg = "Parse Exception in parse()";
							e.sub_msg = "Error in attr: invalid char (should be ' or \" )";
							Throw e;
						}
						/*throw new ParseException(
						"Error in attr: invalid char (should be ' or \" )"
						+ formatLineNumber());*/
						vg->temp_offset = vg->offset;
						parser_state = STATE_ATTR_VAL;
						break;

					case STATE_ATTR_VAL :
						while (TRUE) {
							vg->ch = getChar2(vg);
							if (XMLChar_isValidChar(vg->ch) && vg->ch != '<') {
								if (vg->ch == vg->ch_temp)
									break;
								if (vg->ch == '&') {
									//has_amp = true;
									// as in vtd spec, we mark attr val with entities
									if (!XMLChar_isValidChar(entityIdentifier(vg))) {		
										e.et = parse_exception;
										e.subtype = 0;
										e.msg = "Parse Exception in parse()";
										e.sub_msg = "Error in attr: Invalid XML char";
										Throw e;
									}
									/*{
									throw new ParseException(
									"Error in attr: Invalid XML char"
									+ formatLineNumber());
									}*/
								}

							} else {		
								e.et = parse_exception;
								e.subtype = 0;
								e.msg = "Parse Exception in parse()";
								e.sub_msg = "Error in attr: Invalid XML char";
								Throw e;
							}
							/*throw new ParseException(
							"Error in attr: Invalid XML char"
							+ formatLineNumber());*/
						}

						length1 = vg->offset - vg->temp_offset - vg->increment;
						if (vg->encoding < FORMAT_UTF_16BE){
							if (length1 > MAX_TOKEN_LENGTH){
								e.et = parse_exception;
								e.subtype = 0;
								e.msg = "Parse Exception in parse()";
								e.sub_msg="Token Length Error: ATTR_VAL length too long";
								Throw e;
							}
							writeVTD(vg,
								vg->temp_offset,
								length1,
								TOKEN_ATTR_VAL,
								vg->depth);
						}
						else{
							if (length1 > (MAX_TOKEN_LENGTH << 1)){
								e.et = parse_exception;
								e.subtype = 0;
								e.msg = "Parse Exception in parse()";
								e.sub_msg="Token Length Error: ATTR_VAL length too long";
								Throw e;
							}
							writeVTD(vg,
								vg->temp_offset >> 1,
								length1 >> 1,
								TOKEN_ATTR_VAL,
								vg->depth);
						}
						vg->ch = getChar(vg);
						if (XMLChar_isSpaceChar(vg->ch)) {
							vg->ch = getCharAfterS(vg);
							if (XMLChar_isNameStartChar(vg->ch)) {
								vg->temp_offset = vg->offset - vg->increment;
								parser_state = STATE_ATTR_NAME;
								break;
							}
						}

						if (vg->ch == '/') {
							vg->depth--;
							vg->ch = getChar(vg);
						}

						if (vg->ch == '>') {
							attr_count = 0;
							if (vg->depth != -1) {
								vg->temp_offset = vg->offset;
								vg->ch = getCharAfterSe(vg);
								if (vg->ch == '<') {
									parser_state = STATE_LT_SEEN;
								} else if (XMLChar_isContentChar(vg->ch)) {
									//vg->temp_offset = offset;
									parser_state = STATE_TEXT;
								} else if (vg->ch == '&') {
									//has_amp = true;
									//vg->temp_offset = offset;
									entityIdentifier(vg);
									parser_state = STATE_TEXT;
								} else if (vg->ch == ']') {
								if (skipChar(vg,']')) {
									while (skipChar(vg,']')) {
									}
									if (skipChar(vg,'>')){		
										e.et = parse_exception;
										e.subtype = 0;
										e.msg = "Parse Exception in parse()";
										e.sub_msg = "Error in text content: ]]> in text content";
										Throw e;
									}
									/*throw new ParseException(
									"Error in text content: ]]> in text content"
									+ formatLineNumber());*/
								}
								parser_state = STATE_TEXT;
							   }
								else{		
									e.et = parse_exception;
									e.subtype = 0;
									e.msg = "Parse Exception in parse()";
									e.sub_msg = "Error in text content: Invalid char";
									Throw e;
								}
								/*	throw new ParseException(
								"Error in text content: Invalid char"
								+ formatLineNumber());*/
							} else {
								parser_state = STATE_DOC_END;
							}
							break;
						}

						e.et = parse_exception;
						e.subtype = 0;
						e.msg = "Parse Exception in parse()";
						e.sub_msg = "Starting tag Error: Invalid char in starting tag";
						Throw e;

						/*throw new ParseException(
						"Starting tag Error: Invalid char in starting tag"
						+ formatLineNumber());*/

					case STATE_END_PI :										  

						parser_state = process_end_pi(vg);
						break;

					case STATE_END_COMMENT :
						parser_state = process_end_comment(vg);
						break;

					default :	
						e.et = parse_exception;
						e.subtype = 0;
						e.msg = "Parse Exception in parse()";
						e.sub_msg = "Other error: invalid parser state";
						Throw e;
						/*throw new ParseException(
						"Other error: invalid parser state"
						+ formatLineNumber());*/
			}
		}
	} 
	Catch (e) {
		if (parser_state != STATE_DOC_END 
			|| e.subtype == -1){
			printLineNumber(vg);		
			Throw e;
		}
		finishUp(vg);
		//else {
		//	if (e.et == parse_exception){
		//		printf(e.msg);
		//		printLineNumber(vg);
		//		printf("\n");
		//	}
		//}
	}


}


// Set the XMLDoc container.
void setDoc(VTDGen *vg, UByte *ba, int len){
	int a;
	vg->depth = -1;
	vg->increment = 1;
	vg->BOM_detected = FALSE;
	vg->must_utf_8 = FALSE;
	vg->ch = vg->ch_temp = 0;
	vg->temp_offset = 0;
	vg->XMLDoc = ba;
	vg->docOffset = vg->offset = 0;
	vg->docLen = len;
	vg->endOffset = len;
	if (vg->docLen <= 1024) {
		//a = 1024; //set the floor
		a = 10;
	} else if (vg->docLen <= 1024 * 16 * 4) {
		//a = 2048;
		a = 11;
	} else if (vg->docLen <= 1024 * 256) {
		//a = 1024 * 4;
		a = 12;
	} else {
		//a = 1 << 15;
		a = 15;
	}
	//VTDBuffer = new FastLongBuffer(a);
	//l1Buffer = new FastLongBuffer(128);
	//l2Buffer = new FastLongBuffer(512);
	//l3Buffer = new FastIntBuffer(2048);
	vg->VTDBuffer = createFastLongBuffer3(a, len>>(a+1)); //new FastLongBuffer2(a, ba.length >> (a+1));
	vg->l1Buffer = createFastLongBuffer2(7); //new FastLongBuffer2(7);
	vg->l2Buffer = createFastLongBuffer2(9); //new FastLongBuffer2(9);
	vg->l3Buffer = createFastIntBuffer2(11);  //new FastIntBuffer2(11);
	vg->vtdSize = vg->l1Size = vg->l2Size = vg->l3Size = 0;
}

// Set the XMLDoc container.Also set the offset and len of the document
// len is the size of the byte buffer
// docLen is the length of the XML content in byte
void setDoc2(VTDGen *vg, UByte *ba, int len, int os, int docLen){
	int a;
	vg->depth = -1;
	vg->increment = 1;
	vg->BOM_detected = FALSE;
	vg->must_utf_8 = FALSE;
	vg->ch = vg->ch_temp = 0;
	vg->temp_offset = 0;
	vg->XMLDoc = ba;
	vg->docOffset = vg->offset = os;
	vg->docLen = len;
	vg->endOffset = os + docLen;
	if (docLen <= 1024) {
		//a = 1024; //set the floor
		a = 10;
	} else if (docLen <= 1024 * 16 * 4) {
		//a = 2048;
		a = 11;
	} else if (docLen <= 1024 * 256) {
		//a = 1024 * 4;
		a = 12;
	} else {
		//a = 1 << 15;
		a = 15;
	}
	//VTDBuffer = new FastLongBuffer(a);
	//l1Buffer = new FastLongBuffer(128);
	//l2Buffer = new FastLongBuffer(512);
	//l3Buffer = new FastIntBuffer(2048);
	vg->VTDBuffer = createFastLongBuffer3(a, len>>(a+1)); //new FastLongBuffer2(a, ba.length >> (a+1));
	vg->l1Buffer = createFastLongBuffer2(7); //new FastLongBuffer2(7);
	vg->l2Buffer = createFastLongBuffer2(9); //new FastLongBuffer2(9);
	vg->l3Buffer = createFastIntBuffer2(11);  //new FastIntBuffer2(11);
	vg->vtdSize = vg->l1Size = vg->l2Size = vg->l3Size = 0;
}

// Increments offset only when the next char matches a given value.
static Boolean skipChar(VTDGen *vg, int ch){
	exception e;
	int temp = 0;
	int a = 0, c = 0, d = 0, val = 0, i=0;
	if (vg->offset >= vg->endOffset){
		e.et = parse_exception;
		e.subtype = 0;
		e.msg = "Parse exception in skipChar";
		e.sub_msg = "Premature EOF reached, XML document incomplete";
		Throw e;
	}	
	//throw new EOFException("premature EOF reached");
	switch (vg->encoding) {
			case FORMAT_ASCII :
				temp = vg->XMLDoc[vg->offset];
				if (temp>127){
					e.et = parse_exception;
					e.subtype = 0;
					e.msg = "Parse exception in getChar";
					e.sub_msg = "Invalid char for ASCII encoding";
					Throw e;
				}
				if (ch == temp) {
					vg->offset++;
					return TRUE;
				} else {
					return FALSE;
				}
			case FORMAT_UTF8 :
				temp = vg->XMLDoc[vg->offset];
				if (temp <128) {
					if (ch == temp) {
						vg->offset++;
						return TRUE;
					} else {
						return FALSE;
					}
				}

				//temp = temp & 0xff;

				switch (UTF8Char_byteCount(temp)) { // handle multi-byte code

			case 2 :
				c = 0x1f;
				// A mask determine the val portion of the first byte
				d = 6; // 
				a = 1; //
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
				e.et = parse_exception;
				e.subtype = 0;
				e.msg = "Parse exception in skipChar";
				e.sub_msg = "UTF 8 encoding error: should never happen";
				Throw e;
				//throw new ParseException("UTF 8 encoding error: should never happen");
				}

				val = (temp & c) << d;
				i = a - 1;
				while (i >= 0) {
					temp = vg->XMLDoc[vg->offset + a - i];
					if ((temp & 0xc0) != 0x80){
						e.et = parse_exception;
						e.subtype = 0;
						e.msg = "Parse exception in skipChar";
						e.sub_msg = "UTF 8 encoding error: should never happen";
						Throw e;
					}
					//throw new ParseException("UTF 8 encoding error: should never happen");
					val = val | ((temp & 0x3f) << ((i<<2)+(i<<1)));
					i--;
				}

				if (val == ch) {
					vg->offset += a + 1;
					return TRUE;
				} else {
					return FALSE;
				}

			case FORMAT_UTF_16BE :
				// implement UTF-16BE to UCS4 conversion
				temp = vg->XMLDoc[vg->offset] << 8 | vg->XMLDoc[vg->offset + 1];
				if ((temp < 0xd800)
					|| (temp >= 0xdc00)) { // not a high surrogate
						//offset += 2;
						if (temp == ch) {
							vg->offset += 2;
							return TRUE;
						} else
							return FALSE;
					} else {
						val = temp;
						temp = vg->XMLDoc[vg->offset + 2] << 8 | vg->XMLDoc[vg->offset + 3];
						if (temp < 0xdc00 || temp > 0xdfff) {
							// has to be a low surrogate here{
							e.et = parse_exception;
							e.subtype = 0;
							e.msg = "Parse exception in skipChar";
							e.sub_msg = "UTF 16 BE encoding error: should never happen";
							Throw e;					
							//throw new EncodingException("UTF 16 BE encoding error: should never happen");
						}
						//val = (val - 0xd800) * 0x400 + (temp - 0xdc00) + 0x10000;
						val = ((val - 0xd800) << 10) + (temp - 0xdc00) + 0x10000;

						if (val == ch) {
							vg->offset += 4;
							return TRUE;
						} else
							return FALSE;
					}
			case FORMAT_UTF_16LE :
				temp = vg->XMLDoc[vg->offset + 1] << 8 | vg->XMLDoc[vg->offset];
				if (temp < 0xdc00 || temp > 0xdfff) { // check for low surrogate
					if (temp == ch) {
						vg->offset += 2;
						return TRUE;
					} else {
						return FALSE;
					}
				} else {
					val = temp;
					temp = vg->XMLDoc[vg->offset + 3] << 8 | vg->XMLDoc[vg->offset + 2];
					if (temp < 0xd800 || temp > 0xdc00) {
						// has to be high surrogate
						// has to be a low surrogate here{
						e.et = parse_exception;
						e.subtype = 0;
						e.msg = "Parse exception in skipChar";
						e.sub_msg = "UTF 16 LE encoding error: should never happen";
						Throw e;
						//throw new EncodingException("UTF 16 BE encoding error: should never happen");
					}
					//val = (temp - 0xd800) * 0x400 + (val - 0xdc00) + 0x10000;
					val = ((temp - 0xd800)<<10) + (val - 0xdc00) + 0x10000;

					if (val == ch) {
						vg->offset += 4;
						return TRUE;
					} else
						return FALSE;
				}
			case FORMAT_ISO_8859 :
				temp = vg->XMLDoc[vg->offset];
				if (temp == ch) {
					vg->offset++;
					return TRUE;
				} else {
					return FALSE;
				}
			default :
				e.et = parse_exception;
				e.subtype = 0;
				e.msg = "Parse exception in skipChar";
				e.sub_msg = "Unknown encoding";
				Throw e;
				//throw new EncodingException("Unknown encoding");
	}
}

// Write the VTD and lc into their storage container.
// needs to take care byte endian
// the current implementation only swap bytes for vtd for small endians
// LCs are not swapped, so when they got persisted, byte swap may be needed

static void writeVTD(VTDGen *vg, int offset, int length, tokenType token_type, int depth){
	
	//Long l = ((Long) ((token_type << 4)
	//					|(((depth & 0x0f) << 12) | (depth & 0xf0) >> 4)
	//						| swap_bytes(length))
	//						| (((Long)swap_bytes(offset)) << 32));
	//
	//Long lt[] = { (l&0x00000000000000ffL), 
	//			  (l&0x000000000000ff00L)>>8,
	//	   		  (l&0x0000000000ff0000L)>>16,
	//			  (l&0x00000000ff000000L)>>24,
	//			  (l&0x000000ff00000000L)>>32,
	//			  (l&0x0000ff0000000000L)>>40,
	//			  (l&0x00ff000000000000L)>>48,
	//			 ((l&0xff00000000000000L)>>56)&0xff,
	//	   };
	//int i;
	////printf(" offset --> %d ; length -->%d ; tokenType ---> %d ; depth --> %d \n", offset, length, token_type, depth);
	//for (i=0;i<8;i++){
	//	if (lt[i] ==0)
	//		printf("00 ");
	//	else if (lt[i] < 0x10){
	//		printf("0%x ",lt[i]);
	//	}
	//	else {
	//		printf("%x ",lt[i]);
	//	}
	//}
	//printf("\n");
	
	switch (token_type) {
			case TOKEN_CHARACTER_DATA:
			case TOKEN_CDATA_VAL:
			case TOKEN_COMMENT:

				if (length > MAX_TOKEN_LENGTH) {
					int k;
					int r_offset = offset;
#if BIG_ENDIAN
					Long l = ((Long)((token_type << 28)
						| ((depth & 0xff) << 20) 
						| MAX_TOKEN_LENGTH) << 32);
#else
					Long l = ((Long)(token_type << 4)
							| (((depth & 0x0f) << 12) | (depth & 0xf0) >> 4)
							| swap_bytes(MAX_TOKEN_LENGTH));
#endif
					for (k = length; k > MAX_TOKEN_LENGTH; k = k - MAX_TOKEN_LENGTH) {
#if BIG_ENDIAN
						appendLong(vg->VTDBuffer, l | r_offset);
#else
						appendLong(vg->VTDBuffer, l | (((Long)swap_bytes(r_offset))<< 32));
#endif
//#if BIG_ENDIAN
//
//						appendLong(vg->VTDBuffer,((Long) ((token_type << 28)
//							| ((depth & 0xff) << 20) | MAX_TOKEN_LENGTH) << 32)
//							| r_offset);
//#else
//
//						appendLong(vg->VTDBuffer,((Long) ((token_type << 4)
//							| (((depth & 0xf) << 12)|(depth & 0xf0) >> 4)
//							| 
//#endif

						r_offset += MAX_TOKEN_LENGTH;
					}
#if BIG_ENDIAN

					appendLong(vg->VTDBuffer,((Long) ((token_type << 28)
						| ((depth & 0xff) << 20) | k) << 32)
						| r_offset);
#else
					appendLong(vg->VTDBuffer,((Long) ((token_type << 4)
						|(((depth & 0x0f) << 12) | (depth & 0xf0) >> 4)
							| swap_bytes(k))
							| (((Long)swap_bytes(r_offset)) << 32)));
#endif
				} else {
#if BIG_ENDIAN
					appendLong(vg->VTDBuffer,((Long) ((token_type << 28)
						| ((depth & 0xff) << 20) | length) << 32)
						| offset);
#else
					appendLong(vg->VTDBuffer,((Long) ((token_type << 4)
						|(((depth & 0x0f) << 12) | (depth & 0xf0) >> 4)
							| swap_bytes(length))
							| (((Long)swap_bytes(offset)) << 32)));
#endif
				}
				break;

			//case TOKEN_ENDING_TAG: break;

			default:
#if BIG_ENDIAN

				appendLong(vg->VTDBuffer,((Long) ((token_type << 28)
					| ((depth & 0xff) << 20) | length) << 32)
					| offset);
#else
				appendLong(vg->VTDBuffer,((Long) ((token_type << 4)
						|(((depth & 0x0f) << 12) | (depth & 0xf0) >> 4)
							| swap_bytes(length))
							| (((Long)swap_bytes(offset)) << 32)));

#endif
				break;
	}


	// remember VTD depth start from zero
	if (token_type == TOKEN_STARTING_TAG) {
		switch (depth) {
				case 0 :
					//rootIndex = VTDBuffer.size() - 1;
					vg->rootIndex = vg->VTDBuffer->size - 1;
					break;
				case 1 :
					if (vg->last_depth == 1) {
#if BIG_ENDIAN
						appendLong(vg->l1Buffer,
							((Long) vg->last_l1_index << 32) | 0xffffffffL);
#else
						appendLong(vg->l1Buffer,
							((Long) vg->last_l1_index << 32) | 0xffffffffL);
#endif
					} else if (vg->last_depth == 2) {
#if BIG_ENDIAN
						appendLong(vg->l2Buffer,
							((Long) vg->last_l2_index << 32) | 0xffffffffL);
#else
						appendLong(vg->l2Buffer,
							((Long) vg->last_l2_index << 32) | 0xffffffffL);
#endif
					}
					vg->last_l1_index = vg->VTDBuffer->size - 1;
					vg->last_depth = 1;
					break;
				case 2 :
					if (vg->last_depth == 1) {
#if BIG_ENDIAN
						appendLong(vg->l1Buffer,
							((Long) vg->last_l1_index << 32) + vg->l2Buffer->size);
#else
						appendLong(vg->l1Buffer,
							((Long) vg->last_l1_index << 32) + vg->l2Buffer->size);
#endif
					} else if (vg->last_depth == 2) {
#if BIG_ENDIAN
						appendLong(vg->l2Buffer,
							((Long) vg->last_l2_index << 32) | 0xffffffffL);
#else
						appendLong(vg->l2Buffer,
							((Long) vg->last_l2_index << 32) | 0xffffffffL);
#endif
					}
					vg->last_l2_index = vg->VTDBuffer->size - 1;
					vg->last_depth = 2;
					break;

				case 3 :
#if BIG_ENDIAN
					appendInt(vg->l3Buffer, vg->VTDBuffer->size - 1);
					if (vg->last_depth == 2) {
						appendLong(vg->l2Buffer,
							((Long)vg->last_l2_index << 32) + vg->l3Buffer->size - 1);
					}
#else
					appendInt(vg->l3Buffer, vg->VTDBuffer->size - 1);
					if (vg->last_depth == 2) {
						appendLong(vg->l2Buffer,
							((Long)vg->last_l2_index << 32) + vg->l3Buffer->size - 1);
					}
#endif
					vg->last_depth = 3;
					break;
				default :
					//rootIndex = VTDBuffer.size() - 1;
					break;
		}

	} /*else if (token_type == TOKEN_ENDING_TAG && (depth == 0)) {
		if (vg->last_depth == 1) {
#if BIG_ENDIAN
			appendLong(
				vg->l1Buffer, ((Long) vg->last_l1_index << 32) | 0xffffffffL);
#else
			appendLong(
				vg->l1Buffer, ((Long) vg->last_l1_index << 32) | 0xffffffffL);
#endif
		} else if (vg->last_depth == 2) {
#if BIG_ENDIAN
			appendLong(
				vg->l2Buffer, ((Long) vg->last_l2_index << 32) | 0xffffffffL);
#else
			appendLong(
				vg->l2Buffer, ((Long) vg->last_l2_index << 32) | 0xffffffffL);
#endif
		}
	}*/
}


// finishing up
void finishUp(VTDGen *vg){
	if (vg->last_depth == 1) {
		appendLong(vg->l1Buffer,((Long) vg->last_l1_index << 32) | 0xffffffffL);
	} else if (vg->last_depth == 2) {
		appendLong(vg->l2Buffer,((Long) vg->last_l2_index << 32) | 0xffffffffL);
	}
}

void decide_encoding(VTDGen *vg){
	exception e;

	if (vg->XMLDoc[vg->offset] == (UByte) -2) {
		vg->increment = 2;
		if (vg->XMLDoc[vg->offset + 1] == (UByte)-1) {
			vg->offset += 2;
			vg->encoding = FORMAT_UTF_16BE;
			vg->BOM_detected = TRUE;
			//r = new UTF16BEReader();
		} else{
			e.et = parse_exception;
			e.subtype = 0;
			e.msg = "Parse Exception in parse()";
			e.sub_msg = "Unknown Character encoding: should be 0xff 0xfe";
			Throw e;
		}
		//	throw new EncodingException("Unknown Character encoding: should be 0xff 0xfe");
	} else if (vg->XMLDoc[vg->offset] == (UByte)-1) {
		vg->increment = 2;
		if (vg->XMLDoc[vg->offset + 1] == (UByte)-2) {
			vg->offset += 2;
			vg->encoding = FORMAT_UTF_16LE;
			vg->BOM_detected = TRUE;
			//r = new UTF16LEReader();
		} else{
			e.et = parse_exception;
			e.subtype = 0;
			e.msg = "Parse Exception in parse()";
			e.sub_msg = "Unknown Character encoding";
			Throw e;
		}
		//throw new EncodingException("Unknown Character encoding");
	} else if (vg->XMLDoc[vg->offset] == (UByte)-17) {
		if (vg->XMLDoc[vg->offset+1] == (UByte)-69 && vg->XMLDoc[vg->offset+2]==(UByte)-65){
			vg->offset +=3;
			vg->must_utf_8= TRUE;
		}
		else {
			e.et = parse_exception;
			e.subtype = 0;
			e.msg = "Parse Exception in parse()";
			e.sub_msg = "Unknown Character encoding: not UTF-8";
			Throw e;
		}
		//	throw new EncodingException("Unknown Character encoding: not UTF-8");

	} else if (vg->XMLDoc[vg->offset] == 0) {
		if (vg->XMLDoc[vg->offset+1] == 0x3c 
			&& vg->XMLDoc[vg->offset+2] == 0 
			&& vg->XMLDoc[vg->offset+3] == 0x3f){
				vg->encoding = FORMAT_UTF_16BE;
				vg->increment = 2;
			}
		else{
			e.et = parse_exception;
			e.subtype = 0;
			e.msg = "Parse Exception in parse()";
			e.sub_msg = "Unknown Character encoding: not UTF-16BE";
			Throw e;
		}
		//	throw new EncodingException("Unknown Character encoding: not UTF-16BE");
	} else if (vg->XMLDoc[vg->offset] == 0x3c) {
		if (vg->XMLDoc[vg->offset+1] == 0 
			&& vg->XMLDoc[vg->offset+2] == 0x3f 
			&& vg->XMLDoc[vg->offset+3] == 0x0){
				vg->increment = 2;
				vg->encoding = FORMAT_UTF_16LE;
			}
	}

	if (vg->encoding < FORMAT_UTF_16BE) {
		if ((unsigned int)(vg->offset + vg->docLen) >= (((unsigned int)1) << 30)){
			e.et = parse_exception;
			e.subtype = 0;
			e.msg = "Parse Exception in parse()";
			e.sub_msg = "Other error: file size too large";
			Throw e;
		}
		//throw new ParseException("Other error: file size too large ");
	} else {
		if ((unsigned int)(vg->offset - 2 + vg->docLen) >= (((unsigned int) 1) << 31)){
			e.et = parse_exception;
			e.subtype = 0;
			e.msg = "Parse Exception in parse()";
			e.sub_msg = "Other error: file size too large";
			Throw e;
		}
		//throw new ParseException("Other error: file size too large ");
	}
}
int process_end_pi(VTDGen *vg){
	exception e;
	int length1,parser_state;
	vg->ch = getChar(vg);
	if (XMLChar_isNameStartChar(vg->ch)) {
		if ((vg->ch == 'x' || vg->ch == 'X')
			&& (skipChar(vg,'m') || skipChar(vg,'M'))
			&& (skipChar(vg,'l') && skipChar(vg,'L'))) {
				//vg->temp_offset = offset;
				vg->ch = getChar(vg);
				if (XMLChar_isSpaceChar(vg->ch) || vg->ch == '?'){		
					e.et = parse_exception;
					e.subtype = 0;
					e.msg = "Parse Exception in parse()";
					e.sub_msg = "Error in PI: [xX][mM][lL] not a valid PI target";
					Throw e;
				}
				/*throw new ParseException(
				"Error in PI: [xX][mM][lL] not a valid PI target"
				+ formatLineNumber());*/
				//offset = vg->temp_offset;
			}

			while (TRUE) {
				//vg->ch = getChar(vg);
				if (!XMLChar_isNameChar(vg->ch)) {
					break;
				}
				vg->ch = getChar(vg);
			}

			length1 = vg->offset - vg->temp_offset - vg->increment;
			/*System.out.println(
			""
			+ (char) XMLDoc[vg->temp_offset]
			+ " "
			+ (vg->temp_offset)
			+ " "
			+ length1
			+ " PI Target "
			+ vg->depth);*/
			if (vg->encoding < FORMAT_UTF_16BE){
				if (length1 > MAX_TOKEN_LENGTH){
					e.et = parse_exception;
					e.subtype = 0;
					e.msg = "Parse Exception in parse()";
					e.sub_msg="Token Length Error: PI_NAME length too long";
					Throw e;
				}
				writeVTD(vg,
					vg->temp_offset,
					length1,
					TOKEN_PI_NAME,
					vg->depth);
			}
			else{
				if (length1 > (MAX_TOKEN_LENGTH<<1)){
					e.et = parse_exception;
					e.subtype = 0;
					e.msg = "Parse Exception in parse()";
					e.sub_msg="Token Length Error: PI_NAME length too long";
					Throw e;
				}
				writeVTD(vg,
					vg->temp_offset >> 1,
					length1 >> 1,
					TOKEN_PI_NAME,
					vg->depth);
			}
			//length1 = 0;
			vg->temp_offset = vg->offset;
			if (XMLChar_isSpaceChar(vg->ch)) {
				vg->ch = getCharAfterS(vg);

				while (TRUE) {
					if (XMLChar_isValidChar(vg->ch)) {
						if (vg->ch == '?') {
							if (skipChar(vg,'>')) {
								parser_state = STATE_DOC_END;
								break;
							} else{		
								e.et = parse_exception;
								e.subtype = 0;
								e.msg = "Parse Exception in parse()";
								e.sub_msg = "Error in PI: invalid termination sequence";
								Throw e;
							}
						}
							/*throw new ParseException(
							"Error in PI: invalid termination sequence"
							+ formatLineNumber());*/
					} else{		
						e.et = parse_exception;
						e.subtype = 0;
						e.msg = "Parse Exception in parse()";
						e.sub_msg = "Error in PI: Invalid char in PI val";
						Throw e;
					}
					/*throw new ParseException(
					"Error in PI: Invalid char in PI val"
					+ formatLineNumber());*/
					vg->ch = getChar(vg);
				}
				length1 = vg->offset - vg->temp_offset - (vg->increment<<1);
				if (vg->encoding < FORMAT_UTF_16BE){
					if (length1 > MAX_TOKEN_LENGTH){
						e.et = parse_exception;
						e.subtype = 0;
						e.msg = "Parse Exception in parse()";
						e.sub_msg="Token Length Error: PI_VAL length too long";
						Throw e;
					}
					writeVTD(vg,
						vg->temp_offset,
						length1,
						TOKEN_PI_VAL,
						vg->depth);
				}
				else{
					if (length1 > (MAX_TOKEN_LENGTH << 1)){
						e.et = parse_exception;
						e.subtype = 0;
						e.msg = "Parse Exception in parse()";
						e.sub_msg="Token Length Error: PI_VAL length too long";
						Throw e;
					}
					writeVTD(vg,
						vg->temp_offset >> 1,
						length1 >> 1,
						TOKEN_PI_VAL,
						vg->depth);
				}
				//System.out.println(" " + vg->temp_offset + " " + length1 + " PI val " + vg->depth);
			} else {
				if ((vg->ch == '?') && skipChar(vg,'>')) {
					parser_state = STATE_DOC_END;
				} else{		
					e.et = parse_exception;
					e.subtype = 0;
					e.msg = "Parse Exception in parse()";
					e.sub_msg = "Error in PI: invalid termination sequence";
					Throw e;
				}
				/*	throw new ParseException(
				"Error in PI: invalid termination sequence"
				+ formatLineNumber());*/
			}
			//parser_state = STATE_DOC_END;
	} else{		
		e.et = parse_exception;
		e.subtype = 0;
		e.msg = "Parse Exception in parse()";
		e.sub_msg = "Error in PI: invalid char in PI target";
		Throw e;
	}
	return parser_state;
}
int process_end_comment(VTDGen *vg){
	exception e;		
	int length1,parser_state;
	while (TRUE) {
		vg->ch = getChar(vg);
		if (XMLChar_isValidChar(vg->ch)) {
			if (vg->ch == '-' && skipChar(vg,'-')) {
				length1 =
					vg->offset - vg->temp_offset - (vg->increment<<1);
				break;
			}
		} else{		
			e.et = parse_exception;
			e.subtype = 0;
			e.msg = "Parse Exception in parse()";
			e.sub_msg = "Error in comment: Invalid Char";
			Throw e;
		}
		/*throw new ParseException(
		"Error in comment: Invalid Char"
		+ formatLineNumber());*/
	}
	if (getChar(vg) == '>') {
		//System.out.println(" " + vg->temp_offset + " " + length1 + " comment " + vg->depth);
		if (vg->encoding < FORMAT_UTF_16BE){
			writeVTD(vg,
				vg->temp_offset,
				length1,
				TOKEN_COMMENT,
				vg->depth);
		}
		else
		{
			writeVTD(vg,
				vg->temp_offset >> 1,
				length1 >> 1,
				TOKEN_COMMENT,
				vg->depth);
		}
		//length1 = 0;
		parser_state = STATE_DOC_END;
		return parser_state;
	}		
	e.et = parse_exception;
	e.subtype = 0;
	e.msg = "Parse Exception in parse()";
	e.sub_msg = "Error in comment: '-->' expected";
	Throw e;

}
int process_comment(VTDGen *vg){
	exception e;
	int length1,parser_state;
	while (TRUE) {
		vg->ch = getChar2(vg);
		if (XMLChar_isValidChar(vg->ch)) {
			if (vg->ch == '-' && skipChar(vg,'-')) {
				length1 =
					vg->offset - vg->temp_offset - (vg->increment<<1);
				break;
			}
		} else{		
			e.et = parse_exception;
			e.subtype = 0;
			e.msg = "Parse Exception in parse()";
			e.sub_msg = "Error in comment: Invalid Char";
			Throw e;
		}
		/*throw new ParseException(
		"Error in comment: Invalid Char"
		+ formatLineNumber());*/
	}
	if (getChar(vg) == '>') {
		//System.out.println(" " + (vg->temp_offset) + " " + length1 + " comment " + vg->depth);
		if (vg->encoding < FORMAT_UTF_16BE){
			writeVTD(vg,
				vg->temp_offset,
				length1,
				TOKEN_COMMENT,
				vg->depth);
		}
		else{
			writeVTD(vg,
				vg->temp_offset >> 1,
				length1 >> 1,
				TOKEN_COMMENT,
				vg->depth);
		}
		//length1 = 0;
		vg->temp_offset = vg->offset;
		vg->ch = getCharAfterSe(vg);
		if (vg->ch == '<') {
			parser_state = STATE_LT_SEEN;
		} else if (XMLChar_isContentChar(vg->ch)) {
			//vg->temp_offset = offset;
			parser_state = STATE_TEXT;
		} else if (vg->ch == '&') {
			//has_amp = true;
			//vg->temp_offset = offset;
			entityIdentifier(vg);
			parser_state = STATE_TEXT;
		} else if (vg->ch == ']') {
			if (skipChar(vg,']')) {
				while (skipChar(vg,']')) {
				}
				if (skipChar(vg,'>')){		
					e.et = parse_exception;
					e.subtype = 0;
					e.msg = "Parse Exception in parse()";
					e.sub_msg = "Error in text content: ]]> in text content";
					Throw e;
				}
				/*throw new ParseException(
				"Error in text content: ]]> in text content"
				+ formatLineNumber());*/
			}
			parser_state = STATE_TEXT;
		}
		else{		
			e.et = parse_exception;
			e.subtype = 0;
			e.msg = "Parse Exception in parse()";
			e.sub_msg = "XML decl error";
			Throw e;
		}
		/*throw new ParseException(
		"Error in text content: Invalid char"
		+ formatLineNumber());*/
		return parser_state;
	} else{		
		e.et = parse_exception;
		e.subtype = 0;
		e.msg = "Parse Exception in parse()";
		e.sub_msg = "Error in comment: Invalid terminating sequence";
		Throw e;
	}
}
int process_doc_type(VTDGen *vg){
	exception e;
	int length1,parser_state;
	int	z = 1;

	while (TRUE) {
		vg->ch = getChar(vg);
		if (XMLChar_isValidChar(vg->ch)) {
			if (vg->ch == '>')
				z--;
			else if (vg->ch == '<')
				z++;
			if (z == 0)
				break;
		} else{		
			e.et = parse_exception;
			e.subtype = 0;
			e.msg = "Parse Exception in parse()";
			e.sub_msg = "Error in DOCTYPE: Invalid char";
			Throw e;
		}
		/*throw new ParseException(
		"Error in DOCTYPE: Invalid char"
		+ formatLineNumber());*/
	}
	length1 = vg->offset - vg->temp_offset - vg->increment;
	/*System.out.println(
	" " + (vg->temp_offset) + " " + length1 + " DOCTYPE val " + vg->depth);*/
	if (vg->encoding < FORMAT_UTF_16BE){
		if (length1 > MAX_TOKEN_LENGTH){
			e.et = parse_exception;
			e.subtype = 0;
			e.msg = "Parse Exception in parse()";
			e.sub_msg="Token Length Error: DTD_VAL length too long";
			Throw e;
		}
		writeVTD(vg,
			vg->temp_offset,
			length1,
			TOKEN_DTD_VAL,
			vg->depth);
	}
	else{
		if (length1 > (MAX_TOKEN_LENGTH<<1)){
			e.et = parse_exception;
			e.subtype = 0;
			e.msg = "Parse Exception in parse()";
			e.sub_msg="Token Length Error: DTD_VAL length too long";
			Throw e;
		}
		writeVTD(vg,
			vg->temp_offset >> 1,
			length1 >> 1,
			TOKEN_DTD_VAL,
			vg->depth);
	}
	vg->ch = getCharAfterS(vg);
	if (vg->ch == '<') {
		parser_state = STATE_LT_SEEN;
	} else{		
		e.et = parse_exception;
		e.subtype = 0;
		e.msg = "Parse Exception in parse()";
		e.sub_msg = "Other Error: Invalid char in xml";
		Throw e;
	}
	/*throw new ParseException(
	"Other Error: Invalid char in xml"
	+ formatLineNumber());*/
	return parser_state;
}

int process_cdata(VTDGen *vg){
	exception e;
	int length1,parser_state;
	while (TRUE) {
		vg->ch = getChar2(vg);
		if (XMLChar_isValidChar(vg->ch)) {
			if (vg->ch == ']' && skipChar(vg,']')) {
				while (skipChar(vg,']'));
				if (skipChar(vg,'>')) {
					break;
				} else{		
					e.et = parse_exception;
					e.subtype = 0;
					e.msg = "Parse Exception in parse()";
					e.sub_msg = "Error in CDATA: Invalid terminating sequence";
					Throw e;
				}
				/*throw new ParseException(
				"Error in CDATA: Invalid termination sequence"
				+ formatLineNumber());*/
			}
		} else{		
			e.et = parse_exception;
			e.subtype = 0;
			e.msg = "Parse Exception in parse()";
			e.sub_msg = "Error in CDATA: Invalid Char";
			Throw e;
		}
		/*throw new ParseException(
		"Error in CDATA: Invalid Char"
		+ formatLineNumber());*/
	}
	length1 = vg->offset - vg->temp_offset - vg->increment - (vg->increment<<1);
	if (vg->encoding < FORMAT_UTF_16BE){
		writeVTD(vg,
			vg->temp_offset,
			length1,
			TOKEN_CDATA_VAL,
			vg->depth);
	}
	else{
		writeVTD(vg,
			vg->temp_offset >> 1,
			length1 >> 1,
			TOKEN_CDATA_VAL,
			vg->depth);
	}
	//System.out.println(" " + (vg->temp_offset) + " " + length1 + " CDATA " + vg->depth);
	vg->ch = getCharAfterSe(vg);
	if (vg->ch == '<') {
		parser_state = STATE_LT_SEEN;
	} else if (XMLChar_isContentChar(vg->ch)) {
		vg->temp_offset = vg->offset;
		parser_state = STATE_TEXT;
	} else if (vg->ch == '&') {
		//has_amp = true;
		vg->temp_offset = vg->offset;
		entityIdentifier(vg);
		parser_state = STATE_TEXT;
		//vg->temp_offset = offset;
	} else if (vg->ch == ']') {
		if (skipChar(vg,']')) {
			while (skipChar(vg,']')) {
			}
			if (skipChar(vg,'>')){		
				e.et = parse_exception;
				e.subtype = 0;
				e.msg = "Parse Exception in parse()";
				e.sub_msg = "Error in text content: ]]> in text content";
				Throw e;
			}
			/*throw new ParseException(
			"Error in text content: ]]> in text content"
			+ formatLineNumber());*/
		}
		parser_state = STATE_TEXT;
	}
	else{		
		e.et = parse_exception;
		e.subtype = 0;
		e.msg = "Parse Exception in parse()";
		e.sub_msg = "Other Error: Invalid char in xml";
		Throw e;
	}
	/*throw new ParseException(
	"Other Error: Invalid char in xml"
	+ formatLineNumber());*/
	return parser_state;
}
int process_pi_val(VTDGen *vg){
	//vg->temp_offset = offset;
	exception e;
	int length1,parser_state;
	while (TRUE) {
		if (XMLChar_isValidChar(vg->ch)) {
			//System.out.println(""+(char)ch);
			if (vg->ch == '?'){
				if (skipChar(vg,'>')) {
					break;
				} else{		
					e.et = parse_exception;
					e.subtype = 0;
					e.msg = "Parse Exception in parse()";
					e.sub_msg = "Error in PI: invalid termination sequence for PI";
					Throw e;
				}
			}
				/*	throw new ParseException(
				"Error in PI: invalid termination sequence for PI"
				+ formatLineNumber());*/
		} else{		
			e.et = parse_exception;
			e.subtype = 0;
			e.msg = "Parse Exception in parse()";
			e.sub_msg = "Errors in PI: Invalid char in PI val";
			Throw e;
		}
		/*throw new ParseException(
		"Errors in PI: Invalid char in PI val"
		+ formatLineNumber());*/
		vg->ch = getChar2(vg);
	}
	length1 = vg->offset - vg->temp_offset - (vg->increment<<1);
	/*System.out.println(
	((char) XMLDoc[vg->temp_offset])
	+ " "
	+ (vg->temp_offset)
	+ " "
	+ length1
	+ " PI val "
	+ vg->depth);*/
	if (vg->encoding < FORMAT_UTF_16BE){
		if (length1 > MAX_TOKEN_LENGTH){
			e.et = parse_exception;
			e.subtype = 0;
			e.msg = "Parse Exception in parse()";
			e.sub_msg="Token Length Error: PI_VAL length too long";
			Throw e;
		}
		writeVTD(vg,vg->temp_offset, length1, TOKEN_PI_VAL, vg->depth);
	}
	else{
		if (length1 > (MAX_TOKEN_LENGTH << 1)){
			e.et = parse_exception;
			e.subtype = 0;
			e.msg = "Parse Exception in parse()";
			e.sub_msg="Token Length Error: PI_VAL length too long";
			Throw e;
		}
		writeVTD(vg,
			vg->temp_offset >> 1,
			length1 >> 1,
			TOKEN_PI_VAL,
			vg->depth);
	}
	//length1 = 0;
	vg->temp_offset = vg->offset;
	vg->ch = getCharAfterSe(vg);
	if (vg->ch == '<') {
		parser_state = STATE_LT_SEEN;
	} else if (XMLChar_isContentChar(vg->ch)) {
		//vg->temp_offset = offset;
		parser_state = STATE_TEXT;
	} else if (vg->ch == '&') {
		//has_amp = true;
		//vg->temp_offset = offset;
		entityIdentifier(vg);
		parser_state = STATE_TEXT;
	} else if (vg->ch == ']') {
		if (skipChar(vg,']')) {
			while (skipChar(vg,']')) {
			}
			if (skipChar(vg,'>')){		
				e.et = parse_exception;
				e.subtype = 0;
				e.msg = "Parse Exception in parse()";
				e.sub_msg = "Error in text content: ]]> in text content";
				Throw e;
			}
			/*throw new ParseException(
			"Error in text content: ]]> in text content"
			+ formatLineNumber());*/
		}
		parser_state = STATE_TEXT;
	}
	else{		
		e.et = parse_exception;
		e.subtype = 0;
		e.msg = "Parse Exception in parse()";
		e.sub_msg = "Error in text content: Invalid char";
		Throw e;
	}
	/*throw new ParseException(
	"Error in text content: Invalid char"
	+ formatLineNumber());*/

	return parser_state;
}
int process_pi_tag(VTDGen *vg){
	exception e;
	int length1,parser_state;
	while (TRUE) {
		vg->ch = getChar2(vg);
		if (!XMLChar_isNameChar(vg->ch))
			break;
	}

	length1 = vg->offset - vg->temp_offset - vg->increment;
	/*System.out.println(
	((char) XMLDoc[vg->temp_offset])
	+ " "
	+ (vg->temp_offset)
	+ " "
	+ length1
	+ " PI Target "
	+ vg->depth); */
	if (vg->encoding < FORMAT_UTF_16BE){
		if (length1 > MAX_TOKEN_LENGTH){
			e.et = parse_exception;
			e.subtype = 0;
			e.msg = "Parse Exception in parse()";
			e.sub_msg="Token Length Error: PI_TAG length too long";
			Throw e;
		}
		writeVTD(vg,
			(vg->temp_offset),
			length1,
			TOKEN_PI_NAME,
			vg->depth);
	}
	else{													
		if (length1 > (MAX_TOKEN_LENGTH << 1)){
			e.et = parse_exception;
			e.subtype = 0;
			e.msg = "Parse Exception in parse()";
			e.sub_msg="Token Length Error: PI_TAG length too long";
			Throw e;
		}
		writeVTD(vg,
			(vg->temp_offset) >> 1,
			(length1 >> 1),
			TOKEN_PI_NAME,
			vg->depth);
	}
	//length1 = 0;
	vg->temp_offset = vg->offset;
	if (XMLChar_isSpaceChar(vg->ch)) {
		vg->ch = getChar(vg);
	}
	if (vg->ch == '?') {
		if (skipChar(vg,'>')) {
			vg->temp_offset = vg->offset;
			vg->ch = getCharAfterSe(vg);
			if (vg->ch == '<') {
				parser_state = STATE_LT_SEEN;
			} else if (XMLChar_isContentChar(vg->ch)) {
				parser_state = STATE_TEXT;
			} else if (vg->ch == '&') {
				//has_amp = true;
				entityIdentifier(vg);
				parser_state = STATE_TEXT;
			} else if (vg->ch == ']') {
				if (skipChar(vg,']')) {
					while (skipChar(vg,']')) {
					}
					if (skipChar(vg,'>')){		
						e.et = parse_exception;
						e.subtype = 0;
						e.msg = "Parse Exception in parse()";
						e.sub_msg = "Error in text content: ]]> in text content";
						Throw e;
					}
					/*throw new ParseException(
					"Error in text content: ]]> in text content"
					+ formatLineNumber());*/
				}
				parser_state = STATE_TEXT;
			}else
			{		
				e.et = parse_exception;
				e.subtype = 0;
				e.msg = "Parse Exception in parse()";
				e.sub_msg = "Error in text content: Invalid char";
				Throw e;
			}
			/*throw new ParseException(
			"Error in text content: Invalid char"
			+ formatLineNumber());*/
			return parser_state;
		} else
		{		
			e.et = parse_exception;
			e.subtype = 0;
			e.msg = "Parse Exception in parse()";
			e.sub_msg = "Error in PI: invalid termination sequence";
			Throw e;
		}
		/*throw new ParseException(
		"Error in PI: invalid termination sequence"
		+ formatLineNumber());*/
	}
	parser_state = STATE_PI_VAL;
	return parser_state;
}
int process_dec_attr(VTDGen *vg){
	exception e;
	int parser_state;
	if (vg->ch == 'v'
		&& skipChar(vg,'e')
		&& skipChar(vg,'r')
		&& skipChar(vg,'s')
		&& skipChar(vg,'i')
		&& skipChar(vg,'o')
		&& skipChar(vg,'n')) {
			vg->ch = getCharAfterS(vg);
			if (vg->ch == '=') {

				/*System.out.println(
				" " + (vg->temp_offset - 1) + " " + 7 + " dec attr name version " + vg->depth);*/
				if (vg->encoding < FORMAT_UTF_16BE){

					writeVTD(vg,
						vg->temp_offset - 1,
						7,
						TOKEN_DEC_ATTR_NAME,
						vg->depth);
				}
				else{

					writeVTD(vg,
						(vg->temp_offset - 2) >> 1,
						7,
						TOKEN_DEC_ATTR_NAME,
						vg->depth);
				}
			} 
			else
			{		
				e.et = parse_exception;
				e.subtype = 0;
				e.msg = "Parse Exception in parse()";
				e.sub_msg = "XML decl error: Invalid char";
				Throw e;
			}
			/*throw new ParseException(
			"XML decl error: Invalid char"
			+ formatLineNumber());*/
		} else{		
			e.et = parse_exception;
			e.subtype = 0;
			e.msg = "Parse Exception in parse()";
			e.sub_msg = "XML decl error: should be version";
			Throw e;
		}
		/*throw new ParseException(
		"XML decl error: should be version"
		+ formatLineNumber());*/
		vg->ch_temp = getCharAfterS(vg);
		if (vg->ch_temp != '\'' && vg->ch_temp != '"')
		{		
			e.et = parse_exception;
			e.subtype = 0;
			e.msg = "Parse Exception in parse()";
			e.sub_msg = "XML decl error: Invalid char to start attr name";
			Throw e;
		}
		/*	throw new ParseException(
		"XML decl error: Invalid char to start attr name"
		+ formatLineNumber());*/
		vg->temp_offset = vg->offset;
		// support 1.0 or 1.1
		if (skipChar(vg,'1')
			&& skipChar(vg,'.')
			&& (skipChar(vg,'0') || skipChar(vg,'1'))) {
				/*System.out.println(
				" " + vg->temp_offset + " " + 3 + " dec attr val (version)" + vg->depth);*/
				if (vg->encoding < FORMAT_UTF_16BE){

					writeVTD(vg,
						vg->temp_offset,
						3,
						TOKEN_DEC_ATTR_VAL,
						vg->depth);
				}
				else{

					writeVTD(vg,
						vg->temp_offset >> 1,
						3,
						TOKEN_DEC_ATTR_VAL,
						vg->depth);
				}
			} else
		{		
			e.et = parse_exception;
			e.subtype = 0;
			e.msg = "Parse Exception in parse()";
			e.sub_msg = "XML decl error: Invalid version(other than 1.0 or 1.1) detected";
			Throw e;
		}
		/*	throw new ParseException(
		"XML decl error: Invalid version(other than 1.0 or 1.1) detected"
		+ formatLineNumber());*/
		if (!skipChar(vg,vg->ch_temp))
		{		
			e.et = parse_exception;
			e.subtype = 0;
			e.msg = "Parse Exception in parse()";
			e.sub_msg = "XML decl error: version not terminated properly";
			Throw e;
		}
		/*throw new ParseException(
		"XML decl error: version not terminated properly"
		+ formatLineNumber());*/
		vg->ch = getChar(vg);
		//? space or e 
		if (XMLChar_isSpaceChar(vg->ch)) {
			vg->ch = getCharAfterS(vg);
			vg->temp_offset = vg->offset - vg->increment;
			if (vg->ch == 'e') {
				if (skipChar(vg,'n')
					&& skipChar(vg,'c')
					&& skipChar(vg,'o')
					&& skipChar(vg,'d')
					&& skipChar(vg,'i')
					&& skipChar(vg,'n')
					&& skipChar(vg,'g')) {
						vg->ch = getChar(vg);
						if (XMLChar_isSpaceChar(vg->ch))
							vg->ch = getCharAfterS(vg);
						if (vg->ch == '=') {
							/*System.out.println(
							" " + (vg->temp_offset) + " " + 8 + " dec attr name (encoding) " + vg->depth);*/
							if (vg->encoding < FORMAT_UTF_16BE){

								writeVTD(vg,
									vg->temp_offset,
									8,
									TOKEN_DEC_ATTR_NAME,
									vg->depth);
							}
							else{

								writeVTD(vg,
									vg->temp_offset >> 1,
									8,
									TOKEN_DEC_ATTR_NAME,
									vg->depth);
							}
						} else{		
							e.et = parse_exception;
							e.subtype = 0;
							e.msg = "Parse Exception in parse()";
							e.sub_msg = "XML decl error: Invalid char";
							Throw e;
						}
						/*	throw new ParseException(
						"XML decl error: Invalid char"
						+ formatLineNumber());*/
						vg->ch_temp = getCharAfterS(vg);
						if (vg->ch_temp != '"' && vg->ch_temp != '\'')
						{		
							e.et = parse_exception;
							e.subtype = 0;
							e.msg = "Parse Exception in parse()";
							e.sub_msg = "XML decl error: Invalid char to start attr name";
							Throw e;
						}
						/*	throw new ParseException(
						"XML decl error: Invalid char to start attr name"
						+ formatLineNumber());*/
						vg->temp_offset = vg->offset;
						vg->ch = getChar(vg);
						switch (vg->ch) {
					case 'a' :
					case 'A' :{
						if ((skipChar(vg,'s')
							|| skipChar(vg,'S'))
							&& (skipChar(vg,'c')
							|| skipChar(vg,'C'))
							&& (skipChar(vg,'i')
							|| skipChar(vg,'I'))
							&& (skipChar(vg,'i')
							|| skipChar(vg,'I'))
							&& skipChar(vg,vg->ch_temp)) {
								if (vg->encoding != FORMAT_UTF_16LE
									&& vg->encoding
									!= FORMAT_UTF_16BE) {
										if (vg->must_utf_8){
											e.et = parse_exception;
											e.subtype = 0;
											e.msg = "Parse Exception in parse()";
											e.sub_msg = "Can't switch from UTF-8";
											Throw e;
										}
										vg->encoding = FORMAT_ASCII;
										/*System.out.println(
										" " + (vg->temp_offset) + " " + 5 + " dec attr val (encoding) " + vg->depth);*/
										if (vg->encoding
											< FORMAT_UTF_16BE){
												writeVTD(vg,
													vg->temp_offset,
													5,
													TOKEN_DEC_ATTR_VAL,
													vg->depth);
											}
										else{
											writeVTD(vg,
												vg->temp_offset >> 1,
												5,
												TOKEN_DEC_ATTR_VAL,
												vg->depth);
										}
										break;
									} else
								{		
									e.et = parse_exception;
									e.subtype = 0;
									e.msg = "Parse Exception in parse()";
									e.sub_msg = "XML decl error: Can't switch encoding to ASCII";
									Throw e;
								}
								/*throw new ParseException(
								"XML decl error: Can't switch encoding to ASCII"
								+ formatLineNumber());*/
							}

							e.et = parse_exception;
							e.subtype = 0;
							e.msg = "Parse Exception in parse()";
							e.sub_msg = "XML decl error: Invalid Encoding";
							Throw e;
							  }
							  //throw new ParseException(
							  //	"XML decl error: Invalid Encoding"
							  //		+ formatLineNumber());
					case 'i' :
					case 'I' :{
						if ((skipChar(vg,'s')
							|| skipChar(vg,'S'))
							&& (skipChar(vg,'o')
							|| skipChar(vg,'O'))
							&& skipChar(vg,'-')
							&& skipChar(vg,'8')
							&& skipChar(vg,'8')
							&& skipChar(vg,'5')
							&& skipChar(vg,'9')
							&& skipChar(vg,'-')
							&& skipChar(vg,'1')
							&& skipChar(vg,vg->ch_temp)) {
								if (vg->encoding != FORMAT_UTF_16LE
									&& vg->encoding
									!= FORMAT_UTF_16BE) {
										if (vg->must_utf_8){
											e.et = parse_exception;
											e.subtype = 0;
											e.msg = "Parse Exception in parse()";
											e.sub_msg = "Can't switch from UTF-8";
											Throw e;
										}
										vg->encoding = FORMAT_ISO_8859;
										/*System.out.println(
										" " + (vg->temp_offset) + " " + 10 + " dec attr val (encoding) " + vg->depth);*/
										if (vg->encoding
											< FORMAT_UTF_16BE){
												writeVTD(vg,
													vg->temp_offset,
													10,
													TOKEN_DEC_ATTR_VAL,
													vg->depth);
											}
										else{
											writeVTD(vg,
												vg->temp_offset >> 1,
												10,
												TOKEN_DEC_ATTR_VAL,
												vg->depth);
										}
										break;
									} else
								{		
									e.et = parse_exception;
									e.subtype = 0;
									e.msg = "Parse Exception in parse()";
									e.sub_msg = "XML decl error: Can't switch encoding to ISO-8859";
									Throw e;
								}
								/*	throw new ParseException(
								"XML decl error: Can't switch encoding to ISO-8859"
								+ formatLineNumber());*/
							}

							e.et = parse_exception;
							e.subtype = 0;
							e.msg = "Parse Exception in parse()";
							e.sub_msg = "XML decl error: Invalid Encoding";
							Throw e;
							  }
							  /*	throw new ParseException(
							  "XML decl error: Invalid Encoding"
							  + formatLineNumber());*/
					case 'u' :
					case 'U' :{
						if ((skipChar(vg,'s')
							|| skipChar(vg,'S'))){
							if (skipChar(vg,'-')
								&& (skipChar(vg,'a')
								|| skipChar(vg,'A'))
								&& (skipChar(vg,'s')
								|| skipChar(vg,'S'))
								&& (skipChar(vg,'c')
								|| skipChar(vg,'C'))
								&& (skipChar(vg,'i')
								|| skipChar(vg,'I'))
								&& (skipChar(vg,'i')
								|| skipChar(vg,'I'))
								&& skipChar(vg,vg->ch_temp)) {
									if (vg->encoding
										!= FORMAT_UTF_16LE
										&& vg->encoding
										!= FORMAT_UTF_16BE) {
											if (vg->must_utf_8){
												e.et = parse_exception;
												e.subtype = 0;
												e.msg = "Parse Exception in parse()";
												e.sub_msg = "Can't switch from UTF-8";
												Throw e;
											}
											vg->encoding = FORMAT_ASCII;
											//System.out.println(
											//    " " + (vg->temp_offset) + " " + 5 + " dec attr val (encoding) " + vg->depth);
											if (vg->encoding
												< FORMAT_UTF_16BE){
													writeVTD(vg,
														vg->temp_offset,
														5,
														TOKEN_DEC_ATTR_VAL,
														vg->depth);
												}
											else{
												writeVTD(vg,
													vg->temp_offset
													>> 1,
													5,
													TOKEN_DEC_ATTR_VAL,
													vg->depth);
											}
											break;

										} else
									{		
										e.et = parse_exception;
										e.subtype = 0;
										e.msg = "Parse Exception in parse()";
										e.sub_msg = "XML decl error: Can't switch encoding to US-ASCII";
										Throw e;
									}
									/*throw new ParseException(
									"XML decl error: Can't switch encoding to US-ASCII"
									+ formatLineNumber());*/
								} else{		
									e.et = parse_exception;
									e.subtype = 0;
									e.msg = "Parse Exception in parse()";
									e.sub_msg = "XML decl error: Invalid Encoding";
									Throw e;
								}
							}
								/*throw new ParseException(
								"XML decl error: Invalid Encoding"
								+ formatLineNumber());*/

								if ((skipChar(vg,'t')
									|| skipChar(vg,'T'))
									&& (skipChar(vg,'f')
									|| skipChar(vg,'F'))
									&& skipChar(vg,'-')) {
										if (skipChar(vg,'8')
											&& skipChar(vg,vg->ch_temp)) {
												if (vg->encoding
													!= FORMAT_UTF_16LE
													&& vg->encoding
													!= FORMAT_UTF_16BE) {
														//encoding = FORMAT_UTF8;
														/*System.out.println(
														" " + (vg->temp_offset) + " " + 5 + " dec attr val (encoding) " + vg->depth);*/
														if (vg->encoding
															< FORMAT_UTF_16BE)
														{
															writeVTD(vg,
																vg->temp_offset,
																5,
																TOKEN_DEC_ATTR_VAL,
																vg->depth);
														}
														else
														{
															writeVTD(vg,
																vg->temp_offset
																>> 1,
																5,
																TOKEN_DEC_ATTR_VAL,
																vg->depth);
														}
														break;
													} else
												{		
													e.et = parse_exception;
													e.subtype = 0;
													e.msg = "Parse Exception in parse()";
													e.sub_msg = "XML decl error: Can't switch encoding to UTF-8";
													Throw e;
												}
												/*throw new ParseException(
												"XML decl error: Can't switch encoding to UTF-8"
												+ formatLineNumber());*/
											}
											if (skipChar(vg,'1')
												&& skipChar(vg,'6')) {
													if (skipChar(vg,vg->ch_temp)) {
														if (vg->encoding
															== FORMAT_UTF_16LE
															|| vg->encoding
															== FORMAT_UTF_16BE) {
																/*System.out.println(
																" " + (vg->temp_offset) + " " + 6 + " dec attr val (encoding) " + vg->depth);*/
																if (vg->BOM_detected == FALSE){
																	e.et = parse_exception;
																	e.subtype = 0;
																	e.msg = "Parse Exception in parse()";
																	e.sub_msg = "BOM not detected for UTF-16";
																	Throw e;
																}
																if (vg->encoding
																	< FORMAT_UTF_16BE)
																{
																	writeVTD(vg,
																		vg->temp_offset,
																		6,
																		TOKEN_DEC_ATTR_VAL,
																		vg->depth);
																}
																else{
																	writeVTD(vg,
																		vg->temp_offset
																		>> 1,
																		6,
																		TOKEN_DEC_ATTR_VAL,
																		vg->depth);
																}
																break;
															}

															e.et = parse_exception;
															e.subtype = 0;
															e.msg = "Parse Exception in parse()";
															e.sub_msg = "XML decl error: Can't switch encoding to UTF-16";
															Throw e;

															/*throw new ParseException(
															"XML decl error: Can't switch encoding to UTF-16"
															+ formatLineNumber());*/
													} else if (
														(skipChar(vg,'l')
														|| skipChar(vg,'L'))
														&& (skipChar(vg,'e')
														|| skipChar(vg,'E'))
														&& skipChar(vg,vg->ch_temp)) {
															if (vg->encoding
																== FORMAT_UTF_16LE) {
																	/*System.out.println(
																	" " + (vg->temp_offset) + " " + 7 + " dec attr val (encoding) " + vg->depth);*/
																	if (vg->encoding
																		< FORMAT_UTF_16BE)
																	{
																		writeVTD(vg,
																			vg->temp_offset,
																			7,
																			TOKEN_DEC_ATTR_VAL,
																			vg->depth);
																	}
																	else
																	{
																		writeVTD(vg,
																			vg->temp_offset
																			>> 1,
																			7,
																			TOKEN_DEC_ATTR_VAL,
																			vg->depth);
																	}
																	break;
																}
																e.et = parse_exception;
																e.subtype = 0;
																e.msg = "Parse Exception in parse()";
																e.sub_msg = "XML decl error: Can't switch encoding to UTF-16LE";
																Throw e;
																/*throw new ParseException(
																"XML del error: Can't switch encoding to UTF-16LE"
																+ formatLineNumber());*/
														} else if (
															(skipChar(vg,'b')
															|| skipChar(vg,'B'))
															&& (skipChar(vg,'e')
															|| skipChar(vg,'E'))
															&& skipChar(vg,vg->ch_temp)) {
																if (vg->encoding
																	== FORMAT_UTF_16BE) {
																		/*System.out.println(
																		" " + (vg->temp_offset) + " " + 7 + " dec attr val (encoding) " + vg->depth);*/
																		if (vg->encoding
																			< FORMAT_UTF_16BE)
																		{
																			writeVTD(vg,
																				vg->temp_offset,
																				7,
																				TOKEN_DEC_ATTR_VAL,
																				vg->depth);
																		}
																		else{
																			writeVTD(vg,
																				vg->temp_offset
																				>> 1,
																				7,
																				TOKEN_DEC_ATTR_VAL,
																				vg->depth);
																		}
																		break;
																	}
																	e.et = parse_exception;
																	e.subtype = 0;
																	e.msg = "Parse Exception in parse()";
																	e.sub_msg = "XML decl error: Can't switch encoding to UTF-16BE";
																	Throw e;
																	/*throw new ParseException(
																	"XML del error: Can't swtich encoding to UTF-16BE"
																	+ formatLineNumber());*/
															}
															e.et = parse_exception;
															e.subtype = 0;
															e.msg = "Parse Exception in parse()";
															e.sub_msg = "XML decl error: Invalid encoding";
															Throw e;
															/*	throw new ParseException(
															"XML decl error: Invalid encoding"
															+ formatLineNumber());*/
												}
									}}
					default :
						e.et = parse_exception;
						e.subtype = 0;
						e.msg = "Parse Exception in parse()";
						e.sub_msg = "XML decl error: Invalid encoding";
						Throw e;
						break;
						/*throw new ParseException(
						"XML decl Error: invalid encoding"
						+ formatLineNumber());*/
						}
						vg->ch = getChar(vg);
						if (XMLChar_isSpaceChar(vg->ch))
							vg->ch = getCharAfterS(vg);
						vg->temp_offset = vg->offset - vg->increment;
					} else{		
						e.et = parse_exception;
						e.subtype = 0;
						e.msg = "Parse Exception in parse()";
						e.sub_msg = "XML decl Error: Invalid char";
						Throw e;
					}
					/*throw new ParseException(
					"XML decl Error: Invalid char"
					+ formatLineNumber());*/
			}

			if (vg->ch == 's') {
				if (skipChar(vg,'t')
					&& skipChar(vg,'a')
					&& skipChar(vg,'n')
					&& skipChar(vg,'d')
					&& skipChar(vg,'a')
					&& skipChar(vg,'l')
					&& skipChar(vg,'o')
					&& skipChar(vg,'n')
					&& skipChar(vg,'e')) {

						vg->ch = getCharAfterS(vg);
						if (vg->ch != '='){		
							e.et = parse_exception;
							e.subtype = 0;
							e.msg = "Parse Exception in parse()";
							e.sub_msg = "XML decl error: Invalid char";
							Throw e;
						}
						/*throw new ParseException(
						"XML decl error: Invalid char"
						+ formatLineNumber());*/
						/*System.out.println(
						" " + vg->temp_offset + " " + 3 + " dec attr name (standalone) " + vg->depth);*/
						if (vg->encoding < FORMAT_UTF_16BE){
							writeVTD(vg,
								vg->temp_offset,
								10,
								TOKEN_DEC_ATTR_NAME,
								vg->depth);
						}
						else{
							writeVTD(vg,
								vg->temp_offset >> 1,
								10,
								TOKEN_DEC_ATTR_NAME,
								vg->depth);
						}
						vg->ch_temp = getCharAfterS(vg);
						vg->temp_offset = vg->offset;
						if (vg->ch_temp != '"' && vg->ch_temp != '\''){		
							e.et = parse_exception;
							e.subtype = 0;
							e.msg = "Parse Exception in parse()";
							e.sub_msg = "XML decl error: Invalid char to start attr name";
							Throw e;
						}
						/*	throw new ParseException(
						"XML decl error: Invalid char to start attr name"
						+ formatLineNumber());*/
						vg->ch = getChar(vg);
						if (vg->ch == 'y') {
							if (skipChar(vg,'e')
								&& skipChar(vg,'s')
								&& skipChar(vg,vg->ch_temp)) {
								/*System.out.println(
								    " " + (vg->temp_offset) + " " + 3 + " dec attr val (standalone) " + vg->depth);*/
								if (vg->encoding < FORMAT_UTF_16BE)
									writeVTD(vg,
										vg->temp_offset,
										3,
										TOKEN_DEC_ATTR_VAL,
										vg->depth);
								else
									writeVTD(vg,
										vg->temp_offset >> 1,
										3,
										TOKEN_DEC_ATTR_VAL,
										vg->depth);
								} else{		
									e.et = parse_exception;
									e.msg = "Parse Exception in parse()";
									e.sub_msg = "XML decl error: invalid val for standalone";
									Throw e;
								}
								/*throw new ParseException(
									"XML decl error: invalid val for standalone"
									+ formatLineNumber());*/
						} else if (vg->ch == 'n') {
							if (skipChar(vg,'o')
								&& skipChar(vg,vg->ch_temp)) {
								/*System.out.println(
								    " " + (vg->temp_offset) + " " + 2 + " dec attr val (standalone)" + vg->depth);*/
								if (vg->encoding < FORMAT_UTF_16BE)
									writeVTD(vg,
										vg->temp_offset,
										2,
										TOKEN_DEC_ATTR_VAL,
										vg->depth);
								else
									writeVTD(vg,
										vg->temp_offset >> 1,
										2,
										TOKEN_DEC_ATTR_VAL,
										vg->depth);
								} else{		
									e.et = parse_exception;
									e.msg = "Parse Exception in parse()";
									e.sub_msg = "XML decl error: invalid val for standalone";
									Throw e;
								}
								/*throw new ParseException(
									"XML decl error: invalid val for standalone"
									+ formatLineNumber());*/
						} else{		
							e.et = parse_exception;
							e.msg = "Parse Exception in parse()";
							e.sub_msg = "XML decl error: invalid val for standalone";
							Throw e;
						}
						/*throw new ParseException(
						"XML decl error: invalid val for standalone"
						+ formatLineNumber());*/
					} else{		
						e.et = parse_exception;
						e.subtype = 0;
						e.msg = "Parse Exception in parse()";
						e.sub_msg = "XML decl error";
						Throw e;
					}
					/*throw new ParseException(
					"XML decl error" + formatLineNumber());*/
					vg->ch = getChar(vg);
					if (XMLChar_isSpaceChar(vg->ch))
						vg->ch = getCharAfterS(vg);
			}
		}

		if (vg->ch == '?' && skipChar(vg,'>')) {
			vg->temp_offset = vg->offset;
			vg->ch = getCharAfterS(vg);
			if (vg->ch == '<') {
				parser_state = STATE_LT_SEEN;
			} else{		
				e.et = parse_exception;
				e.subtype = 0;
				e.msg = "Parse Exception in parse()";
				e.sub_msg = "Other Error: Invalid Char in XML";
				Throw e;
			}
			/*throw new ParseException(
			"Other Error: Invalid Char in XML"
			+ formatLineNumber());*/
		} else{		
			e.et = parse_exception;
			e.subtype = 0;
			e.msg = "Parse Exception in parse()";
			e.sub_msg = "XML decl Error: Invalid termination sequence";
			Throw e;
		}
		/*throw new ParseException(
		"XML decl Error: Invalid termination sequence"
		+ formatLineNumber());*/
		return parser_state;

}
