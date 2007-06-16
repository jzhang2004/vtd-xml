/* 
 * Copyright (C) 2002-2007 XimpleWare, info@ximpleware.com
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
            try
            {

                // counting child elements of parlist
                int count = 0;
                // counting child elements of parlist named "par"
                int par_count = 0;

                VTDGen vg = new VTDGen();
                if (vg.parseFile("./bioinfo.xml", true))
                {

                    VTDNav vn = vg.getNav();
                    if (vn.matchElement("bix"))
                    { // match blix
                        // to first child named "package"
                        if (vn.toElement(VTDNav.FC, "package"))
                        {
                            do
                            {
                                Console.WriteLine("package");
                                // to first child named "command"
                                if (vn.toElement(VTDNav.FC, "command"))
                                {
                                    do
                                    {
                                        Console.WriteLine("command");
                                        if (vn.toElement(VTDNav.FC, "parlist"))
                                        {
                                            do
                                            {
                                                Console.WriteLine("parlist");
                                                count++; //increment count
                                                if (vn.toElement(VTDNav.FC))
                                                {
                                                    do
                                                    {
                                                        if (vn.matchElement("par"))
                                                            par_count++;
                                                    }
                                                    while (vn.toElement(VTDNav.NS));
                                                    vn.toElement(VTDNav.P);
                                                }
                                            }
                                            while (vn.toElement(VTDNav.NS, "parlist"));
                                            vn.toElement(VTDNav.P);
                                        }
                                    }
                                    // to next silbing named "command"
                                    while (vn.toElement(VTDNav.NS, "command"));
                                    vn.toElement(VTDNav.P); // go up one level
                                }
                                else
                                    Console.WriteLine(" no child element named 'command' ");
                                // verify result

                            }
                            while (vn.toElement(VTDNav.NS, "package")); // to next sibling named "package"
                            vn.toElement(VTDNav.P); // go up one level	
                        }
                        else
                            Console.WriteLine(" no child element named 'package' ");
                    }
                    else
                        Console.WriteLine(" Root is not 'bix' ");
                    // print out the results
                    Console.WriteLine(" count ====> " + count);
                    Console.WriteLine(" par_count ==> " + par_count);

                    // verify results using iterators
                    int v = 0;
                    vn.toElement(VTDNav.ROOT);
                    AutoPilot ap = new AutoPilot(vn);
                    ap.selectElement("par");
                    while (ap.iterate())
                    {
                        if (vn.getCurrentDepth() == 4)
                        {
                            v++;
                        }
                    }
                    Console.WriteLine(" verify ==> " + v);
                }
            }
            catch (NavException e)
            {
                Console.WriteLine(" Exception during navigation " + e);
            }

        }
    }
}
