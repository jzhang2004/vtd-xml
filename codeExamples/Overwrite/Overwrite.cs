using System;
using System.Collections.Generic;
using System.Text;
using com.ximpleware;
// This example shows you how to overwrite a token using 
// VTDNav's overwrite method call
// The basic concept is that if the replacement value is
// shorter than the original token, then you can directly
// write the replacement bytes into the XML message and
// the original VTD structure doesn't change at all!!!
namespace Overwrite
{
    class Overwrite
    {
        public static byte[] getBytes(String s)
        {
            byte[] ba = new byte[s.Length];
            for (int i = 0; i < ba.Length; i++)
            {
                ba[i] = (byte)s[i];
            }
            return ba;

        }
        static void Main(string[] args)
        {
            	VTDGen vg = new VTDGen();
		        vg.setDoc(getBytes("<root>good</root>"));
		        vg.parse(true);
		        VTDNav vn = vg.getNav();
		        int i=vn.getText();
		        //print "good"
		        Console.WriteLine("text ---> "+vn.toString(i));
		        if (vn.overWrite(i,getBytes("bad"))){
			        //overwrite, if successful, returns true
			        //print "bad" here 
			         Console.WriteLine("text ---> "+vn.toString(i));
		        }
        }
    }
}

