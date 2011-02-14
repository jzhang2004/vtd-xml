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
#include "filterExpr.h"
using namespace com_ximpleware;

FilterExpr::FilterExpr(Expr *e1, Predicate *pr):
e(e1),
p(pr)
{}
	
FilterExpr::~FilterExpr(){
	delete e;
	delete p;
	e=NULL;
	p=NULL;
}

bool FilterExpr::evalBoolean(VTDNav *vn){
	bool a = false;
	int size;
	vn->push2();
	//record stack size
	size = vn->contextBuf2->size;
	try{
		a = (evalNodeSet(vn) != -1);
	}catch (XPathEvalException&){
	}
	//rewind stack
	vn->contextBuf2->size = size;
	reset(vn);
	vn->pop2();
	return a;
}

double FilterExpr::evalNumber(VTDNav *vn){
	double d = 0.0;
	int a = getStringIndex(vn);
	try{
		if (a!=-1) return vn->parseDouble(a);
	}catch (NavException&){
	}
	return 0/d;
}

int FilterExpr::evalNodeSet(VTDNav *vn){
	int i,a;
	if (first_time && p->requireContextSize_p()){
		first_time = false;
		i = 0;
		e->adjust(vn->vtdSize);
		while(e->evalNodeSet(vn)!=-1)
			i++;
		p->setContextSize_p(i);
		reset2(vn);
	}
	a = e->evalNodeSet(vn);
	while (a!=-1){
		if (p->eval_p(vn)==true){
			//p.reset();
			return a;
		}else {
			//p.reset();
			a = e->evalNodeSet(vn);
		}
	}
	return -1;
}

UCSChar* FilterExpr::evalString(VTDNav *vn){
	int a = getStringIndex(vn);
	try {
		if (a != -1)
			return vn->toString(a);
	} catch (std::bad_alloc&) {
		throw;
	}
	return createEmptyString();
}

void FilterExpr::reset(VTDNav *vn){
	reset2(vn);
	//vn.contextStack2.size = stackSize;
	//position = 1;
	first_time = true;
}
void FilterExpr::reset2(VTDNav *vn){
	e->reset(vn);
	p->reset_p(vn);
}
void FilterExpr::toString(UCSChar* string){
	wprintf(L"(");
	e->toString(string);
	wprintf(L")");
	p->toString_p(string);
}

bool FilterExpr::isNumerical(){return false;}

bool FilterExpr::isNodeSet(){return true;}

bool FilterExpr::isString(){return false;}

bool FilterExpr::isBoolean(){return false;}

bool FilterExpr::requireContextSize(){
	return e->requireContextSize();
}

void FilterExpr::setContextSize(int size){
	e->setContextSize(size);
}

void FilterExpr::setPosition(int pos){
	e->setPosition(pos);
}

int FilterExpr::adjust(int n){
	return e->adjust(n);
}