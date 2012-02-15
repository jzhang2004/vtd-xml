package DOMTest;

import com.ximpleware.AutoPilot;
import com.ximpleware.VTDGen;
import com.ximpleware.VTDNav;

public class cdata {
	public static void main(String s[]) throws Exception{
		VTDGen vg =  new VTDGen();
		if (vg.parseFile("c://xml//cdata2.xml", true)){
			VTDNav vn = vg.getNav();
			AutoPilot ap = new AutoPilot(vn);
			ap.selectXPath("/*[/comment()='comment &']");
			int i=-1;
			while((i=ap.evalXPath())!=-1){
				//long l = vn.getContentFragment();
				System.out.println(" -==> "+vn.toString(i+1));
			}
		}
	}
}
