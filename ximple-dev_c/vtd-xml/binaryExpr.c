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
#include <math.h>
#define BUF_SZ_EXP 7
static Boolean computeEQNE(binaryExpr *be, opType op,VTDNav *vn);
static Boolean compNumericalNodeSet(binaryExpr *be, expr* left, expr* right, VTDNav *vn, opType op);
static Boolean compStringNodeSet(binaryExpr *be, expr* left, expr* right, VTDNav *vn, opType op);
static Boolean compNodeSetNodeSet(binaryExpr *be, expr* left, expr* right, VTDNav *vn, opType op);

static Boolean compNumericalNodeSet(binaryExpr *be, expr* left, expr* right, VTDNav *vn, opType op){
	exception e;
	int i,t,i1 = 0,stackSize;
	Try {
		push2(vn);
		stackSize = vn->contextBuf2->size;
		while ((i = right->evalNodeSet(right,vn)) != -1) {
			t = getTokenType(vn,i);
			if (t == TOKEN_STARTING_TAG){
				i1 = getText(vn);
				if (i1 == -1)
					break;
				if (parseDouble(vn,i1) == left->evalNumber(left,vn)){
					vn->contextBuf2->size = stackSize;
					pop2(vn);
					left->reset(left,vn);
					right->reset(right,vn);
					if (op == OP_EQ)
						return TRUE;
					else 
						return FALSE;
				}
			}
			else if (t == TOKEN_ATTR_NAME
				|| t == TOKEN_ATTR_NS) {
					if (parseDouble(vn,i+1) == left->evalNumber(left,vn)){
						vn->contextBuf2->size = stackSize;
						pop2(vn);
						left->reset(left,vn);
						right->reset(right,vn);
						if (op == OP_EQ)
							return TRUE;
						else 
							return FALSE;
					}
			}
			else if (t == TOKEN_CHARACTER_DATA
				|| t == TOKEN_CDATA_VAL){
					if (parseDouble(vn,i) == left->evalNumber(left,vn)){
						vn->contextBuf2->size = stackSize;
						pop2(vn);
						left->reset(left,vn);
						right->reset(right,vn);
						if (op == OP_EQ)
							return TRUE;
						else 
							return FALSE;
					}
			}
		}
		vn->contextBuf2->size = stackSize;
		pop2(vn);
		left->reset(left,vn);
		right->reset(right,vn);
		if (op == OP_EQ)
			return FALSE;
		else 
			return TRUE;
	} Catch (e) {
		//fib1.clear();
		//fib2.clear();
		e.et = other;
		e.msg = "Undefined behavior in evalBoolean_be";
		Throw e;
		//throw new RuntimeException("Undefined behavior");
	}
	return FALSE;
}
static Boolean compStringNodeSet(binaryExpr *be, expr* left, expr* right, VTDNav *vn, opType op){
	exception e;
	int i,t,i1 = 0,stackSize;
	UCSChar *st1;
	Try {
		st1 = left->evalString(left,vn);
		push2(vn);
		stackSize = vn->contextBuf2->size;
		while ((i = right->evalNodeSet(right,vn)) != -1) {
			t = getTokenType(vn,i);
			if (t == TOKEN_STARTING_TAG){
				i1 = getText(vn);
				if (i1==-1)
					break;
				t = getTokenType(vn,i1);
				if (i1 == TOKEN_CHARACTER_DATA){
					if (st1 == NULL || matchTokenString(vn,i1,st1)){
						vn->contextBuf2->size = stackSize;
						pop2(vn);
						left->reset(left,vn);
						right->reset(right,vn);
						if (op == OP_EQ)
							return TRUE;
						else 
							return FALSE;
					}
				}else {
					if (st1 == NULL ||matchRawTokenString(vn,i1, st1)){
						vn->contextBuf2->size = stackSize;
						pop2(vn);
						left->reset(left,vn);
						right->reset(right,vn);
						if (op == OP_EQ)
							return TRUE;
						else 
							return FALSE;
					}
				}
			}
			else if (t == TOKEN_ATTR_NAME
				|| t == TOKEN_ATTR_NS) {
					if (st1 == NULL || matchTokenString(vn,i+1,st1)){
						vn->contextBuf2->size = stackSize;
						pop2(vn);
						left->reset(left,vn);
						right->reset(right,vn);
						if (op == OP_EQ)
							return TRUE;
						else 
							return FALSE;
					}
			}
			else if ( t == TOKEN_CHARACTER_DATA
				|| t == TOKEN_CDATA_VAL){
					if (st1 == NULL || matchTokenString(vn,i,st1)){
						vn->contextBuf2->size = stackSize;
						pop2(vn);
						left->reset(left,vn);
						right->reset(right,vn);
						if (op == OP_EQ)
							return TRUE;
						else 
							return FALSE;
					}
			}else if ( t == TOKEN_CDATA_VAL){
				if (st1 == NULL || matchRawTokenString(vn,i,st1)){
					vn->contextBuf2->size = stackSize;
					pop2(vn);
					left->reset(left,vn);
					right->reset(right,vn);
					if (op == OP_EQ)
						return TRUE;
					else 
						return FALSE;
				}
			}
		}
		vn->contextBuf2->size = stackSize;
		pop2(vn);
		left->reset(left,vn);
		right->reset(right,vn);
		free(st1); // recycle the string object
		if (op == OP_EQ)
			return FALSE;
		else 
			return TRUE;

	} Catch ( e) {
		//fib1.clear();
		//fib2.clear();
		e.et = other;
		e.msg = "undefined run time behavior in computerEQNE";
		Throw e;
	}
	return FALSE;
}

