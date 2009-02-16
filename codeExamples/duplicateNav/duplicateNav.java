/**
 * This example shows how to create multiple VTDNav objects from
 * the same XML, LC and VTD buffer. Parallel XPath evaluation is
 * enabled by duplicateNav(). 
 * 
 */

import com.ximpleware.*;
public class duplicateNav {
    public static void main(String[] args) throws Exception{
        VTDGen vg = new VTDGen();
        if (vg.parseFile("mix3.xml",true)){
            VTDNav vn = vg.getNav();
            // duplicated VTDNav instances share the same XML, LC buffers and VTD buffers.
            VTDNav vn2 = vn.duplicateNav();
            VTDNav vn3 = vn2.duplicateNav();
            AutoPilot ap = new AutoPilot(vn3);
            ap.selectXPath("//*");
            int i;
            while((i=ap.evalXPath())!=-1){
                System.out.println(vn3.toString(i));
            }
        }
    }
}
