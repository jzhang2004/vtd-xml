%{
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
#include "stdlib.h"
#include <wchar.h>
#include "xpath.h"
#include "XMLChar.h"
#include "helper.h"
#include "l8.tab.h"


	

#ifndef min
#define min(a,b) { (a>b)?b:a  }
#endif
#undef  YY_INPUT
#define YY_INPUT(b,r,ms)  r = my_yyinput(b,ms)
#define YY_USER_ACTION num_chars += yyleng;
static wchar_t *tmpstring;

int my_yyinput(unsigned short *buf, int s);
//#define YY_USER_INIT  buf_state = yy_scan_string( name );

//static YY_BUFFER_STATE buf_state;
wchar_t *mydup(YY_CHAR *text, int len);
static int i;
int isName;
int colonPosition;
int num_chars;
%}
%option noyywrap

ws 	   [ \t\r\n]
digits	[0-9]+
nc	   ([^!-/:-@[-^ \n\r\t|]|"#"|"&"|";"|"?"|_|"\\"|"^"|"%"|"-"|".")
nc2   ([^!-/;-@[-^ \n\r\t|0-9]|"#"|"&"|";"|"?"|_|"\\"|"^"|"%"|".")

%%

{ws}	; //do nothing

"+" 	{isName = 1 ; return ADD;}
"-" 	{isName = 1 ; return SUB;}
"."	{isName = 0 ; return DOT;}
".." 	{isName = 0 ; return DDOT;}
"@"	{isName = 1 ; return AT;}

","	{isName = 1 ; return COMMA;}
"("	{isName = 1 ; return LP;}
")"	{isName = 0 ; return RP;}
"["	{isName = 1 ; return LB;}
"]"	{isName = 0 ; return RB;}
">"	{isName = 1 ; return GT;}
"<"	{isName = 1 ; return LT;}
">="	{isName = 1 ; return GE;}
"<="	{isName = 1 ; return LE;}
"="	{isName = 1 ; return EQ;}
"!="	{isName = 1 ; return NE;}
"$"      {isName = 1; return DOLLAR;}
"*"	{if (isName ==0 ){
		isName = 1;
		return MULT;
	}
	 else {
		isName = 0;
		yylval.name.qname = NULL;
		yylval.name.localname = NULL;
		yylval.name.prefix = NULL;
		yylval.name.qname = _wcsdup(L"*");
		//addObj(yylval.name.qname);
		//yylval.name.localname = NULL;
		//yylval.name.prefix = NULL;
		addObj(yylval.name.qname);
		return NAME;
	 }	 
	}

"/"	{isName = 1 ; return SLASH;}
"//"	{isName = 1 ; return DSLASH;}

"div"	{if (isName == 0 ) {
		isName = 1;
		return DIV;
	}
	 else {
		isName = 0;
		yylval.name.qname = NULL;
		yylval.name.localname = NULL;
		yylval.name.prefix = NULL;
		yylval.name.qname = _wcsdup(L"div");
		addObj(yylval.name.qname);
		return NAME;
	 } 
	}
"mod"	{if (isName == 0 ) {
		isName = 1;
		return MOD;
	}
	 else {
		yylval.name.qname = NULL;
		yylval.name.localname = NULL;
		yylval.name.prefix = NULL;
		isName = 0;
		yylval.name.qname = _wcsdup(L"mod");
		addObj(yylval.name.qname);
		return NAME;
	 } 
	}

"and"	{if (isName == 0 ) {
		isName = 1;
		return AND;
	}
	 else {
		yylval.name.qname = NULL;
		yylval.name.localname = NULL;
		yylval.name.prefix = NULL;
		isName = 0;
		yylval.name.qname = _wcsdup(L"and");
		addObj(yylval.name.qname);
		return NAME;
	 } 
	}

"or" {if (isName == 0 ){
		isName = 1; 
		return OR;
	}
	 else {
		yylval.name.qname = NULL;
		yylval.name.localname = NULL;
		yylval.name.prefix = NULL;
		isName = 0;
		yylval.name.qname = _wcsdup(L"or");
		addObj(yylval.name.qname);
		return NAME;
	 }
	}

"|"	{isName = 1 ; return UNION ; }

last{ws}*"("		{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_LAST;
				return FNAME;				 
			}

position{ws}*"("	{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_POSITION;
				return FNAME;	
			}

count{ws}*"("		{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_COUNT;
				return FNAME;	
			}

local-name{ws}*"("	{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_LOCAL_NAME;
				return FNAME;	
			}

namespace-uri{ws}*"("	{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_NAMESPACE_URI;
				return FNAME;	
			}

name{ws}*"("		{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_NAME;
				return FNAME;	
			}

string{ws}*"("		{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_STRING;
				return FNAME;	
			}


concat{ws}*"("		{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_CONCAT;
				return FNAME;	
			}

starts-with{ws}*"("	{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_STARTS_WITH;
				return FNAME;	
			}

contains{ws}*"("	{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_CONTAINS;
				return FNAME;	
			}

substring-before{ws}*"("  {  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_SUBSTRING_BEFORE;
				return FNAME;	
			}

substring-after{ws}*"(" {  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_SUBSTRING_AFTER;
				return FNAME;	
			} 	

substring{ws}*"("	{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_SUBSTRING;
				return FNAME;	
			}

string-length{ws}*"("	{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_STRING_LENGTH;
				return FNAME;	
			}

normalize-space{ws}*"("	{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_NORMALIZE_SPACE;
				return FNAME;	
			}

translate{ws}*"("	{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_TRANSLATE;
				return FNAME;	
			}
			
abs{ws}*"("		{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_ABS;
				return FNAME;	
			}

round-half-to-even{ws}*"("	{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_ROUND_HALF_TO_EVEN;
				return FNAME;	
			}

round-half-to-odd{ws}*"("	{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_ROUND_HALF_TO_ODD;
				return FNAME;	
			}

code-points-to-string{ws}*"("	{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_CODE_POINTS_TO_STRING;
				return FNAME;	
			}

compare{ws}*"("		{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_COMPARE;
				return FNAME;	
			}

upper-case{ws}*"("	{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_UPPER_CASE;
				return FNAME;	
			}

lower-case{ws}*"("	{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_LOWER_CASE;
				return FNAME;	
			}

ends-with{ws}*"("	{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_ENDS_WITH;
				return FNAME;	
			}

QName{ws}*"("		{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_QNAME;
				return FNAME;	
			}

local-name-from-QName{ws}*"("		{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_LOCAL_NAME_FROM_QNAME;
				return FNAME;	
			}

namespace-uri-from-QName{ws}*"(" 	{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_NAMESPACE_URI_FROM_QNAME;
				return FNAME;	
			}

namespace-uri-for-prefix{ws}*"("	{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_NAMESPACE_URI_FOR_PREFIX;
				return FNAME;	
			}

resolve-QName{ws}*"(" 	{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_RESOLVE_QNAME;
				return FNAME;	
			}

iri-to-uri{ws}*"("	{	isName = 1; 
			 	unput('(');
				yylval.fname = FN_IRI_TO_URI;
				return FNAME;	
			}

escape-html-uri{ws}*"(" {	isName = 1; 
			 	unput('(');
				yylval.fname = FN_ESCAPE_HTML_URI;
				return FNAME;	
			}


encode-for-uri{ws}*"(" {	isName = 1; 
			 	unput('(');
				yylval.fname = FN_ENCODE_FOR_URI;
				return FNAME;	
			}

boolean{ws}*"("		{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_BOOLEAN;
				return FNAME;	
			}

not{ws}*"("		{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_NOT;
				return FNAME;	
			}

true{ws}*"("		{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_TRUE;
				return FNAME;	
			}

false{ws}*"("		{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_FALSE;
				return FNAME;	
			}

lang{ws}*"("		{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_LANG;
				return FNAME;	
			}

number{ws}*"("		{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_NUMBER;
				return FNAME;	
			}

sum{ws}*"("		{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_SUM;
				return FNAME;	
			}

floor{ws}*"("		{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_FLOOR;
				return FNAME;	
			}

ceiling{ws}*"("		{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_CEILING;
				return FNAME;	
			}

round{ws}*"("		{  	isName = 1; 
			 	unput('(');
				yylval.fname = FN_ROUND;
				return FNAME;	
			}


\"[^\"]*\" |
'[^']*'			{
				isName = 0;
				tmpstring = mydup(yytext,yyleng);
				yylval.literal = malloc(sizeof(wchar_t)*(yyleng-1));
				if (yylval.literal!=NULL){
					wcsncpy(yylval.literal, (wchar_t *)(tmpstring+1), yyleng-2);
					addObj(yylval.literal);
				} 	
				*(yylval.literal+yyleng-2) = L'\0';
				free(tmpstring);
				tmpstring = NULL;
				return LITERAL;
			}

