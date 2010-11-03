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
#include "locationPathExpr.h"
#include "autoPilot.h"
#include "textIter.h"

using namespace com_ximpleware;

LocationPathExpr::LocationPathExpr() try : 
pathType(RELATIVE_PATH),
s(NULL),
currentStep(NULL),
state(XPATH_EVAL_START),
ih(new IntHash())
{
}
catch (std::bad_alloc&){
	throw OutOfMemException("Allocation failed for LocationPathExpr");
}

LocationPathExpr::~LocationPathExpr(){
	Step *tmp, *tmp2;
	if (s != NULL){
		tmp = s;
		tmp2 = tmp->nextS;
		while(tmp2!=NULL){
			delete(tmp);
			tmp= tmp2;
			tmp2 = tmp2->nextS;
		}
		delete(tmp);
	}
	delete(ih);
}

bool LocationPathExpr::evalBoolean(VTDNav *vn){
	bool a = false;
	int size;
	vn->push2();
	// record stack size
	size = vn->contextBuf2->size;
	try{
		a = (evalNodeSet(vn) != -1);
	}catch (...){
	}
	//rewind stack
	vn->contextBuf2->size = size;
	reset(vn);
	vn->pop2();
	return a;
}
double LocationPathExpr::evalNumber(VTDNav *vn){
	double d = 0.0;
	int a = getStringIndex(vn);
	try{
		if (a!=-1) return vn->parseDouble(a);
	}catch (...){
	}
	return 0/d;
}
int LocationPathExpr::evalNodeSet(VTDNav *vn){
    int result;
	if (currentStep == NULL) {
		if ( pathType ==  ABSOLUTE_PATH){
			vn->toElement(ROOT);
			vn->toElement(PARENT);
		}
		currentStep =  s;
		if (currentStep == NULL){
			if (state ==  XPATH_EVAL_START){
				state = XPATH_EVAL_END;
				return 0;
			}
			else{
				return -1;
			}
		}
	}

	while (true) {
		switch (currentStep->axis_type) {
			case AXIS_CHILD:
			    if ( (result = process_child(vn))!=-2)
				   return result;
			    break;
			case AXIS_DESCENDANT_OR_SELF:
			case AXIS_DESCENDANT:
			case AXIS_PRECEDING:
			case AXIS_FOLLOWING:
			    if ((result = process_DDFP(vn))!= -2)
			        return result;
			    break;
			case AXIS_PARENT:
			    if ((result = process_parent(vn))!= -2)
			        return result;
			    break;
			case AXIS_ANCESTOR:
			    if ((result = process_ancestor(vn))!= -2)
			        return result;
			    break;
			case AXIS_ANCESTOR_OR_SELF:
			    if ((result = process_ancestor_or_self(vn))!= -2)
			        return result;
			    break;
			case AXIS_SELF:
			    if ((result = process_self(vn))!= -2)
			        return result;
			    break;
			case AXIS_FOLLOWING_SIBLING:
			    if ((result = process_following_sibling(vn))!= -2)
			        return result;
			    break;
			case AXIS_PRECEDING_SIBLING:
			    if ((result = process_preceding_sibling(vn))!= -2)
			        return result;
			    break;
			case AXIS_ATTRIBUTE:
			    if ((result = process_attribute(vn))!= -2)
			        return result;
			    break;
			case AXIS_NAMESPACE:
			    if ((result = process_namespace(vn))!= -2)
			        return result;
				break;
			default:
				throw XPathEvalException("axis not supported");
			}
		}
}
UCSChar* LocationPathExpr::evalString(VTDNav *vn){
	int a = getStringIndex(vn);
	try {
		if (a != -1)
			return vn->toString(a);
	} catch (std::bad_alloc&) {
		throw;
	}
	return createEmptyString();
}

void LocationPathExpr::reset(VTDNav *vn){}
void LocationPathExpr::toString(UCSChar *string){
	Step *ts = s;
	if (pathType == ABSOLUTE_PATH){
		wprintf(L"/");
	}
	if (ts == NULL)
		return;
	else
		ts->toString_s(string);
}

bool LocationPathExpr::isNumerical(){return false;}
bool LocationPathExpr::isNodeSet(){return true;}
bool LocationPathExpr::isString(){return false;}
bool LocationPathExpr::isBoolean(){return false;}

bool LocationPathExpr::requireContextSize(){return false;}
void LocationPathExpr::setContextSize(int size){}

void LocationPathExpr::setPosition(int pos){}
int LocationPathExpr::adjust(int n){
	int i;
	if (pathType == RELATIVE_PATH){
		i= min(6,IntHash::determineHashWidth(n));//hashwidth 64
	} else {
		i=IntHash::determineHashWidth(n);
	}
	if (ih!=NULL && i==ih->e)
	{}else {
		delete (ih);
		ih =  new IntHash(i);
	}
	return i;
}


