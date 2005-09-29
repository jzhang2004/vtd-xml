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

literalExpr *createLiteralExpr(UCSChar *st){
	exception e;
	literalExpr *l = (literalExpr*) malloc(sizeof(literalExpr));
	if (l==NULL){
		e.et = out_of_mem;
		e.msg = "literalExpr allocation failed ";
		Throw e;
	}
	l->freeExpr = &freeLiteralExpr;
	l->evalBoolean = &evalBoolean_le;
	l->evalNodeSet = &evalNodeSet_le;
	l->evalNumber  = &evalNumber_le;
	l->evalString  = &evalString_le;
	l->isNumerical = &isNumerical_le;
	l->isBoolean = &isBoolean_le;
	l->isString =  &isString_le;
	l->isNodeSet = &isNodeSet_le;
	l->requireContextSize = &requireContextSize_le;
	l->setContextSize = &setContextSize_le;
	l->setPosition = &setPosition_le;
	l->reset = &reset_le;
	l->toString = &toString_le;
	l->s= st;

	
	return l;

}

void freeLiteralExpr(literalExpr *le){
	if (le == NULL) return;
	free(le->s);// this assume s is dynamically created
	free(le);
}

int	evalNodeSet_le (literalExpr *le,VTDNav *vn){
	exception e;
	e.et = xpath_eval_exception;
	e.msg = "LiteralExpr can't eval to a node set!";
	Throw e;	
}

double	evalNumber_le (literalExpr *le,VTDNav *vn){
	double d  = 0;
	UCSChar *temp;
	double result = wcstod(le->s,&temp);
	while(*temp!=0){
		if ( *temp == L' ' 
			|| *temp == L'\n'
			|| *temp == L'\t'
			|| *temp == L'\r'){ 			
				temp++;
			}
		else 
			return d/d; //NaN
	}
	return result;
}

UCSChar* evalString_le (literalExpr *le,VTDNav *vn){
	return wcsdup(le->s);
}

Boolean evalBoolean_le (literalExpr *le,VTDNav *vn){
	size_t len = wcslen(le->s);
	return len != 0;
}

Boolean isBoolean_le (literalExpr *le){
	return FALSE;
}

Boolean isNumerical_le (literalExpr *le){
	return FALSE;
}

Boolean isString_le (literalExpr *le){
	return TRUE;
}

Boolean isNodeSet_le (literalExpr *le){
	return FALSE;
}

Boolean requireContextSize_le(literalExpr *le){
	return FALSE;
}

void	reset_le(literalExpr *le, VTDNav *vn){
}

void	setContextSize_le(literalExpr *le,int s){
}

void	setPosition_le(literalExpr *le,int pos){
}

void    toString_le(literalExpr *le, UCSChar* string){
	wprintf(L"\"");
	wprintf(L"%ls",le->s);
	wprintf(L"\"");
}