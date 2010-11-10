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
	int i = 0,t,result,len,count=0;
	UCSChar *tmpStr = NULL;
	char* xml = "<root>good</root>";
	
	VTDNav *vn = NULL;
	try{
		VTDGen vg;
		vg.setDoc(xml,strlen((const char*)xml));
		vg.parse(true);
		vn = vg.getNav();
		i = vn->getText();
		tmpStr = vn->toString(i);
		wprintf(L" text value is %s\n",tmpStr);
		delete(tmpStr);
		len=strlen("bad");
		vn->overWrite(i,"bad",0,len);
		tmpStr = vn->toString(i);
		wprintf(L" text value is %s\n",tmpStr);
		delete(tmpStr);
		// remember C has no automatic garbage collector
		// needs to deallocate manually.
		delete(vn);
	}
	catch (...) {
		// manual garbage collection here
		delete (vn);
	}
	return 0;
}