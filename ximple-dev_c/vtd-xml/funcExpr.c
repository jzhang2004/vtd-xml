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

static double sum(funcExpr *fne, VTDNav *vn);
static int argCount(funcExpr *fne);
static int count(funcExpr *fne, VTDNav *vn);
static UCSChar *getLocalName(funcExpr *fne, VTDNav *vn);
static UCSChar *getName(funcExpr *fne, VTDNav *vn);
static UCSChar *getNameSpaceURI(funcExpr *fne, VTDNav *vn);
static UCSChar *getString(funcExpr *fne, VTDNav *vn);
static Boolean contains(funcExpr *fne, VTDNav *vn);
static UCSChar* concat(funcExpr *fne, VTDNav *vn);
static Boolean startsWith(funcExpr *fne, VTDNav *vn);
static UCSChar* subString(funcExpr *fne, VTDNav *vn);
static UCSChar* normalizeString(funcExpr *fne, VTDNav *vn);
static UCSChar* subStringBefore(funcExpr *fne, VTDNav *vn);
static UCSChar* subStringAfter(funcExpr *fne, VTDNav *vn);
static Boolean isWS(UCSChar c);
static inline UCSChar* normalize(UCSChar *s);
static double round(double v);
static double round(double v) 
{ 
 return (v>0.0) ? floor(v+0.5) : ceil(v-0.5);
}

static UCSChar *fname(funcExpr *fne,funcName i);

static UCSChar *getString(funcExpr *fne, VTDNav *vn){
	exception e;
	if (argCount(fne)== 0){
		Try{
			if (vn->atTerminal){
				if (getTokenType(vn,vn->LN) == TOKEN_CDATA_VAL )
					return toRawString(vn,vn->LN);
				return toString(vn,vn->LN);
			}
			return toString(vn, getCurrentIndex(vn));
		}
		Catch(e){
			 // this will almost never occur
		}
	}
	else if (argCount(fne) == 1){
		return fne->al->e->evalString(fne->al->e, vn);
	} else {
		e.et = invalid_argument;
		e.msg = "string()'s  <funcExpr> argument count is invalid";
		Throw e;			        
	}
	return createEmptyString();
}

static UCSChar *getLocalName(funcExpr *fne, VTDNav *vn){
	exception e;
	int index;
	int type;
	if (argCount(fne)== 0){
		Try{
			index = getCurrentIndex(vn);
			type = getTokenType(vn,index);
			if (vn->ns && type == TOKEN_STARTING_TAG && type == TOKEN_ATTR_NAME) {
				int offset = getTokenOffset(vn,index);
				int length = getTokenLength(vn,index);
				if (length < 0x10000)
					return toRawString(vn,index);
				else {
					int preLen = length >> 16;
					int QLen = length & 0xffff;
					if (preLen != 0)
						return toRawString2(vn,offset + preLen+1, QLen
						- preLen - 1);
				}
			} 
		}
		Catch(e){
			
		}
	}
	else if (argCount(fne) == 1){
		int a = -1;
		int type;
		push2(vn);
		Try{
			a = fne->al->e->evalNodeSet(fne->al->e,vn);						
			fne->al->e->reset(fne->al->e,vn);
			pop2(vn);						
		}Catch(e){
			fne->al->e->reset(fne->al->e,vn);
			pop2(vn);
		}
		if (a == -1 || vn->ns == FALSE)
			return createEmptyString();
		type = getTokenType(vn,a);

		if (type != TOKEN_STARTING_TAG && type != TOKEN_ATTR_NAME)
			return createEmptyString();
		Try {			    
			int offset = getTokenOffset(vn,a);
			int length = getTokenLength(vn,a);
			if (length < 0x10000)
				return toRawString(vn,a);
			else {
				int preLen = length >> 16;
				int QLen = length & 0xffff;
				if (preLen != 0)
					return toRawString2(vn, offset + preLen+1, 
					QLen - preLen - 1);
			}
		} Catch (e) {
			 // this will almost never occur
		}		
	} else {
		e.et = invalid_argument;
		e.msg = "local-name()'s  <funcExpr> argument count is invalid";
		Throw e;			        
	}
	return createEmptyString();
}
static UCSChar *getNameSpaceURI(funcExpr *fne, VTDNav *vn){
		exception e;
	if (argCount(fne)== 0){
		Try{
			int i = getCurrentIndex(vn);
			int type = getTokenType(vn,i);
			if (vn->ns && (type == TOKEN_STARTING_TAG 
				|| type == TOKEN_ATTR_NAME)) {
				int a = lookupNS(vn);
				if (a == 0)
					return createEmptyString();
				else
					return toString(vn,a);
			}			
		}Catch (e){			
		}
	}
	else if (argCount(fne) == 1){
		int a = -1;
		push2(vn);
		Try{
			a = fne->al->e->evalNodeSet(fne->al->e,vn);
			fne->al->e->reset(fne->al->e,vn);
			pop2(vn);
		}Catch(e){
			fne->al->e->reset(fne->al->e,vn);
			pop2(vn);
		}
		Try{
			if (a==-1 || vn->ns == FALSE){
				return createEmptyString();
			}
			else {
				int type = getTokenType(vn,a);
				if (type == TOKEN_STARTING_TAG 
					|| type == TOKEN_ATTR_NAME)
					return toString(vn,lookupNS(vn));
			}
		}Catch(e){}		
	} else {
		e.et = invalid_argument;
		e.msg = "namespace-uri()'s  <funcExpr> argument count is invalid";
		Throw e;			        
	}
	return createEmptyString();
}

