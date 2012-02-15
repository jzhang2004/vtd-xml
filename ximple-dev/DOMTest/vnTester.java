package DOMTest;
/**
 * This class tests various member methods of VTDNav 
 */
import com.ximpleware.*;
public class vnTester {
    public static void println(String s){
        System.out.println(s);
    }
    
    public static VTDNav parseString(String s) throws ParseException {
        VTDGen vg = new VTDGen();
        vg.setDoc(s.getBytes());
        vg.parse(true);
        return vg.getNav();        
    }
    // this function tests various matchs
    public static void test1() throws Exception{
        String s = "<test1> " 
            +"<a>good</a>" 
            +"<b>good&amp;&lt;&gt;&apos;&quot;</b>"
            +"<good>good</good>" 
            +"<c>&#97;&#98;&#99;&#100;&#101;&#102;&#103;</c>"
            +"<d>&#x61;&#x62;&#x63;&#x64;&#x65;&#x66;&#x67;&#x100;</d>"
            +"<e attr=''></e>"
            +"<b><![CDATA[good&amp;&lt;&gt;&apos;&quot;]]></b>"
            +"</test1>";
        VTDNav vn = parseString(s);
        vn.toElement(VTDNav.FIRST_CHILD);
        int i1 = vn.getText();
        int k = i1;
        if (!vn.matchRawTokenString(i1,"good")
                || !vn.matchTokenString(i1,"good"))
            println("test1 1 failed");
        else
            println("test1 1 succeed");
        vn.toElement(VTDNav.NEXT_SIBLING);
        int i2 = vn.getText();
        if (!vn.matchRawTokenString(i2,"good&amp;&lt;&gt;&apos;&quot;")
                || !vn.matchTokenString(i2,"good&<>'\""))
            println("test1 2 failed");
        else
            println("test1 2 succeed");
        vn.toElement(VTDNav.NEXT_SIBLING);
        i2 = vn.getText();
        if (!vn.matchTokens(i2,vn,i1))
            println("test1 3 failed");
        else 
            println("test1 3 succeed");
        vn.toElement(VTDNav.NEXT_SIBLING);
        i2 = vn.getText();
        if (!vn.matchRawTokenString(i2,"&#97;&#98;&#99;&#100;&#101;&#102;&#103;")
                || !vn.matchTokenString(i2,"abcdefg"))
            println("test1 4 failed");
        else 
            println("test1 4 succeed");
        vn.toElement(VTDNav.NEXT_SIBLING);
        i2 = vn.getText();
        if (!vn.matchRawTokenString(i2,"&#x61;&#x62;&#x63;&#x64;&#x65;&#x66;&#x67;&#x100;")
                || !vn.matchTokenString(i2,"abcdefg\u0100"))
            println("test1 5 failed");
        else 
            println("test1 5 succeed");
        vn.toElement(VTDNav.NEXT_SIBLING);
        i2 = vn.getAttrVal("attr");
        if (!vn.matchRawTokenString(i2,"")
                || !vn.matchTokenString(i2,""))
            println("test1 6 failed");
        else
            println("test1 6 succeed");
        vn.toElement(VTDNav.NEXT_SIBLING);
        i2 = vn.getText();
        if (vn.matchRawTokenString(i2,"good&amp;&lt;&gt;&apos;&quot;")
                && !vn.matchTokenString(i2,"good&<>"))
            println("test1 7 succeed");
        else
            println("test1 7 failed");
    } 
    