int LocationPathExpr::computeContextSize(Predicate *p, VTDNav *vn){
	bool b = false;
	//Predicate *tp = NULL;
    int i = 0;
    AutoPilot *ap = (AutoPilot *)currentStep->o;
	UCSChar *helper = NULL;
	switch(currentStep->axis_type){
    	case AXIS_CHILD:
			if (currentStep->nt->testType < NT_TEXT){
    			b = vn->toElement(FIRST_CHILD);
    			if (b) {
    				do {
						if (currentStep->eval_s2(vn, p)) {
							i++;
    					}
    				} while (vn->toElement(NEXT_SIBLING));
    				vn->toElement(PARENT);
					currentStep->resetP2_s(vn,p);
    				return i;
   				} else
    				return 0;
			}else {
				int result;
				TextIter* ti = new TextIter();
	    	    ti->touch(vn);
	    	    selectNodeType(ti);
	    	    while((result=ti->getNext())!=-1){
					vn->LN = result;
					vn->atTerminal = true;
	    	        if (currentStep->evalPredicates2(vn,p)){
	    	            i++;
	    	        }
	    	    }
				vn->atTerminal = false;
				currentStep->resetP2_s(vn,p);
	    	    return i;
			}

		case AXIS_DESCENDANT_OR_SELF:
		case AXIS_DESCENDANT:
		case AXIS_PRECEDING:
		case AXIS_FOLLOWING:

			if (currentStep->nt->testType == NT_NODE){
				helper = (UCSChar *)L"*";
			}else if (currentStep->nt->testType == NT_NAMETEST){
				helper = currentStep->nt->nodeName;
			}else    			
				throw XPathEvalException(
				   "can't run descendant following, or following-sibling axis over comment(), pi(), and text()");
			if (ap==NULL)
					ap =  new AutoPilot(vn);
				else
					ap->bind(vn);
			if (currentStep->axis_type == AXIS_DESCENDANT_OR_SELF ){
				if (currentStep->nt->testType == NT_NODE)
					ap->setSpecial(true);
				else
					ap->setSpecial(false);
			}
			//currentStep.o = ap = new AutoPilot(vn);
			if (currentStep->axis_type == AXIS_DESCENDANT_OR_SELF)
				if (currentStep->nt->localName!=NULL)
					ap->selectElementNS(currentStep->nt->URL,currentStep->nt->localName);
				else
					ap->selectElement(helper);
			else if (currentStep->axis_type == AXIS_DESCENDANT)
				if (currentStep->nt->localName!=NULL)
					ap->selectElementNS_D(currentStep->nt->URL,currentStep->nt->localName);
				else
					ap->selectElement_D(helper);
			else if (currentStep->axis_type == AXIS_PRECEDING)
				if (currentStep->nt->localName!=NULL)
					ap->selectElementNS_P(currentStep->nt->URL,currentStep->nt->localName);
				else
					ap->selectElement_P(helper);
			else
				if (currentStep->nt->localName!=NULL)
					ap->selectElementNS_F(currentStep->nt->URL,currentStep->nt->localName);
				else
					ap->selectElement_F(helper);
			vn->push2();
			while(ap->iterate()){
				if (currentStep->evalPredicates2(vn,p)){
					i++;
				}
			}
			vn->pop2();
			currentStep->resetP2_s(vn,p);
			currentStep->o= ap;
			//freeAutoPilot(ap);
			return i;

		case AXIS_PARENT:
			vn->push2();
			i = 0;
			if (vn->toElement( PARENT)){
				if (currentStep->eval_s2(vn,p)){
					i++;
				}
			}
			vn->pop2();
			currentStep->resetP2_s(vn,p);
			return i;

		case AXIS_ANCESTOR:
			vn->push2();
			i = 0;
			while (vn->toElement(PARENT)) {
				if (currentStep->eval_s2(vn, p)) {
					i++;
				}
			}
			vn->pop2();
			currentStep->resetP2_s(vn,p);
			return i;

		case AXIS_ANCESTOR_OR_SELF:
			vn->push2();
			i = 0;
			do {
				if (currentStep->eval_s2(vn, p)) {
					i++;
				}
			}while(vn->toElement(PARENT));
			vn->pop2();
			currentStep->resetP2_s(vn,p);
			return i;

		case AXIS_SELF:
			i = 0;
			if (vn->toElement(PARENT)){
				if (currentStep->eval_s2(vn,p)){
					i++;
				}
			}
			currentStep->resetP2_s(vn,p);
			return i;

		case AXIS_FOLLOWING_SIBLING:
			vn->push2();
			while(vn->toElement(NEXT_SIBLING)){
				if (currentStep->eval_s2(vn,p)){
					i++;
				}
			}
			vn->pop2();
			currentStep->resetP2_s(vn,p);
			return i;

		case AXIS_PRECEDING_SIBLING:
			vn->push2();
			while(vn->toElement(PREV_SIBLING)){
				if (currentStep->eval_s2(vn,p)){
					i++;
				}
			}
			vn->pop2();
			currentStep->resetP2_s(vn,p);
			return i;

		case AXIS_ATTRIBUTE:
			if (ap==NULL)
					ap =  new AutoPilot(vn);
				else
					ap->bind(vn);
			//ap = createAutoPilot(vn);
			if (currentStep->nt->testType == NT_NODE)
				ap->selectAttr((UCSChar *)L"*");
			else if (currentStep->nt->localName!=NULL)
				ap->selectAttrNS(currentStep->nt->URL,
				currentStep->nt->localName);
			else
				ap->selectAttr(currentStep->nt->nodeName);
			i = 0;
			while(ap->iterateAttr()!=-1){
				if (currentStep->evalPredicates2( vn, p)){
					i++;
				}
			}
			currentStep->resetP2_s(vn,p);
			currentStep->o= ap;
			//freeAutoPilot(ap);
			return i;

		case AXIS_NAMESPACE:
			if (ap==NULL)
					ap = new AutoPilot(vn);
				else
					ap->bind(vn);
			if (currentStep->nt->testType == NT_NODE)
				ap->selectNameSpace((UCSChar *)L"*");
			else
				ap->selectNameSpace(currentStep->nt->nodeName);
			i = 0;
			while(ap->iterateNameSpace()!=-1){
				if (currentStep->evalPredicates2( vn, p)){
					i++;
				}
			}
			currentStep->resetP2_s(vn,p);
			currentStep->o= ap;
			//freeAutoPilot(ap);
			return i;

		default:
			throw XPathEvalException("unknown state");
			return 0;
	}
}
int LocationPathExpr::process_ancestor_or_self(VTDNav *vn){
	bool b = false, b1= false;
	int result;
	//int contextSize;
	Predicate *t= NULL;

	switch ( state) {
		case  XPATH_EVAL_START:
			t = currentStep->p;
			while (t != NULL) {
				if (t->requireContextSize_p()) {
					int i = computeContextSize(t, vn);
					if (i == 0) {
						b1 = true;
						break;
					} else
						t->setContextSize_p(i);
				}
				t = t->nextP;
			}
			if (b1) {
				state = XPATH_EVAL_END;
				break;
			}

			state =  XPATH_EVAL_END;
			vn->push2();

			if (currentStep->get_ft()== true){
				currentStep->set_ft(false);
				if (currentStep->eval_s(vn)) {
					if (currentStep->getNextStep() != NULL) {
						state =  XPATH_EVAL_FORWARD;
						currentStep = currentStep->nextS;
						break;
					} else {
						//vn.pop();
						state =  XPATH_EVAL_TERMINAL;
						if (vn->atTerminal)
							result = vn->LN;
						else
							result = vn->getCurrentIndex();
						if ( isUnique(result))
							return result;
					}
				}
			}

			while (vn->toElement(PARENT)) {
				if (currentStep->eval_s(vn)) {
					if (currentStep->nextS != NULL) {
						state =  XPATH_EVAL_FORWARD;
						currentStep = currentStep->nextS;
						break;
					} else {
						//vn.pop();
						state =  XPATH_EVAL_TERMINAL;
						result = vn->getCurrentIndex();
						if ( isUnique(result))
							return result;
					}
				}
			}

			if ( state ==  XPATH_EVAL_END) {
				currentStep->resetP_s( vn);
				vn->pop2();
			}

			break;

		case  XPATH_EVAL_FORWARD:
			t = currentStep->p;
			while (t != NULL) {
				if (t->requireContextSize_p()) {
					int i = computeContextSize(t, vn);
					if (i == 0) {
						b1 = true;
						break;
					} else
						t->setContextSize_p(i);
				}
				t = t->nextP;
			}
			if (b1) {
				currentStep = currentStep->prevS;
				state = XPATH_EVAL_BACKWARD;
				break;
			}

			state =  XPATH_EVAL_BACKWARD;
			vn->push2();
			if (currentStep->ft == true) {
				currentStep->ft= false;
				if (currentStep->eval_s(vn)) {
					if (currentStep->nextS != NULL) {
						state =  XPATH_EVAL_FORWARD;
						currentStep = currentStep->nextS;
						break;
					} else {
						//vn.pop();
						state =  XPATH_EVAL_TERMINAL;
						if (vn->atTerminal)
							result = vn->LN;
						else
							result = vn->getCurrentIndex();
						if ( isUnique(result))
							return result;
					}
				}
			}
			while (vn->toElement(PARENT)) {
				if (currentStep->eval_s(vn)) {
					if (currentStep->nextS != NULL) {
						state =  XPATH_EVAL_FORWARD;
						currentStep = currentStep->nextS;
						break;
					} else {
						//vn.pop();
						state =  XPATH_EVAL_TERMINAL;
						result = vn->getCurrentIndex();
						if ( isUnique(result))
							return result;
					}
				}
			}

			if ( state ==  XPATH_EVAL_BACKWARD) {
				currentStep->resetP_s(vn);
				currentStep->ft = true;
				vn->pop2();
				currentStep = currentStep->prevS;
			}
			break;

		case  XPATH_EVAL_END:
			currentStep = NULL;
			// reset();
	    	return -1;


		case  XPATH_EVAL_BACKWARD:
			b = false;
			vn->push2();

			while (vn->toElement(PARENT)) {
				if (currentStep->eval_s(vn)) {
					if (currentStep->nextS != NULL) {
						state =  XPATH_EVAL_FORWARD;
						currentStep = currentStep->nextS;
						b = true;
						break;
					} else {
						//vn.pop();
						state =  XPATH_EVAL_TERMINAL;
						result = vn->getCurrentIndex();
						if ( isUnique(result))
							return result;
					}
				}
			}
			if (b == false) {
				vn->pop2();
				currentStep->resetP_s(vn);
				if (currentStep->prevS != NULL) {
					currentStep->ft = true;
					state =  XPATH_EVAL_BACKWARD;
					currentStep = currentStep->prevS;
				} else {
					state =  XPATH_EVAL_END;
				}
			}
			break;

		case  XPATH_EVAL_TERMINAL:
			while (vn->toElement(PARENT)) {
				if (currentStep->eval_s(vn)) {
					result = vn->getCurrentIndex();
					if ( isUnique(result))
						return result;
				}
			}
			vn->pop2();
			currentStep->resetP_s(vn);
			if (currentStep->prevS != NULL) {
				currentStep->ft = true;
				state =  XPATH_EVAL_BACKWARD;
				currentStep = currentStep->prevS;
			}
			else {
				 state =  XPATH_EVAL_END;
			}
			break;


		default:
			throw XPathEvalException("unknown state");
		}
		return -2;
}
int LocationPathExpr::process_ancestor(VTDNav *vn){
	int result;
	bool b = false, b1 = false;
	//int contextSize;
	Predicate *t= NULL;

	switch(state){
			case XPATH_EVAL_START:
				t = currentStep->p;
				while (t != NULL) {
					if (t->requireContextSize_p()) {
						int i = computeContextSize(t, vn);
						if (i == 0) {
							b1 = true;
							break;
						} else
							t->setContextSize_p(i);
					}
					t = t->nextP;
				}
				if (b1) {
					state = XPATH_EVAL_END;
					break;
				}

				state = XPATH_EVAL_END;
				if (vn->getCurrentDepth() != -1) {
					vn->push2();

					while (vn->toElement(PARENT)) {
						if (currentStep->eval_s(vn)) {
							if (currentStep->nextS != NULL) {
								state = XPATH_EVAL_FORWARD;
								currentStep = currentStep->nextS;
								break;
							} else {
								//vn.pop();
								state = XPATH_EVAL_TERMINAL;
								result = vn->getCurrentIndex();
								if (isUnique(result))
									return result;
							}
						}
					}
					if (state == XPATH_EVAL_END) {
						currentStep->resetP_s(vn);
						vn->pop2();
					}
				}
				break;

			case XPATH_EVAL_END:
				currentStep =NULL;
				// reset();
				return -1;

			case XPATH_EVAL_FORWARD:
				t = currentStep->p;
				while(t!=NULL){
					if (t->requireContextSize_p()){
						int i = computeContextSize(t,vn);
						if (i==0){
							b1 = true;
							break;
						}else
							t->setContextSize_p(i);
					}
					t = t->nextP;
				}
				if (b1){
					currentStep = currentStep->prevS;
					state = XPATH_EVAL_BACKWARD;
					break;
				}
				state =  XPATH_EVAL_BACKWARD;
				vn->push2();

				while(vn->toElement(PARENT)){
					if (currentStep->eval_s(vn)){
						if (currentStep->nextS != NULL){
							state =  XPATH_EVAL_FORWARD;
							currentStep = currentStep->nextS;
							break;
						}
						else {
							//vn.pop();
							state =  XPATH_EVAL_TERMINAL;
							result = vn->getCurrentIndex();
							if ( isUnique(result))
								return result;
						}
					}
				}
				if ( state== XPATH_EVAL_BACKWARD){
					currentStep->resetP_s(vn);
					vn->pop2();
					currentStep=currentStep->prevS;
				}
				break;

			case XPATH_EVAL_BACKWARD:
				b = false;
				vn->push2();

				while (vn->toElement(PARENT)) {
					if (currentStep->eval_s(vn)) {
						if (currentStep->nextS!= NULL) {
							state =  XPATH_EVAL_FORWARD;
							currentStep = currentStep->nextS;
							b = true;
							break;
						} else {
							//vn.pop();
							state =  XPATH_EVAL_TERMINAL;
							result = vn->getCurrentIndex();
							if ( isUnique(result))
								return result;
						}
					}
				}
				if (b==false){
					vn->pop2();
					if (currentStep->prevS!=NULL) {
						currentStep->resetP_s(vn);
						state =  XPATH_EVAL_BACKWARD;
						currentStep = currentStep->prevS;
					}
					else {
						state =  XPATH_EVAL_END;
					}
				}
				break;

			case XPATH_EVAL_TERMINAL:
				while (vn->toElement(PARENT)) {
					if (currentStep->eval_s(vn)) {
						result = vn->getCurrentIndex();
						if ( isUnique(result))
							return result;
					}
				}
				vn->pop2();

				if (currentStep->prevS!=NULL) {
					currentStep->resetP_s(vn);
					state =  XPATH_EVAL_BACKWARD;
					currentStep = currentStep->prevS;
				}
				else {
					state =  XPATH_EVAL_END;
				}
				break;

			default:
				throw XPathEvalException("unknown state");
	}
	return -2;
}
int LocationPathExpr::process_attribute(VTDNav *vn){
	AutoPilot *ap = NULL;
	bool b1 = false;
	Predicate *t= NULL;
	int temp;
	switch(state){
		case  XPATH_EVAL_START:
		case  XPATH_EVAL_FORWARD:

			t = currentStep->p;
			while(t!=NULL){
				if (t->requireContextSize_p()){
					int i = computeContextSize(t,vn);
					if (i==0){
						b1 = true;
						break;
					}else
						t->setContextSize_p(i);
				}
				t = t->nextP;
			}
			if (b1){
				if (state == XPATH_EVAL_FORWARD){
					state= XPATH_EVAL_BACKWARD;
					currentStep = currentStep->prevS;
				}else
					state= XPATH_EVAL_END;
				break;
			}

			if (vn->getAtTerminal()==true){
				if (state ==XPATH_EVAL_START)
					state = XPATH_EVAL_END;
				else {
					state = XPATH_EVAL_BACKWARD;
					currentStep  = currentStep->prevS;
				}
			} else {
				if (currentStep->ft == true) {
					if (currentStep->o == NULL)
						currentStep->o = ap = new AutoPilot(vn);
					else {
						ap = currentStep->o;
						ap->bind(vn);
					}
					if (currentStep->nt->testType== NT_NODE)
						ap->selectAttr((UCSChar *)L"*");
					else if (currentStep->nt->localName != NULL)
						ap->selectAttrNS(currentStep->nt->URL,
                                currentStep->nt->localName);
					else 
						ap->selectAttr(currentStep->nt->nodeName);
					currentStep->ft = false;
				}
				if ( state==  XPATH_EVAL_START)
					state=  XPATH_EVAL_END;
				vn->setAtTerminal(true);
				while( (temp = ap->iterateAttr()) != -1){
					if (currentStep->evalPredicates(vn)){
						break;
					}
				}
				if (temp == -1){
					currentStep->ft = true;
					currentStep->resetP_s(vn);
					vn->setAtTerminal(false);
					if ( state==  XPATH_EVAL_FORWARD){
						state =  XPATH_EVAL_BACKWARD;
						currentStep = currentStep->prevS;
					}
				}else {

					if (currentStep->nextS != NULL){
						vn->LN = temp;
						state=  XPATH_EVAL_FORWARD;
						currentStep = currentStep->nextS;
					}
					else {
						//vn.pop();
						state=  XPATH_EVAL_TERMINAL;
						if ( isUnique(temp)){
							vn->LN = temp;
							return temp;
						}
					}

				}
			}
			break;

		case  XPATH_EVAL_END:
			currentStep = NULL;
			// reset();
			return -1;

		case  XPATH_EVAL_BACKWARD:
			ap = currentStep->o;
			//vn.push();
			while( (temp = ap->iterateAttr()) != -1){
				if (currentStep->evalPredicates(vn)){
					break;
				}
			}
			if (temp == -1) {
				currentStep->ft = true;
				//freeAutoPilot(currentStep->o);
				//currentStep->o = NULL;
				currentStep->resetP_s(vn);
				vn->setAtTerminal(false);
				if (currentStep->prevS != NULL) {
					state =  XPATH_EVAL_BACKWARD;
					currentStep = currentStep->prevS;
				} else
					state =  XPATH_EVAL_END;
			} else {
				if (currentStep->nextS != NULL) {
					state =  XPATH_EVAL_FORWARD;
					currentStep = currentStep->nextS;
				} else {
					state =  XPATH_EVAL_TERMINAL;
					if ( isUnique(temp)){
						vn->LN = temp;
						return temp;
					}
				}
			}
			break;

		case  XPATH_EVAL_TERMINAL:
			ap = currentStep->o;
			while( (temp = ap->iterateAttr()) != -1){
				if (currentStep->evalPredicates(vn)){
					break;
				}
			}
			if (temp != -1)
				if (isUnique(temp)){
					vn->LN = temp;
					return temp;
				}
				vn->setAtTerminal(false);
				currentStep->resetP_s(vn);
				if (currentStep->prevS == NULL) {
					currentStep->ft = true;
					//freeAutoPilot(currentStep->o);
					//currentStep->o = NULL;
					state=  XPATH_EVAL_END;
				} else {
					state=  XPATH_EVAL_BACKWARD;
					currentStep->ft = true;
					//freeAutoPilot(currentStep->o);
					//currentStep->o = NULL;
					currentStep = currentStep->prevS;
				}

				break;

		default:
			throw XPathEvalException("unknown state");
	}
	return -2;
}
int LocationPathExpr::process_child(VTDNav *vn){
	int result;
	bool b = false, b1 = false;
	Predicate *t= NULL;

	switch(state){
				case XPATH_EVAL_START:
					if (currentStep->nt->testType < NT_TEXT){
						/* first search for any predicate that
						// requires contextSize
						// if so, compute its context size
						// if size > 0
						// set context
						// if size ==0
						// immediately set the state to backward or end*/
						t = currentStep->p;
						while(t!=NULL){
							if (t->requireContextSize_p()){
								int i = computeContextSize(t,vn);
								if (i==0){
									b1 = true;
									break;
								}else
									t->setContextSize_p(i);
							}
							t = t->nextP;
						}
						if (b1){
							state= XPATH_EVAL_END;
							break;
						}

						b=vn->toElement(FIRST_CHILD);
						state=  XPATH_EVAL_END;
						if (b == true){
						 do {
							 if (currentStep->eval_s(vn)) {
								 if (currentStep->nextS != NULL){
									 //currentStep.position++;
									 state=  XPATH_EVAL_FORWARD;
									 currentStep = currentStep->nextS;
								 }
								 else {
									 state=  XPATH_EVAL_TERMINAL;
									 result = vn->getCurrentIndex();
									 if ( isUnique(result)){
										 return result;
									 }
								 }
								 break;
							 }
						 } while (vn->toElement(NEXT_SIBLING));
						 if (state == XPATH_EVAL_END)
							 vn->toElement(PARENT);
					 }
					}else {
						TextIter *ti = NULL;
						if (vn->getAtTerminal()==true){
							state = XPATH_EVAL_END;
						}else {
						    // compute context size;
						    t = currentStep->p;
			    	        while(t!=NULL){
			    	            if (t->requireContextSize_p()){
			    	                int i = computeContextSize(t,vn);
			    	                if (i==0){
			    	                    b1 = true;
			    	                    break;
			    	                }else
			    	                    t->setContextSize_p(i);
			    	            }
			    	            t = t->nextP;
			    	        }
			    	        // b1 false indicate context size is zero. no need to go any further...
			    	        if (b1){
			    	            state = XPATH_EVAL_END;
			    	            break;
			    	        }
			    	        // get textIter
						    if (currentStep->o != NULL){
						        ti = (TextIter*) currentStep->o;
						    } else {
						        ti = new TextIter();
						        currentStep->o = (AutoPilot *)ti;
						    }
							selectNodeType(ti);
						    ti->touch(vn);
						    state = XPATH_EVAL_END;
						    while((result = ti->getNext())!=-1){
								vn->LN = result;
								vn->atTerminal = true;
								if (currentStep->evalPredicates(vn)){
									break;
								}
						    }
						    // old code
							//result = vn.getText();
							if (result != -1){
								vn->setAtTerminal(true);
								//currentStep.resetP(vn);
								vn->LN = result;
								if (currentStep->getNextStep() != NULL){
								    vn->LN = result;
				   				    state =  XPATH_EVAL_FORWARD;
									currentStep = currentStep->getNextStep();
								}
								else {
									//vn.pop();
									state =  XPATH_EVAL_TERMINAL;
									if (isUnique(result)){
									    vn->LN = result;
										return result;
									}
								}
							} else{
								//currentStep.set_ft(true);
								currentStep->resetP_s(vn);
								vn->setAtTerminal( false);
							}
						}
					}
					break;
				case XPATH_EVAL_END:
					currentStep =NULL;
					// reset();
					return -1;

				case XPATH_EVAL_FORWARD:
					if (currentStep->nt->testType < NT_TEXT){
						t = currentStep->p;
						while(t!=NULL){
							if (t->requireContextSize_p()){
								int i = computeContextSize( t,vn);
								if (i==0){
									b1 = true;
									break;
								}else
									t->setContextSize_p(i);
							}
							t = t->nextP;
						}
						if (b1){
							currentStep = currentStep->prevS;
							state= XPATH_EVAL_BACKWARD;
							break;
						}

						state =  XPATH_EVAL_BACKWARD;
						if (vn->toElement(FIRST_CHILD)) {
							do {
								if (currentStep->eval_s(vn)) {
									if (currentStep->nextS != NULL) {
										state=  XPATH_EVAL_FORWARD;
										currentStep = currentStep->nextS;
									} else {
										state=  XPATH_EVAL_TERMINAL;
										result = vn->getCurrentIndex();
										if ( isUnique(result))
											return result;
									}
									goto forward;
								}
							} while (vn->toElement(NEXT_SIBLING));
							vn->toElement(PARENT);
							currentStep->resetP_s(vn);
							currentStep = currentStep->prevS;
						} else {
							//vn.toElement(VTDNav.P);
							currentStep = currentStep->prevS;
						}
forward:;
					}else {
						TextIter *ti = NULL;

 // predicate at an attribute is not evaled
						if (vn->getAtTerminal() == true){
							state =  XPATH_EVAL_BACKWARD;
							currentStep = currentStep->getPrevStep();
						}else {
						    // compute context size;
						    t = currentStep->p;
			    	        while(t!=NULL){
			    	            if (t->requireContextSize_p()){
			    	                int i = computeContextSize(t,vn);
			    	                if (i==0){
			    	                    b1 = true;
			    	                    break;
			    	                }else
			    	                    t->setContextSize_p(i);
			    	            }
			    	            t = t->nextP;
			    	        }
			    	        // b1 false indicate context size is zero. no need to go any further...
			    	        if (b1){
			    	            state =  XPATH_EVAL_BACKWARD;
			    	            break;
			    	        }
			    	        // get textIter
						    if (currentStep->o != NULL){
						        ti = (TextIter*) currentStep->o;
						    } else {
						        ti = new TextIter();
						        currentStep->o = (AutoPilot *)ti;
						    }
						    ti->touch(vn);
							selectNodeType(ti);
						    //result = ti.getNext();

						    while((result = ti->getNext())!=-1){
								vn->LN = result;
								vn->atTerminal = true;
								if (currentStep->evalPredicates(vn)){
									break;
								}
						    }

			                if (result == -1) {
			                    //currentStep.set_ft(true);
			                    //currentStep.resetP(vn);
			                    vn->setAtTerminal( false);
			                    if (state ==  XPATH_EVAL_FORWARD) {
			                        state =  XPATH_EVAL_BACKWARD;
									currentStep = currentStep->getPrevStep();
			                    }
			                } else {
								vn->setAtTerminal( true);
			                    if (currentStep->getNextStep() != NULL) {
			                        vn->LN = result;
			                        state =  XPATH_EVAL_FORWARD;
			                        currentStep = currentStep->getNextStep();
			                    } else {
			                        //vn.pop();
			                        state =  XPATH_EVAL_TERMINAL;
			                        if (isUnique(result)) {
			                            vn->LN = result;
			                            return result;
			                        }
			                    }
			                }
						}

					}

					break;

				case XPATH_EVAL_BACKWARD:
					if (currentStep->nt->testType < NT_TEXT) {
						b = false;
						while (vn->toElement(NEXT_SIBLING)) {
							if (currentStep->eval_s(vn)) {
								b = true;
								break;
							}
						}
						if (b == true) {
							state=  XPATH_EVAL_FORWARD;
							currentStep = currentStep->nextS;
						} else if (currentStep->prevS == NULL){
							currentStep->resetP_s(vn);
							vn->toElement(PARENT);
							state=  XPATH_EVAL_END;
						}
						else {
							currentStep->resetP_s(vn);
							state=  XPATH_EVAL_BACKWARD;
							vn->toElement(PARENT);
							currentStep = currentStep->prevS;
						}
					}else {
						vn->setAtTerminal(false);
						if (currentStep->prevS == NULL)
							state=  XPATH_EVAL_END;
						else {
							state=  XPATH_EVAL_BACKWARD;
							currentStep = currentStep->prevS;
						}
					}
					break;

				case XPATH_EVAL_TERMINAL:
					if (currentStep->nt->testType < NT_TEXT) {
						while (vn->toElement(NEXT_SIBLING)) {
							if (currentStep->eval_s(vn)) {
								result = vn->getCurrentIndex();
								if ( isUnique(result))
									return result;
							}
						}
						currentStep->resetP_s(vn);
						if (currentStep->prevS == NULL){
							state=  XPATH_EVAL_END;
							vn->toElement(PARENT);
						}
						else {
							vn->toElement(PARENT);

							state=  XPATH_EVAL_BACKWARD;
							currentStep = currentStep->prevS;
						}
					}else {
						TextIter* ti = (TextIter*) currentStep->o;
					    while ((result= ti->getNext())!=-1) {
							vn->LN = result;
							vn->atTerminal = true;
							if (currentStep->evalPredicates(vn)) {
					            if ( isUnique(result))
									return result;
					        }
					    }
						currentStep->resetP_s( vn);
						vn->setAtTerminal( false);
						if (currentStep->getPrevStep() == NULL)
							 state=  XPATH_EVAL_END;
						else {
							 state=  XPATH_EVAL_BACKWARD;
							 currentStep = currentStep->getPrevStep();
						}
						///////////////////////////////
					}
					break;

				default:
					throw XPathEvalException("unknown state");
	}
	return -2;
}
int LocationPathExpr::process_DDFP(VTDNav *vn){
	AutoPilot *ap;
	bool b = false, b1 = false;
	Predicate *t= NULL;
	int result;
	UCSChar *helper;
	switch(state){
			case XPATH_EVAL_START:
			case XPATH_EVAL_FORWARD:
				if (vn->atTerminal){
					if (state == XPATH_EVAL_START)
						state= XPATH_EVAL_END;
					else {
						// no need to set_ft to true
						// no need to resetP
						state= XPATH_EVAL_BACKWARD;
						currentStep = currentStep->prevS;
					}
					break;
				}

				t = currentStep->p;
				while(t!=NULL){
					if (t->requireContextSize_p()){
						int i = computeContextSize( t,vn);
						if (i==0){
							b1 = true;
							break;
						}else
							t->setContextSize_p(i);
					}
					t = t->nextP;
				}
				if (b1){
					if (state ==XPATH_EVAL_START)
						state= XPATH_EVAL_END;
					else {
						currentStep = currentStep->prevS;
						state= XPATH_EVAL_BACKWARD;
					}
					break;
				}


				helper = NULL;
				if (currentStep->nt->testType == NT_NAMETEST){
					helper = currentStep->nt->nodeName;
				}else if (currentStep->nt->testType == NT_NODE){
					helper = (UCSChar *) L"*";
				}else
    				throw XPathEvalException(
					"can't run descendant following, or following-sibling axis over comment(), pi(), and text()"); 
				if (currentStep->o == NULL)
					currentStep->o = ap = new AutoPilot(vn);
				else{
					ap = currentStep->o;
					ap->bind(vn);
				}
				if (currentStep->ft == true) {

					if (currentStep->axis_type == AXIS_DESCENDANT_OR_SELF ){
						if (currentStep->nt->testType == NT_NODE)
							ap->setSpecial(true);
						else
							ap->setSpecial(false);
					}
					//currentStep.o = ap = createAutoPilot(vn);
					if (currentStep->axis_type == AXIS_DESCENDANT_OR_SELF)
						ap->selectElement(helper);
					else if (currentStep->axis_type == AXIS_DESCENDANT)
						ap->selectElement_D(helper);
					else if (currentStep->axis_type == AXIS_PRECEDING)
						ap->selectElement_P(helper);
					else
						ap->selectElement_F(helper);
					currentStep->ft = false;
				}
				if ( state==  XPATH_EVAL_START)
					state=  XPATH_EVAL_END;

				vn->push2(); // not the most efficient. good for now
				//System.out.println("  --++ push in //");
				b = false;
				while(ap->iterate()){
					if (currentStep->evalPredicates(vn)){
						b = true;
						break;
					}
				}
				if (b == false) {
					vn->pop2();
					//System.out.println("  --++ pop in //");
					currentStep->ft = true;
					currentStep->resetP_s(vn);
					if ( state==  XPATH_EVAL_FORWARD){
						state =  XPATH_EVAL_BACKWARD;
						currentStep = currentStep->prevS;
					}
				} else {
					if (currentStep->nextS != NULL){
						state =  XPATH_EVAL_FORWARD;
						currentStep = currentStep->nextS;
					}
					else {
						//vn.pop();
						state =  XPATH_EVAL_TERMINAL;
						result = vn->getCurrentIndex();
						if ( isUnique(result))
							return result;
					}
				}
				break;

			case XPATH_EVAL_END:
				currentStep = NULL;
				// reset();
				return -1;

			case XPATH_EVAL_BACKWARD:
				//currentStep = currentStep->prevS;
				ap = currentStep->o;
				//vn.push();
				b = false;
				while(ap->iterate()){
					if (currentStep->evalPredicates(vn)){
						b = true;
						break;
					}
				}
				if (b == false) {
					vn->pop2();
					currentStep->ft = true;
					currentStep->resetP_s(vn);
					//System.out.println("  --++ pop in //");
					if (currentStep->prevS != NULL) {
						state=  XPATH_EVAL_BACKWARD;
						currentStep = currentStep->prevS;
					} else
						state=  XPATH_EVAL_END;
				} else {
					if (currentStep->nextS != NULL) {
						//vn.push();
						//System.out.println("  --++ push in //");
						state=  XPATH_EVAL_FORWARD;
						currentStep = currentStep->nextS;
					} else {
						state=  XPATH_EVAL_TERMINAL;
						result = vn->getCurrentIndex();
						if ( isUnique(result))
							return result;
					}
				}
				break;

			case XPATH_EVAL_TERMINAL:
				ap = currentStep->o;
				b = false;
				while (ap->iterate()) {
					if (currentStep->evalPredicates(vn)) {
						b = true;
						break;
					}
				}
				if (b == true) {
					if (currentStep->evalPredicates(vn)) {
						result = vn->getCurrentIndex();
						if (isUnique(result))
							return result;
					}
				} else if (currentStep->prevS == NULL) {
					currentStep->resetP_s(vn);
					vn->pop2();
					state= XPATH_EVAL_END;
				} else {
					vn->pop2();
					currentStep->ft = true;
					currentStep->resetP_s(vn);
					//System.out.println(" --++ pop in //");
					state= XPATH_EVAL_BACKWARD;
					//currentStep.ft = true;
					currentStep = currentStep->prevS;
				}
				break;

			default:
				throw XPathEvalException(
					"unknown state");
	}
	return -2;
}
int LocationPathExpr::process_following_sibling(VTDNav *vn){

	bool b = false, b1 = false;
	//int contextSize;
	Predicate *t= NULL;
	int result;
	switch( state){
		  case  XPATH_EVAL_START:
		  case  XPATH_EVAL_FORWARD:

			  t = currentStep->p;
			  while(t!=NULL){
				  if (t->requireContextSize_p()){
					  int i = computeContextSize( t,vn);
					  if (i==0){
						  b1 = true;
						  break;
					  }else
						  t->setContextSize_p(i);
				  }
				  t = t->nextP;
			  }
			  if (b1){
				  if (state == XPATH_EVAL_FORWARD){
					  state= XPATH_EVAL_BACKWARD;
					  currentStep = currentStep->prevS;
				  }else
					  state= XPATH_EVAL_END;
				  break;
			  }
			  if ( state==  XPATH_EVAL_START)
				  state=  XPATH_EVAL_END;
			  else
				  state=  XPATH_EVAL_BACKWARD;
			  vn->push2();
			  while (vn->toElement(NEXT_SIBLING)){
				  if (currentStep->eval_s(vn)){
					  if (currentStep->nextS!=NULL){
						  state=  XPATH_EVAL_FORWARD;
						  currentStep = currentStep->nextS;
						  break;
					  } else {
						  state=  XPATH_EVAL_TERMINAL;
						  result = vn->getCurrentIndex();
						  if ( isUnique(result))
							  return result;
					  }
				  }
			  }

			  if ( state==  XPATH_EVAL_END){
				  currentStep->resetP_s(vn);
				  vn->pop2();
			  }else if ( state==  XPATH_EVAL_BACKWARD){
				  currentStep->resetP_s(vn);
				  vn->pop2();
				  currentStep = currentStep->prevS;
			  }
			  break;

		  case  XPATH_EVAL_END:
			  currentStep = NULL;
			  // reset();
			  return -1;

		  case  XPATH_EVAL_BACKWARD:
			  while (vn->toElement(NEXT_SIBLING)){
				  if (currentStep->eval_s(vn)){
					  if (currentStep->nextS!=NULL){
						  state=  XPATH_EVAL_FORWARD;
						  currentStep = currentStep->nextS;
						  b = true;
						  break;
					  } else {
						  state=  XPATH_EVAL_TERMINAL;
						  result = vn->getCurrentIndex();
						  if ( isUnique(result))
							  return result;
					  }
				  }
			  }
			  if (b==false){
				  vn->pop2();
				  currentStep->resetP_s(vn);
				  if (currentStep->prevS==NULL){
					  state=  XPATH_EVAL_END;
				  }else{
					  state=  XPATH_EVAL_BACKWARD;
					  currentStep = currentStep->prevS;
				  }
			  }
			  break;

		  case  XPATH_EVAL_TERMINAL:
			  while (vn->toElement(NEXT_SIBLING)){
				  if (currentStep->eval_s(vn)){
					  // state=  XPATH_EVAL_TERMINAL;
					  result = vn->getCurrentIndex();
					  if ( isUnique(result))
						  return result;
				  }
			  }
			  vn->pop2();
			  currentStep->resetP_s(vn);
			  if(currentStep->prevS!=NULL){
				  currentStep = currentStep->prevS;
				  state=  XPATH_EVAL_BACKWARD;
			  }else{
				  state=  XPATH_EVAL_END;
			  }
			  break;

		  default:
			  throw XPathEvalException("unknown state");
	}
	return -2;
}
int LocationPathExpr::process_parent(VTDNav *vn){
	bool /*b = false,*/ b1 = false;
	Predicate *t= NULL;
	int result;
	switch ( state) {
			case  XPATH_EVAL_START:
			case  XPATH_EVAL_FORWARD:
				t = currentStep->p;
				while(t!=NULL){
					if (t->requireContextSize_p()){
						int i = computeContextSize(t,vn);
						if (i==0){
							b1 = true;
							break;
						}else
							t->setContextSize_p(i);
					}
					t = t->nextP;
				}
				if (b1){
					if (state == XPATH_EVAL_FORWARD){
						state= XPATH_EVAL_BACKWARD;
						currentStep = currentStep->prevS;
					}else
						state= XPATH_EVAL_END;
					break;
				}

				if (vn->getCurrentDepth() == -1) {
					if ( state==  XPATH_EVAL_START)
						state=  XPATH_EVAL_END;
					else {
						//vn.pop();
						state=  XPATH_EVAL_BACKWARD;
						currentStep = currentStep->prevS;
					}
				} else {
					vn->push2();
					vn->toElement(PARENT); // must return true
					if (currentStep->eval_s(vn)){
						if (currentStep->nextS != NULL) {
							state=  XPATH_EVAL_FORWARD;
							currentStep = currentStep->nextS;
						} else {
							state=  XPATH_EVAL_TERMINAL;
							result = vn->getCurrentIndex();
							if ( isUnique(result))
								return result;
						}
					}else{
						vn->pop2();
						currentStep->resetP_s(vn);
						if ( state==  XPATH_EVAL_START)
							state=  XPATH_EVAL_END;
						else {
							state=  XPATH_EVAL_BACKWARD;
							currentStep = currentStep->prevS;
						}
					}
				}

				break;

			case  XPATH_EVAL_END:
				currentStep = NULL;
				// reset();
				return -1;

			case  XPATH_EVAL_BACKWARD:
			case  XPATH_EVAL_TERMINAL:
				if (currentStep->prevS == NULL) {
					vn->pop2();
					state=  XPATH_EVAL_END;
					break;
				}else {
					vn->pop2();
					state=  XPATH_EVAL_BACKWARD;
					currentStep = currentStep->prevS;
					break;
				}

			default:
				throw XPathEvalException("unknown state");

	}
	return -2;
}
int LocationPathExpr::process_preceding_sibling(VTDNav *vn){

	bool b = false, b1 = false;
	Predicate *t= NULL;
	int result;
	switch(state){
		  case  XPATH_EVAL_START:
		  case  XPATH_EVAL_FORWARD:
			  t = currentStep->p;
			  while(t!=NULL){
				  if (t->requireContextSize_p()){
					  int i = computeContextSize(t,vn);
					  if (i==0){
						  b1 = true;
						  break;
					  }else
						  t->setContextSize_p(i);
				  }
				  t = t->nextP;
			  }
			  if (b1){
				  if (state == XPATH_EVAL_FORWARD){
					  state= XPATH_EVAL_BACKWARD;
					  currentStep = currentStep->prevS;
				  }else
					  state= XPATH_EVAL_END;
				  break;
			  }
			  if ( state==  XPATH_EVAL_START)
				  state=  XPATH_EVAL_END;
			  else
				  state=  XPATH_EVAL_BACKWARD;
			  vn->push2();
			  while (vn->toElement(PREV_SIBLING)){
				  if (currentStep->eval_s(vn)){
					  if (currentStep->nextS!=NULL){
						  state=  XPATH_EVAL_FORWARD;
						  currentStep = currentStep->nextS;
						  break;
					  } else {
						  state=  XPATH_EVAL_TERMINAL;
						  result = vn->getCurrentIndex();
						  if ( isUnique(result))
							  return result;
					  }
				  }
			  }

			  if ( state==  XPATH_EVAL_END){
				  currentStep->resetP_s(vn);
				  vn->pop2();
			  }else if ( state==  XPATH_EVAL_BACKWARD){
				  currentStep->resetP_s(vn);
				  vn->pop2();
				  currentStep = currentStep->prevS;
			  }
			  break;

		  case  XPATH_EVAL_END:
			  currentStep = NULL;
			  // reset();
			  return -1;

		  case  XPATH_EVAL_BACKWARD:
			  while (vn->toElement(PREV_SIBLING)){
				  if (currentStep->eval_s(vn)){
					  if (currentStep->nextS!=NULL){
						  state=  XPATH_EVAL_FORWARD;
						  currentStep = currentStep->nextS;
						  b = true;
						  break;
					  } else {
						  state=  XPATH_EVAL_TERMINAL;
						  result = vn->getCurrentIndex();
						  if ( isUnique(result))
							  return result;
					  }
				  }
			  }
			  if (b==false){
				  vn->pop2();
				  currentStep->resetP_s(vn);
				  if (currentStep->prevS==NULL){
					  state=  XPATH_EVAL_END;
				  }else{
					  state=  XPATH_EVAL_BACKWARD;
					  currentStep = currentStep->prevS;
				  }
			  }
			  break;

		  case  XPATH_EVAL_TERMINAL:
			  while (vn->toElement(PREV_SIBLING)){
				  if (currentStep->eval_s(vn)){
					  // state =  XPATH_EVAL_TERMINAL;
					  result = vn->getCurrentIndex();
					  if ( isUnique(result))
						  return result;
				  }
			  }
			  vn->pop2();
			  if(currentStep->prevS!=NULL){
				  currentStep = currentStep->prevS;
				  state=  XPATH_EVAL_BACKWARD;
			  }else{
				  state=  XPATH_EVAL_END;
			  }
			  break;

		  default:
			  throw XPathEvalException("unknown state");
	}
	return -2;
}
int LocationPathExpr::process_self(VTDNav *vn){		
	
	bool /*b = false,*/ b1 = false;
	    //int contextSize;
	    Predicate *t= NULL;
	    int result;
		switch( state){
		  case  XPATH_EVAL_START:
		  case  XPATH_EVAL_FORWARD:
  	        t = currentStep->p;
	        while(t!=NULL){
	            if (t->requireContextSize_p()){
	                int i = computeContextSize(t,vn);
	                if (i==0){
	                    b1 = true;
	                    break;
	                }else
	                    t->setContextSize_p(i);
	            }
	            t = t->nextP;
	        }
	        if (b1){
	            if (state == XPATH_EVAL_FORWARD){
	                state= XPATH_EVAL_BACKWARD;
	                currentStep = currentStep->prevS;
	            }else
	                state= XPATH_EVAL_END;
	            break;
	        }
		  	if (currentStep->eval_s(vn)){
		  		if (currentStep->nextS!=NULL){
		  			 state=  XPATH_EVAL_FORWARD;
		  			currentStep = currentStep->nextS;
		  		}
		  		else{
		  			 state=  XPATH_EVAL_TERMINAL;
		  			 if (vn->atTerminal == true)
		  			     result = vn->LN;
		  			 else
		  			     result = vn->getCurrentIndex();
					if ( isUnique(result))
						return result;
		  		}
		  	}else {
		  		currentStep->resetP_s(vn);
		  		if ( state==  XPATH_EVAL_START)
		  			 state=  XPATH_EVAL_END;
		  		else
		  			 state=  XPATH_EVAL_BACKWARD;
		  	}
		    break;

		  case  XPATH_EVAL_END:
		  	currentStep = NULL;
		  	// reset();
		  	return -1;

		  case  XPATH_EVAL_BACKWARD:
		  case  XPATH_EVAL_TERMINAL:
		  	if (currentStep->prevS!=NULL){
	  			 state=  XPATH_EVAL_BACKWARD;
	  			currentStep= currentStep->prevS;
	  		}else{
	  			 state=  XPATH_EVAL_END;
	  		}
		  	break;

		  default:
			throw XPathEvalException("unknown state");
		}
	    return -2;
}
int LocationPathExpr::process_namespace(VTDNav *vn){
	AutoPilot *ap = NULL;
	bool b1 = false;
	Predicate *t= NULL;
	int temp;
	switch(state){
		case  XPATH_EVAL_START:
		case  XPATH_EVAL_FORWARD:

			t = currentStep->p;
			while(t!=NULL){
				if (t->requireContextSize_p()){
					int i = computeContextSize(t,vn);
					if (i==0){
						b1 = true;
						break;
					}else
						t->setContextSize_p(i);
				}
				t = t->nextP;
			}
			if (b1){
				if (state == XPATH_EVAL_FORWARD){
					state= XPATH_EVAL_BACKWARD;
					currentStep = currentStep->prevS;
				}else
					state= XPATH_EVAL_END;
				break;
			}

			if (vn->getAtTerminal()==true){
				if (state ==XPATH_EVAL_START)
					state = XPATH_EVAL_END;
				else {
					state = XPATH_EVAL_BACKWARD;
					currentStep  = currentStep->prevS;
				}
			} else {
				if (currentStep->ft == true) {
					if (currentStep->o == NULL)
						currentStep->o = ap = new AutoPilot(vn);
					else {
						ap = currentStep->o;
						ap->bind(vn);
					}
					if (currentStep->nt->testType== NT_NODE)
						ap->selectNameSpace((UCSChar *)L"*");
					else 
						ap->selectNameSpace(currentStep->nt->nodeName);
					currentStep->ft = false;
				}
				if ( state==  XPATH_EVAL_START)
					state=  XPATH_EVAL_END;
				vn->push2();
				//vn->setAtTerminal(true);
				while( (temp = ap->iterateNameSpace()) != -1){
					if (currentStep->evalPredicates(vn)){
						break;
					}
				}
				if (temp == -1){
					currentStep->ft = true;
					currentStep->resetP_s(vn);
					vn->setAtTerminal(false);
					if ( state==  XPATH_EVAL_FORWARD){
						state =  XPATH_EVAL_BACKWARD;
						currentStep = currentStep->prevS;
					}
				}else {

					if (currentStep->nextS != NULL){
						vn->LN = temp;
						state=  XPATH_EVAL_FORWARD;
						currentStep = currentStep->nextS;
					}
					else {
						//vn.pop();
						state=  XPATH_EVAL_TERMINAL;
						if ( isUnique(temp)){
							vn->LN = temp;
							return temp;
						}
					}

				}
			}
			break;

		case  XPATH_EVAL_END:
			currentStep = NULL;
			// reset();
			return -1;

		case  XPATH_EVAL_BACKWARD:
			ap = currentStep->o;
			//vn.push();
			while( (temp = ap->iterateNameSpace()) != -1){
				if (currentStep->evalPredicates(vn)){
					break;
				}
			}
			if (temp == -1) {
				vn->pop2();
				currentStep->ft = true;
				//freeAutoPilot(currentStep->o);
				//currentStep->o = NULL;
				currentStep->resetP_s(vn);
				vn->setAtTerminal(false);
				if (currentStep->prevS != NULL) {
					state =  XPATH_EVAL_BACKWARD;
					currentStep = currentStep->prevS;
				} else
					state =  XPATH_EVAL_END;
			} else {
				if (currentStep->nextS != NULL) {
					state =  XPATH_EVAL_FORWARD;
					currentStep = currentStep->nextS;
				} else {
					state =  XPATH_EVAL_TERMINAL;
					if ( isUnique(temp)){
						vn->LN = temp;
						return temp;
					}
				}
			}
			break;

		case  XPATH_EVAL_TERMINAL:
			ap = currentStep->o;
			while( (temp = ap->iterateNameSpace()) != -1){
				if (currentStep->evalPredicates(vn)){
					break;
				}
			}
			if (temp != -1)
				if (isUnique(temp)){
					vn->LN = temp;
					return temp;
				}
				vn->setAtTerminal(false);
				currentStep->resetP_s(vn);
				if (currentStep->prevS == NULL) {
					currentStep->ft = true;
					//freeAutoPilot(currentStep->o);
					//currentStep->o = NULL;
					vn->pop2();
					state=  XPATH_EVAL_END;
				} else {
					state=  XPATH_EVAL_BACKWARD;
					vn->pop2();
					currentStep->ft = true;
					//freeAutoPilot(currentStep->o);
					//currentStep->o = NULL;
					currentStep = currentStep->prevS;
				}

				break;

		default:
			throw XPathEvalException(
				"unknown state");
	}
	return -2;
}
void LocationPathExpr::selectNodeType(TextIter *ti){
		if (currentStep->nt->testType == NT_TEXT )
			ti->selectText();
		else if (currentStep->nt->testType == NT_COMMENT )
			ti->selectComment();
		else if (currentStep->nt->testType == NT_PI0 )
			ti->selectPI0();
		else {
			ti->selectPI1(currentStep->nt->nodeName);
		}
}