static UCSChar *getName(funcExpr *fne, VTDNav *vn){
	exception e;
	int a;
	int type;
	if (argCount(fne)== 0){
		a = getCurrentIndex(vn);
		type = getTokenType(vn,a);
		if ( type == TOKEN_STARTING_TAG
			|| type == TOKEN_ATTR_NAME){
			Try{
				return toString(vn,a);
			}Catch(e){
				return createEmptyString();
			}            
		}
		else 
			return createEmptyString();
	}
	else if (argCount(fne) == 1){
		a = -1;		
		push2(vn);
		Try{
			a = fne->al->e->evalNodeSet(fne->al->e, vn);	    
			fne->al->e->reset(fne->al->e,vn);
			pop2(vn);						
		}Catch(e){
			fne->al->e->reset(fne->al->e,vn);
			pop2(vn);
		}	        
		Try{
			if (a == -1 || vn->ns == FALSE){
				return createEmptyString();
			}				    
			else{
				int type = getTokenType(vn,a);
				if ( type== TOKEN_STARTING_TAG || type == TOKEN_ATTR_NAME)
					return toString(vn,a);
				return createEmptyString();
			}			
		}Catch(e){
		}
		
		//return fne->al->e->evalString(fne->al->e, vn);
	} else {
		e.et = invalid_argument;
		e.msg = "name()'s  <funcExpr> argument count is invalid";
		Throw e;			        
	}	
	return createEmptyString();
}


aList *createAlist(){
	exception e;
	aList *al = (aList *)malloc(sizeof(aList));
	if (al==NULL){
		e.et = out_of_mem;
		e.msg = "funcExpr allocation failed ";
		Throw e;
	}
	al->next  = NULL;
	return al;
}

void  freeAlist(aList *al){
	aList *temp = al;
	while(temp!=NULL){
		if (temp->e!= NULL )
			(temp->e->freeExpr)(temp->e);
		temp = temp->next;
	}
}

void  reset_al(aList *al, VTDNav *vn){
	aList *temp = al;
	while(temp!=NULL){
		if (temp->e!= NULL )
			(temp->e->reset)(temp->e,vn);
		temp = temp->next;
	}
}

void  toString_al(aList *al, UCSChar* string){
	aList *temp = al;
	
	while(temp!=NULL){
		(temp->e->toString)(temp->e, string);
		temp = temp->next;
		if (temp!=NULL)
			wprintf(L",");
	}
}


