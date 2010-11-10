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

//write index (integrated)
int main(){
	FILE *fw = NULL;
	char* filename = "oldpo.xml";
	VTDGen *vg = NULL; // This is the VTDGen that parses XML

	// allocate a piece of buffer then reads in the document content
	// assume "c:\soap2.xml" is the name of the file
	fw = fopen("c:\\xml\\oldpo.vxl","wb");
	//stat(filename,&s);

	//i = (int) s.st_size;	
	//wprintf(L"size of the file is %d \n",i);
	//xml = (UByte *)malloc(sizeof(UByte) *i);
	//i = fread(xml,sizeof(UByte),i,f);
	try{
		vg = new VTDGen();
		if (vg->parseFile(true,"c:\\xml\\oldpo.xml")){
			vg->writeIndex(fw);
		}
		delete (vg);
		fclose(fw);
	}
	catch(...) {

		delete(vg);
	}
	return 0;
}
