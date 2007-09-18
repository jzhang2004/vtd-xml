import com.ximpleware.*;
import java.util.*;

class XMLBinder {
    ArrayList bind(String fileName) throws Exception{
        ArrayList al = new ArrayList();
        VTDGen vg = new VTDGen();
        AutoPilot ap0 = new AutoPilot();
        AutoPilot ap1 = new AutoPilot();
        AutoPilot ap2 = new AutoPilot();
        AutoPilot ap3 = new AutoPilot();
        AutoPilot ap4 = new AutoPilot();
        
        ap0.selectXPath("/CATALOG/CD[YEAR > 1982 and YEAR < 1990]");
        ap1.selectXPath("TITLE"); 
        ap2.selectXPath("ARTIST"); 
        ap3.selectXPath("PRICE");
        ap4.selectXPath("YEAR");
        
        if (vg.parseFile(fileName,false)){
            VTDNav vn = vg.getNav();
            ap0.bind(vn);
            ap1.bind(vn);
            ap2.bind(vn);
            ap3.bind(vn);
            ap4.bind(vn);
            while(ap0.evalXPath()!=-1){
                CDRecord cdr = new CDRecord();
                cdr.title = ap1.evalXPathToString();
                cdr.artist = ap2.evalXPathToString();
                cdr.price = ap3.evalXPathToNumber();
                cdr.year = (int)ap4.evalXPathToNumber();
                al.add(cdr);
            }
            ap0.resetXPath();
            
        }
        return al;
    }
   
     public static void main(String[] args) throws Exception{
        XMLBinder xb = new XMLBinder();
        ArrayList al = xb.bind("cd.xml");
        Iterator it = al.iterator();
        while(it.hasNext()){
            CDRecord cdr = (CDRecord) it.next();            
            System.out.println("===================");
            System.out.println("TITLE:  ==>  "+cdr.title);
            System.out.println("ARTIST: ==>  "+cdr.artist);
            System.out.println("PRICE:  ==>  "+cdr.price);
            System.out.println("YEAR:   ==>  "+cdr.year);            
        }
    }
}
