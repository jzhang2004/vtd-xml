/*
 * generate random xml with namespaces
 */
package DOMTest;

import java.io.File;
import java.io.FileOutputStream;
import java.io.FileWriter;


public class xmlGen3 {
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
	f21,f22,f23,f24,f25,f26,f27,f28,f29,f30;
	public static FileWriter fw;
	public static FileOutputStream fos0, fos1, fos2, fos3, fos4,fos5,fos6,fos7,fos8,fos9,fos10,fos11,
	fos12,fos13,fos14,fos15,fos16,fos17,fos18,fos19,fos20,fos21,fos22,fos23,fos24,fos25,fos26,fos27,
	fos28,fos29,fos30;
    public static String s;
	public static void composeXML2(String xml1, int i, String msg) throws Exception {
		byte[] ba0 = null, ba1 = null, ba2 = null, ba3 = null, ba4 = null;
		ba0 = (enc_ascii+"<!--"+ msg+"-->" + xml1).getBytes("ascii");
		ba1 = (enc_8859 +"<!--"+ msg+"-->" + xml1).getBytes("iso-8859-1");
		ba2 = (enc_utf_8+"<!--"+ msg+"-->" + xml1).getBytes("utf-8");
		ba3 = (enc_utf_16le+"<!--"+ msg+"-->" + xml1).getBytes("utf-16le");
		ba4 = (enc_utf_16be+"<!--"+ msg+"-->" + xml1).getBytes("utf-16be");

		
		fw.write("nav_" + i
				+ "_*.xml "+"<!--"+ msg+"-->"+"\n");
		fw.flush();
		f0 = new File(testDir1 + "nt_" + i + "_0.xml");
		f1 = new File(testDir1 + "nt_" + i + "_1.xml");
		f2 = new File(testDir1 + "nav_" + i + "_2.xml");
		f3 = new File(testDir1 + "nav_" + i + "_3.xml");
		f4 = new File(testDir1 + "nav_" + i + "_4.xml");

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
		f0 = new File(testDir1 + "nav_" + i + "_0.xml");
		f1 = new File(testDir1 + "nav_" + i + "_1.xml");
		f2 = new File(testDir1 + "nav_" + i + "_2.xml");
		f3 = new File(testDir1 + "nav_" + i + "_3.xml");
		f4 = new File(testDir1 + "nav_" + i + "_4.xml");

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
	
	public static String randomXML(int depth){
		String s = "<top xmlns:level1 ='top' xmlns:level2 = 'top' xmlns:level3='top' xmlns:level4 = 'top'" + compose(1,depth)+"</top>";
		return s; 
	}
	
	public static String compose(int i, int depth){
		int k = (int)(Math.random()*10);
		String s=" attr='"+k+"'>\n";
		if (i==depth){
			return " attr ='0'>";// xmlns:level1 ='top' xmlns:level2 = 'top' xmlns:level3='top' xmlns:level4 = 'top'>\n";
		}
		else {
			for(int z=0;z<k;z++){
				s = s+"<level"+i+":level"+i+" xmlns:level"+i+"='level"+i+"'"+compose(i+1,depth)+
				"</level"+i+":level"+i+">\n";
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
		    for (int i=40;i<80;i++){
		    	composeXML1(randomXML(7),i,"random structure of XML to test name space");
		    }
		    System.out.println("finished!");
		}
	    catch (Exception e) {
	    	e.printStackTrace();
	    	System.out.println("exception type" + e);
	    }
	}
}
