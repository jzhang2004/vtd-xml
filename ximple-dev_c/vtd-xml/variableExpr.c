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

variableExpr *createVariableExpr(UCSChar* s, expr *e){
	variableExpr *v = (variableExpr*) malloc(sizeof(variableExpr));
	if (v==NULL){
		throwException2(out_of_mem,"variableExpr allocation failed ");
		return NULL;
	}
	v->freeExpr = (free_Expr)&freeVariableExpr;
	v->evalBoolean = (eval_Boolean)&evalBoolean_ve;
	v->evalNodeSet = (eval_NodeSet)&evalNodeSet_ve;
	v->evalNumber  = (eval_Number)&evalNumber_ve;
	v->evalString  = (eval_String)&evalString_ve;
	v->isNumerical = (is_Numerical)&isNumerical_ve;
	v->isBoolean = (is_Boolean)&isBoolean_ve;
	v->isString =  (is_String)&isString_ve;
	v->isNodeSet = (is_NodeSet)&isNodeSet_ve;
	v->requireContextSize = (require_ContextSize)&requireContextSize_ve;
	v->setContextSize = (set_ContextSize)&setContextSize_ve;
	v->setPosition = (set_Position)&setPosition_ve;
	v->reset = (reset_)&reset_ve;
	v->toString = (to_String)&toString_ve;
	v->adjust= (adjust_)&adjust_ve;
	v->exprName = s;
	v->exprVal = e;
	return v;
}
void freeVariableExpr(variableExpr *ve){
	if (ve == NULL) return;
	free(ve->exprName);// this assume s is dynamically created
	ve->exprVal->freeExpr(ve->exprVal);
	free(ve);
}
int	evalNodeSet_ve (variableExpr *ve,VTDNav *vn){
	return ve->exprVal->evalNodeSet(ve->exprVal, vn);
}
double	evalNumber_ve (variableExpr *ve,VTDNav *vn){
	return ve->exprVal->evalNumber(ve->exprVal, vn);
}
UCSChar* evalString_ve  (variableExpr *ve,VTDNav *vn){
	return ve->exprVal->evalString(ve->exprVal, vn);
}
Boolean evalBoolean_ve (variableExpr *ve,VTDNav *vn){
	return ve->exprVal->evalBoolean(ve->exprVal, vn);
}

Boolean isBoolean_ve (variableExpr *ve){
	return ve->exprVal->isBoolean(ve->exprVal);
}
Boolean isNumerical_ve (variableExpr *ve){
	return ve->exprVal->isNumerical(ve->exprVal);
}
Boolean isString_ve (variableExpr *ve){
	return ve->exprVal->isString(ve->exprVal);
}
Boolean isNodeSet_ve (variableExpr *ve){
	return ve->exprVal->isNodeSet(ve->exprVal);
}
Boolean requireContextSize_ve(variableExpr *ve){
	return ve->exprVal->requireContextSize(ve->exprVal);
}
void reset_ve(variableExpr *ve, VTDNav *vn){
	ve->exprVal->reset(ve->exprVal, vn);
}
void  setContextSize_ve(variableExpr *ve,int s){
	ve->exprVal->setContextSize(ve->exprVal,s);
}
void  setPosition_ve(variableExpr *ve,int pos){
	ve->exprVal->setPosition(ve->exprVal, pos);
}
void  toString_ve(variableExpr *ve, UCSChar* string){
	wprintf(L"$");
	wprintf(L"%ls",ve->exprName);
}
int	adjust_ve(variableExpr *ve, int n){
	return ve->exprVal->adjust(ve->exprVal,n);
}
