package DOMTest;


import java.io.ByteArrayOutputStream;
import com.ximpleware.AutoPilot;
import com.ximpleware.ModifyException;
import com.ximpleware.VTDGen;
import com.ximpleware.VTDNav;
import com.ximpleware.XMLModifier;

 

public class Insert
{
    public static void main( String[] arguments ) throws Exception
    { 
        System.out.println(insert("<foo></foo>", "//foo", "bar"));
        System.out.println(insert("<foo></foo>", "//bogus", "bar"));
        System.out.println(insert("<foo/>", "//foo", "bar"));
    }

    /**
     * Insert a node into an existing document.
     * @param document
     * @param xPath
     * @param node
     * @return
     * @throws Exception
     */

    private static String insert( String document, String xPath, String node ) throws Exception
    {
        VTDGen vg = new VTDGen();
        XMLModifier xm = new XMLModifier();
        vg.setDoc(document.getBytes());
        vg.parse(true);
        VTDNav vn = vg.getNav();
        AutoPilot ap = new AutoPilot(vn);
        xm.bind(vn);
        ap.selectXPath(xPath);
        int xmlIndex = ap.evalXPath();
        if ( xmlIndex > 0 )
        {
            try
            {
                xm.insertAfterHead("</"+node+">");
            }
            catch ( ModifyException me )
            {
                System.out.println("Failed to insert: "+node+" at index: "+xmlIndex);
                throw me;
            }
        }
        ByteArrayOutputStream output = new ByteArrayOutputStream();
        xm.output(output);
        return output.toString();
    }
}

 

