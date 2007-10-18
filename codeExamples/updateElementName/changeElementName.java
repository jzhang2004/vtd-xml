/*
 * Change all elements to lalalala
 */
import com.ximpleware.*;

public class changeElementName {

    public static void main(String[] args) throws Exception{
        
        String xml = "<aaaa> <bbbbb> <ccccc> </ccccc> <ccccc/> <ccccc></ccccc> </bbbbb> </aaaa>";
        VTDGen vg = new VTDGen();
        vg.setDoc(xml.getBytes());
        vg.parse(false);
        VTDNav vn = vg.getNav();
        AutoPilot ap = new AutoPilot(vn);
        ap.selectXPath("//*");
        XMLModifier xm = new XMLModifier(vn);
        int i;
        while(ap.evalXPath()!=-1){
            xm.updateElementName("lalalala");
        }
        xm.output("lala.xml");
        
    }
}

