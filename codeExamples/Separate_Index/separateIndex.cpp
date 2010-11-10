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

// load separate index
int main(){
	VTDGen *vg = NULL, *vg2=NULL;
	VTDNav *vn = NULL;
	AutoPilot *ap = NULL;
	int result;

	try{	
		vg = new VTDGen();
		// parse file	
		if (vg->parseFile(true,"c:/xml/oldpo.xml")==false){
			// parsing failed
			free(vg->getXML());
			free(vg);
			return 0;	
		}

		// write index
		vg->writeSeparateIndex("c:/xml/oldpo.vtd");
		vg2 = new VTDGen();
		vn = vg2->loadSeparateIndex("c:/xml/oldpo.xml","c:/xml/oldpo.vtd");

		ap = new AutoPilot(vn);

		//bind(ap, vn); 
		ap->selectXPath(L"//*");

		while((result= ap->evalXPath())!=-1){
			UCSChar* s = vn->toString(result);
			wprintf(L" result == %d  %s\n",result,s);
			//insertAfterHead2(xm,"abcd",4);
			//updateToken(xm, result, L"abc");	
		}
		free(vn->getXML());
		free(ap);
		free(vn);	  
		free(vg);
		free(vg2);

	}
	catch (...) {

		// manual garbage collection here
		free(vg);
		free(vg2);
	}	
	return 0;
}