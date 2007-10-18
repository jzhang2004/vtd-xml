/**
 * Erase all the attribute value node in an XML doc using overwrite
 * The key is to use "" to overwrite a attribute value
 * NS is turned on so ns nodes are overwritten
 */
import com.ximpleware.*;
import java.io.*;
public class erase {

    public static void main(String[] args) throws Exception {
        VTDGen vg = new VTDGen();
        AutoPilot ap = new AutoPilot();
        //ap.selectXPath("/*/*/*");
        AutoPilot ap2 = new AutoPilot();
        ap2.selectXPath("//@*");
        if (vg.parseFile("soap2.xml",true)){
            VTDNav vn = vg.getNav();
            ap.bind(vn);
            ap2.bind(vn);
            //ap.evalXPath();
            int i;
            while((i=ap2.evalXPath())!=-1){
                //System.out.println("attr name ---> "+ i+ " "+vn.toString(i)+"  value ---> "+vn.toString(i+1));
                vn.overWrite(i+1,"".getBytes());
            }
            System.out.println(new String(vn.getXML().getBytes()));
        }
    }
}

