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
#ifndef VTDGEN_H
#define VTDGEN_H

#include "customTypes.h"
#include "fastLongBuffer.h"
#include "fastIntBuffer.h"
#include "vtdNav.h"
#include "UTF8Char.h"
#include "XMLCHAR.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// define document encoding

// other constants

#define ATTR_NAME_ARRAY_SIZE 16
#define TAG_STACK_SIZE 256
#define MAX_DEPTH 255
#define MAX_TOKEN_LENGTH ((1<<20)-1)
#define MAX_PREFIX_LENGTH ((1<<9)-1)
#define MAX_QNAME_LENGTH ((1<<11)-1)

typedef struct vTDGen {
	int ns;
	int VTDDepth;
	int encoding;
	int last_depth;
	int last_l1_index;
	int last_l2_index;
	int last_i3_index;
	int offset;
	int prev_offset;
	int rootIndex;
	Byte* XMLDoc; // byte buffer containing
	int docLen; // length of XML (can be a segment of XMLDoc)
	int bufLen; // length of XMLDoc (possibly bigger than docLen)

	// buffers
	FastLongBuffer *VTDBuffer;
	FastLongBuffer *l1Buffer;
	FastLongBuffer *l2Buffer;
	FastIntBuffer *l3Buffer;

	int vtdSize;
	int l1Size;
	int l2Size;
	int l3Size;

	Boolean stateTransfered;

	int endOffset;
	Long* tag_stack;
	Long* attr_name_array;
	int anaLen;
	int docOffset;
} VTDGen;

// create VTDGen
VTDGen *createVTDGen();

// free VTDGen
void freeVTDGen(VTDGen *vg);

// clear the internal state of VTDGen so it can process 
// the next XML file
void clear(VTDGen *vg);

// Returns the VTDNav object after parsing, it also cleans 
// internal state so VTDGen can process the next file.
VTDNav *getNav(VTDGen *vg);

// Generating VTD tokens and Location cache info.
// One specifies whether the parsing is namespace aware or not.
void parse(VTDGen *vg, Boolean ns);

// Set the XMLDoc container.
void setDoc(VTDGen *vg, Byte *byteArray, int arrayLen);

// Set the XMLDoc container.Also set the offset and len of the document 
void setDoc2(VTDGen *vg, Byte *byteArray, int arrayLen, int offset, int docLen);

#endif