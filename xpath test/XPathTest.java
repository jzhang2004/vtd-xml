/*
 * Created on Jun 22, 2005
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */

/**
 * @author jimmy zhang
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 *  
 * 
 * * first thing is to make /a/b/c work for a small XML file
 * then //a/b 7/3/2005 								   done!
 * then //a/b//c 7/6/2005 							   done!
 * then /descendent::a/child::b/descendent::c 		   done!
 * change return type of XpathEval to return an interger done !
 * support parent										done! 
 * and ancestor or self 								done!
 *  //c/.. 											   done!
 * added document node 								   done! 
 * child::text() is the only form 
 * install jdk 1.5 and try various xpath 		  half done!
 * implement the eval function of node test            done!
 * supporting self, following sibling, preceding sibling done but not tested
 * supporting following and preceding				   done but not tested
 * supporting attributes
 * parse an XPath expression
 * think about how to do //text();
 * think about how to do //@a[following::node()]
 * 
 * How to check uniqueness?
 * use a custom hash table??
 * 
 * stick with element only hierarchy model
 * can following axis be applied to an attribute node?
 * 
 */

//import org.apache.xpath.*;
import java.io.*;
import com.ximpleware.*;

public class XPathTest {
	interface XPathNode{
		public boolean eval(VTDNav vn) throws NavException;
		
	}
	
	public XPathTest(){
		currentStep =null;
		offset = 0;
	}
	// define the states of xpath parser
	public static final int PARSERSTATE_START = 0,
	PARSERSTATE_END = 1,
	PARSERSTATE_AXIS = 2,
	PARSERSTATE_NODETEST = 3,
	PARSERSTATE_PREDICATE = 4;
	
	int parser_state = 0;
	
	public static final int AXIS_CHILD = 0,
	AXIS_DESCENDANT  =1,
	AXIS_PARENT = 2,
	AXIS_ANCESTOR  = 3,
	AXIS_FOLLOWING_SIBLING =4,
	AXIS_PRECEDING_SIBLING  = 5,
	AXIS_FOLLOWING =6,
	AXIS_PRECEDING =7,
	AXIS_ATTRIBUTE = 8,
	AXIS_NAMESPACE = 9,
	AXIS_SELF = 10,
	AXIS_DESCENDANT_OR_SELF = 11,
	AXIS_ANCESTOR_OR_SELF = 12;
	
	class NodeTest implements XPathNode{
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
	}
	
	class Predicate implements XPathNode{
		public Predicate(){
			nextP = (Predicate) null;
		}
		public boolean eval(VTDNav vn) throws NavException{
			return false;
		}
		Predicate nextP;
	}
	
	class Step implements XPathNode{
		int axis_type;
		NodeTest nt;  
		Predicate p;// linked list
		Step nextS; // points to next step
		
		Step prevS; // points to the prev step
		Object o; //AutoPilot goes here
		boolean ft; // first time
		public Step(){
			nextS = prevS = (Step)null;
			p = null;
			nt = null;
			ft = true;
		}
		
		public NodeTest getNodeTest(){
			return this.nt;
		}
		public Step getNextStep(){
			return nextS;
		}
		
		public void setNextStep(Step s){
			nextS = s;
		}
		
		public boolean get_ft(){
			return ft;
		}
		
		public void set_ft(boolean b){
			ft = b;
		}
				
		public Step getPrevStep(){
			return prevS;
		}
		
		public void setPrevStep(Step s){
			prevS = s;
		}
		
		public void setNodeTest(NodeTest n){
			nt = n;
		}
		
		public void setPredicate(Predicate p1){
			p = p1;
		}
		
		public boolean eval(VTDNav vn) throws NavException{
			return false;
		}
		
		public void setAxisType(int st){
			axis_type = st;
		}
		
		
	}
	
	class XPathExpr {
		public static final int ABSOLUTE_PATH =0,
								RELATIVE_PATH =1;
		Step s;
		int path_type;
		int state;
		FastIntBuffer fib; // for uniqueness checking
		public static final int START = 0, // initial state
							    END=1,   // return to begin
								TERMINAL =2, // no more next step
								FORWARD = 3, // 
								BACKWARD =4, //
								DOWN = 5; // 
								
		public XPathExpr(){
			state = START;
			s = null;
			path_type = RELATIVE_PATH;
			fib = new FastIntBuffer(8);// page size 256 = 2^ 8
		}
		
		public void setStep(Step st){
			s = st;
		}
		
		public void setPathType(int ptype){
			path_type = ptype;
		}
		
		public boolean isUnique(int i){
			int size = fib.size();
			for (int j=0; j<size;j++){
				if (i == fib.intAt(j))
					return false;
			}
			fib.append(i);
			return true;
		}
		
		public void reset(){
			state = START;
			fib.clear();
			
		}
	}
	
	class XPathException extends Exception{
		public XPathException(String s){
			super(s);
		}
	}
	String XPathInput;
	VTDNav vn;
	
	int offset;
	int endOffset;
	boolean relative; //true if relative path; false otherwise
	
	
	private boolean isWS(char c){
		if (c==' '|| c=='\n' || c=='\t' || c=='\r')
			return true;
		return false;
	}
	private char getCharAfterS() throws XPathException{
		if (offset>endOffset)
			throw new XPathException("Premature ending");
		while(isWS(XPathInput.charAt(offset))){
			offset++;
			if (offset>endOffset){
				throw new XPathException("Premature ending");
			}
		}
		char ch = XPathInput.charAt(offset);
		offset++;
		return ch;		
	}

	private char getChar()throws XPathException{
		if (offset>endOffset)
			throw new XPathException("Premature ending");
		char  ch= XPathInput.charAt(offset);
		offset++;
		return ch;
		
	}
	
	private void putBack() {
		offset--;
	}
	public XPathExpr parseXPath(String s)throws XPathException{
		if (s==null || s.length()<1)
			throw new IllegalArgumentException("XPath expression can't be null or zero length");
		// initialize
		XPathInput = s;
		endOffset = s.length()-1;
		offset = 0;
		while(true){
			
			
			
			break;
		}
		//
		return null;
	}
	
