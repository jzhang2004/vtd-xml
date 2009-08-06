package DOMTest;
import com.ximpleware.*;
public class XPathTester {
    public static void println(String s) {
        System.out.println(s);
    }

    public static VTDNav parseString(String s) throws Exception {
        VTDGen vg = new VTDGen();
        vg.setDoc(s.getBytes());
        vg.parse(true);
        return vg.getNav();
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
                println("test1 1 failed");
            else 
                println("test1 1 succeed");
        }
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
            println("test2 1 failed");
        
        ap.selectXPath("/test2/e1!=/test2/f1");
        if (ap.evalXPathToBoolean())
            println("test2 2 succeed");
        else
            println("test2 2 failed");
        
        ap.selectXPath("/test2/e1 > /test2/f1");
        
        if (ap.evalXPathToBoolean())
            println("test2 3 succeed");
        else
            println("test2 3 failed");
        
        ap.selectXPath("/test2/e1 >= /test2/f1");
        
        if (ap.evalXPathToBoolean())
            println("test2 4 succeed");
        else
            println("test2 4 failed");
        
        ap.selectXPath("/test2/e1 <= /test2/f1");
        
        if (ap.evalXPathToBoolean())
            println("test2 5 succeed");
        else
            println("test2 5 failed");
        
        ap.selectXPath("/test2/e1 < /test2/f1");
        
        if (ap.evalXPathToBoolean())
            println("test2 6 succeed");
        else
            println("test2 6 failed");
        
        ap.selectXPath("/test2/e1 < /test2/e1");
        
        if (ap.evalXPathToBoolean())
            println("test2 7 succeed");
        else
            println("test2 7 failed");
        
        ap.selectXPath("/test2/e1 <= /test2/e1");
        
        if (ap.evalXPathToBoolean())
            println("test2 8 succeed");
        else
            println("test2 8 failed");
        
        ap.selectXPath("/test2/e1 > /test2/e1");
        
        if (ap.evalXPathToBoolean())
            println("test2 9 succeed");
        else
            println("test2 9 failed");
        
        ap.selectXPath("/test2/e1 >= /test2/e1");
        
        if (ap.evalXPathToBoolean())
            println("test2 10 succeed");
        else
            println("test2 10 failed");
        
        ap.selectXPath("/test2/e1 = /test2/e1");
        
        if (ap.evalXPathToBoolean())
            println("test2 11 succeed");
        else
            println("test2 11 failed");
        
        ap.selectXPath("/test2/e1 != /test2/e1");
        
        if (ap.evalXPathToBoolean())
            println("test2 12 succeed");
        else
            println("test2 12 failed");
        
        ap.selectXPath("/test2/*[starts-with(name(.),'e')] !=/test2/f1");
        
        if (ap.evalXPathToBoolean())
            println("test2 13 succeed");
        else
            println("test2 13 failed");
        
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
                println("test3 1 failed");

            ap.selectXPath("/test2/*/@*=4");
            if (ap.evalXPathToBoolean())
                println("test3 2 succeed");
            else
                println("test3 2 failed");

            ap.selectXPath("/test2/*/@* >4");
            if (ap.evalXPathToBoolean())
                println("test3 3 succeed");
            else
                println("test3 3 failed");

            ap.selectXPath("/test2/*/@* <=6");
            if (ap.evalXPathToBoolean())
                println("test3 4 succeed");
            else
                println("test3 4 failed");

            ap.selectXPath("/test2/*/@* <4 ");
            if (ap.evalXPathToBoolean())
                println("test3 5 succeed");
            else
                println("test3 5 failed");

            ap.selectXPath("/test2/*/@* <=6");
            if (ap.evalXPathToBoolean())
                println("test3 6 succeed");
            else
                println("test3 6 failed");

            ap.selectXPath("/test2/*/@* =6");
            if (ap.evalXPathToBoolean())
                println("test3 7 succeed");
            else
                println("test3 7 failed");

            ap.selectXPath("/test2/*/@* =4");
            if (ap.evalXPathToBoolean())
                println("test3 8 succeed");
            else
                println("test3 8 failed");

