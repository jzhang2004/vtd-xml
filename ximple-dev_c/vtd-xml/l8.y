%{
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
#include "customTypes.h"
#include <string.h>
#include <stdio.h>
#include "xpath1.h"
#include "helper.h"

Step *tmpStep;
Step *tmpStep2;
NodeTest *tmpNt;
locationPathExpr *tmpLPExpr ;
aList *tmpAl;
Predicate *tmpP;

static expr *x;
static NsList *xpathNSList = NULL;
%}

%union {
	UCSChar*   literal;
	axisType at;
	int	 integer;
	double	 number;
	struct nametype {
		UCSChar* prefix;
		UCSChar* qname;
		UCSChar* localname;
	} name;
	struct {
       		nodeTestType nt;
       		UCSChar*   arg;
	} ntest;
	funcName fname;
	expr *expression;
 	locationPathExpr *lpe;
	pathExpr *pe;
	Step *s;
	aList *a;
	NodeTest *nodetest;
	Predicate* p;
}

%token <at> AXISNAME 
%token <literal> LITERAL
%token <number>  NUMBER
%token <name> NAME
%token <fname> FNAME
%token <ntest> NTEST
%type <expression> Expr OrExpr AndExpr EqualityExpr RelationalExpr AdditiveExpr
%type <expression> MultiplicativeExpr UnaryExpr UnionExpr FilterExpr
%type <expression> FunctionCall Argument PrimaryExpr
%type <lpe> LocationPath
%type <pe> PathExpr
%type <s> Step AbbreviatedStep RelativeLocationPath  AbsoluteLocationPath
%type <s> AbbreviatedAbsoluteLocationPath  AbbreviatedRelativeLocationPath
%type <p> Predicate PredicateList
%type <a> ArgumentList
%type <at> AxisSpecifier AbbreviatedAxisSpecifier
%type <fname> FunctionName
%type <nodetest> NodeTest
%token OR AND EQ NE GT LT GE LE ADD SUB MULT DIV MOD
%token LP RP DOLLAR  UNION SLASH DSLASH COMMA ERROR
%token AT DOT DDOT LB RB DOLLAR
%left  OR
%left  AND
%left  EQ NE
%left  GE LE GT LT
%nonassoc UMINUS

%%
Expr 		:    OrExpr  { x=$$ = $1;}
       		;

OrExpr 		:    AndExpr  {$$ = $1;}
		|    OrExpr OR AndExpr {
								Try {
										$$ = createBinaryExpr($1, OP_OR,$3);
										addObj($$);
									}
								Catch(e){
									//freeAllObj();
									 return 1;
								} 
		
								}
		;

AndExpr		:    EqualityExpr { $$ = $1;}
	 	|    AndExpr AND EqualityExpr {
	 							Try {
	 									$$ = createBinaryExpr($1,OP_AND,$3);
	 									addObj($$);
	 								}
	 							Catch(e){
	 								//freeAllObj();
									return 1;
								}
								}
		;

EqualityExpr    :    RelationalExpr  { $$ = $1;}
		|    EqualityExpr EQ RelationalExpr  { 
								Try {
	 									$$ = createBinaryExpr($1,OP_EQ,$3);
	 									addObj($$);
	 								}
	 							Catch(e){
	 								//freeAllObj();
									return 1;
								}
								}
		|    EqualityExpr NE RelationalExpr {
			 					Try {
	 									$$ = createBinaryExpr($1,OP_NE,$3);
	 									addObj($$);
	 								}
	 							Catch(e){
	 								//freeAllObj();
									return 1;
								}
								}
		;

RelationalExpr  :    AdditiveExpr  { $$ = $1; }
  		|    RelationalExpr LT AdditiveExpr  {
			 					Try {
	 									$$ = createBinaryExpr($1,OP_LT,$3);
	 									addObj($$);
	 								}
	 							Catch(e){
	 								//freeAllObj();
									return 1;
								}
								}
		|    RelationalExpr GT AdditiveExpr  {
			 					Try {
	 									$$ = createBinaryExpr($1,OP_GT,$3);
	 									addObj($$);	 									
	 								}
	 							Catch(e){
	 								//freeAllObj();
									return 1;
								}
								}
		|    RelationalExpr LE AdditiveExpr  {
			 					Try {
	 									$$ = createBinaryExpr($1,OP_LE,$3);
	 									addObj($$);
	 								}
	 							Catch(e){
	 								//freeAllObj();
									return 1;
								}
								}
		|    RelationalExpr GE AdditiveExpr  {
			 					Try {
	 									$$ = createBinaryExpr($1,OP_GE,$3);
	 									addObj($$);
	 								}
	 							Catch(e){
	 								//freeAllObj();
									return 1;
								}
								}
		;