// predicate 
Predicate::Predicate():
d(0),
count(0),
nextP(NULL),
e(NULL)
{}

Predicate::~Predicate(){
	if (e!=NULL)
		delete e;
}
bool Predicate::eval_p(VTDNav *vn){
	bool b;
	count++; // increment the position
	e->setPosition(count);
	if (e->isNumerical()){
		b = (e->evalNumber(vn)== count);
	}
	else{
		b = e->evalBoolean(vn);
	}
	return b;
}
void Predicate::setIndex_p(int i){
	if (i<=0){
		throw XPathEvalException("Invalid index number");
	}
	d = (double) i;
}
void Predicate::setContextSize_p(int size){
	e->setContextSize(size);
}
bool Predicate::requireContextSize_p(){
	return e->requireContextSize();
}
void Predicate::reset_p(VTDNav *vn){
	count = 0;
	e->reset(vn); // is this really needed?
}
void Predicate::toString_p( UCSChar *string){
	//String s = "["+expr+"]";
		if (nextP==NULL){
			wprintf(L"[");
			e->toString(string);
			wprintf(L"]");
		} else {
			wprintf(L"[");
			e->toString(string);
			wprintf(L"]");
			nextP->toString_p(string);
		}
}







Step::Step():
nt(NULL),
p(NULL),
pt(NULL),
nextS(NULL),
position(1),
prevS(NULL),
o(NULL),
ft(true)
{}


