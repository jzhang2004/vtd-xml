/*
 * Created on Dec 2, 2004
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package DOMTest;
import java.io.File;
import java.io.FileInputStream;

import com.ximpleware.NavException;
import com.ximpleware.ParseException;
import com.ximpleware.VTDGen;
import com.ximpleware.VTDNav;

/**
 * @author jimmy zhang
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class MiscTest {
	public static boolean test(String s){
		try{
			VTDGen vg = new VTDGen();
			File f = new File(s);
			byte[] ba = new byte[(int)f.length()];
			FileInputStream fis = new FileInputStream(f);
			fis.read(ba);
			vg.setDoc(ba);
			vg.parse(true);
			VTDNav vn =vg.getNav();
			int i = vn.getText();
			if (vn.toElement(VTDNav.FIRST_CHILD)){
				if (vn.matchElement("float")){
					do {
						float f1 = Float.parseFloat(vn.toString(vn.getText()));
						float f2 = vn.parseFloat(vn.getText());
						if (f1 != f2)
							return false;						
					}while(vn.toElement(VTDNav.NS));
					return true;
				}else if (vn.matchElement("double")){
					do {
						double d1 = Double.parseDouble(vn.toString(vn.getText()));
						double d2 = vn.parseDouble(vn.getText());
						if (d1 != d2)
							return false;						
					}while(vn.toElement(VTDNav.NS));
					return true;
				}else if (vn.matchElement("int")){
					do {
						int i1 = Integer.parseInt(vn.toString(vn.getText()));
						int i2 = vn.parseInt(vn.getText());
						if (i1 != i2)
							return false;						
					}while(vn.toElement(VTDNav.NS));
					return true;
				}else if (vn.matchElement("long")){
					do {
						long l1 = Long.parseLong(vn.toString(vn.getText()));
						long l2 = vn.parseLong(vn.getText());
						if (l1 != l2)
							return false;						
					}while(vn.toElement(VTDNav.NS));
					return true;
				}
				return false;				
			}else {
				if (i==-1){
					i = vn.getCurrentIndex()+3;
					if (vn.getTokenType(i)== VTDNav.TOKEN_COMMENT
							|| vn.getTokenLength(i) == vn.parseInt(vn.getAttrVal("len")))
						return true;
						
					return false;
				}
				if (vn.getTokenType(i)== VTDNav.TOKEN_CDATA_VAL
						|| vn.getTokenType(i) == VTDNav.TOKEN_CHARACTER_DATA){
					int z = vn.getAttrVal("len");
					if (z!=-1){
						int i4 = vn.parseInt(z);
						if (i4!= vn.getTokenLength(i)){
							return false;
						}
						return true;
					}
					return true;
				}
				return false;			
			}
		}
		
		catch (ParseException e){
			System.out.println("ParserException: "+e);
			e.printStackTrace();
			return false;
		}catch (NavException e){
			System.out.println("NavException:"+e);
			return false;
		}
		catch (Exception e){
			e.printStackTrace();
			System.out.println("other exceptions");
			return false;
		}
		//return true;
	}
	public static boolean testIndex(String s){
		try{
			VTDGen vg = new VTDGen();
			File f = new File(s);
			byte[] ba = new byte[(int)f.length()];
			FileInputStream fis = new FileInputStream(f);
			fis.read(ba);
			vg.setDoc(ba);
			vg.parse(true);
			vg.writeIndex("tmp.vxl");
			vg = new VTDGen();
			vg.loadIndex("tmp.vxl");
			VTDNav vn =vg.getNav();
			int i = vn.getText();
			if (vn.toElement(VTDNav.FIRST_CHILD)){
				if (vn.matchElement("float")){
					do {
						float f1 = Float.parseFloat(vn.toString(vn.getText()));
						float f2 = vn.parseFloat(vn.getText());
						if (f1 != f2)
							return false;						
					}while(vn.toElement(VTDNav.NS));
					return true;
				}else if (vn.matchElement("double")){
					do {
						double d1 = Double.parseDouble(vn.toString(vn.getText()));
						double d2 = vn.parseDouble(vn.getText());
						if (d1 != d2)
							return false;						
					}while(vn.toElement(VTDNav.NS));
					return true;
				}else if (vn.matchElement("int")){
					do {
						int i1 = Integer.parseInt(vn.toString(vn.getText()));
						int i2 = vn.parseInt(vn.getText());
						if (i1 != i2)
							return false;						
					}while(vn.toElement(VTDNav.NS));
					return true;
				}else if (vn.matchElement("long")){
					do {
						long l1 = Long.parseLong(vn.toString(vn.getText()));
						long l2 = vn.parseLong(vn.getText());
						if (l1 != l2)
							return false;						
					}while(vn.toElement(VTDNav.NS));
					return true;
				}
				return false;				
			}else {
				if (i==-1){
					i = vn.getCurrentIndex()+3;
					if (vn.getTokenType(i)== VTDNav.TOKEN_COMMENT
							|| vn.getTokenLength(i) == vn.parseInt(vn.getAttrVal("len")))
						return true;
						
					return false;
				}
				if (vn.getTokenType(i)== VTDNav.TOKEN_CDATA_VAL
						|| vn.getTokenType(i) == VTDNav.TOKEN_CHARACTER_DATA){
					int z = vn.getAttrVal("len");
					if (z!=-1){
						int i4 = vn.parseInt(z);
						if (i4!= vn.getTokenLength(i)){
							return false;
						}
						return true;
					}
					return true;
				}
				return false;			
			}
		}
		
		catch (ParseException e){
			System.out.println("ParserException: "+e);
			e.printStackTrace();
			return false;
		}catch (NavException e){
			System.out.println("NavException:"+e);
			return false;
		}
		catch (Exception e){
			e.printStackTrace();
			System.out.println("other exceptions");
			return false;
		}
		//return true;
	}
	public static void main(String[] args) {
		String fileName = "d://ximple-dev//testcases//VTDNav//"+args[0];
	       if (test(fileName)){
	        	System.out.println(fileName+" passed!");
	        }
	}
}
