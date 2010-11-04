/*
* Copyright (C) 2002-2010 XimpleWare, info@ximpleware.com
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
#include "xpath.h"
#include "textIter.h"
static UCSChar *axisName(axisType i);
static Boolean isUnique_lpe(locationPathExpr *lpe, int i);
static int computeContextSize(locationPathExpr *lpe, Predicate *p, VTDNav *vn);
static int process_ancestor_or_self(locationPathExpr *lpe, VTDNav *vn);
static int process_ancestor(locationPathExpr *lpe, VTDNav *vn);
static int process_attribute(locationPathExpr *lpe, VTDNav *vn);
static int process_child(locationPathExpr *lpe, VTDNav *vn);
static int process_DDFP(locationPathExpr *lpe, VTDNav *vn);
static int process_following_sibling(locationPathExpr *lpe, VTDNav *vn);
static int process_parent(locationPathExpr *lpe, VTDNav *vn);
static int process_preceding_sibling(locationPathExpr *lpe, VTDNav *vn);
static int process_self(locationPathExpr *lpe, VTDNav *vn);
static int process_namespace(locationPathExpr *lpe, VTDNav *vn);
static void selectNodeType(locationPathExpr *lpe, TextIter *ti);

static void selectNodeType(locationPathExpr *lpe, TextIter *ti){
		if (lpe->currentStep->nt->testType == NT_TEXT )
			selectText(ti);
		else if (lpe->currentStep->nt->testType == NT_COMMENT )
			selectComment(ti);
		else if (lpe->currentStep->nt->testType == NT_PI0 )
			selectPI0(ti);
		else {
			selectPI1(ti,lpe->currentStep->nt->nodeName);
		}
		
	}


UCSChar *axisName(axisType i){
	switch(i){
			case AXIS_CHILD: return L"child::";
			case AXIS_DESCENDANT: return L"descendant::";
			case AXIS_PARENT: return L"parent::";
			case AXIS_FOLLOWING_SIBLING: return L"following-sibling::";
			case AXIS_PRECEDING_SIBLING: return L"preceding-sibling::";
			case AXIS_FOLLOWING: return L"following::";
			case AXIS_PRECEDING: return L"preceding::";
			case AXIS_ATTRIBUTE: return L"attribute::";
			case AXIS_NAMESPACE: return L"namespace::";
			case AXIS_SELF: return L"self::";
			case AXIS_DESCENDANT_OR_SELF: return L"descendant-or-self::";
			case AXIS_ANCESTOR: return L"ancestor::";
			default: return L"ancestor-or-self::";
	}
}

 Boolean isUnique_lpe(locationPathExpr *lpe, int i){

	return isUniqueIntHash(lpe->ih,i);
}

int computeContextSize(locationPathExpr *lpe, Predicate *p, VTDNav *vn){
	Boolean b = FALSE;
	//Predicate *tp = NULL;
    int i = 0;
    AutoPilot *ap = (AutoPilot *)lpe->currentStep->o;
	UCSChar *helper = NULL;
	switch(lpe->currentStep->axis_type){
    	case AXIS_CHILD:
			if (lpe->currentStep->nt->testType < NT_TEXT){
    			b = toElement(vn,FIRST_CHILD);
    			if (b) {
    				do {
						if (eval_s2(lpe->currentStep,vn, p)) {
							i++;
    					}
    				} while (toElement(vn, NEXT_SIBLING));
    				toElement(vn,PARENT);
					resetP2_s(lpe->currentStep,vn,p);
    				return i;
   				} else
    				return 0;
			}else {
				int result;
				TextIter* ti = createTextIter();
	    	    touch(ti,vn);
	    	    selectNodeType(lpe,ti);
	    	    while((result=getNext(ti))!=-1){
					vn->LN = result;
					vn->atTerminal = TRUE;
	    	        if (evalPredicates2(lpe->currentStep,vn,p)){
	    	            i++;
	    	        }
	    	    }
				vn->atTerminal = FALSE;
				resetP2_s(lpe->currentStep,vn,p);
	    	    return i;
			}

		case AXIS_DESCENDANT_OR_SELF:
		case AXIS_DESCENDANT:
		case AXIS_PRECEDING:
		case AXIS_FOLLOWING:

			if (lpe->currentStep->nt->testType == NT_NODE){
				helper = L"*";
			}else if (lpe->currentStep->nt->testType == NT_NAMETEST){
				helper = lpe->currentStep->nt->nodeName;
			}else    			
				throwException2(xpath_eval_exception,
				   "can't run descendant following, or following-sibling axis over comment(), pi(), and text()");
			if (ap==NULL)
					ap = createAutoPilot(vn);
				else
					bind(ap,vn);
			if (lpe->currentStep->axis_type == AXIS_DESCENDANT_OR_SELF )
				if (lpe->currentStep->nt->testType == NT_NODE)
					setSpecial(ap,TRUE);
				else
					setSpecial(ap,FALSE);
			//currentStep.o = ap = new AutoPilot(vn);
			if (lpe->currentStep->axis_type == AXIS_DESCENDANT_OR_SELF)
				if (lpe->currentStep->nt->localName!=NULL)
					selectElementNS(ap,lpe->currentStep->nt->URL,lpe->currentStep->nt->localName);
				else
					selectElement(ap,helper);
			else if (lpe->currentStep->axis_type == AXIS_DESCENDANT)
				if (lpe->currentStep->nt->localName!=NULL)
					selectElementNS_D(ap,lpe->currentStep->nt->URL,lpe->currentStep->nt->localName);
				else
					selectElement_D(ap,helper);
			else if (lpe->currentStep->axis_type == AXIS_PRECEDING)
				if (lpe->currentStep->nt->localName!=NULL)
					selectElementNS_P(ap,lpe->currentStep->nt->URL,lpe->currentStep->nt->localName);
				else
					selectElement_P(ap,helper);
			else
				if (lpe->currentStep->nt->localName!=NULL)
					selectElementNS_F(ap,lpe->currentStep->nt->URL,lpe->currentStep->nt->localName);
				else
					selectElement_F(ap,helper);
			push2(vn);
			while(iterateAP(ap)){
				if (evalPredicates2(lpe->currentStep,vn,p)){
					i++;
				}
			}
			pop2(vn);
			resetP2_s(lpe->currentStep,vn,p);
			lpe->currentStep->o= ap;
			//freeAutoPilot(ap);
			return i;

		case AXIS_PARENT:
			push2(vn);
			i = 0;
			if (toElement(vn, PARENT)){
				if (eval_s2(lpe->currentStep,vn,p)){
					i++;
				}
			}
			pop2(vn);
			resetP2_s(lpe->currentStep,vn,p);
			return i;

		case AXIS_ANCESTOR:
			push2(vn);
			i = 0;
			while (toElement(vn,PARENT)) {
				if (eval_s2(lpe->currentStep,vn, p)) {
					i++;
				}
			}
			pop2(vn);
			resetP2_s(lpe->currentStep,vn,p);
			return i;

		case AXIS_ANCESTOR_OR_SELF:
			push2(vn);
			i = 0;
			do {
				if (eval_s2(lpe->currentStep,vn, p)) {
					i++;
				}
			}while(toElement(vn,PARENT));
			pop2(vn);
			resetP2_s(lpe->currentStep,vn,p);
			return i;

		case AXIS_SELF:
			i = 0;
			if (toElement(vn,PARENT)){
				if (eval_s2(lpe->currentStep,vn,p)){
					i++;
				}
			}
			resetP2_s(lpe->currentStep,vn,p);
			return i;

		case AXIS_FOLLOWING_SIBLING:
			push2(vn);
			while(toElement(vn,NEXT_SIBLING)){
				if (eval_s2(lpe->currentStep,vn,p)){
					i++;
				}
			}
			pop2(vn);
			resetP2_s(lpe->currentStep,vn,p);
			return i;

		case AXIS_PRECEDING_SIBLING:
			push2(vn);
			while(toElement(vn,PREV_SIBLING)){
				if (eval_s2(lpe->currentStep,vn,p)){
					i++;
				}
			}
			pop2(vn);
			resetP2_s(lpe->currentStep,vn,p);
			return i;

		case AXIS_ATTRIBUTE:
			if (ap==NULL)
					ap = createAutoPilot(vn);
				else
					bind(ap,vn);
			//ap = createAutoPilot(vn);
			if (lpe->currentStep->nt->testType == NT_NODE)
				selectAttr(ap,L"*");
			else if (lpe->currentStep->nt->localName!=NULL)
				selectAttrNS(ap,lpe->currentStep->nt->URL,
				lpe->currentStep->nt->localName);
			else
				selectAttr(ap,lpe->currentStep->nt->nodeName);
			i = 0;
			while(iterateAttr2(ap)!=-1){
				if ( evalPredicates2(lpe->currentStep, vn, p)){
					i++;
				}
			}
			resetP2_s(lpe->currentStep,vn,p);
			lpe->currentStep->o= ap;
			//freeAutoPilot(ap);
			return i;

		case AXIS_NAMESPACE:
			if (ap==NULL)
					ap = createAutoPilot(vn);
				else
					bind(ap,vn);
			if (lpe->currentStep->nt->testType == NT_NODE)
				selectNameSpace(ap,L"*");
			else
				selectNameSpace(ap,lpe->currentStep->nt->nodeName);
			i = 0;
			while(iterateNameSpace(ap)!=-1){
				if ( evalPredicates2(lpe->currentStep, vn, p)){
					i++;
				}
			}
			resetP2_s(lpe->currentStep,vn,p);
			lpe->currentStep->o= ap;
			//freeAutoPilot(ap);
			return i;

		default:
			throwException2(xpath_eval_exception,
				"unknown state");
			return 0;
	}

}

static int process_ancestor_or_self(locationPathExpr *lpe, VTDNav *vn){
	Boolean b = FALSE, b1= FALSE;
	int result;
	//int contextSize;
	Predicate *t= NULL;

	switch ( lpe->state) {
		case  XPATH_EVAL_START:
			t = lpe->currentStep->p;
			while (t != NULL) {
				if (requireContextSize_p(t)) {
					int i = computeContextSize(lpe, t, vn);
					if (i == 0) {
						b1 = TRUE;
						break;
					} else
						setContextSize_p(t,i);
				}
				t = t->nextP;
			}
			if (b1) {
				lpe->state = XPATH_EVAL_END;
				break;
			}

			lpe->state =  XPATH_EVAL_END;
			push2(vn);

			if (get_ft(lpe->currentStep)== TRUE){
				set_ft(lpe->currentStep,FALSE);
				if (eval_s(lpe->currentStep, vn)) {
					if (getNextStep(lpe->currentStep) != NULL) {
						lpe->state =  XPATH_EVAL_FORWARD;
						lpe->currentStep = lpe->currentStep->nextS;
						break;
					} else {
						//vn.pop();
						lpe->state =  XPATH_EVAL_TERMINAL;
						if (vn->atTerminal)
							result = vn->LN;
						else
							result = getCurrentIndex(vn);
						if ( isUnique_lpe(lpe,result))
							return result;
					}
				}
			}

			while (toElement(vn,PARENT)) {
				if (eval_s(lpe->currentStep,vn)) {
					if (lpe->currentStep->nextS != NULL) {
						lpe->state =  XPATH_EVAL_FORWARD;
						lpe->currentStep = lpe->currentStep->nextS;
						break;
					} else {
						//vn.pop();
						lpe->state =  XPATH_EVAL_TERMINAL;
						result = getCurrentIndex(vn);
						if ( isUnique_lpe(lpe,result))
							return result;
					}
				}
			}

			if ( lpe->state ==  XPATH_EVAL_END) {
				resetP_s(lpe->currentStep, vn);
				pop2(vn);
			}

			break;

		case  XPATH_EVAL_FORWARD:
			t = lpe->currentStep->p;
			while (t != NULL) {
				if (requireContextSize_p(t)) {
					int i = computeContextSize(lpe, t, vn);
					if (i == 0) {
						b1 = TRUE;
						break;
					} else
						setContextSize_p(t,i);
				}
				t = t->nextP;
			}
			if (b1) {
				lpe->currentStep = lpe->currentStep->prevS;
				lpe->state = XPATH_EVAL_BACKWARD;
				break;
			}

			lpe->state =  XPATH_EVAL_BACKWARD;
			push2(vn);
			if (lpe->currentStep->ft == TRUE) {
				lpe->currentStep->ft= FALSE;
				if (eval_s(lpe->currentStep,vn)) {
					if (lpe->currentStep->nextS != NULL) {
						lpe->state =  XPATH_EVAL_FORWARD;
						lpe->currentStep = lpe->currentStep->nextS;
						break;
					} else {
						//vn.pop();
						lpe->state =  XPATH_EVAL_TERMINAL;
						if (vn->atTerminal)
							result = vn->LN;
						else
							result = getCurrentIndex(vn);
						if ( isUnique_lpe(lpe,result))
							return result;
					}
				}
			}
			while (toElement(vn,PARENT)) {
				if (eval_s(lpe->currentStep,vn)) {
					if (lpe->currentStep->nextS != NULL) {
						lpe->state =  XPATH_EVAL_FORWARD;
						lpe->currentStep = lpe->currentStep->nextS;
						break;
					} else {
						//vn.pop();
						lpe->state =  XPATH_EVAL_TERMINAL;
						result = getCurrentIndex(vn);
						if ( isUnique_lpe(lpe,result))
							return result;
					}
				}
			}

			if ( lpe->state ==  XPATH_EVAL_BACKWARD) {
				resetP_s(lpe->currentStep,vn);
				lpe->currentStep->ft = TRUE;
				pop2(vn);
				lpe->currentStep = lpe->currentStep->prevS;
			}
			break;

		case  XPATH_EVAL_END:
			lpe->currentStep = NULL;
			// reset();
	    	return -1;


		case  XPATH_EVAL_BACKWARD:
			b = FALSE;
			push2(vn);

			while (toElement(vn, PARENT)) {
				if (eval_s(lpe->currentStep,vn)) {
					if (lpe->currentStep->nextS != NULL) {
						lpe->state =  XPATH_EVAL_FORWARD;
						lpe->currentStep = lpe->currentStep->nextS;
						b = TRUE;
						break;
					} else {
						//vn.pop();
						lpe->state =  XPATH_EVAL_TERMINAL;
						result = getCurrentIndex(vn);
						if ( isUnique_lpe(lpe,result))
							return result;
					}
				}
			}
			if (b == FALSE) {
				pop2(vn);
				resetP_s(lpe->currentStep,vn);
				if (lpe->currentStep->prevS != NULL) {
					lpe->currentStep->ft = TRUE;
					lpe->state =  XPATH_EVAL_BACKWARD;
					lpe->currentStep = lpe->currentStep->prevS;
				} else {
					lpe->state =  XPATH_EVAL_END;
				}
			}
			break;

		case  XPATH_EVAL_TERMINAL:
			while (toElement(vn, PARENT)) {
				if (eval_s(lpe->currentStep,vn)) {
					result = getCurrentIndex(vn);
					if ( isUnique_lpe(lpe, result))
						return result;
				}
			}
			pop2(vn);
			resetP_s(lpe->currentStep,vn);
			if (lpe->currentStep->prevS != NULL) {
				lpe->currentStep->ft = TRUE;
				lpe->state =  XPATH_EVAL_BACKWARD;
				lpe->currentStep = lpe->currentStep->prevS;
			}
			else {
				 lpe->state =  XPATH_EVAL_END;
			}
			break;


		default:
			throwException2(xpath_eval_exception,
					"unknown state");
		}
		return -2;
}
static int process_ancestor(locationPathExpr *lpe, VTDNav *vn){
	int result;
	Boolean b = FALSE, b1 = FALSE;
	//int contextSize;
	Predicate *t= NULL;

	switch(lpe->state){
			case XPATH_EVAL_START:
				t = lpe->currentStep->p;
				while (t != NULL) {
					if (requireContextSize_p(t)) {
						int i = computeContextSize(lpe, t, vn);
						if (i == 0) {
							b1 = TRUE;
							break;
						} else
							setContextSize_p(t,i);
					}
					t = t->nextP;
				}
				if (b1) {
					lpe->state = XPATH_EVAL_END;
					break;
				}

				lpe->state = XPATH_EVAL_END;
				if (getCurrentDepth(vn) != -1) {
					push2(vn);

					while (toElement(vn,PARENT)) {
						if (eval_s(lpe->currentStep,vn)) {
							if (lpe->currentStep->nextS != NULL) {
								lpe->state = XPATH_EVAL_FORWARD;
								lpe->currentStep = lpe->currentStep->nextS;
								break;
							} else {
								//vn.pop();
								lpe->state = XPATH_EVAL_TERMINAL;
								result = getCurrentIndex(vn);
								if (isUnique_lpe(lpe,result))
									return result;
							}
						}
					}
					if (lpe->state == XPATH_EVAL_END) {
						resetP_s(lpe->currentStep,vn);
						pop2(vn);
					}
				}
				break;

			case XPATH_EVAL_END:
				lpe->currentStep =NULL;
				// reset();
				return -1;

			case XPATH_EVAL_FORWARD:
				t = lpe->currentStep->p;
				while(t!=NULL){
					if (requireContextSize_p(t)){
						int i = computeContextSize(lpe,t,vn);
						if (i==0){
							b1 = TRUE;
							break;
						}else
							setContextSize_p(t,i);
					}
					t = t->nextP;
				}
				if (b1){
					lpe->currentStep = lpe->currentStep->prevS;
					lpe->state = XPATH_EVAL_BACKWARD;
					break;
				}
				lpe->state =  XPATH_EVAL_BACKWARD;
				push2(vn);

				while(toElement(vn,P)){
					if (eval_s(lpe->currentStep,vn)){
						if (lpe->currentStep->nextS != NULL){
							lpe->state =  XPATH_EVAL_FORWARD;
							lpe->currentStep = lpe->currentStep->nextS;
							break;
						}
						else {
							//vn.pop();
							lpe->state =  XPATH_EVAL_TERMINAL;
							result = getCurrentIndex(vn);
							if ( isUnique_lpe(lpe,result))
								return result;
						}
					}
				}
				if ( lpe->state== XPATH_EVAL_BACKWARD){
					resetP_s(lpe->currentStep,vn);
					pop2(vn);
					lpe->currentStep=lpe->currentStep->prevS;
				}
				break;

			case XPATH_EVAL_BACKWARD:
				b = FALSE;
				push2(vn);

				while (toElement(vn,PARENT)) {
					if (eval_s(lpe->currentStep,vn)) {
						if (lpe->currentStep->nextS!= NULL) {
							lpe->state =  XPATH_EVAL_FORWARD;
							lpe->currentStep = lpe->currentStep->nextS;
							b = TRUE;
							break;
						} else {
							//vn.pop();
							lpe->state =  XPATH_EVAL_TERMINAL;
							result = getCurrentIndex(vn);
							if ( isUnique_lpe(lpe,result))
								return result;
						}
					}
				}
				if (b==FALSE){
					pop2(vn);
					if (lpe->currentStep->prevS!=NULL) {
						resetP_s(lpe->currentStep,vn);
						lpe->state =  XPATH_EVAL_BACKWARD;
						lpe->currentStep = lpe->currentStep->prevS;
					}
					else {
						lpe->state =  XPATH_EVAL_END;
					}
				}
				break;

			case XPATH_EVAL_TERMINAL:
				while (toElement(vn,PARENT)) {
					if (eval_s(lpe->currentStep,vn)) {
						result = getCurrentIndex(vn);
						if ( isUnique_lpe(lpe,result))
							return result;
					}
				}
				pop2(vn);

				if (lpe->currentStep->prevS!=NULL) {
					resetP_s(lpe->currentStep,vn);
					lpe->state =  XPATH_EVAL_BACKWARD;
					lpe->currentStep = lpe->currentStep->prevS;
				}
				else {
					lpe->state =  XPATH_EVAL_END;
				}
				break;

			default:
				throwException2(xpath_eval_exception,
					"unknown state");
	}
	return -2;

}
static int process_attribute(locationPathExpr *lpe, VTDNav *vn){
	AutoPilot *ap = NULL;
	Boolean b1 = FALSE;
	Predicate *t= NULL;
	int temp;
	switch(lpe->state){
		case  XPATH_EVAL_START:
		case  XPATH_EVAL_FORWARD:

			t = lpe->currentStep->p;
			while(t!=NULL){
				if (requireContextSize_p(t)){
					int i = computeContextSize(lpe,t,vn);
					if (i==0){
						b1 = TRUE;
						break;
					}else
						setContextSize_p(t,i);
				}
				t = t->nextP;
			}
			if (b1){
				if (lpe->state == XPATH_EVAL_FORWARD){
					lpe->state= XPATH_EVAL_BACKWARD;
					lpe->currentStep = lpe->currentStep->prevS;
				}else
					lpe->state= XPATH_EVAL_END;
				break;
			}

			if (getAtTerminal(vn)==TRUE){
				if (lpe->state ==XPATH_EVAL_START)
					lpe->state = XPATH_EVAL_END;
				else {
					lpe->state = XPATH_EVAL_BACKWARD;
					lpe->currentStep  = lpe->currentStep->prevS;
				}
			} else {
				if (lpe->currentStep->ft == TRUE) {
					if (lpe->currentStep->o == NULL)
						lpe->currentStep->o = ap = createAutoPilot(vn);
					else {
						ap = lpe->currentStep->o;
						bind(ap,vn);
					}
					if (lpe->currentStep->nt->testType== NT_NODE)
						selectAttr(ap,L"*");
					else if (lpe->currentStep->nt->localName != NULL)
						selectAttrNS(ap,lpe->currentStep->nt->URL,
                                lpe->currentStep->nt->localName);
					else 
						selectAttr(ap,lpe->currentStep->nt->nodeName);
					lpe->currentStep->ft = FALSE;
				}
				if ( lpe->state==  XPATH_EVAL_START)
					lpe->state=  XPATH_EVAL_END;
				setAtTerminal(vn,TRUE);
				while( (temp = iterateAttr2(ap)) != -1){
					if (evalPredicates(lpe->currentStep,vn)){
						break;
					}
				}
				if (temp == -1){
					lpe->currentStep->ft = TRUE;
					resetP_s(lpe->currentStep,vn);
					setAtTerminal(vn,FALSE);
					if ( lpe->state==  XPATH_EVAL_FORWARD){
						lpe->state =  XPATH_EVAL_BACKWARD;
						lpe->currentStep = lpe->currentStep->prevS;
					}
				}else {

					if (lpe->currentStep->nextS != NULL){
						vn->LN = temp;
						lpe->state=  XPATH_EVAL_FORWARD;
						lpe->currentStep = lpe->currentStep->nextS;
					}
					else {
						//vn.pop();
						lpe->state=  XPATH_EVAL_TERMINAL;
						if ( isUnique_lpe(lpe,temp)){
							vn->LN = temp;
							return temp;
						}
					}

				}
			}
			break;

		case  XPATH_EVAL_END:
			lpe->currentStep = NULL;
			// reset();
			return -1;

		case  XPATH_EVAL_BACKWARD:
			ap = lpe->currentStep->o;
			//vn.push();
			while( (temp = iterateAttr2(ap)) != -1){
				if (evalPredicates(lpe->currentStep,vn)){
					break;
				}
			}
			if (temp == -1) {
				lpe->currentStep->ft = TRUE;
				//freeAutoPilot(lpe->currentStep->o);
				//lpe->currentStep->o = NULL;
				resetP_s(lpe->currentStep,vn);
				setAtTerminal(vn,FALSE);
				if (lpe->currentStep->prevS != NULL) {
					lpe->state =  XPATH_EVAL_BACKWARD;
					lpe->currentStep = lpe->currentStep->prevS;
				} else
					lpe->state =  XPATH_EVAL_END;
			} else {
				if (lpe->currentStep->nextS != NULL) {
					lpe->state =  XPATH_EVAL_FORWARD;
					lpe->currentStep = lpe->currentStep->nextS;
				} else {
					lpe->state =  XPATH_EVAL_TERMINAL;
					if ( isUnique_lpe(lpe,temp)){
						vn->LN = temp;
						return temp;
					}
				}
			}
			break;

		case  XPATH_EVAL_TERMINAL:
			ap = lpe->currentStep->o;
			while( (temp = iterateAttr2(ap)) != -1){
				if (evalPredicates(lpe->currentStep,vn)){
					break;
				}
			}
			if (temp != -1)
				if (isUnique_lpe(lpe,temp)){
					vn->LN = temp;
					return temp;
				}
				setAtTerminal(vn,FALSE);
				resetP_s(lpe->currentStep,vn);
				if (lpe->currentStep->prevS == NULL) {
					lpe->currentStep->ft = TRUE;
					//freeAutoPilot(lpe->currentStep->o);
					//lpe->currentStep->o = NULL;
					lpe->state=  XPATH_EVAL_END;
				} else {
					lpe->state=  XPATH_EVAL_BACKWARD;
					lpe->currentStep->ft = TRUE;
					//freeAutoPilot(lpe->currentStep->o);
					//lpe->currentStep->o = NULL;
					lpe->currentStep = lpe->currentStep->prevS;
				}

				break;

		default:
			throwException2(xpath_eval_exception,
				"unknown state");
	}
	return -2;
}


static int process_namespace(locationPathExpr *lpe, VTDNav *vn){
	AutoPilot *ap = NULL;
	Boolean b1 = FALSE;
	Predicate *t= NULL;
	int temp;
	switch(lpe->state){
		case  XPATH_EVAL_START:
		case  XPATH_EVAL_FORWARD:

			t = lpe->currentStep->p;
			while(t!=NULL){
				if (requireContextSize_p(t)){
					int i = computeContextSize(lpe,t,vn);
					if (i==0){
						b1 = TRUE;
						break;
					}else
						setContextSize_p(t,i);
				}
				t = t->nextP;
			}
			if (b1){
				if (lpe->state == XPATH_EVAL_FORWARD){
					lpe->state= XPATH_EVAL_BACKWARD;
					lpe->currentStep = lpe->currentStep->prevS;
				}else
					lpe->state= XPATH_EVAL_END;
				break;
			}

			if (getAtTerminal(vn)==TRUE){
				if (lpe->state ==XPATH_EVAL_START)
					lpe->state = XPATH_EVAL_END;
				else {
					lpe->state = XPATH_EVAL_BACKWARD;
					lpe->currentStep  = lpe->currentStep->prevS;
				}
			} else {
				if (lpe->currentStep->ft == TRUE) {
					if (lpe->currentStep->o == NULL)
						lpe->currentStep->o = ap = createAutoPilot(vn);
					else {
						ap = lpe->currentStep->o;
						bind(ap,vn);
					}
					if (lpe->currentStep->nt->testType== NT_NODE)
						selectNameSpace(ap,L"*");
					else 
						selectNameSpace(ap,lpe->currentStep->nt->nodeName);
					lpe->currentStep->ft = FALSE;
				}
				if ( lpe->state==  XPATH_EVAL_START)
					lpe->state=  XPATH_EVAL_END;
				push2(vn);
				//setAtTerminal(vn,TRUE);
				while( (temp = iterateNameSpace(ap)) != -1){
					if (evalPredicates(lpe->currentStep,vn)){
						break;
					}
				}
				if (temp == -1){
					lpe->currentStep->ft = TRUE;
					resetP_s(lpe->currentStep,vn);
					setAtTerminal(vn,FALSE);
					if ( lpe->state==  XPATH_EVAL_FORWARD){
						lpe->state =  XPATH_EVAL_BACKWARD;
						lpe->currentStep = lpe->currentStep->prevS;
					}
				}else {

					if (lpe->currentStep->nextS != NULL){
						vn->LN = temp;
						lpe->state=  XPATH_EVAL_FORWARD;
						lpe->currentStep = lpe->currentStep->nextS;
					}
					else {
						//vn.pop();
						lpe->state=  XPATH_EVAL_TERMINAL;
						if ( isUnique_lpe(lpe,temp)){
							vn->LN = temp;
							return temp;
						}
					}

				}
			}
			break;

		case  XPATH_EVAL_END:
			lpe->currentStep = NULL;
			// reset();
			return -1;

		case  XPATH_EVAL_BACKWARD:
			ap = lpe->currentStep->o;
			//vn.push();
			while( (temp = iterateNameSpace(ap)) != -1){
				if (evalPredicates(lpe->currentStep,vn)){
					break;
				}
			}
			if (temp == -1) {
				pop2(vn);
				lpe->currentStep->ft = TRUE;
				//freeAutoPilot(lpe->currentStep->o);
				//lpe->currentStep->o = NULL;
				resetP_s(lpe->currentStep,vn);
				setAtTerminal(vn,FALSE);
				if (lpe->currentStep->prevS != NULL) {
					lpe->state =  XPATH_EVAL_BACKWARD;
					lpe->currentStep = lpe->currentStep->prevS;
				} else
					lpe->state =  XPATH_EVAL_END;
			} else {
				if (lpe->currentStep->nextS != NULL) {
					lpe->state =  XPATH_EVAL_FORWARD;
					lpe->currentStep = lpe->currentStep->nextS;
				} else {
					lpe->state =  XPATH_EVAL_TERMINAL;
					if ( isUnique_lpe(lpe,temp)){
						vn->LN = temp;
						return temp;
					}
				}
			}
			break;

		case  XPATH_EVAL_TERMINAL:
			ap = lpe->currentStep->o;
			while( (temp = iterateNameSpace(ap)) != -1){
				if (evalPredicates(lpe->currentStep,vn)){
					break;
				}
			}
			if (temp != -1)
				if (isUnique_lpe(lpe,temp)){
					vn->LN = temp;
					return temp;
				}
				setAtTerminal(vn,FALSE);
				resetP_s(lpe->currentStep,vn);
				if (lpe->currentStep->prevS == NULL) {
					lpe->currentStep->ft = TRUE;
					//freeAutoPilot(lpe->currentStep->o);
					//lpe->currentStep->o = NULL;
					pop2(vn);
					lpe->state=  XPATH_EVAL_END;
				} else {
					lpe->state=  XPATH_EVAL_BACKWARD;
					pop2(vn);
					lpe->currentStep->ft = TRUE;
					//freeAutoPilot(lpe->currentStep->o);
					//lpe->currentStep->o = NULL;
					lpe->currentStep = lpe->currentStep->prevS;
				}

				break;

		default:
			throwException2(xpath_eval_exception,
				"unknown state");
	}
	return -2;
}
static int process_child(locationPathExpr *lpe, VTDNav *vn){
	int result;
	Boolean b = FALSE, b1 = FALSE;
	Predicate *t= NULL;

	switch(lpe->state){
				case XPATH_EVAL_START:
					if (lpe->currentStep->nt->testType < NT_TEXT){
						/* first search for any predicate that
						// requires contextSize
						// if so, compute its context size
						// if size > 0
						// set context
						// if size ==0
						// immediately set the state to backward or end*/
						t = lpe->currentStep->p;
						while(t!=NULL){
							if (requireContextSize_p(t)){
								int i = computeContextSize(lpe, t,vn);
								if (i==0){
									b1 = TRUE;
									break;
								}else
									setContextSize_p(t,i);
							}
							t = t->nextP;
						}
						if (b1){
							lpe->state= XPATH_EVAL_END;
							break;
						}

						b=toElement(vn,FIRST_CHILD);
						lpe->state=  XPATH_EVAL_END;
						if (b == TRUE){
						 do {
							 if (eval_s(lpe->currentStep,vn)) {
								 if (lpe->currentStep->nextS != NULL){
									 //currentStep.position++;
									 lpe->state=  XPATH_EVAL_FORWARD;
									 lpe->currentStep = lpe->currentStep->nextS;
								 }
								 else {
									 lpe->state=  XPATH_EVAL_TERMINAL;
									 result = getCurrentIndex(vn);
									 if ( isUnique_lpe(lpe,result)){
										 return result;
									 }
								 }
								 break;
							 }
						 } while (toElement(vn,NS));
						 if (lpe->state == XPATH_EVAL_END)
							 toElement(vn,PARENT);
					 }
					}else {
						TextIter *ti = NULL;
						if (getAtTerminal(vn)==TRUE){
							lpe->state = XPATH_EVAL_END;
						}else {
						    // compute context size;
						    t = lpe->currentStep->p;
			    	        while(t!=NULL){
			    	            if (requireContextSize_p(t)){
			    	                int i = computeContextSize(lpe,t,vn);
			    	                if (i==0){
			    	                    b1 = TRUE;
			    	                    break;
			    	                }else
			    	                    setContextSize_p(t,i);
			    	            }
			    	            t = t->nextP;
			    	        }
			    	        // b1 false indicate context size is zero. no need to go any further...
			    	        if (b1){
			    	            lpe->state = XPATH_EVAL_END;
			    	            break;
			    	        }
			    	        // get textIter
						    if (lpe->currentStep->o != NULL){
						        ti = (TextIter*) lpe->currentStep->o;
						    } else {
						        ti = createTextIter();
						        lpe->currentStep->o = (struct autoPilot *)ti;
						    }
							selectNodeType(lpe,ti);
						    touch(ti,vn);
						    lpe->state = XPATH_EVAL_END;
						    while((result = getNext(ti))!=-1){
								vn->LN = result;
								vn->atTerminal = TRUE;
								if (evalPredicates(lpe->currentStep,vn)){
									break;
								}
						    }
						    // old code
							//result = vn.getText();
							if (result != -1){
								setAtTerminal(vn,TRUE);
								//currentStep.resetP(vn);
								vn->LN = result;
								if (getNextStep(lpe->currentStep) != NULL){
								    vn->LN = result;
				   				    lpe->state =  XPATH_EVAL_FORWARD;
									lpe->currentStep = getNextStep(lpe->currentStep);
								}
								else {
									//vn.pop();
									lpe->state =  XPATH_EVAL_TERMINAL;
									if (isUnique_lpe(lpe, result)){
									    vn->LN = result;
										return result;
									}
								}
							} else{
								//currentStep.set_ft(true);
								resetP_s(lpe->currentStep,vn);
								setAtTerminal(vn, FALSE);
							}
						}
					}
					break;
				case XPATH_EVAL_END:
					lpe->currentStep =NULL;
					// reset();
					return -1;

				case XPATH_EVAL_FORWARD:
					if (lpe->currentStep->nt->testType < NT_TEXT){
						t = lpe->currentStep->p;
						while(t!=NULL){
							if (requireContextSize_p(t)){
								int i = computeContextSize(lpe, t,vn);
								if (i==0){
									b1 = TRUE;
									break;
								}else
									setContextSize_p(t,i);
							}
							t = t->nextP;
						}
						if (b1){
							lpe->currentStep = lpe->currentStep->prevS;
							lpe->state= XPATH_EVAL_BACKWARD;
							break;
						}

						lpe->state =  XPATH_EVAL_BACKWARD;
						if (toElement(vn,FIRST_CHILD)) {
							do {
								if (eval_s(lpe->currentStep,vn)) {
									if (lpe->currentStep->nextS != NULL) {
										lpe->state=  XPATH_EVAL_FORWARD;
										lpe->currentStep = lpe->currentStep->nextS;
									} else {
										lpe->state=  XPATH_EVAL_TERMINAL;
										result = getCurrentIndex(vn);
										if ( isUnique_lpe(lpe,result))
											return result;
									}
									goto forward;
								}
							} while (toElement(vn,NEXT_SIBLING));
							toElement(vn,PARENT);
							resetP_s(lpe->currentStep,vn);
							lpe->currentStep = lpe->currentStep->prevS;
						} else {
							//vn.toElement(VTDNav.P);
							lpe->currentStep = lpe->currentStep->prevS;
						}
forward:;
					}else {
						TextIter *ti = NULL;

 // predicate at an attribute is not evaled
						if (getAtTerminal(vn) == TRUE){
							lpe->state =  XPATH_EVAL_BACKWARD;
							lpe->currentStep = getPrevStep(lpe->currentStep);
						}else {
						    // compute context size;
						    t = lpe->currentStep->p;
			    	        while(t!=NULL){
			    	            if (requireContextSize_p(t)){
			    	                int i = computeContextSize(lpe,t,vn);
			    	                if (i==0){
			    	                    b1 = TRUE;
			    	                    break;
			    	                }else
			    	                    setContextSize_p(t,i);
			    	            }
			    	            t = t->nextP;
			    	        }
			    	        // b1 false indicate context size is zero. no need to go any further...
			    	        if (b1){
			    	            lpe->state =  XPATH_EVAL_BACKWARD;
			    	            break;
			    	        }
			    	        // get textIter
						    if (lpe->currentStep->o != NULL){
						        ti = (TextIter*) lpe->currentStep->o;
						    } else {
						        ti = createTextIter();
						        lpe->currentStep->o = (AutoPilot *)ti;
						    }
						    touch(ti,vn);
							selectNodeType(lpe,ti);
						    //result = ti.getNext();

						    while((result = getNext(ti))!=-1){
								vn->LN = result;
								vn->atTerminal = TRUE;
								if (evalPredicates(lpe->currentStep,vn)){
									break;
								}
						    }

			                if (result == -1) {
			                    //currentStep.set_ft(true);
			                    //currentStep.resetP(vn);
			                    setAtTerminal(vn, FALSE);
			                    if (lpe->state ==  XPATH_EVAL_FORWARD) {
			                        lpe->state =  XPATH_EVAL_BACKWARD;
									lpe->currentStep = getPrevStep(lpe->currentStep);
			                    }
			                } else {
								setAtTerminal(vn, TRUE);
			                    if (getNextStep(lpe->currentStep) != NULL) {
			                        vn->LN = result;
			                        lpe->state =  XPATH_EVAL_FORWARD;
			                        lpe->currentStep = getNextStep(lpe->currentStep);
			                    } else {
			                        //vn.pop();
			                        lpe->state =  XPATH_EVAL_TERMINAL;
			                        if (isUnique_lpe(lpe,result)) {
			                            vn->LN = result;
			                            return result;
			                        }
			                    }
			                }
						}

					}

					break;

				case XPATH_EVAL_BACKWARD:
					if (lpe->currentStep->nt->testType < NT_TEXT) {
						b = FALSE;
						while (toElement(vn,NEXT_SIBLING)) {
							if (eval_s(lpe->currentStep,vn)) {
								b = TRUE;
								break;
							}
						}
						if (b == TRUE) {
							lpe->state=  XPATH_EVAL_FORWARD;
							lpe->currentStep = lpe->currentStep->nextS;
						} else if (lpe->currentStep->prevS == NULL){
							resetP_s(lpe->currentStep,vn);
							toElement(vn,PARENT);
							lpe->state=  XPATH_EVAL_END;
						}
						else {
							resetP_s(lpe->currentStep,vn);
							lpe->state=  XPATH_EVAL_BACKWARD;
							toElement(vn,PARENT);
							lpe->currentStep = lpe->currentStep->prevS;
						}
					}else {
						setAtTerminal(vn,FALSE);
						if (lpe->currentStep->prevS == NULL)
							lpe->state=  XPATH_EVAL_END;
						else {
							lpe->state=  XPATH_EVAL_BACKWARD;
							lpe->currentStep = lpe->currentStep->prevS;
						}
					}
					break;

				case XPATH_EVAL_TERMINAL:
					if (lpe->currentStep->nt->testType < NT_TEXT) {
						while (toElement(vn,NEXT_SIBLING)) {
							if (eval_s(lpe->currentStep,vn)) {
								result = getCurrentIndex(vn);
								if ( isUnique_lpe(lpe,result))
									return result;
							}
						}
						resetP_s(lpe->currentStep,vn);
						if (lpe->currentStep->prevS == NULL){
							lpe->state=  XPATH_EVAL_END;
							toElement(vn,PARENT);
						}
						else {
							toElement(vn,PARENT);

							lpe->state=  XPATH_EVAL_BACKWARD;
							lpe->currentStep = lpe->currentStep->prevS;
						}
					}else {
						TextIter* ti = (TextIter*) lpe->currentStep->o;
					    while ((result= getNext(ti))!=-1) {
							vn->LN = result;
							vn->atTerminal = TRUE;
							if (evalPredicates(lpe->currentStep,vn)) {
					            if ( isUnique_lpe(lpe,result))
									return result;
					        }
					    }
						resetP_s(lpe->currentStep, vn);
						setAtTerminal(vn, FALSE);
						if (getPrevStep(lpe->currentStep) == NULL)
							 lpe->state=  XPATH_EVAL_END;
						else {
							 lpe->state=  XPATH_EVAL_BACKWARD;
							 lpe->currentStep = getPrevStep(lpe->currentStep);
						}
						///////////////////////////////
					}
					break;

				default:
					throwException2(xpath_eval_exception,
						"unknown state");
	}
	return -2;
}
static int process_DDFP(locationPathExpr *lpe, VTDNav *vn){
	AutoPilot *ap;
	Boolean b = FALSE, b1 = FALSE;
	Predicate *t= NULL;
	int result;
	UCSChar *helper;
	switch(lpe->state){
			case XPATH_EVAL_START:
			case XPATH_EVAL_FORWARD:
				if (vn->atTerminal){
					if (lpe->state == XPATH_EVAL_START)
						lpe->state= XPATH_EVAL_END;
					else {
						// no need to set_ft to TRUE
						// no need to resetP
						lpe->state= XPATH_EVAL_BACKWARD;
						lpe->currentStep = lpe->currentStep->prevS;
					}
					break;
				}

				t = lpe->currentStep->p;
				while(t!=NULL){
					if (requireContextSize_p(t)){
						int i = computeContextSize(lpe, t,vn);
						if (i==0){
							b1 = TRUE;
							break;
						}else
							setContextSize_p(t,i);
					}
					t = t->nextP;
				}
				if (b1){
					if (lpe->state ==XPATH_EVAL_START)
						lpe->state= XPATH_EVAL_END;
					else {
						lpe->currentStep = lpe->currentStep->prevS;
						lpe->state= XPATH_EVAL_BACKWARD;
					}
					break;
				}


				helper = NULL;
				if (lpe->currentStep->nt->testType == NT_NAMETEST){
					helper = lpe->currentStep->nt->nodeName;
				}else if (lpe->currentStep->nt->testType == NT_NODE){
					helper = L"*";
				}else
    				throwException2(xpath_eval_exception,
					"can't run descendant following, or following-sibling axis over comment(), pi(), and text()"); 
				if (lpe->currentStep->o == NULL)
					lpe->currentStep->o = ap = createAutoPilot(vn);
				else{
					ap = lpe->currentStep->o;
					bind(ap,vn);
				}
				if (lpe->currentStep->ft == TRUE) {

					if (lpe->currentStep->axis_type == AXIS_DESCENDANT_OR_SELF )
						if (lpe->currentStep->nt->testType == NT_NODE)
							setSpecial(ap,TRUE);
						else
							setSpecial(ap,FALSE);
					//currentStep.o = ap = createAutoPilot(vn);
					if (lpe->currentStep->axis_type == AXIS_DESCENDANT_OR_SELF)
						selectElement(ap,helper);
					else if (lpe->currentStep->axis_type == AXIS_DESCENDANT)
						selectElement_D(ap,helper);
					else if (lpe->currentStep->axis_type == AXIS_PRECEDING)
						selectElement_P(ap,helper);
					else
						selectElement_F(ap,helper);
					lpe->currentStep->ft = FALSE;
				}
				if ( lpe->state==  XPATH_EVAL_START)
					lpe->state=  XPATH_EVAL_END;

				push2(vn); // not the most efficient. good for now
				//System.out.println("  --++ push in //");
				b = FALSE;
				while(iterateAP(ap)){
					if (evalPredicates(lpe->currentStep,vn)){
						b = TRUE;
						break;
					}
				}
				if (b == FALSE) {
					pop2(vn);
					//System.out.println("  --++ pop in //");
					lpe->currentStep->ft = TRUE;
					resetP_s(lpe->currentStep,vn);
					if ( lpe->state==  XPATH_EVAL_FORWARD){
						lpe->state =  XPATH_EVAL_BACKWARD;
						lpe->currentStep = lpe->currentStep->prevS;
					}
				} else {
					if (lpe->currentStep->nextS != NULL){
						lpe->state =  XPATH_EVAL_FORWARD;
						lpe->currentStep = lpe->currentStep->nextS;
					}
					else {
						//vn.pop();
						lpe->state =  XPATH_EVAL_TERMINAL;
						result = getCurrentIndex(vn);
						if ( isUnique_lpe(lpe,result))
							return result;
					}
				}
				break;

			case XPATH_EVAL_END:
				lpe->currentStep = NULL;
				// reset();
				return -1;

			case XPATH_EVAL_BACKWARD:
				//currentStep = lpe->currentStep->prevS;
				ap = lpe->currentStep->o;
				//vn.push();
				b = FALSE;
				while(iterateAP(ap)){
					if (evalPredicates(lpe->currentStep,vn)){
						b = TRUE;
						break;
					}
				}
				if (b == FALSE) {
					pop2(vn);
					lpe->currentStep->ft = TRUE;
					resetP_s(lpe->currentStep,vn);
					//System.out.println("  --++ pop in //");
					if (lpe->currentStep->prevS != NULL) {
						lpe->state=  XPATH_EVAL_BACKWARD;
						lpe->currentStep = lpe->currentStep->prevS;
					} else
						lpe->state=  XPATH_EVAL_END;
				} else {
					if (lpe->currentStep->nextS != NULL) {
						//vn.push();
						//System.out.println("  --++ push in //");
						lpe->state=  XPATH_EVAL_FORWARD;
						lpe->currentStep = lpe->currentStep->nextS;
					} else {
						lpe->state=  XPATH_EVAL_TERMINAL;
						result = getCurrentIndex(vn);
						if ( isUnique_lpe(lpe,result))
							return result;
					}
				}
				break;

			case XPATH_EVAL_TERMINAL:
				ap = lpe->currentStep->o;
				b = FALSE;
				while (iterateAP(ap)) {
					if (evalPredicates(lpe->currentStep,vn)) {
						b = TRUE;
						break;
					}
				}
				if (b == TRUE) {
					if (evalPredicates(lpe->currentStep,vn)) {
						result = getCurrentIndex(vn);
						if (isUnique_lpe(lpe,result))
							return result;
					}
				} else if (lpe->currentStep->prevS == NULL) {
					resetP_s(lpe->currentStep,vn);
					pop2(vn);
					lpe->state= XPATH_EVAL_END;
				} else {
					pop2(vn);
					lpe->currentStep->ft = TRUE;
					resetP_s(lpe->currentStep,vn);
					//System.out.println(" --++ pop in //");
					lpe->state= XPATH_EVAL_BACKWARD;
					//currentStep.ft = TRUE;
					lpe->currentStep = lpe->currentStep->prevS;
				}
				break;

			default:
				throwException2(xpath_eval_exception,
					"unknown state");
	}
	return -2;
}
static int process_following_sibling(locationPathExpr *lpe, VTDNav *vn){

	Boolean b = FALSE, b1 = FALSE;
	//int contextSize;
	Predicate *t= NULL;
	int result;
	switch( lpe->state){
		  case  XPATH_EVAL_START:
		  case  XPATH_EVAL_FORWARD:

			  t = lpe->currentStep->p;
			  while(t!=NULL){
				  if (requireContextSize_p(t)){
					  int i = computeContextSize(lpe, t,vn);
					  if (i==0){
						  b1 = TRUE;
						  break;
					  }else
						  setContextSize_p(t,i);
				  }
				  t = t->nextP;
			  }
			  if (b1){
				  if (lpe->state == XPATH_EVAL_FORWARD){
					  lpe->state= XPATH_EVAL_BACKWARD;
					  lpe->currentStep = lpe->currentStep->prevS;
				  }else
					  lpe->state= XPATH_EVAL_END;
				  break;
			  }
			  if ( lpe->state==  XPATH_EVAL_START)
				  lpe->state=  XPATH_EVAL_END;
			  else
				  lpe->state=  XPATH_EVAL_BACKWARD;
			  push2(vn);
			  while (toElement(vn,NEXT_SIBLING)){
				  if (eval_s(lpe->currentStep,vn)){
					  if (lpe->currentStep->nextS!=NULL){
						  lpe->state=  XPATH_EVAL_FORWARD;
						  lpe->currentStep = lpe->currentStep->nextS;
						  break;
					  } else {
						  lpe->state=  XPATH_EVAL_TERMINAL;
						  result = getCurrentIndex(vn);
						  if ( isUnique_lpe(lpe,result))
							  return result;
					  }
				  }
			  }

			  if ( lpe->state==  XPATH_EVAL_END){
				  resetP_s(lpe->currentStep,vn);
				  pop2(vn);
			  }else if ( lpe->state==  XPATH_EVAL_BACKWARD){
				  resetP_s(lpe->currentStep,vn);
				  pop2(vn);
				  lpe->currentStep = lpe->currentStep->prevS;
			  }
			  break;

		  case  XPATH_EVAL_END:
			  lpe->currentStep = NULL;
			  // reset();
			  return -1;

		  case  XPATH_EVAL_BACKWARD:
			  while (toElement(vn,NEXT_SIBLING)){
				  if (eval_s(lpe->currentStep,vn)){
					  if (lpe->currentStep->nextS!=NULL){
						  lpe->state=  XPATH_EVAL_FORWARD;
						  lpe->currentStep = lpe->currentStep->nextS;
						  b = TRUE;
						  break;
					  } else {
						  lpe->state=  XPATH_EVAL_TERMINAL;
						  result = getCurrentIndex(vn);
						  if ( isUnique_lpe(lpe,result))
							  return result;
					  }
				  }
			  }
			  if (b==FALSE){
				  pop2(vn);
				  resetP_s(lpe->currentStep,vn);
				  if (lpe->currentStep->prevS==NULL){
					  lpe->state=  XPATH_EVAL_END;
				  }else{
					  lpe->state=  XPATH_EVAL_BACKWARD;
					  lpe->currentStep = lpe->currentStep->prevS;
				  }
			  }
			  break;

		  case  XPATH_EVAL_TERMINAL:
			  while (toElement(vn,NEXT_SIBLING)){
				  if (eval_s(lpe->currentStep,vn)){
					  // lpe->state=  XPATH_EVAL_TERMINAL;
					  result = getCurrentIndex(vn);
					  if ( isUnique_lpe(lpe,result))
						  return result;
				  }
			  }
			  pop2(vn);
			  resetP_s(lpe->currentStep,vn);
			  if(lpe->currentStep->prevS!=NULL){
				  lpe->currentStep = lpe->currentStep->prevS;
				  lpe->state=  XPATH_EVAL_BACKWARD;
			  }else{
				  lpe->state=  XPATH_EVAL_END;
			  }
			  break;

		  default:
			  throwException2(xpath_eval_exception,
				  "unknown state");
	}
	return -2;
}

