package DOMTest;

import java.io.File;
import java.io.FileInputStream;

import com.ximpleware.AutoPilot;
import com.ximpleware.FastIntBuffer;
import com.ximpleware.NavException;
import com.ximpleware.ParseException;
import com.ximpleware.VTDGen;
import com.ximpleware.VTDNav;

public class NavTestNS2 {
	public static boolean test(String s,int d){
		try{
			VTDGen vg = new VTDGen();
			vg.selectLcDepth(d);
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
												//m1++;
												do {
													if (!vn.matchElementNS("level5","level5"))
														return false;
													if (vn.matchElementNS("top","level5"))
														return false;
													int n = vn.parseInt(vn.getAttrVal("attr"));
													int n1 = 0;
													if (vn.toElement(VTDNav.FC)){ // level 5
														//m1++;
														do {
															if (!vn.matchElementNS("level6","level6"))
																return false;
															if (vn.matchElementNS("top","level6"))
																return false;
															int o = vn.parseInt(vn.getAttrVal("attr"));
															int o1 = 0;
															if (vn.toElement(VTDNav.FC)){ // level 5
																do{
																	o1++;
																}while(vn.toElement(VTDNav.NS));
																vn.toElement(VTDNav.P);
																if (o1!=0)
																	return false;
																if (o1!=o)
																	return false;
															}
															n1++;
														}while(vn.toElement(VTDNav.NS));
														vn.toElement(VTDNav.P);
													}
													if (n!=n1)
														return false;
														/*if (m!=0)
															return false;*/
													m1++;
												}while(vn.toElement(VTDNav.NS));
												vn.toElement(VTDNav.P);
											}
											if (m!=m1)
												return false;
											/*if (m!=0)
												return false;*/
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
												//m1++;
												do {
													if (!vn.matchElementNS("level5","level5"))
														return false;
													if (vn.matchElementNS("top","level5"))
														return false;
													int n = vn.parseInt(vn.getAttrVal("attr"));
													int n1 = 0;
													if (vn.toElement(VTDNav.LC)){ // level 5
														//m1++;
														do {
															if (!vn.matchElementNS("level6","level6"))
																return false;
															if (vn.matchElementNS("top","level6"))
																return false;
															int o = vn.parseInt(vn.getAttrVal("attr"));
															int o1 = 0;
															if (vn.toElement(VTDNav.LC)){ // level 5
																do{
																	o1++;
																}while(vn.toElement(VTDNav.PS));
																vn.toElement(VTDNav.P);
																if (o1!=0)
																	return false;
																if (o1!=o)
																	return false;
															}
															n1++;
														}while(vn.toElement(VTDNav.PS));
														vn.toElement(VTDNav.P);
													}
													if (n!=n1)
														return false;
													m1++;
												}while(vn.toElement(VTDNav.PS));
												vn.toElement(VTDNav.P);
											}
											if (m!=m1)
												return false;
											/*if (m!=0)
												return false;*/
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
	public static boolean testIndex(String s, int d){
			try{
				VTDGen vg = new VTDGen();
				vg.selectLcDepth(d);
				File f = new File(s);
				byte[] ba = new byte[(int)f.length()];
				FileInputStream fis = new FileInputStream(f);
				fis.read(ba);
				vg.setDoc(ba);
				vg.parse(true);
				vg.writeIndex("tmp.vxl");
				vg = new VTDGen();
				VTDNav vn = vg.loadIndex("tmp.vxl");
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
													//m1++;
													do {
														if (!vn.matchElementNS("level5","level5"))
															return false;
														if (vn.matchElementNS("top","level5"))
															return false;
														int n = vn.parseInt(vn.getAttrVal("attr"));
														int n1 = 0;
														if (vn.toElement(VTDNav.FC)){ // level 5
															//m1++;
															do {
																if (!vn.matchElementNS("level6","level6"))
																	return false;
																if (vn.matchElementNS("top","level6"))
																	return false;
																int o = vn.parseInt(vn.getAttrVal("attr"));
																int o1 = 0;
																if (vn.toElement(VTDNav.FC)){ // level 5
																	do{
																		o1++;
																	}while(vn.toElement(VTDNav.NS));
																	vn.toElement(VTDNav.P);
																	if (o1!=0)
																		return false;
																	if (o1!=o)
																		return false;
																}
																n1++;
															}while(vn.toElement(VTDNav.NS));
															vn.toElement(VTDNav.P);
														}
														if (n!=n1)
															return false;
															/*if (m!=0)
																return false;*/
														m1++;
													}while(vn.toElement(VTDNav.NS));
													vn.toElement(VTDNav.P);
												}
												if (m!=m1)
													return false;
												/*if (m!=0)
													return false;*/
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
													//m1++;
													do {
														if (!vn.matchElementNS("level5","level5"))
															return false;
														if (vn.matchElementNS("top","level5"))
															return false;
														int n = vn.parseInt(vn.getAttrVal("attr"));
														int n1 = 0;
														if (vn.toElement(VTDNav.LC)){ // level 5
															//m1++;
															do {
																if (!vn.matchElementNS("level6","level6"))
																	return false;
																if (vn.matchElementNS("top","level6"))
																	return false;
																int o = vn.parseInt(vn.getAttrVal("attr"));
																int o1 = 0;
																if (vn.toElement(VTDNav.LC)){ // level 5
																	do{
																		o1++;
																	}while(vn.toElement(VTDNav.PS));
																	vn.toElement(VTDNav.P);
																	if (o1!=0)
																		return false;
																	if (o1!=o)
																		return false;
																}
																n1++;
															}while(vn.toElement(VTDNav.PS));
															vn.toElement(VTDNav.P);
														}
														if (n!=n1)
															return false;
														m1++;
													}while(vn.toElement(VTDNav.PS));
													vn.toElement(VTDNav.P);
												}
												if (m!=m1)
													return false;
												/*if (m!=0)
													return false;*/
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
	}
	
	public static boolean testRecoverNode(String s, int d){
		try {
			VTDGen vg = new VTDGen();
			FastIntBuffer fib1 = new FastIntBuffer();
			FastIntBuffer fib2 = new FastIntBuffer();
			FastIntBuffer fib3 = new FastIntBuffer();
			vg.selectLcDepth(d);
			File f = new File(s);
			byte[] ba = new byte[(int) f.length()];
			FileInputStream fis = new FileInputStream(f);
			fis.read(ba);
			vg.setDoc(ba);
			vg.parse(true);
			VTDNav vn= vg.getNav();
			AutoPilot ap = new AutoPilot(vn);
			ap.selectXPath("//*|//@*");
			int i=-1,k=0;
			while((i=ap.evalXPath())!=-1){
				if ((k&1)!=0 ) {
					fib1.append(i);
					vn.sampleState(fib2);
				}
				k++;
			}
			for (int z=0; z<fib1.size() ; z++ ){
				vn.recoverNode(fib1.intAt(z));
				vn.sampleState(fib3);
			}
			if (fib2.size()!=fib3.size())
				return false;
			for (int g=0;g<fib2.size();g++){
				if (fib2.intAt(g)!= fib3.intAt(g))
					return false;
			}
			
			return true;
		} catch (ParseException e) {
			System.out.println("ParserException: " + e);
			e.printStackTrace();
			return false;
		} catch (NavException e) {
			System.out.println("NavException:" + e);
			return false;
		} catch (Exception e) {
			e.printStackTrace();
			System.out.println("other exceptions");
			return false;
		}

	}
