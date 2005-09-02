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

// iter_type defines the type of iteration, to be used in function iterateAP
typedef enum iter_type { UNDEFINED, 
						SIMPLE, 
						SIMPLE_NS,
						DESCENDANT, 
						DESCENDANT_NS,
						FOLLOWING,
						FOLLOWING_NS, 
						PRECEDING, 
						PRECEDING_NS, 
						ATTR, 
						ATTR_NS} iterType;

typedef struct autoPilot{
	UCSChar *URL;
	UCSChar *localName;
	UCSChar *elementName;
	int depth;
	VTDNav *vn;
	int index; // for iterAttr
	Boolean ft;
	Boolean special;
    iterType it;
	//int startIndex;
	int size; // iterAttr
	int *contextCopy; // for preceding axis
	int stackSzie; // record stack size for xpath evaluation 
} AutoPilot;

//create AutoPilot
AutoPilot *createAutoPilot(VTDNav *v);

// free AutoPilot
void freeAutoPilot(AutoPilot *ap);

// Select an attribute name for iteration, * choose all attributes of an element
void selectAttribute(AutoPilot *ap, UCSChar *an);

// Select an attribute name, both local part and namespace URL part
void selectAttributeNS(AutoPilot *ap, UCSChar *URL, UCSChar *ln);


//Select the element name before iterating
void selectElement(AutoPilot *ap, UCSChar *en);

//Select the element name (name space version) before iterating.
// * URL, if set to *, matches every namespace
// * URL, if set to null, indicates the namespace is undefined.
// * localname, if set to *, matches any localname
void selectElementNS(AutoPilot *ap, UCSChar *URL, UCSChar *ln);


/**
 * Select all descendent elements along the descendent axis, without ns awareness
 * This function is not intended to be called directly
 * @param en
 */
void selectElement_D(AutoPilot *ap, UCSChar *en);

/**
 * Select all descendent elements along the Descendent axis, with ns awareness
 * This function is not intended to be called directly
 */
void selectElementNS_D(AutoPilot *ap, UCSChar *URL, UCSChar *ln);


/**
 * Select all descendent elements along the following axis, without ns awareness
 * This function is not intended to be called directly
 * @param en
 */
void selectElement_F(AutoPilot *ap, UCSChar *en);

/**
 * Select all descendent elements along the following axis, with ns awareness
 * This function is not intended to be called directly
 */
void selectElementNS_F(AutoPilot *ap, UCSChar *URL, UCSChar *ln);

/**
 * Select all descendent elements along the preceding axis, without ns awareness
 * This function is not intended to be called directly
 * @param en
 */
void selectElement_P(AutoPilot *ap, UCSChar *en);

/**
 * This function is not intended to be called directly
 * Select all descendent elements along the preceding axis, with ns awareness
 */
void selectElementNS_P(AutoPilot *ap, UCSChar *URL, UCSChar *ln);


/**
 * Setspecial is used by XPath evaluator to distinguish between
 * node() and *
 * node() corresponding to b= true;
 * This function is not intended to be called directly
 */
void setSpecial(AutoPilot *ap, Boolean b);


//Iterate over all the selected element nodes.

Boolean iterateAP(AutoPilot *ap);

// This method implements the attribute axis for XPath
int iterateAttr(AutoPilot *ap);

/**
 * This method implements the attribute axis for XPath
 */
Boolean iterateAttrAP(AutoPilot *ap);
#endif