static int process_parent(locationPathExpr *lpe, VTDNav *vn){
	Boolean /*b = FALSE,*/ b1 = FALSE;
	Predicate *t= NULL;
	int result;
	switch ( lpe->state) {
			case  XPATH_EVAL_START:
			case  XPATH_EVAL_FORWARD:
				t = lpe->currentStep->p;
				while(t!=NULL){
					if (requireContextSize_p(t)){
						int i = computeContextSize(lpe, t,vn);
						if (i==0){
							b1 = TRUE;
							break;
						}else
							setContextSize_p(t,i);
					}
					t = t->nextP;
				}
				if (b1){
					if (lpe->state == XPATH_EVAL_FORWARD){
						lpe->state= XPATH_EVAL_BACKWARD;
						lpe->currentStep = lpe->currentStep->prevS;
					}else
						lpe->state= XPATH_EVAL_END;
					break;
				}

				if (getCurrentDepth(vn) == -1) {
					if ( lpe->state==  XPATH_EVAL_START)
						lpe->state=  XPATH_EVAL_END;
					else {
						//vn.pop();
						lpe->state=  XPATH_EVAL_BACKWARD;
						lpe->currentStep = lpe->currentStep->prevS;
					}
				} else {
					push2(vn);
					toElement(vn,PARENT); // must return TRUE
					if (eval_s(lpe->currentStep,vn)){
						if (lpe->currentStep->nextS != NULL) {
							lpe->state=  XPATH_EVAL_FORWARD;
							lpe->currentStep = lpe->currentStep->nextS;
						} else {
							lpe->state=  XPATH_EVAL_TERMINAL;
							result = getCurrentIndex(vn);
							if ( isUnique_lpe(lpe,result))
								return result;
						}
					}else{
						pop2(vn);
						resetP_s(lpe->currentStep,vn);
						if ( lpe->state==  XPATH_EVAL_START)
							lpe->state=  XPATH_EVAL_END;
						else {
							lpe->state=  XPATH_EVAL_BACKWARD;
							lpe->currentStep = lpe->currentStep->prevS;
						}
					}
				}

				break;

			case  XPATH_EVAL_END:
				lpe->currentStep = NULL;
				// reset();
				return -1;

			case  XPATH_EVAL_BACKWARD:
			case  XPATH_EVAL_TERMINAL:
				if (lpe->currentStep->prevS == NULL) {
					pop2(vn);
					lpe->state=  XPATH_EVAL_END;
					break;
				}else {
					pop2(vn);
					lpe->state=  XPATH_EVAL_BACKWARD;
					lpe->currentStep = lpe->currentStep->prevS;
					break;
				}

			default:
				throwException2(xpath_eval_exception,
					"unknown state");

	}
	return -2;
}

