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
#ifndef XMLMODIFIER_H
#define XMLMODIFIER_H
#include "vtdGen.h"
#include "xpath1.h"
#define XML_DELETE  0
#define XML_INSERT_BYTE  1
#define XML_INSERT_SEGMENT_BYTE  2
#define XML_INSERT_STRING  3
#define XML_INSERT_SEGMENT_STRING 4
#define DELETE_LIMIT 0x1ffffff
#define MASK_DELETE 0x00000000000000000LL
#define MASK_INSERT_SEGMENT_BYTE  0x2000000000000000LL
#define MASK_INSERT_BYTE 0x4000000000000000LL
#define MASK_INSERT_SEGMENT_STRING 0x6000000000000000LL 
#define MASK_INSERT_STRING 0x8000000000000000LL 

typedef Long (*getBytes)(UCSChar *s);
typedef struct xMLModifier{
	int encoding;
	IntHash *deleteHash;
	IntHash *insertHash;
	FastLongBuffer *flb; /* lower 32 bit offset, upper 29 bits 
						 length, upper 3 bits */
	FastLongBuffer *fob; /*lower 32 bits the object pointer, 
						 upper 32 bits the length of the byte array*/ 
	VTDNav *md; /*master document*/
	getBytes gbytes;
} XMLModifier;

XMLModifier *createXMLModifier();
XMLModifier *createXMLModifier2(VTDNav *vn);
void freeXMLModifier(XMLModifier *xm);
void bind4XMLModifier(XMLModifier *xm, VTDNav *md);
   /**
     * Removes content from the master XML document 
     * It first calls getCurrentIndex() if the result is 
     * a starting tag, then the entire element referred to
     * by the starting tag is removed
     * If the result is an attribute name or ns node, then 
     * the corresponding attribute name/value pair is removed
     * If the token type is one of text, CDATA or commment,
     * then the entire node, including the starting and ending 
     * delimiting text surrounding the content, is removed
     *
     */
void remove4XMLModifier(XMLModifier *xm);
void removeToken(XMLModifier *xm, int i);
void removeAttribute(XMLModifier *xm,int attrNameIndex);
/*void removeContent(XMLModifier *xm, int offset, int len);*/

void updateToken(XMLModifier *xm, int index, UCSChar *newContent);
void insertAfterElement(XMLModifier *xm, UCSChar *s);
void insertBeforeElement(XMLModifier *xm, UCSChar *s);
void insertAttribute(XMLModifier *xm, UCSChar *attr);

/*
void updateToken2(XMLModifier *xm, int index, UByte *newContentBytes, int len);
void insertAfterElement2(XMLModifier *xm, UByte *b, int len);
void insertBeforeElement2(XMLModifier *xm, UByte *b, int len);
void insertAttribute2(XMLModifier *xm, UByte *attr, int len);
*/

void output(XMLModifier *xm, FILE *f);
void resetXMLModifier(XMLModifier *xm);
#endif