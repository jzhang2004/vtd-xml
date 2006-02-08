/* 
 * Copyright (C) 2002-2006 XimpleWare, info@ximpleware.com
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
// This method insert a prefix/URL pair into the nsList, if there are prefix duplicates 
// the URL in the list is replaced with new URL
static void insertItem(AutoPilot *ap, UCSChar *prefix, UCSChar *URL);

// given a prefix, find the URL
//UCSChar *lookup(NsList *nl, UCSChar *prefix);


// This method insert a prefix/URL pair into the nsList, if there are prefix duplicates 
// the URL is overwritten
void insertItem(AutoPilot *ap, UCSChar *prefix, UCSChar *URL){
	NsList *tmp = ap->nl;
	// search first
	if (tmp == NULL){
		tmp = (NsList *)malloc(sizeof(NsList));
		tmp->next = NULL; 
		tmp->prefix = prefix;
		tmp->URL = URL;
		ap->nl = tmp;
		return;
	}else {
		if (wcscmp(tmp->prefix,prefix) == 0){
			tmp->URL = URL; // overwritten
			return;
		}
		while(tmp->next!=NULL){
			if (wcscmp(tmp->prefix,prefix) == 0){
				tmp->URL = URL; // overwritten
				return;
			}
			tmp = tmp->next;
		}
		
		tmp->next = (NsList *)malloc(sizeof(NsList));
		tmp->next->next = NULL; 
		tmp->next->prefix = prefix;
		tmp->next->URL = URL;
		return;
	}
}

// given a prefix, find the URL
UCSChar *lookup(NsList *l, UCSChar *prefix){
	NsList *tmp = l;
	if (tmp==NULL)
		return NULL;
	while(tmp!= NULL){
		if (wcscmp(tmp->prefix,prefix)==0){
			return tmp->URL;
		}
		tmp = tmp->next;
	}
	return NULL;
}

//create AutoPilot throw exception if allocation failed
AutoPilot *createAutoPilot(VTDNav *v){
	exception e;
	AutoPilot *ap = NULL;
	if (v == NULL){
		e.et = invalid_argument;
		e.msg = " createAutoPilot failed: can't take NULL VTDNav pointer";
		Throw e;
	}

	ap = (AutoPilot *)malloc(sizeof(AutoPilot));
	if (ap == NULL){
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
	//ap->startIndex = -1;
	ap->xpe = NULL;
	ap->nl = NULL;
	ap->contextCopy = NULL;
	ap->special = FALSE;
	return ap;
}

// a argument-less constructor for autoPilot, 
// out_of_mem exception if allocation failed 
AutoPilot *createAutoPilot2(){
	exception e;
	AutoPilot *ap = NULL;
	ap = (AutoPilot *)malloc(sizeof(AutoPilot));
	if (ap == NULL){
		e.et = out_of_mem;
		e.msg = "createAutoPilot failed";
		Throw e;
	}
    //throw new IllegalArgumentException(" instance of VTDNav can't be null ");
    ap->elementName = NULL;
	ap->localName = NULL;
	ap->URL = NULL;
    ap->vn = NULL;
    //depth = v.getCurrentDepth();
    ap->it = UNDEFINED; // not defined
    ap->ft = TRUE;
	ap->xpe = NULL;
	//ap->startIndex = -1;
	ap->nl = NULL;
	ap->contextCopy = NULL;
	ap->special = FALSE;
	return ap;

}

// free AutoPilot
void freeAutoPilot(AutoPilot *ap){
	
	if (ap!=NULL){
		NsList *tmp = ap->nl;
		NsList *tmp2 = NULL;
		while(tmp!=NULL){
			tmp2 = tmp->next;
			free(tmp);
			tmp  = tmp2;			
		}
		if (ap->contextCopy!=NULL)
			free(ap->contextCopy);
		if (ap->xpe!= NULL)
		ap->xpe->freeExpr(ap->xpe);
	}	
	free(ap);

}

void printExprString(AutoPilot *ap){
	if (ap->xpe!=NULL)
		ap->xpe->toString(ap->xpe,NULL);
}

// Select an attribute name for iteration, * choose all attributes of an element
// this function is not called directly
void selectAttr(AutoPilot *ap, UCSChar *an){
	exception e;
	if (an == NULL){
		 e.et = invalid_argument;
		 e.msg = " invalid argument for selectElement, elementName can't be NULL";
		 Throw e;
	}
	ap->it= ATTR;
    ap->ft = TRUE;
    ap->size = getTokenCount(ap->vn);
	ap->elementName = an;
}

// Select an attribute name, both local part and namespace URL part
void selectAttrNS(AutoPilot *ap, UCSChar *URL, UCSChar *ln){
	exception e;
	if (ln == NULL){
		 e.et = invalid_argument;
		 e.msg = " invalid argument for selectElement, localName can't be NULL";
		 Throw e;
	}
	ap->it = ATTR_NS;
    ap->ft = TRUE;
    ap->localName = ln;
    ap->URL = URL;
}



//Select the element name before iterating
void selectElement(AutoPilot *ap, UCSChar *en){
	exception e;
    if (en == NULL){
		 e.et = invalid_argument;
		 e.msg = " invalid argument for selectElement, elementName can't be NULL";
		 Throw e;
	 }
    ap->it = SIMPLE;
    ap->depth = getCurrentDepth(ap->vn);
    //ap->startIndex = getCurrentIndex(ap->vn);
    ap->elementName = en;
    ap->ft = TRUE;
}

//Select the element name (name space version) before iterating.
// * URL, if set to *, matches every namespace
// * URL, if set to null, indicates the namespace is undefined.
// * localname, if set to *, matches any localname
void selectElementNS(AutoPilot *ap, UCSChar *URL, UCSChar *ln){
	exception e;
    if (ln == NULL){
		 e.et = invalid_argument;
		 e.msg = " invalid argument for selectElementNS, localName can't be NULL";
		 Throw e;
	}
    ap->it = SIMPLE_NS;
    ap->depth = getCurrentDepth(ap->vn);
    //ap->startIndex = getCurrentIndex(ap->vn);
    ap->localName = ln;
	ap->URL = URL;
    ap->ft = TRUE;
}


/**
 * this function is intended to be called during XPath evaluation
 * Select all descendent elements along the descendent axis, without ns awareness
 * @param en
 */