static int process_preceding_sibling(locationPathExpr *lpe, VTDNav *vn){

	Boolean b = FALSE, b1 = FALSE;
	Predicate *t= NULL;
	int result;
	switch(lpe->state){
		  case  XPATH_EVAL_START:
		  case  XPATH_EVAL_FORWARD:
			  t = lpe->currentStep->p;
			  while(t!=NULL){
				  if (requireContextSize_p(t)){
					  int i = computeContextSize(lpe,t,vn);
					  if (i==0){
						  b1 = TRUE;
						  break;
					  }else
						  setContextSize_p(t,i);
				  }
				  t = t->nextP;
			  }
			  if (b1){
				  if (lpe->state == XPATH_EVAL_FORWARD){
					  lpe->state= XPATH_EVAL_BACKWARD;
					  lpe->currentStep = lpe->currentStep->prevS;
				  }else
					  lpe->state= XPATH_EVAL_END;
				  break;
			  }
			  if ( lpe->state==  XPATH_EVAL_START)
				  lpe->state=  XPATH_EVAL_END;
			  else
				  lpe->state=  XPATH_EVAL_BACKWARD;
			  push2(vn);
			  while (toElement(vn,PREV_SIBLING)){
				  if (eval_s(lpe->currentStep,vn)){
					  if (lpe->currentStep->nextS!=NULL){
						  lpe->state=  XPATH_EVAL_FORWARD;
						  lpe->currentStep = lpe->currentStep->nextS;
						  break;
					  } else {
						  lpe->state=  XPATH_EVAL_TERMINAL;
						  result = getCurrentIndex(vn);
						  if ( isUnique_lpe(lpe,result))
							  return result;
					  }
				  }
			  }

			  if ( lpe->state==  XPATH_EVAL_END){
				  resetP_s(lpe->currentStep,vn);
				  pop2(vn);
			  }else if ( lpe->state==  XPATH_EVAL_BACKWARD){
				  resetP_s(lpe->currentStep,vn);
				  pop2(vn);
				  lpe->currentStep = lpe->currentStep->prevS;
			  }
			  break;

		  case  XPATH_EVAL_END:
			  lpe->currentStep = NULL;
			  // reset();
			  return -1;

		  case  XPATH_EVAL_BACKWARD:
			  while (toElement(vn,PREV_SIBLING)){
				  if (eval_s(lpe->currentStep,vn)){
					  if (lpe->currentStep->nextS!=NULL){
						  lpe->state=  XPATH_EVAL_FORWARD;
						  lpe->currentStep = lpe->currentStep->nextS;
						  b = TRUE;
						  break;
					  } else {
						  lpe->state=  XPATH_EVAL_TERMINAL;
						  result = getCurrentIndex(vn);
						  if ( isUnique_lpe(lpe,result))
							  return result;
					  }
				  }
			  }
			  if (b==FALSE){
				  pop2(vn);
				  resetP_s(lpe->currentStep,vn);
				  if (lpe->currentStep->prevS==NULL){
					  lpe->state=  XPATH_EVAL_END;
				  }else{
					  lpe->state=  XPATH_EVAL_BACKWARD;
					  lpe->currentStep = lpe->currentStep->prevS;
				  }
			  }
			  break;

		  case  XPATH_EVAL_TERMINAL:
			  while (toElement(vn,PREV_SIBLING)){
				  if (eval_s(lpe->currentStep,vn)){
					  // state =  XPATH_EVAL_TERMINAL;
					  result = getCurrentIndex(vn);
					  if ( isUnique_lpe(lpe,result))
						  return result;
				  }
			  }
			  pop2(vn);
			  if(lpe->currentStep->prevS!=NULL){
				  lpe->currentStep = lpe->currentStep->prevS;
				  lpe->state=  XPATH_EVAL_BACKWARD;
			  }else{
				  lpe->state=  XPATH_EVAL_END;
			  }
			  break;

		  default:
			  throwException2(xpath_eval_exception,
				  "unknown state");
	}
	return -2;
}
static int process_self(locationPathExpr *lpe, VTDNav *vn){
		Boolean /*b = FALSE,*/ b1 = FALSE;
	    //int contextSize;
	    Predicate *t= NULL;
	    int result;
		switch( lpe->state){
		  case  XPATH_EVAL_START:
		  case  XPATH_EVAL_FORWARD:
  	        t = lpe->currentStep->p;
	        while(t!=NULL){
	            if (requireContextSize_p(t)){
	                int i = computeContextSize(lpe,t,vn);
	                if (i==0){
	                    b1 = TRUE;
	                    break;
	                }else
	                    setContextSize_p(t,i);
	            }
	            t = t->nextP;
	        }
	        if (b1){
	            if (lpe->state == XPATH_EVAL_FORWARD){
	                lpe->state= XPATH_EVAL_BACKWARD;
	                lpe->currentStep = lpe->currentStep->prevS;
	            }else
	                lpe->state= XPATH_EVAL_END;
	            break;
	        }
		  	if (eval_s(lpe->currentStep,vn)){
		  		if (lpe->currentStep->nextS!=NULL){
		  			 lpe->state=  XPATH_EVAL_FORWARD;
		  			lpe->currentStep = lpe->currentStep->nextS;
		  		}
		  		else{
		  			 lpe->state=  XPATH_EVAL_TERMINAL;
		  			 if (vn->atTerminal == TRUE)
		  			     result = vn->LN;
		  			 else
		  			     result = getCurrentIndex(vn);
					if ( isUnique_lpe(lpe,result))
						return result;
		  		}
		  	}else {
		  		resetP_s(lpe->currentStep,vn);
		  		if ( lpe->state==  XPATH_EVAL_START)
		  			 lpe->state=  XPATH_EVAL_END;
		  		else
		  			 lpe->state=  XPATH_EVAL_BACKWARD;
		  	}
		    break;

		  case  XPATH_EVAL_END:
		  	lpe->currentStep = NULL;
		  	// reset();
		  	return -1;

		  case  XPATH_EVAL_BACKWARD:
		  case  XPATH_EVAL_TERMINAL:
		  	if (lpe->currentStep->prevS!=NULL){
	  			 lpe->state=  XPATH_EVAL_BACKWARD;
	  			lpe->currentStep= lpe->currentStep->prevS;
	  		}else{
	  			 lpe->state=  XPATH_EVAL_END;
	  		}
		  	break;

		  default:
			throwException2(xpath_eval_exception,"unknown state");
		}
	    return -2;
	}



