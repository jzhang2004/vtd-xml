/* 
* Copyright (C) 2002-2009 XimpleWare, info@ximpleware.com
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

unaryExpr *createUnaryExpr(opType op, expr *e1){
	unaryExpr* ue = (unaryExpr *)malloc(sizeof(unaryExpr));
	if (ue == NULL){
		//e1->freeExpr(e1);
		throwException2(out_of_mem,
			"unaryExpr allocation failed ");
		return NULL;
	}
	ue->freeExpr = &freeUnaryExpr;
	ue->evalBoolean = &evalBoolean_ue;
	ue->evalNodeSet = &evalNodeSet_ue;
	ue->evalNumber  = &evalNumber_ue;
	ue->evalString  = &evalString_ue;
	ue->isNumerical = &isNumerical_ue;
	ue->isBoolean = &isBoolean_ue;
	ue->isString =  &isString_ue;
	ue->isNodeSet = &isNodeSet_ue;
	ue->requireContextSize = &requireContextSize_ue;
	ue->setContextSize = &setContextSize_ue;
	ue->setPosition = &setPosition_ue;
	ue->reset = &reset_ue;
	ue->toString = &toString_ue;
	ue->adjust = &adjust_ue;
	ue->e = e1;
	ue->op = op;

	return ue;
}
void freeUnaryExpr(unaryExpr *ue){
	if (ue == NULL) return;
	ue->e->freeExpr(ue->e);
	free(ue);
}

int	evalNodeSet_ue (unaryExpr *ue,VTDNav *vn){
	throwException2(xpath_eval_exception,
		"unaryExpr can't eval to a node set!");
	return -1;
}

double	evalNumber_ue (unaryExpr *ue,VTDNav *vn){
	return -1*(ue->e->evalNumber)(ue->e, vn);
}

UCSChar* evalString_ue  (unaryExpr *ue,VTDNav *vn){
	UCSChar *string1 = (ue->e->evalString)(ue->e,vn);
	size_t len = wcslen(string1);
	UCSChar *string = (UCSChar*) malloc((len+1)*sizeof(UCSChar));
	if (string == NULL) {
			throwException2(out_of_mem,
				"allocate string failed in funcExpr's evalString()");
	}

	swprintf(string,64,L"-%ls",string1);

	free(string1);
	return string;
}

Boolean evalBoolean_ue (unaryExpr *ue,VTDNav *vn){
	return (ue->e->evalBoolean)(ue->e, vn);
}

Boolean isBoolean_ue (unaryExpr *ue){
	return FALSE;
}

Boolean isNumerical_ue (unaryExpr *ue){
	return TRUE;
}

Boolean isString_ue (unaryExpr *ue){
	return FALSE;
}

Boolean isNodeSet_ue (unaryExpr *ue){
	return FALSE;
}

Boolean requireContextSize_ue(unaryExpr *ue){
	return (ue->e->requireContextSize)(ue->e);
}

void	reset_ue(unaryExpr *ue, VTDNav *vn){
	(ue->e->reset)(ue->e, vn);
}

void	setContextSize_ue(unaryExpr *ue,int s){
	(ue->e->setContextSize)(ue->e, s);
}

void	setPosition_ue(unaryExpr *ue,int pos){
	(ue->e->setPosition)(ue->e, pos);
}

void    toString_ue(unaryExpr *ue, UCSChar* string){
	wprintf(L"-");
	(ue->e->toString)(ue->e, string);
}

int adjust_ue(unaryExpr *e, int n){return 0;}