	// corresponding to //a/b//c
	public XPathExpr buildExpr2(VTDNav vn){
		Step currentStep;
		XPathExpr  xpe= new XPathExpr();
		
		xpe.setPathType(XPathExpr.ABSOLUTE_PATH);
		
		Step s0 = new Step();// for a
		
		s0.setAxisType(AXIS_DESCENDANT_OR_SELF);
		s0.o = new AutoPilot(vn);
		
		NodeTest nt0 = new NodeTest();
		nt0.setTestType(NodeTest.NAMETEST);
		nt0.nodeName = "a";
		//ap0.selectElement(nt0.qname);
		s0.setNodeTest(nt0);
		s0.setPrevStep(null);
		currentStep = s0;
		xpe.setStep(s0);
		
		
		Step s1 = new Step();// for a
		
		s1.setAxisType(AXIS_CHILD);
		
		NodeTest nt1 = new NodeTest();
		nt1.setTestType(NodeTest.NAMETEST);
		
		nt1.nodeName = "b";
		s1.setNodeTest(nt1);
		currentStep.setNextStep(s1);
		s1.setPrevStep(currentStep);
		currentStep = s1;
		//xpe.setStep(s1);
		
		Step s2 = new Step();// for a		
		s2.setAxisType(AXIS_DESCENDANT_OR_SELF);		
		s2.o = new AutoPilot(vn);
		NodeTest nt2 = new NodeTest();		
		nt2.setTestType(NodeTest.NAMETEST);
		nt2.nodeName = "c";		
		s2.setNodeTest(nt2);		
		currentStep.setNextStep(s2);		
		s2.setPrevStep(currentStep);		
		currentStep  = s2;		
		// set null pointer to the end		
		currentStep.setNextStep(null);			
		return xpe;
	
	}
	
	// /descendent::a/child:b/descendent::c
	public XPathExpr buildExpr3(VTDNav vn){
		Step currentStep;
		XPathExpr  xpe= new XPathExpr();
		xpe.setPathType(XPathExpr.ABSOLUTE_PATH);
		
		Step s0 = new Step();// for a
		
		s0.setAxisType(AXIS_DESCENDANT);
		s0.o = new AutoPilot(vn);
		
		NodeTest nt0 = new NodeTest();
		nt0.setTestType(NodeTest.NAMETEST);
		nt0.nodeName = "a";
		//ap0.selectElement(nt0.qname);
		s0.setNodeTest(nt0);
		s0.setPrevStep(null);
		currentStep = s0;
		xpe.setStep(s0);
		
		
		Step s1 = new Step();// for a
		
		s1.setAxisType(AXIS_CHILD);
		
		NodeTest nt1 = new NodeTest();
		nt1.setTestType(NodeTest.NAMETEST);
		
		nt1.nodeName = "b";
		s1.setNodeTest(nt1);
		currentStep.setNextStep(s1);
		s1.setPrevStep(currentStep);
		currentStep = s1;
		//xpe.setStep(s1);
		
		Step s2 = new Step();// for a		
		s2.setAxisType(AXIS_DESCENDANT_OR_SELF);		
		s2.o = new AutoPilot(vn);
		NodeTest nt2 = new NodeTest();		
		nt2.setTestType(NodeTest.NAMETEST);
		nt2.nodeName = "c";		
		s2.setNodeTest(nt2);		
		currentStep.setNextStep(s2);		
		s2.setPrevStep(currentStep);		
		currentStep  = s2;		
		// set null pointer to the end		
		currentStep.setNextStep(null);			
		return xpe;
	
	}
	// //a/..
	public XPathExpr buildExpr4(VTDNav vn){
		Step currentStep;
		XPathExpr  xpe= new XPathExpr();
		xpe.setPathType(XPathExpr.ABSOLUTE_PATH);
		
		Step s0 = new Step();// for a
		
		s0.setAxisType(AXIS_DESCENDANT_OR_SELF);
		s0.o = new AutoPilot(vn);
		
		NodeTest nt0 = new NodeTest();
		nt0.setTestType(NodeTest.NAMETEST);
		nt0.nodeName = "a";
		//ap0.selectElement(nt0.qname);
		s0.setNodeTest(nt0);
		s0.setPrevStep(null);
		currentStep = s0;
		xpe.setStep(s0);
		
		Step s1 = new Step();// for a
		
		s1.setAxisType(AXIS_PARENT);
		
		NodeTest nt1 = new NodeTest();
		nt1.setTestType(NodeTest.NODE);
		
		nt1.nodeName = null;
		s1.setNodeTest(nt1);
		currentStep.setNextStep(s1);
		s1.setPrevStep(currentStep);
		currentStep = s1;
		
		currentStep.setNextStep(null);	
		return xpe;
		
	}
	
