package DOMTest;



import java.util.Hashtable;
import java.util.Iterator;
import java.util.Vector;

import com.ximpleware.AutoPilot;
import com.ximpleware.NavException;
import com.ximpleware.VTDGen;
import com.ximpleware.VTDNav;
import com.ximpleware.XPathEvalException;
import com.ximpleware.XPathParseException;

public class john2 {
	private final static String utf16leFile = "c:/xml/Test_UTF16LE.txt.xlf";
	private final static String utf8File = "c:/xml/Test.txt.xlf";
	private VTDNav vn = null;

	public String getElementContent() throws NavException {
		String result = null;
		long l = vn.getContentFragment();
		if (l == -1) {
			return null;
		}

		int offset = (int) l;
		int len = (int) (l >> 32);
		if (offset != -1 && len != -1) {
			if (vn.getEncoding()>VTDNav.FORMAT_WIN_1258){
				offset = offset >>1;
				len = len >>1;
			}
			result = vn.toRawString(offset, len);
		}
		return result;
	}

	public String getElementContent(String xpath) throws NavException, XPathParseException, XPathEvalException {
		String text = null;
		if (xpath != null && !xpath.equals("")) {
			AutoPilot ap = new AutoPilot(vn);
			ap.selectXPath(xpath);
			vn.push();
			if (ap.evalXPath() != -1) {
				text = getElementContent();
			}
			vn.pop();
		}
		return text;
	}

	public Hashtable<String, String> getCurrentElementAttributs(String nsPrefix, String nsUrl)
			throws XPathParseException, XPathEvalException, NavException {
		Hashtable<String, String> attributes = new Hashtable<String, String>();
		AutoPilot apAttributes = new AutoPilot(vn);
		apAttributes.declareXPathNameSpace(nsPrefix, nsUrl);
		apAttributes.selectXPath("@*");

		int inx = -1;
		while ((inx = apAttributes.evalXPath()) != -1) {
			String name = vn.toString(inx);
			inx = vn.getAttrVal(name);
			String value = inx != -1 ? vn.toString(inx) : "";
			attributes.put(name, value);
		}
		apAttributes.resetXPath();

		if (attributes.isEmpty()) {
			attributes = null;
		}
		return attributes;
	}

	public Hashtable<String, String> getCurrentElementAttributs(Hashtable<String, String> ns)
			throws XPathParseException, XPathEvalException, NavException {
		Hashtable<String, String> attributes = new Hashtable<String, String>();
		AutoPilot apAttributes = new AutoPilot(vn);
		if (ns != null) {
			Iterator<String> nsIt = ns.keySet().iterator();
			while (nsIt.hasNext()) {
				String nsUrl = nsIt.next();
				String nsPrefix = ns.get(nsUrl);
				apAttributes.declareXPathNameSpace(nsPrefix, nsUrl);
			}
		}

		apAttributes.selectXPath("@*");

		int inx = -1;
		while ((inx = apAttributes.evalXPath()) != -1) {
			String name = vn.toString(inx);
			inx = vn.getAttrVal(name);
			String value = inx != -1 ? vn.toString(inx) : "";
			attributes.put(name, value);
		}
		apAttributes.resetXPath();

		if (attributes.isEmpty()) {
			attributes = null;
		}
		return attributes;
	}

	public Hashtable<String, String> getCurrentElementAttributs() throws XPathParseException, XPathEvalException,
			NavException {
		Hashtable<String, String> attributes = new Hashtable<String, String>();
		AutoPilot apAttributes = new AutoPilot(vn);
		apAttributes.selectXPath("@*");

		int inx = -1;
		while ((inx = apAttributes.evalXPath()) != -1) {
			String name = vn.toString(inx);
			inx = vn.getAttrVal(name);
			String value = inx != -1 ? vn.toString(inx) : "";
			attributes.put(name, value);
		}
		apAttributes.resetXPath();

		if (attributes.isEmpty()) {
			attributes = null;
		}
		return attributes;
	}

	public String getElementPureText() throws XPathParseException, XPathEvalException, NavException {
		String txtNode = "./text()";
		AutoPilot ap = new AutoPilot(vn);
		StringBuilder result = new StringBuilder();
		ap.selectXPath(txtNode);
		int txtIndex = -1;
		vn.push();
		while ((txtIndex = ap.evalXPath()) != -1) {
			result.append(vn.toString(txtIndex));
		}
		vn.pop();

		if (result.length() == 0) {
			return null;
		} else {
			return result.toString();
		}
	}

	public String getCurrentElementName() throws NavException {
		return vn.toString(vn.getCurrentIndex());
	}

	public String getCurrentElementName(int inx) throws NavException {
		return vn.toString(inx);
	}

	public int getChildElementsCount() throws XPathParseException, XPathEvalException, NavException {
		int result = 0;
		AutoPilot ap = new AutoPilot(vn);
		ap.selectXPath("./*");
		vn.push();
		while (ap.evalXPath() != -1) {
			result++;
		}
		vn.pop();
		return result;
	}

