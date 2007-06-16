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

/* In this C# program, we demonstrate content-inspection aspect of our
 * API by processing some parts of the SOAP header 
 * It will pull out header element fragements (in its serialized format) 
 * with attributes MUSTUNDERSTAND
 * The method is "getElementFragment()"
 * It is important because sometimes one must encrypt or sign/verfiy the 
 * digital signature of an element in its serial format! 
 */
using System;
using System.Collections.Generic;
using System.Text;
using com.ximpleware;

namespace SOAPProcessor
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                // open file to output extracted fragments
                System.IO.FileInfo f1 = new System.IO.FileInfo("./out.txt");
                System.IO.FileStream fos = new System.IO.FileStream(f1.FullName, System.IO.FileMode.Create);

                // instantiate the parser
                VTDGen vg = new VTDGen();
                if (vg.parseFile("./soap2.xml", true))
                {
                    VTDNav vn = vg.getNav();
                    // get to the SOAP header
                    if (vn.toElementNS(VTDNav.FC, "http://www.w3.org/2003/05/soap-envelope", "Header"))
                    {
                        if (vn.toElement(VTDNav.FC))
                        // to first child
                        {
                            do
                            {
                                // test MUSTHAVE
                                if (vn.hasAttrNS("http://www.w3.org/2003/05/soap-envelope", "mustUnderstand"))
                                {
                                    long l = vn.getElementFragment();
                                    int len = (int)(l >> 32);
                                    int offset = (int)l;
                                    byte[] b = vn.getXML().getBytes();
                                    fos.Write(b, offset, len); //write the fragment out into out.txt
                                    System.Text.Encoding encoder = System.Text.Encoding.GetEncoding("ASCII");
                                    byte[] bytes = encoder.GetBytes("\n=========\n");

                                    fos.Write(bytes, 0, bytes.Length);
                                }
                            }
                            while (vn.toElement(VTDNav.NS)); // navigate next sibling	 
                        }
                        else
                            System.Console.Out.WriteLine("Header has not child elements");
                    }
                    else
                        System.Console.Out.WriteLine(" Dosesn't have a header");
                    
                    fos.Close();
                }
            }
            catch (NavException e)
            {
                System.Console.Out.WriteLine(" Exception during navigation " + e);
            }
            catch (System.IO.IOException e)
            {
                System.Console.Out.WriteLine(" IO exception condition" + e);
            }

        }
    }
}
