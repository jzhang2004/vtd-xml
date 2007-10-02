// A simple app that shuffles elements in an XML file
// it uses XPath to address individual element
// then re-arrange the fragments
/* The input XML looks like this:
   <root>
     <a> text </a>
     <b> text </b>
     <c> text </c>
     <a> text </a>
     <b> text </b>
     <c> text </c>
     <a> text </a>
     <b> text </b>
     <c> text </c>
   </root>
*/

/* The ouput XML looks like this:
   <root>
     <a> text </a>
     <a> text </a>
     <a> text </a>
     <b> text </b>
     <b> text </b>
     <b> text </b>
     <c> text </c>
     <c> text </c>
     <c> text </c>
   </root>  
 */

import com.ximpleware.*;
import java.io.*;
public class shuffle {
    public static void main(String[] args) throws Exception {
        VTDGen vg = new VTDGen();
        AutoPilot ap0 = new AutoPilot();
        AutoPilot ap1 = new AutoPilot();
        AutoPilot ap2 = new AutoPilot();
        ap0.selectXPath("/root/a");
        ap1.selectXPath("/root/b");
        ap2.selectXPath("/root/c");
        
        if (vg.parseFile("old.xml",false)){
            VTDNav vn = vg.getNav();
            ap0.bind(vn);
            ap1.bind(vn);
            ap2.bind(vn);
            FileOutputStream fos = new FileOutputStream("new.xml");
            fos.write("<root>".getBytes());
            byte[] ba = vn.getXML().getBytes();
            while(ap0.evalXPath()!=-1){
                long l= vn.getElementFragment();
                int offset = (int)l;
                int len = (int)(l>>32);
                fos.write('\n');
                fos.write(ba,offset, len);
            }
            ap0.resetXPath();
            while(ap1.evalXPath()!=-1){
                long l= vn.getElementFragment();
                int offset = (int)l;
                int len = (int)(l>>32);
                fos.write('\n');
                fos.write(ba,offset, len);
            }
            ap1.resetXPath();
            while(ap2.evalXPath()!=-1){
                long l= vn.getElementFragment();
                int offset = (int)l;
                int len = (int)(l>>32);
                fos.write('\n');
                fos.write(ba,offset, len);
            }
            ap2.resetXPath();
            fos.write('\n');
            fos.write("</root>".getBytes());
        }
    }
}
