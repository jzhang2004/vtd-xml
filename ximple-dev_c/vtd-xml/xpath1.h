/* 
 * Copyright (C) 2002-2004 XimpleWare, info@ximpleware.com
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

/* This file defines various expression types,
	expr is the base structure to which all other expression types can be cast to
*/

#ifndef X1_H
#define X1_H
#include "customTypes.h"
#include "vtdNav.h"
#include "autoPilot.h"
// expr doesn't have a constructor
/*typedef struct Expr {
	free_Expr freeExpr;
	eval_NodeSet evalNodeSet;
	eval_Number evalNumber;
	eval_String evalString;
	eval_Boolean evalBoolean;
	is_Boolean isBoolean;
	is_Numerical is_Numerical;
	require_ContextSize requireContextSize;
	reset_ reset;
	set_ContextSize setContextSize;
	set_Position setPosition;
	to_String toString;
} expr; */
/*typedef struct Expr {
	void(*free_Expr)(expr *e) freeExpr;
	eval_NodeSet evalNodeSet;
	eval_Number evalNumber;
	eval_String evalString;
	eval_Boolean evalBoolean;
	is_Boolean isBoolean;
	is_Numerical is_Numerical;
	require_ContextSize requireContextSize;
	reset_ reset;
	set_ContextSize setContextSize;
	set_Position setPosition;
	to_String toString;
} expr; */ 

// definition for struct intHash
#define mask1 0x1ff
#define mask2 0xfffffe00
#define hashWidth 512
#define pageSizeE 6
typedef struct intHash {
   struct fastIntBuffer **storage;	
} IntHash;

// function for intHash

IntHash* createIntHash();
void freeIntHash(IntHash *ih);
Boolean isUniqueIntHash(IntHash *ih,int i);
void resetIntHash(IntHash *ih);

// define abstract functions in the base expr
typedef void (*free_Expr) (struct Expr *e);
typedef int(*eval_NodeSet) (struct Expr *e, VTDNav *vn);
typedef double(*eval_Number) (struct Expr *e, VTDNav *vn);
typedef UCSChar*(*eval_String)  (struct Expr *e, VTDNav *vn);
typedef Boolean (*eval_Boolean) (struct Expr *e, VTDNav *vn);
typedef Boolean (*is_Boolean) (struct Expr *e);
typedef Boolean (*is_Numerical) (struct Expr *e);
typedef Boolean (*is_String) (struct Expr *e);
typedef Boolean (*is_NodeSet) (struct Expr *e);
typedef Boolean (*require_ContextSize)(struct Expr *e);
typedef void	(*reset_)(struct Expr *e, VTDNav *vn);
typedef void	(*set_ContextSize)(struct Expr *e, int s);
typedef void	(*set_Position)(struct Expr *e, int pos);
typedef void    (*to_String)(struct Expr *e, UCSChar* string);

typedef struct Expr {
	free_Expr freeExpr;
	eval_NodeSet evalNodeSet;
	eval_Number evalNumber;
	eval_String evalString;
	eval_Boolean evalBoolean;
	is_Boolean isBoolean;
	is_Numerical isNumerical;
	is_String  isString;
	is_NodeSet isNodeSet;	
	require_ContextSize requireContextSize;
	reset_ reset;
	set_ContextSize setContextSize;
	set_Position setPosition;
	to_String toString;
} expr;


// LiteralExpr 
typedef struct LiteralExpr {
	free_Expr freeExpr;
	eval_NodeSet evalNodeSet;
	eval_Number evalNumber;
	eval_String evalString;
	eval_Boolean evalBoolean;
	is_Boolean isBoolean;
	is_Numerical isNumerical;
	is_String  isString;
	is_NodeSet isNodeSet;	
	require_ContextSize requireContextSize;
	reset_ reset;
	set_ContextSize setContextSize;
	set_Position setPosition;
	to_String toString;
	UCSChar *s;
} literalExpr;

// functions for LiteralExpr 
literalExpr *createLiteralExpr(UCSChar *st);
void freeLiteralExpr(literalExpr *e);
int		evalNodeSet_le (literalExpr *e,VTDNav *vn);
double	evalNumber_le (literalExpr *e,VTDNav *vn);
UCSChar* evalString_le  (literalExpr *e,VTDNav *vn);
Boolean evalBoolean_le (literalExpr *e,VTDNav *vn);
Boolean isBoolean_le (literalExpr *e);
Boolean isNumerical_le (literalExpr *e);
Boolean isString_le (literalExpr *e);
Boolean isNodeSet_le (literalExpr *e);
Boolean requireContextSize_le(literalExpr *e);
void	reset_le(literalExpr *e, VTDNav *vn);
void	setContextSize_le(literalExpr *e,int s);
void	setPosition_le(literalExpr *e,int pos);
void    toString_le(literalExpr *e, UCSChar* string);

