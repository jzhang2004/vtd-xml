/* 
 * Copyright (C) 2002-2008 XimpleWare, info@ximpleware.com
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

/* In this C# program, we demonstrate how to use AutoPilot and VTDNav
 * class to filter elements of a particular name space. We also are going 
 * to use VTDGen's parseFile to simplify programming.
 */
using System;
using System.Collections.Generic;
using System.Text;
using com.ximpleware;

namespace stats
{
    class Program
    {
	static void Main(string[] args)
        {
            VTDGen vg = new VTDGen();
            if (vg.parseFile("mix3.xml", true))
            {
                vg.writeSeparateIndex("mix32.vtd");
            }
            VTDNav vn = vg.loadSeparateIndex("mix3.xml", "mix3.vtd");
            AutoPilot ap = new AutoPilot(vn);
            ap.selectXPath("//*");
            int i;
            while((i=ap.evalXPath())!=-1){
                Console.WriteLine("element name: "+vn.toString(i));
            }
        }
       
    }
}