AdditiveExpr    :    MultiplicativeExpr  {$$  = $1; }
		|    AdditiveExpr ADD MultiplicativeExpr  {
			 					Try {
	 									$$ = createBinaryExpr($1,OP_ADD,$3);
	 									addObj($$);
	 								}
	 							Catch(e){
	 								//freeAllObj();
									return 1;
								}
								}
		|    AdditiveExpr SUB MultiplicativeExpr {
			 					Try {
	 									$$ = createBinaryExpr($1,OP_SUB,$3);
	 									addObj($$);
	 								}
	 							Catch(e){
	 								//freeAllObj();
									return 1;
								}
								}
		;

MultiplicativeExpr    :  UnaryExpr  {$$ = $1; }
   		|    MultiplicativeExpr MULT UnaryExpr  {
			 					Try {
	 									$$ = createBinaryExpr($1,OP_MULT,$3);
	 									addObj($$);
	 								}
	 							Catch(e){
	 								//freeAllObj();
									return 1;
								}
								}
		|    MultiplicativeExpr DIV UnaryExpr  {
			 					Try {
	 									$$ = createBinaryExpr($1,OP_DIV,$3);
	 									addObj($$);
	 								}
	 							Catch(e){
	 								//freeAllObj();
									return 1;
								}
								}
		|    MultiplicativeExpr MOD UnaryExpr  {
			 					Try {
	 									$$ = createBinaryExpr($1,OP_MOD,$3);
	 									addObj($$);
	 								}
	 							Catch(e){
	 								//freeAllObj();
									return 1;
								}
								}
		;

UnaryExpr    	:    UnionExpr  { $$ = $1;}
  		|    SUB UnaryExpr  %prec UMINUS  {
			 					Try {
	 									$$ = createUnaryExpr(OP_NE,$2);
	 									addObj($$);
	 								}
	 							Catch(e){
	 								//freeAllObj();
									return 1;
								}
								}
		;


UnionExpr    	:    PathExpr  	{ $$ = $1; }		
   		|    UnionExpr UNION PathExpr {/*freeAllObj();*/return 1;}
		;


PathExpr     	:    LocationPath  { $$ = $1;  }
   		|    FilterExpr  { $$ = $1;}
  		|    FilterExpr SLASH RelativeLocationPath  {  														
  														Try{
  															tmpLPExpr = createLocationPathExpr();
  															addObj(tmpLPExpr);
  															addObj(tmpLPExpr->fib);
  															setStep(tmpLPExpr, $3);
  															$$ = createPathExpr($1, tmpLPExpr);
  															addObj($$);
  															addObj($$->fib);
  															tmpLPExpr = NULL;
  														}Catch(e){  														
  															//freeAllObj();
  															return 1;
  														}  		
  													}
  													
		|    FilterExpr DSLASH RelativeLocationPath {
														Try {
															tmpStep = createStep();
															addObj(tmpStep);
															setAxisType(tmpStep,AXIS_DESCENDANT_OR_SELF);
															tmpNt = createNodeTest();
															addObj(tmpNt);
															setTestType(tmpNt, NT_NODE);
															setNodeTest(tmpStep, tmpNt);
															tmpStep->nextS = $3;
															$3->prevS = tmpStep;
															tmpLPExpr = createLocationPathExpr();
															addObj(tmpLPExpr);
															addObj(tmpLPExpr->fib);
															setStep(tmpLPExpr, tmpStep);
															$$ = createPathExpr($1, tmpLPExpr);
															addObj($$);
															addObj($$->fib);															
														} Catch (e){
															//freeAllObj();		
															return 1;																											
														}								
													}
		;


