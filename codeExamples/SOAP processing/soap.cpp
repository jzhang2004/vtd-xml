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
	FILE *fo = NULL;
	int i = 0;

	Long l = 0;
	int len = 0;
	int offset = 0;

	char* filename = "c:/xml/soap2.xml";
	struct stat s;
	UByte *xml = NULL; // this is the buffer containing the XML content, UByte means unsigned byte
	//VTDGen *vg = NULL; // This is the VTDGen that parses XML
	VTDNav *vn = NULL; // This is the VTDNav that navigates the VTD records
	AutoPilot *ap = NULL;
	char *sm = "\n================\n";

	// allocate a piece of buffer then reads in the document content
	// assume "c:\soap2.xml" is the name of the file
	f = fopen(filename,"r");
	fo = fopen("c:/xml/out.txt","w");

	stat(filename,&s);

	i = (int) s.st_size;	
	printf("size of the file is %d \n",i);
	xml = new UByte[i];
	fread(xml,sizeof(UByte),i,f);
	VTDGen vg;
	try{
		
		vg.setDoc(xml,i);
		vg.parse(true);
		vn = vg.getNav();
		AutoPilot ap;
		ap.declareXPathNameSpace(L"ns1",L"http://www.w3.org/2003/05/soap-envelope");
		//if (ap.selectXPath(L"/ns1:Envelope/ns1:Header/*[@ns1:mustUnderstand]")){
		if (ap.selectXPath(L"/ns1:Envelope/ns1:Header/*[@ns1:mustUnderstand]")){
		//if (ap.selectXPath(L"/a/b/*")){
			ap.printExprString();
			ap.bind(vn);
			int i=-1;
			while((i=ap.evalXPath())!= -1){
				//printf("\n hi ==> %d \n",i);
				l = vn->getElementFragment();
				offset = (int) l;
				len = (int) (l>>32);
				fwrite((char *)(xml+offset),sizeof(UByte),len,fo);
				fwrite((char *) sm,sizeof(UByte),strlen((char*)sm),fo);
			}
		}
		fclose(f);
		fclose(fo);
		// remember C has no automatic garbage collector
		// needs to deallocate manually.
		delete(vn);
	}
	catch (ParseException &e){
		//vg.printLineNumber();
		printf(" error ===> %s \n",e.getMessage());
	}
	catch (...) {
		delete (vn);
	}
	return 0;
}	