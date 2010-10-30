#include <stdio.h>
#include <iostream>
#include "UTF8Char.h"
#include "test.h"
#include "XMLChar.h"
#include "expr.h"
#include "autoPilot.h"
#include "FlexLexer.h"
#include <sstream>
using namespace std;
using namespace com_ximpleware;
using namespace abc1;
//using namespace abc1;



int a[10];
int& abc(int b){
	return a[b];
}

class  test {
	
public:	int a;
private: int b;
};

struct s{
	int a;
	double b;
	s(int i):
	a(i)
	{
	}
};
class Foo
{
        Foo() try : _str( "text of string" ) ,a(1),b(2)
        { 
        } 
        catch ( ... ) 
        { 
                std::cerr << "Couldn't create _str";
                // now, the exception is rethrown as if we'd written
                // "throw;" here
        }

		int a;
		int b;
		std::string _str;

		void try1(){
			throw abc1::i;
		}
};


int main(){
/*test *t1=new test();
i=1;
//s s1 = {1,2};
printf("a is %d \n",t1->a);
const int i=1;
const int& j=i;
int a[10],c[10];
const int * const b = a;
//throw std::bad_alloc("array list allocation in FastLongBuffer's constructor failed");
abc1::i=1;
printf("a is %d \n",abc1::i);
try_good;
abc1::f1();
abc1::f2();*/
	wchar_t* tests[] =	{
		L"child::*[child::a or self::b][position()=last() - 1]",
		L"/descendant::test:*",L"abc.abc",
		L"/descendant::test:*",
		L"/descendant::test:e",
		L"/descendant::test:f",
		L"/root/test:*/text()",
		
		L"a/d[2]/e",
		L"*[c or d]",
		L"a/c[d]",
		L"a/c[d=\"Text for D\"]",
		L"a[3][@a1=\"va1\"]",
		L"a[@a1=\"va1\"][1]",
		L"a[@a1=\"va1\"]",
		L"/root/a/c/../@a1",
		L"/root/a/..",
		L".//c",
		L".",
		L"//c/d",
		L"//c",
		L"/root/a//d",
		L"/root/a/d",
		L"//a",
		L"//d",
		L"a/d[2]/e",
		L"*/d",
		L"a[last()]",
		L"a[1]",
		L"/root/a[1]/@*",
		L"/root/a[1]/@a1",
		L"text()",
		L"*",
		L"a",
		L"/root",
		L"child::*[self::a or self::b]",
		L"child::a/child::c[child::d='Text for D']",
		L"child::a/child::c[child::d]",
		L"child::a[position()=1][attribute::a1=\"va1\"]",
		L"child::a[attribute::a1=\"va1\"][position()=1]",
		L"child::a[attribute::a2=\"va2\"]",
		L"child::a/child::d[position()=2]/child::e[position()=1]",
		L"/descendant::a[position()=2]",
		L"child::a[1]/child::d[1]/preceding-sibling::c[position()=1]",
		L"child::a[2]/following-sibling::b[position()=1]",
		L"child::a[position()=2]",
		L"child::a[position()=last()-1]",
		L"child::a[position()=last()]",
		L"child::a[position()=1]",
		L"/descendant::a/child::c",
		L"/",
		L"/descendant::a",
		L"/child::root/child::*/child::d",
		L"child::b/descendant::a",
		L"self::root",
		L"/child::root/child::a/descendant-or-self::a",
		L"/child::root/child::a/child::c/ancestor::root",
		L"/child::root/descendant::a",
		L"/child::root/child::a/attribute::*",
		L"/child::root/child::a/attribute::a1",
		L"/child::root/child::node()",
		L"/child::root/child::a/child::d/child::text()",
		L"/child::root/child::*",
		L"/child::root/child::a",
		L"/root/a[(1+1-1)*2 div 2]",
		L"/child::root/child::a/ancestor-or-self::a",
		L"'hello'",
		L"1+2-3+count(/a/b/c)"
	};

AutoPilot ap;
ap.declareXPathNameSpace(L"test",L"test");
for(int i=0;i<66;i++){
	//ap.declareVariableExpr(L"good",L"/a/b/c");
	wprintf(L"input test string ==> %ls\n",tests[i]);
	ap.selectXPath(tests[i]);
	ap.printExprString();
	printf("\n");
}

//  wstringstream wss(L"/a/b");
//	yyFlexLexer *yfl= new yyFlexLexer(&wss,&wcout);
//	yfl->set_debug(1);
	
//	while(yfl->yylex()!=0){		
//	}

//b = c;
//j=c[0];
}