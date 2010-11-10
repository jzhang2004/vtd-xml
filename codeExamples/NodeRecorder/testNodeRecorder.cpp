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
	int i;	

	try{
		VTDGen vg;
		if (vg.parseFile(true,"c:/xml/newpo.xml")){
			AutoPilot ap;
			NodeRecorder nr;
			VTDNav *vn = vg.getNav();
			ap.bind(vn);
			nr.bind(vn);
			if (ap.selectXPath(L"(/*/*/*)[position()=1 or position()=10]")){
				while((i=ap.evalXPath())!=-1){
					nr.record();
				}
				ap.resetXPath();
				// reset the nr before iteration
				nr.resetPointer();
				while((i=nr.iterate())!=-1){
					wprintf(L"string value ==> %s \n",vn->toString(i));
				}
				nr.clear();

				while((i=ap.evalXPath())!=-1){
					nr.record();
				}
				ap.resetXPath();

				nr.resetPointer();
				while((i=nr.iterate())!=-1){
					wprintf(L"string value ==> %s \n",vn->toString(i));
				}
				nr.clear();
			}
			delete vn->getXML();
			delete vn;
		}



	}
	catch (...) {

	}
	return 0;
}
