package DOMTest;
import com.ximpleware.*;
import java.io.*;
import java.net.HttpURLConnection;
import java.net.URL;
public class t1 {
	public  static  void main1(String[] s) throws Exception{
		/*byte[] doc = new byte[32000]; 
		URL url = new URL("http://shuttles.rpi.edu/displays/netlink.kml"); 
		HttpURLConnection conn = (HttpURLConnection) url.openConnection(); 
		InputStream is = conn.getInputStream(); is.read(doc);
		//System.out.println("k=="+k);*/
		VTDGen vg = new VTDGen(); 
		if (vg.parseZIPFile("c:\\xml\\bioinfo.zip", "bioinfo.xml", false)==false){
			System.out.println("bad");
		}
		VTDNav vn = vg.getNav();
		long l = vn.getContentFragment();
		FileOutputStream fos = new FileOutputStream("out.txt");
		fos.write(vn.getXML().getBytes(), (int)l, (int)(l<<32));
		System.out.println("good");
	}
	
	@SuppressWarnings("unused")
	public  static  void main(String[] s) throws Exception{
		String x="<?xml version='1.0'?>" +
				"<?piname?><!--comment--><?piname?><root><a><b><c a='1'></c>abc</b></a>text</root><?piname?><!--comment--><?piname abc?><!--comment-->";
		x="<!--comment--><!--comment--><root a='a'><?piname ?>abc<b/><!--comment--><b/><b/><b/>cde<?piname b?><!--comment--><b a='a'/><?piname ?>text</root><!--comment-->";
		x="<!--comment--><b> abc <c/> <c/>abc3 <d/>abc4<e><f/></e> abc5 <!--comment--><?pi1 pi1?></b>";
		x="<!--comment--><b> abc <c a='b'>abc<a/>abc<a/><!--comment--><a></a><?abc123 abc123?><a></a><![CDATA[cool!]]> cdeffff<a><cde a='b'>text</cde>text2</a>abc</c><a/></b><!--comment-->";
		x="<!--comment--><b> abc <e>abc <!--comment--> <?abc123 ?> <c a='b'>abc</c>cde<a/><?abc1?><a/>abc</e>eee</b>";
		x="<!--comment--><b> <e><f>abcd <h/><!-- abe--><f/> <?abc ---?></f>abc</e>abc333<e/></b>";
		x="<!--comment--><b><e><h><f a='b'><ee><eee>abc<!--comment--><e ab='ab'> <f>ewrong</f> </e><?abc ebay?><f/><?abc3 ebay2?>!!!<h/></eee></ee></f></h></e></b>";
		
		VTDGen vg = new VTDGen();
		//vg.enableIgnoredWhiteSpace(true);
		vg.setDoc(x.getBytes());
		vg.parse(false);
		VTDNav vn = vg.getNav();
		//vn.toNode(VTDNav.P);
		vn.toNode(VTDNav.FIRST_CHILD);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));	
		vn.toNode(VTDNav.LAST_CHILD);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		vn.toNode(VTDNav.LAST_CHILD);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		vn.toNode(VTDNav.LAST_CHILD);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		vn.toNode(VTDNav.LAST_CHILD);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		vn.toNode(VTDNav.LAST_CHILD);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		vn.toNode(VTDNav.PREV_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		vn.toNode(VTDNav.PREV_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		vn.toNode(VTDNav.PREV_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		vn.toNode(VTDNav.PREV_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		vn.toNode(VTDNav.PREV_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		vn.toNode(VTDNav.PREV_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		vn.toNode(VTDNav.PREV_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		vn.toNode(VTDNav.PREV_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		vn.toNode(VTDNav.PREV_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		
		if (true) return;	
		vn.toNode(VTDNav.FIRST_CHILD);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));	
		vn.toNode(VTDNav.FIRST_CHILD);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		vn.toNode(VTDNav.LAST_CHILD);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		//if (true) return;		
		vn.toNode(VTDNav.PREV_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));		
		//vn.toNode(VTDNav.NEXT_SIBLING);
		//System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		vn.toNode(VTDNav.PREV_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));		
		vn.toNode(VTDNav.PREV_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		vn.toNode(VTDNav.PREV_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		//if (true) return;
		vn.toNode(VTDNav.PREV_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));		
		vn.toNode(VTDNav.PREV_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));		
		
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		
		//vn.toNode(VTDNav.PREV_SIBLING);
		//System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		
		//vn.toNode(VTDNav.PREV_SIBLING);
		//System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		
		//vn.toNode(VTDNav.PREV_SIBLING);
		//System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.PREV_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.PREV_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.PREV_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.PREV_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex())+" "+vn.getTokenDepth(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.PARENT);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		
		
		
		/*vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.PARENT);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		vn.toNode(VTDNav.PREV_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		vn.toNode(VTDNav.PREV_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.PREV_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		vn.toNode(VTDNav.PREV_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.PREV_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		vn.toNode(VTDNav.PREV_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.PREV_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		vn.toNode(VTDNav.PREV_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.PREV_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		vn.toNode(VTDNav.PREV_SIBLING);
		System.out.println(" node name  ==> " +  vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		
		System.out.println(" token type of 18 "+ vn.getTokenType(18));
		/*while(vn.toNode(VTDNav.NEXT_SIBLING))
		System.out.println(" node name  ==> " + vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		while(vn.toNode(VTDNav.PREV_SIBLING))
		System.out.println(" node name  ==> " + vn.getCurrentIndex()+" " +vn.toString(vn.getCurrentIndex()));
		
		
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " + vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		vn.toNode(VTDNav.PREV_SIBLING);
		System.out.println(" node name  ==> " + vn.getCurrentIndex()+" " + vn.toString(vn.getCurrentIndex()));
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " + vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " + vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		vn.toNode(VTDNav.PREV_SIBLING);
		System.out.println(" node name  ==> " + vn.getCurrentIndex()+" " + vn.toString(vn.getCurrentIndex()));
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " + vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " + vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		vn.toNode(VTDNav.NEXT_SIBLING);
		System.out.println(" node name  ==> " + vn.getCurrentIndex()+" "+ vn.toString(vn.getCurrentIndex()));
		*/
	}
}
