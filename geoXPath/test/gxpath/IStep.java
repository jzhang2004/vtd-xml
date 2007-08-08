package gxpath;

import java.io.FileInputStream;

import com.ximpleware.AutoPilot;
import com.ximpleware.VTDGen;
import com.ximpleware.VTDNav;

/**
 * Class for thoughts validation
 * 
 * @author yanj
 * @date 2007-6-4
 * 
 */
public class IStep {
    public static void main(String[] args) {
        String file = "data/places.gml";
        String[] xpath = {"//gml:featureMember[position() mod 2 = 0]",
                "//gml:Box/gml:coord/gml:X",
                "//ogr:geometryProperty",
                "//ogr:geometryProperty/*[contains(name(), \"Point\")]",
                "//ogr:geometryProperty/*[1][contains(name(../*/*[1]), \"Point\")]",
                "gp:within(//gml:Point, \"<Polygon><outerBoundaryIs><LinearRing><coordinates>24000.0,38666.666666666664 25567.427339446596,38157.37865166653 26536.150710120408,36824.04531833319 26536.15071012041,35175.95468166681 25567.427339446596,33842.62134833347 24000.0,33333.333333333336 22432.572660553404,33842.62134833347 21463.849289879592,35175.95468166681 21463.84928987959,36824.04531833319 22432.572660553404,38157.37865166653 24000.0,38666.666666666664 </coordinates></LinearRing></outerBoundaryIs><innerBoundaryIs><LinearRing><coordinates>24000.0,36000.0 25338.261212717716,37486.289650954786 24069.798993405002,37998.78165403819 24000.0,36000.0</coordinates></LinearRing></innerBoundaryIs></Polygon>\")", //error: Function Expr can't eval to nodeset
                "//gml:Point[not(gp:within(../gml:Point))]",
                "//gml:Point[gp:within(\"<Polygon><outerBoundaryIs><LinearRing><coordinates>24000.0,38666.666666666664 25567.427339446596,38157.37865166653 26536.150710120408,36824.04531833319 26536.15071012041,35175.95468166681 25567.427339446596,33842.62134833347 24000.0,33333.333333333336 22432.572660553404,33842.62134833347 21463.849289879592,35175.95468166681 21463.84928987959,36824.04531833319 22432.572660553404,38157.37865166653 24000.0,38666.666666666664 </coordinates></LinearRing></outerBoundaryIs><innerBoundaryIs><LinearRing><coordinates>24000.0,36000.0 25338.261212717716,37486.289650954786 24069.798993405002,37998.78165403819 24000.0,36000.0</coordinates></LinearRing></innerBoundaryIs></Polygon>\")]"};
//        String[] gxpath = {"basic", "predicate", "analysis"};

        try {
            FileInputStream fis = new FileInputStream(file);
            byte[] data = new byte[fis.available()];
            fis.read(data);
            fis.close();

            VTDGen vg = new VTDGen();
            vg.setDoc(data);

            AutoPilot ap = new AutoPilot();
            ap.declareXPathNameSpace("gml", "http://www.opengis.net/gml");
            ap.declareXPathNameSpace("ogr", "http://ogr.maptools.org/");
            ap.selectXPath(xpath[4]);
            vg.parse(true);
            VTDNav vn = vg.getNav();
            ap.bind(vn);
            int i = ap.evalXPath();
            int n = 0;
            while (i != -1) {
                n++;
                long element = vn.getElementFragment();
                int offset = (int) element;
                int length = (int) (element >>> 32);

                System.out.println(vn.toString(i) + " length:" + length
                        + " offset:" + offset);
                System.out.println(new String(data, offset, length));
                
                i = ap.evalXPath();
            }
            ap.resetXPath();
            System.out.println("n = " + n);

        } catch (Exception e) {
            e.printStackTrace();
        }

    }

}
