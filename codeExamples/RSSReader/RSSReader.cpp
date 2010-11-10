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
	FILE *f = NULL;
	int i = 0,t,result,count=0;
	wchar_t *tmpString;	

	char* filename = "c:/xml/servers.xml";
	struct stat s;
	UByte *xml = NULL; // this is the buffer containing the XML content, UByte means unsigned byte
	//VTDGen *vg = NULL; // This is the VTDGen that parses XML
	VTDNav *vn = NULL; // This is the VTDNav that navigates the VTD records
	//AutoPilot *ap = NULL;

	// allocate a piece of buffer then reads in the document content
	// assume "c:\soap2.xml" is the name of the file
	f = fopen(filename,"r");

	stat(filename,&s);

	i = (int) s.st_size;	
	wprintf(L"size of the file is %d \n",i);
	xml = (UByte *)malloc(sizeof(UByte) *i);
	i = fread(xml,sizeof(UByte),i,f);
	try{
		VTDGen vg;
		vg.setDoc(xml,i);
		vg.parse(true);
		vn = vg.getNav();
		AutoPilot ap;
		ap.declareXPathNameSpace(L"ns1",L"http://purl.org/dc/elements/1.1/");
		if (ap.selectXPath(L"//ns1:*")){
			ap.bind(vn);
			while((result=ap.evalXPath())!= -1){
				wprintf(L"result is %d \n",result);
				tmpString = vn->toString(result);		
				wprintf(L"Element name ==> %ls \n",tmpString);
				delete(tmpString);
				t = vn->getText();
				if (t!=-1){
					tmpString = vn->toNormalizedString(t);
					wprintf(L" text ==> %ls \n",tmpString);
					delete(tmpString);
				}
				wprintf(L"\n =======================\n ");
				count ++;
			}
		}
		wprintf(L"\nTotal number of elements %d \n",count);
		fclose(f);
		// remember C has no automatic garbage collector
		// needs to deallocate manually.
		delete(vn);
	}
	catch (...) {
		//if (e.et == parse_exception)
		//	wprintf(L"parse exception e ==> %s \n %s\n", e.msg, e.sub_msg);	
		// manual garbage collection here
		delete vn;
	}

	return 0;
}
