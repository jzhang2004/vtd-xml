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

	FILE *f = NULL ,*fw = NULL;
	int i = 0,result,count=0;
	//wchar_t *tmpString;	

	char* filename = "c:/xml/oldpo.xml";
	struct stat s;
	UByte *xml = NULL; // this is the buffer containing the XML content, UByte means unsigned byte
	VTDGen *vg = NULL; // This is the VTDGen that parses XML
	VTDNav *vn = NULL; // This is the VTDNav that navigates the VTD records
	AutoPilot *ap = NULL;
	XMLModifier *xm = NULL;

	// allocate a piece of buffer then reads in the document content
	// assume "c:\soap2.xml" is the name of the file
	f = fopen(filename,"rb");
	fw = fopen("c:/xml/newpo.xml","wb");
	stat(filename,&s);

	i = (int) s.st_size;	
	wprintf(L"size of the file is %d \n",i);
	xml =  new UByte[i];
	i = fread(xml,sizeof(UByte),i,f);
	try{
		VTDGen vg;
		vg.setDoc(xml,i);
		vg.parse(true);
		vn = vg.getNav();
		AutoPilot ap;//= createAutoPilot2();
		XMLModifier xm;
		ap.bind(vn);
		xm.bind(vn);
		if(ap.selectXPath(L"/purchaseOrder/items/item[@partNum='872-AA']")){

			while((result=ap.evalXPath())!= -1){
				xm.remove();
				xm.insertBeforeElement(L"<something/>");	
			}
		}
		result = -1;
		if (ap.selectXPath(L"/purchaseOrder/items/item/USPrice[.<40]/text()")){
			ap.printExprString();
			printf("\n");
			while((result=ap.evalXPath())!= -1){
				xm.updateToken(result,L"200");
			}
		}
		xm.output(fw);

		fclose(f);
		fclose(fw);
		// remember C has no automatic garbage collector
		// needs to deallocate manually.
		delete(vn);

	}
	catch (ParseException& e) {
		printf(e.getMessage());
	}
	delete xml;
	return 0;
}