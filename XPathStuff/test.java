import com.ximpleware.*;
import java.io.*;

class test {

	public static void main(String[] args) {
		
			
		
		try{
			//String s = "  ../  descendant-:or-self / self ::text() /@ ab [][]";
			
			parser p = new parser(new StringReader("//b[true()]"));
      		//	Object result = p.parse().value;
			Object o = p.parse().value;
			System.out.println(o);
			LocationPathExpr lpe = (LocationPathExpr) o;
			System.out.println(lpe);

			File f  = new File("./test.xml");
			FileInputStream fis = new FileInputStream(f);
			byte[] b = new byte[(int)f.length()];
			fis.read(b);
			
			VTDGen vg = new VTDGen();
			vg.setDoc(b);
			vg.parse(true); // set namespace awareness to true
			VTDNav vn = vg.getNav();
			

			vn.toElement(VTDNav.FC);

			int result;

			
			System.out.println("=================");
			
			while((result = lpe.evalNodeSet(vn))!=-1){
				//vn.dumpContext();
				System.out.println("token index -->" + vn.toString(result));
				System.out.println("\tindex ---> "+ result+"  depth ==>"+vn.getCurrentDepth());
			}
			
			System.out.println("=================");
			lpe.reset();
			while((result = lpe.evalNodeSet(vn))!=-1){
				//vn.dumpContext();
				System.out.println("token index -->" + vn.toString(result));
				System.out.println("\tindex ---> "+ result+"  depth ==>"+vn.getCurrentDepth());
			}
			
		}
		catch(XPathEvalException e){
		}
		catch(XPathParseException e){
		}
		catch(ParseException e){
		}
		catch(NavException e){
		}
		catch (Exception e){
			System.out.println(e);
		}
			
	}
}