NodeTest *createNodeTest(){
	NodeTest *nt = (NodeTest *)malloc(sizeof(NodeTest));
	if (nt==NULL){
		throwException2(out_of_mem,
			"NodeTest allocation failed ");
	}
	nt->nsEnabled = FALSE;
	nt->localName = NULL;
	nt->prefix = NULL;
	nt->URL = NULL;
	nt->nodeName = NULL;
	return nt;
}
void freeNodeTest(NodeTest *nt){
	if (nt==NULL) return;
	free(nt->localName);
	free(nt->nodeName);
	free(nt->prefix);
	//free(nt->URL);
	free(nt);
}
Boolean eval_nt(NodeTest *nt, VTDNav *vn){
	if (nt->testType == NT_NODE)
		return TRUE;
	else if(nt->testType == NT_NAMETEST){
	    if (vn->atTerminal == TRUE)
	        return FALSE;
	    if (nt->localName!=NULL)
	        return matchElementNS(vn,nt->URL,nt->localName);
	    else
	        return matchElement(vn,nt->nodeName);
	}
	return FALSE;
}
void setNodeName(NodeTest *nt, UCSChar *name){
	nt->nodeName = name;
}

void setNodeNameNS(NodeTest *nt, UCSChar *p, UCSChar *ln){
	nt->localName = ln;
	nt->prefix = p;
}