            ap.selectXPath("/test2/* <=1000");
            if (ap.evalXPathToBoolean())
                println("test3 9 succeed");
            else
                println("test3 9 failed");

            ap.selectXPath("/test2/* =1000");
            if (ap.evalXPathToBoolean())
                println("test3 10 succeed");
            else
                println("test3 10 failed");

            ap.selectXPath("/test2/* !=1000");
            if (ap.evalXPathToBoolean())
                println("test3 11 succeed");
            else
                println("test3 11 failed");

            ap.selectXPath("/test2/* >=1000");
            if (ap.evalXPathToBoolean())
                println("test3 12 succeed");
            else
                println("test3 12 failed");

            ap.selectXPath("/test2/* >1000");
            if (ap.evalXPathToBoolean())
                println("test3 13 succeed");
            else
                println("test3 13 failed");

            ap.selectXPath("/test2/* > count(/test2/*/@*[. > 1]) ");
            if (ap.evalXPathToBoolean())
                println("test3 14 succeed");
            else
                println("test3 14 failed");

            ap.selectXPath("/test2/* = count(/test2/*/@*[. > 1]) ");
            if (ap.evalXPathToBoolean())
                println("test3 15 succeed");
            else
                println("test3 15 failed");

            ap.selectXPath("/test2/* < count(/test2/*/@*[. > 1]) ");
            if (!ap.evalXPathToBoolean())
                println("test3 16 succeed");
            else
                println("test3 16 failed");

            ap.selectXPath("/test2/* > count(/test2/*/@*[1 < .]) ");
            if (ap.evalXPathToBoolean())
                println("test3 17 succeed");
            else
                println("test3 17 failed");

            ap.selectXPath("/test2/* = count(/test2/*/@*[1< .]) ");
            if (ap.evalXPathToBoolean())
                println("test3 18 succeed");
            else
                println("test3 18 failed");

            ap.selectXPath("/test2/* < count(/test2/*/@*[1<.]) ");
            if (!ap.evalXPathToBoolean())
                println("test3 19 succeed");
            else
                println("test3 19 failed");

            ap.selectXPath("  count(/test2/*/@*[1 < .])< /test2/* ");
            if (ap.evalXPathToBoolean())
                println("test3 20 succeed");
            else
                println("test3 20 failed");

            ap.selectXPath(" count(/test2/*/@*[1 < .])= /test2/* ");
            if (ap.evalXPathToBoolean())
                println("test3 21 succeed");
            else
                println("test3 21 failed");

            ap.selectXPath(" count(/test2/*/@*[1 < .])> /test2/* ");
            if (!ap.evalXPathToBoolean())
                println("test3 22 succeed");
            else
                println("test3 22 failed");     
       
        
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
                println("test4 1 failed");

            ap.selectXPath("/test2/*/@* ='4'");
            if (ap.evalXPathToBoolean())
                println("test4 2 succeed");
            else
                println("test4 2 failed");

            ap.selectXPath("/test2/*/@* >='4'");
            if (ap.evalXPathToBoolean())
                println("test4 3 succeed");
            else
                println("test4 3 failed");

            ap.selectXPath("/test2/*/@* >'4'");
            if (ap.evalXPathToBoolean())
                println("test4 4 succeed");
            else
                println("test4 4 failed");

            ap.selectXPath("/test2/*/@* <'4'");
            if (ap.evalXPathToBoolean())
                println("test4 5 succeed");
            else
                println("test4 5 failed");

            ap.selectXPath("/test2/*/@* <='4'");
            if (ap.evalXPathToBoolean())
                println("test4 6 succeed");
            else
                println("test4 6 failed");

            ap.selectXPath("/test2/*/@* <='4'");
            if (ap.evalXPathToBoolean())
                println("test4 7 succeed");
            else
                println("test4 7 failed");

            ap.selectXPath("/test2/*/@* =''");
            if (ap.evalXPathToBoolean())
                println("test4 8 succeed");
            else
                println("test4 8 failed");

