package DOMTest;
import com.ximpleware.*;
import com.ximpleware.extended.*;
public class alex {

	public static void main(String s[]) throws Exception{
		VTDGen vg =  new VTDGen();
		vg.selectLcDepth(5);
		//vg.enableIgnoredWhiteSpace(true);
		if (vg.parseFile("c://xml//test22.xml", true)){
			VTDNav vn = vg.getNav();
			//System.out.println(vn.getXPathStringVal());
			AutoPilot ap = new AutoPilot(vn);
			//ap.selectXPath("//CDResults[../../../TargetName/@Value=//SiteInformation[TargetName/@Value!=//SiteInformation[1]/TargetName/@Value and TargetName/@Value!=//SiteInformation[TargetName/@Value!=//SiteInformation[1]/TargetName/@Value][1]/TargetName/@Value][1]/TargetName/@Value]/BottomCD/@Value");
			//ap.selectNode();
			//while(ap.iterate2()){
			//	if (vn.matchTokenString(vn.getCurrentIndex(),"CDResults"))
			//	 System.out.println(vn.toString(vn.getCurrentIndex()));
			//}
			//System.out.println("max nesting depth -->"+vn.getNestingLevel());
			//ap.selectXPath("//SiteInformation[TargetName/@Value!=//SiteInformation[1]/TargetName/@Value][1]/TargetName/@Value");
			//ap.selectXPath("//CDResults[../../../TargetName/@Value=//SiteInformation[TargetName/@Value!=//SiteInformation[1]/TargetName/@Value and TargetName/@Value!=//SiteInformation[TargetName/@Value!=//SiteInformation[1]/TargetName/@Value][1]/TargetName/@Value][1]/TargetName/@Value]/BottomCD/@Value");
			//ap.selectXPath("//CDResults[../../../TargetName/@Value=//SiteInformation[TargetName/@Value!=//SiteInformation[1]/TargetName/@Value]]/BottomCD/@Value");
			//ap.selectXPath("//CDResults[../../../TargetName/@Value=//SiteInformation[TargetName/@Value!=//SiteInformation[1]/TargetName/@Value]]/BottomCD/@Value");
			//ap.selectXPath("//SiteInformation[TargetName/@Value!=//SiteInformation[1]/TargetName/@Value]/BottomCD/@Value");
			//ap.selectXPath("//SiteInformation[1]/TargetName/@Value");
			//ap.selectXPath("count(/*/CCC[position()=last() and @a='1'])");
			//ap.selectXPath("//a[//c|..]|./a/b|..");
			//System.out.println(" expr---> "+ ap.getExprString());
			ap.selectXPath("//CDResults["+
		             "../../../TargetName/@Value=" +
		        
						    "//SiteInformation["+
                                                "TargetName/@Value!=//SiteInformation[1]/TargetName/@Value "+
                                                        "and " +
                                                 " TargetName/@Value!=//SiteInformation["+
                                                         					   "TargetName/@Value!=//SiteInformation[1]/TargetName/@Value"+
                                                         				       "][1]/TargetName/@Value"+
							 "][1]/TargetName/@Value]/BottomCD/@Value");
			//ap.selectNode();
			//ap.selectXPath("/descendant-or-self::node()/child::SiteInformation [1]/child::TargetName/attribute::Value");
			//ap.selectXPath("(./a/b/c[//abc])[abc][//cde]");
			//ap.selectXPath("(//*)[../ab | abc | cde]");
			//ap.selectXPath("/node()/node()[position()>2 and position()<100]");
			//ap.selectXPath("/*/good/comment()/*/that/*");
			//ap.declareVariableExpr("myexpr", "//abc");
			//ap.selectXPath("$myexpr[$myexpr]");
			//ap.selectXPath("(//node())[100]");
			//System.out.println(" expr---> "+ ap.getExprString());
			//ap.selectXPath("(//a)['1'=./abc or ( ./cde = 'abc' and //kef=1 )]");
			//ap.selectXPath("/*/*[1]");
			System.out.println(" expr---> "+ ap.getExprString());
			//ap.selectXPath("//node()");
			int i=-1;
			int size=0;
			System.out.println("ok");
			while((i=ap.evalXPath())!=-1){	
				if (true){
					String ss = null;
					//if (vn.getTokenType(i)==VTDNav.TOKEN_STARTING_TAG ){
					if (vn.getTokenType(i)== VTDNav.TOKEN_STARTING_TAG)
						ss = " starting tag";
					else
						ss = " leaf";
					//System.out.println(vn.toString(i)+"    depth ==> "+ vn.getTokenDepth(i)+" index ==>"+i+" type ==>"+ss);			
					System.out.println(vn.toString(i+1)+"    depth ==> "+ vn.getTokenDepth(i+1)+" index ==>"+i+" type ==>"+ss);
				}
				size++;
			}
			System.out.println("==== \n size is ==> "+ size);
		}
	}
}
