#include "everything.h"
struct exception_context the_exception_context[1];
int main(){
	exception e;
	VTDGen *vg = NULL;
	VTDNav *vn = NULL;
	AutoPilot *ap = NULL;
	XMLModifier *xm = NULL;
	FILE *f = NULL;
	UCSChar *string = NULL; 
	int i;
    
	Try{
		xm = createXMLModifier();
		ap = createAutoPilot2();
		selectXPath(ap,L"/root/b");
		vg = createVTDGen();
		if (parseFile(vg,FALSE,"d:/ximpleware_2.2_c/vtd-xml/codeGuru/7/input.xml")){
			vn = getNav(vg,f);
			bind(ap,vn);
			bind4XMLModifier(xm,vn);
			while((i=evalXPath(ap))!=-1){
				remove4XMLModifier(xm);
			}
			output2(xm,"d:/ximpleware_2.2_c/vtd-xml/codeGuru/7/new.xml");
			free(vn->XMLDoc);
		}
		else
			free(vg->XMLDoc);
	}Catch(e){// handle various types of exceptions here
	}
	freeXMLModifier(xm);
	freeAutoPilot(ap);
	freeVTDGen(vg);
	freeVTDNav(vn);
	return 0;
}