void selectElement_D(AutoPilot *ap, UCSChar *en){
	exception e;
    if (en == NULL){
		 e.et = invalid_argument;
		 e.msg = " invalid argument for selectElement_D, elementName can't be NULL";
		 Throw e;
	 }
	ap->it = DESCENDANT;
	ap->depth = getCurrentDepth(ap->vn);
	//startIndex = vn.getCurrentIndex();
	ap->elementName = en;
	ap->ft = TRUE;
}

/**
 * this function is intended to be called during XPath evaluation
 * Select all descendent elements along the Descendent axis, with ns awareness
 */
void selectElementNS_D(AutoPilot *ap, UCSChar *URL, UCSChar *ln){
	exception e;
    if (ln == NULL){
		 e.et = invalid_argument;
		 e.msg = " invalid argument for selectElementNS_D, localName can't be NULL";
		 Throw e;
	}
	ap->it = DESCENDANT_NS;
    ap->depth = getCurrentDepth(ap->vn);
    //startIndex = vn.getCurrentIndex();
    ap->localName = ln;
	ap->URL = URL;
    ap->ft = TRUE;
}


/**
 * this function is intended to be called during XPath evaluation
 * Select all descendent elements along the following axis, without ns awareness
 * @param en
 */
void selectElement_F(AutoPilot *ap, UCSChar *en){
	exception e;
    if (en == NULL){
		 e.et = invalid_argument;
		 e.msg = " invalid argument for selectElement_F, elementName can't be NULL";
		 Throw e;
	 }
	ap->it = FOLLOWING;
	ap->ft = TRUE;
	ap->elementName = en;
}
/**
 * this function is intended to be called during XPath evaluation
 * Select all descendent elements along the following axis, with ns awareness
 */
void selectElementNS_F(AutoPilot *ap, UCSChar *URL, UCSChar *ln){
	exception e;
    if (ln == NULL){
		 e.et = invalid_argument;
		 e.msg = " invalid argument for selectElementNS_F, localName can't be NULL";
		 Throw e;
	}
	ap->it = FOLLOWING_NS;
    ap->ft= TRUE;
    ap->localName = ln;
    ap->URL = URL;
}

/**
 * this function is intended to be called during XPath evaluation
 * Select all descendent elements along the preceding axis, without ns awareness
 * @param en
 */
void selectElement_P(AutoPilot *ap, UCSChar *en){
	int i;
	int a = sizeof(int)* ap->vn->nestingLevel;
	exception e;
    if (en == NULL){
		 e.et = invalid_argument;
		 e.msg = " invalid argument for selectElement_P, elementName can't be NULL";
		 Throw e;
	 }
	ap->depth = getCurrentDepth(ap->vn);
	ap->it = PRECEDING;
    ap->ft = TRUE;	
	ap->elementName = en;
    ap->contextCopy = (int *)malloc(a); //(int[])vn.context.clone();
	memcpy(ap->contextCopy,ap->vn->context,a);
	for(i = ap->vn->context[0]+1 ; i<ap->vn->nestingLevel ; i++){
        ap->contextCopy[i]=-1;
    }
    ap->contextCopy[0]=ap->vn->rootIndex;
}

