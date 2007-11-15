#include "everything.h"
struct exception_context the_exception_context[1];
UCSChar *ba0,*ba1,*ba2,*ba3,*ba4,*ba5;
void convert(VTDNav *vn, XMLModifier *xm){
	
	int i = -1;
	toElement(vn,FIRST_CHILD);
	i = getText(vn);
	updateToken(xm,i,ba0);

	toElement(vn,NEXT_SIBLING);
	i = getText(vn);
	updateToken(xm,i,ba1);

	toElement(vn,NEXT_SIBLING);
	i = getText(vn);
	updateToken(xm,i,ba2);

	toElement(vn,NEXT_SIBLING);
	i = getText(vn);
	updateToken(xm,i,ba3);

	toElement(vn,NEXT_SIBLING);
	i = getText(vn);
	updateToken(xm,i,ba4);

	toElement(vn,NEXT_SIBLING);
	i = getText(vn);
	updateToken(xm,i,ba5);

	toElement(vn,PARENT);
}

int main(){
	exception e;
	VTDGen *vg = NULL;
	VTDNav *vn = NULL;
	AutoPilot *ap1 = NULL;
	XMLModifier *xm = NULL;
	UCSChar *string = NULL; 
	int i;
	Long l;   
	ba0=L"                      ";
	ba1=L"              ";
	ba2=L"       ";
	ba3=L"              ";
	ba4=L"        ";
	ba5=L"        ";
	Try{			
		vg = createVTDGen();		
		if (parseFile(vg,TRUE,"d:/ximpleware_2.2_c/vtd-xml/codeGuru/11/old_cd.xml")==FALSE){
			free(vg->XMLDoc);
			freeVTDGen(vg);
			return 0;
		}	
		xm = createXMLModifier();
		ap1 = createAutoPilot2();
		selectXPath(ap1,L"/CATALOG/CD");
		vn = getNav(vg);
		bind(ap1,vn);
		bind4XMLModifier(xm,vn);		
		while((i=evalXPath(ap1))!=-1){
			convert(vn,xm);		
		}	
		output2(xm,"d:/ximpleware_2.2_c/vtd-xml/codeGuru/11/cd_template.xml");
		free(vn->XMLDoc);
	}Catch(e){// handle various types of exceptions here
	}
	freeAutoPilot(ap1);
	freeXMLModifier(xm);
	freeVTDGen(vg);
	freeVTDNav(vn);
	return 0;
}

