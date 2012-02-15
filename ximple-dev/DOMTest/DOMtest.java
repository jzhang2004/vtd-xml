package DOMTest;

import java.io.*;
import org.w3c.dom.*;
import org.w3c.*;
import javax.xml.parsers.*;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.FactoryConfigurationError;
import javax.xml.parsers.ParserConfigurationException;
import org.w3c.dom.Document;
import org.xml.sax.SAXException;
import com.ximpleware.*;
//import org.apache.xerces.*;


class DOMtest {
	public static void main(String[] argv) {
		String fileName = "c://xml//cdata.xml";//"d://ximple-dev//testcases//VTDGen//bad//nt_0_4.xml";
		//System.out.println("filename ==>"+argv[0]);
		try {
			//File f = new File("d://ximple-dev//testcases//xml_gen//test_utf16le.xml");
			//byte[] ba1 = new byte[(int)f.length()];
			//FileInputStream fis = new FileInputStream(f);
			//fis.read(ba1);
			//System.out.println(" first byte ==> "+ Integer.toHexString(ba1[0]));
			//System.out.println(" second byte ==> "+ Integer.toHexString(ba1[1]));
			//System.out.println(" xml content :"+ new String(ba1,"UnicodeLittle"));
			
			System.setProperty("javax.xml.parsers.SAXParserFactory",
			"org.apache.xerces.jaxp.SAXParserFactoryImpl");
			System.setProperty("javax.xml.parsers.DOMParserFactory",
			"org.apache.xerces.jaxp.DOMParserFactoryImpl");
			//System.out.println("hello world");
			/*String s =
				"<?xml version='1.0' encoding=\"\"?>"+ //"ass\"?>"+
				"<this><a xmlns=' abc ' xmlns:xmlnsls=\"a\" a=' &#xA; fsfs  '"
				+" xmlns:a='1'> &#xA; </a></this>"; */
			//byte[] ba = s.getBytes("UnicodeLittle");
			//System.out.println("first byte "+ba[0]);
			//System.out.println("second byte "+ba[1]);
			//ByteArrayInputStream bais = new ByteArrayInputStream(ba);
			DocumentBuilderFactory factory =
				DocumentBuilderFactory.newInstance();
			factory.setNamespaceAware(true);
			factory.setExpandEntityReferences(false);
			DocumentBuilder parser = factory.newDocumentBuilder();
			//Document d = parser.parse(bais);
			Document d = null;
			File f = new File(fileName);
			FileInputStream fis = new FileInputStream(f);
			//d = parser.parse("d://ximple-dev//testcases//xml_gen//test_ascii.xml");
			//System.out.println("ascii ok");
			//d = parser.parse("d://ximple-dev//testcases//xml_gen//test_8859.xml");
			//System.out.println("8859-1 ok");
			//d = parser.parse("d://ximple-dev//testcases//xml_gen//test_utf8.xml");
			//System.out.println("utf-8 ok");
			//d = parser.parse("d://ximple-dev//testcases//xml_gen//test_utf16le.xml");
			//System.out.println("utf-16le ok");
			//d = parser.parse("d://ximple-dev//testcases//xml_gen//test_utf16be.xml");
			d = parser.parse(fis);
			System.out.println(d.getNodeName());
			System.out.println("utf-16be ok"); 
			/*Node n = d.getDocumentElement();

			n = n.getFirstChild();
			Element e1 = (Element) n;
			System.out.println(e1);
			n.normalize();
			n = n.getFirstChild();
			//System.out.println		//.getFirstChild();
			//n.normalize();
			System.out.println(n);
			Attr a = e1.getAttributeNode("a");
			System.out.println(a.getValue()); */
			
			int it = 0xffffff;
			//System.out.println("value --->"+ it);
			//System.out.println("value --->"+ (it - ((it>>8)<<8)));
			
		} catch (Exception e) {
			//e.printStackTrace();
			System.out.println("exception ===> " + e);
		}
		
		// test vtdGen
		
		try{
			VTDGen vg = new VTDGen();
			File f = new File(fileName);
			byte[] ba = new byte[(int)f.length()];
			FileInputStream fis = new FileInputStream(f);
			fis.read(ba);
//			for (int i=0;i<ba.length;i++){
//				if (ba[i]<0)
//				System.out.println("i ==>"+i+ " "+ Integer.toHexString(0xff & ba[i]));
//			}
			//System.out.println("208==>"+Integer.toHexString(ba[208]&0xff));
			//System.out.println("209==>"+Integer.toHexString(ba[209]&0xff));
			vg.setDoc(ba);
			vg.parse(true);
		    System.out.println("vtdgen ok");
		}catch (ParseException e){
			System.out.println("ParserException: "+e);
			//e.printStackTrace();
		}catch (Exception e){
			//e.printStackTrace();
			System.out.println("other exceptions");
		}
		
		// test vtdGen1
		try{
			VTDGen vg = new VTDGen();
			File f = new File(fileName);
			byte[] ba = new byte[(int)f.length()];
			FileInputStream fis = new FileInputStream(f);
			fis.read(ba);
			vg.setDoc(ba);
			vg.parse(true);
			System.out.println("vtdgen1 ok");
		
		}catch (ParseException e){
			System.out.println("ParserException: "+e);
			//e.printStackTrace();
		}catch (Exception e){
			//e.printStackTrace();
			System.out.println("other exceptions"+e);
		}
	}
}
