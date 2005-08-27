import java_cup.runtime.Symbol;

%%
%cup
%extends sym
%yylexthrow XPathParseException

%{

  private Symbol sym(int sym) {
    return new Symbol(sym);
  }

  private Symbol sym(int sym, Object val) {
    return new Symbol(sym, val);
  }

  int op_assert; 
  NameType name;
  FuncName fname; 
  Ntest ntest;
  AxisType at;
  int len;
  String literal;
  Double number; 
%}


ws  = 	   [ \t\r\n]
digits	=  [0-9]+
nc	=  ([^\!-/:-@\[-\^ \n\r\t\|]|"#"|"&"|";"|"?"|_|"\\"|"^"|"%"|"-")
nc2	=  ([^\!-/:-@\[-\^ \n\r\t\|]|"#"|"&"|";"|"?"|_|"\\"|"^"|"%")

%%
{ws}+ { /* eat white space */}

"+" 	{op_assert = 1 ; return sym(ADD);}
- 	{op_assert = 1 ; return sym(SUB);}
"."	{op_assert = 0 ; return sym(DOT);}
".." 	{op_assert = 0 ; return sym(DDOT);}
"@"	{op_assert = 1 ; return sym(AT);}

","	{op_assert = 1 ; return sym(COMMA);}
"("	{op_assert = 1 ; return sym(LP);}
")"	{op_assert = 0 ; return sym(RP);}
"["	{op_assert = 1 ; return sym(LB);}
"]"	{op_assert = 0 ; return sym(RB);}
">"	{op_assert = 1 ; return sym(GT);}
"<"	{op_assert = 1 ; return sym(LT);}
">="	{op_assert = 1 ; return sym(GE);}
"<="	{op_assert = 1 ; return sym(LE);}
"="	{op_assert = 1 ; return sym(EQ);}
"!="	{op_assert = 1 ; return sym(NE);}

"*"	{if (op_assert ==0 )
		return  sym(MULT);
	 else {
		op_assert = 1;		
		name = new NameType();
		name.qname = "*";
		return sym(NAME,name);
	 }	 
	}

"/"	{op_assert = 1 ; 
	 //System.out.println("SLASH returned ");
	 return sym(SLASH);
	}

"//"	{op_assert = 1 ; 
	 //System.out.println("DSLASH returned "); 
	 return sym(DSLASH);
	}

div	{     if (op_assert == 0 ) {
		 
		 return sym(DIV);
	      } else {
		 op_assert = 1 ;
		 name = new NameType();
		 name.qname = "div";
		 return sym(NAME,name);
	      }
	}

mod	{     if (op_assert == 0) {
		  
		 return sym(MOD);
	      } else {
		 op_assert = 1 ;
		 name = new NameType();
		 name.qname = "mod";
		 return sym(NAME,name);
	      }
	}

and	{     if (op_assert == 0) {
		
		 return sym(AND);
	      } else {
		 op_assert = 1 ;
		 name = new NameType();
		 name.qname = "add";
		 return sym(NAME,name);
	      }
	}

or	{     if (op_assert == 0) {		
		 return sym(OR);
	      } else {
		 op_assert = 1 ;
		 name = new NameType();
		 name.qname = "or";
		 return sym(NAME,name);
	      }
	}

"|"	{op_assert = 1 ; return sym(UNION) ; }

last{ws}*"("		{  	op_assert = 0; 
			 	yypushback(1);
				fname = new FuncName();
				fname.i = FuncName.LAST;
				return sym(FNAME,fname);				 
			}
position{ws}*"("	{  	op_assert = 0; 
			 	yypushback(1);
				//yyparser.yylval.fname.i =  FuncName.POSITION;
				fname = new FuncName();
				fname.i = FuncName.POSITION;
				return sym(FNAME,fname);	
			}
count{ws}*"("		{  	op_assert = 0; 
			 	yypushback(1);
				//yyparser.yylval.fname.i =  FuncName.COUNT;
				fname = new FuncName();
				fname.i = FuncName.COUNT;
				return sym(FNAME,fname);
			}
local-name{ws}*"("	{  	op_assert = 0; 
			 	yypushback(1);
				//yyparser.yylval.fname.i =  FuncName.LOCAL_NAME;
				fname = new FuncName();
				fname.i = FuncName.LOCAL_NAME;
				return sym(FNAME,fname);
			}
namespace-uri{ws}*"("	{  	op_assert = 0; 
			 	yypushback(1);
				//yyparser.yylval.fname.i =  FuncName.NAMESPACE_URI;
				fname = new FuncName();
				fname.i = FuncName.NAMESPACE_URI;
				return sym(FNAME,fname);
			}
name{ws}*"("		{  	op_assert = 0; 
			 	yypushback(1);
				//yyparser.yylval.fname.i =  FuncName.NAME;
				fname = new FuncName();
				fname.i = FuncName.NAME;
				return sym(FNAME,fname);	
			}

string{ws}*"("		{  	op_assert = 0; 
			 	yypushback(1);
				//yyparser.yylval.fname.i =  FuncName.STRING;
				fname = new FuncName();
				fname.i = FuncName.STRING;
				return sym(FNAME,fname);	
			}


concat{ws}*"("		{  	op_assert = 0; 
			 	yypushback(1);
				//yyparser.yylval.fname.i = FuncName.CONCAT;
				fname = new FuncName();
				fname.i = FuncName.CONCAT;
				return sym(FNAME,fname);	
			}

starts-with{ws}*"("	{  	op_assert = 0; 
			 	yypushback(1);
				//yyparser.yylval.fname.i = FuncName.STARTS_WITH;
				fname = new FuncName();
				fname.i = FuncName.STARTS_WITH;
				return sym(FNAME,fname);	
			}

contains{ws}*"("	{  	op_assert = 0; 
			 	yypushback(1);
				//yyparser.yylval.fname.i = FuncName.CONTAINS;
				fname = new FuncName();
				fname.i = FuncName.CONTAINS;
				return sym(FNAME,fname);	
			}

substring-before{ws}*"("  {  	op_assert = 0; 
			 	yypushback(1);
				//yyparser.yylval.fname.i = FuncName.SUBSTRING_BEFORE;
				fname = new FuncName();
				fname.i = FuncName.SUBSTRING_BEFORE;
				return sym(FNAME,fname);	
			}

substring-after{ws}*"(" {  	op_assert = 0; 
			 	yypushback(1);
				//yyparser.yylval.fname.i = FuncName.SUBSTRING_AFTER;
				fname = new FuncName();
				fname.i = FuncName.SUBSTRING_AFTER;
				return sym(FNAME,fname);	
			} 	

substring{ws}*"("	{  	op_assert = 0; 
			 	yypushback(1);
				//yyparser.yylval.fname.i = FuncName.SUBSTRING;
				fname = new FuncName();
				fname.i = FuncName.SUBSTRING;
				return sym(FNAME,fname);	
			}

string-length{ws}*"("	{  	op_assert = 0; 
			 	yypushback(1);
				//yyparser.yylval.fname.i = FuncName.STRING_LENGTH;
				fname = new FuncName();
				fname.i = FuncName.STRING_LENGTH;
				return sym(FNAME,fname);	
			}

normalize-space{ws}*"("	{  	op_assert =0; 
			 	yypushback(1);
				//yyparser.yylval.fname.i = FuncName.NORMALIZE_SPACE;
				fname = new FuncName();
				fname.i = FuncName.NORMALIZE_SPACE;
				return sym(FNAME,fname);	
			}

translate{ws}*"("	{  	op_assert = 0; 
			 	yypushback(1);
				//yyparser.yylval.fname.i = FuncName.TRANSLATE;
				fname = new FuncName();
				fname.i = FuncName.TRANSLATE;
				return sym(FNAME,fname);
			}

boolean{ws}*"("		{  	op_assert = 0; 
			 	yypushback(1);
				//yyparser.yylval.fname.i = FuncName.BOOLEAN;
				fname = new FuncName();
				fname.i = FuncName.BOOLEAN;
				return sym(FNAME,fname);	
			}

not{ws}*"("		{  	op_assert = 0; 
			 	yypushback(1);
				//yyparser.yylval.fname.i = FuncName.NOT;
				fname = new FuncName();
				fname.i = FuncName.NOT;
				return sym(FNAME,fname);	
			}

true{ws}*"("		{  	op_assert = 0; 
			 	yypushback(1);
				//yyparser.yylval.fname.i = FuncName.TRUE;
				fname = new FuncName();
				fname.i = FuncName.TRUE;
				return sym(FNAME,fname);	
			}

false{ws}*"("		{  	op_assert = 0; 
			 	yypushback(1);
				//yyparser.yylval.fname.i = FuncName.FALSE;
				fname = new FuncName();
				fname.i = FuncName.FALSE;
				return sym(FNAME,fname);	
			}

lang{ws}*"("		{  	op_assert = 0; 
			 	yypushback(1);
				//yyparser.yylval.fname.i = FuncName.LANG;
				fname = new FuncName();
				fname.i = FuncName.LANG;
				return sym(FNAME,fname);	
			}

number{ws}*"("		{  	op_assert = 0; 
			 	yypushback(1);
				//yyparser.yylval.fname.i = FuncName.NUMBER;
				fname = new FuncName();
				fname.i = FuncName.NUMBER;
				return sym(FNAME,fname);	
			}

sum{ws}*"("		{  	op_assert = 0; 
			 	yypushback(1);
				//yyparser.yylval.fname.i = FuncName.SUM;
				fname = new FuncName();
				fname.i = FuncName.SUM;
				return sym(FNAME,fname);	
			}

floor{ws}*"("		{  	op_assert = 0; 
			 	yypushback(1);
				//yyparser.yylval.fname.i = FuncName.FLOOR;
				fname = new FuncName();
				fname.i = FuncName.FLOOR;
				return sym(FNAME,fname);	
			}

ceiling{ws}*"("		{  	op_assert = 0; 
			 	yypushback(1);
				//yyparser.yylval.fname.i = FuncName.CEILING;
				fname = new FuncName();
				fname.i = FuncName.CEILING;
				return sym(FNAME,fname);	
			}

round{ws}*"("		{  	op_assert = 0; 
			 	yypushback(1);
				//yyparser.yylval.fname.i = FuncName.ROUND;
				fname = new FuncName();
				fname.i = FuncName.ROUND;
				return sym(FNAME,fname);	
			}

\"[^\"]*\" |
'[^']*'			{
				op_assert = 0;
				len = yytext().length();
				literal = yytext().substring(1, len-1);
				return sym(LITERAL,literal);
			}