Step::~Step(){
	Predicate *tmp, *tmp2;
	
	if (p != NULL){
		tmp = p;
		tmp2 = tmp->nextP;
		while(tmp2!=NULL){
			delete(tmp);
			tmp= tmp2;
			tmp2 = tmp2->nextP;
		}
		delete(tmp);
	}
	if (nt->testType == NT_TEXT){
		if (o!=NULL)
			delete((TextIter *)o);
	}else
		delete((AutoPilot *)o);
	delete(nt);
}
void Step::reset_s(VTDNav *vn){
	ft = true;
	resetP_s(vn);
	position =1;
}

void Step::resetP_s(VTDNav *vn){
	Predicate *temp = p;
	while(temp!=NULL){
		temp->reset_p(vn);
		temp = temp->nextP;
	}
}
void Step::resetP2_s(VTDNav *vn, Predicate *p1){
		Predicate *temp = p;
		while(temp!=p1){
			temp->reset_p(vn);
			temp = temp->nextP;
		}
}

NodeTest *Step::getNodeTest(){
	return nt;
}

Step *Step::getNextStep(){
	return nextS;
}

bool Step::get_ft(){
	return ft;
}

void Step::set_ft(bool b){
	ft = b;
}

Step *Step::getPrevStep(){
	return prevS;
}