void setTestType(NodeTest *nt, nodeTestType ntt){
	nt->testType = ntt;
}

void toString_nt(NodeTest *nt, UCSChar *string){
	switch (nt->testType){
		case NT_NAMETEST :
		    if (nt->localName == NULL)
		        wprintf(L"%ls",nt->nodeName);
		    else
				wprintf(L"%ls:%ls", nt->prefix,nt->localName);
			break;
		case NT_NODE: wprintf(L"node()");break;
		case NT_TEXT: wprintf(L"text()");break;
		case NT_PI0:  wprintf(L"processing-instruction()");break;
		case NT_PI1: wprintf(L"processing-instruction(");
			if (wcschr(nt->nodeName,'"')!=NULL){
				wprintf(L"'");
				wprintf(nt->nodeName);
				wprintf(L"'");
			}else{
				wprintf(L"\"");
				wprintf(nt->nodeName);
				wprintf(L"\"");
			}
			         wprintf(L")");
					 break;
		default:  wprintf(L"comment()");
	}
}


Predicate *createPredicate(){
	Predicate *p = (Predicate *)malloc(sizeof(Predicate));
	if (p==NULL){
		throwException2(out_of_mem,
			"Predicate allocation failed ");
	}
	p->nextP = NULL;
	p->e = NULL;
	p->count = 0;
	p->d = 0;
	return p;
}

