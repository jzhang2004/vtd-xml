#include "everything.h"
struct exception_context the_exception_context[1];
int main(){
	exception e;
	VTDGen *vg = NULL;
	VTDNav *vn = NULL;
	AutoPilot *ap = NULL;
	FILE *f = NULL;
	UCSChar *string = NULL; 
	int i;
   	f = fopen("d:/ximpleware_2.2_c/vtd-xml/codeGuru/5/input.vxl","rb");
	if (f==NULL)
		return 0;
	Try{
		ap = createAutoPilot2();
		selectXPath(ap,L"/a/b/text()");
		vg = createVTDGen();		
		vn = loadIndex (vg,f);
		bind(ap,vn);
		while((i=evalXPath(ap))!=-1){
			string = toString(vn,i);
			wprintf(L"the text node value is %d ==> %s \n",i,string);
			free(string);
		}
		free(vn->XMLDoc);		
	}Catch(e){// handle various types of exceptions here
	}
	fclose(f);		
	freeAutoPilot(ap);
	freeVTDGen(vg);
	freeVTDNav(vn);
	return 0;
}

