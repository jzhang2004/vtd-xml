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
		if (parseFile(vg,FALSE,"d:/ximpleware_2.2_c/vtd-xml/codeGuru/8/input.xml")){
			vn = getNav(vg,f);
			bind(ap,vn);
			bind4XMLModifier(xm,vn);
			while((i=evalXPath(ap))!=-1){
				insertBeforeElement2(xm,"<lala/>\n",8);
				insertAfterElement2(xm,"\n<lala/>",8);
			}
			output2(xm,"d:/ximpleware_2.2_c/vtd-xml/codeGuru/8/new.xml");
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

