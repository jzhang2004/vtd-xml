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

/* This is the XPath version of SOAPProcessor.cs
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

namespace SOAPProcessor2
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
                    AutoPilot ap = new AutoPilot();
                    ap.bind(vn);
                    ap.declareXPathNameSpace("ns1", "http://www.w3.org/2003/05/soap-envelope");
                    // get to the SOAP header
                    ap.selectXPath("/ns1:Envelope/ns1:Header/*[@ns1:mustUnderstand]");
                    Console.WriteLine("expr string is " + ap.getExprString());
                    while (ap.evalXPath() != -1)
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
