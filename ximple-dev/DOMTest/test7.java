package DOMTest;
import com.ximpleware.*;
import com.ximpleware.parser.*;
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

public class test7 {
	static Runtime rt;

	public static void main(String[] args) {

		//File f = new File(args[0]);
		File f = new File("c:/xml/test11.xml");

		try {

			FileInputStream fis = new FileInputStream(f);
			rt = Runtime.getRuntime();
			byte[] ba = new byte[(int) f.length()];

			fis.read(ba);
			// vg.setDoc(ba);
			long startMem = rt.totalMemory() - rt.freeMemory();
			DocumentBuilderFactory factory = DocumentBuilderFactory
					.newInstance();
			factory.setFeature(
					"http://apache.org/xml/features/dom/defer-node-expansion",
					false);
			factory.setNamespaceAware(true);
			factory.setExpandEntityReferences(false);

			DocumentBuilder parser = factory.newDocumentBuilder();
			ByteArrayInputStream bais = new ByteArrayInputStream(ba);
			Document d = parser.parse(bais);
			Node n = d.getElementsByTagNameNS(null,"abc").item(0);
			System.out.println(n);
			NamedNodeMap n1 = n.getAttributes();
			Node n2 = n1.getNamedItemNS(null,"abc");
			//Node n1 = n.
			System.out.println(n2);
			//Node n1 = n.
			
			long endMem = rt.totalMemory() - rt.freeMemory();
			System.out.println("Memory Use: " + ((float) endMem - startMem)
					/ (1 << 20) + " MB.");
			System.out.println("Multiplying factor: "
					+ ((float) endMem - startMem) / f.length());
		} catch (Exception e) {
			System.out.println("exception ==> " + e);
		}
	}
}
