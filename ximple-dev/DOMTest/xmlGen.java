/*
 * Created on Nov 5, 2004
 *
 * This Java file generates all test cases xml files.
 */
package DOMTest;

import java.io.*;

/**
 * @author jimmy zhang
 * 
 * This Java file generates bad part of the parsing test cases xml files.
 *  
 */
public class xmlGen {
	public final static String testDir1 = 
		"d://ximple-dev//testcases//VTDGen//bad//";
	public final static String readme = testDir1 + "readme.txt";
	public final static String enc_ascii = "<?xml version='1.0' encoding=\"us-ascii\"?>";
	public final static String enc_8859 = "<?xml version='1.0' encoding=\"iso-8859-1\"?>";
	public final static String enc_utf_8 = "<?xml version='1.0' encoding=\"utf-8\"?>";
	public final static String enc_utf_16le = "<?xml version='1.0' encoding=\"utf-16le\"?>";
	public final static String enc_utf_16be = "<?xml version='1.0' encoding=\"utf-16be\"?>";
	public static File rf,f0,f1,f2,f3,f4;
	public static FileWriter fw;
	public static FileOutputStream fos0, fos1, fos2, fos3, fos4;
	
	public static void composeXML1(String xml1, int i, String msg) throws Exception {
		byte[] ba0 = null, ba1 = null, ba2 = null, ba3 = null, ba4 = null;
		ba0 = (enc_ascii+"<!--"+ msg+"-->" + xml1).getBytes("ascii");
		ba1 = (enc_8859 +"<!--"+ msg+"-->" + xml1).getBytes("iso-8859-1");
		ba2 = (enc_utf_8+"<!--"+ msg+"-->" + xml1).getBytes("utf-8");
		ba3 = (enc_utf_16le+"<!--"+ msg+"-->" + xml1).getBytes("utf-16le");
		ba4 = (enc_utf_16be+"<!--"+ msg+"-->" + xml1).getBytes("utf-16be");

	   // test id
		fw.write("nt_" + i
				+ "_*.xml "+"<!--"+ msg+"-->"+"\n");
		fw.flush();
		f0 = new File(testDir1 + "nt_" + i + "_0.xml");
		f1 = new File(testDir1 + "nt_" + i + "_1.xml");
		f2 = new File(testDir1 + "nt_" + i + "_2.xml");
		f3 = new File(testDir1 + "nt_" + i + "_3.xml");
		f4 = new File(testDir1 + "nt_" + i + "_4.xml");

		fos0 = new FileOutputStream(f0);
		fos1 = new FileOutputStream(f1);
		fos2 = new FileOutputStream(f2);
		fos3 = new FileOutputStream(f3);
		fos4 = new FileOutputStream(f4);

		fos0.write(ba0);
		fos1.write(ba1);
		fos2.write(ba2);
		fos3.write(ba3);
		fos4.write(ba4);

		fos0.close();
		fos1.close();
		fos2.close();
		fos3.close();
		fos4.close();
	}  
	
	public static void composeXML2(String xml1, int i, String msg) throws Exception {
		byte[] ba0 = null, ba1 = null, ba2 = null, ba3 = null, ba4 = null;
		//ba0 = (enc_ascii+"<!--"+ msg+"-->" + xml1).getBytes("ascii");
		//ba1 = (enc_8859 +"<!--"+ msg+"-->" + xml1).getBytes("iso-8859-1");
		ba2 = (enc_utf_8+"<!--"+ msg+"-->" + xml1).getBytes("utf-8");
		ba3 = (enc_utf_16le+"<!--"+ msg+"-->" + xml1).getBytes("utf-16le");
		ba4 = (enc_utf_16be+"<!--"+ msg+"-->" + xml1).getBytes("utf-16be");

		
		fw.write("nt_" + i
				+ "_*.xml "+"<!--"+ msg+"-->"+"\n");
		fw.flush();
		//f0 = new File(testDir1 + "nt_" + i + "_0.xml");
		//f1 = new File(testDir1 + "nt_" + i + "_1.xml");
		f2 = new File(testDir1 + "nt_" + i + "_2.xml");
		f3 = new File(testDir1 + "nt_" + i + "_3.xml");
		f4 = new File(testDir1 + "nt_" + i + "_4.xml");

		//fos0 = new FileOutputStream(f0);
		//fos1 = new FileOutputStream(f1);
		fos2 = new FileOutputStream(f2);
		fos3 = new FileOutputStream(f3);
		fos4 = new FileOutputStream(f4);

		//fos0.write(ba0);
		//fos1.write(ba1);
		fos2.write(ba2);
		fos3.write(ba3);
		fos4.write(ba4);

		//fos0.close();
		//fos1.close();
		fos2.close();
		fos3.close();
		fos4.close();		
	}
	