// number expression
typedef struct NumberExpr {
	free_Expr freeExpr;
	eval_NodeSet evalNodeSet;
	eval_Number evalNumber;
	eval_String evalString;
	eval_Boolean evalBoolean;
	is_Boolean isBoolean;
	is_Numerical isNumerical;
	is_String  isString;
	is_NodeSet isNodeSet;
	require_ContextSize requireContextSize;
	reset_ reset;
	set_ContextSize setContextSize;
	set_Position setPosition;
	to_String toString;
	double dval;
} numberExpr;

// functions for numberExpr
numberExpr *createNumberExpr(double d);
void freeNumberExpr(numberExpr *e);
int		evalNodeSet_ne (numberExpr *e,VTDNav *vn);
double	evalNumber_ne (numberExpr *e,VTDNav *vn);
UCSChar* evalString_ne  (numberExpr *e,VTDNav *vn);
Boolean evalBoolean_ne (numberExpr *e,VTDNav *vn);
Boolean isBoolean_ne (numberExpr *e);
Boolean isNumerical_ne (numberExpr *e);
Boolean isString_ne (numberExpr *e);
Boolean isNodeSet_ne (numberExpr *e);
Boolean requireContextSize_ne(numberExpr *e);
void	reset_ne(numberExpr *e, VTDNav *vn);
void	setContextSize_ne(numberExpr *e,int s);
void	setPosition_ne(numberExpr *e,int pos);
void    toString_ne(numberExpr *e, UCSChar* string);

// binary Expr
// define operand
typedef enum OpType{		
 	 OP_ADD,
	 OP_SUB,
	 OP_MULT,
	 OP_DIV,
	 OP_MOD,
	 OP_OR ,
	 OP_AND,
	 OP_EQ,
	 OP_NE,
	 OP_LE,
	 OP_GE,
	 OP_LT,
	 OP_GT
} opType;

typedef struct BinaryExpr {
	free_Expr freeExpr;
	eval_NodeSet evalNodeSet;
	eval_Number evalNumber;
	eval_String evalString;
	eval_Boolean evalBoolean;
	is_Boolean isBoolean;
	is_Numerical isNumerical;	
	is_String  isString;
	is_NodeSet isNodeSet;
	require_ContextSize requireContextSize;
	reset_ reset;
	set_ContextSize setContextSize;
	set_Position setPosition;
	to_String toString;
	expr *left;
	opType op;
	expr *right;
	FastIntBuffer *fib1;
	FastIntBuffer *fib2;
	Boolean isBool;
	Boolean isNum;
} binaryExpr;

binaryExpr *createBinaryExpr(expr *e1, opType op, expr *e2);
void freeBinaryExpr(binaryExpr *e);
int		evalNodeSet_be (binaryExpr *e,VTDNav *vn);
double	evalNumber_be (binaryExpr *e,VTDNav *vn);
UCSChar* evalString_be  (binaryExpr *e,VTDNav *vn);
Boolean evalBoolean_be (binaryExpr *e,VTDNav *vn);
Boolean isBoolean_be (binaryExpr *e);
Boolean isNumerical_be (binaryExpr *e);
Boolean isString_be (binaryExpr *e);
Boolean isNodeSet_be (binaryExpr *e);
Boolean requireContextSize_be(binaryExpr *e);
void	reset_be(binaryExpr *e, VTDNav *vn);
void	setContextSize_be(binaryExpr *e,int s);
void	setPosition_be(binaryExpr *e,int pos);
void    toString_be(binaryExpr *e, UCSChar* string);

// unary Expr
typedef struct UnaryExpr {
	free_Expr freeExpr;
	eval_NodeSet evalNodeSet;
	eval_Number evalNumber;
	eval_String evalString;
	eval_Boolean evalBoolean;
	is_Boolean isBoolean;
	is_Numerical isNumerical;
	is_String  isString;
	is_NodeSet isNodeSet;
	require_ContextSize requireContextSize;
	reset_ reset;
	set_ContextSize setContextSize;
	set_Position setPosition;
	to_String toString;
	opType op;
	expr *e;
} unaryExpr;

