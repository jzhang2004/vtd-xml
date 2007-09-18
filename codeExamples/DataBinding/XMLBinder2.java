import com.ximpleware.*;
import java.util.*;
import java.io.*;

class XMLBinder2 {

    ArrayList bind2(VTDNav vn) throws Exception {
        ArrayList al = new ArrayList();
        AutoPilot ap0 = new AutoPilot();
        AutoPilot ap1 = new AutoPilot();
        AutoPilot ap2 = new AutoPilot();
        AutoPilot ap3 = new AutoPilot();
        AutoPilot ap4 = new AutoPilot();

        ap0.selectXPath("/CATALOG/CD[YEAR > 1982 and YEAR < 1990]");
        /* /CATALOG/CD[YEAR > 1982 and YEAR < 1990]/TITLE */
        ap1.selectXPath("TITLE");
        /* /CATALOG/CD[YEAR > 1982 and YEAR < 1990]/ARTIST */
        ap2.selectXPath("ARTIST");
        /* /CATALOG/CD[YEAR > 1982 and YEAR < 1990]/PRICE */
        ap3.selectXPath("PRICE");
        /* /CATALOG/CD[YEAR > 1982 and YEAR < 1990]/YEAR */
        ap4.selectXPath("YEAR");

        ap0.bind(vn);
        ap1.bind(vn);
        ap2.bind(vn);
        ap3.bind(vn);
        ap4.bind(vn);
        while (ap0.evalXPath() != -1) {
            CDRecord2 cdr = new CDRecord2();
            cdr.title = ap1.evalXPathToString();
            cdr.artist = ap2.evalXPathToString();
            vn.push();
            ap3.evalXPath();
            cdr.priceIndex = vn.getText(); 
            cdr.price = vn.parseDouble(cdr.priceIndex);
            ap3.resetXPath();
            vn.pop();
            cdr.year = (int) ap4.evalXPathToNumber();
            al.add(cdr);
        }
        ap0.resetXPath();
        return al;
    }

   public static void main(String[] args) throws Exception{
        XMLBinder2 xb = new XMLBinder2();       
        VTDGen vg = new VTDGen();
        XMLModifier xm = new XMLModifier();

        if (vg.parseFile("cd.xml",false)){
           VTDNav vn = vg.getNav();
           ArrayList al2 = xb.bind2(vn);
           Iterator it2 = al2.iterator();
           xm.bind(vn);
           while(it2.hasNext()){
               CDRecord2 cdr = (CDRecord2) it2.next();
               // reduce prices by 1
               xm.updateToken(cdr.priceIndex, ""+(cdr.price - 1));       
           }
           xm.output(new FileOutputStream("cd_update.xml"));       
        }
    }
}