	/**
	 * A bare minimum XPath parser for location path
	 * @param xpathString
	 * @return
	 */
	public XPathExpr compileXPath(String s) throws XPathException{
		// test the first char--> if / then it is an absolute path
		if (s==null || s.length()<1)
			throw new IllegalArgumentException("XPath expression can't be null or zero length");
		// initialize
		XPathInput = s;
		int tempOffset1, tempOffset2;
		endOffset = s.length()-1;
		offset = 0;
		char ch = 0;
		XPathExpr xpe = new XPathExpr();
		Step currentStep = null, tempStep = null, prevStep=null;
		NodeTest currentNt  = null;
		parser_state = PARSERSTATE_START;
		
		//  a boolean to track the case of ///, which is not allowed,
		//  /descendant-or-self::node()/ descendant-or-self::node(), however, is ok
		boolean b = false;
		
		
		// check for the possiblity of empty/all whitespace string
		boolean bad = false;
		int i=0;
		for(i=0;i<=endOffset;i++){
			if (!isWS(s.charAt(i)))
				break;
		}
		if (i>endOffset)
			throw new XPathException("all whitespace string is an invalid xpath expression");
		///////////////////////////////////
		boolean main_loop = true;
		while(main_loop){
			
			switch(parser_state){
			
			case PARSERSTATE_START: // detect whether it is an absolute path or not
				ch = getCharAfterS();
				if (ch == '/'){
					xpe.setPathType(XPathExpr.ABSOLUTE_PATH);
					ch = getCharAfterS();
				}
				else {
					xpe.setPathType(XPathExpr.RELATIVE_PATH);
				}
				parser_state = PARSERSTATE_AXIS;
				
				currentStep = new Step();
				currentStep.setPrevStep(null);
				xpe.setStep(currentStep);
				break;
				
				
			case PARSERSTATE_END:
				break;
				
				// transition into state after '/' has been detected
			case PARSERSTATE_AXIS:
				
				currentNt = new NodeTest();
				// look for abreviated form of xpath
				// start with teh first non-ws char after /
				switch(ch){
					case '/':
						if (b==false){
							currentStep.setAxisType(AXIS_DESCENDANT_OR_SELF);
							currentNt.setTestType(NodeTest.NODE);	
							b = true;
							currentStep.setNodeTest(currentNt);
							
							prevStep = currentStep;
							currentStep = new Step();
							prevStep.setNextStep(currentStep);
							currentStep.setPrevStep(prevStep);
							ch = getCharAfterS();		// "//" will throw exception
						}else 
							throw new XPathException(" /// not a valid expression");
						break;
						
					case '@': 
						b = false;
						currentStep.setAxisType(AXIS_ATTRIBUTE);
						currentNt.setTestType(NodeTest.NAMETEST);	
						//currentStep.setNodeTest(currentNt);
						parser_state = PARSERSTATE_NODETEST;
						ch = getCharAfterS();
						offset -=1;
						
						break;
						
					case '.': 
						b = false;
						try{
							ch = getChar();
						}catch(XPathException e){
							currentStep.setAxisType(AXIS_SELF);
							currentNt.setTestType(NodeTest.NODE);
							currentStep.setNodeTest(currentNt);
							currentStep.setNextStep(null);
							return xpe;
						}
						if (ch == '.'){
							currentStep.setAxisType(AXIS_PARENT);
							currentNt.setTestType(NodeTest.NODE);							
						}else {
							putBack();
							currentStep.setAxisType(AXIS_SELF);
							currentNt.setTestType(NodeTest.NODE);							
						}
						
						currentStep.setNodeTest(currentNt);						

						prevStep = currentStep;
						currentStep = new Step();
						prevStep.setNextStep(currentStep);
						currentStep.setPrevStep(prevStep);
						
						// parser state remains PARSERSTATE_AXIS
						try {
							// end of string will throw exception here
							ch = getCharAfterS();
						}catch(XPathException e){
							main_loop = false;
							break;
						}
						
						// next char must be /
						if (ch!='/')
							throw new XPathException("Invalid char in xpath");
						ch = getCharAfterS();
						break;
					default:
						// try to determine the axis type in a non-abbreviated expression
						// otherwise the default axis is child
						b = false;
						// save teh offset
						tempOffset1 = offset-1;
					
					try{
						while(ch!=':'&& ch !='/' && ch!='[' && ch!='('	&& !isWS(ch)){
							ch = getChar();
						}						
					}catch(XPathException e){
						currentStep.setAxisType(AXIS_CHILD);
						currentNt.setNodeName(s.substring(tempOffset1, offset));
						currentStep.setNodeTest(currentNt);
						currentStep.setNextStep(null);
						return xpe;
					}
					
					tempOffset2 = offset-1;
					// no non-ws char left is actually ok
					try{
						if (isWS(ch)){
							ch = getCharAfterS();
						}
					}catch(XPathException e){
						currentStep.setAxisType(AXIS_CHILD);
						currentNt.setNodeName(s.substring(tempOffset1, tempOffset2));
						currentStep.setNodeTest(currentNt);
						currentStep.setNextStep(null);
						return xpe;					
					}
					
					switch(ch){
					case ':' :						
						ch = getChar();
						if (ch == ':'){
							String axisName = s.substring(tempOffset1, tempOffset2);
							int axis = determineAxis(axisName);
							if (axis == -1)
								throw new XPathException("unknown axis type in xpath expression");
							currentStep.setAxisType(axis);
							parser_state = PARSERSTATE_NODETEST;
						}else {
							// not an axis name
							offset = tempOffset1;
							parser_state = PARSERSTATE_NODETEST;							
						}						
						break;
						// no axis name found
					default: 					
						offset = tempOffset1;
						parser_state = PARSERSTATE_NODETEST;
						currentStep.setAxisType(AXIS_CHILD);
					}					
				}
				break;
				// node is a name test, node() is  
			case PARSERSTATE_NODETEST:
				tempOffset1 = offset;
				currentStep.setNodeTest(currentNt);
				ch = getChar();
				try{
				while(ch!='/' && ch!='[' && ch!='(' && !isWS(ch)){
					ch = getChar();
				}
				}catch(XPathException e){
					currentNt.setNodeName(s.substring(tempOffset1,offset));
					currentStep.setNextStep(null);
					//main_loop = false;
					return xpe;
				}
				
				tempOffset2 = offset-1;
				if (isWS(ch)){
					try{
						ch = getCharAfterS();
					}
					catch(XPathException e){
						currentNt.setNodeName(s.substring(tempOffset1, tempOffset2));
						//currentStep.setNodeTest(currentNt);
						currentStep.setNextStep(null);
						//main_loop = false;
						return xpe;						
					}
				} 
				if (ch=='('){
					ch = getChar();
					while(ch!=')'){
						ch = getChar();
					}
					int t = determineTestType(s.substring(tempOffset1, tempOffset2));
					if (t==NodeTest.NODE && currentStep.axis_type == XPathTest.AXIS_ATTRIBUTE)
						throw new XPathException(" Must supply a name for attribute axis ");
					if (t!=-1){
						currentNt.setTestType(t);
					}
					else{
						throw new XPathException("Invalid node test type");
					}
					// once () are detected, it is ok to have remaining chars all ws.
					try{
						ch = getCharAfterS();
					}
					catch(XPathException e){
						
						//currentStep.setNodeTest(currentNt);
						currentStep.setNextStep(null);
						//main_loop = false;
						return xpe;				
					}
				}else {
					currentNt.setNodeName(s.substring(tempOffset1, tempOffset2));
					//currentStep.setNodeTest(currentNt);
				}
				
				switch(ch){
				case '/':					
					parser_state = PARSERSTATE_AXIS;
					prevStep = currentStep;
					currentStep = new Step();
					prevStep.setNextStep(currentStep);
					currentStep.setPrevStep(prevStep);
					try{
						ch = getCharAfterS();
					}catch (XPathException e){
						currentStep.setNextStep(null);
						//main_loop = false;
						return xpe;
					}
					break;
				default:
					parser_state = PARSERSTATE_PREDICATE;
				
				}
				
				break;
				
			case PARSERSTATE_PREDICATE:
				int k=1; // already detected [
				while(k!=0){
					ch = getChar();
					if (ch=='[')
						k++;
					else if (ch==']')
						k--;
				}
				try {
					ch = getCharAfterS();
				}catch (XPathException e){
					currentStep.setNextStep(null);
					//main_loop = false;
					return xpe;
				}
				if (ch == '['){
					// no state change
				}else if (ch =='/') {
					parser_state = PARSERSTATE_AXIS;
					prevStep = currentStep;
					currentStep = new Step();
					prevStep.setNextStep(currentStep);
					currentStep.setPrevStep(prevStep);
					try{
						ch = getCharAfterS();
					}catch (XPathException e){
						
						currentStep.setNextStep(null);
						//main_loop = false;
						return xpe;
					}
				}else 
					throw new XPathException("Invalid Char after predicate");
				break;
				
			default:
				throw new XPathException("Invalid parser state during XPath compilation");
			
			}
		}
		currentStep=null;
		prevStep.setNextStep(null);
			
		
		return xpe;
	}
	
	private int determineTestType(String s){
		if (s.compareTo("node")==0){
			return NodeTest.NODE;
		}
		if (s.compareTo("text")==0){
			return NodeTest.TEXT;
		}			
		if (s.compareTo("processing-instruction")==0){
			return NodeTest.PI0;
		}
		if (s.compareTo("comment")==0){
			return NodeTest.COMMENT;
		}
		return -1;
		
	}
	
