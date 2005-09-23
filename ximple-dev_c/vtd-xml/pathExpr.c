/* 
* Copyright (C) 2002-2005 XimpleWare, info@ximpleware.com
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
#include "xpath1.h"
static Boolean isUnique(pathExpr *pe,int i);

Boolean isUnique(pathExpr *pe, int i){
		int size = pe->fib->size;
		int j;
		for (j=0; j<size;j++){
			if (i == intAt(pe->fib,j))
				return FALSE;
		}
		appendInt(pe->fib,i);
		return TRUE;
}
pathExpr *createPathExpr(expr *f, locationPathExpr *l){
	exception e;
	pathExpr *pe = (pathExpr *)malloc(sizeof(pathExpr));
	if (pe==NULL){
		e.et = out_of_mem;
		e.msg = "pathExpr allocation failed ";
		Throw e;
	}
	Try{
		pe->fib = createFastIntBuffer2(8);
	}
	Catch(e){
		free(pe);
		Throw e;
	}

	pe->freeExpr = &freePathExpr;
	pe->evalBoolean = &evalBoolean_pe;
	pe->evalNodeSet = &evalNodeSet_pe;
	pe->evalNumber  = &evalNumber_pe;
	pe->evalString  = &evalString_pe;
	pe->isNumerical = &isNumerical_pe;
	pe->isBoolean = &isBoolean_pe;
	pe->isString =  &isString_pe;
	pe->isNodeSet = &isNodeSet_pe;
	pe->requireContextSize = &requireContextSize_pe;
	pe->setContextSize = &setContextSize_pe;
	pe->setPosition = &setPosition_pe;
	pe->reset = &reset_pe;
	pe->toString = &toString_pe;
	pe->fe = f;
	pe->lpe= l;
	pe->evalState = 0;
	
	return pe;
	
}
void freePathExpr(pathExpr *pe){
	if (pe==NULL) return;
	freeFastIntBuffer(pe->fib);
	pe->fe->freeExpr(pe->fe);
	freeLocationPathExpr(pe->lpe);
	free(pe);
}


int	evalNodeSet_pe (pathExpr *pe,VTDNav *vn){
	int a;
	exception e;
	while (TRUE) {
		switch (pe->evalState) {
		case 0: //this state is teh initial state;
			a = pe->fe->evalNodeSet(pe->fe,vn);
			if (a == -1){
				pe->evalState =4;
			}
			else
				pe->evalState = 1;
			break;
		case 1: // fe returns valid value, then iterate the locationPath
			push2(vn);
			a = evalNodeSet_lpe(pe->lpe, vn);
			if (a == -1) {
				reset_lpe(pe->lpe, vn);					
				pe->evalState = 3;
			} else {
				pe->evalState = 2;
				if (isUnique(pe,a))
					return a;
			}
			break;
		case 2:
			a = evalNodeSet_lpe(pe->lpe, vn);
			if (a == -1) {
				reset_lpe(pe->lpe, vn);
				pe->evalState = 3;
			} else{
				if (isUnique(pe, a))
					return a;
				//return a;
			}
			break;
		case 3:
			pop2(vn);
			a = pe->fe->evalNodeSet(pe->fe,vn);
			if (a == -1)
				pe->evalState = 4;
			else{
			    push2(vn);
				pe->evalState = 2;
			}
			break;
		case 4:
			return -1;
		default:
			e.et = other;
			e.msg = "Invalid state evaluating PathExpr";
			Throw e;
		}
	}
}
double	evalNumber_pe (pathExpr *pe,VTDNav *vn){
	exception e;
	Boolean a = FALSE;
	int size;
	push2(vn);
	// record teh stack size
	size = vn->contextBuf2->size;
       Try{	
		a = (evalNodeSet_pe(pe,vn) != -1);
	}Catch (e){
	}
//rewind stack
	vn->contextBuf2->size = size;
	reset_pe(pe,vn);
	pop2(vn);
	return a;
}

UCSChar* evalString_pe  (pathExpr *pe,VTDNav *vn){
	exception e;
	int size = vn->contextBuf2->size ,a = -1;
	push2(vn);
	Try {
		a = evalNodeSet_pe(pe,vn);
		if (a != -1) {
			if (getTokenType(vn,a) == TOKEN_ATTR_NAME) {
				a++;
			}
			if (getTokenType(vn,a) == TOKEN_STARTING_TAG) {
				a = getText(vn);
			}
		}
	} Catch (e) {
	}
	vn->contextBuf2->size = size;
	reset_pe(pe,vn);
	pop2(vn);
	Try {
		if (a != -1)
			return toString(vn,a);
	} Catch (e) {
		if (e.et ==out_of_mem){
			Throw e;
		}
	}
	return NULL;	

}
Boolean evalBoolean_pe (pathExpr *pe,VTDNav *vn){
	exception e;
	Boolean b = FALSE;
	int size;
	push2(vn);
	// record teh stack size
	size = vn->contextBuf2->size;
    Try{	
		b = (evalNodeSet_pe(pe,vn) != -1);
	}Catch (e){
	}
		//rewind stack
	vn->contextBuf2->size = size;
	reset_pe(pe,vn);
	pop2(vn);
	return b;
}

Boolean isBoolean_pe (pathExpr *pe){
	return FALSE;
}

Boolean isNumerical_pe (pathExpr *pe){
	return FALSE;
}

Boolean isString_pe (pathExpr *pe){
	return FALSE;
}

Boolean isNodeSet_pe (pathExpr *pe){
	return TRUE;
}

Boolean requireContextSize_pe(pathExpr *pe){
	return FALSE;
}


void reset_pe(pathExpr *pe, VTDNav *vn){
	pe->fe->reset(pe->fe,vn);
	reset_lpe(pe->lpe,vn);
	clearFastIntBuffer(pe->fib);
	pe->evalState = 0;
}
void setContextSize_pe(pathExpr *pe,int s){
}

void setPosition_pe(pathExpr *pe,int pos){

}
void toString_pe(pathExpr *pe, UCSChar* string){
	wprintf(L"(");
	pe->fe->toString(pe->fe,string);
	wprintf(L")/");
	toString_lpe(pe->lpe, string);
}