/**
 * 
 * @param args
 */
	public static void main1(String args) {
		String fileName = "c://benchmark_2.7//xml//"+args;
       if (test(fileName,3) ){
        	System.out.println(fileName+" passed!");
        }
       else
    	   System.out.println(fileName+ " failed!");
	    if (testIndex(fileName,3 )){
	    	System.out.println(fileName+ " passed!");
	    }else
    	   System.out.println(fileName+ " failed!");
	    
	    if (testRecoverNode(fileName,3 )){
	    	System.out.println(fileName+ " passed!");
	    }else
    	   System.out.println(fileName+ " failed!");
	    
	    if (test(fileName,5) ){
        	System.out.println(fileName+" passed!");
        }
       else
    	   System.out.println(fileName+ " failed!");
	    if (testIndex(fileName,5 )){
	    	System.out.println(fileName+ " passed!");
	    }else
    	   System.out.println(fileName+ " failed!");
	    if (testRecoverNode(fileName,5 )){
	    	System.out.println(fileName+ " passed!");
	    }else
    	   System.out.println(fileName+ " failed!");
	}
	
	public static void main(String[] args) {
		String s1 = "nav";
		for(int i=40;i<80;i++){
			for (int j=0;j<5;j++)
				main1("nav_"+i+"_"+j+".xml");
		}
	}
}
