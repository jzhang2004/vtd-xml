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
static Boolean isUnique_pe(pathExpr *pe,int i);

Boolean isUnique_pe(pathExpr *pe, int i){
	return isUniqueIntHash(pe->ih,i);
}
pathExpr *createPathExpr(expr *f, locationPathExpr *l){
	exception e;
	pathExpr *pe = (pathExpr *)malloc(sizeof(pathExpr));
	if (pe==NULL){
		throwException2(out_of_mem,
			"pathExpr allocation failed ");
		return NULL;
	}
	Try{
		pe->ih = createIntHash();
	}
	Catch(e){
		free(pe);
		Throw e;
	}

	pe->freeExpr = (free_Expr) &freePathExpr;
	pe->evalBoolean = (eval_Boolean)&evalBoolean_pe;
	pe->evalNodeSet = (eval_NodeSet)&evalNodeSet_pe;
	pe->evalNumber  = (eval_Number)&evalNumber_pe;
	pe->evalString  = (eval_String)&evalString_pe;
	pe->isNumerical =  (is_Numerical)&isNumerical_pe;
	pe->isBoolean = (is_Boolean)&isBoolean_pe;
	pe->isString =  (is_String)&isString_pe;
	pe->isNodeSet = (is_NodeSet)&isNodeSet_pe;
	pe->requireContextSize = (require_ContextSize)&requireContextSize_pe;
	pe->setContextSize = (set_ContextSize)&setContextSize_pe;
	pe->setPosition = (set_Position)&setPosition_pe;
	pe->reset = (reset_)&reset_pe;
	pe->toString = (to_String)&toString_pe;
	pe->adjust = (adjust_)&adjust_pe;
	pe->fe = f;
	pe->lpe= l;
	pe->evalState = 0;

	return pe;

}
void freePathExpr(pathExpr *pe){
	if (pe==NULL) return;
	freeIntHash(pe->ih);
	pe->fe->freeExpr(pe->fe);
	freeLocationPathExpr(pe->lpe);
	free(pe);
}


int	evalNodeSet_pe (pathExpr *pe,VTDNav *vn){
	int a;
	while (TRUE) {
		switch (pe->evalState) {
		case 0: /*this state is teh initial state;*/
			a = pe->fe->evalNodeSet(pe->fe,vn);
			if (a == -1){
				pe->evalState =4;
			}
			else
				pe->evalState = 1;
			break;
		case 1: /* fe returns valid value, then iterate the locationPath*/
			push2(vn);
			a = evalNodeSet_lpe(pe->lpe, vn);
			if (a == -1) {
				reset_lpe(pe->lpe, vn);
				pe->evalState = 3;
			} else {
				pe->evalState = 2;
				if (isUnique_pe(pe,a))
					return a;
			}
			break;
		case 2:
			a = evalNodeSet_lpe(pe->lpe, vn);
			if (a == -1) {
				reset_lpe(pe->lpe, vn);
				pe->evalState = 3;
			} else{
				if (isUnique_pe(pe, a))
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
			throwException2(other_exception,
				"Invalid state evaluating PathExpr");
		}
	}
}
double	evalNumber_pe (pathExpr *pe,VTDNav *vn){

	double d = 0.0;
	exception e;
	int a = getStringIndex((expr *)pe,vn);
	Try{
		if (a!=-1) return parseDouble(vn,a);
	}Catch (e){
	}
	return 0/d;
}

UCSChar* evalString_pe  (pathExpr *pe,VTDNav *vn){
	exception e;
	int a = getStringIndex((expr *)pe,vn);
	Try {
		if (a != -1)
			return toString(vn,a);
	} Catch (e) {
		if (e.et ==out_of_mem){
			Throw e;
		}
	}
	return createEmptyString();

}
Boolean evalBoolean_pe (pathExpr *pe,VTDNav *vn){
	exception e;
	Boolean b = FALSE;
	int size;
	push2(vn);
	/* record teh stack size*/
	size = vn->contextBuf2->size;
    Try{
		b = (evalNodeSet_pe(pe,vn) != -1);
	}Catch (e){
	}
	/*rewind stack*/
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
	resetIntHash(pe->ih);
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

int adjust_pe(pathExpr *pe, int n){
	int i=pe->fe->adjust(pe->fe,n);
	pe->lpe->adjust((expr *)(pe->lpe),n);
	if (pe->ih!=NULL && i==pe->ih->e)
	{}else{
		freeIntHash(pe->ih);
		pe->ih = createIntHash2(i);
	}
	return i;
}