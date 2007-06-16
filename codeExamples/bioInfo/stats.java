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

/* 
 * In this java program, we demonstrate how to process a complex
 * xml file describing DNA information
 * We are going to perform navigation equivalent to the xpath expression
 * "/bix/package/command/parlist"
 * then count the total number of child elements, and number of 
 * child elements with the name "par"
 * Since version 1.6, VTDGen added a method called parseFile(), 
 * this example also shows how to use that as well in here.
 */
import com.ximpleware.*;

public class stats {

  public static void main(String argv[]){
     try {
       
        // counting child elements of parlist
	int count = 0;
        // counting child elements of parlist named "par"
	int par_count = 0;

	VTDGen vg = new VTDGen();
	if (vg.parseFile("./bioinfo.xml",true)){
	
	VTDNav vn = vg.getNav();
        if (vn.matchElement("bix")){ // match blix
     	  // to first child named "package"
          if (vn.toElement(VTDNav.FC,"package")){ 
	    do {
		    System.out.println("package");
		    // to first child named "command"
	      if (vn.toElement(VTDNav.FC,"command"))
	      {
	 	 do {
		 	System.out.println("command");
 			if (vn.toElement(VTDNav.FC, "parlist")){
	   		   do {
				System.out.println("parlist");
				count++; //increment count
				if (vn.toElement(VTDNav.FC)){
			        do {	
				      if (vn.matchElement("par"))
					 par_count++;
			   	}
				while(vn.toElement(VTDNav.NS));
				vn.toElement(VTDNav.P);
			     }
			}
			while (vn.toElement(VTDNav.NS,"parlist"));
			vn.toElement(VTDNav.P);
		    }						
	     }
	     // to next silbing named "command"
	     while (vn.toElement(VTDNav.NS, "command")); 
	     vn.toElement(VTDNav.P); // go up one level
	    }
	    else 
	      System.out.println(" no child element named 'command' ");
	    // verify result

	  }
	  while(vn.toElement(VTDNav.NS,"package")); // to next sibling named "package"
	  vn.toElement(VTDNav.P); // go up one level	
	 }else
	    System.out.println(" no child element named 'package' ");	
	} else 
	  System.out.println(" Root is not 'bix' ");
	// print out the results
	System.out.println(" count ====> " + count);
	System.out.println(" par_count ==> " + par_count);

	// verify results using iterators
        int v=0;
        vn.toElement(VTDNav.ROOT);
        AutoPilot ap = new AutoPilot(vn);
        ap.selectElement("par");
        while(ap.iterate()){
    	   if (vn.getCurrentDepth() == 4) 
	   {
		v++;
	   }
	}
	System.out.println(" verify ==> "+v);
        }
     }
     catch (NavException e){
	     System.out.println(" Exception during navigation "+e);
     }
 
  }
}