static Boolean compNodeSetNodeSet(binaryExpr *be, expr* left, expr* right, VTDNav *vn, opType op){
	exception e;
	int i,t,i1=0,j,k,s1,s2;
	Try {
		if (be->fib1 == NULL)
			be->fib1 = createFastIntBuffer2(BUF_SZ_EXP);
		if (be->fib2 == NULL)
			be->fib2 = createFastIntBuffer2(BUF_SZ_EXP);

		push2(vn);
		while ((i = be->left->evalNodeSet(be->left,vn)) != -1) {
			t = getTokenType(vn,i);
			if (t == TOKEN_STARTING_TAG)
				i1 = getText(vn);
			if (i1!=-1)
				appendInt(be->fib1,i1);
			else if (t == TOKEN_ATTR_NAME
				|| t == TOKEN_ATTR_NS)
				appendInt(be->fib1,i+1);
			else if (t == TOKEN_CHARACTER_DATA
				|| t == TOKEN_CDATA_VAL)
				appendInt(be->fib1,i);
		}
		pop2(vn);
		push2(vn);
		while ((i = be->right->evalNodeSet(be->right,vn)) != -1) {
			t = getTokenType(vn,i);
			if (t == TOKEN_STARTING_TAG)
				appendInt(be->fib2,getText(vn));
			else if (t == TOKEN_ATTR_NAME
				|| t == TOKEN_ATTR_NS)
				appendInt(be->fib2,i+1);
			else if (t == TOKEN_CHARACTER_DATA
				|| t == TOKEN_CDATA_VAL)
				appendInt(be->fib2,i);
		}
		pop2(vn);
		s1 = be->fib1->size;
		s2 = be->fib2->size;

		// start a while loop comparison
		for (j = 0; j < s1; j++) {
			for (k = 0; k < s2; k++) {
				if (matchTokens(vn,intAt(be->fib1,j), vn, intAt(be->fib2,k))) {
					clearFastIntBuffer(be->fib1);
					clearFastIntBuffer(be->fib2);
					if (op == OP_EQ)
						return TRUE;
					else 
						return FALSE;
				}
			}
		}
		clearFastIntBuffer(be->fib1);
		clearFastIntBuffer(be->fib2);
		if (op == OP_EQ)
			return FALSE;
		else
			return TRUE;

	} Catch (e) {
		if (e.et == out_of_mem){
			Throw e;
		}
		if(be->fib1!=NULL)
			clearFastIntBuffer(be->fib1);
		if (be->fib2 != NULL)
			clearFastIntBuffer(be->fib2);

		e.et = other;
		e.msg = "undefined run time behavior in computerEQNE";
		Throw e;
	}
	return FALSE;
}

