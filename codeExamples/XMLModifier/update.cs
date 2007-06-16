/* 
* Copyright (C) 2002-2006 XimpleWare, info@ximpleware.com
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

using System;
using System.Collections.Generic;
using System.Text;
using com.ximpleware;
/* In this java program, we demonstrate how to use XMLModifier to incrementally
 * update an simple XML purchase order.
 * a particular name space. We also are going 
 * to use VTDGen's parseFile to simplify programming.
 */
namespace update
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                // open a file and read the content into a byte array
                VTDGen vg = new VTDGen();
                if (vg.parseFile("./oldpo.xml", true))
                {
                    VTDNav vn = vg.getNav();
                    System.IO.FileInfo f1 = new System.IO.FileInfo("./newpo.txt");
                    System.IO.FileStream fos = new System.IO.FileStream(f1.FullName, System.IO.FileMode.Create);
                    
                    AutoPilot ap = new AutoPilot(vn);
                    XMLModifier xm = new XMLModifier(vn);
                    ap.selectXPath("/purchaseOrder/items/item[@partNum='872-AA']");
                    int i = -1;
                    while ((i = ap.evalXPath()) != -1)
                    {
                        xm.remove();
                        xm.insertBeforeElement("<something/>\n");
                    }
                    ap.selectXPath("/purchaseOrder/items/item/USPrice[.<40]/text()");
                    while ((i = ap.evalXPath()) != -1)
                    {
                        xm.updateToken(i, "200");
                    }
                    xm.output(fos);
                    fos.Close();
                }
            }
            catch (NavException e)
            {
                Console.WriteLine(" Exception during navigation " + e);
            }
            catch (ModifyException e)
            {
                Console.WriteLine(" Modify exception occurred " + e);
            }
            catch (System.IO.IOException e)
            {
                System.Console.Out.WriteLine(" IO exception condition" + e);
            }
        }
    }
 }

}