/**
 * this function is intended to be called during XPath evaluation
 * Select all descendent elements along the preceding axis, with ns awareness
 */
void selectElementNS_P(AutoPilot *ap, UCSChar *URL, UCSChar *ln){
	int i;
	int a = sizeof(int)* ap->vn->nestingLevel;
	exception e;
    if (ln == NULL){
		 e.et = invalid_argument;
		 e.msg = " invalid argument for selectElementNS_P, localName can't be NULL";
		 Throw e;
	}
	ap->depth = getCurrentDepth(ap->vn);
	ap->it = PRECEDING_NS;
    ap->ft = TRUE;	
	ap->URL = URL;
	ap->localName = ln;
    ap->contextCopy = (int *)malloc(a); //(int[])vn.context.clone();
	memcpy(ap->contextCopy,ap->vn->context,a);
	for(i = ap->vn->context[0]+1 ; i<ap->vn->nestingLevel ; i++){
        ap->contextCopy[i]=-1;
    }
    ap->contextCopy[0]=ap->vn->rootIndex;
}


/**
 * Setspecial is used by XPath evaluator to distinguish between
 * node() and *
 * node() corresponding to b= true;
 */
void setSpecial(AutoPilot *ap, Boolean b){
	ap->special = b;
}

//Iterate over all the selected element nodes in document order.
Boolean iterateAP(AutoPilot *ap){
	exception e;
	switch (ap->it) {
		case SIMPLE :
			//	throw new PilotException(" Element name not set ");
			if (ap->vn->atTerminal)
				return FALSE;
			if (ap->ft == FALSE)
				return iterate(ap->vn, ap->depth, ap->elementName, ap->special);
			else {
				ap->ft = FALSE;
				if (ap->special || matchElement(ap->vn, ap->elementName)) {
					return TRUE;
				} else
					return iterate(ap->vn, ap->depth, ap->elementName,ap->special);
			}
		case SIMPLE_NS :
			if (ap->vn->atTerminal)
				return FALSE;
			if (ap->ft == FALSE)
				return iterateNS(ap->vn, ap->depth, ap->URL, ap->localName);
			else {
				ap->ft = FALSE;
				if (matchElementNS(ap->vn, ap->URL, ap->localName)) {
					
					return TRUE;
				} else
					return iterateNS(ap->vn, ap->depth, ap->URL, ap->localName);
			}
		case DESCENDANT:
			if (ap->vn->atTerminal)
				return FALSE;
			return iterate(ap->vn, ap->depth, ap->elementName, ap->special);
		case DESCENDANT_NS:
         	if (ap->vn->atTerminal)
         	    return FALSE;         	
         	return iterateNS(ap->vn, ap->depth, ap->URL, ap->localName);
		case FOLLOWING:
		   	if (ap->vn->atTerminal)
         	    return FALSE;
            if (ap->ft == FALSE)
                return iterate_following(ap->vn, ap->elementName, ap->special);
            else {
            	ap->ft = FALSE;
            	// find the first next sibling of 
            	while(TRUE){
            		while (toElement(ap->vn, NEXT_SIBLING)){
            			 if (ap->special || matchElement(ap->vn,ap->elementName)) {                	
                            return TRUE;
            			 }
            			 return iterate_following(ap->vn, ap->elementName, ap->special);
            		}
                    if (toElement(ap->vn, PARENT)==FALSE){
                    	//return vn.iterate_following(name, special);
                        return FALSE;
                    } 
            	}
            }
		case FOLLOWING_NS:
        	if (ap->vn->atTerminal)
         	    return FALSE;
         	if (ap->ft == FALSE)
                return iterate_followingNS(ap->vn,ap->URL,ap->localName);
            else {
            	ap->ft = FALSE;
            	// find the first next sibling of 
            	while(TRUE){
            		while (toElement(ap->vn, NEXT_SIBLING)){
            			 if (matchElementNS(ap->vn, ap->URL,ap->localName)) {                	
                            return TRUE;
            			 }
						 return iterate_followingNS(ap->vn, ap->URL,ap->localName);
            		}
                    if (toElement(ap->vn,PARENT)==FALSE){
						return FALSE;
                    } 
            	}
            }
		case PRECEDING:
			if (ap->vn->atTerminal)
         	    return FALSE;
         	return iterate_preceding(ap->vn, ap->elementName, ap->contextCopy, ap->special);

		case PRECEDING_NS:
			if (ap->vn->atTerminal)
         	    return FALSE;
         	return iterate_precedingNS(ap->vn, ap->URL,ap->localName,ap->contextCopy);
		default :
			
			e.et = pilot_exception;
			e.msg = "unknow iteration type for iterateAP";
			Throw e;
				
			//return;
	}
}