    // this function tests various comparison functions
    public static void test2() throws Exception{
        int i3;
        String s = "<test1> " 
            +"<a>good</a>" 
            +"<b>good&amp;&lt;&gt;&apos;&quot;</b>"
            +"<good>good</good>" 
            +"<c>&#97;&#98;&#99;&#100;&#101;&#102;&#103;</c>"
            +"<d>&#x61;&#x62;&#x63;&#x64;&#x65;&#x66;&#x67;&#x100;</d>"
            +"<e attr=''></e>"
            +"<b><![CDATA[good&amp;&lt;&gt;&apos;&quot;]]></b>"
            +"</test1>";
        VTDNav vn = parseString(s);
        vn.toElement(VTDNav.FIRST_CHILD);
        int i1 = vn.getText();
        int k = i1;
        if ((vn.compareRawTokenString(i1,"good")==0)
                && (vn.compareRawTokenString(i1,"good1")== -1)
                && (vn.compareRawTokenString(i1,"goo")==1)
                && (vn.compareTokenString(i1,"good")==0)
                && (vn.compareTokenString(i1,"good1")== -1)
                && (vn.compareTokenString(i1,"goo")==1))
            println("test2 1 succeed");
        else
            println("test2 1 failed");
        vn.toElement(VTDNav.NEXT_SIBLING);
        int i2 = vn.getText();
        if ((vn.compareRawTokenString(i2,"good&amp;&lt;&gt;&apos;&quot;")==0)
                && (vn.compareRawTokenString(i2,"good&amp;&lt;&gt;&apos;&quot")==1)
                && (vn.compareRawTokenString(i2,"good&amp;&lt;&gt;&apos;&quot;abd")==-1)
                && (vn.compareTokenString(i2,"good&<>'\"")==0)
                && (vn.compareTokenString(i2,"good&<>'")==1)
                && (vn.compareTokenString(i2,"good&<>'\"a")==-1)
                )            
            println("test2 2 succeed");
        else
            println("test2 2 failed");
        i3 = i2;
        vn.toElement(VTDNav.NEXT_SIBLING);
        i2 = vn.getText();
        if ((vn.compareTokens(i2,vn,i1)==0)
                && (vn.compareTokens(i2,vn,i3)== -1 )
                && (vn.compareTokens(i3,vn,i2)== 1)        	
        )
            println("test2 3 succeed");
        else 
            println("test2 3 failed");
        vn.toElement(VTDNav.NEXT_SIBLING);
        i2 = vn.getText();
        if ((vn.compareRawTokenString(i2,"&#97;&#98;&#99;&#100;&#101;&#102;&#103;")==0)
                && (vn.compareRawTokenString(i2,"&#97;&#98;&#99;&#100;&#101;&#102;&#103;a")==-1)
                && (vn.compareRawTokenString(i2,"&#97;&#98;&#99;&#100;&#101;&#102;&#103")==1)
                && (vn.compareTokenString(i2,"abcdefg")==0)
                && (vn.compareTokenString(i2,"abcdef")==1)
                && (vn.compareTokenString(i2,"abcdefg1")==-1))
            println("test2 4 succeed");
        else 
            println("test2 4 failed");
        vn.toElement(VTDNav.NEXT_SIBLING);
        i2 = vn.getText();
        //System.out.println(vn.toRawString(i2));
        if ((vn.compareRawTokenString(i2,"&#x61;&#x62;&#x63;&#x64;&#x65;&#x66;&#x67;&#x100;")==0)
                && (vn.compareRawTokenString(i2,"&#x61;&#x62;&#x63;&#x64;&#x65;&#x66;&#x67;&#x100;a")==-1)
                && (vn.compareRawTokenString(i2,"&#x61;&#x62;&#x63;&#x64;&#x65;&#x66;&#x67;&#x100")==1)
                && (vn.compareTokenString(i2,"abcdefg\u0100")==0)
                && (vn.compareTokenString(i2,"abcdefg")==1)
                && (vn.compareTokenString(i2,"abcdefg\u0101")==-1))
            println("test2 5 succeed");
        else 
            println("test2 5 failed");
        vn.toElement(VTDNav.NEXT_SIBLING);
        i2 = vn.getAttrVal("attr");
        if (vn.compareRawTokenString(i2,"")==0
                || vn.compareTokenString(i2,"a")==-1)
            println("test2 6 succeed");
        else
            println("test2 6 failed");
        vn.toElement(VTDNav.NEXT_SIBLING);
        i2 = vn.getText();
        if (vn.compareRawTokenString(i2,"good&amp;&lt;&gt;&apos;&quot;")==0
                && vn.compareTokenString(i2,"good&<>")!=0)
            println("test2 7 succeed");
        else
            println("test2 7 failed");
    }
    