	/**
	 * 
	 * @param axisName
	 * @return
	 */
	private int determineAxis(String axisName){
		if (axisName.compareTo("child")==0){
			return AXIS_CHILD;
		}
		if (axisName.compareTo("self")==0){
			return AXIS_SELF;
		}
		if (axisName.compareTo("descendant")==0){
			return AXIS_DESCENDANT;
		}
		if (axisName.compareTo("parent")==0){
			return AXIS_PARENT;
		}
		if (axisName.compareTo("descendant-or-self")==0){
			return AXIS_DESCENDANT_OR_SELF;
		}
		if (axisName.compareTo("following")==0){
			return AXIS_FOLLOWING;
		}
		if (axisName.compareTo("preceding")==0){
			return AXIS_PRECEDING;
		}
		if (axisName.compareTo("following-sibling")==0){
			return AXIS_FOLLOWING_SIBLING;
		}
		if (axisName.compareTo("preceding-sibling")==0){
			return AXIS_PRECEDING_SIBLING;
		}
		if (axisName.compareTo("ancestor")==0){
			return AXIS_ANCESTOR;
		}
		if (axisName.compareTo("ancestor-or-self")==0){
			return AXIS_ANCESTOR_OR_SELF;
		}		
		if (axisName.compareTo("attribute")==0){
			return AXIS_ATTRIBUTE;
		}
		if (axisName.compareTo("namespace")==0){
			return AXIS_NAMESPACE;
		}
		return -1;
	}
	
