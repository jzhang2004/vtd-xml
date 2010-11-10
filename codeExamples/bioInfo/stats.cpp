#include <stdio.h>
#include <stdlib.h>
#include <iostream>
//#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <io.h>
#include "vtdGen.h"
#include "vtdNav.h"
#include "autoPilot.h"
#include "XMLModifier.h"
#include "bookMark.h"
#include "nodeRecorder.h"
//#include "bookMark.h"

using namespace com_ximpleware;

int main(){
	FILE *f = NULL;
	int i = 0,count=0,par_count=0,v=0;

	char* filename = "c:/xml/bioinfo.xml";
	struct stat s;
	UByte *xml = NULL; // this is the buffer containing the XML content, UByte means unsigned byte


	// allocate a piece of buffer then reads in the document content
	// assume "c:\soap2.xml" is the name of the file
	f = fopen(filename,"r");

	stat(filename,&s);

	i = (int) s.st_size;	
	wprintf(L"size of the file is %d \n",i);

	xml = new UByte[i];
	i = fread(xml,sizeof(UByte),i,f);
	try{
		VTDGen vg;
		vg.setDoc(xml,i);
		vg.parse(true);
		VTDNav *vn = vg.getNav();
		AutoPilot ap;
		ap.bind(vn);
		if (ap.selectXPath(L"/bix/package/command/parlist")){
			while(ap.evalXPath()!= -1){
				count++;
			}
		}

		if (ap.selectXPath(L"/bix/package/command/parlist/par")){
			while(ap.evalXPath()!= -1){
				par_count++;
			}
		}
		wprintf(L"count ==> %d \n",count);
		wprintf(L"par_count ==> %d \n",par_count);

		vn->toElement(ROOT);
		ap.selectElement(L"par");
		while(ap.iterate()){
			if (vn->getCurrentDepth() == 4){
				v++;
			}
		}
		wprintf(L"verify ==> %d \n",v);
		fclose(f);
		// remember C has no automatic garbage collector
		// needs to deallocate manually.
		delete(vn);
		delete xml;
	}
	catch (...) {

	}
	return 0;
}