void freePredicate(Predicate *p){
	if (p==NULL)
		return;
	if(p->e!=NULL)
		p->e->freeExpr(p->e);
	free(p);
}

Boolean eval_p(Predicate *p, VTDNav *vn){
	Boolean b;
	p->count++; // increment the position
	p->e->setPosition(p->e,p->count);
	if (p->e->isNumerical(p->e)){
		b = (p->e->evalNumber(p->e,vn)== p->count);
	}
	else{
		b = p->e->evalBoolean(p->e,vn);
	}
	return b;
}

void setIndex_p(Predicate *p, int index){
	if (index<=0){
		throwException2(xpath_eval_exception,
			"Invalid index number");
	}
	p->d = (double) index;
}

void setContextSize_p(Predicate *p, int size){
	p->e->setContextSize(p->e,size);
}

Boolean requireContextSize_p(Predicate *p){
	return p->e->requireContextSize(p->e);
}

void reset_p(Predicate *p, VTDNav* vn){
	p->count = 0;
	p->e->reset(p->e,vn); // is this really needed?
}

void toString_p(Predicate *p, UCSChar *string){
		//String s = "["+expr+"]";
		if (p->nextP==NULL){
			wprintf(L"[");
			p->e->toString(p->e,string);
			wprintf(L"]");
		} else {
			wprintf(L"[");
			p->e->toString(p->e,string);
			wprintf(L"]");
			toString_p(p->nextP,string);
		}
}

