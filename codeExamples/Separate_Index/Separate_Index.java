import com.ximpleware.*;
public class Separate_Index {
    public static void main(String[] args) throws Exception{
        VTDGen vg = new VTDGen();
        if (vg.parseFile("mix3.xml",true)){
            vg.writeSeparateIndex("mix3.vtd");
        }
        VTDNav vn = vg.loadSeparateIndex("mix3.xml","mix3.vtd");
        AutoPilot ap = new AutoPilot(vn);
        ap.selectXPath("//*");
        int i;
        while((i=ap.evalXPath())!=-1){
            System.out.println(vn.toString(i));
        }
    }
}

