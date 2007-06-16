import java.io.*;
import com.ximpleware.*;
//This example shows you how to create an VTD+XML index
public class indexWrite {
	public static void main(String args[]) {
		try{
			VTDGen vg = new VTDGen();
			if (vg.parseFile("po.xml",true)){
				// recommended extension is .vxl
				vg.writeIndex("po.vxl");
			}
		}catch(Exception e){
		}
	}
}
