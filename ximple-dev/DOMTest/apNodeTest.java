package DOMTest;
import com.ximpleware.*;
public class apNodeTest {
	public static void main(String s[]) throws Exception{
		String xml = "<?xml version='1.0'?> <a attr='attrval' xmlns='abc'> " +
				"abc <a/> <![CDATA[cdata]]> <?abc efg?> <!--comment--> " +
				"</a>";
		VTDGen vg = new VTDGen();
		vg.setDoc(xml.getBytes());
		vg.parse(true);
		VTDNav vn = vg.getNav();
		AutoPilot ap = new AutoPilot(vn);
		vn.toNode(VTDNav.P);
		//ap.selectNode();
		//while(ap.iterate2()){
		//	System.out.println(" "+ vn.getCurrentIndex()+"  "+vn.toString(vn.getCurrentIndex()));
		//}
		System.out.println("-----------------------");
		ap.declareXPathNameSpace("abc", "abc");
		ap.selectXPath("//node()");
		//System.out.println(" ===>"+ap.evalXPathToString());
		int i=-1;
		while((i=ap.evalXPath())!=-1){
			System.out.println(" "+ vn.getCurrentIndex()+"  "+vn.toString(vn.getCurrentIndex()));
		}
		
		ap.selectXPath("//processing-instruction()");
		while((i=ap.evalXPath())!=-1){
			System.out.println(" "+ vn.getCurrentIndex()+"  "+vn.toString(vn.getCurrentIndex()));
		}
	}
}
