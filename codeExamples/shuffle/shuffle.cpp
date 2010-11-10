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

	VTDGen vg;
	AutoPilot ap0, ap1, ap2;

	ap0.selectXPath(L"/root/a");
	ap1.selectXPath(L"/root/b");
	ap2.selectXPath(L"/root/c");
	if (vg.parseFile(false,"c:/xml/old.xml"))
	{
		VTDNav* vn = vg.getNav();
		ap0.bind(vn);
		ap1.bind(vn);
		ap2.bind(vn);


		//FileStream fos = new FileStream("new.xml", System.IO.FileMode.OpenOrCreate);
		FILE* fos = fopen("c:/xml/new.xml","wb");
		//fos.Write("<root>".getBytes());
		char *ba1, *ba2, *ba3;
		//ba0 = eg.GetBytes("
		ba1 = "<root>";
		ba2 = "</root>";
		ba3 = "\r\n";
		//fos.Write(ba1, 0, ba1.Length);
		fwrite(ba1,sizeof(char),strlen(ba1),fos);
		char* ba = (char *)vn->getXML();
		while (ap0.evalXPath() != -1)
		{
			Long l = vn->getElementFragment();
			int offset = (int)l;
			int len = (int)(l >> 32);
			fwrite(ba3,sizeof(char),strlen(ba3),fos);
			fwrite(ba+offset,sizeof(char),len,fos);
			//fos.Write(ba3,0,ba3.Length);
			//fos.Write(ba, offset, len);
		}
		ap0.resetXPath();
		while (ap1.evalXPath() != -1)
		{
			Long l = vn->getElementFragment();
			int offset = (int)l;
			int len = (int)(l >> 32);
			fwrite(ba3,sizeof(char),strlen(ba3),fos);
			fwrite(ba+offset,sizeof(char),len,fos);
			//fos.Write(ba3,0,ba3.Length);
			//fos.Write(ba, offset, len);
		}
		ap1.resetXPath();
		while (ap2.evalXPath() != -1)
		{
			Long l = vn->getElementFragment();
			int offset = (int)l;
			int len = (int)(l >> 32);
			fwrite(ba3,sizeof(char),strlen(ba3),fos);
			fwrite(ba+offset,sizeof(char),len,fos);
			//fos.Write(ba3,0,ba3.Length);
			//fos.Write(ba, offset, len);
		}
		ap2.resetXPath();
		fwrite(ba3,sizeof(char),strlen(ba3),fos);
		fwrite(ba2,sizeof(char),strlen(ba2),fos);
		fclose(fos);
		delete (vn->getXML());
		delete (vn);
		//fos.Write(ba3,0,ba3.Length);
		//fos.Write(ba2,0,ba2.Length);
	}
	return 0;
}