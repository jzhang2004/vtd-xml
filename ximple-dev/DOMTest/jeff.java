package DOMTest;

import com.ximpleware.*;
import com.ximpleware.VTDGen;

public class jeff {
	/** 
	 * * Verify the existence of the data that must be present. Done pre-sending so * we make sure we have good data. 
	 * * * @param xml - The fax data xml that is used by the fax server. 
	 * * @param errMsg - A String array that contains one string object that can be modified with the error 
	 * * message if there was an error. */ 
	public static void main(String argv[]) 
	{ 
		//int response = HttpServletResponse.SC_OK; 
		VTDGen vg = new VTDGen(); 
		if (vg.parseFile("c:\\xml\\testfax.xml", true)){
			System.out.println("good");
		}			
	}
}
