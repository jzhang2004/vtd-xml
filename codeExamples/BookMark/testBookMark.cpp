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

// test book mark
int main(){
	VTDGen *vg = new VTDGen();
	AutoPilot *ap = new AutoPilot();
	if (vg->parseFile(true,"c:\\xml\\oldpo.xml")){
		VTDNav *vn;
		try{
			vn = vg->getNav();
			ap->bind(vn);
			BookMark *bm = new BookMark();
			bm->bind(vn);
			ap->selectXPath(L"/purchaseOrder/items/item[@partNum='872-AA']/USPrice[.>100]");
			//ap->selectXPath(L"/purchaseOrder/items/item[@partNum='872-AA']");
			ap->printExprString();
			printf("\n");
			int i=-1;
			while((i=ap->evalXPath())!=-1){
				// remember the cursor position after navigation using xpath
				bm->recordCursorPosition(vn); //recordCursorPosition2(bm) also works.
				printf(" index val ==> %d \n", vn->getCurrentIndex());
			}
			vn->toElement(ROOT); // set the cursor to root
			printf(" index val ==> %d \n", vn->getCurrentIndex());
			bm->setCursorPosition(); // set the cursor back to remembered position
			printf(" index val ==> %d \n", vn->getCurrentIndex());
		}catch(...){
		}

		delete vn->getXML();
		delete vn;
		//delete bm;
		delete ap;
	}

	delete vg;
	return 0;
}