FilterExpr   	:    PrimaryExpr  { $$ = $1;}
  		|    FilterExpr Predicate  {Try {
  										$$ = createFilterExpr($1,$2);
  										addObj($$);
  										}
  									Catch(e){
  										//freeAllObj();
  										return 1;
  									}
  								}  									
  								;



PrimaryExpr     :    VariableReference {/*freeAllObj();*/ return 1;}
		|    LP Expr RP  { $$ = $2;} 
		|    LITERAL  { Try {
							$$ = createLiteralExpr($1);
							addObj($$);
							addObj($1);
						} Catch (e) {
							//freeAllObj();
							return 1;	
						}						
					  } 
		|    NUMBER  { Try {
						    $$ = createNumberExpr($1);
						    addObj($$);
					   } Catch (e) {
							//freeAllObj();
							return 1;
					   }
					 } 
		|    FunctionCall { $$ = $1;} 
		|    ERROR {
						//freeAllObj();
						printf(" Invalid char encoutered \n");
						return 1;
					}
		;


FunctionCall    :    FunctionName LP ArgumentList RP { Try {
															$$ = createFuncExpr($1, $3);
															addObj($$);
													   }
													   Catch(e){
															//freeAllObj();
															return 1;
													   }

													 } 
		;

ArgumentList	: 	{ $$ = NULL;}   
	     	|    Argument  {	Try {
	     							$$ = createAlist();
	     							addObj($$);
	     							$$->e = $1;  								     	
	     						}Catch (e){
	     							//freeAllObj();
	     							return 1;
	     						}	
	     					} 
	     	|    Argument COMMA ArgumentList  {
	     											Try {	     											
	     												$$ = createAlist();
	     												addObj($$);
	     												$$->e = $1;
	     												$$->next = $3;	     												
	     											} Catch (e){
	     												//freeAllObj();
	     												return 1;	     													     												
	     											}	     	
	     									  } 
		;

Argument   	:    Expr  {$$ = $1;} 
	   	; 


LocationPath    :    RelativeLocationPath	{ Try {
													$$ = createLocationPathExpr();
													addObj($$);
													addObj($$->fib);
													setStep($$, $1);
												  }
											  Catch (e) {
													//freeAllObj();
													return 1;
											  }
											}
		|    AbsoluteLocationPath	{		Try {  printf("absolute locationpath \n");
													$$ = createLocationPathExpr();
													addObj($$);
													addObj($$->fib);
													$$->pathType = ABSOLUTE_PATH;
													setStep($$, $1);
												  }
											  Catch (e) {
													//freeLocationPathExpr($$);
													//freeAllObj();
													return 1;
											  }
									}
		;

AbsoluteLocationPath :  SLASH 				{$$ = NULL;}
		|    SLASH RelativeLocationPath		{printf("slash detected \n"); $$ = $2;}
		|    AbbreviatedAbsoluteLocationPath	{$$ = $1;}
		;

RelativeLocationPath :  Step {$$ = $1;}
		|    Step SLASH RelativeLocationPath  { $1->nextS = $3;
												$3->prevS = $1;
												$$ = $1;		
												}
		|    AbbreviatedRelativeLocationPath {$$ = $1;}
		;


Step		:    AxisSpecifier NodeTest PredicateList {
														Try {
															$$ = createStep();
															addObj($$);
															setAxisType($$, $1);
															setNodeTest($$, $2);
															setPredicate($$, $3);
															}
														Catch(e){
															//freeAllObj();
															return 1;
														}
														}
      		|    AbbreviatedStep			{$$ = $1;}
		;


NodeTest	:    NAME 				{ Try {
										$$ = createNodeTest();
										addObj($$);
										setTestType($$,NT_NAMETEST);
										wprintf(L"$1.qname %ls\n",$1.qname);
										setNodeName($$,$1.qname);
										if ($1.localname!=NULL){
											setNodeNameNS($$,$1.prefix,$1.localname);
											// the part for URL mapping goes here
											$$->URL = lookup(xpathNSList,$1.prefix);
											if ($$->URL == NULL)
											return 1;
										}
										$1.prefix = $1.localname = $1.qname = NULL;
										
									  } Catch(e){
										//freeAllObj();
										return 1;
									  }
									} 
	 	|    NTEST				{ Try{
	 								$$ = createNodeTest();
	 								addObj($$);
	 								setTestType($$,$1.nt);
	 								}
	 								Catch(e){
	 									//freeAllObj();
	 									return 1;
	 								}	 								
	 							}
		;