unaryExpr *createUnaryExpr(opType op, expr *e1);
void freeUnaryExpr(unaryExpr *e);
int		evalNodeSet_ue (unaryExpr *e,VTDNav *vn);
double	evalNumber_ue (unaryExpr *e,VTDNav *vn);
UCSChar* evalString_ue  (unaryExpr *e,VTDNav *vn);
Boolean evalBoolean_ue (unaryExpr *e,VTDNav *vn);
Boolean isBoolean_ue (unaryExpr *e);
Boolean isNumerical_ue (unaryExpr *e);
Boolean isString_ue (unaryExpr *e);
Boolean isNodeSet_ue (unaryExpr *e);
Boolean requireContextSize_ue(unaryExpr *e);
void	reset_ue(unaryExpr *e, VTDNav *vn);
void	setContextSize_ue(unaryExpr *e,int s);
void	setPosition_ue(unaryExpr *e,int pos);
void    toString_ue(unaryExpr *e, UCSChar* string);

// function Expr
typedef enum FuncName {FN_LAST,
		   FN_POSITION,
		   FN_COUNT,
		   FN_LOCAL_NAME,
		   FN_NAMESPACE_URI,
		   FN_NAME,
		   FN_STRING,
		   FN_CONCAT,
		   FN_STARTS_WITH,
		   FN_CONTAINS,
		   FN_SUBSTRING_BEFORE,
		   FN_SUBSTRING_AFTER,
		   FN_SUBSTRING,
		   FN_STRING_LENGTH,
		   FN_NORMALIZE_SPACE,
		   FN_TRANSLATE,
		   FN_BOOLEAN,
		   FN_NOT,
		   FN_TRUE,
		   FN_FALSE,
		   FN_LANG,
		   FN_NUMBER,
		   FN_SUM,
		   FN_FLOOR,
		   FN_CEILING,
		   FN_ROUND
} funcName;
typedef struct AList {
	expr *e;
	struct AList *next;
} aList;

aList *createAlist();
void  freeAlist(aList *al);
void  reset_al(aList *al, VTDNav *vn);
void  toString_al(aList *al, UCSChar* string);

typedef struct FuncExpr {
	free_Expr freeExpr;
	eval_NodeSet evalNodeSet;
	eval_Number evalNumber;
	eval_String evalString;
	eval_Boolean evalBoolean;
	is_Boolean isBoolean;
	is_Numerical isNumerical;
	is_String  isString;
	is_NodeSet isNodeSet;
	require_ContextSize requireContextSize;
	reset_ reset;
	set_ContextSize setContextSize;
	set_Position setPosition;
	to_String toString;
	funcName opCode;
	aList *al;
	Boolean isNum;
	Boolean isBool;
	Boolean isStr;
	int contextSize;
	//double d;
	int position;
	int a;
} funcExpr;

funcExpr *createFuncExpr(funcName oc, aList *a);
void freeFuncExpr(funcExpr *e);
int	evalNodeSet_fne (funcExpr *e,VTDNav *vn);
double	evalNumber_fne (funcExpr *e,VTDNav *vn);
UCSChar* evalString_fne  (funcExpr *e,VTDNav *vn);
Boolean evalBoolean_fne (funcExpr *e,VTDNav *vn);
Boolean isBoolean_fne (funcExpr *e);
Boolean isNumerical_fne (funcExpr *e);
Boolean isString_fne (funcExpr *e);
Boolean isNodeSet_fne (funcExpr *e);
Boolean requireContextSize_fne(funcExpr *e);
void	reset_fne(funcExpr *e, VTDNav *vn);
void	setContextSize_fne(funcExpr *e,int s);
void	setPosition_fne(funcExpr *e,int pos);
void    toString_fne(funcExpr *e, UCSChar* string);

// location Expr
typedef enum AxisType {  AXIS_CHILD,
						 AXIS_DESCENDANT,
						 AXIS_PARENT,
						 AXIS_ANCESTOR,
						 AXIS_FOLLOWING_SIBLING,
						 AXIS_PRECEDING_SIBLING,
						 AXIS_FOLLOWING,
						 AXIS_PRECEDING,
						 AXIS_ATTRIBUTE,
						 AXIS_NAMESPACE,
						 AXIS_SELF,
						 AXIS_DESCENDANT_OR_SELF,
						 AXIS_ANCESTOR_OR_SELF
					} axisType;

