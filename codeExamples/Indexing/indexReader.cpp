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

	UByte *xml = NULL; // this is the buffer containing the XML content, UByte means unsigned byte
	
	VTDNav *vn = NULL; // This is the VTDNav that navigates the VTD records
	

	// allocate a piece of buffer then reads in the document content
	// assume "c:\soap2.xml" is the name of the file
	f = fopen("c:/xml/oldpo.vxl","rb");
	fw = fopen("c:/xml/newpo_update.xml","wb");

	try{
		VTDGen vg;
		vn = vg.loadIndex(f);
		AutoPilot ap;
		XMLModifier xm;
		if (ap.selectXPath(L"/purchaseOrder/items/item[@partNum='872-AA']")){
			ap.bind(vn);
			xm.bind(vn);
			while((result=ap.evalXPath())!= -1){
				xm.remove();
				xm.insertBeforeElement(L"<something/>");	
			}
		}
		if (ap.selectXPath(L"/purchaseOrder/items/item/USPrice[.<40]/text()")){
			while((result=ap.evalXPath())!= -1){
				xm.updateToken(result,L"200");
			}
		}
		xm.output(fw);

		fclose(f);
		fclose(fw);
		// remember C has no automatic garbage collector
		// needs to deallocate manually.
		delete (vn);
		//delete (vg);
		//delete (xm);
		//delete (ap);
	}
	catch (...) {
		// manual garbage collection here
		delete (vn);
	}

	return 0;

}