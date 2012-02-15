package DOMTest;

import com.ximpleware.*;
import com.ximpleware.extended.*;
import java.io.*;
import java.nio.*;
import java.nio.channels.*;

public class test3 {

	/**
	 * @param args
	 */
	public static void main1(String[] args) throws Exception{
		FileInputStream fis = new FileInputStream("c:/xml/test1.txt");
		FileChannel fc1 = fis.getChannel();
		FileOutputStream fos2 = new FileOutputStream("c:/xml/test2.txt");
		
		FileChannel fc2 = fos2.getChannel();
		long l1 = fc2.transferFrom(fc1,0,9);
		System.out.println(" "+l1+"  bytes written");
		fc2.position(fc2.position()+9);
		long l2 = fc1.transferTo(0,9,fc2);
		System.out.println(" "+l2+"  bytes written");
		// TODO Auto-generated method stub
		VTDGen vg = new VTDGen();
		FileOutputStream fos = new FileOutputStream("c:/xml/segment.txt");
		
		if (vg.parseFile("c:/xml/test2.xml", false)){
			VTDNav vn = vg.getNav();
			AutoPilot ap = new AutoPilot(vn);
			XMLModifier xm = new XMLModifier(vn);
			ap.selectXPath("/root/*");
			int i;
			while((i=ap.evalXPath())!=-1){
				//xm.updateToken(i, "10000");
				long l = vn.getContentFragment();
				if (l!=-1)
					fos.write(vn.getXML().getBytes(), 
							(int)l,(int)(l>>32));
			}
			ap.resetXPath();			
			//xm.output("c:/xml/out.xml");
		}
		
		VTDGenHuge vgh = new VTDGenHuge();
		FileOutputStream fos1 = new FileOutputStream("c:/xml/segment2.txt");
		if (vgh.parseFile("c:/xml/test2.xml", true, VTDGenHuge.MEM_MAPPED)){
			VTDNavHuge vnh = vgh.getNav();
			AutoPilotHuge aph = new AutoPilotHuge(vnh);
			//XMLModifier xm = new XMLModifier(vn);
			aph.selectXPath("/root");
			int i;
			long[] l;
			while((i=aph.evalXPath())!=-1){
				//xm.updateToken(i, "10000");
				//ElementFragmentNsHuge e = vnh.getElementFragmentNs();
				//if (e!=null)
				//	e.writeToFileOutputStream(fos1);
				l=vnh.getContentFragment();
				if (l!=null)
					vnh.getXML().writeToFileOutputStream(fos1, l[0], l[1]);
			}
			aph.resetXPath();
		}
	}
		
	public static void main(String[] args) throws Exception {

	}

}