Boolean computeEQNE(binaryExpr *be, opType op,VTDNav *vn){
	int i1 = 0;
	Boolean btemp = FALSE;
	UCSChar *st1=NULL, *st2=NULL;

	if (be->left->isNodeSet(be->left) && be->right->isNodeSet(be->right)) {
		return compNodeSetNodeSet(be, be->left, be->right, vn, be->op);
	} else {      
		// first argument is always numerical, second a node set
		if (be->left->isNumerical(be->left) && be->right->isNodeSet(be->right)){
			return compNumericalNodeSet(be, be->left, be->right, vn,be->op);
		}
	    if (be->left->isNodeSet(be->left) && be->right->isNumerical(be->right)){
			return compNumericalNodeSet(be, be->right, be->left, vn,be->op);
		}
		// first argument is always String, second a node set
		if (be->left->isString(be->left) && be->right->isNodeSet(be->right)){
			return compStringNodeSet(be, be->left, be->right,vn,be->op);
		}
		if (be->left->isNodeSet(be->left) && be->right->isString(be->right)){
			return compStringNodeSet(be, be->right, be->left,vn,be->op);
		}

	}
	if (be->left->isBoolean(be->left) || be->right->isBoolean(be->right)){
		if (op == OP_EQ)
			return be->left->isBoolean(be->left) == be->right->isBoolean(be->right);
		else
			return be->left->isBoolean(be->left) != be->right->isBoolean(be->right);
	}

	if (be->left->isNumerical(be->left) || be->right->isNumerical(be->right)){
		if (op == OP_EQ)
			return be->left->evalNumber(be->left,vn) == be->right->evalNumber(be->right,vn);
		else
			return be->left->evalNumber(be->left,vn) != be->right->evalNumber(be->right,vn);
	}
	st1 = be->left->evalString(be->left,vn);
	st2 = be->right->evalString(be->right,vn);
	if (st1 == NULL || st2 == NULL){
		btemp = FALSE;
	}else{
		btemp = wcscmp(st1, st2);
	}
	free(st1);
	free(st2);
	if (op == OP_EQ){
		return btemp==0;
	}
	else
		return btemp!=0;
}

binaryExpr *createBinaryExpr(expr *e1, opType op, expr *e2){
	binaryExpr *be = malloc(sizeof(binaryExpr));
	if (be == NULL){
		throwException2(out_of_mem,
			"binaryExpr allocation failed");
	}
	be->freeExpr = &freeBinaryExpr;
	be->evalBoolean = &evalBoolean_be;
	be->evalNodeSet = &evalNodeSet_be;
	be->evalNumber  = &evalNumber_be;
	be->evalString  = &evalString_be;
	be->isNumerical = &isNumerical_be;
	be->isBoolean = &isBoolean_be;
	be->isString =  &isString_be;
	be->isNodeSet = &isNodeSet_be;
	be->requireContextSize = &requireContextSize_be;
	be->setContextSize = &setContextSize_be;
	be->setPosition = &setPosition_be;
	be->reset = &reset_be;
	be->toString = &toString_be;
	be->left = e1;
	be->op = op;
	be->right = e2;
	be->fib1 = be->fib2 = NULL;
	switch(be->op){
	 	case OP_ADD:
		case OP_SUB:
		case OP_MULT:
		case OP_DIV:
		case OP_MOD: be->isNum = TRUE; be->isBool = FALSE; break;
		case OP_OR :
		case OP_AND:
		case OP_EQ:
		case OP_NE:
		case OP_LE:
		case OP_GE:
		case OP_LT:
		default: be->isNum= FALSE; be->isBool = TRUE;
	}
	return be;
}

void freeBinaryExpr(binaryExpr *be){
	if (be == NULL) return;
	be->left->freeExpr(be->left);
	be->right->freeExpr(be->right);
	freeFastIntBuffer(be->fib1);
	freeFastIntBuffer(be->fib2);
	free(be);
}

int	evalNodeSet_be (binaryExpr *be,VTDNav *vn){
	exception e;
	e.et = xpath_eval_exception;
	e.msg = "can't evaluate nodeset on a binary expr";
	Throw e;
}

double	evalNumber_be (binaryExpr *be,VTDNav *vn){
	switch(be->op){
			case OP_ADD: return be->left->evalNumber(be->left,vn) + be->right->evalNumber(be->right,vn);
			case OP_SUB: return be->left->evalNumber(be->left,vn) - be->right->evalNumber(be->right,vn);
			case OP_MULT:return be->left->evalNumber(be->left,vn) * be->right->evalNumber(be->right,vn);
			case OP_DIV: return be->left->evalNumber(be->left,vn) / be->right->evalNumber(be->right,vn);
			case OP_MOD: return fmod(be->left->evalNumber(be->left,vn), be->right->evalNumber(be->right,vn));
			default	: if (evalBoolean_be(be,vn) == TRUE)
						  return 1;
				return 0;

	}
}


