import com.ximpleware.*;
class NodeTest implements LocationPathNode{
	public String nodeName;
	public String prefix;
	public String localName;
	boolean nsEnabled;
	public int testType;
	
	public static final int NAMETEST = 0,
				NODE =1,
				TEXT =2,
				PI0=3,
				PI1 = 4,
				COMMENT = 5;
	public NodeTest(){
		nsEnabled = false;
	}
	public void setNsEnabled(boolean b){
		nsEnabled = b;
	}
	public void setTestType(int t){
		testType = t;
	}
	public void setNodeName(String s){
		nodeName = s;
	}
	public void setNodeNameNS(String p, String ln){
		prefix = p;
		localName = ln;
	}
	public boolean eval(VTDNav vn)throws NavException{
		if (testType == NODE)
			return true;
		else if(testType == NAMETEST){
			return vn.matchElement(nodeName);
		}
		return false;
	}

	public String toString(){
		switch (testType){
			case NAMETEST :   return nodeName;
			case NODE: return "node()";
			case TEXT: return "text()";
			case PI0: 
			case PI1: return "processing-instruction()";
			default:  return "comment()";
		}
	}
	
}
