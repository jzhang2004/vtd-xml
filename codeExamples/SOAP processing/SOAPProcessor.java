/* 
 * Copyright (C) 2002-2004 XimpleWare, info@ximpleware.com
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

/* In this java program, we demonstrate content-inspection aspect of our
 * API by processing some parts of the SOAP header 
 * It will pull out header element fragements (in its serialized format) 
 * with attributes MUSTUNDERSTAND
 * The method is "getElementFragment()"
 * It is important because sometimes one must encrypt or sign/verfiy the 
 * digital signature of an element in its serial format! 
 */
import com.ximpleware.*;
import java.io.*;

public class SOAPProcessor {

  public static void main(String argv[]){
     try {
	// open file to output extracted fragments
	File f1 = new File("./out.txt");
	FileOutputStream fos = new FileOutputStream(f1);
	
	// instantiate the parser
	VTDGen vg = new VTDGen();
	if (vg.parseFile("./soap2.xml",true)){

	VTDNav vn = vg.getNav();
	// get to the SOAP header
	if (vn.toElementNS(VTDNav.FC,"http://www.w3.org/2003/05/soap-envelope","Header"))
	{
	      if (vn.toElement(VTDNav.FC)) // to first child
	      {
		 do {
		    // test MUSTHAVE
		    if (vn.hasAttrNS("http://www.w3.org/2003/05/soap-envelope","mustUnderstand")){
			long l = vn.getElementFragment();
			int len = (int) (l>>32);
			int offset = (int) l;
			byte[] b = vn.getXML().getBytes();
			fos.write(b, offset, len); //write the fragment out into out.txt
			 System.Text.Encoding encoder = System.Text.Encoding.GetEncoding("ASCII");
                                    byte[] bytes = encoder.GetBytes("\n=========\n");

                                    fos.Write(bytes, 0, bytes.Length);
			fos.write("\n=========\n".getBytes());
		    }
		 }
	         while (vn.toElement(VTDNav.NS)); // navigate next sibling	 
	      }
	      else
		 System.out.println("Header has not child elements");
	}
	else
	   System.out.println(" Dosesn't have a header");
//	fis.close();
	fos.close();
	}

     }
     catch (NavException e){
	     System.out.println(" Exception during navigation "+e);
     }
     catch (java.io.IOException e)
     {
	     System.out.println(" IO exception condition"+e);
     }
  }
}