Step *createStep(){
	Step *s = malloc(sizeof(Step));
	if (s==NULL){
		throwException2(out_of_mem,
			"Step allocation failed ");
	}
	s->nextS = s->prevS = NULL;
	s->p  = s->pt = NULL;
	s->nt = NULL;
	s->ft = TRUE;
	s->position = 1;
	s->o = NULL;

	return s;
}

void freeStep(Step *s){
	Predicate *tmp, *tmp2;
	if (s==NULL)return;
	if (s->p != NULL){
		tmp = s->p;
		tmp2 = tmp->nextP;
		while(tmp2!=NULL){
			freePredicate(tmp);
			tmp= tmp2;
			tmp2 = tmp2->nextP;
		}
		freePredicate(tmp);
	}
	if (s->nt->testType == NT_TEXT){
		if (s->o!=NULL)
			freeTextIter((TextIter *)s->o);
	}else
		freeAutoPilot(s->o);
	freeNodeTest(s->nt);
	free(s);
}

void reset_s(Step *s, VTDNav *vn){
	s->ft = TRUE;
	resetP_s(s,vn);
	s->position =1 ;
}

void resetP_s(Step *s,VTDNav *vn){
		Predicate *temp = s->p;
		while(temp!=NULL){
			reset_p(temp,vn);
			temp = temp->nextP;
		}
}
void resetP2_s(Step *s,VTDNav *vn, Predicate *p1){
		Predicate *temp = s->p;
		while(temp!=p1){
			reset_p(temp,vn);
			temp = temp->nextP;
		}
}

