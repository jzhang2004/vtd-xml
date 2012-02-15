package DOMTest;

import com.ximpleware.*;
//import com.ximpleware.BookMark;
//import com.ximpleware.FastIntBuffer;
import com.ximpleware.VTDGen;
import com.ximpleware.VTDNav;

public class test6 {

	/**
	 * @param args
	 */
	public static int add(int i, int j){
		i=i+1;
		return i+j;
	}
	public static void main(String[] args) throws Exception{
		// TODO Auto-generated method stub
		int t=0;
		//System.out.println(" output " + add(t,t));
		//System.out.println(" t ==> "+t);
		
		VTDGen vg = new VTDGen();
		//String s = "<a xmlns:abc='abc' xmlns:b='abc' xmlns=''> <b xmlns='cde'> <b:b xmlns:b='abc'/> </b> <b:b/> </a>";
		//String s="<?xml version='1.0' encoding='UTF-16LE'?><xml:a xmlns='' xmlns:xmlns='abc' xml:s1=' abc '/>";

		//vg.setDoc(s.getBytes("UTF-16LE"));
		//String s1="<?xml version='1.0' encoding='UTF-16LE'?>";
		//String s="<?xml version='1.0' encoding='UTF-16LE'?><xml1:a xmlns:xml1=' ' xmlns:xml2=' ' xmlns:xml3=' '"+
		String s="<xml1:a xmlns='' xmlns:xml1=' &lt;1nnnn' xmlns:xml2=' ' xmlns:xml3=' &lt;nnnn'"+
		" abc='' xml1:a='' b='' xml3:c='' d='' e=' ' xml2:f=' ' xml3:a=' '><xml1:abc xmlns=\" http://www.w3.org/XML/1998/namespace\"/>"
		+"<a><b/> </a><abc xmlns='abc\r\r\n' xmlns:abc='nnnn' abc:abc='nnnn' xml1:abc='nnnn'/>"
		+"</xml1:a>";
		VTDNav vn;
		vg.setDoc(s.getBytes());
		//vg.setDoc(s.getBytes("UTF-16LE"));
		vg.parse(true);
		
		vn = vg.getNav();
		if (vn.matchElementNS("http://www.w3.org/XML/1998/namespace","a"))
			System.out.println("good 1");
		if(vn.getAttrValNS("http://www.w3.org/XML/1998/namespace", "lang")!=-1)
			System.out.println("good 2");
		if(vn.getAttrValNS("", "abc")!=-1)
			System.out.println("good 3");
		if(vn.getAttrValNS(null, "abc")!=-1)
			System.out.println("good 4");
		//System.out.println("--"+vn.toString(vn.getAttrValNS("http://www.w3.org/XML/1998/namespace", "lang"))+"--");
		vn.toElement(VTDNav.FC);
		if (vn.matchElementNS("abc \n","abc"))
			System.out.println("good 5");
		for (int i=0;i<vn.getTokenCount();i++){
			String s1= vn.getPrefixString(i);
			if (s1!=null)
				System.out.println(i+"==>"+s1);
		}
		//System.out.println("a success ===>"+vn.toNormalizedString2(5)+"----");
		//System.out.println(" ");
		
		s = "<a><b/><b/><b/></a>";
		vg.setDoc(s.getBytes());
		vg.parse(false);
		vn = vg.getNav();
		vn.toElement(VTDNav.FIRST_CHILD);
		long l = vn.getSiblingElementFragments(2);
		int offset = (int)l;
		int length = (int)(l>>32);
		System.out.println("offset ==> "+offset+"  length==> "+length);
		XMLModifier xm= new XMLModifier(vn);
		xm.remove(l);
		//xm.output(new java.io.FileOutputStream("c:/xml/p.txt"));
		xm.outputAndReparse();
		//XMLByteOutputStream xbos = new XMLByteOutputStream(16);
		//byte[] ba = {1,1,1,1};
		//xbos.write(ba,0,5);
		
		s = "<pn category=\"ONE\" GENERAL0=\"0\" GENERAL1=\"-1\" >previoustext</pn>";
		vg.setDoc(s.getBytes());
		vg.parse(true);
		vn = vg.getNav();
		AutoPilot testAP = new AutoPilot(vn);
		testAP.selectXPath("/pn[@category]"); 
		xm.bind(vn);
		int j; 
		while((j = testAP.evalXPath()) != -1) 
		{ 
		     int p = vn.getText(); 
		     xm.updateToken(p, "new text"); 
		} 
		XMLByteOutputStream xbos =new XMLByteOutputStream(xm.getUpdatedDocumentSize());
		xm.output(xbos);
		System.out.println(new String(xbos.getXML()));

	}
	
}