	public int getChildElementsCount(String xpath) throws XPathParseException, XPathEvalException, NavException {
		int result = 0;
		AutoPilot ap = new AutoPilot(vn);
		ap.selectXPath(xpath);
		vn.push();
		if (ap.evalXPath() != -1) {
			result = getChildElementsCount();
		}
		vn.pop();
		return result;
	}

	public String getChildContent(String elementName) throws XPathParseException, XPathEvalException, NavException {
		String text = null;
		AutoPilot ap = new AutoPilot(vn);
		ap.selectXPath("./" + elementName);
		vn.push();
		if (ap.evalXPath() != -1) {
			text = getElementContent();
		}
		vn.pop();
		return text;
	}

	public Vector<String> getChildrenContent(String elementName) throws XPathParseException, XPathEvalException,
			NavException {
		Vector<String> texts = new Vector<String>();
		AutoPilot ap = new AutoPilot(vn);
		ap.selectXPath("./" + elementName);
		while (ap.evalXPath() != -1) {
			vn.push();
			texts.add(getElementContent());
			vn.pop();
		}

		if (texts.isEmpty()) {
			texts = null;
		}
		return texts;
	}

	public void testGetChildContent() throws XPathParseException, XPathEvalException, NavException {
		AutoPilot ap = new AutoPilot(vn);
		ap.selectXPath("//trans-unit[position()=1]");
		vn.push();
		int inx = ap.evalXPath();
		if (-1 == inx) {
			System.out.println("testGetChildContent Error");
		}

		String srcText = getChildContent("source");
		if (!"This <ph id=\"1\">&lt;b&gt;</ph>is the first <ph id=\"2\">&lt;/b&gt;</ph>test.".equals(srcText)) {
			System.out.println("testGetChildContent Error");
		}
		vn.pop();

		ap.resetXPath();
		ap.selectXPath("//trans-unit");
		vn.push();
		inx = ap.evalXPath();
		if (-1 == inx) {
			System.out.println("testGetChildContent Error");
		}
		String txt = getChildContent("note");
		vn.pop();
		if (!"The note 1.".equals(txt)) {
			System.out.println("testGetChildContent Error");
		}
	}

	public void testGetChildrenContent() throws XPathParseException, XPathEvalException, NavException {
		AutoPilot ap = new AutoPilot(vn);
		ap.selectXPath("//trans-unit[position()=2]");
		ap.evalXPath();
		Vector<String> notes = getChildrenContent("note");
		if (3 != notes.size()) {
			System.out.println("testGetChildrenContent Error");
			return;
		}

		if (!"The 1 note.".equals(notes.get(0))) {
			System.out.println("testGetChildrenContent Error");
		}

		if (!"The 2 note.".equals(notes.get(1))) {
			System.out.println("testGetChildrenContent Error");
		}

		if (!"The 3 note.".equals(notes.get(2))) {
			System.out.println("testGetChildrenContent Error");
		}
	}

	public void testGetCurrentElementAttributsNoParams() throws XPathEvalException, NavException, XPathParseException {
		AutoPilot ap = new AutoPilot(vn);
		ap.selectXPath("//trans-unit[position()=2]");
		vn.push();
		int inx = ap.evalXPath();
		if (inx == -1) {
			System.out.println("testGetCurrentElementAttributesNoParams Error.");
		}
		Hashtable<String, String> eAtts = new Hashtable<String, String>();
		eAtts.put("approved", "yes");
		eAtts.put("id", "1");
		eAtts.put("merged-trans", "yes");
		eAtts.put("reformat", "yes");
		eAtts.put("size-unit", "pixel");
		eAtts.put("translate", "yes");
		eAtts.put("xml:space", "preserve");
		eAtts.put("hs:ext", "yes");
		eAtts.put("xsi:test", "test");

		Hashtable<String, String> aAtts = getCurrentElementAttributs();
		vn.pop();
		if (!eAtts.equals(aAtts)) {
			System.out.println("testGetCurrentElementAttributesNoParams Error.");
		}
	}

	public void testGetCurrentElementAttributs1Param() throws XPathParseException, XPathEvalException, NavException {
		AutoPilot ap = new AutoPilot(vn);
		ap.selectXPath("//trans-unit[position()=2]");
		vn.push();
		ap.evalXPath();
		Hashtable<String, String> eAtts = new Hashtable<String, String>();
		eAtts.put("approved", "yes");
		eAtts.put("id", "1");
		eAtts.put("merged-trans", "yes");
		eAtts.put("reformat", "yes");
		eAtts.put("size-unit", "pixel");
		eAtts.put("translate", "yes");
		eAtts.put("xml:space", "preserve");
		eAtts.put("hs:ext", "yes");
		eAtts.put("xsi:test", "test");

		Hashtable<String, String> aAtts = getCurrentElementAttributs("es",
				"http://www.heartsome.net.cn/2008/XLFExtension");
		vn.pop();
		if (!eAtts.equals(aAtts)) {
			System.out.println("testGetCurrentElementAttributs1Param Error");
		}
	}

