package DOMTest;
import com.ximpleware.extended.*;
import com.ximpleware.*;
import java.io.*;

import org.w3c.dom.*;
import org.w3c.*;
import javax.xml.parsers.*;

import org.w3c.dom.Document;
import org.xml.sax.SAXException;

public class extended {

	public static void main2(String args[])

	{
		long count = 0;
		if (args.length != 1)
			throw new IllegalArgumentException("Usage: Java "
					+ extended.class.getName() + " <XMLFileName>");

		long start = System.currentTimeMillis();
		VTDGenHuge vg = new VTDGenHuge();
		try {
			String xmlFile = args[0];

			// vg.parseFile(xmlFile, false, VTDNavHuge.MEM_MAPPED);

			vg.parseFile(xmlFile, false);

			VTDNavHuge vn = vg.getNav();

			if (vn.matchElement("ArticleSet")) {
				if (vn.toElement(VTDNavHuge.FC, "Article")) {

					do {

						if (vn.toElement(VTDNavHuge.FC, "Journal")) {

							do {
								if (vn.toElement(VTDNavHuge.FC, "Issn")) {

									do {
										count++;

										int s = vn.getText();
										String st = "";
										if (s != -1) {
											st = vn.toString(s);

										}
										System.out.println(count + " Issn: "
												+ st);
										st = null;

									} while (vn
											.toElement(VTDNavHuge.NS, "Issn"));
									vn.toElement(VTDNavHuge.P);

								}

							} while (vn.toElement(VTDNavHuge.NS, "Journal"));
							vn.toElement(VTDNavHuge.P);

						}

					} while (vn.toElement(VTDNavHuge.NS, "Article"));
					vn.toElement(VTDNavHuge.P);

				}

			}
			System.out.println(" count ====> " + count);

			long end = System.currentTimeMillis();
			System.out.println("files took" + (end - start) + "milliseconds");

		} catch (VTDExceptionHuge e) {
			System.out.println(" Exception during navigation " + e);
		}

	}

	

	public static void main(String argv[]) throws Exception{
		VTDGen vg = new VTDGen();
		if (vg.parseFile("c:/xml/extended/t1.xml", true)){
			VTDNav vn = vg.getNav();
			System.out.println("good");
		}
		

		System.setProperty("javax.xml.parsers.SAXParserFactory",
		"org.apache.xerces.jaxp.SAXParserFactoryImpl");
		System.setProperty("javax.xml.parsers.DOMParserFactory",
		"org.apache.xerces.jaxp.DOMParserFactoryImpl");
		
		String fileName = "c:/xml/extended/t1.xml";
		DocumentBuilderFactory factory =
			DocumentBuilderFactory.newInstance();
		factory.setNamespaceAware(true);
		factory.setExpandEntityReferences(false);
		DocumentBuilder parser = factory.newDocumentBuilder();
		//Document d = parser.parse(bais);
		Document d = null;
		File f = new File(fileName);
		FileInputStream fis = new FileInputStream(f);
		d = parser.parse(fis);
		
		
		VTDGenHuge vgh = new VTDGenHuge();
		if (vgh.parseFile("c:/xml/extended/t1.xml", true, VTDGenHuge.MEM_MAPPED)){
			VTDNavHuge vn = vgh.getNav();
			System.out.println("good");
			AutoPilotHuge aph = new AutoPilotHuge(vn);
			aph.selectXPath("//*");
			int i;
			while((i=aph.evalXPath())!=-1){
				System.out.println(" "+vn.toString(i));
			}
		}
	}
	
}
