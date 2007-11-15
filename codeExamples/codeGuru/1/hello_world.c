#include "everything.h"
struct exception_context the_exception_context[1];
int main(){
	exception e;
	VTDGen *vg = NULL;
	VTDNav *vn = NULL;
	UCSChar *string = NULL;
	Try{
		vg = createVTDGen();
		if (parseFile(vg,TRUE,"d:/ximpleware_2.2_c/vtd-xml/codeGuru/1/input.xml")){
			vn = getNav(vg);
			if (toElementNS(vn,FIRST_CHILD,L"someURL",L"b")){
				int i = getText(vn);
				if (i!=-1){
					string = toString(vn,i);
					wprintf(L"the text node value is %d ==> %s \n",i,string);
					free(string);
				}
			}
			free(vn->XMLDoc);
		} else {
			free(vg->XMLDoc);
		}
	}Catch(e){// handle various types of exceptions here
	}
	freeVTDGen(vg);
	freeVTDNav(vn);
	return 0;
}