([0-9]+)("."([0-9]+)?)? |
"."[0-9]+		{
				isName = 0;
				tmpstring = mydup(yytext,yyleng);
				yylval.number = wcstod(tmpstring,NULL);
				free(tmpstring);
				tmpstring = NULL;
				return NUMBER;
			}


text{ws}*"("{ws}*")"	{	
				isName = 0;
				yylval.ntest.nt = NT_TEXT;
				yylval.ntest.arg = NULL;
				return NTEST;
			}

comment{ws}*"("{ws}*")"	{
				isName = 0;
				yylval.ntest.nt = NT_COMMENT;
				yylval.ntest.arg = NULL;
				return NTEST;
			}

node{ws}*"("{ws}*")"	{
				isName = 0;
				yylval.ntest.nt = NT_NODE;
				yylval.ntest.arg = NULL;
				return NTEST;
			}

processing-instruction{ws}*"("{ws}*")"	{
						isName = 0;
						yylval.ntest.nt = NT_PI0;
						yylval.ntest.arg = NULL;
						return NTEST;
					}


ancestor{ws}*::		{	isName = 1;
				yylval.at = AXIS_ANCESTOR;
				return AXISNAME;
			}

ancestor-or-self{ws}*::	{	isName = 1;
				yylval.at = AXIS_ANCESTOR_OR_SELF;
				return AXISNAME;
			}


