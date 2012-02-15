/*
 * Created on Nov 22, 2004
 *
 * This file generates random structured XML
 */
package DOMTest;

import java.io.File;
import java.io.FileOutputStream;
import java.io.FileWriter;

/**
 * generate random XML without namespaces
 */


public class xmlGen2 {
	public final static String testDir1 = "c://benchmark_2.7//xml//";
		//"d://ximple-dev//testcases//VTDNav//";
	public final static String readme = testDir1 + "readme.txt";
	public final static String enc_ascii = "<?xml version='1.0' encoding=\"us-ascii\"?>";
	public final static String enc_8859 = "<?xml version='1.0' encoding=\"iso-8859-1\"?>";
	public final static String enc_utf_8 = "<?xml version='1.0' encoding=\"utf-8\"?>";
	public final static String enc_utf_16le = "<?xml version='1.0' encoding=\"utf-16le\"?>";
	public final static String enc_utf_16be = "<?xml version='1.0' encoding=\"utf-16be\"?>";
	public final static String enc_8859_2="<?xml version='1.0' encoding=\"iso-8859-2\"?>";
	public final static String enc_8859_3="<?xml version='1.0' encoding=\"iso-8859-3\"?>";
	public final static String enc_8859_4="<?xml version='1.0' encoding=\"iso-8859-4\"?>";
	public final static String enc_8859_5="<?xml version='1.0' encoding=\"iso-8859-5\"?>";
	public final static String enc_8859_6="<?xml version='1.0' encoding=\"iso-8859-6\"?>";
	public final static String enc_8859_7="<?xml version='1.0' encoding=\"iso-8859-7\"?>";
	public final static String enc_8859_8="<?xml version='1.0' encoding=\"iso-8859-8\"?>";
	public final static String enc_8859_9="<?xml version='1.0' encoding=\"iso-8859-9\"?>";
	public final static String enc_8859_10="<?xml version='1.0' encoding=\"iso-8859-10\"?>";
	public final static String enc_cp_1250="<?xml version='1.0' encoding=\"cp1250\"?>";
	public final static String enc_cp_1251="<?xml version='1.0' encoding=\"cp1251\"?>";
	public final static String enc_cp_1252="<?xml version='1.0' encoding=\"cp1252\"?>";
	public final static String enc_cp_1253="<?xml version='1.0' encoding=\"cp1253\"?>";
	public final static String enc_cp_1254="<?xml version='1.0' encoding=\"cp1254\"?>";
	public final static String enc_cp_1255="<?xml version='1.0' encoding=\"cp1255\"?>";
	public final static String enc_cp_1256="<?xml version='1.0' encoding=\"cp1256\"?>";
	public final static String enc_cp_1257="<?xml version='1.0' encoding=\"cp1257\"?>";
	public final static String enc_cp_1258="<?xml version='1.0' encoding=\"cp1258\"?>";
	public final static String enc_win_1250 = "<?xml version='1.0' encoding=\"windows-1250\"?>";
	public final static String enc_win_1251 = "<?xml version='1.0' encoding=\"windows-1251\"?>";
	public final static String enc_win_1252 = "<?xml version='1.0' encoding=\"windows-1252\"?>";
	public final static String enc_win_1253 = "<?xml version='1.0' encoding=\"windows-1253\"?>";
	public final static String enc_win_1254 = "<?xml version='1.0' encoding=\"windows-1254\"?>";
	public final static String enc_win_1255 = "<?xml version='1.0' encoding=\"windows-1255\"?>";
	public final static String enc_win_1256 = "<?xml version='1.0' encoding=\"windows-1256\"?>";
	public final static String enc_win_1257 = "<?xml version='1.0' encoding=\"windows-1257\"?>";
	public final static String enc_win_1258 = "<?xml version='1.0' encoding=\"windows-1258\"?>";
	public static File rf,f0,f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,
	f21,f22,f23,f24,f25,f26,f27,f28,f29,f30,f31;
	public static FileWriter fw;
	public static FileOutputStream fos0, fos1, fos2, fos3, fos4,fos5,fos6,fos7,fos8,fos9,fos10,fos11,
	fos12,fos13,fos14,fos15,fos16,fos17,fos18,fos19,fos20,fos21,fos22,fos23,fos24,fos25,fos26,fos27,
	fos28,fos29,fos30,fos31;
    public static String s;
	public static void composeXML2(String xml1, int i, String msg) throws Exception {
		byte[] ba0 = null, ba1 = null, ba2 = null, ba3 = null, ba4 = null,
		ba5=null, ba6=null, ba7=null, ba8=null, ba9=null,ba10=null, ba11=null,
		ba12=null, ba13=null, ba14=null, ba15=null, ba16=null, ba17=null, ba18=null,
		ba19=null, ba20=null, ba21=null, ba22=null, ba23=null, ba24=null, ba25=null,
		ba26=null, ba27=null, ba28=null, ba29=null, ba30=null;
		ba0 = (enc_ascii+"<!--"+ msg+"-->" + xml1).getBytes("ascii");
		ba1 = (enc_8859 +"<!--"+ msg+"-->" + xml1).getBytes("iso-8859-1");
		ba2 = (enc_utf_8+"<!--"+ msg+"-->" + xml1).getBytes("utf-8");
		ba3 = (enc_utf_16le+"<!--"+ msg+"-->" + xml1).getBytes("utf-16le");
		ba4 = (enc_utf_16be+"<!--"+ msg+"-->" + xml1).getBytes("utf-16be");

		ba5 = (enc_8859_2+"<!--"+ msg+"-->" + xml1).getBytes("iso-8859-2");
		ba6 = (enc_8859_3+"<!--"+ msg+"-->" + xml1).getBytes("iso-8859-3");
		ba7 = (enc_8859_4+"<!--"+ msg+"-->" + xml1).getBytes("iso-8859-4");
		ba8 = (enc_8859_5+"<!--"+ msg+"-->" + xml1).getBytes("iso-8859-5");
		ba9 = (enc_8859_6+"<!--"+ msg+"-->" + xml1).getBytes("iso-8859-6");
		ba10 = (enc_8859_7+"<!--"+ msg+"-->" + xml1).getBytes("iso-8859-7");
		ba11 = (enc_8859_8+"<!--"+ msg+"-->" + xml1).getBytes("iso-8859-8");
		ba12 = (enc_8859_9+"<!--"+ msg+"-->" + xml1).getBytes("iso-8859-9");
		ba13 = (enc_8859_10+"<!--"+ msg+"-->" + xml1).getBytes("iso-8859-10");
		ba14 = (enc_cp_1250+"<!--"+ msg+"-->" + xml1).getBytes("cp1250");
		ba15 = (enc_8859_2+"<!--"+ msg+"-->" + xml1).getBytes("iso-8859-2");
		ba16 = (enc_8859_2+"<!--"+ msg+"-->" + xml1).getBytes("iso-8859-2");
		
		fw.write("nav_" + i
				+ "_*.xml "+"<!--"+ msg+"-->"+"\n");
		fw.flush();
		f0 = new File(testDir1 + "nt_" + i + "_0.xml");
		f1 = new File(testDir1 + "nt_" + i + "_1.xml");
		f2 = new File(testDir1 + "nav_" + i + "_2.xml");
		f3 = new File(testDir1 + "nav_" + i + "_3.xml");
		f4 = new File(testDir1 + "nav_" + i + "_4.xml");

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

	public static void composeXML1(String xml1, int i, String msg) throws Exception {
		byte[] ba0 = null, ba1 = null, ba2 = null, ba3 = null, ba4 = null,
		ba5=null, ba6=null, ba7=null, ba8=null, ba9=null,ba10=null, ba11=null,
		ba12=null, ba13=null, ba14=null, ba15=null, ba16=null, ba17=null, ba18=null,
		ba19=null, ba20=null, ba21=null, ba22=null, ba23=null, ba24=null, ba25=null,
		ba26=null, ba27=null, ba28=null, ba29=null, ba30=null, ba31=null;
		ba0 = (enc_ascii+"<!--"+ msg+"-->" + xml1).getBytes("ascii");
		ba1 = (enc_8859 +"<!--"+ msg+"-->" + xml1).getBytes("iso-8859-1");
		ba2 = (enc_utf_8+"<!--"+ msg+"-->" + xml1).getBytes("utf-8");
		ba3 = (enc_utf_16le+"<!--"+ msg+"-->" + xml1).getBytes("utf-16le");
		ba4 = (enc_utf_16be+"<!--"+ msg+"-->" + xml1).getBytes("utf-16be");

		ba5 = (enc_8859_2+"<!--"+ msg+"-->" + xml1).getBytes("iso-8859-2");
		ba6 = (enc_8859_3+"<!--"+ msg+"-->" + xml1).getBytes("iso-8859-3");
		ba7 = (enc_8859_4+"<!--"+ msg+"-->" + xml1).getBytes("iso-8859-4");
		ba8 = (enc_8859_5+"<!--"+ msg+"-->" + xml1).getBytes("iso-8859-5");
		ba9 = (enc_8859_6+"<!--"+ msg+"-->" + xml1).getBytes("iso-8859-6");
		ba10 = (enc_8859_7+"<!--"+ msg+"-->" + xml1).getBytes("iso-8859-7");
		ba11 = (enc_8859_8+"<!--"+ msg+"-->" + xml1).getBytes("iso-8859-8");
		ba12 = (enc_8859_9+"<!--"+ msg+"-->" + xml1).getBytes("iso-8859-9");
		//ba13 = (enc_8859_10+"<!--"+ msg+"-->" + xml1).getBytes("iso-8859-10");
		ba14 = (enc_cp_1250+"<!--"+ msg+"-->" + xml1).getBytes("cp1250");
		ba15 = (enc_cp_1251+"<!--"+ msg+"-->" + xml1).getBytes("cp1251");
		ba16 = (enc_cp_1252+"<!--"+ msg+"-->" + xml1).getBytes("cp1252");
		ba17 = (enc_cp_1253+"<!--"+ msg+"-->" + xml1).getBytes("cp1253");
		ba18 = (enc_cp_1254+"<!--"+ msg+"-->" + xml1).getBytes("cp1254");
		ba19 = (enc_cp_1255+"<!--"+ msg+"-->" + xml1).getBytes("cp1255");
		ba20 = (enc_cp_1256+"<!--"+ msg+"-->" + xml1).getBytes("cp1256");
		ba21 = (enc_cp_1257+"<!--"+ msg+"-->" + xml1).getBytes("cp1257");
		ba22 = (enc_cp_1258+"<!--"+ msg+"-->" + xml1).getBytes("cp1258");
		
		ba23 = (enc_win_1250+"<!--"+ msg+"-->" + xml1).getBytes("cp1250");
		ba24 = (enc_win_1251+"<!--"+ msg+"-->" + xml1).getBytes("cp1251");
		ba25 = (enc_win_1252+"<!--"+ msg+"-->" + xml1).getBytes("cp1252");
		ba26 = (enc_win_1253+"<!--"+ msg+"-->" + xml1).getBytes("cp1253");
		ba27 = (enc_win_1254+"<!--"+ msg+"-->" + xml1).getBytes("cp1254");
		ba28 = (enc_win_1255+"<!--"+ msg+"-->" + xml1).getBytes("cp1255");
		ba29 = (enc_win_1256+"<!--"+ msg+"-->" + xml1).getBytes("cp1256");
		ba30 = (enc_win_1257+"<!--"+ msg+"-->" + xml1).getBytes("cp1257");
		ba31 = (enc_win_1258+"<!--"+ msg+"-->" + xml1).getBytes("cp1258");
	   // test id
		fw.write("nt_" + i
				+ "_*.xml "+"<!--"+ msg+"-->"+"\n");
		fw.flush();
		f0 = new File(testDir1 + "nav_" + i + "_0.xml");
		f1 = new File(testDir1 + "nav_" + i + "_1.xml");
		f2 = new File(testDir1 + "nav_" + i + "_2.xml");
		f3 = new File(testDir1 + "nav_" + i + "_3.xml");
		f4 = new File(testDir1 + "nav_" + i + "_4.xml");
		/*
		f5 = new File(testDir1 + "nav_" + i + "_5.xml");
		f6 = new File(testDir1 + "nav_" + i + "_6.xml");
		f7 = new File(testDir1 + "nav_" + i + "_7.xml");
		f8 = new File(testDir1 + "nav_" + i + "_8.xml");
		f9 = new File(testDir1 + "nav_" + i + "_9.xml");
		f10 = new File(testDir1 + "nav_" + i + "_10.xml");
		f11 = new File(testDir1 + "nav_" + i + "_11.xml");
		f12 = new File(testDir1 + "nav_" + i + "_12.xml");
		f14 = new File(testDir1 + "nav_" + i + "_14.xml");
		f15 = new File(testDir1 + "nav_" + i + "_15.xml");
		f16 = new File(testDir1 + "nav_" + i + "_16.xml");
		f17 = new File(testDir1 + "nav_" + i + "_17.xml");
		f18 = new File(testDir1 + "nav_" + i + "_18.xml");
		f19 = new File(testDir1 + "nav_" + i + "_19.xml");
		f20 = new File(testDir1 + "nav_" + i + "_20.xml");
		f21 = new File(testDir1 + "nav_" + i + "_21.xml");
		f22 = new File(testDir1 + "nav_" + i + "_22.xml");
		f23 = new File(testDir1 + "nav_" + i + "_23.xml");
		f24 = new File(testDir1 + "nav_" + i + "_24.xml");
		f25 = new File(testDir1 + "nav_" + i + "_25.xml");
		f26 = new File(testDir1 + "nav_" + i + "_26.xml");
		f27 = new File(testDir1 + "nav_" + i + "_27.xml");
		f28 = new File(testDir1 + "nav_" + i + "_28.xml");
		f29 = new File(testDir1 + "nav_" + i + "_29.xml");
		f30 = new File(testDir1 + "nav_" + i + "_30.xml");
		f31 = new File(testDir1 + "nav_" + i + "_31.xml");*/
	
		
		
		fos0 = new FileOutputStream(f0);
		fos1 = new FileOutputStream(f1);
		fos2 = new FileOutputStream(f2);
		fos3 = new FileOutputStream(f3);
		fos4 = new FileOutputStream(f4);
		/*
		fos5 = new FileOutputStream(f5);
		fos6 = new FileOutputStream(f6);
		fos7 = new FileOutputStream(f7);
		fos8 = new FileOutputStream(f8);
		fos9 = new FileOutputStream(f9);
		fos10 = new FileOutputStream(f10);
		fos11 = new FileOutputStream(f11);
		fos12 = new FileOutputStream(f12);
		//fos13 = new FileOutputStream(f13);
		fos14 = new FileOutputStream(f14);
		fos15 = new FileOutputStream(f15);
		fos16 = new FileOutputStream(f16);
		fos17 = new FileOutputStream(f17);
		fos18 = new FileOutputStream(f18);
		fos19 = new FileOutputStream(f19);
		fos20 = new FileOutputStream(f20);
		fos21 = new FileOutputStream(f21);
		fos22 = new FileOutputStream(f22);
		fos23 = new FileOutputStream(f23);
		fos24 = new FileOutputStream(f24);
		fos25 = new FileOutputStream(f25);
		fos26 = new FileOutputStream(f26);
		fos27 = new FileOutputStream(f27);
		fos28 = new FileOutputStream(f28);
		fos29 = new FileOutputStream(f29);
		fos30 = new FileOutputStream(f30);
		fos31 = new FileOutputStream(f31);*/


		fos0.write(ba0);
		fos1.write(ba1);
		fos2.write(ba2);
		fos3.write(ba3);
		fos4.write(ba4);
		/*fos5.write(ba5);
		fos6.write(ba6);
		fos7.write(ba7);
		fos8.write(ba8);
		fos9.write(ba9);
		fos10.write(ba10);
		fos11.write(ba11);
		fos12.write(ba12);
		//fos13.write(ba3);
		fos14.write(ba14);
		fos15.write(ba15);
		fos16.write(ba16);
		fos17.write(ba17);
		fos18.write(ba18);
		fos19.write(ba19);
		fos20.write(ba20);
		fos21.write(ba21);
		fos22.write(ba22);
		fos23.write(ba23);
		fos24.write(ba24);
		fos25.write(ba25);
		fos26.write(ba26);
		fos27.write(ba27);
		fos28.write(ba28);
		fos29.write(ba29);
		fos30.write(ba30);
		fos31.write(ba31);*/
	

		fos0.close();
		fos1.close();
		fos2.close();
		fos3.close();
		fos4.close();
	}  
	
	public static String randomXML(int depth){
		String s = "<top" + compose(1,depth)+"</top>";
		return s; 
	}
	
	public static String compose(int i, int depth){
		int k = (int)(Math.random()*10);
		String s=" attr='"+k+"'>\n";
		if (i==depth){
			return " attr ='0'>\n";
		}
		else {
			for(int z=0;z<k;z++){
				s = s+"<level"+i+compose(i+1,depth)+"</level"+i+">\n";
			}
		}
		return s;
	}

	public static void main(String[] args) {
		String[] s1={};
		try {
			// open readme file
			rf = new File(readme);
			fw = new FileWriter(rf);
		    for (int i=0;i<40;i++){
		    	composeXML1(randomXML(7),i,"random structure of XML");
		    }
		    System.out.println("finished!");
		}
	    catch (Exception e) {
	    	e.printStackTrace();
	    	System.out.println("exception type" + e);
	    }
	}
}
