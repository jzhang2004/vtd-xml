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
static int getCharAfterSe(VTDGen *vg);
static int getCharAfterS(VTDGen *vg);
static int getCharAfterS2(VTDGen *vg, Boolean entityOK);
static Boolean skipChar(VTDGen *vg, int ch);
static void writeVTD(VTDGen *vg, int offset, int length, tokenType token_type, int depth);

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
	vg->offset = 0;
	vg->rootIndex = 0;
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
							e.msg = "Parse exception in entityIdentifier";
							e.sub_msg = "Errors in Entity: Illegal builtin reference";
							Throw e;
						}
						//throw new EntityException("Errors in Entity: Illegal builtin reference");
				} else{
					e.et = parse_exception;
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
					e.msg = "Parse exception in entityIdentifier";
					e.sub_msg = "Errors in Entity: Illegal builtin reference";
					Throw e;
				}
				//throw new EntityException("Errors in Entity: Illegal builtin reference");
			default :
				e.et = parse_exception;
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
	int end = vg->offset;

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
		e.msg = "Parse exception in getChar";
		e.sub_msg = "Premature EOF reached, XML document incomplete";
		Throw e;			
	}
	//throw new EOFException("permature EOF reached, XML document incomplete");
	switch (vg->encoding) {
			case FORMAT_ASCII :
				temp = vg->XMLDoc[vg->offset];
				if (temp>127){
					e.et = parse_exception;
					e.msg = "Parse exception in getChar";
					e.sub_msg = "Invalid char for ASCII encoding";
					Throw e;
				}
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
							e.msg = "Parse exception in getChar";
							e.sub_msg = "UTF 16 BE encoding error: should never happen";
							Throw e;
						}
						val = temp;
						temp = vg->XMLDoc[vg->offset + 2] << 8 | vg->XMLDoc[vg->offset + 3];
						if (temp < 0xdc00 || temp > 0xdfff) {
							// has to be a low surrogate here
							e.et = parse_exception;
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
						e.msg = "Parse exception in getChar";
						e.sub_msg = "UTF 16 LE encoding error: should never happen";
						Throw e;
					}
					val = temp;
					temp = vg->XMLDoc[vg->offset + 3] << 8 | vg->XMLDoc[vg->offset + 2];
					if (temp < 0xdc00 || temp > 0xdfff) {
						// has to be high surrogate
						e.et = parse_exception;
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
				e.msg = "Parse exception in getChar";
				e.sub_msg = "Unknown encoding";
				Throw e;
				//	throw new EncodingException("Unknown encoding");
	}
}

