package DOMTest;

import java.io.FileOutputStream;

import com.ximpleware.*; 
public class christoph {
	public static void main1(String[] args) throws Exception {		
		VTDGen vg = new VTDGen();		
		if (vg.parseHttpUrl("c:\\xml\\input.xml", true)) {
			VTDNav vn = vg.getNav();
			AutoPilot ap = new AutoPilot();
			ap.selectXPath("/records/record");
			ap.bind(vn);
			int i=-1,j=0;
			while ((i = ap.evalXPath()) != -1) {
				long l=vn.getElementFragment();
				(new FileOutputStream("out"+j+".xml")).write(vn.getXML().getBytes(), (int)l,(int)(l>>32));
				j++;
			}
		}
	}
	public static void main(String[] args) throws Exception {	
		TestCaseUTF8CharPosition();
	}
	
	private final static void TestCaseUTF8CharPosition() throws Exception
	{
	String xmlDocument = "";
	xmlDocument += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>";
	xmlDocument += "<root>";
	xmlDocument += "<token>token_1</token>";
	xmlDocument += "<token>token_2</token>";
	xmlDocument += "<token>token_WithUnicodeChar_é</token>";
	xmlDocument += "<token>token_3</token>";
	xmlDocument += "<token>token_4</token>";
	xmlDocument += "</root>";

	VTDGen vg = new VTDGen();
	vg.setDoc(xmlDocument.getBytes("UTF-8"));
	vg.parse(true);

	VTDNav vn = vg.getNav();
	AutoPilot ap = new AutoPilot(vn);
	ap.selectElementNS("","token");

	while(ap.iterate()) {
	int currentIndex = vn.getCurrentIndex();
	int startTokenCharOffset = vn.getTokenOffset(currentIndex);
	System.out.println("\n ===> " + vn.toString(vn.getCurrentIndex()));

	String extractFromCharPosition =
	xmlDocument.substring(startTokenCharOffset, startTokenCharOffset +
	"token".length());
	System.out.print("\n" + extractFromCharPosition);
	if(!"token".equals(extractFromCharPosition)) {
	System.out.print( " <- error ");
	}
	}
	}

}
