/*
 * Created on Nov 26, 2004
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
public class NavTestNS {
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
			int i= vn.parseInt(vn.getAttrVal("attr"));
			int i1 = 0;
			if(vn.toElement(VTDNav.FC)){ // to level 1
				//i1++;
				do {
					if (!vn.matchElementNS("level1","level1"))
						return false;
					if (vn.matchElementNS("top","level1"))
						return false;
					int j = vn.parseInt(vn.getAttrVal("attr"));
					int j1 = 0;
					if (vn.toElement(VTDNav.FC)){ // to level 2
						//j1++;
						do {
							if (!vn.matchElementNS("level2","level2"))
								return false;
							if (vn.matchElementNS("top","level2"))
								return false;
							int k = vn.parseInt(vn.getAttrVal("attr"));
							int k1 = 0;
							if (vn.toElement(VTDNav.FC)){ // level 3
								//k1++;
								do{
									if (!vn.matchElementNS("level3","level3"))
										return false;
									if (vn.matchElementNS("top","level3"))
										return false;
									int l = vn.parseInt(vn.getAttrVal("attr"));
									int l1 = 0;
									if (vn.toElement(VTDNav.FC)){ // level 4
										//l1++;
										do {
											if (!vn.matchElementNS("level4","level4"))
												return false;
											if (vn.matchElementNS("top","level4"))
												return false;
											int m = vn.parseInt(vn.getAttrVal("attr"));
											int m1 = 0;
											if (vn.toElement(VTDNav.FC)){ // level 5
												m1++;
												do {
													if (!vn.matchElement("level5"))
														return false;
													m1++;
												}while(vn.toElement(VTDNav.NS));
												vn.toElement(VTDNav.P);
											}
											if (m!=m1)
												return false;
											if (m!=0)
												return false;
											l1++;
										}while(vn.toElement(VTDNav.NS));
										vn.toElement(VTDNav.P);
									}
									if (l!=l1)
										return false;
									k1++;
								}while(vn.toElement(VTDNav.NS));
								vn.toElement(VTDNav.P);
							}
							if (k1 != k)
								return false;
							j1++;
						}while(vn.toElement(VTDNav.NS));
						vn.toElement(VTDNav.P);
					}
					if (j1 != j)
						return false;
					i1++;
				}while(vn.toElement(VTDNav.NS));				
			}
			if (i1!=i)
				return false;
			vn.toElement(VTDNav.ROOT);
			
			i= vn.parseInt(vn.getAttrVal("attr"));
			i1 = 0;
			if(vn.toElement(VTDNav.LC)){ // to level 1
				//i1++;
				do {
					if (!vn.matchElementNS("level1","level1"))
						return false;
					if (vn.matchElementNS("top","level1"))
						return false;
					int j = vn.parseInt(vn.getAttrVal("attr"));
					int j1 = 0;
					if (vn.toElement(VTDNav.LC)){ // to level 2
						//j1++;
						do {
							if (!vn.matchElementNS("level2","level2"))
								return false;
							if (vn.matchElementNS("top","level2"))
								return false;
							int k = vn.parseInt(vn.getAttrVal("attr"));
							int k1 = 0;
							if (vn.toElement(VTDNav.LC)){ // level 3
								//k1++;
								do{
									if (!vn.matchElementNS("level3","level3"))
										return false;
									if (vn.matchElementNS("top","level3"))
										return false;
									int l = vn.parseInt(vn.getAttrVal("attr"));
									int l1 = 0;
									if (vn.toElement(VTDNav.LC)){ // level 4
										//l1++;
										do {	
											if (!vn.matchElementNS("level4","level4"))
												return false;
											if (vn.matchElementNS("top","level4"))
												return false;
											int m = vn.parseInt(vn.getAttrVal("attr"));
											int m1 = 0;
											if (vn.toElement(VTDNav.LC)){ // level 5
												m1++;
												do {
													if (!vn.matchElement("level5"))
														return false;
													m1++;
												}while(vn.toElement(VTDNav.PS));
												vn.toElement(VTDNav.P);
											}
											if (m!=m1)
												return false;
											if (m!=0)
												return false;
											l1++;
										}while(vn.toElement(VTDNav.PS));
										vn.toElement(VTDNav.P);
									}
									if (l!=l1)
										return false;
									k1++;
								}while(vn.toElement(VTDNav.PS));
								vn.toElement(VTDNav.P);
							}
							if (k1 != k)
								return false;
							j1++;
						}while(vn.toElement(VTDNav.PS));
						vn.toElement(VTDNav.P);
					}
					if (j1 != j)
						return false;
					i1++;
				}while(vn.toElement(VTDNav.PS));				
			}
			if (i1!=i)
				return false;
			return true;
		    //System.out.println("vtdgen ok");
		}catch (ParseException e){
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
/**
 * 
 * @param args
 */
	public static void main(String[] args) {
		String fileName = "d://ximple-dev//testcases//VTDNav//"+args[0];
       if (test(fileName)){
        	System.out.println(fileName+" passed!");
        }
       else
       	 System.out.println(fileName + " failed!");
		//if (test(fileName))
        //System.out.println("passed !");
	}
}