double sum(funcExpr *fne, VTDNav *vn){
	exception e;
	double d = 0;
	double n=0;
	int i1;
	if (argCount(fne) != 1 
		|| fne->al->e->isNodeSet(fne->al->e) == FALSE){
			e.et = invalid_argument;
			e.msg = "sum() <funcExpr> 's argument has to be a node set ";
			Throw e;
		}
		
	push2(vn);
	Try {
		fne->a = 0;
		while ((fne->a = fne->al->e->evalNodeSet(fne->al->e,vn)) != -1) {
			tokenType t = getTokenType(vn,fne->a);
			if (t == TOKEN_STARTING_TAG){
				i1 = getText(vn);
				if (i1!=-1)
					d += parseDouble(vn,i1);
				if (d!=d)
					break;
			}
			else if (t == TOKEN_ATTR_NAME
				|| t == TOKEN_ATTR_NS){
					d += parseDouble(vn,fne->a+1);
					if (d!=d)
						break;
				}
			else if (t == TOKEN_CHARACTER_DATA
				|| t == TOKEN_CDATA_VAL){
					d += parseDouble(vn, fne->a);
					if (d != d)
						break;
				}
				//    fib1.append(i);
		}
		fne->al->e->reset(fne->al->e,vn);
		pop2(vn);
		return d;
	} Catch (e) {
		fne->al->e->reset(fne->al->e,vn);
		pop2(vn);
	}   
	return n/n;
}

int argCount(funcExpr *fne){
	aList *temp = fne->al;
	int count = 0;
	while(temp!=NULL){
		count++;
		temp = temp->next;
	}
	return count;
}
int count(funcExpr *fne, VTDNav *vn){
	exception e;
	int a = -1;
	
	if (argCount(fne)!=1 
		||(fne->al->e->isNodeSet)(fne->al->e)==FALSE){
			e.et = invalid_argument;
			e.msg = "count <funcExpr> 's argument has to be a node set ";
			Throw e;
		}

	push2(vn);
	Try{
		a = 0;
		while(fne->al->e->evalNodeSet(fne->al->e,vn)!=-1){
			a ++;
		}
		fne->al->e->reset(fne->al->e,vn);
		pop2(vn);
		
	}Catch(e){
		fne->al->e->reset(fne->al->e,vn);
		pop2(vn);
	}
	return a;
}


UCSChar *fname(funcExpr *fne, funcName i){
	switch(fne->opCode){
			case FN_LAST: 			return L"last";
			case FN_POSITION: 		return L"position";
			case FN_COUNT: 			return L"count";
			case FN_LOCAL_NAME: 		return L"local-name";
			case FN_NAMESPACE_URI: 		return L"namespace-uri";
			case FN_NAME: 			return L"name";
			case FN_STRING: 			return L"string";
			case FN_CONCAT: 			return L"concat";
			case FN_STARTS_WITH:		return L"starts-with";
			case FN_CONTAINS: 		return L"contains";
			case FN_SUBSTRING_BEFORE: 	return L"substring_before";
			case FN_SUBSTRING_AFTER: 		return L"substring_after";
			case FN_SUBSTRING: 		return L"substring";
			case FN_STRING_LENGTH: 		return L"string-length";
			case FN_NORMALIZE_SPACE: 		return L"normalize-space";
			case FN_TRANSLATE:	 	return L"translate";
			case FN_BOOLEAN: 			return L"boolean";
			case FN_NOT: 			return L"not";
			case FN_TRUE: 			return L"true";
			case FN_FALSE: 			return L"false";
			case FN_LANG: 			return L"lang";
			case FN_NUMBER:			return L"number";
			case FN_SUM: 			return L"sum";
			case FN_FLOOR: 			return L"floor";
			case FN_CEILING: 			return L"ceiling";
			default:			return L"round";
	}
}

