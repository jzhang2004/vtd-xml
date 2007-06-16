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

/* This is the XPath version of SOAPProcessor.java
 * It will pull out header element fragements (in its serialized format) 
 * with attributes MUSTUNDERSTAND
 * The method is "getElementFragment()"
 * It is important because sometimes one must encrypt or sign/verfiy the 
 * digital signature of an element in its serial format! 
 */
import com.ximpleware.*;
import com.ximpleware.xpath.*;
import java.io.*;

public class SOAPProcessor2 {

  public static void main(String argv[]){
     try {
	// open a file and read the content into a byte array
	// Modify this part of the code if the message is transmitted via HTTP
        File f = new File("./soap2.xml");
	FileInputStream fis =  new FileInputStream(f);
        byte[] b = new byte[(int) f.length()];
	fis.read(b);

	// open file to output extracted fragments
	File f1 = new File("./out.txt");
	FileOutputStream fos = new FileOutputStream(f1);

	AutoPilot ap = new AutoPilot();
	// instantiate the parser
	VTDGen vg = new VTDGen();
	vg.setDoc(b);
	vg.parse(true);  // set namespace awareness to true 
	VTDNav vn = vg.getNav();
	ap.bind(vn);
	ap.declareXPathNameSpace("ns1","http://www.w3.org/2003/05/soap-envelope");
	// get to the SOAP header
	ap.selectXPath("/ns1:Envelope/ns1:Header/*[@ns1:mustUnderstand]");
	System.out.println("expr string is " + ap.getExprString());
	while(ap.evalXPath()!= -1){
		long l = vn.getElementFragment();
		int len = (int) (l>>32);
		int offset = (int) l;
		fos.write(b, offset, len); //write the fragment out into out.txt
		fos.write("\n=========\n".getBytes());
	}

	fis.close();
	fos.close();

     }
     catch (ParseException e){
	     System.out.println(" XML file parsing error \n"+e);
     }
     catch (NavException e){
	     System.out.println(" Exception during navigation "+e);
     }
     catch (XPathParseException e){

     }
     catch (XPathEvalException e){
	     
     }
     catch (java.io.IOException e)
     {
	     System.out.println(" IO exception condition"+e);
     }
  }
}