	public static void main(String[] args) {
		String[] xmls1 = {	
				"contains invalid char in character data for all encodingss",
				"<note>\n"
				+ "<to>Tove \u000b </to>\n"
				+ "<from>Jani</from>\n"
				+ "<heading>Reminder</heading>\n"
				+ "<body><?pi pifsfsfsf?>Don't forget me this weekend!</body>\n"
				+ "</note>",
				
				"contains invalid char in character data for utf-8 and utf-16",			
				"<note>\n" + "<to>Tove\u0019</to>\n" + "<from>Jani</from>\n"
				+ "<heading>Reminder</heading>\n"
				+ "<body>Don't forget me this weekend!</body>\n"
				+ "</note>",
				
				"contains invalid char in starting tag for all encodings",				
				"<note>\n" + "<1to>Tove</1to>\n" + "<from>Jani</from>\n"
				+ "<heading>Reminder</heading>\n"
				+ "<body>Don't forget me this weekend!</body>\n"
				+ "</note>",
				
				"contains invalid char in starting tag common to utf-8 and utf-16",				
				"<note>\n" + "<\u05f3to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading>Reminder</heading>\n"
				+ "<body>Don't forget me this weekend!</body>\n"
				+ "</note>",
				
				"contains invalid char in attr name for all encodings",				
				"<note>\n" + "<to 1at = '1234'>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading>Reminder</heading>\n"
				+ "<body>Don't forget me this weekend!</body>\n"
				+ "</note>",
					
				"contains invalid char in attr name common to utf-8 and utf-16",
				"<note>\n" + "<to at\u0d58 = '1234'>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading>Reminder</heading>\n"
				+ "<body>Don't forget me this weekend!</body>\n"
				+ "</note>",
							
				"contains invalid char in attr val for all encodings",				
				"<note a='\u0002'>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading>Reminder</heading>\n"
				+ "<body>Don't forget me this weekend!</body>\n"
				+ "</note>",
					
				"contains invalid char in attr val common to utf-8 and utf-16",
				"<note a='\u0018'>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading>Reminder</heading>\n"
				+ "<body>Don't forget me this weekend!</body>\n"
				+ "</note>",
				
				"contains invalid char in PI target for all encodings",				
				"<note>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading>Reminder</heading>\n"
				+ "<body><?1ab ?>Don't forget me this weekend!</body>\n"
				+ "</note>",
					
				"contains invalid char in PI target common to utf-8 and utf-16",
				"<note>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading><?\u1234asb fskfs?>Reminder</heading>\n"
				+ "<body>Don't forget me this weekend!</body>\n"
				+ "</note>",
				
				"contains invalid char in PI value for all encodings",				
				"<note>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading><?pi aaa\u0003?>Reminder</heading>\n"
				+ "<body>Don't forget me this weekend!</body>\n"
				+ "</note>",
					
				"contains invalid char in PI value common to utf-8 and utf-16",
				"<note>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading><?pi aaa\u0017?>Reminder</heading>\n"
				+ "<body>Don't forget me this weekend!</body>\n"
				+ "</note>",
				
				"contains invalid char in comment for all encodings",				
				"<note>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading><!--\u0002aaaa-->Reminder</heading>\n"
				+ "<body>Don't forget me this weekend!</body>\n"
				+ "</note>",
					
				"contains invalid char in comment common to utf-8 and utf-16",
				"<note>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading><!--\u0019aaaaaaaaaaaaa-->Reminder</heading>\n"
				+ "<body>Don't forget me this weekend!</body>\n"
				+ "</note>",
				
				"contains invalid char in CDATA for all encodings",				
				"<note>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading><![CDATA[aaaa\u0002]]>Reminder</heading>\n"
				+ "<body>Don't forget me this weekend!</body>\n"
				+ "</note>",
					
				"contains invalid char in CDATA common to utf-8 and utf-16",
				"<note>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading><![CDATA[aaaa\u0019]]>Reminder</heading>\n"
				+ "<body>Don't forget me this weekend!</body>\n"
				+ "</note>",
				
				"contains invalid char in comment at the end of xml for all encodings",				
				"<note>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading>Reminder</heading>\n"
				+ "<body>Don't forget me this weekend!</body>\n"
				+ "</note><!--\u0002aaaa-->",
					
				"contains invalid char in comment at the end of xml common to utf-8 and utf-16",
				"<note>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading>Reminder</heading>\n"
				+ "<body>Don't forget me this weekend!</body>\n"
				+ "</note><!--\u0019aaaaaaaaaaaaa-->",
				
				"contains invalid char in PI target at the end of xml for all encodings",				
				"<note>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading>Reminder</heading>\n"
				+ "<body>Don't forget me this weekend!</body>\n"
				+ "</note><?1ab ?>",
					
				"contains invalid char in PI target at the end of xml common to utf-8 and utf-16",
				"<note>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading>Reminder</heading>\n"
				+ "<body>Don't forget me this weekend!</body>\n"
				+ "</note><?\u1234asb fskfs?>",
				
				"contains invalid char in PI value at the end of doc for all encodings",				
				"<note>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading>Reminder</heading>\n"
				+ "<body>Don't forget me this weekend!</body>\n"
				+ "</note><?pi aaa\u0003?>",
					
				"contains invalid char in PI value at the end of doc common to utf-8 and utf-16",
				"<note>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading>Reminder</heading>\n"
				+ "<body>Don't forget me this weekend!</body>\n"
				+ "</note><?pi aaa\u0017?>",
				};
		
		
		// invalid char sequence
		String[] xmls2 = {				
				"contains invalid char sequence in Character data for all common encodings",				
				"<note>\n" + "<to>Tov]]>e</to>\n" + "<from>Jani</from>\n"
				+ "<heading>Reminder</heading>\n"
				+ "<body>Don't forget me this weekend!</body>\n"
				+ "</note>",
						
				"contains invalid char sequence in comment for all common encodings",				
				"<note>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading>Reminder</heading>\n"
				+ "<body>Don't <!-- abc -- cde -->forget me this weekend!</body>\n"
				+ "</note>",
				
				"contains invalid char sequence in CDATA for all common encodings",				
				"<note>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading>Reminder</heading>\n"
				+ "<![CDATA[abc]]>]]><body>Don't forget me this weekend!</body>\n"
				+ "</note>",
				};
		
		// structure problems
		String xmls3[] ={
				"starting ending tag mismatch 1",				
				"<note\r\t>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading>Reminder</heading>\n"
				+ "<![CDATA[abc]]><body>Don't forget me this weekend!</body>\n"
				+ "</nota>",	
				
				"starting ending tag mismatch 2",				
				"<note>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading>Reminder</heading>\n"
				+ "<![CDATA[abc]]><body>Don't forget me this weekend!</body>\n"
				+ "</not>",
				
				" ending tag missing 1",				
				"<note\r\t>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading>Reminder</heading>\n"
				+ "<![CDATA[abc]]><body>Don't forget me this weekend!</body>\n"
				,	
				
				"starting/ending tag missing",				
				"<note>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading/>Reminder</heading>\n"
				+ "<![CDATA[abc]]><body>Don't forget me this weekend!</body>\n"
				+ "</not>",
				
				"attr val not terminated properly 1",
				"<note>\n" + "<to a='b\">Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading>Reminder</heading>\n"
				+ "<![CDATA[abc]]><body>Don't forget me this weekend!</body>\n"
				+ "</note>",
				
				"attr val not terminated properly 2",
				"<note>\n" + "<to a>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading>Reminder</heading>\n"
				+ "<![CDATA[abc]]><body>Don't forget me this weekend!</body>\n"
				+ "</note>",
				
				"PI not terminated properly 1",
				"<note>\n" + "<to a='b>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading><?pi >Reminder</heading>\n"
				+ "<![CDATA[abc]]><body>Don't forget me this weekend!</body>\n"
				+ "</note>",
				
				"PI not terminated properly 2",
				"<note>\n" + "<to a='b>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading><?pi ? Reminder</heading>\n"
				+ "<![CDATA[abc]]><body>Don't forget me this weekend!</body>\n"
				+ "</note>",
				
				"CDATA not terminated properly 1",
				"<note>\n" + "<to >Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading> Reminder</heading>\n"
				+ "<![CDATA[abc]><body>Don't forget me this weekend!</body>\n"
				+ "</note>",
				
				"CDATA not terminated properly 2",
				"<note>\n" + "<to >Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading> Reminder</heading>\n"
				+ "<![CDATA[abc]] <body>Don't forget me this weekend!</body>\n"
				+ "</note>",
				
				"random error1",
				"<note>\n" + "<to >Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading = '1'> Reminder</heading>\n"
				+ "<![CDATA[abc]]> <body>Don't <forget me this weekend!</body>\n"
				+ "</note>",
				
				"random error2",
				"<note>\n" + "<to \n" + "<from>Jani</from>\n"
				+ "<heading = '1'> Reminder</heading>\n"
				+ "<![CDATA[abc]]> <body>Don't <forget me this weekend!</body>\n"
				+ "</note>",
				
				"random error3",
				"<note>\n" + "<to >Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading = '1'/heading>\n"
				+ "<![CDATA[abc]]> <body>Don't <forget me this weekend!</body>\n"
				+ "</note>",
				
				"random error4",
				"<note>\n" + "<to >Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading = '1'/heading>\n"
				+ "<![CDATA[abc]]> <body>Don't <forget me this weekend!</body>\n"
				+ "</note> <!----->",
				
				"random error5",
				"<note>\n" + "<to > <?fsb ? >Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading = '1'/heading>\n"
				+ "<![CDATA[abc]]> <body>Don't forget me this weekend!</body>\n"
				+ "</note>",
				
				"random error6",
				"<note>\n" + "<to >Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading a= '1'></heading>\n"
				+ "<![CDATA[abc]]> <body>Don't forget me this weekend!</body>\n"
				+ "</note> sjfksjkf",
				
				"random error7",
				"bad character data<note>\n" + "<to >Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading a= '1'></heading>\n"
				+ "<![CDATA[abc]]> <body>Don't forget me this weekend!</body>\n"
				+ "</note>",
				
				"random error8",
				" <![CDATA[abc]]><note><to>Tove</to>\n" +  "<from>Jani</from>\n"
				+ "<heading a= '1'></heading>\n"
				+ "<![CDATA[abc]]> <body>Don't forget me this weekend!</body>\n"
				+ "</note>",
				
				"random error9",
				" <?fsb ?>  <note>\n" + "<to >Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading a= '1'></heading>\n"
				+ "<![CDATA[abc]]> <body>Don't forget me this weekend!</body>\n"
				+ "</note><![CDATA[abc]]>",
		};
		
		// invalid entities
		String xmls4[] ={
				"invalid entity 1",				
				"<note\r\t>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading>Remi &#xffff;nder</heading>\n"
				+ "<![CDATA[abc]]><body>Don't forget me this weekend!</body>\n"
				+ "</note>",
				
				"invalid entity 2",				
				"<note\r\t>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading>Remi &#x19;nder</heading>\n"
				+ "<![CDATA[abc]]><body>Don't forget me this weekend!</body>\n"
				+ "</note>",
				
				"invalid entity 3",				
				"<note\r\t>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading>Remi &#x110000;nder</heading>\n"
				+ "<![CDATA[abc]]><body>Don't forget me this weekend!</body>\n"
				+ "</note>",
				
				"invalid entity 4",				
				"<note\r\t>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading>Remi &#123nder</heading>\n"
				+ "<![CDATA[abc]]><body>Don't forget me this weekend!</body>\n"
				+ "</note>",
				
				
		};
		
		// token length and depth overflow 
		
		StringBuffer sb0 = new StringBuffer(0xfffff+1);
		for (int z = 0;z<0xfffff+2;z++)
			sb0.append('a');
		String s0 = sb0.toString();
		
		sb0 = new StringBuffer();
		for (int z = 0;z<2049;z++)
			sb0.append('a');
		String s01= sb0.toString();
		
		sb0 = new StringBuffer();
		for (int z = 0;z<513;z++)
			sb0.append('a');
		sb0.append(":b");
		String s02= sb0.toString();
		
		StringBuffer sb1 = new StringBuffer();
		for(int z = 0;z<256;z++)
			sb1.append("<a>");
		for(int z = 0;z<256;z++)
			sb1.append("</a>");
		String s03 = sb1.toString();
		String s04 ="";
		for (int t=0;t<277;t++){
			
		  s04 = s04+" a"+t+"='abc'";
		}
		s04 = s04 + " a0='abc'";
		
		String s1 = sb1.toString();
		String[] xmls5 = {
				"PI name too long",
				"<note\r\t>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading> <?"+s0+"?></heading>\n"
				+ "<![CDATA[abc]]><body>Don't forget me this weekend!</body>\n"
				+ "</note>",
				
				"PI val too long",
				"<note\r\t>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading><?a1 "+s0+"?> </heading>\n"
				+ "<![CDATA[ab"+s0+"c]]><body>Don't forget me this weekend!</body>\n"
				+ "</note>",
				
				"attr val too long",
				"<note\r\t>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading attrname = '"+ s0 +"'> </heading>\n"
				+ "<![CDATA[abc]]><body>Don't forget me this weekend!</body>\n"
				+ "</note>",
				
				"attr name too long",
				"<note\r\t>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading "+ s01 +" = 'a'> </heading>\n"
				+ "<![CDATA[abc]]><body>Don't forget me this weekend!</body>\n"
				+ "</note>",
				
				"attr name prefix too long",
				"<note\r\t>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<heading "+ s02+"='afs'> </heading>\n"
				+ "<![CDATA[abc]]><body>Don't forget me this weekend!</body>\n"
				+ "</note>",
				
				"starting tag too long",
				"<note\r\t>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<"+ s01 +" attrname = '1'> </heading>\n"
				+ "<![CDATA[abc]]><body>Don't forget me this weekend!</body>\n"
				+ "</note>",
				
				"starting tag prefix too long",
				"<note\r\t>\n" + "<to>Tove</to>\n" + "<from>Jani</from>\n"
				+ "<"+s02 +" attrname = '1'> </heading>\n"
				+ "<![CDATA[abc]]><body>Don't forget me this weekend!</body>\n"
				+ "</note>",
								
				"depth overflow",
				s03,
				
				"nonunique attr name",
				"<abc"+ s04+">"+"</abc>",
				};
		
		String xml1;
		int i;
		try {
			// open readme file
			rf = new File(readme);
			fw = new FileWriter(rf);
			// generating xmls with illegal chars
			for (i=0;i<(xmls1.length>>2);i++){
				composeXML1(xmls1[i*4+1],2*i,xmls1[i*4]);
				composeXML2(xmls1[i*4+3],2*i+1,xmls1[i*4+2]);
			}
			// illegal char sequences
			int k = 2*i;
			fw.write("\n");
			System.out.println(" i val is "+i*2);
			for (i = 0;i<((xmls2.length)>>1);i++){
				composeXML1(xmls2[i*2+1],i+k,xmls2[i*2]);
			}
			
			// bad structures
			k = k + i;
			fw.write("\n");
			System.out.println(" i val is "+i);
			for (i = 0;i<((xmls3.length)>>1);i++){
				composeXML1(xmls3[i*2+1],i+k,xmls3[i*2]);
			}
			// invalid entities
			k = k + i;
			fw.write("\n");
			System.out.println(" i val is "+i);
			for (i = 0;i<((xmls4.length)>>1);i++){
				composeXML1(xmls4[i*2+1],i+k,xmls4[i*2]);
			}
			
			// various overflow
			k = k + i;
			fw.write("\n");
			System.out.println(" i val is "+i);
			for (i = 0;i<((xmls5.length)>>1);i++){
				composeXML1(xmls5[i*2+1],i+k,xmls5[i*2]);
			}
		} catch (Exception e) {
			e.printStackTrace();
			System.out.println("exception type" + e);
		}
	}
}