package DOMTest;
import com.ximpleware.*;
public class test4 {

	public static void main(String argv[]) throws Exception {
		String fileName = "c:/xml/test11.xml";
		// String fileName = "testFile/yamagata.tmx";
		VTDGen vg = new VTDGen();
		if (vg.parseFile(fileName, true)) {
			VTDNav vn = vg.getNav();
			AutoPilot ap = new AutoPilot(vn);
			// Header
			ap.selectXPath("/tmx/header");
			if (ap.evalXPath() != -1) {
				String result = null;
				long l = vn.getContentFragment();
				int offset = (int) l;
				int len = (int) (l >> 32);
				if (offset != -1 && len != -1) {
					if (vn.getEncoding() > VTDNav.FORMAT_WIN_1258)
					{
						offset = offset<<1;
						len = len<<1;
					}
					result = vn.toRawString(offset, len);
				}
				System.out.println(result);
			}
		}
	}
}