typedef enum {	NT_NAMETEST ,
		NT_NODE,
		NT_TEXT,
		NT_PI0,
		NT_PI1,
		NT_COMMENT
} nodeTestType;

typedef struct nodeTest{
	UCSChar* nodeName;
	UCSChar* prefix;
	UCSChar* localName;
	UCSChar* URL;
	Boolean nsEnabled;
	nodeTestType testType;
} NodeTest;

NodeTest *createNodeTest();
void freeNodeTest(NodeTest *nt);
Boolean eval_nt(NodeTest *nt, VTDNav *vn);
void setNodeName(NodeTest *nt, UCSChar *name);
void setNodeNameNS(NodeTest *nt, UCSChar *p, UCSChar *ln);
void setTestType(NodeTest *nt, nodeTestType ntt); 
void toString_nt(NodeTest *nt, UCSChar *string);


typedef struct predicate{
	double d; // only supports a[1] style of location path for now
	int count;
	struct predicate *nextP;
	expr *e;
} Predicate;

Predicate *createPredicate();
void freePredicate(Predicate *p);
Boolean eval_p(Predicate *p, VTDNav *vn);
void setIndex_p(Predicate *p, int i);
void setContextSize_p(Predicate *p, int size);
Boolean requireContextSize_p(Predicate *p);
void reset_p(Predicate *p, VTDNav *vn);
void toString_p(Predicate *p, UCSChar *string);

typedef struct step{
	axisType axis_type;
	NodeTest *nt;  
	Predicate *p,*pt;// linked list
	struct step *nextS; // points to next step
	int position; // position
	struct step *prevS; // points to the prev step
	struct autoPilot *o; //AutoPilot goes here
	Boolean ft; // first time
}Step;

Step *createStep();
void freeStep(Step *s);
void reset_s(Step *s, VTDNav *vn);
void resetP_s(Step *s,VTDNav *vn);
void resetP2_s(Step *s,VTDNav *vn, Predicate *p1);
NodeTest *getNodeTest(Step *s);
Step *getNextStep(Step *s);
Boolean get_ft(Step *s);
void set_ft(Step *s, Boolean b);
Step *getPrevStep(Step *s);
void setNodeTest(Step *s,NodeTest *n);
void setPredicate(Step *s,Predicate *p1);
Boolean eval_s(Step *s,VTDNav *vn);
Boolean eval_s2(Step *s,VTDNav *vn, Predicate *p);
Boolean evalPredicates(Step *s,VTDNav *vn);
Boolean evalPredicates2(Step *s,VTDNav *vn, Predicate *p);
void setAxisType(Step *s,axisType st);
void toString_s(Step *s, UCSChar *string);

typedef enum{
	ABSOLUTE_PATH,
	RELATIVE_PATH
} pt;

typedef enum{
	XPATH_EVAL_START, 
	XPATH_EVAL_END,
	XPATH_EVAL_TERMINAL,
	XPATH_EVAL_FORWARD, 
	XPATH_EVAL_BACKWARD			
} LPstate;

typedef struct LocationPathExpr {
	free_Expr freeExpr;
	eval_NodeSet evalNodeSet;
	eval_Number evalNumber;
	eval_String evalString;
	eval_Boolean evalBoolean;
	is_Boolean isBoolean;
	is_Numerical isNumerical;
	is_String  isString;
	is_NodeSet isNodeSet;
	require_ContextSize requireContextSize;
	reset_ reset;
	set_ContextSize setContextSize;
	set_Position setPosition;
	to_String toString;
	Step* s;
	Step* currentStep;
	pt pathType;
	LPstate state;
	/*FastIntBuffer* fib; // for uniqueness checking */
	IntHash *ih; // for uniqueness checking
} locationPathExpr;

locationPathExpr *createLocationPathExpr();
void freeLocationPathExpr(locationPathExpr *e);
int		evalNodeSet_lpe (locationPathExpr *e,VTDNav *vn);
double	evalNumber_lpe (locationPathExpr *e,VTDNav *vn);
UCSChar* evalString_lpe  (locationPathExpr *e,VTDNav *vn);
Boolean evalBoolean_lpe (locationPathExpr *e,VTDNav *vn);
Boolean isBoolean_lpe (locationPathExpr *e);
Boolean isNumerical_lpe (locationPathExpr *e);
Boolean isString_lpe (locationPathExpr *e);
Boolean isNodeSet_lpe (locationPathExpr *e);
Boolean requireContextSize_lpe(locationPathExpr *e);
void	reset_lpe(locationPathExpr *e, VTDNav *vn);
void	setContextSize_lpe(locationPathExpr *e,int s);
void	setPosition_lpe(locationPathExpr *e,int pos);
void    toString_lpe(locationPathExpr *e, UCSChar* string);