    // for the time being, parsing by human :)
	public XPathExpr buildExpr(){
		Step currentStep;
		XPathExpr  xpe= new XPathExpr();
		xpe.setPathType(XPathExpr.ABSOLUTE_PATH);
		
		Step s0 = new Step();// for a
		
		s0.setAxisType(AXIS_CHILD);
		
		NodeTest nt0 = new NodeTest();
		
		nt0.setTestType(NodeTest.NAMETEST);
		
		nt0.nodeName = "a";
		
		s0.setNodeTest(nt0);
		
		s0.setPrevStep(null);
		
		currentStep = s0;
		
		xpe.setStep(s0);
		
		// now move on to b
		Step s1 = new Step();// for a
		
		s1.setAxisType(AXIS_CHILD);
		
		NodeTest nt1 = new NodeTest();
		
		nt1.setTestType(NodeTest.NAMETEST);
		
		nt1.nodeName = "b";
		
		s1.setNodeTest(nt1);
		
		currentStep.setNextStep(s1);
		
		s1.setPrevStep(currentStep);
		
		currentStep = s1;
		
		// now move on to c
		
		Step s2 = new Step();// for a
		
		s2.setAxisType(AXIS_CHILD);
		
		NodeTest nt2 = new NodeTest();
		
		nt2.setTestType(NodeTest.NAMETEST);
		
		nt2.nodeName = "c";
		
		s2.setNodeTest(nt2);
		
		currentStep.setNextStep(s2);
		
		s2.setPrevStep(currentStep);
		
		currentStep  = s2;
		
		// set null pointer to the end
		
		currentStep.setNextStep(null);		
		
		return xpe;
	}
	/**
	 * 
	 * @param vn
	 * @param xpe
	 * @return
	 * @throws XPathException
	 * @throws NavException
	 */
	public int evalXpathExpr3(VTDNav vn, XPathExpr xpe) throws XPathException,
			NavException {
		AutoPilot ap = null;
		int result;

		boolean b = false;
		if (currentStep == null) {
			if (xpe.path_type == XPathExpr.ABSOLUTE_PATH){
				vn.toElement(VTDNav.ROOT);
				vn.toElement(VTDNav.PARENT);
			}
			currentStep = xpe.s;
		}

		while (true) {

			switch (currentStep.axis_type) {

			case AXIS_CHILD:
				switch (xpe.state) {
				case XPathExpr.START:
					vn.toElement(VTDNav.FIRST_CHILD);
					xpe.state = XPathExpr.END;
					do {
						if (currentStep.nt.eval(vn)) {
							if (currentStep.getNextStep() != null){
								xpe.state = XPathExpr.FORWARD;
								currentStep = currentStep.getNextStep();
							}
							else {
								xpe.state = XPathExpr.TERMINAL;
								result = vn.getCurrentIndex();
								if (xpe.isUnique(result))
									return result;
							}
							break;
						}
					} while (vn.toElement(VTDNav.NS));
					break;

				case XPathExpr.END:
					currentStep =null;
					xpe.reset();
					return -1;

				case XPathExpr.FORWARD:
					//currentStep = currentStep.getNextStep();
					xpe.state = XPathExpr.BACKWARD;
					forward: if (vn.toElement(VTDNav.FC)) {
						do {
							if (currentStep.nt.eval(vn)) {
								if (currentStep.getNextStep() != null){
									xpe.state = XPathExpr.FORWARD;
									currentStep = currentStep.getNextStep();
								}
								else {
									xpe.state = XPathExpr.TERMINAL;
									result = vn.getCurrentIndex();
									if (xpe.isUnique(result))
										return result;
								}
								break forward;
							}
						} while (vn.toElement(VTDNav.NS));
						vn.toElement(VTDNav.P);
						currentStep = currentStep.getPrevStep();
					}else {
						//vn.toElement(VTDNav.P);
						currentStep = currentStep.getPrevStep();
					}
					break;

				case XPathExpr.BACKWARD:
					//currentStep = currentStep.getPrevStep();
					b = false;
				    while(vn.toElement(VTDNav.NS)){
				    	if (currentStep.nt.eval(vn)){
				    		b = true;
				    		break;
				    	}
				    }
					if (b == true) {
						xpe.state = XPathExpr.FORWARD;
						currentStep = currentStep.getNextStep();
					} else if (currentStep.getPrevStep() == null)
						xpe.state = XPathExpr.END;
					else {
						xpe.state = XPathExpr.BACKWARD;
						vn.toElement(VTDNav.P);
						currentStep = currentStep.getPrevStep();
					}
					break;

				case XPathExpr.TERMINAL:
					while (vn.toElement(VTDNav.NS)) {
						if (currentStep.nt.eval(vn)){
							//xpe.state = XPathExpr.TERMINAL;
							result = vn.getCurrentIndex();
							if (xpe.isUnique(result))
								return result;
						}
					}

					if (currentStep.getPrevStep() == null)
						xpe.state = XPathExpr.END;

					else {
						vn.toElement(VTDNav.P);
						xpe.state = XPathExpr.BACKWARD;
						currentStep = currentStep.getPrevStep();
					}
					break;

				default:
					throw new XPathException("unknown state");
				}
				break;

			case AXIS_DESCENDANT_OR_SELF:
			case AXIS_DESCENDANT:
			case AXIS_PRECEDING:								
			case AXIS_FOLLOWING:
				switch (xpe.state) {
				case XPathExpr.START:
				case XPathExpr.FORWARD:
					// currentStep.o;
					String helper = null;
					if (currentStep.nt.testType == NodeTest.NODE){
						helper = "*";
					}else {
						helper = currentStep.nt.nodeName;
					}
					if (currentStep.get_ft() == true) {
						currentStep.o = ap = new AutoPilot(vn);
					    if (currentStep.axis_type == AXIS_DESCENDANT_OR_SELF)
							ap.selectElement(helper);
						else if (currentStep.axis_type == AXIS_DESCENDANT)
							ap.selectElement_D(helper);
						else if (currentStep.axis_type == AXIS_PRECEDING)
							ap.selectElement_P(helper);
						else 
							ap.selectElement_F(helper);
						currentStep.set_ft(false);
					}
					if (xpe.state == XPathExpr.START)
						xpe.state = XPathExpr.END;

					vn.push(); // not the most efficient. good for now
					//System.out.println("  --++ push in //");
					b = ap.iterate();
					if (b == false) {
						vn.pop();
						//System.out.println("  --++ pop in //");
						if (xpe.state == XPathExpr.FORWARD){
							xpe.state = XPathExpr.BACKWARD;
							currentStep = currentStep.getPrevStep();							
						}						
					} else {
						if (currentStep.getNextStep() != null){
							xpe.state = XPathExpr.FORWARD;
							currentStep = currentStep.getNextStep();
						}
						else {
							//vn.pop();
							xpe.state = XPathExpr.TERMINAL;
							result = vn.getCurrentIndex();
							if (xpe.isUnique(result))
								return result;
						}
					}
					break;
					
				case XPathExpr.END:
					currentStep = null;
					xpe.reset();
					return -1;

				case XPathExpr.BACKWARD:
					//currentStep = currentStep.getPrevStep();
					ap = (AutoPilot) currentStep.o;
					//vn.push();
					b = ap.iterate();
					if (b == false) {
						vn.pop();
						//System.out.println("  --++ pop in //");
						if (currentStep.getPrevStep() != null) {
							xpe.state = XPathExpr.BACKWARD;
							currentStep = currentStep.getPrevStep();
						} else
							xpe.state = XPathExpr.END;
					} else {
						if (currentStep.getNextStep() != null) {
							//vn.push();
							//System.out.println("  --++ push in //");
							xpe.state = XPathExpr.FORWARD;
							currentStep = currentStep.getNextStep();
						} else {
							xpe.state = XPathExpr.TERMINAL;
							result = vn.getCurrentIndex();
							if (xpe.isUnique(result))
								return result;
						}
					}
					break;

				case XPathExpr.TERMINAL:
					ap = (AutoPilot) currentStep.o;
					b = ap.iterate();
					if (b == true){
						result = vn.getCurrentIndex();
						if (xpe.isUnique(result))
							return result;
					}
					else if (currentStep.getPrevStep() == null) {
						xpe.state = XPathExpr.END;
					} else {
						vn.pop();
						//System.out.println("  --++ pop in //");
						xpe.state = XPathExpr.BACKWARD;
						currentStep.ft = true;
						currentStep = currentStep.getPrevStep();
					}
					break;

				default:
					throw new XPathException("unknown state");
				}
				break;

			case AXIS_PARENT:
				switch (xpe.state) {
				case XPathExpr.START:
				case XPathExpr.FORWARD:
					// assuming .. without specifying element name
					// makes the qname = null
					//if (vn.matchElement("b")){
					//	System.out.println(" b encountered ==> "+ vn.getCurrentDepth());
					//}
					if (vn.getCurrentDepth() == -1) {
						if (xpe.state == XPathExpr.START)
							xpe.state = XPathExpr.END;
						else {
							//vn.pop();
							xpe.state = XPathExpr.BACKWARD;
							currentStep = currentStep.getPrevStep();
						}
					} else {
						vn.push();
						vn.toElement(VTDNav.P); // must return true
						if (currentStep.nt.eval(vn)){
						    if (currentStep.getNextStep() != null) {

							   xpe.state = XPathExpr.FORWARD;
							   currentStep = currentStep.getNextStep();
						    } else {
							   xpe.state = XPathExpr.TERMINAL;
							   result = vn.getCurrentIndex();
								if (xpe.isUnique(result))
									return result;
						    }
						}else{
							vn.pop();
							if (xpe.state == XPathExpr.START)
								xpe.state = XPathExpr.END;
							else {								
								xpe.state = XPathExpr.BACKWARD;
								currentStep = currentStep.getPrevStep();
							}
						}
					}

					break;				
					
				case XPathExpr.END:
					currentStep = null;
					xpe.reset();
				    return -1;
					
				case XPathExpr.BACKWARD:
				case XPathExpr.TERMINAL:
					if (currentStep.getPrevStep() == null) {
						xpe.state = XPathExpr.END;
						break;
					}else {
						vn.pop();
						xpe.state = XPathExpr.BACKWARD;
						currentStep = currentStep.getPrevStep();
						break;
					}
					
				default:
					throw new XPathException("unknown state");
				}

				break;

			case AXIS_ANCESTOR: // reverse document order
				switch (xpe.state) {
				case XPathExpr.START:
					    
				   xpe.state = XPathExpr.END;
				   if (vn.getCurrentDepth()!=-1){
				   		vn.push();
						
				   		while(vn.toElement(VTDNav.P)){
				   			if (currentStep.nt.eval(vn)){
				   				if (currentStep.getNextStep() != null){
				   					xpe.state = XPathExpr.FORWARD;
				   					currentStep = currentStep.getNextStep();
				   					break;
				   				}
				   				else {
								    //vn.pop();
				   					xpe.state = XPathExpr.TERMINAL;
				   					result = vn.getCurrentIndex();
									if (xpe.isUnique(result))
										return result;
				   				}
				   			}							
				   		}
				   		if (xpe.state ==XPathExpr.END){
				   			vn.pop();
				   		}
				   }
				   break;
				  	
				case XPathExpr.FORWARD:
				    xpe.state = XPathExpr.BACKWARD;
				   	vn.push();
						
				   	while(vn.toElement(VTDNav.P)){
				   		if (currentStep.nt.eval(vn)){
				   			if (currentStep.getNextStep() != null){
				   				xpe.state = XPathExpr.FORWARD;
				   				currentStep = currentStep.getNextStep();
				   				break;
				   			}
				   			else {
				   				//vn.pop();
				   				xpe.state = XPathExpr.TERMINAL;
				   				result = vn.getCurrentIndex();
								if (xpe.isUnique(result))
									return result;
				   			}
				   		}							
				   	}
				   	if (xpe.state ==XPathExpr.BACKWARD){
				   		vn.pop();
				   		currentStep=currentStep.getPrevStep();
				   	}
				    
				  	break;
				
				case XPathExpr.END:
					currentStep =null;
					xpe.reset();
				    return -1;
				
				case XPathExpr.BACKWARD:
					b = false;
					vn.push();

					while (vn.toElement(VTDNav.P)) {
						if (currentStep.nt.eval(vn)) {
							if (currentStep.getNextStep()!= null) {
								xpe.state = XPathExpr.FORWARD;
								currentStep = currentStep.getNextStep();
								b = true;
								break;
							} else {
								//vn.pop();
								xpe.state = XPathExpr.TERMINAL;
								result = vn.getCurrentIndex();
								if (xpe.isUnique(result))
									return result;
							}
						}
					}
					if (b==false){
						vn.pop();
						if (currentStep.getPrevStep()!=null) {
							xpe.state = XPathExpr.BACKWARD;
							currentStep = currentStep.getPrevStep();
						}
						else {
							xpe.state = XPathExpr.END;
						}
					}
					break;
				
				case XPathExpr.TERMINAL:
					while (vn.toElement(VTDNav.P)) {
						if (currentStep.nt.eval(vn)) {
							result = vn.getCurrentIndex();
							if (xpe.isUnique(result))
								return result;
						}
					}
					vn.pop();
					if (currentStep.getPrevStep()!=null) {
						xpe.state = XPathExpr.BACKWARD;
						currentStep = currentStep.getPrevStep();
					}
					else {
						xpe.state = XPathExpr.END;
					}
					break;
				
				default:
					throw new XPathException("unknown state");
				}

				break;
				
			case AXIS_ANCESTOR_OR_SELF: // reverse doc order
				switch (xpe.state) {
				case XPathExpr.START:
					
					xpe.state = XPathExpr.END;
					vn.push();
					
					if (currentStep.get_ft()== true){						
						currentStep.set_ft(false);
						if (currentStep.nt.eval(vn)) {
							if (currentStep.getNextStep() != null) {
								xpe.state = XPathExpr.FORWARD;
								currentStep = currentStep.getNextStep();
								break;
							} else {
								//vn.pop();
								xpe.state = XPathExpr.TERMINAL;
								result = vn.getCurrentIndex();
								if (xpe.isUnique(result))
									return result;
							}
						}
					}
					else {
						while (vn.toElement(VTDNav.P)) {
							if (currentStep.nt.eval(vn)) {
								if (currentStep.getNextStep() != null) {
									xpe.state = XPathExpr.FORWARD;
									currentStep = currentStep.getNextStep();
									break;
								} else {
									//vn.pop();
									xpe.state = XPathExpr.TERMINAL;
									result = vn.getCurrentIndex();
									if (xpe.isUnique(result))
										return result;
								}
							}
						}
					}
					if (xpe.state == XPathExpr.END) {
						vn.pop();
					}

					break;
					
				case XPathExpr.FORWARD:
					xpe.state = XPathExpr.BACKWARD;
					vn.push();
					if (currentStep.get_ft() == true) {
						currentStep.set_ft(false);
						if (currentStep.nt.eval(vn)) {
							if (currentStep.getNextStep() != null) {
								xpe.state = XPathExpr.FORWARD;
								currentStep = currentStep.getNextStep();
								break;
							} else {
								//vn.pop();
								xpe.state = XPathExpr.TERMINAL;
								result = vn.getCurrentIndex();
								if (xpe.isUnique(result))
									return result;
							}
						}
					} else {
						while (vn.toElement(VTDNav.P)) {
							if (currentStep.nt.eval(vn)) {
								if (currentStep.getNextStep() != null) {
									xpe.state = XPathExpr.FORWARD;
									currentStep = currentStep.getNextStep();
									break;
								} else {
									//vn.pop();
									xpe.state = XPathExpr.TERMINAL;
									result = vn.getCurrentIndex();
									if (xpe.isUnique(result))
										return result;
								}
							}
						}
					}
					if (xpe.state == XPathExpr.BACKWARD) {
						vn.pop();
						currentStep = currentStep.getPrevStep();
					}
					break;
				
				case XPathExpr.END:
					currentStep = null;
					xpe.reset();
			    	return -1;
					
				
				case XPathExpr.BACKWARD:
					b = false;
					vn.push();

					while (vn.toElement(VTDNav.P)) {
						if (currentStep.nt.eval(vn)) {
							if (currentStep.getNextStep() != null) {
								xpe.state = XPathExpr.FORWARD;
								currentStep = currentStep.getNextStep();
								b = true;
								break;
							} else {
								//vn.pop();
								xpe.state = XPathExpr.TERMINAL;
								result = vn.getCurrentIndex();
								if (xpe.isUnique(result))
									return result;
							}
						}
					}
					if (b == false) {
						vn.pop();
						if (currentStep.getPrevStep() != null) {
							xpe.state = XPathExpr.BACKWARD;
							currentStep = currentStep.getPrevStep();
						} else {
							xpe.state = XPathExpr.END;
						}
					}
					break;
				
				case XPathExpr.TERMINAL:
					while (vn.toElement(VTDNav.P)) {
						if (currentStep.nt.eval(vn)) {
							result = vn.getCurrentIndex();
							if (xpe.isUnique(result))
								return result;
						}
					}
					vn.pop();
					if (currentStep.getPrevStep()!=null) {
						
						xpe.state = XPathExpr.BACKWARD;
						currentStep = currentStep.getPrevStep();
					}
					else {
						xpe.state = XPathExpr.END;
					}
					break;
					
				
				default:
					throw new XPathException("unknown state");
				}

				break;
				
			case AXIS_SELF:
				switch(xpe.state){
				  case XPathExpr.START:
				  case XPathExpr.FORWARD:	
				  	if (currentStep.nt.eval(vn)){
				  		if (currentStep.getNextStep()!=null){
				  			xpe.state = XPathExpr.FORWARD;
				  			currentStep = currentStep.getNextStep();
				  		}
				  		else{
				  			xpe.state = XPathExpr.TERMINAL;
				  			result = vn.getCurrentIndex();
							if (xpe.isUnique(result))
								return result;
				  		}
				  	}else {
				  		if (xpe.state == XPathExpr.START)
				  			xpe.state = XPathExpr.END;
				  		else 
				  			xpe.state = XPathExpr.BACKWARD;
				  	}
				    break;
				  	
				  case XPathExpr.END:
				  	currentStep = null;
				  	xpe.reset();
				  	break;
				  	
				  case XPathExpr.BACKWARD:
				  	if (currentStep.nt.eval(vn)){
				  		if (currentStep.getNextStep()!=null){
				  			xpe.state = XPathExpr.BACKWARD;
				  			currentStep = currentStep.getPrevStep();
				  		}
				  		else{
				  			xpe.state = XPathExpr.TERMINAL;
				  			result = vn.getCurrentIndex();
							if (xpe.isUnique(result))
								return result;
				  		}
				  	}else {
				  		if (currentStep.getPrevStep()!=null){
				  			xpe.state = XPathExpr.BACKWARD;
				  			currentStep= currentStep.getPrevStep();
				  		}else{
				  			xpe.state = XPathExpr.END;				  			
				  		}
				  	}
				  	break;
				  
				  case XPathExpr.TERMINAL:
				  	if (currentStep.getPrevStep()!=null){
			  			xpe.state = XPathExpr.BACKWARD;
			  			currentStep= currentStep.getPrevStep();
			  		}else{
			  			xpe.state = XPathExpr.END;				  			
			  		}
				  	break;
				  
				  default:
					throw new XPathException("unknown state");
				}
				break;
				
			case AXIS_FOLLOWING_SIBLING: 
				
				switch(xpe.state){
				  case XPathExpr.START:
				  case XPathExpr.FORWARD:
				  	if (xpe.state == XPathExpr.START)
				  		xpe.state = XPathExpr.END;
				  	else
				  		xpe.state = XPathExpr.BACKWARD;
				  	vn.push();
				  	while (vn.toElement(VTDNav.NS)){
				  		if (currentStep.nt.eval(vn)){
				  			if (currentStep.getNextStep()!=null){
				  				xpe.state = XPathExpr.FORWARD;
				  				currentStep = currentStep.getNextStep();
				  				break;
				  			} else {
				  				xpe.state = XPathExpr.TERMINAL;
				  				result = vn.getCurrentIndex();
								if (xpe.isUnique(result))
									return result;
				  			}
				  		}
				  	}
				  	
				  	if (xpe.state == XPathExpr.END){
				  		vn.pop();
				  	}else if (xpe.state == XPathExpr.BACKWARD){
				  		vn.pop();
				  		currentStep = currentStep.getPrevStep();				  		
				  	}
				    break;
				  	 
				  case XPathExpr.END:
				  	currentStep = null;
				  	xpe.reset();
				  	return -1;
				  	
				  case XPathExpr.BACKWARD:
				  	while (vn.toElement(VTDNav.NS)){
				  		if (currentStep.nt.eval(vn)){
				  			if (currentStep.getNextStep()!=null){
				  				xpe.state = XPathExpr.FORWARD;
				  				currentStep = currentStep.getNextStep();
				  				b = true;
				  				break;
				  			} else {
				  				xpe.state = XPathExpr.TERMINAL;
				  				result = vn.getCurrentIndex();
								if (xpe.isUnique(result))
									return result;
				  			}
				  		}
				  	}
				    if (b==false){
				    	vn.pop();
				    	if (currentStep.getPrevStep()==null){
				    		xpe.state = XPathExpr.END;
				    	}else{
				    		xpe.state = XPathExpr.BACKWARD;
				    		currentStep = currentStep.getPrevStep();
				    	}
				    }
				  	break;
				  
				  case XPathExpr.TERMINAL:
				  	while (vn.toElement(VTDNav.NS)){
				  		if (currentStep.nt.eval(vn)){
				  			//xpe.state = XPathExpr.TERMINAL;
				  			result = vn.getCurrentIndex();
							if (xpe.isUnique(result))
								return result;
				  		}
				  	}
				  	vn.pop();
				  	if(currentStep.getPrevStep()!=null){
				  		currentStep = currentStep.getPrevStep();
				  		xpe.state = XPathExpr.BACKWARD;
				  	}else{
				  		xpe.state = XPathExpr.END;
				  	}
				  	break;

				  default:
					throw new XPathException("unknown state");
				}
				break;
				
			case AXIS_PRECEDING_SIBLING:
				switch(xpe.state){
				  case XPathExpr.START:
				  case XPathExpr.FORWARD:
				  	if (xpe.state == XPathExpr.START)
				  		xpe.state = XPathExpr.END;
				  	else
				  		xpe.state = XPathExpr.BACKWARD;
				  	vn.push();
				  	while (vn.toElement(VTDNav.PS)){
				  		if (currentStep.nt.eval(vn)){
				  			if (currentStep.getNextStep()!=null){
				  				xpe.state = XPathExpr.FORWARD;
				  				currentStep = currentStep.getNextStep();
				  				break;
				  			} else {
				  				xpe.state = XPathExpr.TERMINAL;
				  				result = vn.getCurrentIndex();
								if (xpe.isUnique(result))
									return result;
				  			}
				  		}
				  	}
				  	
				  	if (xpe.state == XPathExpr.END){
				  		vn.pop();
				  	}else if (xpe.state == XPathExpr.BACKWARD){
				  		vn.pop();
				  		currentStep = currentStep.getPrevStep();				  		
				  	}
				  	 break;
				  	 
				  case XPathExpr.END:
				  	currentStep = null;
				  	xpe.reset();
				  	return -1;
				  
				  case XPathExpr.BACKWARD:
				  	while (vn.toElement(VTDNav.PS)){
				  		if (currentStep.nt.eval(vn)){
				  			if (currentStep.getNextStep()!=null){
				  				xpe.state = XPathExpr.FORWARD;
				  				currentStep = currentStep.getNextStep();
				  				b = true;
				  				break;
				  			} else {
				  				xpe.state = XPathExpr.TERMINAL;
				  				result = vn.getCurrentIndex();
								if (xpe.isUnique(result))
									return result;
				  			}
				  		}
				  	}
				    if (b==false){
				    	vn.pop();
				    	if (currentStep.getPrevStep()==null){
				    		xpe.state = XPathExpr.END;
				    	}else{
				    		xpe.state = XPathExpr.BACKWARD;
				    		currentStep = currentStep.getPrevStep();
				    	}
				    }
				  	break;
				  
				  case XPathExpr.TERMINAL:
				  	while (vn.toElement(VTDNav.PS)){
				  		if (currentStep.nt.eval(vn)){
				  			//xpe.state = XPathExpr.TERMINAL;
				  			result = vn.getCurrentIndex();
							if (xpe.isUnique(result))
								return result;
				  		}
				  	}
				  	vn.pop();
				  	if(currentStep.getPrevStep()!=null){
				  		currentStep = currentStep.getPrevStep();
				  		xpe.state = XPathExpr.BACKWARD;
				  	}else{
				  		xpe.state = XPathExpr.END;
				  	}
				  	break;
				  
				  default:
					throw new XPathException("unknown state");
				}
				break;
			

				
			case AXIS_ATTRIBUTE:
				switch(xpe.state){
					case XPathExpr.START:
					case XPathExpr.FORWARD:
						if (currentStep.get_ft() == true) {
							currentStep.o = ap = new AutoPilot(vn);
						    ap.selectAttr(currentStep.nt.nodeName);
							currentStep.set_ft(false);
						}
						if (xpe.state == XPathExpr.START)
							xpe.state = XPathExpr.END;
						int temp = ap.iterateAttr();
						if (temp == -1){
							if (xpe.state == XPathExpr.FORWARD){
								xpe.state = XPathExpr.BACKWARD;
								currentStep = currentStep.getPrevStep();							
							}	
						}else {
							if (currentStep.getNextStep() != null){
								xpe.state = XPathExpr.FORWARD;
								currentStep = currentStep.getNextStep();
							}
							else {
								//vn.pop();
								xpe.state = XPathExpr.TERMINAL;
								if (xpe.isUnique(temp))
									return temp;
							}
							
						}
						break;
						
					case XPathExpr.END:
						currentStep = null;
						xpe.reset();
				  		return -1;
				  		
					case XPathExpr.BACKWARD:
						ap = (AutoPilot) currentStep.o;
						//vn.push();
						temp = ap.iterateAttr();
						if (temp == -1) {
							if (currentStep.getPrevStep() != null) {
								xpe.state = XPathExpr.BACKWARD;
								currentStep = currentStep.getPrevStep();
							} else
								xpe.state = XPathExpr.END;
						} else {
							if (currentStep.getNextStep() != null) {
								xpe.state = XPathExpr.FORWARD;
								currentStep = currentStep.getNextStep();
							} else {
								xpe.state = XPathExpr.TERMINAL;
								if (xpe.isUnique(temp))
									return temp;
							}
						}
						break;
						
					case XPathExpr.TERMINAL:
						ap = (AutoPilot) currentStep.o;
						temp = ap.iterateAttr();
						if (temp != -1)
							return temp;
						if (currentStep.getPrevStep() == null) {
							xpe.state = XPathExpr.END;
						} else {
							xpe.state = XPathExpr.BACKWARD;
							currentStep.ft = true;
							currentStep = currentStep.getPrevStep();
						}
						break;					
					
					default:
						throw new XPathException("unknown state");
				}
				break;
			

			default:
				throw new XPathException("axis not supported");

			}

		}

	}

	
	
	
	Step currentStep;
	
