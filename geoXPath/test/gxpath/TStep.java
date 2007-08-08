package gxpath;

import java.io.FileInputStream;
import java.util.HashMap;

import org.apache.log4j.Logger;
import org.apache.log4j.PropertyConfigurator;

import com.ximpleware.AutoPilot;
import com.ximpleware.VTDGen;
import com.ximpleware.VTDNav;

/**
 * do Formal Experiment
 * for paper
 * 
 * @author yanj
 * @date 2007-7-25
 *
 */
public class TStep {
    private static Logger logger = Logger.getLogger(TStep.class);
    
    public static void main(String[] args) {
        PropertyConfigurator.configure("/home/yanj/Desktop/log4j.properties");
        logger.info(">>>>>>>>>>>>>>>>VTD-XML<<<<<<<<<<<<<<<<<<<<<");
        
        String ipolygon = "<gml:Polygon><gml:outerBoundaryIs><gml:LinearRing><gml:coordinates>-77.008232,38.966557 -76.911209,38.889988 -77.045448,38.78812 -77.035248,38.813915 -77.045189,38.829365 -77.040405,38.838413 -77.039078,38.862431 -77.067886,38.886101 -77.078949,38.9156 -77.122627,38.93206 -77.042389,38.993431 -77.008232,38.966557</gml:coordinates></gml:LinearRing></gml:outerBoundaryIs></gml:Polygon>";
        HashMap<String, String[]> testSet = new HashMap<String, String[]>();
        testSet.put("/home/yanj/geoserver-wfs-data/data/topp_states.xml", 
                new String[]{
                "//gml:boundedBy/gml:Box"
                //"//gml:featureMember/topp:states[contains(gp:geometry-type(./topp:the_geom/gml:MultiPolygon), \"gml:Polygon\")]",
                //"//gml:featureMember/topp:states[gp:within(./topp:the_geom/gml:MultiPolygon, \"" + ipolygon + "\")]",
                //"//gml:featureMember/topp:states[gp:intersection(./topp:the_geom/gml:MultiPolygon, \"" + ipolygon + "\")]"
        });/*
        testSet.put("/home/yanj/geoserver-wfs-data/data/topp_water_shorelines.xml", 
                new String[]{
                "//gml:featureMember/topp:water_shorelines[contains(gp:geometry-type(./topp:the_geom/gml:LineString), \"gml:Polygon\")]",
                "//gml:featureMember/topp:water_shorelines[gp:within(./topp:the_geom/gml:LineString, \"" + ipolygon + "\")]",
                "//gml:featureMember/topp:water_shorelines[gp:intersection(./topp:the_geom/gml:LineString, \"" + ipolygon + "\")]"
        });
        testSet.put("/home/yanj/geoserver-wfs-data/data/topp_major_roads.xml", 
                new String[]{
                "//gml:featureMember/topp:major_roads[contains(gp:geometry-type(./topp:gen_full/gml:LineString), \"gml:Polygon\")]",
                "//gml:featureMember/topp:major_roads[gp:within(./topp:gen_full/gml:LineString, \"" + ipolygon + "\")]",
                "//gml:featureMember/topp:major_roads[gp:intersection(./topp:gen_full/gml:LineString, \"" + ipolygon + "\")]"
        });
        testSet.put("/home/yanj/geoserver-wfs-data/data/topp_countries.xml", 
                new String[]{
                "//gml:featureMember/topp:countries[contains(gp:geometry-type(./topp:gen_full/*), \"gml:Polygon\")]",
                "//gml:featureMember/topp:countries[gp:within(./topp:gen_full/*, \"" + ipolygon + "\")]",
                "//gml:featureMember/topp:countries[gp:intersection(./topp:gen_full/*, \"" + ipolygon + "\")]"
        });
        testSet.put("/home/yanj/geoserver-wfs-data/data/topp_poly_county.xml", 
                new String[]{
                "//gml:featureMember/topp:poly_county[contains(gp:geometry-type(./topp:gen_full/gml:Polygon), \"gml:Polygon\")]",
                "//gml:featureMember/topp:poly_county[gp:within(./topp:gen_full/gml:Polygon, \"" + ipolygon + "\")]",
                "//gml:featureMember/topp:poly_county[gp:intersection(./topp:gen_full/gml:Polygon, \"" + ipolygon + "\")]"
        });*/

        try{
            for (String file : testSet.keySet()){
                FileInputStream fis = new FileInputStream(file);
                byte[] data = new byte[fis.available()];
                long readtime = System.currentTimeMillis();
                fis.read(data);
                readtime = System.currentTimeMillis() - readtime;
                fis.close();
                VTDGen vg = new VTDGen();
                vg.setDoc(data);
                long parsetime = System.currentTimeMillis();
                vg.parse(true);
                parsetime = System.currentTimeMillis() - parsetime;
                VTDNav vn = vg.getNav();
                
                String filename = file.substring(file.lastIndexOf('/') + 1);
                String log = "File:\t" + filename + "\n";
                log += "Time of read file:\t" + readtime + "\n";
                log += "Time of parse file:\t" + parsetime;
                logger.info(log);
                
                AutoPilot ap = new AutoPilot();
                ap.declareXPathNameSpace("gml", "http://www.opengis.net/gml");
                ap.declareXPathNameSpace("topp", "http://www.openplans.org/topp");
                
                ap.bind(vn);
                ap.selectXPath("/*");   //prewarming
                ap.resetXPath();
                for (String gxpath : testSet.get(file)){
                    long gpparsetime = System.currentTimeMillis();
                    ap.selectXPath(gxpath);
                    gpparsetime = System.currentTimeMillis() - gpparsetime;
                    
                    int l1 = gxpath.indexOf("gp:");
                    String pathname = gxpath.substring(l1);
                    int l2 = pathname.indexOf('(');
                    pathname = pathname.substring(0, l2);
                    log = "------------------------------------------\n";
                    log += "GXPath:\t" + pathname + "\n";
                    log += "Time of path-parseing (selectXPath):\t" + gpparsetime;
                    logger.info(log);
                    
                    long evaltime = System.currentTimeMillis();
                    int i = ap.evalXPath();
                    int n = 0;
                    while (i != -1){
                        /*
                        long element = vn.getElementFragment();
                        int offset = (int) element;
                        int length = (int) (element >>> 32);
                        logger.info("\t\t>>>result node " + n + ":\n" + new String(data, offset, length));
                        */
                        n++;
                        i = ap.evalXPath();
                    }
                    evaltime = System.currentTimeMillis() - evaltime;
                    ap.resetXPath();
                    vn.toElement(VTDNav.ROOT);
                    vn.toElement(VTDNav.PARENT);
                    
                    log = "Time of evaluation:\t" + evaltime + "\n";
                    log += "Amount of Result Nodes:\t" + n;
                    logger.info(log);
                }
                logger.info("=================================");
            }
        }catch(Exception e){
            e.printStackTrace();
        }
    }

}
