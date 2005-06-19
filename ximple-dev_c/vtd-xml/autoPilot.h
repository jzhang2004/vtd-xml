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
#ifndef AUTOPILOT_H
#define AUTOPILOT_H

#include "customTypes.h"
#include "vtdNav.h"
#include "cexcept.h"


typedef enum iter_type { UNDEFINED, SIMPLE, SIMPLE_NS } iterType; 
typedef struct autoPilot{
	UCSChar *URL;
	UCSChar *localName;
	UCSChar *elementName;
	int depth;
	VTDNav *vn;
	Boolean ft;
    iterType it;
	int startIndex;
} AutoPilot;

//create AutoPilot
AutoPilot *createAutoPilot(VTDNav *v);

// free AutoPilot
void freeAutoPilot(AutoPilot *ap);

//Select the element name before iterating
void selectElement(AutoPilot *ap, UCSChar *en);

//Select the element name (name space version) before iterating.
// * URL, if set to *, matches every namespace
// * URL, if set to null, indicates the namespace is undefined.
// * localname, if set to *, matches any localname
void selectElementNS(AutoPilot *ap, UCSChar *URL, UCSChar *ln);

//Iterate over all the selected element nodes.
Boolean iterateAP(AutoPilot *ap);

#endif

