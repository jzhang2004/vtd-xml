package DOMTest;

import java.io.*;
import org.w3c.dom.*;
import org.w3c.*;
import javax.xml.parsers.*;
import org.w3c.dom.*;

class DOMtest {
	public static void main(String[] argv) {
		try {

			System.out.println("hello world");
			String s =
				"<this><a xmlns=' ab ' xmlns:xmlnsls='a' a=' &#xA; fsfs  '"
				+" xmlns:a='1'> &#xA; </a></this>";
			byte[] ba = s.getBytes("UTF-8");
			ByteArrayInputStream bais = new ByteArrayInputStream(ba);
			DocumentBuilderFactory factory =
				DocumentBuilderFactory.newInstance();
			factory.setNamespaceAware(true);
			factory.setExpandEntityReferences(false);
			DocumentBuilder parser = factory.newDocumentBuilder();
			Document d = parser.parse(bais);

			Node n = d.getDocumentElement();

			n = n.getFirstChild();
			Element e1 = (Element) n;
			System.out.println(e1);
			n.normalize();
			n = n.getFirstChild();
			//System.out.println		//.getFirstChild();
			//n.normalize();
			System.out.println(n);
			Attr a = e1.getAttributeNode("a");
			System.out.println(a.getValue());
			
			int it = 0xffffff;
			System.out.println("value --->"+ it);
			System.out.println("value --->"+ (it - ((it>>8)<<8)));
		} catch (Exception e) {
			System.out.println("exception ===> " + e);
		}
	}
}