	public void dumpXPathExpr(XPathExpr xpe){
		if (xpe.path_type==XPathExpr.RELATIVE_PATH)
			System.out.println("path type ==> Relative path");
		else 
			System.out.println("path type ==> Absolute path");
		Step s = xpe.s;
		int count = 1;
		while(s!=null){
			System.out.println("==== Step:"+ count+" ====");
			System.out.println(" axis type ==> "+getAxisName(s.axis_type));
			NodeTest nt = s.getNodeTest();
			System.out.println(" node test type ==>"+getNodeTestType(nt.testType));
			if (nt.testType==NodeTest.NAMETEST){
				System.out.println(" node name ==>" + nt.nodeName);
			}
			s = s.getNextStep();
			count++;
		}
		s = xpe.s;
		while(s.getNextStep()!=null){
			s = s.getNextStep();			
		}
		while(s.getPrevStep()!=null){
			s = s.getPrevStep();
		}
		if (s == xpe.s)
			System.out.println(" check passed " );
		System.out.println();
		System.out.println();
	}
	
	public String getAxisName(int i){
		switch(i){
		case AXIS_CHILD: return "child";
		case AXIS_DESCENDANT: return "descendant";
		case AXIS_PARENT: return "parent";
		case AXIS_ANCESTOR: return "ancestor";
		case AXIS_FOLLOWING_SIBLING: return "following-sibling";
		case AXIS_PRECEDING_SIBLING: return "preceding-sibling";
		case AXIS_FOLLOWING: return "following";
		case AXIS_PRECEDING: return "preceding";
		case AXIS_ATTRIBUTE: return "attribute";
		case AXIS_NAMESPACE: return "namespace";
		case AXIS_SELF:  return "self";
		case AXIS_DESCENDANT_OR_SELF: return "descendant-or-self";
		case AXIS_ANCESTOR_OR_SELF: return "ancestor-or-self";
		default: return "";
		}
	}
	
