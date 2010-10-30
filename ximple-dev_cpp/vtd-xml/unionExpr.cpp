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
#include "unionExpr.h"
#pragma warning(disable:4355)
using namespace com_ximpleware;

UnionExpr::UnionExpr(Expr *e):
current(this),
next(NULL),
fe(e),
evalState(0),
ih(NULL)
{
}
UnionExpr::~UnionExpr(){
	UnionExpr *tmp, *tmp2;
	delete(fe);
	delete(ih);
	tmp = next;

	while(tmp!= NULL){
		delete(tmp->fe);
		delete(tmp->ih);
		tmp2 = tmp;
		tmp = tmp->next;		
		delete(tmp2);
	}
}

bool UnionExpr::evalBoolean(VTDNav *vn){
	
	bool b = false;
	int size;
	if (fe->isNodeSet()==false){
		return fe->evalBoolean(vn);
	}else{
			vn->push2();
			/* record teh stack size*/
			size = vn->contextBuf2->size;
			try{	
				b = (evalNodeSet(vn) != -1);
			}catch (...){
			}
			/*rewind stack */
			vn->contextBuf2->size = size;
			reset(vn);
			vn->pop2();
			return b;
	}
}

double UnionExpr::evalNumber(VTDNav *vn){
	double d = 0.0;
	int a;
	if (fe->isNodeSet()==false){   
		return fe->evalNumber(vn);   
	}
	a = getStringIndex(vn);
	try{
		if (a!=-1) return vn->parseDouble(a);
	}catch (...){
	}
	return 0/d;
}

int UnionExpr::evalNodeSet(VTDNav *vn){
	int a;
	if (next == NULL) {
		return fe->evalNodeSet(vn);
	} else {
		while (true) {
			switch (evalState) {
				case 0:
					if (ih == NULL ){
						//exception ee;
						try{
							ih = new IntHash();
						}
						catch(...){
							throw;
						}
					}
					if (current != NULL) {
						vn->push2();
						while ((a = current->fe->evalNodeSet(vn))!= -1) {
								if (isUnique_une(a)) {
									evalState = 1;
									return a;
								}
						}
						evalState = 2;
						vn->pop2();
						break;
					} else
						evalState = 3;
					break;

				case 1:
					while ((a = current->fe->evalNodeSet(vn)) != -1) 
					{
						if (isUnique_une(a)) {
							evalState = 1;
							return a;
						}
					}
					evalState = 2;
					vn->pop2();
					break;

				case 2:
					current = current->next;
					if (current != NULL) {
						vn->push2();
						while ((a = current->fe->evalNodeSet(vn)) != -1) {
								if (isUnique_une(a)) {
									evalState = 1;
									return a;
								}
						}
						vn->pop2();
						break;
					} else
						evalState = 3;
					break;

				case 3:
					return -1;

				default:
					throw OtherException("Invalid state evaluating unionExpr");
			}
		}
	}
}

UCSChar* UnionExpr::evalString(VTDNav *vn){
	int a;
	if (fe->isNodeSet()==false){   
		return fe->evalString(vn);   
	}
	a = getStringIndex(vn);
	try {
		if (a != -1)
			return vn->toString(a);
	} catch (std::bad_alloc& ) {
		throw;
	}
	return createEmptyString();	
}

void UnionExpr::reset(VTDNav *vn){
	UnionExpr *tmp;
	fe->reset(vn);
	current = this;
	tmp = next;
    while (tmp != NULL) {
        tmp->fe->reset(vn);
        tmp = tmp->next;
    }
	if (ih!=NULL)
		ih->reset();
	evalState = 0;
}

void UnionExpr::toString(UCSChar *string){
       if (next == NULL)
            fe->toString(string);
	   else{
            fe->toString(string);
			wprintf(L" | ");
			next->toString(string);
	   }
}

bool UnionExpr::isNumerical(){
	return fe->isNumerical();
}
bool UnionExpr::isNodeSet(){
	return fe->isNodeSet();
}
bool UnionExpr::isString(){
	return fe->isString();
}
bool UnionExpr::isBoolean(){
	return fe->isBoolean();
}

bool UnionExpr::requireContextSize(){
// boolean b = false;
	UnionExpr* tmp = this;

	while (tmp != NULL) {
		if (tmp->fe->requireContextSize()== true){
			return true;
		}            
		tmp = tmp->next;
	}        
	return false;
}
void UnionExpr::setContextSize(int size){
	UnionExpr *tmp;
    current = this;
    current->fe->setContextSize( size);
    tmp = next;
    while (tmp != NULL) {
        tmp->fe->setContextSize( size);
        tmp = tmp->next;
    }
}

void UnionExpr::setPosition(int pos){
	UnionExpr *tmp;
    current = this;
    current->fe->setPosition( pos);
    tmp = next;
    while (tmp != NULL) {
        tmp->fe->setPosition(pos);
        tmp = tmp->next;
    }
}

int UnionExpr::adjust(int n){
	int i=fe->adjust(n);
	UnionExpr *tmp = NULL;		
	tmp = next;
	while (tmp != NULL) {
		tmp->fe->adjust(n);
		tmp = tmp->next;
	}
	if (ih!=NULL && i==ih->e)
	{}else{
		delete (ih);
		ih = new IntHash(i);
	}
	return i;
}
