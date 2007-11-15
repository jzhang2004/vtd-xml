#include "everything.h"
struct exception_context the_exception_context[1];
int main(){
	exception e;
	VTDGen *vg = NULL;
	VTDNav *vn = NULL;
	AutoPilot *ap1 = NULL,*ap2 = NULL,*ap3=NULL;
	FILE *f = NULL;
	UCSChar *string = NULL; 
	int i;
	Long l;    
	Try{
		ap1 = createAutoPilot2(); 
		ap2 = createAutoPilot2(); 
		ap3 = createAutoPilot2();
		selectXPath(ap1,L"/root/a");
		selectXPath(ap2,L"/root/b");
		selectXPath(ap3,L"/root/c");
		f = fopen("d:/ximpleware_2.2_c/vtd-xml/codeGuru/10/output.xml","wb");
		vg = createVTDGen();		
		if (parseFile(vg,TRUE,"d:/ximpleware_2.2_c/vtd-xml/codeGuru/10/input.xml")==FALSE){
			free(vg->XMLDoc);
			freeVTDGen(vg);
			return 0;
		}		
		vn = getNav(vg);
		bind(ap1,vn); bind(ap2,vn);bind(ap3,vn);
		fwrite("<root>",1,6,f);
		while((i=evalXPath(ap1))!=-1){
			l = getElementFragment(vn);
			fwrite("\n",1,1,f);
			fwrite(vn->XMLDoc+(int)l,1,(int)(l>>32),f);
		}
		while((i=evalXPath(ap2))!=-1){
			l = getElementFragment(vn);
			fwrite("\n",1,1,f);
			fwrite(vn->XMLDoc+(int)l,1,(int)(l>>32),f);
		}
		while((i=evalXPath(ap3))!=-1){
			l = getElementFragment(vn);
			fwrite("\n",1,1,f);
			fwrite(vn->XMLDoc+(int)l,1,(int)(l>>32),f);
		}
		fwrite("\n</root>",1,8,f);		
		free(vn->XMLDoc);
	}Catch(e){// handle various types of exceptions here
	}
	freeAutoPilot(ap1); freeAutoPilot(ap2); freeAutoPilot(ap3); 
	freeVTDGen(vg);
	freeVTDNav(vn);
	return 0;
}

