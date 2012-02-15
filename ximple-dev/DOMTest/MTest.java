// Use java -Xmx4g -Xms1g MTest
// or you get an OutOfMemoryException.
// Use a computer with >4G RAM for this test.
package DOMTest;
import java.io.*;
import com.ximpleware.*;

public class MTest {
    public static void main(String[] args) throws Exception{
        StringBuilder sb = new StringBuilder();
        String startTag="<a>";
        String endTag="</a>";
        int tags = startTag.length() + endTag.length();
        System.out.println("Length of tags: "+tags);
        int length = 512*1024*1024 -tags ; // this does _not_ work
        // int length = 512*1024*1024 -tags -1;  // this works. Note -1
        sb.append(startTag);
        for (int i = 0; i < length; i++) {
            sb.append("a");
        }
        sb.append(endTag);
        byte[] b=sb.toString().getBytes();
	    System.out.println("size of byte array is "+ b.length );
        VTDGen vg = new VTDGen();
        AutoPilot ap = new AutoPilot();
        ap.selectXPath("/a");
        XMLModifier xm = new XMLModifier();
        vg.setDoc(b);
        vg.parse(true);
        VTDNav vn = vg.getNav();
        ap.bind(vn);
        xm.bind(vn);
        while (ap.evalXPath() != -1) {
            long l = vn.getContentFragment();
            xm.insertAfterElement("<b>b</b>");
            xm.remove();
        }
        ByteArrayOutputStream bout = new ByteArrayOutputStream();
        xm.output(bout);  // Exeption here!
        // NullpointerException at com.ximpleware.XMLModifier.output(XMLModifier.java:1708)
        // when document is 512 MB or bigger.

        System.out.println(bout.toString());
        // when document is <512 MB output is  <b>b</b>
    }
}
