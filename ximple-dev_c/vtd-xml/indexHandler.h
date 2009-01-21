/* 
* Copyright (C) 2002-2009 XimpleWare, info@ximpleware.com
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

#ifndef INDEX_HANDLER_H
#define INDEX_HANDLER_H

#include "customTypes.h"
#include <stdio.h>
#include "vtdGen.h"

Boolean isLittleEndian();

/*writeIndex writes VTD+XML into a file
  This function throws index_write_exception*/
Boolean _writeIndex(Byte version, 
				int encodingType, 
				Boolean ns, 
				Boolean byteOrder, 
				int nestDepth, 
				int LCLevel, 
				int rootIndex, 
				UByte* xmlDoc, 
				int docOffset, 
				int docLen, 
				FastLongBuffer *vtdBuffer, 
				FastLongBuffer *l1Buffer, 
				FastLongBuffer *l2Buffer, 
				FastIntBuffer *l3Buffer, 
				FILE *f);

/*writeSeparateIndex writes VTD index into a separate file from XML
  notice that this function assumes XML document byte starts from the begining
  This function throws index_write_exception*/
Boolean _writeSeparateIndex(Byte version, 
				int encodingType, 
				Boolean ns, 
				Boolean byteOrder, 
				int nestDepth, 
				int LCLevel, 
				int rootIndex, 
				//UByte* xmlDoc, 
				int docOffset, 
				int docLen, 
				FastLongBuffer *vtdBuffer, 
				FastLongBuffer *l1Buffer, 
				FastLongBuffer *l2Buffer, 
				FastIntBuffer *l3Buffer, 
				FILE *f);

/*readIndex loads VTD+XML into VTDGen*/
Boolean _readIndex(FILE *f, VTDGen *vg);

Boolean _readIndex2(UByte *ba, int len, VTDGen *vg);

Boolean _readSeparateIndex(FILE *xml, int XMLSize, FILE *vtdIndex, VTDGen *vg);
#endif