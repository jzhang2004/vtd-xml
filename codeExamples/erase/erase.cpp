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

//erase
int main(){	
	try{
		VTDGen *vg = NULL; /* This is the VTDGen that parses XML */
		VTDNav *vn = NULL; /* This is the VTDNav that navigates the VTD records */
		AutoPilot *ap = NULL, *ap2=NULL;
		XMLModifier *xm = NULL;
		ElementFragmentNs *ef = NULL;
		int i= -1;

		vg = new VTDGen();
		ap = new AutoPilot();
		ap2 = new AutoPilot();
		xm = new XMLModifier();
		//selectXPath(ap,L"(/*/*/*)[position()>1 and position()<4]");
		ap2->selectXPath(L"//@*");
		printf("hi \n");
		if (vg->parseFile(true, "c:/xml/soap2.xml")){
			//FILE *f1 = fopen("./example/new3.xml","wb");
			vn = vg->getNav();
			printf("hi \n");
			//bind(ap,vn);
			ap2->bind(vn);
			printf("hi \n");
			//bind4XMLModifier(xm,vn);
			//i=evalXPath(ap2);
			//printf(" i's value is %d \n",i);
			//l=getElementFragment(vn);
			//ef = getElementFragmentNs(vn);
			//writeFragmentToFile(ef,f1);
			//fclose(f1);
			ap2->printExprString();
			while( (i=ap2->evalXPath())!=-1){
				//insertAfterElement4(xm,ef);
				//insertAfterElement3(xm,vn->XMLDoc,(int)l,(int)(l>>32));
				printf(" i's value is %d \n",i);
				vn->overWrite(i+1,(UByte *)"",0,0);
			}
			vn->dumpXML("c:/xml/newSoap.xml");
			//fwrite(vn->XMLDoc+vn->docOffset,sizeof(UByte),vn->docLen,f1);
			//xm->output("c:/xml/new3.xml");
			//fclose(f1);
			delete (vn->getXML());
			delete (vn);
		}
		delete (ef);
		delete (xm);
		delete (ap);
		delete (ap2);
		delete (vg);

	}catch(...){
		printf("exception !!!!!!!!!!! \n");
	}
	return 0;
}

