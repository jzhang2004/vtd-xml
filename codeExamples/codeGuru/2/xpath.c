#include "everything.h"
struct exception_context the_exception_context[1];
int main(){
	exception e;
	VTDGen *vg = NULL;
	VTDNav *vn = NULL;
	AutoPilot *ap = NULL;
	UCSChar *string = NULL; 
	int i;
    
	Try{
		ap = createAutoPilot2();
		selectXPath(ap,L"/a/b/text()");
		vg = createVTDGen();
		if (parseFile(vg,FALSE,"d:/ximpleware_2.2_c/vtd-xml/codeGuru/2/input.xml")){
			vn = getNav(vg);
			bind(ap,vn);
			while((i=evalXPath(ap))!=-1){
				string = toString(vn,i);
				wprintf(L"the text node value is %d ==> %s \n",i,string);
				free(string);
			}
			free(vn->XMLDoc);
		} else {
			free(vg->XMLDoc);
		}
	}Catch(e){// handle various types of exceptions here
	}
	freeAutoPilot(ap);
	freeVTDGen(vg);
	freeVTDNav(vn);
	return 0;
}
