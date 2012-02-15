package DOMTest;

import java.io.File;
import java.io.FileInputStream;

import com.ximpleware.AutoPilot;
import com.ximpleware.FastIntBuffer;
import com.ximpleware.NavException;
import com.ximpleware.ParseException;
import com.ximpleware.VTDGen;
import com.ximpleware.VTDNav;

public class APTest2 {
	public static boolean test(String fileName, int d) {
		FastIntBuffer flb1 = new FastIntBuffer();
		FastIntBuffer flb2 = new FastIntBuffer();
		FastIntBuffer flb3 = new FastIntBuffer();
		// FastIntBuffer flb4 = new FastIntBuffer();
		// FastIntBuffer flb5 = new FastIntBuffer();
		try {
			int z = 0;
			VTDGen vg = new VTDGen();
			vg.selectLcDepth(d);
			File f = new File(fileName);
			byte[] ba = new byte[(int) f.length()];
			FileInputStream fis = new FileInputStream(f);
			fis.read(ba);
			vg.setDoc(ba);
			vg.parse(true);
			VTDNav vn = vg.getNav();
			int sz = vn.getTokenCount();
			int h;
			for (h = 0; h < sz; h++) {
				// int len = vn.getTokenLength(i);
				// int os = vn.getTokenOffset(i);
				String s = vn.toRawString(h);
				if (!vn.matchRawTokenString(h, s))
					return false;
			}
			for (h = 0; h < sz; h++) {
				if (vn.getTokenType(h) == VTDNav.TOKEN_STARTING_TAG) {
					String s = vn.toString(h);
					if (!vn.matchTokenString(h, s)) {
						return false;
					}
				}
			}
			if (vn.toElement(VTDNav.FIRST_CHILD)) {
				do {
					z++;
					vn.sampleState(flb1);// leve1
				} while (vn.toElement(VTDNav.NEXT_SIBLING));
			}
			
			//System.out.println("level1 occurance "+z);
			// System.out.println("flb2 size is :"+flb1.size());
			z = 0;
			vn.toElement(VTDNav.ROOT);
			if (vn.toElement(VTDNav.FIRST_CHILD)) {
				do {
					if (vn.toElement(VTDNav.FIRST_CHILD)) {
						do {
							z++;
							vn.sampleState(flb1); // level2
						} while (vn.toElement(VTDNav.NEXT_SIBLING));
						vn.toElement(VTDNav.PARENT);
					}

				} while (vn.toElement(VTDNav.NEXT_SIBLING));
			}
			//System.out.println("level2 occurance "+z);
			// System.out.println("flb2 size is :"+flb1.size());
			z = 0;
			vn.toElement(VTDNav.ROOT);
			if (vn.toElement(VTDNav.FIRST_CHILD)) {
				do {
					if (vn.toElement(VTDNav.FIRST_CHILD)) {
						do {
							if (vn.toElement(VTDNav.FIRST_CHILD)) {
								do {
									z++;
									vn.sampleState(flb1); // level3
								} while (vn.toElement(VTDNav.NEXT_SIBLING));
								vn.toElement(VTDNav.PARENT);
							}
						} while (vn.toElement(VTDNav.NEXT_SIBLING));
						vn.toElement(VTDNav.PARENT);
					}
				} while (vn.toElement(VTDNav.NEXT_SIBLING));
				vn.toElement(VTDNav.PARENT);
			}
			//System.out.println("level3 occurance "+z);
			// System.out.println("flb2 size is :"+flb1.size());
			z = 0;
			vn.toElement(VTDNav.ROOT);
			if (vn.toElement(VTDNav.FIRST_CHILD)) {
				do {
					if (vn.toElement(VTDNav.FIRST_CHILD)) {
						do {
							if (vn.toElement(VTDNav.FIRST_CHILD)) {
								do {
									if (vn.toElement(VTDNav.FIRST_CHILD)) {
										do {
											z++;
											vn.sampleState(flb1); // level3
										} while (vn
												.toElement(VTDNav.NEXT_SIBLING));
										vn.toElement(VTDNav.PARENT);
									}
								} while (vn.toElement(VTDNav.NEXT_SIBLING));
								vn.toElement(VTDNav.PARENT);
							}
						} while (vn.toElement(VTDNav.NEXT_SIBLING));
						vn.toElement(VTDNav.PARENT);
					}
				} while (vn.toElement(VTDNav.NEXT_SIBLING));
				vn.toElement(VTDNav.PARENT);
			}

			//System.out.println("level4 occurance "+z);
			// System.out.println("flb2 size is :"+flb1.size());
			z = 0;
			if (vn.toElement(VTDNav.FIRST_CHILD)) {
				do {
					if (vn.toElement(VTDNav.FIRST_CHILD)) {
						do {
							if (vn.toElement(VTDNav.FIRST_CHILD)) {
								do {
									if (vn.toElement(VTDNav.FIRST_CHILD)) {
										do {
											if (vn.toElement(VTDNav.FIRST_CHILD)) {
												do {
													z++;
													vn.sampleState(flb1); // level3
												} while (vn
														.toElement(VTDNav.NEXT_SIBLING));
												vn.toElement(VTDNav.PARENT);
											}
										} while (vn
												.toElement(VTDNav.NEXT_SIBLING));
										vn.toElement(VTDNav.PARENT);
									}
								} while (vn.toElement(VTDNav.NEXT_SIBLING));
								vn.toElement(VTDNav.PARENT);
							}
						} while (vn.toElement(VTDNav.NEXT_SIBLING));
						vn.toElement(VTDNav.PARENT);
					}
				} while (vn.toElement(VTDNav.NEXT_SIBLING));
				vn.toElement(VTDNav.PARENT);
			}

			//System.out.println("level5 occurance "+z);
			// System.out.println("flb2 size is :"+flb1.size());

			z = 0;
			if (vn.toElement(VTDNav.FIRST_CHILD)) {
				do {
					if (vn.toElement(VTDNav.FIRST_CHILD)) {
						do {
							if (vn.toElement(VTDNav.FIRST_CHILD)) {
								do {
									if (vn.toElement(VTDNav.FIRST_CHILD)) {
										do {
											if (vn.toElement(VTDNav.FIRST_CHILD)) {
												do {
													if (vn.toElement(VTDNav.FIRST_CHILD)) {
														do {
															z++;
															vn.sampleState(flb1); // level3
														} while (vn
																.toElement(VTDNav.NEXT_SIBLING));
														vn.toElement(VTDNav.PARENT);
													}
												} while (vn
														.toElement(VTDNav.NEXT_SIBLING));
												vn.toElement(VTDNav.PARENT);
											}
										} while (vn
												.toElement(VTDNav.NEXT_SIBLING));
										vn.toElement(VTDNav.PARENT);
									}
								} while (vn.toElement(VTDNav.NEXT_SIBLING));
								vn.toElement(VTDNav.PARENT);
							}
						} while (vn.toElement(VTDNav.NEXT_SIBLING));
						vn.toElement(VTDNav.PARENT);
					}
				} while (vn.toElement(VTDNav.NEXT_SIBLING));
				vn.toElement(VTDNav.PARENT);
			}

			//System.out.println("level6 occurance "+z);
			// System.out.println("flb2 size is :"+flb1.size());

			z = 0;
			vn.toElement(VTDNav.ROOT);
			AutoPilot ap = new AutoPilot(vn);
			ap.selectElement("level1:level1");
			while (ap.iterate()) {
				z++;
				// System.out.println("index "+vn.getCurrentIndex()+": "+vn.toString(vn.getCurrentIndex()));
				vn.sampleState(flb2);
			}
			//System.out.println("level1 occurance "+z);
			// System.out.println("flb2 size is :"+flb2.size());
			z = 0;
			vn.toElement(VTDNav.ROOT);
			ap.selectElement("level2:level2");
			while (ap.iterate()) {
				z++;
				vn.sampleState(flb2);
			}
			//System.out.println("level2 occurance "+z);
			// System.out.println("flb2 size is :"+flb2.size());
			z = 0;
			vn.toElement(VTDNav.ROOT);
			ap.selectElement("level3:level3");
			while (ap.iterate()) {
				z++;
				vn.sampleState(flb2);
			}
			
			//System.out.println("level3 occurance "+z);
			// System.out.println("flb2 size is :"+flb2.size());
			z = 0;
			vn.toElement(VTDNav.ROOT);
			ap.selectElement("level4:level4");
			while (ap.iterate()) {
				z++;

				vn.sampleState(flb2);
			}
			//System.out.println("level4 occurance "+z);
			// System.out.println("flb2 size is :"+flb2.size());
			z = 0;
			vn.toElement(VTDNav.ROOT);
			ap.selectElement("level5:level5");
			while (ap.iterate()) {
				z++;
				vn.sampleState(flb2);
			}
			//System.out.println("level5 occurance "+z);
			// System.out.println("flb2 size is :"+flb2.size());

			z = 0;
			vn.toElement(VTDNav.ROOT);
			ap.selectElement("level6:level6");
			while (ap.iterate()) {
				z++;
				vn.sampleState(flb2);
			}
			//System.out.println("level6 occurance "+z);
			// System.out.println("flb2 size is :"+flb2.size());

			z = 0;
			if (flb1.size() != flb2.size()) {
				return false;
			}
			for (int i = 0; i < flb1.size(); i++) {
				if (flb1.intAt(i) != flb2.intAt(i))
					return false;
			}
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
		return true;

	}

	public static boolean test2(String fileName, int d){
		FastIntBuffer flb1 = new FastIntBuffer();
		FastIntBuffer flb2 = new FastIntBuffer();
		FastIntBuffer flb3 = new FastIntBuffer();
	try{
		int z = 0;
		VTDGen vg = new VTDGen();
		vg.selectLcDepth(d);
		File f = new File(fileName);
		byte[] ba = new byte[(int)f.length()];
		FileInputStream fis = new FileInputStream(f);
		fis.read(ba);
		vg.setDoc(ba);
		vg.parse(true);
		VTDNav vn =vg.getNav();
		if (vn.toElement(VTDNav.FIRST_CHILD)){
		 do{
		 	z++;
		    vn.sampleState(flb1);//leve1
		 } while(vn.toElement(VTDNav.NEXT_SIBLING));
		}
		//System.out.println("level1 occurance "+z);
		//System.out.println("flb2 size is :"+flb1.size());
		z=0;
		vn.toElement(VTDNav.ROOT);
		if (vn.toElement(VTDNav.FIRST_CHILD)){
			 do{
			 	if (vn.toElement(VTDNav.FIRST_CHILD)){
					 do{
					 	z++;
					    vn.sampleState(flb1); // level2
					 } while(vn.toElement(VTDNav.NEXT_SIBLING));
					 vn.toElement(VTDNav.PARENT);
					}
			 	
			 } while(vn.toElement(VTDNav.NEXT_SIBLING));
		}
		//System.out.println("level2 occurance "+z);
		//System.out.println("flb2 size is :"+flb1.size());
		z = 0;
		vn.toElement(VTDNav.ROOT);
		if (vn.toElement(VTDNav.FIRST_CHILD)){
			 do{
			 	if (vn.toElement(VTDNav.FIRST_CHILD)){
					 do{
					 	if (vn.toElement(VTDNav.FIRST_CHILD)){
							 do{
							 	z++;
							    vn.sampleState(flb1); // level3
							 } while(vn.toElement(VTDNav.NEXT_SIBLING));
							 vn.toElement(VTDNav.PARENT);
						}
					 } while(vn.toElement(VTDNav.NEXT_SIBLING));
					 vn.toElement(VTDNav.PARENT);
				}
			 } while(vn.toElement(VTDNav.NEXT_SIBLING));
			 vn.toElement(VTDNav.PARENT);
		}
		//System.out.println("level3 occurance "+z);
		//System.out.println("flb2 size is :"+flb1.size());
		z = 0;
		vn.toElement(VTDNav.ROOT);
		if (vn.toElement(VTDNav.FIRST_CHILD)){
			 do{
			 	if (vn.toElement(VTDNav.FIRST_CHILD)){
					 do{
					 	if (vn.toElement(VTDNav.FIRST_CHILD)){
							 do{
								 if (vn.toElement(VTDNav.FIRST_CHILD)){
									 do{
										 z++;
										 vn.sampleState(flb1); // level3								
									 } while(vn.toElement(VTDNav.NEXT_SIBLING));
									 vn.toElement(VTDNav.PARENT);
								}
							 } while(vn.toElement(VTDNav.NEXT_SIBLING));
							 vn.toElement(VTDNav.PARENT);
						}
					 } while(vn.toElement(VTDNav.NEXT_SIBLING));
					 vn.toElement(VTDNav.PARENT);
				}
			 } while(vn.toElement(VTDNav.NEXT_SIBLING));
			 vn.toElement(VTDNav.PARENT);
		}
		//System.out.println("level4 occurance "+z);
		//System.out.println("flb2 size is :"+flb1.size());
		z = 0;
		vn.toElement(VTDNav.ROOT);
		if (vn.toElement(VTDNav.FIRST_CHILD)){
			 do{
			 	if (vn.toElement(VTDNav.FIRST_CHILD)){
					 do{
					 	if (vn.toElement(VTDNav.FIRST_CHILD)){
							 do{
								 if (vn.toElement(VTDNav.FIRST_CHILD)){
									 do{
										 if (vn.toElement(VTDNav.FIRST_CHILD)){
											 do{
												 z++;
												 vn.sampleState(flb1); // level3		
											 } while(vn.toElement(VTDNav.NEXT_SIBLING));
											 vn.toElement(VTDNav.PARENT);
										}
									 } while(vn.toElement(VTDNav.NEXT_SIBLING));
									 vn.toElement(VTDNav.PARENT);
								}
							 } while(vn.toElement(VTDNav.NEXT_SIBLING));
							 vn.toElement(VTDNav.PARENT);
						}
					 } while(vn.toElement(VTDNav.NEXT_SIBLING));
					 vn.toElement(VTDNav.PARENT);
				}
			 } while(vn.toElement(VTDNav.NEXT_SIBLING));
			 vn.toElement(VTDNav.PARENT);
		}
		//System.out.println("level5 occurance "+z);
		//System.out.println("flb2 size is :"+flb1.size());
		z = 0;
		vn.toElement(VTDNav.ROOT);
		if (vn.toElement(VTDNav.FIRST_CHILD)){
			 do{
			 	if (vn.toElement(VTDNav.FIRST_CHILD)){
					 do{
					 	if (vn.toElement(VTDNav.FIRST_CHILD)){
							 do{
								 if (vn.toElement(VTDNav.FIRST_CHILD)){
									 do{
										 if (vn.toElement(VTDNav.FIRST_CHILD)){
											 do{
												 if (vn.toElement(VTDNav.FIRST_CHILD)){
													 do{
														 z++;
														 vn.sampleState(flb1); // level3	
													 } while(vn.toElement(VTDNav.NEXT_SIBLING));
													 vn.toElement(VTDNav.PARENT);
												}
											 } while(vn.toElement(VTDNav.NEXT_SIBLING));
											 vn.toElement(VTDNav.PARENT);
										}
									 } while(vn.toElement(VTDNav.NEXT_SIBLING));
									 vn.toElement(VTDNav.PARENT);
								}
							 } while(vn.toElement(VTDNav.NEXT_SIBLING));
							 vn.toElement(VTDNav.PARENT);
						}
					 } while(vn.toElement(VTDNav.NEXT_SIBLING));
					 vn.toElement(VTDNav.PARENT);
				}
			 } while(vn.toElement(VTDNav.NEXT_SIBLING));
			 vn.toElement(VTDNav.PARENT);
		}
		//System.out.println("level6 occurance "+z);
		//System.out.println("flb2 size is :"+flb1.size());
		z= 0;
		vn.toElement(VTDNav.ROOT);
		AutoPilot ap = new AutoPilot(vn);
		ap.selectElement("level1:level1");
		while(ap.iterate()){
			z++;
			//System.out.println("index "+vn.getCurrentIndex()+": "+vn.toString(vn.getCurrentIndex()));
			vn.sampleState(flb2);
		}
		//System.out.println("level1 occurance "+z);
		//System.out.println("flb2 size is :"+flb2.size());
		z = 0;
		vn.toElement(VTDNav.ROOT);
		ap.selectElement("level2:level2");
		while(ap.iterate()){
			z++;
			vn.sampleState(flb2);
		}
		//System.out.println("level2 occurance "+z);
		//System.out.println("flb2 size is :"+flb2.size());
		z = 0;
		vn.toElement(VTDNav.ROOT);
		ap.selectElement("level3:level3");
		while(ap.iterate()){
			z++;
			vn.sampleState(flb2);
		}
		//System.out.println("level3 occurance "+z);
		//System.out.println("flb2 size is :"+flb2.size());
		z = 0;
		vn.toElement(VTDNav.ROOT);
		ap.selectElement("level4:level4");
		while(ap.iterate()){
			z++;			
			vn.sampleState(flb2);
		}
		//System.out.println("level4 occurance "+z);
		//System.out.println("flb2 size is :"+flb2.size());
		z = 0;
		vn.toElement(VTDNav.ROOT);
		ap.selectElement("level5:level5");
		while(ap.iterate()){
			z++;			
			vn.sampleState(flb2);
		}
		//System.out.println("level5 occurance "+z);
		//System.out.println("flb2 size is :"+flb2.size());
		z = 0;
		vn.toElement(VTDNav.ROOT);
		ap.selectElement("level6:level6");
		while(ap.iterate()){
			z++;			
			vn.sampleState(flb2);
		}
		//System.out.println("level6 occurance "+z);
		//System.out.println("flb2 size is :"+flb2.size());
		
		z = 0;
		if (flb1.size()!=flb2.size()){
			return false;
		}
		for (int i=0;i<flb1.size();i++){
		  if (flb1.intAt(i)!= flb2.intAt(i))
		  	return false;
		}
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
		return true;		
	}



	public static boolean test2Index(String fileName,int d){
		FastIntBuffer flb1 = new FastIntBuffer();
		FastIntBuffer flb2 = new FastIntBuffer();
		FastIntBuffer flb3 = new FastIntBuffer();
	try{
		int z = 0;
		VTDGen vg = new VTDGen();
		vg.selectLcDepth(d);
		File f = new File(fileName);
		byte[] ba = new byte[(int)f.length()];
		FileInputStream fis = new FileInputStream(f);
		fis.read(ba);
		vg.setDoc(ba);
		vg.parse(true);
		vg.writeIndex("tmp.vxl");
		vg = new VTDGen();
		VTDNav vn = vg.loadIndex("tmp.vxl");
		if (vn.toElement(VTDNav.FIRST_CHILD)){
		 do{
		 	z++;
		    vn.sampleState(flb1);//leve1
		 } while(vn.toElement(VTDNav.NEXT_SIBLING));
		}
		//System.out.println("level1 occurance "+z);
		//System.out.println("flb2 size is :"+flb1.size());
		z=0;
		vn.toElement(VTDNav.ROOT);
		if (vn.toElement(VTDNav.FIRST_CHILD)){
			 do{
			 	if (vn.toElement(VTDNav.FIRST_CHILD)){
					 do{
					 	z++;
					    vn.sampleState(flb1); // level2
					 } while(vn.toElement(VTDNav.NEXT_SIBLING));
					 vn.toElement(VTDNav.PARENT);
					}
			 	
			 } while(vn.toElement(VTDNav.NEXT_SIBLING));
		}
		//System.out.println("level2 occurance "+z);
		//System.out.println("flb2 size is :"+flb1.size());
		z = 0;
		vn.toElement(VTDNav.ROOT);
		if (vn.toElement(VTDNav.FIRST_CHILD)){
			 do{
			 	if (vn.toElement(VTDNav.FIRST_CHILD)){
					 do{
					 	if (vn.toElement(VTDNav.FIRST_CHILD)){
							 do{
							 	z++;
							    vn.sampleState(flb1); // level3
							 } while(vn.toElement(VTDNav.NEXT_SIBLING));
							 vn.toElement(VTDNav.PARENT);
						}
					 } while(vn.toElement(VTDNav.NEXT_SIBLING));
					 vn.toElement(VTDNav.PARENT);
				}
			 } while(vn.toElement(VTDNav.NEXT_SIBLING));
			 vn.toElement(VTDNav.PARENT);
		}
		//System.out.println("level3 occurance "+z);
		//System.out.println("flb2 size is :"+flb1.size());
		z = 0;
		vn.toElement(VTDNav.ROOT);
		if (vn.toElement(VTDNav.FIRST_CHILD)){
			 do{
			 	if (vn.toElement(VTDNav.FIRST_CHILD)){
					 do{
					 	if (vn.toElement(VTDNav.FIRST_CHILD)){
							 do{
								 if (vn.toElement(VTDNav.FIRST_CHILD)){
									 do{
										 z++;
										 vn.sampleState(flb1); // level3								
									 } while(vn.toElement(VTDNav.NEXT_SIBLING));
									 vn.toElement(VTDNav.PARENT);
								}
							 } while(vn.toElement(VTDNav.NEXT_SIBLING));
							 vn.toElement(VTDNav.PARENT);
						}
					 } while(vn.toElement(VTDNav.NEXT_SIBLING));
					 vn.toElement(VTDNav.PARENT);
				}
			 } while(vn.toElement(VTDNav.NEXT_SIBLING));
			 vn.toElement(VTDNav.PARENT);
		}
		//System.out.println("level4 occurance "+z);
		//System.out.println("flb2 size is :"+flb1.size());
		z = 0;
		vn.toElement(VTDNav.ROOT);
		if (vn.toElement(VTDNav.FIRST_CHILD)){
			 do{
			 	if (vn.toElement(VTDNav.FIRST_CHILD)){
					 do{
					 	if (vn.toElement(VTDNav.FIRST_CHILD)){
							 do{
								 if (vn.toElement(VTDNav.FIRST_CHILD)){
									 do{
										 if (vn.toElement(VTDNav.FIRST_CHILD)){
											 do{
												 z++;
												 vn.sampleState(flb1); // level3		
											 } while(vn.toElement(VTDNav.NEXT_SIBLING));
											 vn.toElement(VTDNav.PARENT);
										}
									 } while(vn.toElement(VTDNav.NEXT_SIBLING));
									 vn.toElement(VTDNav.PARENT);
								}
							 } while(vn.toElement(VTDNav.NEXT_SIBLING));
							 vn.toElement(VTDNav.PARENT);
						}
					 } while(vn.toElement(VTDNav.NEXT_SIBLING));
					 vn.toElement(VTDNav.PARENT);
				}
			 } while(vn.toElement(VTDNav.NEXT_SIBLING));
			 vn.toElement(VTDNav.PARENT);
		}
		//System.out.println("level5 occurance "+z);
		//System.out.println("flb2 size is :"+flb1.size());
		z = 0;
		vn.toElement(VTDNav.ROOT);
		if (vn.toElement(VTDNav.FIRST_CHILD)){
			 do{
			 	if (vn.toElement(VTDNav.FIRST_CHILD)){
					 do{
					 	if (vn.toElement(VTDNav.FIRST_CHILD)){
							 do{
								 if (vn.toElement(VTDNav.FIRST_CHILD)){
									 do{
										 if (vn.toElement(VTDNav.FIRST_CHILD)){
											 do{
												 if (vn.toElement(VTDNav.FIRST_CHILD)){
													 do{
														 z++;
														 vn.sampleState(flb1); // level3	
													 } while(vn.toElement(VTDNav.NEXT_SIBLING));
													 vn.toElement(VTDNav.PARENT);
												}
											 } while(vn.toElement(VTDNav.NEXT_SIBLING));
											 vn.toElement(VTDNav.PARENT);
										}
									 } while(vn.toElement(VTDNav.NEXT_SIBLING));
									 vn.toElement(VTDNav.PARENT);
								}
							 } while(vn.toElement(VTDNav.NEXT_SIBLING));
							 vn.toElement(VTDNav.PARENT);
						}
					 } while(vn.toElement(VTDNav.NEXT_SIBLING));
					 vn.toElement(VTDNav.PARENT);
				}
			 } while(vn.toElement(VTDNav.NEXT_SIBLING));
			 vn.toElement(VTDNav.PARENT);
		}
		//System.out.println("level6 occurance "+z);
		//System.out.println("flb2 size is :"+flb1.size());
		z= 0;
		vn.toElement(VTDNav.ROOT);
		AutoPilot ap = new AutoPilot(vn);
		ap.selectElement("level1:level1");
		while(ap.iterate()){
			z++;
			//System.out.println("index "+vn.getCurrentIndex()+": "+vn.toString(vn.getCurrentIndex()));
			vn.sampleState(flb2);
		}
		//System.out.println("level1 occurance "+z);
		//System.out.println("flb2 size is :"+flb2.size());
		z = 0;
		vn.toElement(VTDNav.ROOT);
		ap.selectElement("level2:level2");
		while(ap.iterate()){
			z++;
			vn.sampleState(flb2);
		}
		//System.out.println("level2 occurance "+z);
		//System.out.println("flb2 size is :"+flb2.size());
		z = 0;
		vn.toElement(VTDNav.ROOT);
		ap.selectElement("level3:level3");
		while(ap.iterate()){
			z++;
			vn.sampleState(flb2);
		}
		//System.out.println("level3 occurance "+z);
		//System.out.println("flb2 size is :"+flb2.size());
		z = 0;
		vn.toElement(VTDNav.ROOT);
		ap.selectElement("level4:level4");
		while(ap.iterate()){
			z++;			
			vn.sampleState(flb2);
		}
		//System.out.println("level4 occurance "+z);
		//System.out.println("flb2 size is :"+flb2.size());
		z = 0;
		vn.toElement(VTDNav.ROOT);
		ap.selectElement("level5:level5");
		while(ap.iterate()){
			z++;			
			vn.sampleState(flb2);
		}
		//System.out.println("level5 occurance "+z);
		//System.out.println("flb2 size is :"+flb2.size());
		z = 0;
		vn.toElement(VTDNav.ROOT);
		ap.selectElement("level6:level6");
		while(ap.iterate()){
			z++;			
			vn.sampleState(flb2);
		}
		//System.out.println("level6 occurance "+z);
		//System.out.println("flb2 size is :"+flb2.size());
		
		z = 0;
		if (flb1.size()!=flb2.size()){
			return false;
		}
		for (int i=0;i<flb1.size();i++){
		  if (flb1.intAt(i)!= flb2.intAt(i))
		  	return false;
		}
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
		return true;	
	}
	
	public static void main1(String args) {
		String fileName = "c://benchmark_2.7//xml//"+args;
	    if (test2(fileName,3)) {
			System.out.println(fileName + " passed!");
		} else
			System.out.println(fileName + " failed!");

		if (test2Index(fileName,3) /*&& test2Index(fileName)*/) {
			System.out.println(fileName + " passed!");
		} else
			System.out.println(fileName + " failed!");
		if (test2(fileName,5)) {
			System.out.println(fileName + " passed!");
		} else
			System.out.println(fileName + " failed!");

		if (test2Index(fileName,5) /*&& test2Index(fileName)*/) {
			System.out.println(fileName + " passed!");
		} else
			System.out.println(fileName + " failed!");
	}
	
	public static void main(String[] args){
		//String s1 = "nav";
		for(int i=0;i<40;i++){
			//System.out.print("i = "+i);
			for (int j=0;j<5;j++)
				main1("ap_"+i+"_"+j+".xml");
		}
	}
}