void Step::setNodeTest(NodeTest *n){
	nt = n;
}

void Step::setPredicate(Predicate *p1){
	p = p1;
}

bool Step::eval_s(VTDNav *vn){
	return nt->eval_nt(vn) && evalPredicates(vn);
}

bool Step::eval_s2(VTDNav *vn, Predicate *p){
	return nt->eval_nt(vn) && evalPredicates2(vn,p);
}

bool Step::evalPredicates(VTDNav *vn){
	Predicate *temp = p;
	while(temp!=NULL) {
		if (temp->eval_p(vn)== false)
			return false;
		temp = temp->nextP;
	}
	return true;
}

bool Step::evalPredicates2(VTDNav *vn, Predicate *p){
	Predicate *temp = p;
	while(temp!=p) {
		if (temp->eval_p(vn)== false)
			return false;
		temp = temp->nextP;
	}
	return true;
}

void Step::setAxisType(axisType st){
	axis_type = st;
}

void Step::toString_s(UCSChar *string){
	//String s;
	if (p == NULL){
		wprintf(axisName(axis_type));
	    nt->toString_nt(string);
	}
	else {
		wprintf(axisName(axis_type));
		nt->toString_nt(string);
		wprintf(L" ");
		p->toString_p(string);
	}
	if (nextS == NULL)
		return;
	else {
		wprintf(L"/");
		nextS->toString_s(string);
	}
}