NodeTest *getNodeTest(Step *s){
	return s->nt;
}
Step *getNextStep(Step *s){
	return s->nextS;
}
Boolean get_ft(Step *s){
	return s->ft;
}
void set_ft(Step *s, Boolean b){
	s->ft = b;
}
Step *getPrevStep(Step *s){
	return s->prevS;
}
void setNodeTest(Step *s,NodeTest *n){
	s->nt = n;
}
void setPredicate(Step *s,Predicate *p1){
	s->p = p1;
}
Boolean eval_s(Step *s,VTDNav *vn){
	return eval_nt(s->nt,vn) && evalPredicates(s,vn);
}
Boolean eval_s2(Step *s,VTDNav *vn, Predicate *p){
	return eval_nt(s->nt,vn) && evalPredicates2(s,vn,p);
}
Boolean evalPredicates(Step *s,VTDNav *vn){
	Predicate *temp = s->p;
	while(temp!=NULL) {
		if (eval_p(temp,vn)== FALSE)
			return FALSE;
		temp = temp->nextP;
	}
	return TRUE;
}
Boolean evalPredicates2(Step *s,VTDNav *vn, Predicate *p){
	Predicate *temp = s->p;
	while(temp!=p) {
		if (eval_p(temp,vn)== FALSE)
			return FALSE;
		temp = temp->nextP;
	}
	return TRUE;
}
void setAxisType(Step *s,axisType st){
	s->axis_type = st;

}
void toString_s(Step *s, UCSChar *string){
	//String s;

	if (s->p == NULL){
		wprintf(axisName(s->axis_type));
	    toString_nt(s->nt,string);
	}
	else {
		wprintf(axisName(s->axis_type));
		toString_nt(s->nt,string);
		wprintf(L" ");
		toString_p(s->p, string);
	}
	if (s->nextS == NULL)
		return;
	else {
		wprintf(L"/");
		toString_s(s->nextS, string);
	}
}


locationPathExpr *createLocationPathExpr(){
	exception e;
	locationPathExpr *lpe = malloc(sizeof(locationPathExpr));
	if (lpe==NULL){
		throwException2(out_of_mem,
			"locationPathExpr allocation failed ");
		return NULL;
	}
	Try{
		lpe->ih = createIntHash();
	}Catch(e){
		free(lpe);
		Throw e;
	}
	lpe->freeExpr = (free_Expr) &freeLocationPathExpr;
	lpe->evalBoolean = (eval_Boolean)&evalBoolean_lpe;
	lpe->evalNodeSet = (eval_NodeSet)&evalNodeSet_lpe;
	lpe->evalNumber  = (eval_Number)&evalNumber_lpe;
	lpe->evalString  = (eval_String)&evalString_lpe;
	lpe->isNumerical = (is_Numerical)&isNumerical_lpe;
	lpe->isBoolean = (is_Boolean)&isBoolean_lpe;
	lpe->isString =  (is_String)&isString_lpe;
	lpe->isNodeSet = (is_NodeSet)&isNodeSet_lpe;
	lpe->requireContextSize = (require_ContextSize)&requireContextSize_lpe;
	lpe->setContextSize = (set_ContextSize)&setContextSize_lpe;
	lpe->setPosition = (set_Position)&setPosition_lpe;
	lpe->reset = (reset_)&reset_lpe;
	lpe->toString = (to_String)&toString_lpe;
	lpe->adjust = (adjust_)&adjust_lpe;

	lpe->state = XPATH_EVAL_START;
	lpe->s = NULL;
	lpe->pathType = RELATIVE_PATH;
	lpe->currentStep = NULL;
	return lpe;
}

void freeLocationPathExpr(locationPathExpr *lpe){
	Step *tmp, *tmp2;
	if (lpe ==NULL) return;
	if (lpe->s != NULL){
		tmp = lpe->s;
		tmp2 = tmp->nextS;
		while(tmp2!=NULL){
			freeStep(tmp);
			tmp= tmp2;
			tmp2 = tmp2->nextS;
		}
		freeStep(tmp);
	}
	freeIntHash(lpe->ih);
	free(lpe);
}

int	evalNodeSet_lpe (locationPathExpr *lpe,VTDNav *vn){
    int result;
	if (lpe->currentStep == NULL) {
		if ( lpe->pathType ==  ABSOLUTE_PATH){
			toElement(vn,ROOT);
			toElement(vn,PARENT);
		}
		lpe->currentStep =  lpe->s;
		if (lpe->currentStep == NULL){
			if (  lpe->state ==  XPATH_EVAL_START){
				 lpe->state = XPATH_EVAL_END;
				return 0;
			}
			else{
				return -1;
			}
		}
	}

	while (TRUE) {
		switch (lpe->currentStep->axis_type) {
			case AXIS_CHILD:
			    if ( (result = process_child(lpe,vn))!=-2)
				   return result;
			    break;
			case AXIS_DESCENDANT_OR_SELF:
			case AXIS_DESCENDANT:
			case AXIS_PRECEDING:
			case AXIS_FOLLOWING:
			    if ((result = process_DDFP(lpe,vn))!= -2)
			        return result;
			    break;
			case AXIS_PARENT:
			    if ((result = process_parent(lpe, vn))!= -2)
			        return result;
			    break;
			case AXIS_ANCESTOR:
			    if ((result = process_ancestor(lpe,vn))!= -2)
			        return result;
			    break;
			case AXIS_ANCESTOR_OR_SELF:
			    if ((result = process_ancestor_or_self(lpe,vn))!= -2)
			        return result;
			    break;
			case AXIS_SELF:
			    if ((result = process_self(lpe,vn))!= -2)
			        return result;
			    break;
			case AXIS_FOLLOWING_SIBLING:
			    if ((result = process_following_sibling(lpe,vn))!= -2)
			        return result;
			    break;
			case AXIS_PRECEDING_SIBLING:
			    if ((result = process_preceding_sibling(lpe, vn))!= -2)
			        return result;
			    break;
			case AXIS_ATTRIBUTE:
			    if ((result = process_attribute(lpe,vn))!= -2)
			        return result;
			    break;
			case AXIS_NAMESPACE:
			    if ((result = process_namespace(lpe,vn))!= -2)
			        return result;
				break;
			default:
				throwException2(xpath_eval_exception,
					"axis not supported");
			}
		}
}

double	evalNumber_lpe (locationPathExpr *lpe,VTDNav *vn){
	double d = 0.0;
	exception e;
	int a = getStringIndex((expr *)lpe,vn);
	Try{
		if (a!=-1) return parseDouble(vn,a);
	}Catch (e){
	}
	return 0/d;
}

UCSChar* evalString_lpe  (locationPathExpr *lpe,VTDNav *vn){
	exception e;
	int a = getStringIndex((expr *)lpe,vn);
	Try {
		if (a != -1)
			return toString(vn,a);
	} Catch (e) {
		if(e.et == out_of_mem)
			Throw e;
	}
	return createEmptyString();
}

Boolean evalBoolean_lpe (locationPathExpr *lpe,VTDNav *vn){
	exception e;
	Boolean a = FALSE;
	int size;
	push2(vn);
	// record stack size
	size = vn->contextBuf2->size;
	Try{
		a = (evalNodeSet_lpe(lpe,vn) != -1);
	}Catch (e){
	}
	//rewind stack
	vn->contextBuf2->size = size;
	reset_lpe(lpe,vn);
	pop2(vn);
	return a;
}

Boolean isBoolean_lpe (locationPathExpr *lpe){
	return FALSE;
}

Boolean isNumerical_lpe (locationPathExpr *lpe){
	return FALSE;
}

Boolean isString_lpe (locationPathExpr *lpe){
	return FALSE;
}

Boolean isNodeSet_lpe (locationPathExpr *lpe){
	return TRUE;
}

Boolean requireContextSize_lpe(locationPathExpr *lpe){
	return FALSE;
}
void reset_lpe(locationPathExpr *lpe, VTDNav *vn){
	Step *temp = lpe->s;
	lpe->state = XPATH_EVAL_START;
	resetIntHash(lpe->ih);
	lpe->currentStep = NULL;
	while(temp!=NULL){
		reset_s(temp,vn);
		temp = temp->nextS;
	}
}
void	setContextSize_lpe(locationPathExpr *lpe,int s){
}
void	setPosition_lpe(locationPathExpr *lpe,int pos){
}
void    toString_lpe(locationPathExpr *lpe, UCSChar* string){

	Step *ts = lpe->s;
	if (lpe->pathType == ABSOLUTE_PATH){
		wprintf(L"/");
	}
	if (ts == NULL)
		return;
	else
		toString_s(ts,string);
}

void setStep(locationPathExpr *lpe, Step* st){
	lpe->s = st;
}

int adjust_lpe(locationPathExpr *lpe, int n){
	int i;
	if (lpe->pathType == RELATIVE_PATH){
		i= min(6,determineHashWidth(n));//hashwidth 64
	} else {
		i=determineHashWidth(n);
	}
	if (lpe->ih!=NULL && i==lpe->ih->e)
	{}else {
		freeIntHash(lpe->ih);
		lpe->ih = createIntHash2(i);
	}
	return i;
}