PredicateList	: { $$ = NULL;} 
	     	|    Predicate	PredicateList { $1->nextP = $2;
	     									$$ = $1;	
	     								  }  
		;

AxisSpecifier	:    AXISNAME				{$$  = $1;}
	      	|    AbbreviatedAxisSpecifier		{$$  = $1;}
	      	;

AbbreviatedAxisSpecifier  : { printf("abbreviated child axis \n");$$  = AXIS_CHILD;}
			  | AT	{$$ = AXIS_ATTRIBUTE;}
			;

AbbreviatedStep	:    DOT {Try{
								$$ = createStep();
								addObj($$);
								tmpNt = createNodeTest();
								addObj(tmpNt);
								setTestType(tmpNt, NT_NODE);
								setAxisType($$,AXIS_SELF);
								setNodeTest($$,tmpNt);
							}
							Catch(e){
								//freeAllObj();
								return 1;
							}
						}
		|    DDOT {
					Try{
						$$ = createStep();
						addObj($$);
						tmpNt = createNodeTest();
						addObj(tmpNt);
						setTestType(tmpNt, NT_NODE);
						setAxisType($$,AXIS_PARENT);
						setNodeTest($$,tmpNt);
					}
					Catch(e){
						//freeAllObj();
						return 1;
					}
		
					}
		;

AbbreviatedAbsoluteLocationPath : DSLASH RelativeLocationPath {
																Try{
																	$$ = createStep();
																	addObj($$);
																	setAxisType($$,AXIS_DESCENDANT_OR_SELF);
																	tmpNt = createNodeTest();
																	addObj(tmpNt);
																	setTestType(tmpNt, NT_NODE);
																	setNodeTest($$,tmpNt);
																	$$->nextS = $2;
																	$2->prevS = $$;																
																}Catch(e){
																	//freeAllObj();
																	return 1;																	
																}
																}
		;

AbbreviatedRelativeLocationPath : Step DSLASH RelativeLocationPath  {
																	Try{
																		$$= createStep();
																		addObj($$);
																		setAxisType($$,AXIS_DESCENDANT_OR_SELF);
																		tmpNt = createNodeTest();
																		addObj(tmpNt);
																		setTestType(tmpNt,NT_NODE);
																		setNodeTest($$, tmpNt);
																		$1->nextS = $$;
																		$$->prevS = $1;
																		$$->nextS = $3;
																		$3->prevS = $$;
																		$$ = $1;
																	}Catch(e){
																		//freeAllObj();
																		return 1;	
																	}
																	}
		;


Predicate 	:    LB Expr RB {
							   Try {
									$$ = createPredicate();
									addObj($$);
									$$->e = $2;
								} Catch(e){
									//freeAllObj();
									return 1;
								}
							}
	   	;

VariableReference : DOLLAR NAME 
		  ;

FunctionName 	:  FNAME  {$$  = $1;}
	      ;
	      
%%

expr *xpathParse(UCSChar *input, NsList *nl){
	int l = wcslen(input);
	int i = 0;	
	xpathNSList = nl;
	xpathInputPtr = xpathInput = (unsigned short *)malloc((l+1)<<1);
	// copy the string from wchar_t to unsigned short
	for (i=0;i<l;i++){
		xpathInput[i] = (unsigned short)input[i];
	}
	xpathInputLimit = (unsigned short *) (xpathInput + ((l)<<1));
	if (yyparse() == 0){
		//isName = 1;
		wprintf(L"yyparse return 0\n");
		free(xpathInput);
		xpathInput = xpathInputPtr = xpathInputLimit = NULL;
		resetObj();
		return x;
	} else {
		wprintf(L"yyparse return 1\n");
		free(xpathInput);
		xpathInput = xpathInputPtr = xpathInputLimit = NULL;
		//isName = 1;
		freeAllObj();
		return NULL;
	}
	
}