NodeTest::NodeTest():
nodeName(NULL),
prefix(NULL),
localName(NULL),
URL(NULL),
nsEnabled(false)
{
}

NodeTest::~NodeTest(){
	delete localName;
	delete nodeName;
	delete prefix;
}
bool NodeTest::eval_nt(VTDNav *vn){
	if (testType == NT_NODE)
		return true;
	else if(testType == NT_NAMETEST){
		if (vn->atTerminal == true)
			return false;
		if (localName!=NULL)
			return vn->matchElementNS(URL,localName);
		else
			return vn->matchElement(nodeName);
	}
	return true;
}
void NodeTest::setNodeName(UCSChar *name){
	nodeName = name;
}
void NodeTest::setNodeNameNS(UCSChar *p, UCSChar *ln){
	localName = ln;
	prefix = p;
}
void NodeTest::setTestType(nodeTestType ntt){
	testType = ntt;
}
void NodeTest::toString_nt(UCSChar *string){
	switch (testType){
		case NT_NAMETEST :
			if (localName == NULL)
				wprintf(L"%ls",nodeName);
			else
				wprintf(L"%ls:%ls", prefix,localName);
			break;
		case NT_NODE: wprintf(L"node()");break;
		case NT_TEXT: wprintf(L"text()");break;
		case NT_PI0:  wprintf(L"processing-instruction()");break;
		case NT_PI1: wprintf(L"processing-instruction(");
			if (wcschr(nodeName,'"')!=NULL){
				wprintf(L"'");
				wprintf(nodeName);
				wprintf(L"'");
			}else{
				wprintf(L"\"");
				wprintf(nodeName);
				wprintf(L"\"");
			}
			wprintf(L")");
			break;
		default:  wprintf(L"comment()");
	}
}









UCSChar *com_ximpleware::axisName(axisType i){
	switch(i){
			case AXIS_CHILD: return (UCSChar *)L"child::";
			case AXIS_DESCENDANT: return (UCSChar *)L"descendant::";
			case AXIS_PARENT: return (UCSChar *)L"parent::";
			case AXIS_FOLLOWING_SIBLING: return (UCSChar *)L"following-sibling::";
			case AXIS_PRECEDING_SIBLING: return (UCSChar *)L"preceding-sibling::";
			case AXIS_FOLLOWING: return (UCSChar *)L"following::";
			case AXIS_PRECEDING: return (UCSChar *)L"preceding::";
			case AXIS_ATTRIBUTE: return (UCSChar *)L"attribute::";
			case AXIS_NAMESPACE: return (UCSChar *)L"namespace::";
			case AXIS_SELF: return (UCSChar *)L"self::";
			case AXIS_DESCENDANT_OR_SELF: return (UCSChar *)L"descendant-or-self::";
			case AXIS_ANCESTOR: return (UCSChar *)L"ancestor::";
			default: return (UCSChar *)L"ancestor-or-self::";
	}
}