	public String getNodeTestType(int i){
		
		switch(i){
		case NodeTest.NAMETEST: return "name test";
		case NodeTest.NODE: return "node test";
		case NodeTest.TEXT: return "text";
		case NodeTest.COMMENT: return "comment";
		default: return "processing-instruction";
		}
		
		
	}
	public static void main(String[] args) {
		
		XPathTest xpt = new XPathTest();	
		
		try{
			//String s = "  ../  descendant-:or-self / self ::text() /@ ab [][]";
			String s = "//c/following::node()";
			XPathExpr xpe = xpt.compileXPath(s);
			System.out.println(s);
			xpt.dumpXPathExpr(xpe);
			File f  = new File("./test.xml");
			FileInputStream fis = new FileInputStream(f);
			byte[] b = new byte[(int)f.length()];
			fis.read(b);
			
			VTDGen vg = new VTDGen();
			vg.setDoc(b);
			vg.parse(true); // set namespace awareness to true
			VTDNav vn = vg.getNav();
			xpt.vn = vn;
			AutoPilot ap = new AutoPilot(vn);
			vn.toElement(VTDNav.P);
			ap.selectElement("*");
			int result;
			//while(ap.iterate()){
			//	System.out.println(" index ---> "+ vn.getCurrentIndex());
			//	System.out.println(" get current depth ==>" + vn.getCurrentDepth());
			//}
			
			System.out.println("=================");
			
			while((result = xpt.evalXpathExpr3(vn,xpe))!=-1){
				//vn.dumpContext();
				System.out.println("element name -->" + vn.toString(result));
				System.out.println("\tindex ---> "+ result+"  depth ==>"+vn.getCurrentDepth());
			}
			
			
			//vn.toElement(VTDNav.R);
//			XPathExpr expr = xpt.buildExpr2(vn);
			// navigate and perform node test according to 
			// the XPath expression
			
			//vn.toElement(VTDNav.FC);
			
//			while((result = xpt.evalXpathExpr3(vn,expr))!=-1){
//				//vn.dumpContext();
//				System.out.println("element name -->" + vn.toString(result));
//				System.out.println("\tindex ---> "+ result+"  depth ==>"+vn.getCurrentDepth());
//			}
//			System.out.println("\n+++++++++++++++++++++++\n");
//			//vn.toElement(VTDNav.R);
//			expr = xpt.buildExpr3(vn);
//			while((result= xpt.evalXpathExpr3(vn,expr))!=-1){
//				//vn.dumpContext();
//				System.out.println("element name -->" + vn.toString(result));
//				System.out.println("\tindex ---> "+ result+"  depth ==>"+vn.getCurrentDepth());
//			}
//			
//			System.out.println("\n+++++++++++++++++++++++\n");
//			
//			expr = xpt.buildExpr4(vn);
//			while((result = xpt.evalXpathExpr3(vn,expr))!=-1){
//				System.out.println("element name -->" + vn.toString(result));
//				System.out.println("\tindex ---> "+ result+"  depth ==>"+vn.getCurrentDepth());
//			}
//			
		}catch(ParseException e){
			
		}catch(NavException e){
			
		}catch(XPathException e){
			System.out.println(e);
		}
		catch (java.io.IOException e){
			
		}
		
	}
}
