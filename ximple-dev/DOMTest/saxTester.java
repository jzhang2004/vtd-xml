package DOMTest;

import java.io.*;
import javax.xml.*;
import javax.xml.parsers.*;
import org.xml.sax.*;
import org.xml.sax.helpers.DefaultHandler;
import javax.xml.parsers.SAXParserFactory;
import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import org.w3c.dom.Document;
import javax.xml.transform.*;
import javax.xml.xpath.*;
import javax.xml.transform.sax.SAXSource;
import javax.xml.transform.stream.*;
import javax.xml.transform.dom.*;

public class saxTester {

	public static void main1(String s[]) throws Exception{
		//String input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?> <!DOCTYPE sgml [ <!ELEMENT sgml ANY> <!ENTITY q \"Sample\"> ]> <sgml>&q;</sgml>";
		DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
		TransformerFactory tfactory = TransformerFactory.newInstance();
        Transformer tf = tfactory.newTransformer(); 
		dbf.setExpandEntityReferences(true);
		DocumentBuilder db = dbf.newDocumentBuilder();
		FileOutputStream baos = new FileOutputStream("c:/xml/entity_out.xml");
		//parse using builder to get DOM representation of the XML file
		Document dom = db.parse("c:/xml/entity.xml");
		tf.transform(new DOMSource(dom), new StreamResult(baos));
		
		SAXParserFactory spf  = SAXParserFactory.newInstance();
		SAXParser sp = spf.newSAXParser();
		
		TransformerFactory tfactory2 = TransformerFactory.newInstance();
        Transformer tf2 = tfactory2.newTransformer(); 
        
        FileOutputStream baos2 = new FileOutputStream("c:/xml/entity_out2.xml");
        
        tf.transform(new SAXSource(new InputSource(new FileInputStream("c:/xml/entity.xml"))), new StreamResult(baos2));
        
        
        //sp.
		//sp.parse("c:/xml/entity.xml");
	
	}
	
	public static void main(String s[]) throws Exception{
		//String input = "<?xml version=\"1.0\" encoding=\"UTF-8\"?> <!DOCTYPE sgml [ <!ELEMENT sgml ANY> <!ENTITY q \"Sample\"> ]> <sgml>&q;</sgml>";
		DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
		TransformerFactory tfactory = TransformerFactory.newInstance();
        Transformer tf = tfactory.newTransformer(); 
		dbf.setExpandEntityReferences(true);
		DocumentBuilder db = dbf.newDocumentBuilder();
		//FileOutputStream baos = new FileOutputStream("c:/xml/t1.xml");
		//parse using builder to get DOM representation of the XML file
		Document dom = db.parse("c:/xml/t1.xml");
		/*tf.transform(new DOMSource(dom), new StreamResult(baos));
		
		SAXParserFactory spf  = SAXParserFactory.newInstance();
		SAXParser sp = spf.newSAXParser();
		
		TransformerFactory tfactory2 = TransformerFactory.newInstance();
        Transformer tf2 = tfactory2.newTransformer(); 
        
        FileOutputStream baos2 = new FileOutputStream("c:/xml/t1.xml");
        
        tf.transform(new SAXSource(new InputSource(new FileInputStream("c:/xml/t1.xml"))), new StreamResult(baos2));*/
        
        
        //sp.
		//sp.parse("c:/xml/entity.xml");
	
	}
	
}