{digits}("."{digits}?)? |
"."{digits}		{
				op_assert = 0;
				number = new Double(yytext());
				//System.out.println("number returned ==> "+ Double.parseDouble(yytext()));
				return sym(NUMBER,number);
			}

text{ws}*"("{ws}*")"	{	
				op_assert = 0;
				ntest = new Ntest();
				ntest.i = Ntest.TEXT;
				ntest.arg = null;
				return sym(NTEST,ntest);
			}

comment{ws}*"("{ws}*")"	{
				op_assert = 0;
				ntest = new Ntest();
				ntest.i = Ntest.COMMENT;
				ntest.arg =  null;
				return sym(NTEST,ntest);
			}

node{ws}*"("{ws}*")"	{
				op_assert = 0;
				ntest = new Ntest();
				ntest.i = Ntest.NODE;
				ntest.arg = null;
				return sym(NTEST,ntest);
			}

processing-instruction{ws}*"("{ws}*")"	{
						op_assert = 0;
						ntest = new Ntest();
						ntest.i = Ntest.PROCESSING_INSTRUCTION;
						ntest.arg = null;
						return sym(NTEST,ntest);
					}

ancestor{ws}*::		{	op_assert = 0;
				at = new AxisType();
				at.i = AxisType.ANCESTOR;
				return sym(AXISNAME,at);
			}

