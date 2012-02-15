package DOMTest;

import java.io.*;
import com.ximpleware.*;
import com.ximpleware.extended.*;
import com.ximpleware.VTDGen;
import com.ximpleware.VTDNav;
import com.ximpleware.XMLModifier;
import com.ximpleware.FastIntBuffer;

class test2 {
	public static void main(String[] arg) throws Exception {
//		VTDGen generator = new VTDGen();
//		generator
//				.setDoc("<tst:resp xmlns:tst='url'><tst:level1></tst:level1></tst:resp>"
//						.getBytes());
//		generator.parse(true);
//		VTDNav navigator = generator.getNav();
//		AutoPilot autoPilot = new AutoPilot(navigator);
//		autoPilot.declareXPathNameSpace("tst", "url");
//		autoPilot.selectXPath("//abc-efg");
//		System.out.println(" expr is ==>"+autoPilot.getExprString());
//		autoPilot.selectXPath("/tst:resp/tst:level1");
//		// workaround use text() in Xpath
//		// autoPilot.selectXPath("/tst:resp/tst:level1/text()");
//		XMLModifier modifier = new XMLModifier(navigator);
//
//		int i = -1;
//		while ((i = autoPilot.evalXPath()) != -1) {
//			modifier.insertAfterHead("<test>1</test>");
//			// workaround use updateToken with text() in Xpath
//			// modifier.updateToken(i, "&lt;test&gt;1&lt;/test&gt;");
//		}
//		ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
//		
//		modifier.output(outputStream);
//		VTDNav vn2=modifier.outputAndReparse();
//		vn2.toElement(VTDNav.R);
//		vn2.toElement(VTDNav.FC);
//		vn2.toElement(VTDNav.FC);
//		System.out.println(" vtd index  ===>" + vn2.getCurrentIndex());
//		VTDNav vn3 = vn2.cloneNav();
//		System.out.println(" vtd index  ===>" + vn3.getCurrentIndex());
//
//		System.out.println(outputStream.toString());
//		System.out.println(new String(vn2.getXML().getBytes()));
//		
//		FastIntBuffer fib = new FastIntBuffer(4);
//		for (i=0;i<25;i++){
//			fib.append(i);
//		}
//		for (i=0;i<25;i++){
//			System.out.println(" int at i =>"+fib.intAt(i));
//		}
//		System.out.println("================");
//		fib.sort(FastIntBuffer.DESCENDING);
//		for (i=0;i<25;i++){
//			System.out.println(" int at i =>"+fib.intAt(i));
//		}
//		System.out.println("================");
//		fib.sort(FastIntBuffer.ASCENDING);
//		for (i=0;i<25;i++){
//			System.out.println(" int at i =>"+fib.intAt(i));
//		}
//		
//		fib.resize(16);
//		for (i=0;i<9;i++)
//		   fib.append(1);
//		System.out.println("================");
//		//fib.sort(FastIntBuffer.ASCENDING);
//		for (i=0;i<25;i++){
//			System.out.println(" int at i =>"+fib.intAt(i));
//		}
		
		// test xpath
		try{
		VTDGen vg = new VTDGen();
		vg.enableIgnoredWhiteSpace(true);
		if (vg.parseFile("c:/xml/oldpo.xml",true)){
			VTDNav vn = vg.getNav();
			AutoPilot ap = new AutoPilot(vn);
			//ap.selectXPath("/fieldset/text()[last()]");
			//ap.selectXPath("/fieldset/text()[normalize-space(..)][1]");
			//ap.selectXPath("/fieldset/text()");
			//ap.selectXPath("/purchaseOrder/items/item[@partNum='872-AA']/USPrice[.>100]");
			//ap.selectXPath("/purchaseOrder/items/item");
			ap.selectXPath("/purchaseOrder/items/item/USPrice[.<40]/text()");
			int i;
			while((i=ap.evalXPath())!=-1){
				System.out.println(" index ==>"+i);
				System.out.println("  value =>"+vn.toNormalizedString(i));
			}
		}
		}catch(VTDException e){
			System.out.println(e);
		}
		
//		VTDGenHuge vgh = new VTDGenHuge();
//		if (vgh.parseFile("c:/xml/text1.xml",true,VTDGenHuge.MEM_MAPPED)){
//			VTDNavHuge vnh = vgh.getNav();
//			vnh.toElement(VTDNavHuge.FC);
//			long[] la = vnh.getElementFragment();
//			vnh.getXML().writeToFileOutputStream(new FileOutputStream("c:/xml/text2.xml"), la[0], la[1]);
//			
//		}
		
	}
}
