using System;
using System.Collections.Generic;
using System.Text;

using com.ximpleware;

namespace DOMTest
{
    class Program
    {
        public static void println(String s)
        {
            System.Console.WriteLine(s);
        }

        public static VTDNav parseString(String s)
        {
            VTDGen vg = new VTDGen();
            System.Text.ASCIIEncoding encoding = new System.Text.ASCIIEncoding();

            vg.setDoc(encoding.GetBytes(s));

            vg.parse(true);

            return vg.getNav();
        }

        static void Main(string[] args)
        {

            test5();

        }


        //test XPath function: translate
        public static void test5() 
        {
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
        if (value.Equals("12:45"))
            println("test5 1 succeed");
        else
            println("test5 1 failed: " + value);
        
        ap.selectXPath("translate('12:30','0123','abc')");
        value = ap.evalXPathToString();
        if (value.Equals("bc:a"))
            println("test5 2 succeed");
        else
            println("test5 2 failed: " + value);
        
        ap.selectXPath("translate('','','abc')");
        value = ap.evalXPathToString();
        if (value.Equals(""))
            println("test5 3 succeed");
        else
            println("test5 3 failed: " + value);
    }

    }
}