attribute{ws}*::	{	isName =1;
				yylval.at = AXIS_ATTRIBUTE;
				return AXISNAME;
			}

child{ws}*::		{	isName = 1;
				yylval.at = AXIS_CHILD;
				return AXISNAME;
			}

descendant{ws}*::	{	isName = 1;
				yylval.at = AXIS_DESCENDANT;
				return AXISNAME;
			}

descendant-or-self{ws}*:: {	isName = 1;
				yylval.at = AXIS_DESCENDANT_OR_SELF;
				return AXISNAME;
			}

following{ws}*::	{	isName = 1;
				yylval.at = AXIS_FOLLOWING;
				return AXISNAME;
			}

following-sibling{ws}*::  {	isName = 1;
				yylval.at = AXIS_FOLLOWING_SIBLING;
				return AXISNAME;
			}

namespace{ws}*::	{	isName = 1;
				yylval.at = AXIS_NAMESPACE;
				return AXISNAME;
			}

parent{ws}*::		{	isName = 1;
				yylval.at = AXIS_PARENT;
				return AXISNAME;
			}

preceding{ws}*::	{	isName = 1;
				yylval.at = AXIS_PRECEDING;
				return AXISNAME;
			}

preceding-sibling{ws}*:: {	isName = 1;
				yylval.at = AXIS_PRECEDING_SIBLING;
				return AXISNAME;
			}

self{ws}*::		{	isName = 1;
				yylval.at = AXIS_SELF;
				return AXISNAME;
			}