            ap.selectXPath("/test2/*/@* >=''");
            if (ap.evalXPathToBoolean())
                println("test4 9 succeed");
            else
                println("test4 9 failed");

            ap.selectXPath("not(/test2/*/@* ='')");
            if (!ap.evalXPathToBoolean())
                println("test4 10 succeed");
            else
                println("test4 10 failed");

            ap.selectXPath("not(/test2/*/@* ='')");
            if (!ap.evalXPathToBoolean())
                println("test4 11 succeed");
            else
                println("test4 11 failed");

            ap.selectXPath("not(/test2/*/@* ='')");
            if (!ap.evalXPathToBoolean())
                println("test4 12 succeed");
            else
                println("test4 12 failed");

            ap.selectXPath("/test2/*/@* = /test2/*");
            if (ap.evalXPathToBoolean())
                println("test4 13 succeed");
            else
                println("test4 13 failed");

            ap.selectXPath("/test2/*/@* >= /test2/*");
            if (ap.evalXPathToBoolean())
                println("test4 14 succeed");
            else
                println("test4 14 failed");

            ap.selectXPath("/test2/*/@* <= /test2/*");
            if (ap.evalXPathToBoolean())
                println("test4 15 succeed");
            else
                println("test4 15 failed");


            ap.selectXPath("'4' = /test2/*/@* ");
            if (ap.evalXPathToBoolean())
                println("test4 16 succeed");
            else
                println("test4 16 failed");

            ap.selectXPath(" '4' = /test2/*/@* ");
            if (ap.evalXPathToBoolean())
                println("test4 17 succeed");
            else
                println("test4 17 failed");

            ap.selectXPath("'4' <= /test2/*/@* ");
            if (ap.evalXPathToBoolean())
                println("test4 18 succeed");
            else
                println("test4 18 failed");

            ap.selectXPath("'4' < /test2/*/@* ");
            if (ap.evalXPathToBoolean())
                println("test4 19 succeed");
            else
                println("test4 19 failed");

            ap.selectXPath("'4' > /test2/*/@* ");
            if (ap.evalXPathToBoolean())
                println("test4 20 succeed");
            else
                println("test4 20 failed");

            ap.selectXPath("/test2/*/@* <='4'");
            if (ap.evalXPathToBoolean())
                println("test4 21 succeed");
            else
                println("test4 21 failed");

            ap.selectXPath("/test2/*/@* <='4'");
            if (ap.evalXPathToBoolean())
                println("test4 22 succeed");
            else
                println("test4 22 failed");

            ap.selectXPath("/test2/*/@* =''");
            if (ap.evalXPathToBoolean())
                println("test4 23 succeed");
            else
                println("test4 23 failed");

            ap.selectXPath("/test2/*/@* >=''");
            if (ap.evalXPathToBoolean())
                println("test4 24 succeed");
            else
                println("test4 24 failed");

            ap.selectXPath("not(/test2/*/@* ='')");
            if (!ap.evalXPathToBoolean())
                println("test4 25 succeed");
            else
                println("test4 25 failed");

            ap.selectXPath("not( '' = /test2/*/@*)");
            if (!ap.evalXPathToBoolean())
                println("test4 26 succeed");
            else
                println("test4 26 failed");

            ap.selectXPath("not(/test2/*/@* ='')");
            if (!ap.evalXPathToBoolean())
                println("test4 27 succeed");
            else
                println("test4 27 failed");

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
            println("test5 1 failed: " + value);
        
        ap.selectXPath("translate('12:30','0123','abc')");
        value = ap.evalXPathToString();
        if (value.equals("bc:a"))
            println("test5 2 succeed");
        else
            println("test5 2 failed: " + value);
        
        ap.selectXPath("translate('','','abc')");
        value = ap.evalXPathToString();
        if (value.equals(""))
            println("test5 3 succeed");
        else
            println("test5 3 failed: " + value);
    }
    
    public static void main(String s[]) throws Exception {
        test1();
        test2();
        test3();
        test4();
        test5();
    }
    
}

