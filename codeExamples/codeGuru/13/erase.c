#include "everything.h"
struct exception_context the_exception_context[1];
int main(){
	exception e;
	VTDGen *vg = NULL;
	VTDNav *vn = NULL;
	AutoPilot *ap1 = NULL;
	FILE *f = NULL;
	UCSChar *string = NULL;
	int i;
	Try{			
		vg = createVTDGen();		
		if (parseFile(vg,TRUE,"d:/ximpleware_2.2_c/vtd-xml/codeGuru/13/old_cd.xml")==FALSE){
			free(vg->XMLDoc);
			freeVTDGen(vg);
			return 0;
		}	
		ap1 = createAutoPilot2();
		selectXPath(ap1,L"/CATALOG/CD[PRICE=10.2]/*/text()");
		vn = getNav(vg);
		bind(ap1,vn);
		while((i=evalXPath(ap1))!=-1){
			overWrite(vn,i,"",0,0);		
		}
		f= fopen("d:/ximpleware_2.2_c/vtd-xml/codeGuru/13/new_cd.xml","wb");
		fwrite(vn->XMLDoc+vn->docOffset,1,vn->docLen,f);
		fclose(f);
		free(vn->XMLDoc);
	}Catch(e){// handle various types of exceptions here
	}
	freeAutoPilot(ap1);
	freeVTDGen(vg);
	freeVTDNav(vn);
	return 0;
}

