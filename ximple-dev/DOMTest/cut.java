package DOMTest;
import java.io.FileOutputStream;

import com.ximpleware.*;
import com.ximpleware.TextIter;
import com.ximpleware.extended.*;

public class cut {
	public static void main(String s[]) throws Exception{
		
		VTDGen vg = new VTDGen();
		if (vg.parseFile("c:/xml/LZZTTrial_Jozef_small.xml", true)){
			VTDNav vn = vg.getNav();
			vn.toElement(VTDNav.P);
			//vn.setCondition1();
			vn.recoverNode(42);
			System.out.println("good");
		}
	}
	
	public static void main2(String s[]) throws Exception{
		String xmlString= "<?xml version='1.0' encoding='UTF-8'?><a><b><c>data</c></b></a>";
		byte[] ba = xmlString.getBytes();
		VTDGen vg = new VTDGen();
		vg.setDoc(ba);
		vg.parse(true);
		VTDNav vn = vg.getNav();
		vn.toElement(VTDNav.FC);
		vn.toElement(VTDNav.FC);
		ElementFragmentNs efs = vn.getElementFragmentNs();
		vn.toElement(VTDNav.P);
		
		XMLModifier xm = new XMLModifier(vn);
		xm.remove();
		xm.insertBeforeElement(efs);
		System.out.println(" size ==> "+xm.getUpdatedDocumentSize());
		XMLByteOutputStream xbos = new XMLByteOutputStream(xm.getUpdatedDocumentSize());
		xm.output(xbos);
		System.out.println(new String(xbos.getXML()));
	}
	
	
	public static void main1(String s[]) throws Exception{
		String xmlString= "<?xml version='1.0' encoding='UTF-16LE'?><a><b><c>data</c></b></a>";
		String xmlString2= "<?xml version='1.0' encoding='UTF-8'?><a><b><c>data</c></b></a>";
		byte[] ba = xmlString2.getBytes("UTF-8");
		byte[] ba2 = xmlString.getBytes("UTF-16LE");
		FileOutputStream fos = new FileOutputStream("c:/xml/x11.xml");
		fos.write(ba);
		FileOutputStream fos2 = new FileOutputStream("c:/xml/x22.xml");
		fos2.write(ba2);
		VTDGen vg = new VTDGen();
		vg.setDoc(ba);
		vg.parse(true);
		VTDNav vn = vg.getNav();
		vg.setDoc(ba);
		vg.parse(true);
		VTDNav vn2 = vg.getNav();
		ElementFragmentNs efs = vn2.getElementFragmentNs();
		XMLModifier xm = new XMLModifier(vn);
		//xm.insertAfterHead(VTDNav.FORMAT_UTF8,"goode".getBytes("UTF-8"),0,4);
		xm.insertAfterHead(efs);
		xm.insertAttribute(" a='1'");
		int i=xm.getUpdatedDocumentSize();
		System.out.println("Output document size "+i);
		XMLByteOutputStream xbos = new XMLByteOutputStream(xm.getUpdatedDocumentSize());
		xm.output(xbos);
		System.out.println(new String(xbos.getXML()));	
		AutoPilot ap = new AutoPilot(vn);
		ap.declareVariableExpr("var1","//a");
		ap.declareVariableExpr("var2","//b");
		ap.declareVariableExpr("var3", "//c");
		ap.selectXPath("$var1 | $var2 | $var3");
		System.out.println(ap.getExprString());
		int k=-1;
		while((k=ap.evalXPath())!=-1){
			System.out.println("index ==> "+k+ "   "+vn.toString(k));
		}
	}
}