	public void testGetCurrentElementAttributs2Params() throws XPathParseException, XPathEvalException, NavException {

		AutoPilot ap = new AutoPilot(vn);
		ap.selectXPath("//trans-unit[position()=2]");
		vn.push();
		ap.evalXPath();

		Hashtable<String, String> eAtts = new Hashtable<String, String>();
		eAtts.put("approved", "yes");
		eAtts.put("id", "1");
		eAtts.put("merged-trans", "yes");
		eAtts.put("reformat", "yes");
		eAtts.put("size-unit", "pixel");
		eAtts.put("translate", "yes");
		eAtts.put("xml:space", "preserve");
		eAtts.put("hs:ext", "yes");
		eAtts.put("xsi:test", "test");

		Hashtable<String, String> ns = new Hashtable<String, String>();
		ns.put("http://www.heartsome.net.cn/2008/XLFExtension", "es");
		ns.put("http://www.w3.org/2001/XMLSchema-instance", "es");
		Hashtable<String, String> aAtts = getCurrentElementAttributs(ns);
		vn.pop();
		if (!eAtts.equals(aAtts)) {
			System.out.println("testGetCurrentElementAttributs2Param Error");
		}
	}

	public void testGetElementContent() throws NavException, XPathParseException, XPathEvalException {
		AutoPilot ap = new AutoPilot(vn);
		ap.selectXPath("//trans-unit/note[position()=1]");
		vn.push();
		int inx = ap.evalXPath();
		if (-1 == inx) {
			System.out.println("testGetElementContent Error");
			return;
		}
		String txt = getElementContent();
		vn.pop();
		if (!"The note 1.".equals(txt)) {
			System.out.println("testGetElementContent Error");
		}
	}

	public void testGetElementContent1Param() throws NavException, XPathParseException, XPathEvalException {
		String txt = getElementContent("//trans-unit/note[position()=1]");
		if (!"The note 1.".equals(txt)) {
			System.out.println("testGetElementContent1Param Error");
		}
	}

	public void testGetElementPureText() throws XPathParseException, XPathEvalException, NavException {
		AutoPilot ap = new AutoPilot(vn);
		ap.selectXPath("//trans-unit[position()=1]/source");
		vn.push();
		int inx = ap.evalXPath();
		if (inx == -1) {
			System.out.println("testGetElementPureText Error");
			return;
		}
		String txt = getElementPureText();
		vn.pop();
		if (!"This is the first test.".equals(txt)) {
			System.out.println("testGetElementPureText Error");
		}
	}

	public void testGetCurrentElementName() throws XPathParseException, XPathEvalException, NavException {
		AutoPilot ap = new AutoPilot(vn);
		ap.selectXPath("//trans-unit[position()=1]/source");
		vn.push();
		int inx = ap.evalXPath();
		String name = getCurrentElementName(inx);
		vn.pop();
		if (!name.equals("source")) {
			System.out.println("testGetCurrentElementName Error");
		}
	}

	public void testGetCurrentElementNameWithoutInx() throws XPathParseException, XPathEvalException, NavException {
		AutoPilot ap = new AutoPilot(vn);
		ap.selectXPath("//trans-unit[position()=1]/source");
		vn.push();
		ap.evalXPath();
		String name = getCurrentElementName();
		vn.pop();
		if (!name.equals("source")) {
			System.out.println("testGetCurrentElementNameWithoutInx Error");
		}
	}

	public void testGetChildElementsCount() throws XPathParseException, XPathEvalException, NavException {
		AutoPilot ap = new AutoPilot(vn);
		ap.selectXPath("/xliff");
		vn.push();
		ap.evalXPath();
		int count = getChildElementsCount();
		vn.pop();

		if (2 != count) {
			System.out.println("testGetChildElementsCount Error");
		}
	}

	public void testGetChildElementsCount2() throws XPathParseException, XPathEvalException, NavException {
		int count = getChildElementsCount("/xliff");
		if (2 != count) {
			System.out.println("testGetChildElementsCount2 Error");
		}
	}

	public boolean parse() {
		VTDGen vg = new VTDGen();
		if (vg.parseFile(utf16leFile, true)) {
			vn = vg.getNav();
			return true;
		}
		return false;
	}

	public static void main(String[] args) throws XPathEvalException, NavException, XPathParseException {
		john2 vnt = new john2();
		if (vnt.parse()) {
			vnt.testGetCurrentElementAttributsNoParams();
			vnt.testGetCurrentElementAttributs1Param();
			vnt.testGetCurrentElementAttributs2Params();
			vnt.testGetElementPureText();
			vnt.testGetCurrentElementName();
			vnt.testGetCurrentElementNameWithoutInx();
			vnt.testGetChildContent();
			vnt.testGetChildElementsCount();
			vnt.testGetChildElementsCount2();
			vnt.testGetChildrenContent();
			vnt.testGetElementContent();
			vnt.testGetElementContent1Param();
		} else {
			System.out.println("Failed to parse the file.");
		}
	}
}
