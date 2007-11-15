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
    f = fopen("d:/ximpleware_2.2_c/vtd-xml/codeGuru/6/input.vxl","rb");
	if (f==NULL)
		return 0;
	Try{
		xm = createXMLModifier();
		ap = createAutoPilot2();
		selectXPath(ap,L"/a/b");
		vg = createVTDGen();		
		vn = loadIndex (vg,f);
		bind(ap,vn);
		bind4XMLModifier(xm,vn);
		while((i=evalXPath(ap))!=-1){
			insertAttribute(xm,L" attr1='val'");
		}
		output2(xm,"d:/ximpleware_2.2_c/vtd-xml/codeGuru/6/new.xml");
		free(vn->XMLDoc);		
	}Catch(e){// handle various types of exceptions here
	}
	fclose(f);		
	freeAutoPilot(ap);
	freeXMLModifier(xm);
	freeVTDGen(vg);
	freeVTDNav(vn);
	return 0;
}