    // this function tests various String conversion functions
    public static void test3() throws Exception{
        int i3;
        String s = "<test1> " 
            +"<a>good</a>" 
            +"<b>good&amp;&lt;&gt;&apos;&quot;</b>"
            +"<good>good</good>" 
            +"<c>&#97;&#98;&#99;&#100;&#101;&#102;&#103;</c>"
            +"<d>&#x61;&#x62;&#x63;&#x64;&#x65;&#x66;&#x67;&#x100;</d>"
            +"<e attr=''></e>"
            +"<b><![CDATA[good&amp;&lt;&gt;&apos;&quot;]]></b>"
            +"</test1>";
        VTDNav vn = parseString(s);
        vn.toElement(VTDNav.FIRST_CHILD);
        int i1 = vn.getText();
        int k = i1;
        if ((vn.toRawString(i1).compareTo("good")==0)
                && (vn.toRawString(i1).compareTo("good1")== -1)
                && (vn.toRawString(i1).compareTo("goo")==1)
                && (vn.toString(i1).compareTo("good")==0)
                && (vn.toString(i1).compareTo("good1")== -1)
                && (vn.toString(i1).compareTo("goo")==1))
            println("test3 1 succeed");
        else
            println("test3 1 failed");
        vn.toElement(VTDNav.NEXT_SIBLING);
        int i2 = vn.getText();
        String s1= vn.toRawString(i2);
        if ((vn.toRawString(i2).compareTo("good&amp;&lt;&gt;&apos;&quot;")==0)
                && (vn.toRawString(i2).compareTo("good&amp;&lt;&gt;&apos;&quot")>0)
                && (s1.compareTo("good&amp;&lt;&gt;&apos;&quot;abd")<0)
                && (vn.toString(i2).compareTo("good&<>'\"")==0)
                && (vn.toString(i2).compareTo("good&<>'")>0)
                && (vn.toString(i2).compareTo("good&<>'\"a")<0)
                )            
            println("test3 2 succeed");
        else
            println("test3 2 failed");
        i3 = i2;
        vn.toElement(VTDNav.NEXT_SIBLING);
        i2 = vn.getText();
        if ((vn.toString(i2).compareTo(vn.toString(i1))==0)
                && (vn.toString(i2).compareTo(vn.toString(i3))< 0)
                && (vn.toString(i3).compareTo(vn.toString(i2))> 0)        	
        )
            println("test3 3 succeed");
        else 
            println("test3 3 failed");
        vn.toElement(VTDNav.NEXT_SIBLING);
        i2 = vn.getText();
        if ((vn.compareRawTokenString(i2,"&#97;&#98;&#99;&#100;&#101;&#102;&#103;")==0)
                && (vn.compareRawTokenString(i2,"&#97;&#98;&#99;&#100;&#101;&#102;&#103;a")==-1)
                && (vn.compareRawTokenString(i2,"&#97;&#98;&#99;&#100;&#101;&#102;&#103")==1)
                && (vn.toString(i2).compareTo("abcdefg")==0)
                && (vn.toString(i2).compareTo("abcdef")>0)
                && (vn.toString(i2).compareTo("abcdefg1")<0))
            println("test3 4 succeed");
        else 
            println("test3 4 failed");
        vn.toElement(VTDNav.NEXT_SIBLING);
        i2 = vn.getText();
        //System.out.println(vn.toRawString(i2));
        if ((vn.compareRawTokenString(i2,"&#x61;&#x62;&#x63;&#x64;&#x65;&#x66;&#x67;&#x100;")==0)
                && (vn.compareRawTokenString(i2,"&#x61;&#x62;&#x63;&#x64;&#x65;&#x66;&#x67;&#x100;a")==-1)
                && (vn.compareRawTokenString(i2,"&#x61;&#x62;&#x63;&#x64;&#x65;&#x66;&#x67;&#x100")==1)
                && (vn.compareTokenString(i2,"abcdefg\u0100")==0)
                && (vn.compareTokenString(i2,"abcdefg")==1)
                && (vn.compareTokenString(i2,"abcdefg\u0101")==-1))
            println("test3 5 succeed");
        else 
            println("test3 5 failed");
        vn.toElement(VTDNav.NEXT_SIBLING);
        i2 = vn.getAttrVal("attr");
        if (vn.toString(i2).compareTo("")==0
                || vn.toString(i2).compareTo("a")<0)
            println("test3 6 succeed");
        else
            println("test3 6 failed");
        vn.toElement(VTDNav.NEXT_SIBLING);
        i2 = vn.getText();
        if (vn.toRawString(i2).compareTo("good&amp;&lt;&gt;&apos;&quot;")==0
                && vn.toString(i2).compareTo("good&<>")!=0)
            println("test3 7 succeed");
        else
            println("test3 7 failed");
    }
 
    // this function tests various Double, int conversion functions
    public static void test4() throws Exception{
        
    }
    public static void main(String args[]) throws Exception{
        test1();
        test2();
        test3();
        test4();
    }
}

