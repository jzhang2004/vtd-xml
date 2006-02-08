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

#include "xpath1.h"

numberExpr *createNumberExpr (double d){
	exception e;
	numberExpr *n = (numberExpr*) malloc(sizeof(numberExpr));
	if (n==NULL){
		e.et = out_of_mem;
		e.msg = "numberExpr allocation failed ";
		Throw e;
	}
	n->freeExpr = &freeNumberExpr;
	n->evalBoolean = &evalBoolean_ne;
	n->evalNodeSet = &evalNodeSet_ne;
	n->evalNumber  = &evalNumber_ne;
	n->evalString  = &evalString_ne;
	n->isNumerical = &isNumerical_ne;
	n->isBoolean = &isBoolean_ne;
	n->isString =  &isString_ne;
	n->isNodeSet = &isNodeSet_ne;
	n->requireContextSize = &requireContextSize_ne;
	n->setContextSize = &setContextSize_ne;
	n->setPosition = &setPosition_ne;
	n->reset = &reset_ne;
	n->toString = &toString_ne;
	n->dval= d;

	return n;
}

void freeNumberExpr(numberExpr *ne){
	free(ne);
}

int	evalNodeSet_ne (numberExpr *ne,VTDNav *vn){
	exception e;
	e.et = xpath_eval_exception;
	e.msg = "numberExpr can't eval to a node set!";
	Throw e;	
}

double	evalNumber_ne (numberExpr *ne,VTDNav *vn){
	return ne->dval;
}

UCSChar* evalString_ne  (numberExpr *ne,VTDNav *vn){
	exception e;
	Boolean b = FALSE;
	double d = 0;
	UCSChar *tmp;
	if (ne->dval != ne->dval){
		tmp = wcsdup(L"NaN");
		b = TRUE;
	}
	else if ( ne->dval == 1/d){
		tmp = wcsdup(L"Infinity");
		b= TRUE;
	}
	else if (ne->dval == -1/d){
		tmp = wcsdup(L"-Infinity");
		b = TRUE;
	}	else 
	tmp = malloc(sizeof(UCSChar)<<8);

	if (tmp == NULL) {
		e.et = out_of_mem;
		e.msg = "string allocation in evalString_ne failed ";
		Throw e;
	}
	if (b)
		return tmp;
    if (ne->dval == (Long) ne->dval){
		swprintf(tmp,L"%d",(Long) ne->dval);
	} else {
		swprintf(tmp,L"%f", ne->dval);
	}
	return tmp;
}

Boolean evalBoolean_ne (numberExpr *ne,VTDNav *vn){
	if (ne->dval == 0 
		|| ne->dval!=ne->dval)
		return FALSE;
	return TRUE;
}

Boolean isBoolean_ne (numberExpr *ne){
	return FALSE;
}

Boolean isNumerical_ne (numberExpr *ne){
	return TRUE;
}

Boolean isString_ne (numberExpr *ne){
	return FALSE;
}

Boolean isNodeSet_ne (numberExpr *ne){
	return FALSE;
}

Boolean requireContextSize_ne(numberExpr *ne){
	return FALSE;
}

void	reset_ne(numberExpr *ne, VTDNav *vn){
}

void	setContextSize_ne(numberExpr *ne,int s){
}

void	setPosition_ne(numberExpr *ne,int pos){
}

void    toString_ne(numberExpr *ne, UCSChar* string){
	if (ne->dval == (long)ne->dval){
		wprintf(L"%d",(long)ne->dval);
	}else 
		wprintf(L"%f",ne->dval);
}