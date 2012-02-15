package DOMTest;
import javax.xml.parsers.*;
import javax.xml.xpath.XPathConstants;
import javax.xml.xpath.XPathExpression;
import javax.xml.xpath.XPathFactory;
//import javax.xml.transform.*;
//import javax.xml.xpath.*;
//import javax.xml.transform.stream.*;
//import javax.xml.transform.dom.*;
import java.io.*;
import org.w3c.dom.*;
import org.xml.sax.InputSource;
//import javax.xml.namespace.NamespaceContext;
import org.jaxen.*;
import org.jaxen.dom.*;
import java.util.*;
import com.ximpleware.*;

public class jaxenTest {
  public static void main(String args[]){
      try{
			int total = 15000, i1 = 0, k = 0;
			System.setProperty("javax.xml.parsers.SAXParserFactory",
					"org.apache.xerces.jaxp.SAXParserFactoryImpl");
			System.setProperty("javax.xml.parsers.DOMParserFactory",
					"org.apache.xerces.jaxp.DOMParserFactoryImpl");
			// create xpath object
			

			// XPathFactory factory1=XPathFactory.newInstance();
			// XPath xPath=factory1.newXPath();
			// TransformerFactory tfactory = TransformerFactory.newInstance();
			// Transformer tf = tfactory.newTransformer();
			// read in file content and create byte array input stream
			File f = new File("c:/xml/test2.xml");
			byte b[] = new byte[(int) f.length()];
			FileInputStream fis = new FileInputStream(f);
			fis.read(b);
			ByteArrayInputStream bais = new ByteArrayInputStream(b);

			DocumentBuilderFactory factory = DocumentBuilderFactory
					.newInstance();
			factory.setFeature(
					"http://apache.org/xml/features/dom/defer-node-expansion",
					false);
			factory.setNamespaceAware(false);
			factory.setExpandEntityReferences(false);
			DocumentBuilder parser = factory.newDocumentBuilder();
			String xpe = "/*/*/*/*/precedinging::node()";
			xpe = "(/root/attack[2]/preceding::node())";
			XPath expression = new org.jaxen.dom.DOMXPath(xpe);
			System.out.println(expression);
			
			System.out.println("====> update_jaxen1 ==>" + xpe);
			// parse and xpath eval

			Document d = null;
			d = parser.parse(bais);
			
			List results = expression.selectNodes(d);
			int sz = results.size();
			System.out.println(" node set size "+sz);
			for (int i=0;i<sz;i++){
				Node n = (Node)results.get(i);
				System.out.println(n);
			}
			System.out.print("VTD ==> ");
			int k1=0;
			VTDGen vg = new VTDGen();
			vg.enableIgnoredWhiteSpace(true);
			if (vg.parseFile("c:/xml/test2.xml",false)){
				VTDNav vn = vg.getNav();
				vn.toElement(VTDNav.PARENT);
				AutoPilot ap = new AutoPilot(vn);
				/*ap.selectFollowingNode();
				while(ap.iterateFollowingNode()){
					k1++;
					System.out.println(vn.getCurrentIndex());
				}*/
				ap.selectXPath(xpe);
				while(ap.evalXPath()!=-1){
					System.out.println(" name ==> " + vn.toString(vn.getCurrentIndex()) );
					k1++;
				}
				System.out.println(" k1 ==> "+k1);
			}

			System.out.print("DOM ==> ");
			XPathFactory  factory1=XPathFactory.newInstance();
            javax.xml.xpath.XPath xPath=factory1.newXPath();
            XPathExpression xPathExpression=
                xPath.compile(xpe);
            NodeList nodeList = (NodeList) xPathExpression.evaluate(d,
                    XPathConstants.NODESET);
            System.out.println("# of nodes ==>" + nodeList.getLength());
            for (int i=0;i<nodeList.getLength();i++){
            	System.out.println(" "+nodeList.item(i) );
            }
			// System.out.println(baos.toString());
			// transform into byteArray output stream

		} catch (IOException e) {

		} catch (Exception e) {

		}
  }
}