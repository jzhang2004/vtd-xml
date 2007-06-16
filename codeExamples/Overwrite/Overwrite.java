import com.ximpleware.*;
// This example shows you how to overwrite a token using 
// VTDNav's overwrite method call
// The basic concept is that if the replacement value is
// shorter than the original token, then you can directly
// write the replacement bytes into the XML message and
// the original VTD structure doesn't change at all!!!
class Overwrite{

	public static void main(String s[]) throws Exception{
		VTDGen vg = new VTDGen();
		vg.setDoc("<root>good</root>".getBytes());
		vg.parse(true);
		VTDNav vn = vg.getNav();
		int i=vn.getText();
		//print "good"
		System.out.println("text ---> "+vn.toString(i));
		if (vn.overWrite(i,"bad".getBytes())){
			//overwrite, if successful, returns true
			//print "bad" here 
			System.out.println("text ---> "+vn.toString(i));
		}
	}	
}
