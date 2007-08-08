package gxpath;

import java.io.FileInputStream;
import java.io.FileWriter;

import com.ximpleware.AutoPilot;
import com.ximpleware.VTDGen;
import com.ximpleware.VTDNav;

/**
 * analyzing the Test Data
 * 
 * @author yanj
 * @date 2007-7-24
 * 
 */
public class DStep {

    public static void main(String[] args) {
        String base = "/home/yanj/geoserver-wfs-data/data/";
        String[] files = { "tiger_giant_polygon.xml",
                "topp_tasmania_cities.xml", 
                "tiger_poi.xml",
                "topp_tasmania_state_boundaries.xml",
                "topp_tasmania_roads.xml", 
                "topp_tasmania_water_bodies.xml",
                "topp_poly_landmarks.xml", 
                "tiger_poly_landmarks.xml",
                "topp_states.xml", "topp_alaska.xml", "topp_gnis.xml",
                "topp_gnis_pop.xml", "topp_pip.xml", "tiger_tiger_roads.xml",
                "topp_water_shorelines.xml", "topp_roads.xml",
                "topp_water_polygon.xml", "topp_us_districts_109.xml",
                "topp_major_roads.xml", "topp_countries.xml",
                "topp_poly_county.xml"
                };
        try {
            FileWriter logFile = new FileWriter(base + "analysis.log");
            logFile.write("FileName \t TotalNum/FeatureNum \t MaxDepth/AvgDepth\n");

            for (String file : files) {
                int featureNum = 0;
                int totalNum = 0;
                int maxDepth = 0;
                float avgDepth = 0;
                String logStr = file + "\t";
                
                file = base + file;
                FileInputStream fis = new FileInputStream(file);
                byte[] data = new byte[fis.available()];
                fis.read(data);
                fis.close();
                
                VTDGen vg = new VTDGen();
                vg.setDoc(data);
                vg.parse(true);

                VTDNav vn = vg.getNav();
                AutoPilot ap = new AutoPilot();
                ap.bind(vn);
                ap.selectElement("*");
                while (ap.iterate()) {
                    totalNum++;
                    if (vn.matchElement("gml:featureMember"))
                        featureNum++;
                    int cd = vn.getCurrentDepth();
                    if (cd > maxDepth)
                        maxDepth = cd;
                    avgDepth += cd;
                }
                avgDepth /= totalNum;
                
                logStr += totalNum + "/" + featureNum + "\t" + maxDepth + "/" + avgDepth + "\n";
                logFile.write(logStr);
            }
            logFile.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

}