funcExpr *createFuncExpr(funcName oc, aList *a){
	exception e;
	funcExpr *fne = (funcExpr *)malloc(sizeof(funcExpr));
	if (fne==NULL){
		e.et = out_of_mem;
		e.msg = "funcExpr allocation failed ";
		Throw e;
	}

	fne->freeExpr = &freeFuncExpr;
	fne->evalBoolean = &evalBoolean_fne;
	fne->evalNodeSet = &evalNodeSet_fne;
	fne->evalNumber  = &evalNumber_fne;
	fne->evalString  = &evalString_fne;
	fne->isNumerical = &isNumerical_fne;
	fne->isBoolean = &isBoolean_fne;
	fne->isString =  &isString_fne;
	fne->isNodeSet = &isNodeSet_fne;
	fne->requireContextSize = &requireContextSize_fne;
	fne->setContextSize = &setContextSize_fne;
	fne->setPosition = &setPosition_fne;
	fne->reset = &reset_fne;
	fne->toString = &toString_fne;
	fne->a = 0;
	fne->opCode = oc;
	fne->al = a;
	fne->isBool = fne->isStr= fne->isNum = FALSE;
	 switch(oc){
			case FN_LAST: 			fne->isNum = TRUE;break;
			case FN_POSITION: 		fne->isNum = TRUE;break;
			case FN_COUNT: 			fne->isNum = TRUE;break;
			case FN_LOCAL_NAME: 		fne->isStr = TRUE; break;
			case FN_NAMESPACE_URI: 	fne->isStr = TRUE; break;
			case FN_NAME: 			fne->isStr = TRUE; break;
			case FN_STRING: 			fne->isStr = TRUE; break;
			case FN_CONCAT: 			fne->isStr = TRUE; break;
			case FN_STARTS_WITH:		fne->isBool= TRUE;break;
			case FN_CONTAINS: 		fne->isBool= TRUE;break;
			case FN_SUBSTRING_BEFORE: fne->isStr = TRUE; break;
			case FN_SUBSTRING_AFTER: 	fne->isStr = TRUE; break;
			case FN_SUBSTRING: 		fne->isStr = TRUE; break;
			case FN_STRING_LENGTH: 	fne->isNum = TRUE;break;
			case FN_NORMALIZE_SPACE: 	fne->isStr = TRUE; break;
			case FN_TRANSLATE:	 	fne->isStr= TRUE;break;
			case FN_BOOLEAN: 			fne->isBool =TRUE;break;
			case FN_NOT: 			    fne->isBool =TRUE;break;
			case FN_TRUE: 			fne->isBool = TRUE;break;
			case FN_FALSE: 			fne->isBool = TRUE;break;
			case FN_LANG: 			fne->isBool = TRUE;break;
			case FN_NUMBER:			fne->isNum = TRUE;break;
			case FN_SUM: 			    fne->isNum = TRUE;break;
			case FN_FLOOR: 			fne->isNum = TRUE;break;
			case FN_CEILING: 			fne->isNum = TRUE;break;
			default:			fne->isNum = TRUE;
	  }	  
	return fne;
}
void freeFuncExpr(funcExpr *fne){
	if (fne ==NULL) return;
	if (fne->al!=NULL)
		freeAlist(fne->al);
	free(fne);
}

