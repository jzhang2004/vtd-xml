/* 
 * Copyright (C) 2002-2010 XimpleWare, info@ximpleware.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
// vtd-xml.cpp : Defines the entry point for the console application.
//

//#include <string.h>
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

	try{
		//VTDGen *vg = NULL; /* This is the VTDGen that parses XML */
		VTDNav *vn = NULL; /* This is the VTDNav that navigates the VTD records */
		//AutoPilot *ap = NULL, *ap2=NULL;
		//XMLModifier *xm = NULL;
		ElementFragmentNs *ef = NULL;
		int i= -1;
		Long l= -1;

		VTDGen vg;// = createVTDGen();
		AutoPilot ap; // = createAutoPilot2();
		AutoPilot ap2; //= createAutoPilot2();
		XMLModifier xm; //= createXMLModifier();
		ap.selectXPath(L"(/*/*/*)[position()>1 and position()<4]");
		ap2.selectXPath(L"/*/*/*");
		if (vg.parseFile(true,"c:/xml/soap2.xml")){
			//FILE *f1 = fopen("d:/new3.xml","wb");
			vn = vg.getNav();
			ap.bind(vn);
			ap2.bind(vn);
			xm.bind(vn);
			ap2.evalXPath();
			ef = vn->getElementFragmentNs();

			while( (i=ap.evalXPath())!=-1){
				xm.insertAfterElement(ef);
				printf(" index %d \n",i);
			}
			//fwrite(vn->XMLDoc+vn->docOffset,sizeof(UByte),vn->docLen,f1);
			xm.output("c:/xml/new3.xml");
			//fclose(f1);
			delete (vn->getXML());
			delete (vn);
		}
		delete(ef);
		/*freeXMLModifier(xm);
		freeAutoPilot(ap);
		freeAutoPilot(ap2);
		freeVTDGen(vg);*/

	}catch(...){
		printf("exception !!!!!!!!!!! \n");
	}
	return 0;

}