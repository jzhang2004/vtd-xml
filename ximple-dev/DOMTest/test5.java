package DOMTest;
import com.ximpleware.*;
public class test5 {
	
	public static void main(String[] s) throws Exception{
		VTDGen vg = new VTDGen();
		if (vg.parseFile("c:/xml/NAI_SC.tmx", true)){
			VTDNav vn = vg.getNav();
			AutoPilot ap = new AutoPilot(vn);
			int i,j=0,k=0;
			FastIntBuffer fb1 = new FastIntBuffer(10);
			FastIntBuffer fb2 = new FastIntBuffer(10);
			BookMark bm1, bm2;
			bm1 = new BookMark(vn);
			bm2 = new BookMark(vn);
			ap.selectXPath("//*");
			AutoPilot ap2 = new AutoPilot(vn);
			ap2.selectXPath("//*");
			while((i=ap.evalXPath())!=-1){
				//bm1.recordCursorPosition();
				//if (i==181063){
				//	System.out.println("here!!!");
				//}
				vn.toElement(VTDNav.ROOT);
				vn.recoverNode(i);
				//bm2.recordCursorPosition();
				
				//System.out.println("" + i+"  depth ==>"+vn.getCurrentDepth());
				/*if(bm1.compare(bm2)==false){
					System.out.println("bad");
					System.out.println("" + i+"  depth ==>"+vn.getCurrentDepth());
					System.out.println(" element name  "+vn.toString(i));
				}*/ //else
					//System.out.println("good");
				
				//	System.
				//fb1.append(i);
				j++;
			}
			System.out.println("j ==>"+j);
			while((i=ap2.evalXPath())!=-1){		
				fb2.append(i);
				k++;
			}
			/*for (int h=0;h<fb1.size();h++){
				if (fb1.intAt(h)!=fb2.intAt(h)){
					System.out.println("bad===="+fb1.intAt(h)+"  "+fb2.intAt(h)+"  "+fb2.intAt(h-1));
					System.out.println("depth ====>"+vn.getTokenDepth(fb1.intAt(h)));
					System.out.println("depth ====>"+vn.getTokenDepth(fb1.intAt(h-1)));
					System.out.println("depth ====>"+vn.getTokenDepth(fb1.intAt(h-2)));
					System.out.println("depth ====>"+vn.getTokenDepth(fb2.intAt(h)));
					return;
				}
			}*/
			System.out.println("k ==>"+k);
			if (j!=k)
				System.out.println("bad!!!!!"+k);
			
		}	
	}
}