int	evalNodeSet_fne (funcExpr *fne,VTDNav *vn){
	exception e;
	e.et = xpath_eval_exception;
	e.msg = "funcExpr can't eval to a node set!";
	Throw e;
}
double	evalNumber_fne (funcExpr *fne,VTDNav *vn){
	exception e;
	int ac;
	size_t len;
	UCSChar *tmpString = NULL;
	switch(fne->opCode){
			case FN_LAST:  if (argCount(fne)!=0 ){
								e.et = invalid_argument;
								e.msg = "floor()'s <funcExpr> argument count is invalid";
								Throw e;
							}
						   return fne->contextSize;			
			case FN_POSITION:   if (argCount(fne)!=0 ){
									e.et = invalid_argument;
									e.msg = "position()'s  <funcExpr> argument count is invalid";
									Throw e;
								}
										 
								return fne->position;

			case FN_COUNT: 		return count(fne, vn);

			case FN_NUMBER:		if (argCount(fne)!=1){
									e.et = invalid_argument;
									e.msg = "number()'s  <funcExpr> argument count is invalid";
									Throw e;
								}
								return fne->al->e->evalNumber(fne->al->e,vn);

			case FN_SUM:	    return sum(fne,vn);
			case FN_FLOOR: 		if (argCount(fne)!=1 ){
									e.et = invalid_argument;
									e.msg = "floor()'s  <funcExpr> argument count is invalid";
									Throw e;
								}
								return floor(fne->al->e->evalNumber(fne->al->e,vn));

			case FN_CEILING:	if (argCount(fne)!=1 ){
									e.et = invalid_argument;
									e.msg = "ceiling()'s  <funcExpr> argument count is invalid";
									Throw e;
								}
								return ceil(fne->al->e->evalNumber(fne->al->e,vn));

			case FN_STRING_LENGTH:
								ac = argCount(fne);
			    				if (ac == 0){
			    				    Try{
			    				        if (vn->atTerminal == TRUE){
			    				            tokenType type = getTokenType(vn,vn->LN);
			    				            
											if (type == TOKEN_ATTR_NAME 
			    				                || type == TOKEN_ATTR_NS){
													tmpString = toString(vn,vn->LN+1);
			    				                //return vn.toString(vn.LN+1).length();
			    				            } else {
												tmpString = toString(vn,vn->LN+1);
			    				                //return vn.toString(vn.LN).length();
			    				            }
											len = wcslen(tmpString);
											free(tmpString);
											return len;
											
			    				        }else {
			    				            int i = getText(vn);
			    				            if (i==-1)
			    				                return 0;
											else {
												tmpString = toString(vn,vn->LN+1);
												len = wcslen(tmpString);
												free(tmpString);
			    				                return len;
											}
			    				        }
			    				    }Catch ( e){
			    				        return 0;
			    				    }
			    				} else if (ac == 1){
									tmpString = fne->al->e->evalString(fne->al->e,vn);
									len = wcslen(tmpString);
									free(tmpString);
			    				    return len;
			    				} else {
			    				   	e.et = invalid_argument;
									e.msg = "string-length()'s  <funcExpr> argument count is invalid";
									Throw e;
			    				}

			case FN_ROUND: 	if (argCount(fne)!=1 )
										return round(fne->al->e->evalNumber(fne->al->e,vn));

			default: if (fne->isBool){
						if (evalBoolean_fne(fne,vn))
							return 1;
						else
							return 0;
					 }else {
                         	double d  = 0;
							UCSChar *string = evalString_fne(fne,vn);
							UCSChar *temp;
							double result = wcstod(string,&temp);
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
							free(string);
							return result;
					
					 }

					 }
}

UCSChar* evalString_fne (funcExpr *fne, VTDNav *vn){
	exception e;
	UCSChar *tmp;

	switch(fne->opCode){
			case FN_LOCAL_NAME: 	
				return getLocalName(fne,vn);
			case FN_NAMESPACE_URI: 	
				return getNameSpaceURI(fne, vn);
			case FN_NAME: 		
				return getName(fne,vn);			    
			case FN_STRING:
				return getString(fne,vn);

			case FN_CONCAT:
				return concat(fne,vn);
			case FN_SUBSTRING_BEFORE: return subStringBefore(fne,vn);		
			case FN_SUBSTRING_AFTER: return subStringAfter(fne,vn);
									
			case FN_SUBSTRING: 	return subString(fne,vn);	
			case FN_TRANSLATE: 	e.et = other;
								e.msg = "Some functions are not supported";
								Throw e;
			case FN_NORMALIZE_SPACE: return normalizeString(fne,vn);
								
			default: if (isBoolean_fne(fne)){
			    		if (evalBoolean_fne(fne,vn)== TRUE)
			    		    tmp = _wcsdup(L"true");
			    		else 
			    		    tmp = _wcsdup(L"false");
						if (tmp == NULL){
							e.et = out_of_mem;
							e.msg = "allocate string failed in funcExpr's evalString()";
							Throw e;
						}
						return tmp;
					 } else {
						 double d1 = 0;
						 double d = evalNumber_fne(fne,vn);
						 Boolean b = FALSE;
							if (d != d){
								tmp = _wcsdup(L"NaN");
								b = TRUE;
							}
							else if ( d == 1/d1){
								tmp = _wcsdup(L"Infinity");
								b = TRUE;
							}
							else if (d == -1/d1){
								tmp = _wcsdup(L"-Infinity");
								b = TRUE;
							}	else 
								tmp = malloc(sizeof(UCSChar)<<8);
		
							if (tmp == NULL) {
									e.et = out_of_mem;
									e.msg = "allocate string failed in funcExpr's evalString()";
									Throw e;
							}
							if(b)
								return tmp;

						    if (d == (Long) d){		
								swprintf(tmp,64,L"%d",(Long)d);
							} else {
								swprintf(tmp,64,L"%f", d);
							}
							return tmp;					     
					 }
	  }
}