{nc2}{nc}*:"*"  		{
				isName = 0;
				//len = wcslen(yytext);
				yylval.name.prefix = NULL;
				yylval.name.localname = NULL;
				yylval.name.qname = NULL;
				if (!XMLChar_isNCNameStart(yytext[0]))
					return ERROR;
				for(i=1;i<yyleng-2;i++){
					if (!XMLChar_isNCName(yytext[i]))
						return ERROR;
				}
				yylval.name.prefix = (UCSChar *)malloc (sizeof(UCSChar)*(yyleng-1));
				if (yylval.name.prefix == NULL)
						return ERROR; 
				addObj(yylval.name.prefix);
				tmpstring = mydup(yytext,yyleng);
				if (tmpstring!=NULL){
					wcsncpy(yylval.name.prefix, tmpstring, yyleng-2);
					yylval.name.prefix[yyleng-2] = 0;
					addObj(yylval.name.prefix);
				}
				free(tmpstring);
				tmpstring = NULL;
				yylval.name.localname = _wcsdup(L"*");
				addObj(yylval.name.localname);
				return NAME;
			}

{nc2}{nc}*:{nc}+ |
{nc2}{nc}*			{
				yylval.name.prefix = NULL;
				yylval.name.localname = NULL;
				yylval.name.qname = NULL;
				if (!XMLChar_isNCNameStart(yytext[0]))
					return ERROR;
				for(i=1;i<yyleng;i++){
					if (!XMLChar_isNCName(yytext[i]) && yytext[i]!= ':')
						return ERROR;
					if (yytext[i]==':')
						colonPosition = i;						
				}
				if (colonPosition != -1){
					yylval.name.prefix = (wchar_t *)malloc(sizeof(wchar_t)*(colonPosition+1));
					if (yylval.name.prefix == NULL)
						return ERROR; 
					addObj(yylval.name.prefix);
					yylval.name.localname = (wchar_t *)malloc(sizeof(wchar_t)*(yyleng - colonPosition));
					if (yylval.name.localname == NULL){
						return ERROR;
					}					
					addObj(yylval.name.localname);
					// copy content of prefix and localname
					for (i=0;i<colonPosition;i++){
						yylval.name.prefix[i] = yytext[i];
					}
					yylval.name.prefix[i] = 0;
					for(i=0;i<yyleng - colonPosition -1;i++){
						yylval.name.localname[i] = yytext[i + colonPosition +1];
					}
					yylval.name.localname[i] = 0;
				}
				isName = 0;
				tmpstring = mydup(yytext,yyleng);
				if (tmpstring ==NULL){
					printf("tmpstring can't be null");
					return ERROR;
				}
				yylval.name.qname = tmpstring;
				addObj(tmpstring);
				tmpstring = NULL;
				//printf("returned a NAME \n");

				colonPosition = -1;

				return NAME;
			}


.			{	isName =1; return ERROR;
			}

%%
unsigned short *xpathInput;
unsigned short *xpathInputPtr;
unsigned short *xpathInputLimit;

int my_yyinput(YY_CHAR *buf, int max_size){
	// n is of unit (unsigned short)
	int n = min(max_size, (xpathInputLimit - xpathInputPtr)>>1);
        //int n =2;	
	if (n>0){
		memcpy(buf, (unsigned short *)xpathInputPtr,n<<1); // YY_CHAR is of size 2
		xpathInputPtr += n<<1;
	}
 	//wprintf(L"maxsize is %d \n",max_size);
  
	return n;
}

int yyerror(char *s){
	fprintf(stderr,"%s\n", s);
	return 1;
}

wchar_t *mydup(YY_CHAR *text,int len){
	int i=0;
	wchar_t *string = malloc(sizeof(wchar_t)*(len+1));
	if (string!=NULL){
		while(text[i]!=0){
			string[i] = (wchar_t) text[i];
			i++;
		}
		string[i]=0;
	}
	return string;
}

