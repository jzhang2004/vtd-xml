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

/*This is the XPath version of RSSReader.cs
 */
using System;
using System.Collections.Generic;
using System.Text;
using com.ximpleware;

namespace RSSReader2
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                // open a file and read the content into a byte array
                VTDGen vg = new VTDGen();
                if (vg.parseFile("./servers.xml", true))
                {
                    VTDNav vn = vg.getNav();
                    AutoPilot ap = new AutoPilot(vn);
                    ap.selectElementNS("http://purl.org/dc/elements/1.1/", "*"); // select name space here; * matches any local name
                    int count = 0;
                    while (ap.iterate())
                    {
                        Console.Write("" + vn.getCurrentIndex() + "  ");
                        Console.WriteLine("Element name ==> " + vn.toString(vn.getCurrentIndex()));
                        int t = vn.getText(); // get the index of the text (char data or CDATA)
                        if (t != -1)
                            Console.WriteLine(" Text  ==> " + vn.toNormalizedString(t));
                        Console.WriteLine("\n ============================== ");
                        count++;
                    }
                    Console.WriteLine("Total # of element " + count);
                }
            }
            catch (NavException e)
            {
                Console.WriteLine(" Exception during navigation " + e);
            }
        }
    }
}

