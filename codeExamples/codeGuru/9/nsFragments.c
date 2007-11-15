#include "everything.h"
struct exception_context the_exception_context[1];
int main(){
	exception e;
	VTDGen *vg1 = NULL, *vg2=NULL;
	VTDNav *vn1 = NULL, *vn2=NULL;
	AutoPilot *ap = NULL;
	XMLModifier *xm = NULL;
	ElementFragmentNs *efn = NULL;
	UCSChar *string = NULL; 
	int i;
	Try{
		xm = createXMLModifier();
		ap = createAutoPilot2();
		selectXPath(ap,L"/root/b");
		
		vg1 = createVTDGen();		
		if (parseFile(vg1,TRUE,"d:/ximpleware_2.2_c/vtd-xml/codeGuru/9/input2.xml")==FALSE){
			free(vg1->XMLDoc);
			freeVTDGen(vg1);
			return 0;
		}
		vg2 = createVTDGen();
		if (parseFile(vg2,TRUE,"d:/ximpleware_2.2_c/vtd-xml/codeGuru/9/input1.xml")==FALSE){
			free(vg2->XMLDoc);
			free(vg1->XMLDoc);
			freeVTDGen(vg1);
			freeVTDGen(vg2);
			return 0;
		}
		vn1 = getNav(vg1,f);
		if (toElementNS(vn1,FIRST_CHILD,L"someURL2",L"b")){
			efn =getElementFragmentNs(vn1);
			vn2 = getNav(vg2,f);
			bind(ap,vn2);
			bind4XMLModifier(xm,vn2);
			while((i=evalXPath(ap))!=-1){
				insertBeforeElement4(xm,efn);
				insertAfterElement4(xm,efn);
			}
			output2(xm,"d:/ximpleware_2.2_c/vtd-xml/codeGuru/9/output.xml");
		}
		free(vn1->XMLDoc);free(vn2->XMLDoc);	
	}Catch(e){// handle various types of exceptions here
	}
	freeXMLModifier(xm);
	freeAutoPilot(ap);freeElementFragmentNs(efn);
	freeVTDGen(vg1);freeVTDGen(vg2);
	freeVTDNav(vn1);freeVTDNav(vn2);
	return 0;
}