ancestor-or-self{ws}*::	{	op_assert = 0;
				at = new AxisType();
				at.i = AxisType.ANCESTOR_OR_SELF;
				return sym(AXISNAME,at);
			}


attribute{ws}*::	{	op_assert = 0;
				at = new AxisType();
				at.i = AxisType.ATTRIBUTE;
				return sym(AXISNAME,at);
			}

child{ws}*::		{	op_assert = 0;
				at = new AxisType();
				at.i = AxisType.CHILD;
				return sym(AXISNAME,at);
			}

descendant{ws}*::	{	op_assert = 0;
				at = new AxisType();
				at.i = AxisType.DESCENDANT;
				return sym(AXISNAME,at);
			}

descendant-or-self{ws}*:: {	op_assert = 0;
				at = new AxisType();
				at.i = AxisType.DESCENDANT_OR_SELF;
				return sym(AXISNAME,at);
			}

following{ws}*::	{	op_assert = 0;
				at = new AxisType();
				at.i = AxisType.FOLLOWING;
				return sym(AXISNAME,at);
			}

following-sibling{ws}*::  {	op_assert = 0;
				at = new AxisType();
				at.i = AxisType.FOLLOWING_SIBLING;
				return sym(AXISNAME,at);
			}

namespace{ws}*::	{	op_assert =0;
				at = new AxisType();
				at.i = AxisType.NAMESPACE;
				return sym(AXISNAME,at);
			}

parent{ws}*::		{	op_assert = 0;
				at = new AxisType();
				at.i = AxisType.PARENT;
				return sym(AXISNAME,at);
			}

preceding{ws}*::	{	op_assert = 0;
				at = new AxisType();
				at.i = AxisType.PRECEDING;
				return sym(AXISNAME,at);
			}

preceding-sibling{ws}*:: {	op_assert = 0;
				at = new AxisType();
				at.i = AxisType.PRECEDING_SIBLING;
				return sym(AXISNAME,at);
			}

self{ws}*::		{	op_assert = 0;
				at = new AxisType();
				at.i = AxisType.SELF;
				return sym(AXISNAME,at);
			}

{nc2}{nc}*:"*"  		{	op_assert = 0;
				len = yytext().length();
				name = new NameType();
				name.prefix = yytext().substring(1,len-1);
				name.localname = "*";
				return sym(NAME,name);
			}

{nc2}{nc}*:{nc}+ |
{nc2}{nc}*		{	op_assert = 0;
				name = new NameType();
				name.qname = new String(yytext());
				//System.out.println("returned a NAME ==>" + yytext());
				if (yytext().charAt(0) =='-'){
				    throw new XPathParseException("Invalid char in name token:"+yytext());
				}
				return sym(NAME,name);
			}

