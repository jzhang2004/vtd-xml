import java.io.*;
import com.ximpleware.*;
//This example shows how to load a VTD+XML index
// into memory then run an XPath against it
// vg.loadIndex returns an instance of VTDNav
public class indexLoad {
	public static void main(String args[]) {
		try{
			VTDGen vg = new VTDGen();
			VTDNav vn = vg.loadIndex("po.vxl");
			AutoPilot ap= new AutoPilot(vn);
			ap.selectXPath("//items");
			int i;
			while((i=ap.evalXPath())!=-1){
				// do whatever you like here
			}
		}catch(Exception e){
		}
	}
}
