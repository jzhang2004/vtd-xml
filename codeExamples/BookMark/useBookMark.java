/* This example shows you how to use BookMark class.
 * A BookMark object corresponds to a cursor location
 * of VTDNav. To use it, you instantiate a BookMark object,
 * bind it to a VTDNav object, then you call recordCursorPosition()
 * so the bookMark remembers the VTDNav object's cursor location.
 * If you navigate VTDNav to somewhere else in the document, call
 * BookMark object's setCursorPosition() will restore VTDNav's 
 * object's cursor position.
 */

import com.ximpleware.*;

public class useBookMark {
    public static void main(String args[]) throws Exception {
        VTDGen vg = new VTDGen();
        int i;
        AutoPilot ap = new AutoPilot();
        ap.selectXPath("/CATALOG/CD[PRICE < 10]");
	BookMark bm = new BookMark();
        if (vg.parseFile("cd.xml", false)) {
            VTDNav vn = vg.getNav();
	    bm.bind(vn);
            ap.bind(vn);

            //XPath eval returns one node at a time
            while ((i = ap.evalXPath()) != -1) {
                // push the current cursor position
                //vn.push();
		bm.recordCursorPosition(); // equivalent to vn.push();
                // get to the first child
                if (vn.toElement(VTDNav.FIRST_CHILD, "TITLE")) {
                    int j = vn.getText();
                    if (j != -1)
                        System.out.println(" text node ==>" + vn.toString(j));
                }
                // restore the cursor position
                //vn.pop(); 
		bm.setCursorPosition(); // equivalent to vn.pop();
            }
            ap.resetXPath();
        }
    }
}

