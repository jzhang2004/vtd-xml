#include "everything.h"

struct exception_context the_exception_context[1];
int main(){
	exception e;
	VTDGen *vg = NULL;
	Try{
		vg = createVTDGen();
		if (parseFile(vg, TRUE, "d:/ximpleware_2.2_c/vtd-xml/codeGuru/4/input.xml")){
			writeIndex2(vg,"d:/ximpleware_2.2_c/vtd-xml/codeGuru/5/input.vxl");
		}
		free(vg->XMLDoc);
	}Catch(e){
	}
	freeVTDGen(vg);
}