/* evaluate boolean value of a functional expression*/
Boolean evalBoolean_fne (funcExpr *fne,VTDNav *vn){
	exception e;
	switch(fne->opCode){
			case FN_STARTS_WITH:			
				if (argCount(fne)!=2){
					e.et = invalid_argument;
					e.msg = "starts-with()'s <funcExpr> argument count is invalid";
					Throw e;
				}
				return startsWith(fne,vn);

			case FN_CONTAINS:
				if (argCount(fne)!=2){
					e.et = invalid_argument;
					e.msg = "contains()'s <funcExpr> argument count is invalid";
					Throw e;
				}
				return contains(fne,vn);

			case FN_TRUE: 
				if (argCount(fne)!=0){
					e.et = invalid_argument;
					e.msg = "true()'s <funcExpr> argument count is invalid";
					Throw e;
				}
				return TRUE;			
			case FN_FALSE:
				if (argCount(fne)!=0){
					e.et = invalid_argument;
					e.msg = "false()'s <funcExpr> argument count is invalid";
					Throw e;
				}						  
				return FALSE;	
			case FN_BOOLEAN:
				if (argCount(fne)!=1){
					e.et = invalid_argument;
					e.msg = "boolean()'s <funcExpr> argument count is invalid";
					Throw e;
				}
				return fne->al->e->evalBoolean(fne->al->e, vn);	
			case FN_NOT:	
				if (argCount(fne)!=1){
					e.et = invalid_argument;
					e.msg = "not()'s <funcExpr> argument count is invalid";
					Throw e;
				}
				return !fne->al->e->evalBoolean(fne->al->e,vn);

			default: 
				if (isNumerical_fne(fne)){
					double d = evalNumber_fne(fne, vn);
					if (d==0 || d!=d)
						return FALSE;
					return TRUE;
				}else{
					UCSChar *tmp = evalString_fne(fne, vn);
					size_t len = wcslen(tmp);
					free(tmp);
					return len!=0;
				}
	}
}

Boolean isBoolean_fne (funcExpr *fne){
	return fne->isBool;
}

Boolean isNumerical_fne (funcExpr *fne){
	return fne->isNum;
}

Boolean isString_fne (funcExpr *fne){
	return fne->isStr;
}

Boolean isNodeSet_fne (funcExpr *fne){
	return FALSE;
}

Boolean requireContextSize_fne(funcExpr *fne){
	    if (fne->opCode == FN_LAST)
	        return TRUE;
	    else {
			aList *temp = fne->al;
	        while(temp!=NULL){
	            if (temp->e->requireContextSize(temp->e)){
	                return TRUE;
	            }
	            temp = temp->next;
	        }
	    }
	    return FALSE;
}

void	reset_fne(funcExpr *fne, VTDNav *vn){
		fne->a = 0;
	    //contextSize = 0;
		if (fne->al!=NULL)
			reset_al(fne->al,vn);
}

