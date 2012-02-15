package DOMTest;
import com.ximpleware.*;
import com.ximpleware.extended.*;
public class XPathTester {
    public static void println(String s) {
        System.out.println(s);
    }

    public static VTDNav parseString(String s) throws Exception {
        VTDGen vg = new VTDGen();
        //vg.selectLcDepth(5);
        vg.setDoc(s.getBytes());
        vg.parse(true);
        return vg.getNav();
    }
    
    public static VTDNavHuge parseString1(String s) throws Exception {
    	 VTDGenHuge vg = new VTDGenHuge();
         vg.setDoc(new XMLBuffer(s.getBytes()));
         vg.parse(true);
         return vg.getNav();    	
    }
    
    public static void test0() throws Exception{
    	
    }
    // test basic node selection functions
    public static void test1() throws Exception{
        String s = "<AAA>"
            +"<BBB/>"
            +"<CCC/>"
            +"<BBB/>"
            +"<BBB/>"
            +"<DDD>"
            +"<BBB/>"
            +"</DDD>"
            +"<CCC/>" 
            +"</AAA>";
        VTDNav vn = parseString(s);
        AutoPilot ap = new AutoPilot(vn);
        
        ap.selectXPath("/AAA");
        int i;
        while((i=ap.evalXPath())!=-1){
            if (vn.compareTokenString(i,"AAA")!=0)
                println("test1 1 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
            else 
                println("test1 1 succeed");
        }
        
        ap.selectXPath("count(//BBB/following-sibling::*)");
        i=-1;
        if (ap.evalXPathToNumber()==5.0){
        	println("test1 2 succeed");
        } else
        	println("test1 2 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("count(//CCC/following-sibling::*)");
        i=-1;
        if (ap.evalXPathToNumber()==4.0){
        	println("test1 3 succeed");
        } else
        	println("test1 3 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("count(//DDD/following-sibling::*)");
        i=-1;
        if (ap.evalXPathToNumber()==1.0){
        	println("test1 4 succeed");
        } else
        	println("test1 4 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("count(//DDD)");
        i=-1;
        if (ap.evalXPathToNumber()==1.0){
        	println("test1 5 succeed");
        } else
        	println("test1 5 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
       	
        ap.selectXPath("count(//*)");
        if (ap.evalXPathToNumber()==8.0){
        	println("test1 6 succeed");
        } else
        	println("test1 6 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("count(//node())");
        if (ap.evalXPathToNumber()==8.0){
        	println("test1 7 succeed");
        } else
        	println("test1 7 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("count(/descendant-or-self::node())");
        if (ap.evalXPathToNumber()==9.0){
        	println("test1 8 succeed");
        } else
        	println("test1 8 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("count(//CCC/preceding-sibling::*)");
        i=-1;
        if (ap.evalXPathToNumber()==5.0){
        	println("test1 9 succeed");
        } else
        	println("test1 9 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("count(//CCC/preceding-sibling::node())");
        i=-1;
        if (ap.evalXPathToNumber()==5.0){
        	println("test1 10 succeed");
        } else
        	println("test1 10 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        s = "<AAA> text1"
            +"<BBB/> <?my my?>"
            +"<CCC a='2'/> text2"
            +"<BBB/> <!--comment--><?my1 my?>"
            +"<BBB/> <!--comment2-->"
            +"<DDD>  <![CDATA[text3]]>"
            +"<BBB/> text3"
            +"</DDD>"
            +"<CCC a='1'/> <![CDATA[text3]]>" 
            +"</AAA>";
        
        vn = parseString(s);
        ap = new AutoPilot(vn);
        
        ap.selectXPath("count(/*/text())");
        if (ap.evalXPathToNumber()==3.0){
        	println("test1 11 succeed");
        } else
        	println("test1 11 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("count(/*/comment())");
        if (ap.evalXPathToNumber()==2.0){
        	println("test1 12 succeed");
        } else
        	println("test1 12 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("count(/*/processing-instruction())");
        if (ap.evalXPathToNumber()==2.0){
        	println("test1 13 succeed");
        } else
        	println("test1 13 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("count(/*/processing-instruction('my'))");
        if (ap.evalXPathToNumber()==1.0){
        	println("test1 14 succeed");
        } else
        	println("test1 14 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("count(/*/node()) = count(/*/processing-instruction())+ count(/*/comment())+ count(/*/text())+ count(/*/*)");
        if (ap.evalXPathToBoolean()){
        	println("test1 15 succeed");
        } else
        	println("test1 15 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("count(/*/*/node()) = count(/*/*/processing-instruction())+ count(/*/*/comment())+ count(/*/*/text())+ count(/*/*/*)");
        if (ap.evalXPathToBoolean()){
        	println("test1 16 succeed");
        } else
        	println("test1 16 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        s = "<AAA> text1"
            +"<BBB/> <?my my?>"
            +"<CCC a='2'/> text2"
            +"<BBB/> <!--comment--><?my1 my?>"
            +"<BBB/> <!--comment2-->"
            +"<DDD>  <![CDATA[text3]]>"
            +"<BBB/> text3"
            +"</DDD>"
            +"<CCC a='1'/> <![CDATA[text3]]>" 
            +"</AAA>";
        vn = parseString(s);
        ap = new AutoPilot(vn);
        ap.selectXPath("count(/*/CCC[position()=last() and @a='1'])");
        if (ap.evalXPathToNumber()==1.0){
        	println("test1 17 succeed");
        } else
        	println("test1 17 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    }
    
    public static void test11() throws Exception{
        String s = "<AAA>"
            +"<BBB/>"
            +"<CCC/>"
            +"<BBB/>"
            +"<BBB/>"
            +"<DDD>"
            +"<BBB/>"
            +"</DDD>"
            +"<CCC/>" 
            +"</AAA>";
        VTDNavHuge vn = parseString1(s);
        AutoPilotHuge ap = new AutoPilotHuge(vn);
        ap.selectXPath("/AAA");
        int i;
        while((i=ap.evalXPath())!=-1){
            if (vn.compareTokenString(i,"AAA")!=0)
                println("test11 1 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
            else 
                println("test11 1 succeed");
        }
        ap.selectXPath("count(//BBB/following-sibling::*)");
        i=-1;
        if (ap.evalXPathToNumber()==5.0){
        	println("test11 2 succeed");
        } else
        	println("test11 2 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("count(//CCC/following-sibling::*)");
        i=-1;
        if (ap.evalXPathToNumber()==4.0){
        	println("test11 3 succeed");
        } else
        	println("test11 3 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("count(//DDD/following-sibling::*)");
        i=-1;
        if (ap.evalXPathToNumber()==1.0){
        	println("test11 4 succeed");
        } else
        	println("test11 4 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    }
    // test relation operators on node set vs node set
    public static void test2() throws Exception{
        String s = "<test2>" +
        		"<e1>good</e1>" +
        		"<e1>bad</e1>" +
        		"<f1>bad</f1>" +
        		"<f1>good</f1>" +
        		"</test2>";
        VTDNav vn = parseString(s);
        AutoPilot ap = new AutoPilot(vn);
        ap.selectXPath("/test2/e1=/test2/f1");
        if (ap.evalXPathToBoolean())
            println("test2 1 succeed");
        else
            println("test2 1 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("/test2/e1!=/test2/f1");
        if (ap.evalXPathToBoolean())
            println("test2 2 succeed");
        else
            println("test2 2 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("/test2/e1 > /test2/f1");
        
        if (ap.evalXPathToBoolean())
            println("test2 3 succeed");
        else
            println("test2 3 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("/test2/e1 >= /test2/f1");
        
        if (ap.evalXPathToBoolean())
            println("test2 4 succeed");
        else
            println("test2 4 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("/test2/e1 <= /test2/f1");
        
        if (ap.evalXPathToBoolean())
            println("test2 5 succeed");
        else
            println("test2 5 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("/test2/e1 < /test2/f1");
        
        if (ap.evalXPathToBoolean())
            println("test2 6 succeed");
        else
            println("test2 6 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("/test2/e1 < /test2/e1");
        
        if (ap.evalXPathToBoolean())
            println("test2 7 succeed");
        else
            println("test2 7 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("/test2/e1 <= /test2/e1");
        
        if (ap.evalXPathToBoolean())
            println("test2 8 succeed");
        else
            println("test2 8 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("/test2/e1 > /test2/e1");
        
        if (ap.evalXPathToBoolean())
            println("test2 9 succeed");
        else
            println("test2 9 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("/test2/e1 >= /test2/e1");
        
        if (ap.evalXPathToBoolean())
            println("test2 10 succeed");
        else
            println("test2 10 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("/test2/e1 = /test2/e1");
        
        if (ap.evalXPathToBoolean())
            println("test2 11 succeed");
        else
            println("test2 11 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("/test2/e1 != /test2/e1");
        
        if (ap.evalXPathToBoolean())
            println("test2 12 succeed");
        else
            println("test2 12 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("/test2/*[starts-with(name(.),'e')] !=/test2/f1");
        
        if (ap.evalXPathToBoolean())
            println("test2 13 succeed");
        else
            println("test2 13 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
    }
    
    public static void test22() throws Exception{
        String s = "<test2>" +
        		"<e1>good</e1>" +
        		"<e1>bad</e1>" +
        		"<f1>bad</f1>" +
        		"<f1>good</f1>" +
        		"</test2>";
        VTDNavHuge vn = parseString1(s);
        AutoPilotHuge ap = new AutoPilotHuge(vn);
        ap.selectXPath("/test2/e1=/test2/f1");
        if (ap.evalXPathToBoolean())
            println("test22 1 succeed");
        else
            println("test22 1 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("/test2/e1!=/test2/f1");
        if (ap.evalXPathToBoolean())
            println("test22 2 succeed");
        else
            println("test22 2 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("/test2/e1 > /test2/f1");
        
        if (ap.evalXPathToBoolean())
            println("test22 3 succeed");
        else
            println("test22 3 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("/test2/e1 >= /test2/f1");
        
        if (ap.evalXPathToBoolean())
            println("test22 4 succeed");
        else
            println("test22 4 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("/test2/e1 <= /test2/f1");
        
        if (ap.evalXPathToBoolean())
            println("test22 5 succeed");
        else
            println("test22 5 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("/test2/e1 < /test2/f1");
        
        if (ap.evalXPathToBoolean())
            println("test22 6 succeed");
        else
            println("test22 6 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("/test2/e1 < /test2/e1");
        
        if (ap.evalXPathToBoolean())
            println("test22 7 succeed");
        else
            println("test22 7 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("/test2/e1 <= /test2/e1");
        
        if (ap.evalXPathToBoolean())
            println("test22 8 succeed");
        else
            println("test22 8 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("/test2/e1 > /test2/e1");
        
        if (ap.evalXPathToBoolean())
            println("test22 9 succeed");
        else
            println("test22 9 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("/test2/e1 >= /test2/e1");
        
        if (ap.evalXPathToBoolean())
            println("test22 10 succeed");
        else
            println("test22 10 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("/test2/e1 = /test2/e1");
        
        if (ap.evalXPathToBoolean())
            println("test22 11 succeed");
        else
            println("test22 11 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("/test2/e1 != /test2/e1");
        
        if (ap.evalXPathToBoolean())
            println("test22 12 succeed");
        else
            println("test22 12 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("/test2/*[starts-with(name(.),'e')] !=/test2/f1");
        
        if (ap.evalXPathToBoolean())
            println("test22 13 succeed");
        else
            println("test22 13 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
    }
    
    // relational operator on node set vs number
    public static void test3() throws Exception{
        String s = "<test2>"
            + "<e1 attr='1'>4</e1>" 
            + "<e1 attr='2'>44</e1>"
            + "<f1 attr='3'>1000</f1>" 
            + "<f1 attr='4' attr1='6'>10e5</f1>" 
            + "</test2>";
        VTDNav vn = parseString(s);
        AutoPilot ap = new AutoPilot(vn);
        ap.selectXPath("/test2/*/@* !=4");
            if (ap.evalXPathToBoolean())
                println("test3 1 succeed");
            else
                println("test3 1 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@*=4");
            if (ap.evalXPathToBoolean())
                println("test3 2 succeed");
            else
                println("test3 2 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* >4");
            if (ap.evalXPathToBoolean())
                println("test3 3 succeed");
            else
                println("test3 3 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* <=6");
            if (ap.evalXPathToBoolean())
                println("test3 4 succeed");
            else
                println("test3 4 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* <4 ");
            if (ap.evalXPathToBoolean())
                println("test3 5 succeed");
            else
                println("test3 5 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* <=6");
            if (ap.evalXPathToBoolean())
                println("test3 6 succeed");
            else
                println("test3 6 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* =6");
            if (ap.evalXPathToBoolean())
                println("test3 7 succeed");
            else
                println("test3 7 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* =4");
            if (ap.evalXPathToBoolean())
                println("test3 8 succeed");
            else
                println("test3 8 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/* <=1000");
            if (ap.evalXPathToBoolean())
                println("test3 9 succeed");
            else
                println("test3 9 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/* =1000");
            if (ap.evalXPathToBoolean())
                println("test3 10 succeed");
            else
                println("test3 10 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/* !=1000");
            if (ap.evalXPathToBoolean())
                println("test3 11 succeed");
            else
                println("test3 11 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/* >=1000");
            if (ap.evalXPathToBoolean())
                println("test3 12 succeed");
            else
                println("test3 12 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/* >1000");
            if (ap.evalXPathToBoolean())
                println("test3 13 succeed");
            else
                println("test3 13 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/* > count(/test2/*/@*[. > 1]) ");
            if (ap.evalXPathToBoolean())
                println("test3 14 succeed");
            else
                println("test3 14 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/* = count(/test2/*/@*[. > 1]) ");
            if (ap.evalXPathToBoolean())
                println("test3 15 succeed");
            else
                println("test3 15 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/* < count(/test2/*/@*[. > 1]) ");
            if (!ap.evalXPathToBoolean())
                println("test3 16 succeed");
            else
                println("test3 16 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/* > count(/test2/*/@*[1 < .]) ");
            if (ap.evalXPathToBoolean())
                println("test3 17 succeed");
            else
                println("test3 17 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/* = count(/test2/*/@*[1< .]) ");
            if (ap.evalXPathToBoolean())
                println("test3 18 succeed");
            else
                println("test3 18 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/* < count(/test2/*/@*[1<.]) ");
            if (!ap.evalXPathToBoolean())
                println("test3 19 succeed");
            else
                println("test3 19 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("  count(/test2/*/@*[1 < .])< /test2/* ");
            if (ap.evalXPathToBoolean())
                println("test3 20 succeed");
            else
                println("test3 20 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath(" count(/test2/*/@*[1 < .])= /test2/* ");
            if (ap.evalXPathToBoolean())
                println("test3 21 succeed");
            else
                println("test3 21 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath(" count(/test2/*/@*[1 < .])> /test2/* ");
            if (!ap.evalXPathToBoolean())
                println("test3 22 succeed");
            else
                println("test3 22 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");     
       
        
    }
    
    public static void test33() throws Exception{
        String s = "<test2>"
            + "<e1 attr='1'>4</e1>" 
            + "<e1 attr='2'>44</e1>"
            + "<f1 attr='3'>1000</f1>" 
            + "<f1 attr='4' attr1='6'>10e5</f1>" 
            + "</test2>";
        VTDNavHuge vn = parseString1(s);
        AutoPilotHuge ap = new AutoPilotHuge(vn);
        ap.selectXPath("/test2/*/@* !=4");
            if (ap.evalXPathToBoolean())
                println("test33 1 succeed");
            else
                println("test33 1 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@*=4");
            if (ap.evalXPathToBoolean())
                println("test33 2 succeed");
            else
                println("test33 2 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* >4");
            if (ap.evalXPathToBoolean())
                println("test33 3 succeed");
            else
                println("test33 3 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* <=6");
            if (ap.evalXPathToBoolean())
                println("test33 4 succeed");
            else
                println("test33 4 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* <4 ");
            if (ap.evalXPathToBoolean())
                println("test33 5 succeed");
            else
                println("test33 5 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* <=6");
            if (ap.evalXPathToBoolean())
                println("test33 6 succeed");
            else
                println("test33 6 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* =6");
            if (ap.evalXPathToBoolean())
                println("test33 7 succeed");
            else
                println("test33 7 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* =4");
            if (ap.evalXPathToBoolean())
                println("test33 8 succeed");
            else
                println("test33 8 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/* <=1000");
            if (ap.evalXPathToBoolean())
                println("test33 9 succeed");
            else
                println("test33 9 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/* =1000");
            if (ap.evalXPathToBoolean())
                println("test33 10 succeed");
            else
                println("test33 10 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/* !=1000");
            if (ap.evalXPathToBoolean())
                println("test33 11 succeed");
            else
                println("test33 11 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/* >=1000");
            if (ap.evalXPathToBoolean())
                println("test33 12 succeed");
            else
                println("test33 12 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/* >1000");
            if (ap.evalXPathToBoolean())
                println("test33 13 succeed");
            else
                println("test33 13 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/* > count(/test2/*/@*[. > 1]) ");
            if (ap.evalXPathToBoolean())
                println("test33 14 succeed");
            else
                println("test33 14 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/* = count(/test2/*/@*[. > 1]) ");
            if (ap.evalXPathToBoolean())
                println("test33 15 succeed");
            else
                println("test33 15 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/* < count(/test2/*/@*[. > 1]) ");
            if (!ap.evalXPathToBoolean())
                println("test33 16 succeed");
            else
                println("test33 16 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/* > count(/test2/*/@*[1 < .]) ");
            if (ap.evalXPathToBoolean())
                println("test33 17 succeed");
            else
                println("test33 17 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/* = count(/test2/*/@*[1< .]) ");
            if (ap.evalXPathToBoolean())
                println("test33 18 succeed");
            else
                println("test33 18 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/* < count(/test2/*/@*[1<.]) ");
            if (!ap.evalXPathToBoolean())
                println("test33 19 succeed");
            else
                println("test33 19 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("  count(/test2/*/@*[1 < .])< /test2/* ");
            if (ap.evalXPathToBoolean())
                println("test33 20 succeed");
            else
                println("test33 20 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath(" count(/test2/*/@*[1 < .])= /test2/* ");
            if (ap.evalXPathToBoolean())
                println("test33 21 succeed");
            else
                println("test33 21 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath(" count(/test2/*/@*[1 < .])> /test2/* ");
            if (!ap.evalXPathToBoolean())
                println("test33 22 succeed");
            else
                println("test33 22 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");     
       
        
    }
    
    // relational operator on node set vs string
    public static void test4() throws Exception{
        String s = "<test2>"
            + "<e1 attr='1'>4</e1>" 
            + "<e1 attr='2'>44</e1>"
            + "<f1 attr='3'>1000</f1>" 
            + "<f1 attr='4' attr1='6' attr2=''>10e5</f1>" 
            + "</test2>";
        VTDNav vn = parseString(s);
        AutoPilot ap = new AutoPilot(vn);
        ap.selectXPath("/test2/*/@* !='4'");
        	if (ap.evalXPathToBoolean())
                println("test4 1 succeed");
            else
                println("test4 1 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* ='4'");
            if (ap.evalXPathToBoolean())
                println("test4 2 succeed");
            else
                println("test4 2 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* >='4'");
            if (ap.evalXPathToBoolean())
                println("test4 3 succeed");
            else
                println("test4 3 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* >'4'");
            if (ap.evalXPathToBoolean())
                println("test4 4 succeed");
            else
                println("test4 4 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* <'4'");
            if (ap.evalXPathToBoolean())
                println("test4 5 succeed");
            else
                println("test4 5 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* <='4'");
            if (ap.evalXPathToBoolean())
                println("test4 6 succeed");
            else
                println("test4 6 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* <='4'");
            if (ap.evalXPathToBoolean())
                println("test4 7 succeed");
            else
                println("test4 7 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* =''");
            if (ap.evalXPathToBoolean())
                println("test4 8 succeed");
            else
                println("test4 8 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* >=''");
            if (ap.evalXPathToBoolean())
                println("test4 9 succeed");
            else
                println("test4 9 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("not(/test2/*/@* ='')");
            if (!ap.evalXPathToBoolean())
                println("test4 10 succeed");
            else
                println("test4 10 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("not(/test2/*/@* ='')");
            if (!ap.evalXPathToBoolean())
                println("test4 11 succeed");
            else
                println("test4 11 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("not(/test2/*/@* ='')");
            if (!ap.evalXPathToBoolean())
                println("test4 12 succeed");
            else
                println("test4 12 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* = /test2/*");
            if (ap.evalXPathToBoolean())
                println("test4 13 succeed");
            else
                println("test4 13 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* >= /test2/*");
            if (ap.evalXPathToBoolean())
                println("test4 14 succeed");
            else
                println("test4 14 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* <= /test2/*");
            if (ap.evalXPathToBoolean())
                println("test4 15 succeed");
            else
                println("test4 15 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");


            ap.selectXPath("'4' = /test2/*/@* ");
            if (ap.evalXPathToBoolean())
                println("test4 16 succeed");
            else
                println("test4 16 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath(" '4' = /test2/*/@* ");
            if (ap.evalXPathToBoolean())
                println("test4 17 succeed");
            else
                println("test4 17 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("'4' <= /test2/*/@* ");
            if (ap.evalXPathToBoolean())
                println("test4 18 succeed");
            else
                println("test4 18 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("'4' < /test2/*/@* ");
            if (ap.evalXPathToBoolean())
                println("test4 19 succeed");
            else
                println("test4 19 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("'4' > /test2/*/@* ");
            if (ap.evalXPathToBoolean())
                println("test4 20 succeed");
            else
                println("test4 20 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* <='4'");
            if (ap.evalXPathToBoolean())
                println("test4 21 succeed");
            else
                println("test4 21 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* <='4'");
            if (ap.evalXPathToBoolean())
                println("test4 22 succeed");
            else
                println("test4 22 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* =''");
            if (ap.evalXPathToBoolean())
                println("test4 23 succeed");
            else
                println("test4 23 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* >=''");
            if (ap.evalXPathToBoolean())
                println("test4 24 succeed");
            else
                println("test4 24 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("not(/test2/*/@* ='')");
            if (!ap.evalXPathToBoolean())
                println("test4 25 succeed");
            else
                println("test4 25 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("not( '' = /test2/*/@*)");
            if (!ap.evalXPathToBoolean())
                println("test4 26 succeed");
            else
                println("test4 26 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("not(/test2/*/@* ='')");
            if (!ap.evalXPathToBoolean())
                println("test4 27 succeed");
            else
                println("test4 27 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
            
            ap.selectXPath("/test2/*/@* = 1");
            if (ap.evalXPathToBoolean())
                println("test4 28 succeed");
            else
                println("test4 28 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
            
            ap.selectXPath("/test2/*/@* = 4");
            if (ap.evalXPathToBoolean())
                println("test4 29 succeed");
            else
                println("test4 29 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
            
            ap.selectXPath("/test2/*/@* != 1");
            if (ap.evalXPathToBoolean())
                println("test4 30 succeed");
            else
                println("test4 30 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

    }
    
    public static void test44() throws Exception{
        String s = "<test2>"
            + "<e1 attr='1'>4</e1>" 
            + "<e1 attr='2'>44</e1>"
            + "<f1 attr='3'>1000</f1>" 
            + "<f1 attr='4' attr1='6' attr2=''>10e5</f1>" 
            + "</test2>";
        VTDNavHuge vn = parseString1(s);
        AutoPilotHuge ap = new AutoPilotHuge(vn);
        ap.selectXPath("/test2/*/@* !='4'");
        	if (ap.evalXPathToBoolean())
                println("test44 1 succeed");
            else
                println("test44 1 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* ='4'");
            if (ap.evalXPathToBoolean())
                println("test44 2 succeed");
            else
                println("test44 2 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* >='4'");
            if (ap.evalXPathToBoolean())
                println("test44 3 succeed");
            else
                println("test44 3 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* >'4'");
            if (ap.evalXPathToBoolean())
                println("test44 4 succeed");
            else
                println("test44 4 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* <'4'");
            if (ap.evalXPathToBoolean())
                println("test44 5 succeed");
            else
                println("test44 5 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* <='4'");
            if (ap.evalXPathToBoolean())
                println("test44 6 succeed");
            else
                println("test44 6 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* <='4'");
            if (ap.evalXPathToBoolean())
                println("test44 7 succeed");
            else
                println("test44 7 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* =''");
            if (ap.evalXPathToBoolean())
                println("test44 8 succeed");
            else
                println("test44 8 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* >=''");
            if (ap.evalXPathToBoolean())
                println("test44 9 succeed");
            else
                println("test44 9 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("not(/test2/*/@* ='')");
            if (!ap.evalXPathToBoolean())
                println("test44 10 succeed");
            else
                println("test44 10 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("not(/test2/*/@* ='')");
            if (!ap.evalXPathToBoolean())
                println("test44 11 succeed");
            else
                println("test44 11 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("not(/test2/*/@* ='')");
            if (!ap.evalXPathToBoolean())
                println("test44 12 succeed");
            else
                println("test44 12 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* = /test2/*");
            if (ap.evalXPathToBoolean())
                println("test44 13 succeed");
            else
                println("test44 13 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* >= /test2/*");
            if (ap.evalXPathToBoolean())
                println("test44 14 succeed");
            else
                println("test44 14 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* <= /test2/*");
            if (ap.evalXPathToBoolean())
                println("test44 15 succeed");
            else
                println("test44 15 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");


            ap.selectXPath("'4' = /test2/*/@* ");
            if (ap.evalXPathToBoolean())
                println("test44 16 succeed");
            else
                println("test44 16 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath(" '4' = /test2/*/@* ");
            if (ap.evalXPathToBoolean())
                println("test44 17 succeed");
            else
                println("test44 17 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("'4' <= /test2/*/@* ");
            if (ap.evalXPathToBoolean())
                println("test44 18 succeed");
            else
                println("test44 18 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("'4' < /test2/*/@* ");
            if (ap.evalXPathToBoolean())
                println("test44 19 succeed");
            else
                println("test44 19 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("'4' > /test2/*/@* ");
            if (ap.evalXPathToBoolean())
                println("test44 20 succeed");
            else
                println("test44 20 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* <='4'");
            if (ap.evalXPathToBoolean())
                println("test44 21 succeed");
            else
                println("test44 21 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* <='4'");
            if (ap.evalXPathToBoolean())
                println("test44 22 succeed");
            else
                println("test44 22 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* =''");
            if (ap.evalXPathToBoolean())
                println("test44 23 succeed");
            else
                println("test44 23 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("/test2/*/@* >=''");
            if (ap.evalXPathToBoolean())
                println("test44 24 succeed");
            else
                println("test44 24 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("not(/test2/*/@* ='')");
            if (!ap.evalXPathToBoolean())
                println("test44 25 succeed");
            else
                println("test44 25 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("not( '' = /test2/*/@*)");
            if (!ap.evalXPathToBoolean())
                println("test44 26 succeed");
            else
                println("test44 26 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

            ap.selectXPath("not(/test2/*/@* ='')");
            if (!ap.evalXPathToBoolean())
                println("test44 27 succeed");
            else
                println("test44 27 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
            
            ap.selectXPath("/test2/*/@* = 1");
            if (ap.evalXPathToBoolean())
                println("test44 28 succeed");
            else
                println("test44 28 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
            
            ap.selectXPath("/test2/*/@* = 4");
            if (ap.evalXPathToBoolean())
                println("test44 29 succeed");
            else
                println("test44 29 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
            
            ap.selectXPath("/test2/*/@* != 1");
            if (ap.evalXPathToBoolean())
                println("test44 30 succeed");
            else
                println("test44 30 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    }
    
    //test XPath function: translate
    public static void test5() throws Exception{
        String s = "<test2>"
            + "<e1 attr='1'>A</e1>" 
            + "<e1 attr='2'>ABC</e1>"
            + "<f1 attr='3'>_aabb_</f1>" 
            + "<f1 attr='4' attr1='6'></f1>" 
            + "</test2>";
        VTDNav vn = parseString(s);
        
        AutoPilot ap = new AutoPilot(vn);
        ap.selectXPath("translate('12:30','30','45')");
        String value = ap.evalXPathToString();
        if (value.equals("12:45"))
            println("test5 1 succeed");
        else
            println("test5 1 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!: " + value);
        
        ap.selectXPath("translate('12:30','0123','abc')");
        value = ap.evalXPathToString();
        if (value.equals("bc:a"))
            println("test5 2 succeed");
        else
            println("test5 2 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!: " + value);
        
        ap.selectXPath("translate('','','abc')");
        value = ap.evalXPathToString();
        if (value.equals(""))
            println("test5 3 succeed");
        else
            println("test5 3 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!: " + value);
    }
    
    //test XPath function: translate
    public static void test55() throws Exception{
        String s = "<test2>"
            + "<e1 attr='1'>A</e1>" 
            + "<e1 attr='2'>ABC</e1>"
            + "<f1 attr='3'>_aabb_</f1>" 
            + "<f1 attr='4' attr1='6'></f1>" 
            + "</test2>";
        VTDNavHuge vn = parseString1(s);
        
        AutoPilotHuge ap = new AutoPilotHuge(vn);
        ap.selectXPath("translate('12:30','30','45')");
        String value = ap.evalXPathToString();
        if (value.equals("12:45"))
            println("test55 1 succeed");
        else
            println("test55 1 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!: " + value);
        
        ap.selectXPath("translate('12:30','0123','abc')");
        value = ap.evalXPathToString();
        if (value.equals("bc:a"))
            println("test55 2 succeed");
        else
            println("test55 2 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!: " + value);
        
        ap.selectXPath("translate('','','abc')");
        value = ap.evalXPathToString();
        if (value.equals(""))
            println("test55 3 succeed");
        else
            println("test55 3 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!: " + value);
        
        //test round-half-to-even
        //non extended version
        VTDNav vn1 = parseString(s);
        
        AutoPilot ap1 = new AutoPilot(vn1);
        ap1.selectXPath("round-half-to-even(0.5)");
        double num = ap1.evalXPathToNumber();
        if (num == 0)
            println("test55 4 succeed");
        else
            println("test55 4 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!: " + num);
        
        ap1.selectXPath("round-half-to-even(1.5)");
        num = ap1.evalXPathToNumber();
        if (num == 2)
            println("test55 5 succeed");
        else
            println("test55 5 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!: " + num);
        
        ap1.selectXPath("round-half-to-even(-1.5)");
        num = ap1.evalXPathToNumber();
        if (num == -2)
            println("test55 6 succeed");
        else
            println("test55 6 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!: " + num);

        ap1.selectXPath("round-half-to-even(2.5)");
        num = ap1.evalXPathToNumber();
        if (num == 2)
            println("test55 7 succeed");
        else
            println("test55 7 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!: " + num);
        
        ap1.selectXPath("round-half-to-even(-2.5)");
        num = ap1.evalXPathToNumber();
        if (num == -2)
            println("test55 8 succeed");
        else
            println("test55 8 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!: " + num);
        
        //this caused exception:
        //"XPath Syntax error: #32" from parser
        //ap1.selectXPath("round-half-to-even(3.567812E+3, 2)");
        //num = ap1.evalXPathToNumber();
        //if (num == 3567.81)
        //    println("test55 9 succeed");
        //else
        //    println("test55 9 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!: " + num);        

        ap1.selectXPath("round-half-to-even(3567.812, 2)");
        num = ap1.evalXPathToNumber();
        if (num == 3567.81)
            println("test55 9 succeed");
        else
            println("test55 9 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!: " + num);        
        
        ap1.selectXPath("round-half-to-even(35612.25, -2)");
        num = ap1.evalXPathToNumber();
        if (num == 35600)
            println("test55 10 succeed");
        else
            println("test55 10 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!: " + num);        
        
    }
    
    public static void test6() throws Exception{
        String s = "<test2>"
            + "<e1 attr='1.1'>4</e1>" 
            + "<e1 attr='2.2'>44</e1>"
            + "<f1 attr='3'>1000</f1>" 
            + "<f1 attr='4' attr1='6' attr2='0.1'>10e5</f1>" 
            + "<test >abc az AZ ABC</test>"
            + "</test2>";
        VTDNav vn = parseString(s);
        AutoPilot ap = new AutoPilot(vn);
        ap.selectXPath("count(//@*)");
        double d = ap.evalXPathToNumber();
        if (d==6)
        	println("test6 1 succeed");
        else 
        	println("test6 1 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        ap.selectXPath("sum(//@*)");
        d = ap.evalXPathToNumber();
        if (((float)d)==16.4)
        	println("test6 2 succeed  " + (float)d);
        else 
        	println("test6 2 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  " +  (float)d);   
        ap.selectXPath("upper-case(/test2/test)");
        String s1 = ap.evalXPathToString();
        if (s1.matches("ABC AZ AZ ABC"))
        	println("test6 3 succeed  "+s1);
        else
        	println("test6 3 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!   "+s1);
        ap.selectXPath("lower-case(/test2/test)");
        s1 = ap.evalXPathToString();
        if (s1.matches("abc az az abc"))
        	println("test6 4 succeed  "+s1);
        else
        	println("test6 4 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!   "+s1);
        ap.selectXPath("starts-with(/test2/test,'abc')");
        if (ap.evalXPathToBoolean())
        	println("test6 5 succeed");
        else
        	println("test6 5 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("ends-with(/test2/test,'ABC')");
        if (ap.evalXPathToBoolean())
        	println("test6 6 succeed");
        else
        	println("test6 6 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        s = "<test2>"
            + "<e1 attr='1.1'>4</e1>" 
            + "<e1 attr='2.2'>44</e1>"
            + "<f1 attr='3'>1000</f1>" 
            + "<f1 attr='4' attr1='6' attr2='0.1'>10e5</f1>" 
            + "<test xml:lang='en'>abc az AZ ABC</test>"
            + "<test3 ><![CDATA[abc &lt; az AZ ABC]]></test3>"
            + "<test4 ><![CDATA[&lt; az AZ ABC &lt;]]></test4>"
            + "<test5 >&lt; az AZ ABC &lt;</test5>"
            + "<a:test xmlns:a='xyz'> xyz </a:test> "
            + "</test2>";
        vn = parseString(s);
        ap = new AutoPilot(vn);
        ap.selectXPath("/test2/test[lang('en')]");
        println(" expr is "+ap.getExprString());
        if( ap.evalXPathToBoolean())
        	println("test6 7 succeed ");
        else
        	println("test6 7 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ");
        
        ap.selectXPath("contains(/test2/test,'bc az AZ ABC')");
        if (ap.evalXPathToBoolean())
        	println("test6 8 succeed");
        else
        	println("test6 8 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("name(/test2/test)");
        if ((ap.evalXPathToString()).equals("test"))
        	println("test6 9 succeed");
        else 
        	println("test6 9 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("/test2/test[name()=\"test\"]");
        if (ap.evalXPathToBoolean())
        	println("test6 10 succeed");
        else 
        	println("test6 10 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.declareXPathNameSpace("abc", "xyz");
        ap.selectXPath("namespace-uri(/test2/abc:test)");
        println("namespace uri "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test6 11 succeed");
        else 
        	println("test6 11 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("/test2/abc:test[namespace-uri()='xyz']");
        //println("namespace uri "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test6 12 succeed");
        else 
        	println("test6 12 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("/test2/abc:test[local-name()='test']");
        //println("namespace uri "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test6 13 succeed");
        else 
        	println("test6 13 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("local-name(/test2/abc:test)");
        println("local name "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test6 14 succeed");
        else 
        	println("test6 14 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("local-name(/test2/abc:test)");
        println("local name "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test6 14 succeed");
        else 
        	println("test6 14 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("contains(/test2/test/text(), 'abc')");
        //println("  "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test6 15 succeed");
        else 
        	println("test6 15 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("contains(/test2/test3/text(), 'ABC')");
        //println("  "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test6 16 succeed");
        else 
        	println("test6 16 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("contains(/test2/test3/text(), '&lt;')");
        //println("  "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test6 17 succeed");
        else 
        	println("test6 17 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("contains(/test2/test3/text(), 'abc')");
        //println("  "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test6 18 succeed");
        else 
        	println("test6 18 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("contains(/test2/test3/text(), 'ABC')");
        //println("  "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test6 19 succeed");
        else 
        	println("test6 19 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("starts-with(/test2/test4/text(), '&lt;')");
        //println("  "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test6 20 succeed");
        else 
        	println("test6 20 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("ends-with(/test2/test4/text(), '&lt;')");
        //println("  "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test6 21 succeed");
        else 
        	println("test6 21 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("starts-with(/test2/test5/text(), '<')");
        //println("  "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test6 22 succeed");
        else 
        	println("test6 22 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("ends-with(/test2/test5/text(), '<')");
        //println("  "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test6 23 succeed");
        else 
        	println("test6 23 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("upper-case(/test2/test5/text())='< AZ AZ ABC <'");
        //println("  "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test6 24 succeed");
        else 
        	println("test6 24 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("lower-case(/test2/test5/text())='< az az abc <'");
        //println("  "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test6 25 succeed");
        else 
        	println("test6 25 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("upper-case(/test2/test4/text())='&LT; AZ AZ ABC &LT;'");
        //println("  "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test6 26 succeed");
        else 
        	println("test6 26 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("lower-case(/test2/test4/text())='&lt; az az abc &lt;'");
        //println("  "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test6 27 succeed");
        else 
        	println("test6 27 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    }
    
    public static void test66() throws Exception{
        String s = "<test2>"
            + "<e1 attr='1.1'>4</e1>" 
            + "<e1 attr='2.2'>44</e1>"
            + "<f1 attr='3'>1000</f1>" 
            + "<f1 attr='4' attr1='6' attr2='0.1'>10e5</f1>" 
            + "<test>abc az AZ ABC</test>"
            + "</test2>";
        VTDNavHuge vn = parseString1(s);
        AutoPilotHuge ap = new AutoPilotHuge(vn);
        ap.selectXPath("count(//@*)");
        double d = ap.evalXPathToNumber();
        if (d==6)
        	println("test66 1 succeed");
        else 
        	println("test66 1 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        ap.selectXPath("sum(//@*)");
        d = ap.evalXPathToNumber();
        if (((float)d)==16.4)
        	println("test66 2 succeed  " + (float)d);
        else 
        	println("test66 2 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  " +  (float)d);
        ap.selectXPath("upper-case(/test2/test)");
        String s1= ap.evalXPathToString();
        if (s1.equals("ABC AZ AZ ABC"))
        	println("test66 3 succeed   "+s1);
        else
        	println("test66 3 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!   "+s1);
        ap.selectXPath("lower-case(/test2/test)");
        s1= ap.evalXPathToString();
        if (s1.equals("abc az az abc"))
        	println("test66 4 succeed   "+s1);
        else
        	println("test66 4 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!   "+s1);
        ap.selectXPath("starts-with(/test2/test,'abc')");
        if (ap.evalXPathToBoolean())
        	println("test66 5 succeed");
        else
        	println("test66 5 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("ends-with(/test2/test,'ABC')");
        if (ap.evalXPathToBoolean())
        	println("test66 6 succeed");
        else
        	println("test66 6 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        s = "<test2>"
            + "<e1 attr='1.1'>4</e1>" 
            + "<e1 attr='2.2'>44</e1>"
            + "<f1 attr='3'>1000</f1>" 
            + "<f1 attr='4' attr1='6' attr2='0.1'>10e5</f1>" 
            + "<test xml:lang='en'>abc az AZ ABC</test>"
            + "<test3 ><![CDATA[abc &lt; az AZ ABC]]></test3>"
            + "<test4 ><![CDATA[&lt; az AZ ABC &lt;]]></test4>"
            + "<test5 >&lt; az AZ ABC &lt;</test5>"
            + "<a:test xmlns:a='xyz'> xyz </a:test> "
            + "</test2>";
        vn = parseString1(s);
        ap = new AutoPilotHuge(vn);
        ap.selectXPath("/test2/test[lang('en')]");
        println(" expr is "+ap.getExprString());
        if( ap.evalXPathToBoolean())
        	println("test66 7 succeed ");
        else
        	println("test66 7 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ");
        
        ap.selectXPath("contains(/test2/test,'AZ ABC')");
        if (ap.evalXPathToBoolean())
        	println("test66 8 succeed");
        else
        	println("test66 8 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("name(/test2/test)");
        if ((ap.evalXPathToString()).equals("test"))
        	println("test66 9 succeed");
        else 
        	println("test66 9 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("/test2/test[name()=\"test\"]");
        if (ap.evalXPathToBoolean())
        	println("test66 10 succeed");
        else 
        	println("test66 10 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.declareXPathNameSpace("abc", "xyz");
        ap.selectXPath("namespace-uri(/test2/abc:test)");
        println("namespace uri "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test66 11 succeed");
        else 
        	println("test66 11 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("/test2/abc:test[namespace-uri()='xyz']");
        //println("namespace uri "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test66 12 succeed");
        else 
        	println("test66 12 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("/test2/abc:test[local-name()='test']");
        //println("namespace uri "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test66 13 succeed");
        else 
        	println("test66 13 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("local-name(/test2/abc:test)");
        println("local name "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test66 14 succeed");
        else 
        	println("test66 14 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("local-name(/test2/abc:test)");
        println("local name "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test66 14 succeed");
        else 
        	println("test66 14 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("contains(/test2/test/text(), 'abc')");
        if (ap.evalXPathToBoolean())
        	println("test66 15 succeed");
        else 
        	println("test66 15 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("contains(/test2/test3/text(), 'ABC')");
        //println("  "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test66 16 succeed");
        else 
        	println("test66 16 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("contains(/test2/test3/text(), '&lt;')");
        //println("  "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test66 17 succeed");
        else 
        	println("test66 17 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("contains(/test2/test3/text(), 'abc')");
        //println("  "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test66 18 succeed");
        else 
        	println("test66 18 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("contains(/test2/test3/text(), 'ABC')");
        //println("  "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test66 19 succeed");
        else 
        	println("test66 19 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("starts-with(/test2/test4/text(), '&lt;')");
        //println("  "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test66 20 succeed");
        else 
        	println("test66 20 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("ends-with(/test2/test4/text(), '&lt;')");
        //println("  "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test66 21 succeed");
        else 
        	println("test66 21 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("starts-with(/test2/test5/text(), '<')");
        //println("  "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test66 22 succeed");
        else 
        	println("test66 22 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("ends-with(/test2/test5/text(), '<')");
        //println("  "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test66 23 succeed");
        else 
        	println("test66 23 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("upper-case(/test2/test5/text())='< AZ AZ ABC <'");
        //println("  "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test66 24 succeed");
        else 
        	println("test66 24 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("lower-case(/test2/test5/text())='< az az abc <'");
        //println("  "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test66 25 succeed");
        else 
        	println("test66 25 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("upper-case(/test2/test4/text())='&LT; AZ AZ ABC &LT;'");
        //println("  "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test66 26 succeed");
        else 
        	println("test66 26 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        
        ap.selectXPath("lower-case(/test2/test4/text())='&lt; az az abc &lt;'");
        //println("  "+ ap.evalXPathToString());
        if (ap.evalXPathToBoolean())
        	println("test66 27 succeed");
        else 
        	println("test66 27 failed!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    }
    /** should spit error*/
    public static void test7(){
    	try {
    	String s = "  <test2>"
            + "<e1 attr='1.1'>4</e1>" 
            + "<e1 attr='2.2'>44</e1>"
            + "<f1 attr='3'>1000</f1>" 
            + "<f1 attr='4' attr1='6' attr2='0.1'>10e5</f1>" 
            + "<test ><![CDATA[abc az AZ ABC]]></test>"
            + "<test3>abc az AZ ABC</test3>"
            + "</test2>";
        VTDNav vn = parseString(s);
        AutoPilot ap = new AutoPilot(vn);
        ap.declareVariableExpr("good", "/test2/test");
        ap.declareVariableExpr("bad", "/*/*");
    	ap.selectXPath("local-name($good)");
    	println(ap.getExprString());
    	int i=-1;
    	println(" evalString ==>"+ap.evalXPathToString());
    	}catch (Exception e){
    		println(e.toString());
    	}
    	
    }
    
    
    public static void test77(){
    	try {
    	String s = "<test2>"
            + "<e1 attr='1.1'>4</e1>" 
            + "<e1 attr='2.2'>44</e1>"
            + "<f1 attr='3'>1000</f1>" 
            + "<f1 attr='4' attr1='6' attr2='0.1'>10e5</f1>" 
            + "<test ><![CDATA[abc az AZ ABC]]></test>"
            + "<test3>abc az AZ ABC</test3>"
            + "</test2>";
        VTDNavHuge vn = parseString1(s);
        AutoPilotHuge ap = new AutoPilotHuge(vn);
        ap.declareVariableExpr("good", "/test2/test");
        ap.declareVariableExpr("bad", "/*/*");
    	ap.selectXPath("local-name($good)");
    	println(ap.getExprString());
    	int i=-1;
    	println(" evalString ==>"+ap.evalXPathToString());
    	}catch (Exception e){
    		println(e.toString());
    	}
    	
    }
    	
    public static void main(String s[]) throws Exception {
    
        test1(); test11();
        test2(); test22();
        test3(); test33();
        test4(); test44();
        test5(); test55();
        
        test6(); test66();
        test7(); test77();
    }
    
}

