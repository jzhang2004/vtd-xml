/* 
* Copyright (C) 2002-2011 XimpleWare, info@ximpleware.com
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
#include "pathExpr.h"
using namespace com_ximpleware;

PathExpr::PathExpr(Expr *f, LocationPathExpr *l) try :
fe(f),
lpe(l),
evalState(0),
ih( new IntHash())
{}
catch (std::bad_alloc&){
	throw OutOfMemException("Allocation failed for PathExpr");
}

PathExpr::~PathExpr(){
	delete ih;
	delete fe;
	delete lpe;
	ih=NULL;
	fe=NULL;
	lpe=NULL;
}
bool PathExpr::evalBoolean(VTDNav *vn){
	bool b = false;
	int size;
	vn->push2();
	/* record teh stack size*/
	size = vn->contextBuf2->size;
    try{
		b = (evalNodeSet(vn) != -1);
	}catch (...){
	}
	/*rewind stack*/
	vn->contextBuf2->size = size;
	reset(vn);
	vn->pop2();
	return b;
}

double PathExpr::evalNumber(VTDNav *vn){
	double d = 0.0;	
	int a = getStringIndex(vn);
	try{
		if (a!=-1) return vn->parseDouble(a);
	}catch (...){
	}
	return 0/d;
}

int PathExpr::evalNodeSet(VTDNav *vn){
	int a;
	while (true) {
		switch (evalState) {
		case 0: /*this state is teh initial state;*/
			a = fe->evalNodeSet(vn);
			if (a == -1){
				evalState =4;
			}
			else
				evalState = 1;
			break;
		case 1: /* fe returns valid value, then iterate the locationPath*/
			vn->push2();
			a = lpe->evalNodeSet(vn);
			if (a == -1) {
				lpe->reset(vn);
				evalState = 3;
			} else {
				evalState = 2;
				if (isUnique(a))
					return a;
			}
			break;
		case 2:
			a = lpe->evalNodeSet(vn);
			if (a == -1) {
				lpe->reset( vn);
				evalState = 3;
			} else{
				if (isUnique(a))
					return a;
				//return a;
			}
			break;
		case 3:
			vn->pop2();
			a = fe->evalNodeSet(vn);
			if (a == -1)
				evalState = 4;
			else{
			    vn->push2();
				evalState = 2;
			}
			break;
		case 4:
			return -1;
		default:
			throw OtherException("Invalid state evaluating PathExpr");
		}
	}
}

UCSChar* PathExpr::evalString(VTDNav *vn){
	int a = getStringIndex(vn);
	try {
		if (a != -1)
			return vn->toString(a);
	} catch (std::bad_alloc&) {
		throw;
	}
	return createEmptyString();
}

void PathExpr::reset(VTDNav *vn){
	fe->reset(vn);
	lpe->reset(vn);
	ih->reset();
	evalState = 0;
}

void PathExpr::toString(UCSChar *string){	
	wprintf(L"(");
	fe->toString(string);
	wprintf(L")/");
	lpe->toString(string);
}

bool PathExpr::isNumerical(){return false;}
bool PathExpr::isNodeSet(){return true;}
bool PathExpr::isString(){return false;}
bool PathExpr::isBoolean(){return false;}

bool PathExpr::requireContextSize(){return false;}
void PathExpr::setContextSize(int size){}

void PathExpr::setPosition(int pos){}
int PathExpr::adjust(int n){
	int i=fe->adjust(n);
	lpe->adjust(n);
	if (ih!=NULL && i==ih->e)
	{}else{
		delete(ih);
		ih = new IntHash(i);
	}
	return i;
}