void	setContextSize_fne(funcExpr *fne,int s){
	    if (fne->opCode == FN_LAST){
	        fne->contextSize = s;
	        //System.out.println("contextSize: "+size);
	    } else {
	        aList *temp = fne->al;
	        //boolean b = false;
	        while(temp!=NULL){
	            temp->e->setContextSize(temp->e,s);
	            temp = temp->next;
	        }
	    }
}

void	setPosition_fne(funcExpr *fne,int pos){
	    if (fne->opCode == FN_POSITION){
	        fne->position = pos;
	        //System.out.println("PO: "+size);
	    } else {
	        aList *temp = fne->al;
	        while(temp!=NULL){
	            temp->e->setPosition(temp->e,pos);
	            temp = temp->next;
	        }
	    }
}

void   toString_fne(funcExpr *fne, UCSChar* string){
	if (fne->al == NULL){
		wprintf(L"%ls()",fname(fne,fne->opCode));
	}else {
		wprintf(L"%ls(",fname(fne,fne->opCode));
		toString_al(fne->al,string);
		wprintf(L")");
	}	
}
static Boolean contains(funcExpr *fne, VTDNav *vn){
	UCSChar* s1 = fne->al->e->evalString(fne->al->e, vn);
	UCSChar* s2 = fne->al->next->e->evalString(fne->al->next->e,vn);
	Boolean b = FALSE;
	if (s1 == NULL || s2 == NULL)
		return FALSE;
	if (wcsstr(s1,s2)!=NULL)
		b = TRUE;
	free(s1);
	free(s2);
    return b;
}
static UCSChar* concat(funcExpr *fne, VTDNav *vn){	
	exception e;
	int totalLen = 0,len = 0,capacity = 16;
	UCSChar *result = NULL, *s = NULL, *tempBuf = NULL;
	result = malloc(sizeof(UCSChar)<<4);
	if (result==NULL){
		e.et = out_of_mem;
		e.msg = "String allocation failed in concat";
		Throw e;
	}
	result[0]=0;/*end of string set*/
	
	if (argCount(fne)>=2){
		aList* temp = fne->al;
		while(temp!=NULL){
			/*perform concatenation here*/
			s = temp->e->evalString(temp->e,vn);
			len = wcslen(s);
			totalLen += len;
			if (totalLen+1> capacity){
				while(totalLen+1> capacity)
					capacity = (capacity<<1);
				tempBuf = result;
				result = realloc(result, capacity*sizeof(UCSChar));
				if (result == NULL) {
					free(tempBuf);
					free(s);
					e.et = out_of_mem;
					e.msg = "String allocation failed in concat";
					Throw e;
				}
			}
			wcscat(result,s);			
			temp = temp->next;
		}
		return result;
	}
	e.et = invalid_argument;							
	e.msg = "concat()'s <funcExpr> argument count is invalid";
	Throw e;

}
static Boolean startsWith(funcExpr *fne, VTDNav *vn){
	UCSChar* s1 = fne->al->e->evalString(fne->al->e, vn);
	UCSChar* s2 = fne->al->next->e->evalString(fne->al->next->e,vn);
	UCSChar* s3 = NULL;
	Boolean b = FALSE;
	if (wcsstr(s1,s2)==s1)
		b = TRUE;
	free(s1);
	free(s2);
    return b;
}
static UCSChar* subString(funcExpr *fne, VTDNav *vn){
	exception e;
	UCSChar *str;
	int len;

	if (argCount(fne)==2){
		double d1;
		int temp;
		str = fne->al->e->evalString(fne->al->e,vn);
		len = wcslen(str);
		d1 = fne->al->next->e->evalNumber(fne->al->next->e,vn);
		if (d1!=d1 || d1>len)
			return createEmptyString();
		temp = max((int)d1-1,0);
		wcsncpy(str,str+temp,len-temp);
		*(str+len-temp) = 0;
		return str;
	} else if (argCount(fne)==3){
		double d1, d2;
		int temp1, temp2;
		str = fne->al->e->evalString(fne->al->e,vn);
		len = wcslen(str);
		d1 = floor(fne->al->next->e->evalNumber(fne->al->next->e,vn)+0.5);
		d2 = floor(fne->al->next->next->e->evalNumber(fne->al->next->next->e,vn)+0.5);
		if ((d1+d2)!=(d1+d2) || d1>len)
			return createEmptyString();
		temp1 = max((int)d1-1,0);
		temp2 = min(len,(int)d1-1+(int)d2);
		wcsncpy(str,str+temp1,temp2-temp1);
		*(str+temp2-temp1)=0;
		return str;
	}
	e.et = invalid_argument;							
	e.msg = "substring()'s <funcExpr> argument count is invalid";
	Throw e;
}




