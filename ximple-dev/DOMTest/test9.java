package DOMTest;
import java.io.File;
import java.io.FileInputStream;
import java.io.*;

//import org.junit.After;
//import org.junit.AfterClass;
//import org.junit.Before;
//import org.junit.BeforeClass;

import com.ximpleware.AutoPilot;
import com.ximpleware.VTDGen;
import com.ximpleware.VTDNav;
import com.ximpleware.XMLByteOutputStream;
import com.ximpleware.XMLModifier;

public class test9 {
	String output;

	public String testVtd() {
		try {

			VTDNav vn1 = createVTDNav("C://xml/tv.xml");
			VTDNav vn2 = createVTDNav("C://xml/sr.xml");
			XMLModifier xm1 = new XMLModifier(vn1);
			XMLModifier xm2 = new XMLModifier(vn2);
			AutoPilot ap1 = new AutoPilot(vn1);
			AutoPilot apSocioAttributes = new AutoPilot(vn1);
			AutoPilot apRelatedTerms = new AutoPilot(vn2);
			ap1.declareXPathNameSpace("autn",
					"http://schemas.autonomy.com/aci/");
			ap1.selectXPath("/autnresponse/responsedata/autn:number_of_fields");
			apRelatedTerms.declareXPathNameSpace("autn",
					"http://schemas.autonomy.com/aci/");
			apRelatedTerms.selectXPath("/autnresponse/responsedata/autn:qs");
			apSocioAttributes
					.selectXPath("/autnresponse/responsedata/autn:field[autn:name='PRDS/PRD/MP/MBAS/MBA']/autn:value");
			/* 
			 * * Iterating through itemResults.xml and extracting the needed
			 * info
			 */
			if (vn2.toElement(VTDNav.ROOT, "autnresponse")) {
				if (vn2.toElement(VTDNav.NEXT_SIBLING, "responsedata")) {
					if (apRelatedTerms.evalXPath() != -1) {
						long efn = vn2.getElementFragment();
						while (ap1.evalXPath() != -1) {
							xm1.insertBeforeElement(vn2, efn);
						}
					}

				}// responsedata
			}// au if (vn1.toElement(VTDNav.ROOT, "autnresponse"))

			if (vn1.toElement(VTDNav.ROOT, "autnresponse")) {
				if (vn1.toElement(VTDNav.FIRST_CHILD, "response")) {
					if (vn1.toElement(VTDNav.NEXT_SIBLING, "responsedata")) {
						do {
							while (apSocioAttributes.evalXPath() != -1) {
								System.out.println("updating attr");
								xm1.insertAttribute(" plant='1' name='goal' ");
							}
						} while (vn1.toElement(VTDNav.NEXT_SIBLING,
								"autn:value"));
					}
				}
			}

			// print ouput xml
			output = getXMLString(xm1);
			System.out.println("output xml = " + output);

		} catch (Exception e) {
			e.printStackTrace();
		}
		return output;
	}

	private static String getXMLString(XMLModifier xm) {
		try {
			XMLByteOutputStream xbos = new XMLByteOutputStream(xm
					.getUpdatedDocumentSize());
			System.out.println(" doc size ==> "+ xm.getUpdatedDocumentSize());
			FileOutputStream fos = new FileOutputStream("c:/xml/out111.xml");
			xm.output(fos);
			xm.output(xbos);
			return xbos.toString();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}

	private static VTDNav createVTDNav(byte[] ba) {

		VTDGen vg = new VTDGen();
		VTDNav vn = null;
		try {
			vg.setDoc(ba);
			vg.parse(true);
			vn = vg.getNav();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return vn;
	}

	private static VTDNav createVTDNav(String filename) {

		File f = new File(filename);
		// counting child elements of parlist
		VTDNav vn = null;
		try {
			FileInputStream fis = new FileInputStream(f);
			byte[] b = new byte[(int) f.length()];
			fis.read(b);
			VTDGen vg = new VTDGen();
			vg.setDoc(b);
			vg.parse(true);
			vn = vg.getNav();
		} catch (Exception e) {
			e.printStackTrace();
		}

		return vn;
	}

	public static void main(String[] args) {
		test9 mytest = new test9();
		System.out.println(mytest.testVtd());

	}

}