// The entity aware version of getCharAfterS
static int getCharAfterSe(VTDGen *vg){
	int n = 0;
	int temp; //offset saver
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

// The entity ignorant version of getCharAfterS
static int getCharAfterS(VTDGen *vg){
	int n, k;
	n = k = 0;
	while (TRUE) {
		n = getChar(vg);
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

	int temp_offset = 0, length1 = 0, length2 = 0, increment = 1;
	int depth = -1, attr_count = 0, ch = 0, ch_temp = 0;
	int prev_ch = 0, prev2_ch = 0;
	int i,j,z;
	parseState parser_state = STATE_DOC_START;
	//boolean has_amp = false; 
	Boolean is_ns = FALSE;
	Boolean unique;
	Boolean unequal;
	Boolean BOM_detected = FALSE;
	Boolean must_utf_8 = FALSE;
	Long x;
	Boolean main_loop = TRUE,
		hasDTD = FALSE,
		hasDecl = FALSE,
		docEnd = FALSE,
		firstLT = TRUE;
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
	if (vg->XMLDoc[vg->offset] == -2) {
		increment = 2;
		if (vg->XMLDoc[vg->offset + 1] == -1) {
			vg->offset += 2;
			vg->encoding = FORMAT_UTF_16BE;
			BOM_detected = TRUE;
			//r = new UTF16BEReader();
		} else{
			e.et = parse_exception;
			e.msg = "Parse Exception in parse()";
			e.sub_msg = "Unknown Character encoding: should be 0xff 0xfe";
			Throw e;
		}
		//	throw new EncodingException("Unknown Character encoding: should be 0xff 0xfe");
	} else if (vg->XMLDoc[vg->offset] == -1) {
		increment = 2;
		if (vg->XMLDoc[vg->offset + 1] == -2) {
			vg->offset += 2;
			vg->encoding = FORMAT_UTF_16LE;
			BOM_detected = TRUE;
			//r = new UTF16LEReader();
		} else{
			e.et = parse_exception;
			e.msg = "Parse Exception in parse()";
			e.sub_msg = "Unknown Character encoding";
			Throw e;
		}
		//throw new EncodingException("Unknown Character encoding");
	} else if (vg->XMLDoc[vg->offset] == -17) {
		if (vg->XMLDoc[vg->offset+1] == -69 && vg->XMLDoc[vg->offset+2]==-65){
		      vg->offset +=3;
		      must_utf_8= TRUE;
		    }
		else {
			e.et = parse_exception;
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
				increment = 2;
			}
		else{
			e.et = parse_exception;
			e.msg = "Parse Exception in parse()";
			e.sub_msg = "Unknown Character encoding: not UTF-16BE";
			Throw e;
		}
		//	throw new EncodingException("Unknown Character encoding: not UTF-16BE");
	} else if (vg->XMLDoc[vg->offset] == 0x3c) {
		if (vg->XMLDoc[vg->offset+1] == 0 
			&& vg->XMLDoc[vg->offset+2] == 0x3f 
			&& vg->XMLDoc[vg->offset+3] == 0x0){
				increment = 2;
				vg->encoding = FORMAT_UTF_16LE;
			}
	}

	if (vg->encoding < FORMAT_UTF_16BE) {
		if ((unsigned int)(vg->offset + vg->docLen) >= (((unsigned int)1) << 30)){
			e.et = parse_exception;
			e.msg = "Parse Exception in parse()";
			e.sub_msg = "Other error: file size too large";
			Throw e;
		}
		//throw new ParseException("Other error: file size too large ");
	} else {
		if ((unsigned int)(vg->offset - 2 + vg->docLen) >= (((unsigned int) 1) << 31)){
			e.et = parse_exception;
			e.msg = "Parse Exception in parse()";
			e.sub_msg = "Other error: file size too large";
			Throw e;
		}
		//throw new ParseException("Other error: file size too large ");
	}
	// enter the main finite state machine
	Try {
		while (main_loop) {
			switch (parser_state) {
					case STATE_DOC_START :
						if (getChar(vg) == '<') {
							temp_offset = vg->offset;
							// xml decl has to be right after the start of the document
							if (skipChar(vg,'?')
								&& (skipChar(vg,'x') || skipChar(vg,'X'))
								&& (skipChar(vg,'m') || skipChar(vg,'M'))
								&& (skipChar(vg,'l') || skipChar(vg,'L'))) {
									if (skipChar(vg,' ')
										|| skipChar(vg,'\t')
										|| skipChar(vg,'\n')
										|| skipChar(vg,'\r')) {
											ch = getCharAfterS(vg);
											temp_offset = vg->offset;
											parser_state = STATE_DEC_ATTR_NAME;
											break;
										} else if (skipChar(vg,'?')){
											e.et = parse_exception;
											e.msg = "Parse Exception in parse()";
											e.sub_msg = "Error in XML decl: premature ending";
											Throw e;
										}
										/*	throw new ParseException(
										"Error in XML decl: premature ending"
										+ formatLineNumber());*/
								}
								vg->offset = temp_offset;
								parser_state = STATE_LT_SEEN;
								break;
						}
						e.et = parse_exception;
						e.msg = "Parse Exception in parse()";
						e.sub_msg = "Other Error: XML not starting properly";
						Throw e;

						/*throw new ParseException(
						"Other Error: XML not starting properly"
						+ formatLineNumber()); */
					case STATE_DOC_END :
						docEnd = TRUE;
						ch = getCharAfterS(vg);
						// eof exception should be thrown here for premature ending
						if (ch == '<') {

							if (skipChar(vg,'?')) {
								// processing instruction after end tag of root element
								temp_offset = vg->offset;
								parser_state = STATE_END_PI;
								break;
							} else if (
								skipChar(vg,'!')
								&& skipChar(vg,'-')
								&& skipChar(vg,'-')) {
									// comments allowed after the end tag of the root element
									temp_offset = vg->offset;
									parser_state = STATE_END_COMMENT;
									break;
								}
						}
						e.et = parse_exception;
						e.msg = "Parse Exception in parse()";
						e.sub_msg = "Other Error: XML not terminated properly";
						Throw e;
						/*throw new ParseException(
						"Other Error: XML not terminated properly"
						+ formatLineNumber());*/

					case STATE_LT_SEEN : //if (depth < -1)
						//    throw new ParseException("Other Errors: Invalid depth");
						temp_offset = vg->offset;
						ch = getChar(vg);
						if (XMLChar_isNameStartChar(ch)) {
							//temp_offset = offset;
							//length1++;
							depth++;
							//if (ch == ':')
							//   length2 = 0;
							parser_state = STATE_START_TAG;
						} else {
							switch (ch) {
					case '/' :
						parser_state = STATE_END_TAG;
						break;
					case '?' :
						temp_offset = vg->offset;
						ch = getChar(vg);
						if (XMLChar_isNameStartChar(ch)) {
							//temp_offset = offset;
							if ((ch == 'x' || ch == 'X')
								&& (skipChar(vg,'m') || skipChar(vg,'M'))
								&& (skipChar(vg,'l') || skipChar(vg,'L'))) {
									ch = getChar(vg);
									if (ch == '?'
										|| XMLChar_isSpaceChar(ch)){
											e.et = parse_exception;
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
						e.msg = "Parse Exception in parse()";
						e.sub_msg = "Other Error: First char after <? invalid";
						Throw e;
						/*throw new ParseException(
						"Other Error: First char after <? invalid"
						+ formatLineNumber()); */

					case '!' : // three possibility (comment, CDATA, DOCTYPE)
						ch = getChar(vg);
						switch (ch) {
					case '-' :
						if (skipChar(vg,'-')) {
							temp_offset = vg->offset;
							parser_state = STATE_COMMENT;
							break;
						} else
						{		
							e.et = parse_exception;
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
							&& (depth != -1)) {
								temp_offset = vg->offset;
								parser_state = STATE_CDATA;
								break;
							} else {
								if (depth == -1){
									e.et = parse_exception;
									e.msg = "Parse Exception in parse()";
									e.sub_msg = "Error in CDATA: Invalid char sequence for CDATA";
									Throw e;
								}
								/*throw new ParseException(
								"Error in CDATA: Wrong place for CDATA"
								+ formatLineNumber());*/
								e.et = parse_exception;
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
							&& (depth == -1)
							&& !hasDTD) {
								hasDTD = TRUE;
								temp_offset = vg->offset;
								parser_state = STATE_DOCTYPE;
								break;
							} else {
								if (hasDTD == TRUE){
									e.et = parse_exception;
									e.msg = "Parse Exception in parse()";
									e.sub_msg = "Error for DOCTYPE: Only DOCTYPE allowed";
									Throw e;
								}
								/*	throw new ParseException(
								"Error for DOCTYPE: Only DOCTYPE allowed"
								+ formatLineNumber()); */
								if (depth != -1){
									e.et = parse_exception;
									e.msg = "Parse Exception in parse()";
									e.sub_msg = "Error for DOCTYPE: DTD at wrong place";
									Throw e;
								}
								/*	throw new ParseException(
								"Error for DOCTYPE: DTD at wrong place"
								+ formatLineNumber());*/

								e.et = parse_exception;
								e.msg = "Parse Exception in parse()";
								e.sub_msg = "Error for DOCTYPE: Invalid char sequence for DOCTYPE";
								Throw e;
								/*throw new ParseException(
								"Error for DOCTYPE: Invalid char sequence for DOCTYPE"
								+ formatLineNumber());*/
							}
					default :

						e.et = parse_exception;
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
							ch = getChar(vg);
							if (XMLChar_isNameChar(ch)) {
								if (ch == ':') {
									length2 = vg->offset - temp_offset - increment;
								}
							} else
								break;
						}
						length1 = vg->offset - temp_offset - increment;
						//writeVTD(offset, TOKEN_STARTING_TAG, length2:length1, depth)
						x = ((Long) length1 << 32) + temp_offset;
						vg->tag_stack[depth] = x;
						if (depth > MAX_DEPTH) {
							e.et = parse_exception;
							e.msg = "Parse Exception in parse()";
							e.sub_msg = "Other Error: Depth exceeds MAX_DEPTH";
							Throw e;
							/*throw new ParseException(
							"Other Error: Depth exceeds MAX_DEPTH"
							+ formatLineNumber());*/
						}
						// System.out.println(
						//     " " + (temp_offset) + " " + length2 + ":" + length1 + " startingTag " + depth);
						if (depth > vg->VTDDepth)
							vg->VTDDepth = depth;
						if (vg->encoding < FORMAT_UTF_16BE){
							/*if (length2>MAX_PREFIX_LENGTH
							|| length1 > MAX_QNAME_LENGTH)
							throw new ParseException(
							"Token Length Error: Starting tag prefix or qname length too long"
							+formatLineNumber()); */
							if (length2>MAX_PREFIX_LENGTH 
								|| length1 > MAX_QNAME_LENGTH){
									e.et = parse_exception;
									e.msg = "Parse Exception in parse()";
									e.sub_msg="Token Length Error: Starting tag prefix or qname length too long";
									Throw e;
								}
#if BIG_ENDIAN
								writeVTD(vg,
									(temp_offset),
									(length2 << 11) | length1,
									TOKEN_STARTING_TAG,
									depth);
#else
								writeVTD(vg,
									(temp_offset),
									(length2 << 16) | length1,
									TOKEN_STARTING_TAG,
									depth);
#endif
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
									e.msg = "Parse Exception in parse()";
									e.sub_msg="Token Length Error: Starting tag prefix or qname length too long";
									Throw e;
								}

								writeVTD(vg,
									(temp_offset) >> 1,
									(length2 << 10) | (length1 >> 1),
									TOKEN_STARTING_TAG,
									depth);
						}
						//offset += length1;
						length2 = 0;
						if (XMLChar_isSpaceChar(ch)) {
							ch = getCharAfterS(vg);
							if (XMLChar_isNameStartChar(ch)) {
								// seen an attribute here
								temp_offset = getPrevOffset(vg);
								parser_state = STATE_ATTR_NAME;
								break;
							}
						}
						if (ch == '/') {
							depth--;
							ch = getChar(vg);
						}
						if (ch == '>') {
							if (depth != -1) {
								temp_offset = vg->offset;
								ch = getCharAfterSe(vg); // consume WSs
								if (ch == '<') {
									parser_state = STATE_LT_SEEN;
									if (skipChar(vg,'/')) {
										length1 =
											vg->offset
											- temp_offset
											- 2 * increment;
										if (length1 > 0) {
											if (vg->encoding < FORMAT_UTF_16BE)
												writeVTD(vg,
												(temp_offset),
												length1,
												TOKEN_CHARACTER_DATA,
												depth);
											else
												writeVTD(vg,
												(temp_offset) >> 1,
												(length1 >> 1),
												TOKEN_CHARACTER_DATA,
												depth);
										}
										//offset += length1;
										parser_state = STATE_END_TAG;
										break;
									}
								} else if (XMLChar_isContentChar(ch)) {
									//temp_offset = offset;
									parser_state = STATE_TEXT;
								} else if (ch == '&') {
									//has_amp = true;
									//temp_offset = offset;
									entityIdentifier(vg);
									parser_state = STATE_TEXT;
								} else if (ch == ']') {
								if (skipChar(vg,']')) {
									while (skipChar(vg,']')) {
									}
									if (skipChar(vg,'>')){		
										e.et = parse_exception;
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
						e.msg = "Parse Exception in parse()";
						e.sub_msg = "Starting tag Error: Invalid char in starting tag";
						Throw e;
						/*throw new ParseException(
						"Starting tag Error: Invalid char in starting tag"
						+ formatLineNumber());*/

					case STATE_END_TAG :
						temp_offset = vg->offset;
						while (TRUE) {
							ch = getChar(vg);
							if (!XMLChar_isNameChar(ch)) {
								break;
							}
						}
						length1 = vg->offset - temp_offset - increment;
						sos = (int) vg->tag_stack[depth];
						sl = (int) (vg->tag_stack[depth] >> 32);
						if (sl == length1) {
							int i;
							for (i = 0; i < length1; i++) {
								if (vg->XMLDoc[sos + i] != vg->XMLDoc[temp_offset + i]){
									e.et = parse_exception;
									e.msg = "Parse Exception in parse()";
									e.sub_msg = "Ending tag error: Start/ending tag mismatch";
									Throw e;
								}
								//throw new ParseException(
								//	"Ending tag error: Start/ending tag mismatch"
								//		+ formatLineNumber());
								// System.out.println(
								//     " " + (char) XMLDoc[sos + i] + "<==> " + (char) XMLDoc[temp_offset + i]);
							}
						} else{
							e.et = parse_exception;
							e.msg = "Parse Exception in parse()";
							e.sub_msg = "Ending tag error: Start/ending tag mismatch, length different";
							Throw e;
						}
						/*	throw new ParseException(
						"Ending tag error: Start/ending tag mismatch, length different"
						+ formatLineNumber());*/
						//System.out.println(" " + temp_offset + " " + length1 + " ending tag " + depth);
						//writeVTD(vg,temp_offset, length1, TOKEN_ENDING_TAG, depth);
						depth--;
						if (XMLChar_isSpaceChar(ch)) {
							ch = getCharAfterS(vg);
						}

						if (ch == '>') {
							if (depth != -1) {
								temp_offset = vg->offset;
								ch = getCharAfterS(vg);
								if (ch == '<')
									parser_state = STATE_LT_SEEN;
								else if (XMLChar_isContentChar(ch)) {
									parser_state = STATE_TEXT;
								} else if (ch == '&') {
									//has_amp = true;
									entityIdentifier(vg);
									parser_state = STATE_TEXT;
								} else if (ch == ']') {
								if (skipChar(vg,']')) {
									while (skipChar(vg,']')) {
									}
									if (skipChar(vg,'>')){		
										e.et = parse_exception;
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
								else
								{	e.et = parse_exception;
								e.msg = "Parse Exception in parse()";
								e.sub_msg = "Other Error: Invalid char in xml";
								Throw e;
								}
								/*throw new ParseException(
								"Other Error: Invalid char in xml"
								+ formatLineNumber());*/
							} else
								parser_state = STATE_DOC_END;
							break;
						} else{	e.et = parse_exception;
						e.msg = "Parse Exception in parse()";
						e.sub_msg = "Other Error: Invalid char in ending tag";
						Throw e;

						/*	throw new ParseException(
						"Other Error: Invalid char in ending tag"
						+ formatLineNumber());*/}
					case STATE_UNRECORDED_TEXT :
						break;
					case STATE_PI_TAG :
						while (TRUE) {
							ch = getChar(vg);
							if (!XMLChar_isNameChar(ch))
								break;
						}

						length1 = vg->offset - temp_offset - increment;
						/*System.out.println(
						((char) XMLDoc[temp_offset])
						+ " "
						+ (temp_offset)
						+ " "
						+ length1
						+ " PI Target "
						+ depth); */
						if (vg->encoding < FORMAT_UTF_16BE){
							if (length1 > MAX_TOKEN_LENGTH){
								e.et = parse_exception;
								e.msg = "Parse Exception in parse()";
								e.sub_msg="Token Length Error: PI_TAG length too long";
								Throw e;
							}
							writeVTD(vg,
								(temp_offset),
								length1,
								TOKEN_PI_NAME,
								depth);
						}
						else{													
							if (length1 > (MAX_TOKEN_LENGTH << 1)){
								e.et = parse_exception;
								e.msg = "Parse Exception in parse()";
								e.sub_msg="Token Length Error: PI_TAG length too long";
								Throw e;
							}
							writeVTD(vg,
								(temp_offset) >> 1,
								(length1 >> 1),
								TOKEN_PI_NAME,
								depth);
						}
						//length1 = 0;
						temp_offset = vg->offset;
						if (XMLChar_isSpaceChar(ch)) {
							ch = getChar(vg);
						}
						if (ch == '?') {
							if (skipChar(vg,'>')) {
								temp_offset = vg->offset;
								ch = getCharAfterSe(vg);
								if (ch == '<') {
									parser_state = STATE_LT_SEEN;
								} else if (XMLChar_isContentChar(ch)) {
									parser_state = STATE_TEXT;
								} else if (ch == '&') {
									//has_amp = true;
									entityIdentifier(vg);
									parser_state = STATE_TEXT;
								} else if (ch == ']') {
								if (skipChar(vg,']')) {
									while (skipChar(vg,']')) {
									}
									if (skipChar(vg,'>')){		
										e.et = parse_exception;
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
									e.msg = "Parse Exception in parse()";
									e.sub_msg = "Error in text content: Invalid char";
									Throw e;
								}
								/*throw new ParseException(
								"Error in text content: Invalid char"
								+ formatLineNumber());*/
								break;
							} else
							{		
								e.et = parse_exception;
								e.msg = "Parse Exception in parse()";
								e.sub_msg = "Error in PI: invalid termination sequence";
								Throw e;
							}
							/*throw new ParseException(
							"Error in PI: invalid termination sequence"
							+ formatLineNumber());*/
						}
						parser_state = STATE_PI_VAL;
						break;
						//throw new ParseException("Error in PI: Invalid char");
					case STATE_PI_VAL :
						//temp_offset = offset;
						while (TRUE) {
							if (XMLChar_isValidChar(ch)) {
								//System.out.println(""+(char)ch);
								if (ch == '?')
									if (skipChar(vg,'>')) {
										break;
									} else{		
										e.et = parse_exception;
										e.msg = "Parse Exception in parse()";
										e.sub_msg = "Error in PI: invalid termination sequence for PI";
										Throw e;
									}
									/*	throw new ParseException(
									"Error in PI: invalid termination sequence for PI"
									+ formatLineNumber());*/
							} else{		
								e.et = parse_exception;
								e.msg = "Parse Exception in parse()";
								e.sub_msg = "Errors in PI: Invalid char in PI val";
								Throw e;
							}
							/*throw new ParseException(
							"Errors in PI: Invalid char in PI val"
							+ formatLineNumber());*/
							ch = getChar(vg);
						}
						length1 = vg->offset - temp_offset - 2 * increment;
						/*System.out.println(
						((char) XMLDoc[temp_offset])
						+ " "
						+ (temp_offset)
						+ " "
						+ length1
						+ " PI val "
						+ depth);*/
						if (vg->encoding < FORMAT_UTF_16BE){
							if (length1 > MAX_TOKEN_LENGTH){
								e.et = parse_exception;
								e.msg = "Parse Exception in parse()";
								e.sub_msg="Token Length Error: PI_VAL length too long";
								Throw e;
							}
							writeVTD(vg,temp_offset, length1, TOKEN_PI_VAL, depth);
						}
						else{
							if (length1 > (MAX_TOKEN_LENGTH << 1)){
								e.et = parse_exception;
								e.msg = "Parse Exception in parse()";
								e.sub_msg="Token Length Error: PI_VAL length too long";
								Throw e;
							}
							writeVTD(vg,
								temp_offset >> 1,
								length1 >> 1,
								TOKEN_PI_VAL,
								depth);
						}
						//length1 = 0;
						temp_offset = vg->offset;
						ch = getCharAfterSe(vg);
						if (ch == '<') {
							parser_state = STATE_LT_SEEN;
						} else if (XMLChar_isContentChar(ch)) {
							//temp_offset = offset;
							parser_state = STATE_TEXT;
						} else if (ch == '&') {
							//has_amp = true;
							//temp_offset = offset;
							entityIdentifier(vg);
							parser_state = STATE_TEXT;
						} else if (ch == ']') {
								if (skipChar(vg,']')) {
									while (skipChar(vg,']')) {
									}
									if (skipChar(vg,'>')){		
										e.et = parse_exception;
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
							e.msg = "Parse Exception in parse()";
							e.sub_msg = "Error in text content: Invalid char";
							Throw e;
						}
						/*throw new ParseException(
						"Error in text content: Invalid char"
						+ formatLineNumber());*/
						break;

					case STATE_DEC_ATTR_NAME :
						if (ch == 'v'
							&& skipChar(vg,'e')
							&& skipChar(vg,'r')
							&& skipChar(vg,'s')
							&& skipChar(vg,'i')
							&& skipChar(vg,'o')
							&& skipChar(vg,'n')) {
								ch = getCharAfterS(vg);
								if (ch == '=') {

									/*System.out.println(
									" " + (temp_offset - 1) + " " + 7 + " dec attr name version " + depth);*/
									if (vg->encoding < FORMAT_UTF_16BE){

										writeVTD(vg,
											temp_offset - increment,
											7,
											TOKEN_DEC_ATTR_NAME,
											depth);
									}
									else{

										writeVTD(vg,
											(temp_offset - increment) >> 1,
											7,
											TOKEN_DEC_ATTR_NAME,
											depth);
									}
								} 
								else
								{		
									e.et = parse_exception;
									e.msg = "Parse Exception in parse()";
									e.sub_msg = "XML decl error: Invalid char";
									Throw e;
								}
								/*throw new ParseException(
								"XML decl error: Invalid char"
								+ formatLineNumber());*/
							} else{		
								e.et = parse_exception;
								e.msg = "Parse Exception in parse()";
								e.sub_msg = "XML decl error: should be version";
								Throw e;
							}
							/*throw new ParseException(
							"XML decl error: should be version"
							+ formatLineNumber());*/
							ch_temp = getCharAfterS(vg);
							if (ch_temp != '\'' && ch_temp != '"')
							{		
								e.et = parse_exception;
								e.msg = "Parse Exception in parse()";
								e.sub_msg = "XML decl error: Invalid char to start attr name";
								Throw e;
							}
							/*	throw new ParseException(
							"XML decl error: Invalid char to start attr name"
							+ formatLineNumber());*/
							temp_offset = vg->offset;
							// support 1.0 or 1.1
							if (skipChar(vg,'1')
								&& skipChar(vg,'.')
								&& (skipChar(vg,'0') || skipChar(vg,'1'))) {
									/*System.out.println(
									" " + temp_offset + " " + 3 + " dec attr val (version)" + depth);*/
									if (vg->encoding < FORMAT_UTF_16BE){

										writeVTD(vg,
											temp_offset,
											3,
											TOKEN_DEC_ATTR_VAL,
											depth);
									}
									else{

										writeVTD(vg,
											temp_offset >> 1,
											3,
											TOKEN_DEC_ATTR_VAL,
											depth);
									}
								} else
							{		
								e.et = parse_exception;
								e.msg = "Parse Exception in parse()";
								e.sub_msg = "XML decl error: Invalid version(other than 1.0 or 1.1) detected";
								Throw e;
							}
							/*	throw new ParseException(
							"XML decl error: Invalid version(other than 1.0 or 1.1) detected"
							+ formatLineNumber());*/
							if (!skipChar(vg,ch_temp))
							{		
								e.et = parse_exception;
								e.msg = "Parse Exception in parse()";
								e.sub_msg = "XML decl error: version not terminated properly";
								Throw e;
							}
							/*throw new ParseException(
							"XML decl error: version not terminated properly"
							+ formatLineNumber());*/
							ch = getChar(vg);
							//? space or e 
							if (XMLChar_isSpaceChar(ch)) {
								ch = getCharAfterS(vg);
								temp_offset = vg->offset - increment;
								if (ch == 'e') {
									if (skipChar(vg,'n')
										&& skipChar(vg,'c')
										&& skipChar(vg,'o')
										&& skipChar(vg,'d')
										&& skipChar(vg,'i')
										&& skipChar(vg,'n')
										&& skipChar(vg,'g')) {
											ch = getChar(vg);
											if (XMLChar_isSpaceChar(ch))
												ch = getCharAfterS(vg);
											if (ch == '=') {
												/*System.out.println(
												" " + (temp_offset) + " " + 8 + " dec attr name (encoding) " + depth);*/
												if (vg->encoding < FORMAT_UTF_16BE){

													writeVTD(vg,
														temp_offset,
														8,
														TOKEN_DEC_ATTR_NAME,
														depth);
												}
												else{

													writeVTD(vg,
														temp_offset >> 1,
														8,
														TOKEN_DEC_ATTR_NAME,
														depth);
												}
											} else{		
												e.et = parse_exception;
												e.msg = "Parse Exception in parse()";
												e.sub_msg = "XML decl error: Invalid char";
												Throw e;
											}
											/*	throw new ParseException(
											"XML decl error: Invalid char"
											+ formatLineNumber());*/
											ch_temp = getCharAfterS(vg);
											if (ch_temp != '"' && ch_temp != '\'')
											{		
												e.et = parse_exception;
												e.msg = "Parse Exception in parse()";
												e.sub_msg = "XML decl error: Invalid char to start attr name";
												Throw e;
											}
											/*	throw new ParseException(
											"XML decl error: Invalid char to start attr name"
											+ formatLineNumber());*/
											temp_offset = vg->offset;
											ch = getChar(vg);
											switch (ch) {
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
							&& skipChar(vg,ch_temp)) {
								if (vg->encoding != FORMAT_UTF_16LE
									&& vg->encoding
									!= FORMAT_UTF_16BE) {
										if (must_utf_8){
											e.et = parse_exception;
											e.msg = "Parse Exception in parse()";
											e.sub_msg = "Can't switch from UTF-8";
											Throw e;
										}
										vg->encoding = FORMAT_ASCII;
										/*System.out.println(
										" " + (temp_offset) + " " + 5 + " dec attr val (encoding) " + depth);*/
										if (vg->encoding
											< FORMAT_UTF_16BE){
												writeVTD(vg,
													temp_offset,
													5,
													TOKEN_DEC_ATTR_VAL,
													depth);
											}
										else{
											writeVTD(vg,
												temp_offset >> 1,
												5,
												TOKEN_DEC_ATTR_VAL,
												depth);
										}
										break;
									} else
								{		
									e.et = parse_exception;
									e.msg = "Parse Exception in parse()";
									e.sub_msg = "XML decl error: Can't switch encoding to ASCII";
									Throw e;
								}
								/*throw new ParseException(
								"XML decl error: Can't switch encoding to ASCII"
								+ formatLineNumber());*/
							}

							e.et = parse_exception;
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
							&& skipChar(vg,ch_temp)) {
								if (vg->encoding != FORMAT_UTF_16LE
									&& vg->encoding
									!= FORMAT_UTF_16BE) {
										if (must_utf_8){
											e.et = parse_exception;
											e.msg = "Parse Exception in parse()";
											e.sub_msg = "Can't switch from UTF-8";
											Throw e;
										}
										vg->encoding = FORMAT_ISO_8859;
										/*System.out.println(
										" " + (temp_offset) + " " + 10 + " dec attr val (encoding) " + depth);*/
										if (vg->encoding
											< FORMAT_UTF_16BE){
												writeVTD(vg,
													temp_offset,
													10,
													TOKEN_DEC_ATTR_VAL,
													depth);
											}
										else{
											writeVTD(vg,
												temp_offset >> 1,
												10,
												TOKEN_DEC_ATTR_VAL,
												depth);
										}
										break;
									} else
								{		
									e.et = parse_exception;
									e.msg = "Parse Exception in parse()";
									e.sub_msg = "XML decl error: Can't switch encoding to ISO-8859";
									Throw e;
								}
								/*	throw new ParseException(
								"XML decl error: Can't switch encoding to ISO-8859"
								+ formatLineNumber());*/
							}

							e.et = parse_exception;
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
							|| skipChar(vg,'S')))
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
								&& skipChar(vg,ch_temp)) {
									if (vg->encoding
										!= FORMAT_UTF_16LE
										&& vg->encoding
										!= FORMAT_UTF_16BE) {
											if (must_utf_8){
												e.et = parse_exception;
												e.msg = "Parse Exception in parse()";
												e.sub_msg = "Can't switch from UTF-8";
												Throw e;
											}
											vg->encoding = FORMAT_ASCII;
											//System.out.println(
											//    " " + (temp_offset) + " " + 5 + " dec attr val (encoding) " + depth);
											if (vg->encoding
												< FORMAT_UTF_16BE){
													writeVTD(vg,
														temp_offset,
														5,
														TOKEN_DEC_ATTR_VAL,
														depth);
												}
											else{
												writeVTD(vg,
													temp_offset
													>> 1,
													5,
													TOKEN_DEC_ATTR_VAL,
													depth);
											}
											break;

										} else
									{		
										e.et = parse_exception;
										e.msg = "Parse Exception in parse()";
										e.sub_msg = "XML decl error: Can't switch encoding to US-ASCII";
										Throw e;
									}
									/*throw new ParseException(
									"XML decl error: Can't switch encoding to US-ASCII"
									+ formatLineNumber());*/
								} else{		
									e.et = parse_exception;
									e.msg = "Parse Exception in parse()";
									e.sub_msg = "XML decl error: Invalid Encoding";
									Throw e;
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
											&& skipChar(vg,ch_temp)) {
												if (vg->encoding
													!= FORMAT_UTF_16LE
													&& vg->encoding
													!= FORMAT_UTF_16BE) {
														//encoding = FORMAT_UTF8;
														/*System.out.println(
														" " + (temp_offset) + " " + 5 + " dec attr val (encoding) " + depth);*/
														if (vg->encoding
															< FORMAT_UTF_16BE)
														{
															writeVTD(vg,
																temp_offset,
																5,
																TOKEN_DEC_ATTR_VAL,
																depth);
														}
														else
														{
															writeVTD(vg,
																temp_offset
																>> 1,
																5,
																TOKEN_DEC_ATTR_VAL,
																depth);
														}
														break;
													} else
												{		
													e.et = parse_exception;
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
													if (skipChar(vg,ch_temp)) {
														if (vg->encoding
															== FORMAT_UTF_16LE
															|| vg->encoding
															== FORMAT_UTF_16BE) {
																/*System.out.println(
																" " + (temp_offset) + " " + 6 + " dec attr val (encoding) " + depth);*/
																if (BOM_detected == FALSE){
																	e.et = parse_exception;
																	e.msg = "Parse Exception in parse()";
																	e.sub_msg = "BOM not detected for UTF-16";
																	Throw e;
																}
																if (vg->encoding
																	< FORMAT_UTF_16BE)
																{
																	writeVTD(vg,
																		temp_offset,
																		6,
																		TOKEN_DEC_ATTR_VAL,
																		depth);
																}
																else{
																	writeVTD(vg,
																		temp_offset
																		>> 1,
																		6,
																		TOKEN_DEC_ATTR_VAL,
																		depth);
																}
																break;
															}

															e.et = parse_exception;
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
														&& skipChar(vg,ch_temp)) {
															if (vg->encoding
																== FORMAT_UTF_16LE) {
																	/*System.out.println(
																	" " + (temp_offset) + " " + 7 + " dec attr val (encoding) " + depth);*/
																	if (vg->encoding
																		< FORMAT_UTF_16BE)
																	{
																		writeVTD(vg,
																			temp_offset,
																			7,
																			TOKEN_DEC_ATTR_VAL,
																			depth);
																	}
																	else
																	{
																		writeVTD(vg,
																			temp_offset
																			>> 1,
																			7,
																			TOKEN_DEC_ATTR_VAL,
																			depth);
																	}
																	break;
																}
																e.et = parse_exception;
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
															&& skipChar(vg,ch_temp)) {
																if (vg->encoding
																	== FORMAT_UTF_16BE) {
																		/*System.out.println(
																		" " + (temp_offset) + " " + 7 + " dec attr val (encoding) " + depth);*/
																		if (vg->encoding
																			< FORMAT_UTF_16BE)
																		{
																			writeVTD(vg,
																				temp_offset,
																				7,
																				TOKEN_DEC_ATTR_VAL,
																				depth);
																		}
																		else{
																			writeVTD(vg,
																				temp_offset
																				>> 1,
																				7,
																				TOKEN_DEC_ATTR_VAL,
																				depth);
																		}
																		break;
																	}
																	e.et = parse_exception;
																	e.msg = "Parse Exception in parse()";
																	e.sub_msg = "XML decl error: Can't switch encoding to UTF-16BE";
																	Throw e;
																	/*throw new ParseException(
																	"XML del error: Can't swtich encoding to UTF-16BE"
																	+ formatLineNumber());*/
															}
															e.et = parse_exception;
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
						e.msg = "Parse Exception in parse()";
						e.sub_msg = "XML decl error: Invalid encoding";
						Throw e;
						break;
						/*throw new ParseException(
						"XML decl Error: invalid encoding"
						+ formatLineNumber());*/
											}
											ch = getChar(vg);
											if (XMLChar_isSpaceChar(ch))
												ch = getCharAfterS(vg);
											temp_offset = vg->offset - increment;
										} else{		
											e.et = parse_exception;
											e.msg = "Parse Exception in parse()";
											e.sub_msg = "XML decl Error: Invalid char";
											Throw e;
										}
										/*throw new ParseException(
										"XML decl Error: Invalid char"
										+ formatLineNumber());*/
								}

								if (ch == 's') {
									if (skipChar(vg,'t')
										&& skipChar(vg,'a')
										&& skipChar(vg,'n')
										&& skipChar(vg,'d')
										&& skipChar(vg,'a')
										&& skipChar(vg,'l')
										&& skipChar(vg,'o')
										&& skipChar(vg,'n')
										&& skipChar(vg,'e')) {

											ch = getCharAfterS(vg);
											if (ch != '='){		
												e.et = parse_exception;
												e.msg = "Parse Exception in parse()";
												e.sub_msg = "XML decl error: Invalid char";
												Throw e;
											}
											/*throw new ParseException(
											"XML decl error: Invalid char"
											+ formatLineNumber());*/
											/*System.out.println(
											" " + temp_offset + " " + 3 + " dec attr name (standalone) " + depth);*/
											if (vg->encoding < FORMAT_UTF_16BE){
												writeVTD(vg,
													temp_offset,
													10,
													TOKEN_DEC_ATTR_NAME,
													depth);
											}
											else{
												writeVTD(vg,
													temp_offset >> 1,
													10,
													TOKEN_DEC_ATTR_NAME,
													depth);
											}
											ch_temp = getCharAfterS(vg);
											temp_offset = vg->offset;
											if (ch_temp != '"' && ch_temp != '\''){		
												e.et = parse_exception;
												e.msg = "Parse Exception in parse()";
												e.sub_msg = "XML decl error: Invalid char to start attr name";
												Throw e;
											}
											/*	throw new ParseException(
											"XML decl error: Invalid char to start attr name"
											+ formatLineNumber());*/
											ch = getChar(vg);
											//if (ch == 'y') {
											//	if (skipChar(vg,'e')
											//		&& skipChar(vg,'s')
											//		&& skipChar(vg,ch_temp)) {
											//		/*System.out.println(
											//		    " " + (temp_offset) + " " + 3 + " dec attr val (standalone) " + depth);*/
											//		if (vg->encoding < FORMAT_UTF_16BE)
											//				temp_offset,
											//				3,
											//				TOKEN_DEC_ATTR_VAL,
											//				depth);
											//		else
											//			writeVTD(vg,
											//				temp_offset >> 1,
											//				3,
											//				TOKEN_DEC_ATTR_VAL,
											//				depth);
											//		} else{		
											//			e.et = parse_exception;
											//			e.msg = "Parse Exception in parse()";
											//			e.sub_msg = "XML decl error: invalid val for standalone";
											//			Throw e;
											//		}
											//		/*throw new ParseException(
											//			"XML decl error: invalid val for standalone"
											//			+ formatLineNumber());*/
											//} else if (ch == 'n') {
											//	if (skipChar(vg,'o')
											//		&& skipChar(vg,ch_temp)) {
											//		/*System.out.println(
											//		    " " + (temp_offset) + " " + 2 + " dec attr val (standalone)" + depth);*/
											//		if (vg->encoding < FORMAT_UTF_16BE)
											//			writeVTD(vg,
											//				temp_offset,
											//				2,
											//				TOKEN_DEC_ATTR_VAL,
											//				depth);
											//		else
											//			writeVTD(vg,
											//				temp_offset >> 1,
											//				2,
											//				TOKEN_DEC_ATTR_VAL,
											//				depth);
											//		} else{		
											//			e.et = parse_exception;
											//			e.msg = "Parse Exception in parse()";
											//			e.sub_msg = "XML decl error: invalid val for standalone";
											//			Throw e;
											//		}
											//		/*throw new ParseException(
											//			"XML decl error: invalid val for standalone"
											//			+ formatLineNumber());*/
											//} else{		
											//	e.et = parse_exception;
											//	e.msg = "Parse Exception in parse()";
											//	e.sub_msg = "XML decl error: invalid val for standalone";
											//	Throw e;
											//}
											/*throw new ParseException(
											"XML decl error: invalid val for standalone"
											+ formatLineNumber());*/
										} else{		
											e.et = parse_exception;
											e.msg = "Parse Exception in parse()";
											e.sub_msg = "XML decl error";
											Throw e;
										}
										/*throw new ParseException(
										"XML decl error" + formatLineNumber());*/
										ch = getChar(vg);
										if (XMLChar_isSpaceChar(ch))
											ch = getCharAfterS(vg);
								}
							}

							if (ch == '?' && skipChar(vg,'>')) {
								temp_offset = vg->offset;
								ch = getCharAfterS(vg);
								if (ch == '<') {
									parser_state = STATE_LT_SEEN;
								} else{		
									e.et = parse_exception;
									e.msg = "Parse Exception in parse()";
									e.sub_msg = "Other Error: Invalid Char in XML";
									Throw e;
								}
								/*throw new ParseException(
								"Other Error: Invalid Char in XML"
								+ formatLineNumber());*/
							} else{		
								e.et = parse_exception;
								e.msg = "Parse Exception in parse()";
								e.sub_msg = "XML decl Error: Invalid termination sequence";
								Throw e;
							}
							/*throw new ParseException(
							"XML decl Error: Invalid termination sequence"
							+ formatLineNumber());*/
							break;

					case STATE_COMMENT :
						while (TRUE) {
							ch = getChar(vg);
							if (XMLChar_isValidChar(ch)) {
								if (ch == '-' && skipChar(vg,'-')) {
									length1 =
										vg->offset - temp_offset - 2 * increment;
									break;
								}
							} else{		
								e.et = parse_exception;
								e.msg = "Parse Exception in parse()";
								e.sub_msg = "Error in comment: Invalid Char";
								Throw e;
							}
							/*throw new ParseException(
							"Error in comment: Invalid Char"
							+ formatLineNumber());*/
						}
						if (getChar(vg) == '>') {
							//System.out.println(" " + (temp_offset) + " " + length1 + " comment " + depth);
							if (vg->encoding < FORMAT_UTF_16BE){
								writeVTD(vg,
									temp_offset,
									length1,
									TOKEN_COMMENT,
									depth);
							}
							else{
								writeVTD(vg,
									temp_offset >> 1,
									length1 >> 1,
									TOKEN_COMMENT,
									depth);
							}
							//length1 = 0;
							temp_offset = vg->offset;
							ch = getCharAfterSe(vg);
							if (ch == '<') {
								parser_state = STATE_LT_SEEN;
							} else if (XMLChar_isContentChar(ch)) {
								//temp_offset = offset;
								parser_state = STATE_TEXT;
							} else if (ch == '&') {
								//has_amp = true;
								//temp_offset = offset;
								entityIdentifier(vg);
								parser_state = STATE_TEXT;
							} else if (ch == ']') {
								if (skipChar(vg,']')) {
									while (skipChar(vg,']')) {
									}
									if (skipChar(vg,'>')){		
										e.et = parse_exception;
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
								e.msg = "Parse Exception in parse()";
								e.sub_msg = "XML decl error";
								Throw e;
							}
							/*throw new ParseException(
							"Error in text content: Invalid char"
							+ formatLineNumber());*/
							break;
						} else{		
							e.et = parse_exception;
							e.msg = "Parse Exception in parse()";
							e.sub_msg = "Error in comment: Invalid terminating sequence";
							Throw e;
						}
						/*throw new ParseException(
						"Error in comment: Invalid terminating sequence"
						+ formatLineNumber());*/
					case STATE_CDATA :
						while (TRUE) {
							ch = getChar(vg);
							if (XMLChar_isValidChar(ch)) {
								if (ch == ']' && skipChar(vg,']')) {
									while (skipChar(vg,']'));
									if (skipChar(vg,'>')) {
										break;
									} else{		
										e.et = parse_exception;
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
								e.msg = "Parse Exception in parse()";
								e.sub_msg = "Error in CDATA: Invalid Char";
								Throw e;
							}
							/*throw new ParseException(
							"Error in CDATA: Invalid Char"
							+ formatLineNumber());*/
						}
						length1 = vg->offset - temp_offset - 3 * increment;
						if (vg->encoding < FORMAT_UTF_16BE){
							writeVTD(vg,
								temp_offset,
								length1,
								TOKEN_CDATA_VAL,
								depth);
						}
						else{
							writeVTD(vg,
								temp_offset >> 1,
								length1 >> 1,
								TOKEN_CDATA_VAL,
								depth);
						}
						//System.out.println(" " + (temp_offset) + " " + length1 + " CDATA " + depth);
						ch = getCharAfterSe(vg);
						if (ch == '<') {
							parser_state = STATE_LT_SEEN;
						} else if (XMLChar_isContentChar(ch)) {
							temp_offset = vg->offset;
							parser_state = STATE_TEXT;
						} else if (ch == '&') {
							//has_amp = true;
							temp_offset = vg->offset;
							entityIdentifier(vg);
							parser_state = STATE_TEXT;
							//temp_offset = offset;
						} else if (ch == ']') {
								if (skipChar(vg,']')) {
									while (skipChar(vg,']')) {
									}
									if (skipChar(vg,'>')){		
										e.et = parse_exception;
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
							e.msg = "Parse Exception in parse()";
							e.sub_msg = "Other Error: Invalid char in xml";
							Throw e;
						}
						/*throw new ParseException(
						"Other Error: Invalid char in xml"
						+ formatLineNumber());*/
						break;
					case STATE_DOCTYPE :
						z = 1;
						while (TRUE) {
							ch = getChar(vg);
							if (XMLChar_isValidChar(ch)) {
								if (ch == '>')
									z--;
								else if (ch == '<')
									z++;
								if (z == 0)
									break;
							} else{		
								e.et = parse_exception;
								e.msg = "Parse Exception in parse()";
								e.sub_msg = "Error in DOCTYPE: Invalid char";
								Throw e;
							}
							/*throw new ParseException(
							"Error in DOCTYPE: Invalid char"
							+ formatLineNumber());*/
						}
						length1 = vg->offset - temp_offset - increment;
						/*System.out.println(
						" " + (temp_offset) + " " + length1 + " DOCTYPE val " + depth);*/
						if (vg->encoding < FORMAT_UTF_16BE){
							if (length1 > MAX_TOKEN_LENGTH){
								e.et = parse_exception;
								e.msg = "Parse Exception in parse()";
								e.sub_msg="Token Length Error: DTD_VAL length too long";
								Throw e;
							}
							writeVTD(vg,
								temp_offset,
								length1,
								TOKEN_DTD_VAL,
								depth);
						}
						else{
							if (length1 > (MAX_TOKEN_LENGTH<<1)){
								e.et = parse_exception;
								e.msg = "Parse Exception in parse()";
								e.sub_msg="Token Length Error: DTD_VAL length too long";
								Throw e;
							}
							writeVTD(vg,
								temp_offset >> 1,
								length1 >> 1,
								TOKEN_DTD_VAL,
								depth);
						}
						ch = getCharAfterS(vg);
						if (ch == '<') {
							parser_state = STATE_LT_SEEN;
						} else{		
							e.et = parse_exception;
							e.msg = "Parse Exception in parse()";
							e.sub_msg = "Other Error: Invalid char in xml";
							Throw e;
						}
						/*throw new ParseException(
						"Other Error: Invalid char in xml"
						+ formatLineNumber());*/
						break;

					case STATE_TEXT :
						if (depth == -1){		
							e.et = parse_exception;
							e.msg = "Parse Exception in parse()";
							e.sub_msg = "Error in text: Char data at the wrong place";
							Throw e;
						}
						/*throw new ParseException(
						"Error in text: Char data at the wrong place"
						+ formatLineNumber());*/
						while (TRUE) {
							ch = getChar(vg);
							if (XMLChar_isContentChar(ch)) {
							} else if (ch == '&') {
								//has_amp = true;
								if (!XMLChar_isValidChar(entityIdentifier(vg))){		
									e.et = parse_exception;
									e.msg = "Parse Exception in parse()";
									e.sub_msg = "Error in text content: Invalid char in text content";
									Throw e;
								}
								/*throw new ParseException(
								"Error in text content: Invalid char in text content "
								+ formatLineNumber());*/

								//parser_state = STATE_TEXT;
							} else if (ch == '<') {
								break;
							} else if (ch == ']') {
								if (skipChar(vg,']')) {
									while (skipChar(vg,']')) {
									}
									if (skipChar(vg,'>')){		
										e.et = parse_exception;
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
								e.msg = "Parse Exception in parse()";
								e.sub_msg = "Error in text content: Invalid char in text content";
								Throw e;
							}
							/*throw new ParseException(
							"Error in text content: Invalid char in text content"
							+ formatLineNumber());*/
						}
						length1 = vg->offset - increment - temp_offset;
						//if (has_amp) {
						/*System.out.println(
						" " + temp_offset + " " + length1 + " text with amp " + depth);*/
						if (vg->encoding < FORMAT_UTF_16BE){
							writeVTD(vg,
								temp_offset,
								length1,
								TOKEN_CHARACTER_DATA,
								depth);
						}
						else{
							writeVTD(vg,
								temp_offset >> 1,
								length1 >> 1,
								TOKEN_CHARACTER_DATA,
								depth);
						}
						//} else {
						//System.out.println(" " + temp_offset + " " + length1 + " text " + depth);
						//  if (encoding < 3)
						//  writeVTD(temp_offset, length1, TOKEN_CHARACTER_DATA_NO_ENTITY, depth);
						//else
						//writeVTD(temp_offset >> 1, length1 >> 1, TOKEN_CHARACTER_DATA_NO_ENTITY, depth);
						//}
						//has_amp = true;
						parser_state = STATE_LT_SEEN;
						break;

					case STATE_ATTR_NAME :

						if (ch == 'x') {
							if (skipChar(vg,'m')
								&& skipChar(vg,'l')
								&& skipChar(vg,'n')
								&& skipChar(vg,'s')) {
									ch = getChar(vg);
									if (ch == '='
										|| XMLChar_isSpaceChar(ch)
										|| ch == ':') {
											is_ns = TRUE; //break;
										}
								}
						}
						while (TRUE) {
							if (XMLChar_isNameChar(ch)) {
								if (ch == ':') {
									length2 = vg->offset - temp_offset - increment;
								}
								ch = getChar(vg);
							} else
								break;
						}
						length1 = getPrevOffset(vg) - temp_offset;
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
						if (XMLDoc[prevOffset + j] != XMLDoc[temp_offset + j]) {
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
									!= vg->XMLDoc[temp_offset + j]) {
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
								((Long) (temp_offset) << 32) + length1;
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
								e.msg = "alloc mem for attr_name_array_failed";
								Throw e;
							}
							vg->anaLen = attr_count + ATTR_NAME_ARRAY_SIZE;

							for (i = 0; i < attr_count; i++) {
								vg->attr_name_array[i] = temp_array[i];
							}
							vg->attr_name_array[attr_count] =
								((Long) (temp_offset) << 32) + length1;
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
										e.msg = "Parse Exception in parse()";
										e.sub_msg="Token Length Error: Attr NS prefix or qname length too long";
										Throw e;
									}

									writeVTD(vg,
										temp_offset,
										(length2 << 11) | length1,
										TOKEN_ATTR_NS,
										depth);

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
										e.msg = "Parse Exception in parse()";
										e.sub_msg="Token Length Error: Attr NS prefix or qname length too long";
										Throw e;
									}

									writeVTD(vg,
										temp_offset >> 1,
										(length2 << 10) | (length1 >> 1),
										TOKEN_ATTR_NS,
										depth);

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
										e.msg = "Parse Exception in parse()";
										e.sub_msg="Token Length Error: Attr name prefix or qname length too long";
										Throw e;
									}
									writeVTD(vg,
										temp_offset,
										(length2 << 11) | length1,
										TOKEN_ATTR_NAME,
										depth);

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
										e.msg = "Parse Exception in parse()";
										e.sub_msg="Token Length Error: Attr name prefix or qname length too long";
										Throw e;
									}

									writeVTD(vg,
										temp_offset >> 1,
										(length2 << 10) | (length1 >> 1),
										TOKEN_ATTR_NAME,
										depth);

							}
						}
						/*System.out.println(
						" " + temp_offset + " " + length2 + ":" + length1 + " attr name " + depth);*/
						length2 = 0;
						if (XMLChar_isSpaceChar(ch)) {
							ch = getCharAfterS(vg);
						}
						if (ch != '='){		
							e.et = parse_exception;
							e.msg = "Parse Exception in parse()";
							e.sub_msg = "Error in attr: invalid char";
							Throw e;
						}
						/*throw new ParseException(
						"Error in attr: invalid char"
						+ formatLineNumber());*/
						ch_temp = getCharAfterS(vg);
						if (ch_temp != '"' && ch_temp != '\''){		
							e.et = parse_exception;
							e.msg = "Parse Exception in parse()";
							e.sub_msg = "Error in attr: invalid char (should be ' or \" )";
							Throw e;
						}
						/*throw new ParseException(
						"Error in attr: invalid char (should be ' or \" )"
						+ formatLineNumber());*/
						temp_offset = vg->offset;
						parser_state = STATE_ATTR_VAL;
						break;
					case STATE_ATTR_VAL :
						while (TRUE) {
							ch = getChar(vg);
							if (XMLChar_isValidChar(ch) && ch != '<') {
								if (ch == ch_temp)
									break;
								if (ch == '&') {
									//has_amp = true;
									// as in vtd spec, we mark attr val with entities
									if (!XMLChar_isValidChar(entityIdentifier(vg))) {		
										e.et = parse_exception;
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
								e.msg = "Parse Exception in parse()";
								e.sub_msg = "Error in attr: Invalid XML char";
								Throw e;
							}
							/*throw new ParseException(
							"Error in attr: Invalid XML char"
							+ formatLineNumber());*/
						}

						length1 = vg->offset - temp_offset - increment;
						if (vg->encoding < FORMAT_UTF_16BE){
							if (length1 > MAX_TOKEN_LENGTH){
								e.et = parse_exception;
								e.msg = "Parse Exception in parse()";
								e.sub_msg="Token Length Error: ATTR_VAL length too long";
								Throw e;
							}
							writeVTD(vg,
								temp_offset,
								length1,
								TOKEN_ATTR_VAL,
								depth);
						}
						else{
							if (length1 > (MAX_TOKEN_LENGTH << 1)){
								e.et = parse_exception;
								e.msg = "Parse Exception in parse()";
								e.sub_msg="Token Length Error: ATTR_VAL length too long";
								Throw e;
							}
							writeVTD(vg,
								temp_offset >> 1,
								length1 >> 1,
								TOKEN_ATTR_VAL,
								depth);
						}
						ch = getChar(vg);
						if (XMLChar_isSpaceChar(ch)) {
							ch = getCharAfterS(vg);
							if (XMLChar_isNameStartChar(ch)) {
								temp_offset = vg->offset - increment;
								parser_state = STATE_ATTR_NAME;
								break;
							}
						}

						if (ch == '/') {
							depth--;
							ch = getChar(vg);
						}

						if (ch == '>') {
							attr_count = 0;
							if (depth != -1) {
								temp_offset = vg->offset;
								ch = getCharAfterSe(vg);
								if (ch == '<') {
									parser_state = STATE_LT_SEEN;
								} else if (XMLChar_isContentChar(ch)) {
									//temp_offset = offset;
									parser_state = STATE_TEXT;
								} else if (ch == '&') {
									//has_amp = true;
									//temp_offset = offset;
									entityIdentifier(vg);
									parser_state = STATE_TEXT;
								} else if (ch == ']') {
								if (skipChar(vg,']')) {
									while (skipChar(vg,']')) {
									}
									if (skipChar(vg,'>')){		
										e.et = parse_exception;
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
						e.msg = "Parse Exception in parse()";
						e.sub_msg = "Starting tag Error: Invalid char in starting tag";
						Throw e;

						/*throw new ParseException(
						"Starting tag Error: Invalid char in starting tag"
						+ formatLineNumber());*/

					case STATE_END_PI :										  
						ch = getChar(vg);
						if (XMLChar_isNameStartChar(ch)) {
							if ((ch == 'x' || ch == 'X')
								&& (skipChar(vg,'m') || skipChar(vg,'M'))
								&& (skipChar(vg,'l') && skipChar(vg,'L'))) {
									//temp_offset = offset;
									ch = getChar(vg);
									if (XMLChar_isSpaceChar(ch) || ch == '?'){		
										e.et = parse_exception;
										e.msg = "Parse Exception in parse()";
										e.sub_msg = "Error in PI: [xX][mM][lL] not a valid PI target";
										Throw e;
									}
									/*throw new ParseException(
									"Error in PI: [xX][mM][lL] not a valid PI target"
									+ formatLineNumber());*/
									//offset = temp_offset;
								}

								while (TRUE) {
									//ch = getChar(vg);
									if (!XMLChar_isNameChar(ch)) {
										break;
									}
									ch = getChar(vg);
								}

								length1 = vg->offset - temp_offset - increment;
								/*System.out.println(
								""
								+ (char) XMLDoc[temp_offset]
								+ " "
								+ (temp_offset)
								+ " "
								+ length1
								+ " PI Target "
								+ depth);*/
								if (vg->encoding < FORMAT_UTF_16BE){
									if (length1 > MAX_TOKEN_LENGTH){
										e.et = parse_exception;
										e.msg = "Parse Exception in parse()";
										e.sub_msg="Token Length Error: PI_NAME length too long";
										Throw e;
									}
									writeVTD(vg,
										temp_offset,
										length1,
										TOKEN_PI_NAME,
										depth);
								}
								else{
									if (length1 > (MAX_TOKEN_LENGTH<<1)){
										e.et = parse_exception;
										e.msg = "Parse Exception in parse()";
										e.sub_msg="Token Length Error: PI_NAME length too long";
										Throw e;
									}
									writeVTD(vg,
										temp_offset >> 1,
										length1 >> 1,
										TOKEN_PI_NAME,
										depth);
								}
								//length1 = 0;
								temp_offset = vg->offset;
								if (XMLChar_isSpaceChar(ch)) {
									ch = getCharAfterS(vg);

									while (TRUE) {
										if (XMLChar_isValidChar(ch)) {
											if (ch == '?')
												if (skipChar(vg,'>')) {
													parser_state = STATE_DOC_END;
													break;
												} else{		
													e.et = parse_exception;
													e.msg = "Parse Exception in parse()";
													e.sub_msg = "Error in PI: invalid termination sequence";
													Throw e;
												}
												/*throw new ParseException(
												"Error in PI: invalid termination sequence"
												+ formatLineNumber());*/
										} else{		
											e.et = parse_exception;
											e.msg = "Parse Exception in parse()";
											e.sub_msg = "Error in PI: Invalid char in PI val";
											Throw e;
										}
										/*throw new ParseException(
										"Error in PI: Invalid char in PI val"
										+ formatLineNumber());*/
										ch = getChar(vg);
									}
									length1 = vg->offset - temp_offset - 2 * increment;
									if (vg->encoding < FORMAT_UTF_16BE){
										if (length1 > MAX_TOKEN_LENGTH){
											e.et = parse_exception;
											e.msg = "Parse Exception in parse()";
											e.sub_msg="Token Length Error: PI_VAL length too long";
											Throw e;
										}
										writeVTD(vg,
											temp_offset,
											length1,
											TOKEN_PI_VAL,
											depth);
									}
									else{
										if (length1 > (MAX_TOKEN_LENGTH << 1)){
											e.et = parse_exception;
											e.msg = "Parse Exception in parse()";
											e.sub_msg="Token Length Error: PI_VAL length too long";
											Throw e;
										}
										writeVTD(vg,
											temp_offset >> 1,
											length1 >> 1,
											TOKEN_PI_VAL,
											depth);
									}
									//System.out.println(" " + temp_offset + " " + length1 + " PI val " + depth);
								} else {
									if ((ch == '?') && skipChar(vg,'>')) {
										parser_state = STATE_DOC_END;
									} else{		
										e.et = parse_exception;
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
							e.msg = "Parse Exception in parse()";
							e.sub_msg = "Error in PI: invalid char in PI target";
							Throw e;
						}
						/*throw new ParseException("Error in PI: invalid char in PI target");*/
						break;
					case STATE_END_COMMENT :
						while (TRUE) {
							ch = getChar(vg);
							if (XMLChar_isValidChar(ch)) {
								if (ch == '-' && skipChar(vg,'-')) {
									length1 =
										vg->offset - temp_offset - 2 * increment;
									break;
								}
							} else{		
								e.et = parse_exception;
								e.msg = "Parse Exception in parse()";
								e.sub_msg = "Error in comment: Invalid Char";
								Throw e;
							}
							/*throw new ParseException(
							"Error in comment: Invalid Char"
							+ formatLineNumber());*/
						}
						if (getChar(vg) == '>') {
							//System.out.println(" " + temp_offset + " " + length1 + " comment " + depth);
							if (vg->encoding < FORMAT_UTF_16BE){
								writeVTD(vg,
									temp_offset,
									length1,
									TOKEN_COMMENT,
									depth);
							}
							else
							{
								writeVTD(vg,
									temp_offset >> 1,
									length1 >> 1,
									TOKEN_COMMENT,
									depth);
							}
							//length1 = 0;
							parser_state = STATE_DOC_END;
							break;
						}		
						e.et = parse_exception;
						e.msg = "Parse Exception in parse()";
						e.sub_msg = "Error in comment: '-->' expected";
						Throw e;

						/*throw new ParseException(
						"Error in comment: '-->' expected"
						+ formatLineNumber());*/
					default :	
						e.et = parse_exception;
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
		if (parser_state != STATE_DOC_END){
			printLineNumber(vg);		
			Throw e;
		}
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
	vg->XMLDoc = ba;
	vg->docOffset = vg->offset = 0;
	vg->docLen = len;
	vg->endOffset = len;
	if (vg->docLen <= 1024) {
		a = 1024; //set the floor
	} else if (vg->docLen <= 1024 * 16 * 4) {
		a = 2048;
	} else if (vg->docLen <= 1024 * 256) {
		a = 1024 * 4;
	} else {
		a = 1 << 15;
	}
	//VTDBuffer = new FastLongBuffer(a);
	//l1Buffer = new FastLongBuffer(128);
	//l2Buffer = new FastLongBuffer(512);
	//l3Buffer = new FastIntBuffer(2048);
	vg->VTDBuffer = createFastLongBuffer3(a, len>>(a+1)); //new FastLongBuffer2(a, ba.length >> (a+1));
	vg->l1Buffer = createFastLongBuffer2(7); //new FastLongBuffer2(7);
	vg->l2Buffer = createFastLongBuffer2(9); //new FastLongBuffer2(9);
	vg->l3Buffer = createFastIntBuffer(11);  //new FastIntBuffer2(11);
	vg->vtdSize = vg->l1Size = vg->l2Size = vg->l3Size = 0;
}

// Set the XMLDoc container.Also set the offset and len of the document 
void setDoc2(VTDGen *vg, UByte *ba, int len, int os, int docLen){
	int a;
	vg->XMLDoc = ba;
	vg->docOffset = vg->offset = os;
	vg->docLen = len;
	vg->endOffset = os + len;
	if (docLen <= 1024) {
		a = 1024; //set the floor
	} else if (docLen <= 1024 * 16 * 4) {
		a = 2048;
	} else if (docLen <= 1024 * 256) {
		a = 1024 * 4;
	} else {
		a = 1 << 15;
	}
	//VTDBuffer = new FastLongBuffer(a);
	//l1Buffer = new FastLongBuffer(128);
	//l2Buffer = new FastLongBuffer(512);
	//l3Buffer = new FastIntBuffer(2048);
	vg->VTDBuffer = createFastLongBuffer3(a, len>>(a+1)); //new FastLongBuffer2(a, ba.length >> (a+1));
	vg->l1Buffer = createFastLongBuffer2(7); //new FastLongBuffer2(7);
	vg->l2Buffer = createFastLongBuffer2(9); //new FastLongBuffer2(9);
	vg->l3Buffer = createFastIntBuffer(11);  //new FastIntBuffer2(11);
	vg->vtdSize = vg->l1Size = vg->l2Size = vg->l3Size = 0;
}

// Increments offset only when the next char matches a given value.
static Boolean skipChar(VTDGen *vg, int ch){
	exception e;
	int temp = 0;
	int a = 0, c = 0, d = 0, val = 0, i=0;
	if (vg->offset >= vg->endOffset){
		e.et = parse_exception;
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
				if (temp <128)
					if (ch == temp) {
						vg->offset++;
						return TRUE;
					} else {
						return FALSE;
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
	//printf(" offset --> %d ; length -->%d ; tokenType ---> %d ; depth --> %d \n", offset, length, token_type, depth);
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
							| swap_bytes(length));
#endif
					for (k = length; k > MAX_TOKEN_LENGTH; k = k - MAX_TOKEN_LENGTH) {
#if BIG_ENDIAN
						appendLong(vg->VTDBuffer, l | r_offset);
#else
						appendLong(vg->VTDBuffer, l | (swap_bytes(r_offset)<< 32));
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
							| (swap_bytes(r_offset) << 32)));
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
							| (swap_bytes(offset) << 32)));
#endif
				}
				break;

			default:
#if BIG_ENDIAN

				appendLong(vg->VTDBuffer,((Long) ((token_type << 28)
					| ((depth & 0xff) << 20) | length) << 32)
					| offset);
#else
				appendLong(vg->VTDBuffer,((Long) ((token_type << 4)
						|(((depth & 0x0f) << 12) | (depth & 0xf0) >> 4)
							| swap_bytes(length))
							| (swap_bytes(offset) << 32)));

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

	} else if (token_type == TOKEN_ENDING_TAG && (depth == 0)) {
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
	}
}


