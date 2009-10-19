package DOMTest;
import com.ximpleware.*;
import org.w3c.dom.*;
import javax.xml.xpath.*;
import javax.xml.parsers.*;
import java.io.IOException;
import org.xml.sax.SAXException;
import org.jaxen.*;
import org.jaxen.dom.*;
import java.util.*;
public class test1 {
  public static void main(String[] s) {
	  VTDGen vg = new VTDGen();
	  //vg.enableIgnoredWhiteSpace(true);
	  try{
	  if (vg.parseFile("c:/books.xml", true)){
		  VTDNav vn = vg.getNav();
		  AutoPilot ap = new AutoPilot(vn);
		  //ap.selectXPath("//book[title/@lang='it']");
		  //ap.selectXPath("/bookList/book[last()]");
		  
		  ap.selectXPath("substring(\"12345\", -1 div 0, 1 div 0)");
		  System.out.println(" expr string ===>"+ap.evalXPathToString());
		  ap.selectXPath("substring(\"12345\", -42, 1 div 0)");
		  System.out.println(" expr string ===>"+ap.evalXPathToString());
		  ap.selectXPath("//processing-instruction()");
		  System.out.println(" expr string ===>"+ap.getExprString());
		  ap.declareVariableExpr("a", "//*");
		  ap.declareVariableExpr("b", "//text()");
		  ap.selectXPath("$a | $b ");
		  ap.selectXPath("//*[processing-instruction()='abc']");
		  //ap.selectXPath("//processing-instruction()");
		  //ap.selectXPath("//*[name() ='book' or name()='bookList']");
		  //System.out.println(" expr string ===>"+ap.getExprString());
		  //ap.selectXPath("/bookList/book/following::*");
		  ap.selectXPath("//processing-instruction('abc')");
		  int i;
		  while((i=ap.evalXPath())!=-1){
			  //System.out.println("index ==> "+i+" depth==> "+vn.getCurrentDepth());
			  System.out.println("text==>"+vn.toString(i));
			  //if (vn.contains(i, "oo"))
				//  System.out.println("book contains oo");
			  //else
			  //	  System.out.println("error");
		  }
		  
		  ap.selectXPath("local-name(//*)");
		  System.out.println(" string ==> "+ap.evalXPathToString());
		  ap.selectXPath("local-name(//*)");
		  System.out.println(" string ==> "+ap.evalXPathToString());
		  ap.selectXPath("//processing-instruction('abc')");
		  System.out.println(" expr string ===>"+ap.getExprString());
		  /*if (vn.endsWith(i, "< test")){
			 System.out.println(" good ");  
		  }else
			  System.out.println(" bad ");*/
	  }
	  }catch (Exception e){
		  System.out.println(e);
	  }
	  System.out.println("\n ******************* main 2************************");
	  main2();
	  System.out.println("\n ******************* main 3************************");
	  main3();
		 
	  }
  
	  
	  public static void main2(){
		  try{
	      DocumentBuilderFactory domFactory = 
	      DocumentBuilderFactory.newInstance();
	      domFactory.setNamespaceAware(true);
	      //domFactory.setValidating(true);
	      //domFactory.setNamespaceAware(true); 
          domFactory.setIgnoringElementContentWhitespace(true);
	      DocumentBuilder builder = domFactory.newDocumentBuilder();
	      Document doc = builder.parse("c:/books.xml");
	      javax.xml.xpath.XPath xpath = XPathFactory.newInstance().newXPath();
	         // XPath Query for showing all nodes value
	      XPathExpression expr = xpath.compile("//processing-instruction('abc')");
	      System.out.println(expr);

	      Object result = expr.evaluate(doc, XPathConstants.NODESET);
	      NodeList nodes = (NodeList) result;
	      for (int i = 0; i < nodes.getLength(); i++) {
	       System.out.println("text ==>"+nodes.item(i).getNodeName()); 
	      }
		  }catch(Exception e){
			  System.err.println(e);
		  }
	    }
	  
	   public static void main3(){
		   try{
		   org.jaxen.XPath expression = new org.jaxen.dom.DOMXPath("//processing-instruction('abc')");
		   
		  // expression = new org.jaxen.dom.DOMXPath(". | $abc");
		   
		   //System.out.println("====> update_jaxen1 ==>"+xpe); 
		   //parse and xpath eval
		   DocumentBuilderFactory factory =
				DocumentBuilderFactory.newInstance();
           factory.setFeature("http://apache.org/xml/features/dom/defer-node-expansion", false );	
           factory.setNamespaceAware(true);
           //factory.setValidating(true);
           //factory.setIgnoringElementContentWhitespace(true);
           factory.setExpandEntityReferences(false);
		   DocumentBuilder parser = factory.newDocumentBuilder();

		   Document d = null;
		   d = parser.parse("c:/books.xml");
		   //int k = total;
		   //while (k > 0) {
		   List results = expression.selectNodes(d);
		   String sv = expression.stringValueOf(d);
		   System.out.println("string value  "+sv);
		   //results = expression.
		   //k--;
		   //}
		   int size = results.size();
		   int k=0;
		   while(k<size){
			   System.out.println("text==>"+ ((Node)results.get(k)).getNodeName());
			   // System.out.println("text==>"+ ((String)results.get(k)));
			   k++;
		   }
		   }catch(Exception e){
			   System.err.println(e);
		   }
	   }	   
}
