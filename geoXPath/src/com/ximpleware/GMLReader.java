package com.ximpleware;

import java.util.ArrayList;

import org.apache.log4j.Logger;

import com.vividsolutions.jts.geom.CoordinateSequence;
import com.vividsolutions.jts.geom.CoordinateSequenceFactory;
import com.vividsolutions.jts.geom.Geometry;
import com.vividsolutions.jts.geom.GeometryFactory;
import com.vividsolutions.jts.geom.LineString;
import com.vividsolutions.jts.geom.LinearRing;
import com.vividsolutions.jts.geom.Point;
import com.vividsolutions.jts.geom.Polygon;
import com.ximpleware.xpath.Expr;

/**
 * 读取GML片断转换为相应的JTS Geometry
 * 
 * @author yanj
 * @date 2007-7-12
 * 
 */
public class GMLReader {
    private static Logger logger = Logger.getLogger(GMLReader.class);
    
    private static final String GML_POINT = "Point";
    private static final String GML_LINESTRING = "LineString";
    private static final String GML_POLYGON = "Polygon";
    private static final String GML_COORDINATES = "coordinates";
    private static final String GML_MULTIPOINT = "MultiPoint";
    private static final String GML_MULTILINESTRING = "MultiLineString";
    private static final String GML_MULTIPOLYGON = "MultiPolygon";

    private static GeometryFactory gf = new GeometryFactory();
    /**
     * 用于VTD-XML遍历、解析过程中的转换
     * 
     * @param vn
     *            带有当前节点信息
     * @return JTS Geometry
     */
    public static Geometry read(VTDNav vn){
        return (Geometry) read_(vn);
    }
    public static Geometry read(VTDNav vn, Expr pe){
        Geometry g = null;
        if (pe.isString()){
            String pe_str = pe.evalString(vn);
            g = parse(pe_str, "UTF-8");
        } else {
            vn.push2();
            try{
                int flag = pe.evalNodeSet(vn);
                if (flag != -1)
                    g = read(vn);
            }catch(Exception e){
                e.printStackTrace();
            }
            pe.reset(vn);
            vn.pop2();
        }
        return g;
    }