Boolean isUnique(locationPathExpr *e,int i);
void setStep(locationPathExpr *e, Step* st);


// filter expr

typedef struct FilterExpr{
	free_Expr freeExpr;
	eval_NodeSet evalNodeSet;
	eval_Number evalNumber;
	eval_String evalString;
	eval_Boolean evalBoolean;
	is_Boolean isBoolean;
	is_Numerical isNumerical;
	is_String  isString;
	is_NodeSet isNodeSet;
	require_ContextSize requireContextSize;
	reset_ reset;
	set_ContextSize setContextSize;
	set_Position setPosition;
	to_String toString;
	struct Expr *e;
	Predicate *p;
	Boolean first_time;
} filterExpr;

filterExpr *createFilterExpr(expr *l, Predicate *pr);
void freeFilterExpr(filterExpr *e);
int		evalNodeSet_fe (filterExpr *e,VTDNav *vn);
double	evalNumber_fe (filterExpr *e,VTDNav *vn);
UCSChar* evalString_fe  (filterExpr *e,VTDNav *vn);
Boolean evalBoolean_fe (filterExpr *e,VTDNav *vn);
Boolean isBoolean_fe (filterExpr *e);
Boolean isNumerical_fe (filterExpr *e);
Boolean isString_fe (filterExpr *e);
Boolean isNodeSet_fe (filterExpr *e);
Boolean requireContextSize_fe(filterExpr *e);
void	reset_fe(filterExpr *e, VTDNav *vn);
void	setContextSize_fe(filterExpr *e,int s);
void	setPosition_fe(filterExpr *e,int pos);
void    toString_fe(filterExpr *e, UCSChar* string);
void	reset2_fe(filterExpr *e, VTDNav *vn);

// path expr

typedef struct PathExpr{
	free_Expr freeExpr;
	eval_NodeSet evalNodeSet;
	eval_Number evalNumber;
	eval_String evalString;
	eval_Boolean evalBoolean;
	is_Boolean isBoolean;
	is_Numerical isNumerical;
	is_String  isString;
	is_NodeSet isNodeSet;
	require_ContextSize requireContextSize;
	reset_ reset;
	set_ContextSize setContextSize;
	set_Position setPosition;
	to_String toString;
	expr *fe;
	locationPathExpr *lpe;
	int evalState;
	/*FastIntBuffer *fib;*/
	IntHash *ih;
} pathExpr;

pathExpr *createPathExpr(expr *f, locationPathExpr *l);
void freePathExpr(pathExpr *e);
int		evalNodeSet_pe (pathExpr *e,VTDNav *vn);
double	evalNumber_pe (pathExpr *e,VTDNav *vn);
UCSChar* evalString_pe  (pathExpr *e,VTDNav *vn);
Boolean evalBoolean_pe (pathExpr *e,VTDNav *vn);
Boolean isBoolean_pe (pathExpr *e);
Boolean isNumerical_pe (pathExpr *e);
Boolean isString_pe (pathExpr *e);
Boolean isNodeSet_pe (pathExpr *e);
Boolean requireContextSize_pe(pathExpr *e);
void	reset_pe(pathExpr *e, VTDNav *vn);
void	setContextSize_pe(pathExpr *e,int s);
void	setPosition_pe(pathExpr *e,int pos);
void    toString_pe(pathExpr *e, UCSChar* string);

int yylex();
//void yyrestart(FILE *i);
int yyerror(char *s);
//YY_BUFFER_STATE yy_scan_string(const YY_CHAR *str);

// three variable to direct yyparse to in-memory string

// This structure is for data item look up purposes
typedef struct nsList {
	UCSChar *URL;
	UCSChar *prefix;
	struct nsList *next;
} NsList;

// given a prefix, find the URL
UCSChar *lookup(NsList *nl, UCSChar *prefix);
expr *xpathParse(UCSChar *input, NsList *nl);





#endif
