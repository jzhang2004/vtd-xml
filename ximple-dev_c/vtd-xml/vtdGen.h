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