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
/* This example demonstrates how to use the buffer reuse version 
 * of VTD-XML. 
 * When your applications need to process many XML documents in 
 * succession. The basic idea is that because VTD records are constant
 * in length, VTD containers (which are long arrays) can be reused
 * instead of allocate anew every time.
 * The key is to replace setDoc() with setDoc_BR().  
 * Also this example show how to reuse XPath expression for the same
 * VTDNav object, or different VTDNav objects by using ap.resetXPath()
 * and ap.bind(vn).
 */
using System;
using System.Collections.Generic;
using System.Text;
using com.ximpleware;
using com.ximpleware.xpath;

namespace BufferReuse
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                int t;
                System.IO.FileInfo f = new System.IO.FileInfo("./soap2.xml");
                System.IO.FileStream fis = 
                    new System.IO.FileStream(f.FullName, System.IO.FileMode.Open, System.IO.FileAccess.Read);
                System.IO.FileInfo f1 = new System.IO.FileInfo("./out.xml");
                System.IO.FileStream fos = new System.IO.FileStream(f1.FullName, System.IO.FileMode.Create);
                byte[] b = new byte[(int)f.Length];
                fis.Read(b, 0, (int)f.Length);
                AutoPilot ap = new AutoPilot();
                ap.declareXPathNameSpace("ns1", "http://www.w3.org/2003/05/soap-envelope");
                // get to the SOAP header
                ap.selectXPath("/ns1:Envelope/ns1:Header/*[@ns1:mustUnderstand]");
                Console.WriteLine("expr string is " + ap.getExprString());
                // instantiate the parser
                VTDGen vg = new VTDGen();
                int j = 0;
                VTDNav vn = null;
                while (j < 10)
                {
                    vg.setDoc_BR(b); // use setDoc_BR (instead of setDoc) to turn on buffer reuse
                    vg.parse(true);  // set namespace awareness to true 
                    vn = vg.getNav();
                    ap.bind(vn); // bind calls resetXPath() so
                    
                    while ((t = ap.evalXPath()) != -1)
                    {

                        Console.WriteLine("j t--> " + j + " " + t);
                        long l = vn.getElementFragment();
                        int len = (int)(l >> 32);
                        int offset = (int)l;
                        fos.Write(b, offset, len); //write the fragment out into out.txt
                        System.Text.Encoding encoder = System.Text.Encoding.GetEncoding("ASCII");
                        byte[] bytes = encoder.GetBytes("\n=========\n");

                        fos.Write(bytes, 0, bytes.Length);
                    }
                    ap.resetXPath();
                    j++;
                }
                j = 0;
                Console.WriteLine("j -->" + j);
                vg.setDoc_BR(b); // use setDoc_BR (instead of setDoc) to turn on buffer reuse
                vg.parse(true);  // set namespace awareness to true 
                vn = vg.getNav();
                ap.bind(vn); // bind calls resetXPath() so
                t =-1;
                while (j < 10)
                {
                    while ((t = ap.evalXPath()) != -1)
                    {
                        Console.WriteLine("j t --> " + j + " " + t);
                        long l = vn.getElementFragment();
                        int len = (int)(l >> 32);
                        int offset = (int)l;
                        fos.Write(b, offset, len); //write the fragment out into out.txt
                        System.Text.Encoding encoder = System.Text.Encoding.GetEncoding("ASCII");
                        byte[] bytes = encoder.GetBytes("\n=========\n");

                        fos.Write(bytes, 0, bytes.Length);
                    }
                    ap.resetXPath();
                    j++;
                }

                fis.Close();
                fos.Close();
            }
            catch (ParseException e)
            {
            }
            catch (NavException e)
            {
            }
            catch (XPathParseException e)
            {
            }
            catch (XPathEvalException e)
            {
            }
            catch (System.IO.IOException e)
            {

            }
        }
    }
}

