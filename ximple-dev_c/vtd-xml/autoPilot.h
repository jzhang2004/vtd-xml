#ifndef AUTOPILOT_H
#define AUTOPILOT_H

#include "customTypes.h"
#include "vtdNav.h"
#include "cexcept.h"


typedef enum iter_type { UNDEFINED, SIMPLE, SIMPLE_NS } iterType; 
typedef struct autoPilot{
	UCS2Char *URL;
	UCS2Char *localName;
	UCS2Char *elementName;
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
void selectElement(AutoPilot *ap, UCS2Char *en);

//Select the element name (name space version) before iterating.
// * URL, if set to *, matches every namespace
// * URL, if set to null, indicates the namespace is undefined.
// * localname, if set to *, matches any localname
void selectElementNS(AutoPilot *ap, UCS2Char *URL, UCS2Char *ln);

//Iterate over all the selected element nodes.
Boolean iterateAP();

#endif