    /**
     * 用于转换GML文本
     * 
     * @param gmlstr
     *            GML文本片断
     * @param charset
     *            GML文本片断的编码格式
     * @return JTS Geometry
     */
    public static Geometry parse(String gmlstr, String charset) {
        byte[] data = null;
        VTDNav vn = null;
        try {
            data = gmlstr.getBytes(charset);
            VTDGen vg = new VTDGen();
            vg.setDoc(data);
            vg.parse(true);
            vn = vg.getNav();
        } catch (Exception e) {
            logger.error(e);
        }
        return read(vn);
    }
    /**
     * 递归实现GML片断向Geometry的转换
     * @param vn
     * @return
     */
    private static Object read_(VTDNav vn) {
        Object result = null;
        try {
            String tname = getCurrentElementName(vn);
            //System.out.println(tname);
            if (GML_POINT.equals(tname)){
                vn.toElement(VTDNav.FIRST_CHILD);
                CoordinateSequence ch = (CoordinateSequence) read_(vn);
                result = gf.createPoint(ch);
                vn.toElement(VTDNav.PARENT);
            } else if (GML_LINESTRING.equals(tname)){
                vn.toElement(VTDNav.FIRST_CHILD);
                CoordinateSequence ch = (CoordinateSequence) read_(vn);
                result = gf.createLineString(ch);
                vn.toElement(VTDNav.PARENT);
            } else if (GML_POLYGON.equals(tname)){
                LinearRing shell = null;
                ArrayList<LinearRing> holeList = new ArrayList<LinearRing>();
                boolean flag = vn.toElement(VTDNav.FIRST_CHILD);    //outerBoundaryIs or innerBoundaryIs
                while (flag == true){
                    if (getCurrentElementName(vn).equals("outerBoundaryIs")){
                        vn.toElement(VTDNav.FIRST_CHILD);           //LinearRing
                        vn.toElement(VTDNav.FIRST_CHILD);           //coordinates
                        CoordinateSequence ch = (CoordinateSequence) read_(vn);
                        shell = gf.createLinearRing(ch);
                        vn.toElement(VTDNav.PARENT);
                        vn.toElement(VTDNav.PARENT);
                    } else { // innerBoundaryIs
                        vn.toElement(VTDNav.FIRST_CHILD);
                        vn.toElement(VTDNav.FIRST_CHILD);
                        CoordinateSequence ch = (CoordinateSequence) read_(vn);
                        LinearRing hole = gf.createLinearRing(ch);
                        holeList.add(hole);
                        vn.toElement(VTDNav.PARENT);
                        vn.toElement(VTDNav.PARENT);
                    }
                    flag = vn.toElement(VTDNav.NEXT_SIBLING);
                }
                LinearRing[] holes = new LinearRing[holeList.size()];
                holeList.toArray(holes);
                result = gf.createPolygon(shell, holes);
                vn.toElement(VTDNav.PARENT);
            } else if (GML_COORDINATES.equals(tname)){
                int cidx = vn.getText();
                String content = vn.toString(cidx);
                //System.out.println(content);
                //only default tuple separator and coord separator supported
                String[] tuples = content.split(" ");
                int tuplenum = tuples.length;
                int dim = tuples[0].split(",").length;  //test dimension
                CoordinateSequenceFactory csf = gf.getCoordinateSequenceFactory();
                CoordinateSequence cs = csf.create(tuplenum, dim);
                for (int i = 0; i < tuplenum; i++){
                    String[] coords = tuples[i].split(",");
                    for (int j = 0; j < coords.length; j++){
                        double value = Double.parseDouble(coords[j]);
                        cs.setOrdinate(i, j, value);
                    }
                }
                result = cs;
            } else if (GML_MULTIPOINT.equals(tname)){
                boolean flag = vn.toElement(VTDNav.FIRST_CHILD); //pointMember
                ArrayList<Point> plist = new ArrayList<Point>();
                while (flag == true){
                    vn.toElement(VTDNav.FIRST_CHILD); //Point
                    Point p = (Point) read_(vn);
                    plist.add(p);
                    vn.toElement(VTDNav.PARENT);
                    flag = vn.toElement(VTDNav.NEXT_SIBLING);
                }
                Point[] ps = new Point[plist.size()];
                plist.toArray(ps);
                result = gf.createMultiPoint(ps);
                vn.toElement(VTDNav.PARENT);
            } else if (GML_MULTILINESTRING.equals(tname)){
                boolean flag = vn.toElement(VTDNav.FIRST_CHILD); //lineStringMember
                ArrayList<LineString> lsList = new ArrayList<LineString>();
                while (flag == true){
                    vn.toElement(VTDNav.FIRST_CHILD); //LineString
                    LineString ls = (LineString) read_(vn);
                    lsList.add(ls);
                    vn.toElement(VTDNav.PARENT);
                    flag = vn.toElement(VTDNav.NEXT_SIBLING);
                }
                LineString[] lss = new LineString[lsList.size()];
                lsList.toArray(lss);
                result = gf.createMultiLineString(lss);
                vn.toElement(VTDNav.PARENT);
            } else if (GML_MULTIPOLYGON.equals(tname)){
                boolean flag = vn.toElement(VTDNav.FIRST_CHILD); //polygonMember
                ArrayList<Polygon> plist = new ArrayList<Polygon>();
                while (flag == true){
                    vn.toElement(VTDNav.FIRST_CHILD); //Polygon
                    Polygon p = (Polygon) read_(vn);
                    plist.add(p);
                    vn.toElement(VTDNav.PARENT);
                    flag = vn.toElement(VTDNav.NEXT_SIBLING);
                }
                Polygon[] ps = new Polygon[plist.size()];
                plist.toArray(ps);
                result = gf.createMultiPolygon(ps);
                vn.toElement(VTDNav.PARENT);
            }
        } catch (Exception e) {
            logger.error(e);
        }
        return result;
    }
    
    /**
     * 获取当前Element的名称，包括前缀
     * 要求遍历至Element标记开端，否则返回空串
     * @param vn
     * @return
     */
    private static String getCurrentElementName(VTDNav vn){
        int index = vn.getCurrentIndex();
        int type = vn.getTokenType(index);
        if (type != VTDNav.TOKEN_STARTING_TAG)
            return "";
        
        int offset = vn.getTokenOffset(index);
        int length = vn.getTokenLength(index);
        //remove prefix
        int prelen = (length >> 16) & 0xffff;
        int loffset = offset;
        int llength = length;
        if (prelen > 0){
            loffset = offset + prelen + 1;
            llength = (length & 0xffff) - prelen - 1;
        }
        
        byte[] data = vn.getXML().getBytes(loffset, llength);
        return new String(data);
    }
    
    /**
     * for gp:envelope-type()
     * @param vn
     * @return
     */
    public static String parseType(VTDNav vn){
        String result = getCurrentElementName(vn);
        return result;
    }
    /**
     * for gp:envelope-type("<Polygon>...</Polygon>")
     * @param str
     * @return
     */
    public static  String parseType(String str){
        String result = "";
        try {
            byte[] data = str.getBytes();
            VTDGen vg = new VTDGen();
            vg.setDoc(data);
            vg.parse(false);
            VTDNav vn = vg.getNav();
            vn.toElement(VTDNav.ROOT);
            result = getCurrentElementName(vn);
        } catch (Exception e) {
            logger.error(e);
        }
        return result;
    }
    /**
     * for gp:envelope-type(./topp:the_geom/*)
     * @param vn
     * @param pe
     * @return
     */
    public static String parseType(VTDNav vn, Expr pe){
        String result = "";
        vn.push2();
        try{
            pe.evalNodeSet(vn);
        }catch(Exception e){
            logger.error(e);
        }
        result = getCurrentElementName(vn);
        pe.reset(vn);
        vn.pop2();
        return result;
    }
}