static UCSChar* subStringBefore(funcExpr *fne, VTDNav *vn){
	exception e;
	if (argCount(fne) == 2){
		UCSChar* s1 = fne->al->e->evalString(fne->al->e, vn);
		UCSChar* s2 = fne->al->next->e->evalString(fne->al->next->e,vn);
		UCSChar* temp = NULL;
		if ((temp=wcsstr(s1,s2))!=NULL){
			*(temp) = 0;
			free(s2);
			return s1;
		}
		else{
			*s2 = 0;
			free(s1);
			return s2;
		}
	}
	e.et = invalid_argument;							
	e.msg = "substring-before()'s <funcExpr> argument count is invalid";
	Throw e;
}

static UCSChar* subStringAfter(funcExpr *fne, VTDNav *vn){
	exception e;
	if (argCount(fne) == 2){
		UCSChar* s1 = fne->al->e->evalString(fne->al->e, vn);
		UCSChar* s2 = fne->al->next->e->evalString(fne->al->next->e,vn);
		int len1=wcslen(s1),len2=wcslen(s2);
		UCSChar* temp = NULL;
		if ((temp=wcsstr(s1,s2))!=NULL){
			int len = wcslen(temp);
			wcsncpy(s1,temp+len2,len-len2);
			*(s1+len-len2) = 0;
			free(s2);
			return s1;
		}
		else{
			*s2 = 0;
			free(s1);
			return s2;
		}
	}
	e.et = invalid_argument;							
	e.msg = "substring-after()'s <funcExpr> argument count is invalid";
	Throw e;
}


static UCSChar* normalizeString(funcExpr *fne, VTDNav *vn){
	exception e;
	if (argCount(fne) == 0){
		UCSChar *s = NULL;
		Try{
			if (vn->atTerminal)
			{
				if (getTokenType(vn,vn->LN) == TOKEN_CDATA_VAL)
					s =toRawString(vn,vn->LN);
				s = toString(vn,vn->LN);
			}
			s = toString(vn,getCurrentIndex(vn));
			return normalize(s);
		}
		Catch(e){
			return createEmptyString();
		}
	} else if (argCount(fne) ==1){
		UCSChar *s = fne->al->e->evalString(fne->al->e, vn);
		return normalize(s);
	}

	e.et = invalid_argument;							
	e.msg = "normalize-space()'s <funcExpr> argument count is invalid";
	Throw e;
}

static UCSChar* normalize(UCSChar *s){
	 int os=0,i,len;
	 len = wcslen(s);
	 // strip off leading ws
	 for (i = 0; i < len; i++)
	 {
		 if (isWS(s[i]))
		 {
		 }
		 else
		 {
			 break;
		 }
	 }
	 while (i < len)
	 {
		 UCSChar c = s[i];
		 if (!isWS(c))
		 {
			 s[os++]=c;
			 i++;
		 }
		 else
		 {
			 while (i < len)
			 {
				 c = s[i];
				 if (isWS(c))
					 i++;
				 else
					 break;
			 }
			 if (i < len){
				 s[os++]=' ';
			 }
		 }
	 }
	 s[os]=0;
	 return s;
}

static Boolean isWS(UCSChar c)
{
	if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
		return TRUE;
	return FALSE;
}