// This method implements the attribute axis for XPath
int iterateAttr(AutoPilot *ap){
	exception e;
	int i;
	switch(ap->it){
			case ATTR:
				if (wcscmp(ap->elementName,L"*")==0){
					if (ap->ft != FALSE){
						ap->ft = FALSE;
						ap->index = getCurrentIndex(ap->vn)+1;
					} else
						ap->index +=2;
					if (ap->vn->ns == FALSE){
						while(ap->index<=ap->size){
							tokenType type = getTokenType(ap->vn,ap->index);
							if (type == TOKEN_ATTR_NAME
								|| type == TOKEN_ATTR_NS){
									return ap->index;
								}else{   	    				
									return -1;
								}
						}
						return -1;
					}else {
						tokenType type = getTokenType(ap->vn,ap->index);
						while(ap->index<=ap->size){
							if (type == TOKEN_ATTR_NAME
								|| type == TOKEN_ATTR_NS){
									if (type == TOKEN_ATTR_NAME){
										return ap->index;
									}
									else 
										ap->index += 2;	    						
								}else{   	    				
									return -1;
								}
						}
						return -1;
					}
				}else{
					if (ap->ft == FALSE){
						return -1;
					} else {
						ap->ft = FALSE;
						i = getAttrVal(ap->vn,ap->elementName);
						if(i!=-1)
							return i-1;
						else 
							return -1;
					}   	    			
				}
			case ATTR_NS:
				if (ap->ft == FALSE){
					return -1;
				} else {
					ap->ft = FALSE;
				    i = getAttrValNS(ap->vn,ap->URL,ap->localName);
					if(i!=-1)
						return i-1;
					else 
						return -1;
				} 
			default:
				e.et = pilot_exception;
				e.msg = "unknow iteration type for iterateAP";
				Throw e;
	}
	
}

/*
 * This function selects the string representing XPath expression
 * Usually evalXPath is called afterwards
 */
Boolean selectXPath(AutoPilot *ap, UCSChar *s){
	exception e;
	if (s==NULL){
		e.et = xpath_parse_exception;
		e.msg = " xpath input string can't be NULL ";
		Throw e;
	}
	if(ap->xpe!=NULL)
		ap->xpe->freeExpr(ap->xpe);
	ap->xpe = xpathParse(s, ap->nl);
	if (ap->xpe == NULL){
		return FALSE;
	}
	return TRUE;
}

/*
 * Evaluate XPath
 */
int evalXPath(AutoPilot *ap){
	exception e;
	if (ap->xpe != NULL){
		if (ap->ft == TRUE){
			if (ap->vn != NULL){
	            ap->stackSize = ap->vn->contextBuf2->size;
			}
			ap->ft = FALSE;
		}
		return ap->xpe->evalNodeSet(ap->xpe,ap->vn);
	}
	else {
		e.et = other;
		e.msg = " xpe is NULL in autoPilot ";
		Throw e;
	}

}

/*
 * Reset XPath
 */
void resetXPath(AutoPilot *ap){
	if (ap->xpe != NULL && ap->vn!=NULL){
		ap->xpe->reset(ap->xpe,ap->vn);
		ap->vn->contextBuf2->size = ap->stackSize;
		ap->ft = TRUE;
	}
}

/*
 * This function creates URL ns prefix 
 *  and is intended to be called prior to selectXPath
 */
void declareXPathNameSpace(AutoPilot *ap, UCSChar* prefix, UCSChar *URL){
	if(ap!=NULL)
		insertItem(ap,prefix,URL);
}

/**
 * 
 */
void bind(AutoPilot *ap, VTDNav *new_vn){
	exception e;
	ap->elementName = NULL;
    ap->vn = new_vn;
    //depth = v.getCurrentDepth();
	ap->it = UNDEFINED; // not defined
    ap->ft = TRUE;
    ap->size = 0;
    ap->special = FALSE;
    //resetXPath(ap);
    //contextCopy = (int[])vn.context.clone();
}

