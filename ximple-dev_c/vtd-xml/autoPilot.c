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
#include "autoPilot.h"

//create AutoPilot
AutoPilot *createAutoPilot(VTDNav *v){
	exception e;
	AutoPilot *ap = NULL;
	if (v == NULL){
		e.et = invalid_argument;
		e.msg = " createAutoPilot failed: can't take NULL VTDNav pointer";
		Throw e;
	}

	ap = (AutoPilot *)malloc(sizeof(AutoPilot));
	if (ap = NULL){
		e.et = out_of_mem;
		e.msg = "createAutoPilot failed";
		Throw e;
	}
    //throw new IllegalArgumentException(" instance of VTDNav can't be null ");
    ap->elementName = NULL;
	ap->localName = NULL;
	ap->URL = NULL;
    ap->vn = v;
    //depth = v.getCurrentDepth();
    ap->it = UNDEFINED; // not defined
    ap->ft = TRUE;
	ap->startIndex = -1;
	return ap;
}

// free AutoPilot
void freeAutoPilot(AutoPilot *ap){
	free(ap);
}

//Select the element name before iterating
void selectElement(AutoPilot *ap, UCS2Char *en){
    ap->it = SIMPLE;
    ap->depth = getCurrentDepth(ap->vn);
    ap->startIndex = getCurrentIndex(ap->vn);
    ap->elementName = en;
    ap->ft = TRUE;
}

//Select the element name (name space version) before iterating.
// * URL, if set to *, matches every namespace
// * URL, if set to null, indicates the namespace is undefined.
// * localname, if set to *, matches any localname
void selectElementNS(AutoPilot *ap, UCS2Char *URL, UCS2Char *ln){
    ap->it = SIMPLE_NS;
    ap->depth = getCurrentDepth(ap->vn);
    ap->startIndex = getCurrentIndex(ap->vn);
    ap->localName = ln;
	ap->URL = URL;
    ap->ft = TRUE;
}

//Iterate over all the selected element nodes.
Boolean iterateAP(AutoPilot *ap){
	exception e;
	switch (ap->it) {
		case SIMPLE :
			if (ap->elementName == NULL){
				e.et = pilot_exception;
				e.msg = "element name not set";
				Throw e;
			}
			//	throw new PilotException(" Element name not set ");
			if (ap->ft == FALSE)
				return iterate(ap->vn, ap->depth, ap->elementName);
			else {
				if (matchElement(ap->vn, ap->elementName)) {
					ap->ft = FALSE;
					return TRUE;
				} else
					return iterate(ap->vn, ap->depth, ap->elementName);
			}
		case SIMPLE_NS :
			if (ap->localName == NULL) {
				e.et = pilot_exception;
				e.msg = "local name not set";
				Throw e;
			}

			if (ap->ft == FALSE)
				return iterateNS(ap->vn, ap->depth, ap->URL, ap->localName);
			else {
				if (matchElementNS(ap->vn, ap->URL, ap->localName)) {
					ap->ft = FALSE;
					return TRUE;
				} else
					return iterateNS(ap->vn, ap->depth, ap->URL, ap->localName);
			}
		default :
			
			e.et = pilot_exception;
			e.msg = "unknow iteration type for iterateAP";
			Throw e;
				
			//return;
	}
}