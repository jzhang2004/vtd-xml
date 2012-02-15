package DOMTest;
import com.ximpleware.*;
import com.ximpleware.extended.*;
import java.io.*;


public class test8{
	
	public static void main1(String[] sv) throws Exception{
		String s = "<abc.abc>text </abc.abc>";
		VTDGen vg =  new VTDGen();
		vg.setDoc(s.getBytes());
		vg.parse(false);
		VTDNav vn = vg.getNav();
		
		if (vn.compareTokenString(2, "text")!=0){
			System.out.println("good!  " + vn.toString(2));
		}
		AutoPilot ap = new AutoPilot(vn);
		//ap.selectXPath("//*[text()!='text']");
		ap.selectXPath("/abc.abc");
		int i=0;
		while((i=ap.evalXPath())!=-1){
			System.out.println("good!2 ====> "+i);
		}
		ap.selectXPath("child::abc.abc");
		System.out.println(" name "+ap.getExprString());
		// create a file big than 1G
		/*FileOutputStream fos = new FileOutputStream("text.txt");
		// fill it with some bits
		byte[] ba = new byte[1<<20];
		for (int z=0;z<1<<20;z++)
			ba[z] = (byte)'x';
		System.out.println("finished it");
		for (int k=0;k<1048;k++){
			fos.write(ba);
		}
		fos.close();
		//FileInputStream fis = new FileInputStream("text.txt");
		
		// instantiate xmlBuffer to read it all in
		XMLBuffer xb = new XMLBuffer();
		xb.readFile("text.txt");
		System.out.print("length ==>" + xb.length());
		System.out.print("last byte is "+ (char)xb.byteAt(xb.length()-1));*/
		
	}
	
	public static void main(String[] args) {
		try {
			System.out.println("1");
			test(256);
		} catch (Exception e) {
			e.printStackTrace(System.out);
		}

		try {
			System.out.println("2");
			test(512);
		} catch (Exception e) {
			e.printStackTrace(System.out);
		}

		try {
			System.out.println("3");
			test(1024);
		} catch (Exception e) {
			e.printStackTrace(System.out);
		}

		try {
			System.out.println("4");
			test(2048);
		} catch (Exception e) {
			e.printStackTrace(System.out);
		}
	}

	public static void test(int SIZE) throws Exception {
		String xml = "<?xml version='1.0' encoding='UTF-8' ?><root>";
		for (int i = 1; i <= SIZE; i++) {
			xml += "<aaa>0</aaa>";
		}
		xml += "</root>";

		VTDGen generator = new VTDGen();
		generator.setDoc(xml.getBytes());
		generator.parse(true);

		VTDNav navigator = generator.getNav();
		AutoPilot pilot = new AutoPilot(navigator);
		XMLModifier modifier = new XMLModifier(navigator);

		for (int i = 1; i <= SIZE; i++) {
			pilot.resetXPath();
			pilot.selectXPath("/root/aaa[" + i + "]");
			pilot.evalXPath();

			modifier.remove();
			modifier.insertAfterElement("<aaa />");
		}

		ByteArrayOutputStream out = new ByteArrayOutputStream();
		modifier.output(out);
		xml = new String(out.toByteArray(), "UTF-8");
		out.close();
	}
	
}