UCSChar* evalString_be  (binaryExpr *be,VTDNav *vn){
	double n = 0.0;
	Boolean b = FALSE;
	UCSChar *tmp;
	if(isNumerical_be(be)){		    
		double d = evalNumber_be(be,vn);
		if (d != d){
			tmp = _wcsdup(L"NaN");
			b= TRUE;
		}
		else if ( d == 1/n){
			tmp = _wcsdup(L"Infinity");
			b = TRUE;
		}
		else if (d == -1/n){
			tmp = _wcsdup(L"-Infinity");
			b  = TRUE;
		}else 	
		tmp = malloc(sizeof(UCSChar)<<8);

		if (tmp == NULL) {
			throwException2(out_of_mem,
				"string allocation in evalString_be failed ");
		}
		if (b)
			return tmp;
		if (d == (Long) d){
			swprintf(tmp,64,L"%d",(Long) d);
		} else {
			swprintf(tmp,64,L"%f", d);
		}
		return tmp;
	} else {
		Boolean b = evalBoolean_be(be,vn);
		if (b)
			tmp= _wcsdup(L"true");
		else
			tmp= _wcsdup(L"false");
		if (tmp == NULL){
			throwException2(out_of_mem,
				"String allocation failed in evalString_be");
		}
		return tmp;

	}
}

Boolean evalBoolean_be (binaryExpr *be,VTDNav *vn){
	// i,i1=0, s1, s2;
	//int stackSize;
	//expr *e1, *e2;
	//int t;
	Boolean b = FALSE;
	double dval;
	switch(be->op){
			case OP_OR: return be->left->evalBoolean(be->left,vn) 
						 || be->right->evalBoolean(be->right,vn);
			case OP_AND:return be->left->evalBoolean(be->left,vn) 
						 && be->right->evalBoolean(be->right,vn);
			case OP_EQ:
			case OP_NE: return computeEQNE(be, be->op,vn);			
			case OP_LE: return be->left->evalNumber(be->left,vn) <= be->right->evalNumber(be->right,vn);
			case OP_GE: return be->left->evalNumber(be->left,vn) >= be->right->evalNumber(be->right,vn);
			case OP_LT: return be->left->evalNumber(be->left,vn) < be->right->evalNumber(be->right,vn);
			case OP_GT: return be->left->evalNumber(be->left,vn) > be->right->evalNumber(be->right,vn);
			default: dval = evalNumber_be(be,vn);
				if (dval ==-0.0 || dval ==+0.0 || (dval!=dval))
					return FALSE;
				return TRUE;
	}
}

Boolean isBoolean_be (binaryExpr *be){
	return be->isBool;
}

Boolean isNumerical_be (binaryExpr *be){
	return be->isNum;
}

Boolean isString_be (binaryExpr *be){
	return FALSE;
}

Boolean isNodeSet_be (binaryExpr *be){
	return FALSE;
}

Boolean requireContextSize_be(binaryExpr *be){
	return be->left->requireContextSize(be->left)
		|| be->right->requireContextSize(be->right);
}

void reset_be(binaryExpr *be, VTDNav *vn){
	be->left->reset(be->left,vn);
	be->right->reset(be->right,vn);
}

void	setContextSize_be(binaryExpr *be,int s){
	be->left->setContextSize(be->left,s);
	be->right->setContextSize(be->right,s);
}

void	setPosition_be(binaryExpr *be,int pos){
	be->left->setPosition(be->left,pos);
	be->right->setPosition(be->right,pos);
}

void    toString_be(binaryExpr *be, UCSChar* string){
	wprintf(L"(");
	be->left->toString(be->left,string);
	switch(be->op){
			case OP_ADD: wprintf(L" + "); break;
			case OP_SUB: wprintf(L" - "); break;
			case OP_MULT: wprintf(L" * "); break;
			case OP_DIV: wprintf(L" / "); break;
			case OP_MOD: wprintf(L" mod "); break;
			case OP_OR : wprintf(L" or ");break;
			case OP_AND: wprintf(L" and "); break;
			case OP_EQ: wprintf(L" = "); break;
			case OP_NE: wprintf(L" != "); break;
			case OP_LE: wprintf(L" <= "); break;
			case OP_GE: wprintf(L" >= "); break;
			case OP_LT: wprintf(L" < "); break;
			default: wprintf(L" > "); break;
	}
	be->right->toString(be->right,string);
	wprintf(L")");
}

UCSChar* createEmptyString(){
	UCSChar* es =  malloc(sizeof(UCSChar));
	if (es!=NULL){
		es[0] = 0;
		return es;
	}
	throwException2(out_of_mem, 
		"string allocation faild in createEmptyString ");
}