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
 * then //a/b 7/3/2005 done!
 * then //a/b//c 7/6/2005 done!
 * then /descendent::a/child::b/descendent::c done!
 * change return type of XpathEval to return an interger done !
 * support parent, self, following sibling, preceding sibling
 * and ancestor or self 
 *  //c/.. 
 */

//import org.apache.xpath.*;
import java.io.*;
import com.ximpleware.*;

public class XPathTest {
	interface XPathNode{
		public boolean eval();
		
	}
	
	public static final int AXIS_CHILD = 0,
	AXIS_DESCENDENT  =1,
	AXIS_PARENT = 2,
	AXIS_ANCESTOR  = 3,
	AXIS_FOLLOWING_SIBLIG =4,
	AXIS_PRECEDING_SIBLING  = 5,
	AXIS_FOLLOWING =6,
	AXIS_PRECEDING =7,
	AXIS_ATTRIBUTE = 8,
	AXIS_NAMESPACE = 9,
	AXIS_SELF = 10,
	AXIS_DESCENDENT_OR_SELF = 11,
	AXIS_ANCESTOR_OR_SELF = 12;
	
	class nodeTest implements XPathNode{
		public String qname;
		String prefix;
		String localname;
		int testType;
		
		public static final int NAMETEST = 0,
								NODE =1,
								TEXT =2,
								PI0=3,
								PI1 = 4,
								COMMENT = 5;
		public void setTestType(int t){
			testType = t;
		}
		public boolean eval(){
			return false;
		}
	}
	
	class predicate implements XPathNode{
		public predicate(){
			nextP = (predicate) null;
		}
		public boolean eval(){
			return false;
		}
		predicate nextP;
	}
	
	class step implements XPathNode{
		int axis_type;
		nodeTest nt;  
		predicate p;// linked list
		step nextS; // points to next step
		step prevS; // points to the prev step
		Object o; //AutoPilot goes here
		boolean ft; // first time
		public step(){
			nextS = prevS = (step)null;
			p = null;
			nt = null;
			ft = true;
		}
		
		public step getNextStep(){
			return nextS;
		}
		
		public void setNextStep(step s){
			nextS = s;
		}
		
		public boolean get_ft(){
			return ft;
		}
		
		public void set_ft(boolean b){
			ft = b;
		}
				
		public step getPrevStep(){
			return prevS;
		}
		
		public void setPrevStep(step s){
			prevS = s;
		}
		
		public void setNodeTest(nodeTest n){
			nt = n;
		}
		
		public void setPredicate(predicate p1){
			p = p1;
		}
		
		public boolean eval(){
			return false;
		}
		
		public void setAxisType(int st){
			axis_type = st;
		}
		
		
	}
	
	class XPathExpr {
		public static final int ABSOLUTE_PATH =0,
								RELATIVE_PATH =1;
		step s;
		int path_type;
		int state;
		
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
		}
		
		public void setStep(step st){
			s = st;
		}
		
		public void setPathType(int ptype){
			path_type = ptype;
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
		if (offset>endOffset){
			throw new XPathException("premature ending");
		}
		while(isWS(XPathInput.charAt(offset)))
			offset++;
		return XPathInput.charAt(offset);		
	}

	public XPathExpr parseXPath()throws XPathException{
	
		return null;
	}
	// corresponding to //a/b//c
	public XPathExpr buildExpr2(VTDNav vn){
		step currentStep;
		XPathExpr  xpe= new XPathExpr();
		//xpe.setPathType(XPathExpr.ABSOLUTE_PATH);
		
		step s0 = new step();// for a
		
		s0.setAxisType(AXIS_DESCENDENT_OR_SELF);
		s0.o = new AutoPilot(vn);
		
		nodeTest nt0 = new nodeTest();
		nt0.setTestType(nodeTest.NAMETEST);
		nt0.qname = "a";
		//ap0.selectElement(nt0.qname);
		s0.setNodeTest(nt0);
		s0.setPrevStep(null);
		currentStep = s0;
		xpe.setStep(s0);
		
		
		step s1 = new step();// for a
		
		s1.setAxisType(AXIS_CHILD);
		
		nodeTest nt1 = new nodeTest();
		nt1.setTestType(nodeTest.NAMETEST);
		
		nt1.qname = "b";
		s1.setNodeTest(nt1);
		currentStep.setNextStep(s1);
		s1.setPrevStep(currentStep);
		currentStep = s1;
		//xpe.setStep(s1);
		
		step s2 = new step();// for a		
		s2.setAxisType(AXIS_DESCENDENT_OR_SELF);		
		s2.o = new AutoPilot(vn);
		nodeTest nt2 = new nodeTest();		
		nt2.setTestType(nodeTest.NAMETEST);
		nt2.qname = "c";		
		s2.setNodeTest(nt2);		
		currentStep.setNextStep(s2);		
		s2.setPrevStep(currentStep);		
		currentStep  = s2;		
		// set null pointer to the end		
		currentStep.setNextStep(null);			
		return xpe;
	
	}
	//descendent::a/child:b/descendent::c
	public XPathExpr buildExpr3(VTDNav vn){
		step currentStep;
		XPathExpr  xpe= new XPathExpr();
		//xpe.setPathType(XPathExpr.ABSOLUTE_PATH);
		
		step s0 = new step();// for a
		
		s0.setAxisType(AXIS_DESCENDENT);
		s0.o = new AutoPilot(vn);
		
		nodeTest nt0 = new nodeTest();
		nt0.setTestType(nodeTest.NAMETEST);
		nt0.qname = "a";
		//ap0.selectElement(nt0.qname);
		s0.setNodeTest(nt0);
		s0.setPrevStep(null);
		currentStep = s0;
		xpe.setStep(s0);
		
		
		step s1 = new step();// for a
		
		s1.setAxisType(AXIS_CHILD);
		
		nodeTest nt1 = new nodeTest();
		nt1.setTestType(nodeTest.NAMETEST);
		
		nt1.qname = "b";
		s1.setNodeTest(nt1);
		currentStep.setNextStep(s1);
		s1.setPrevStep(currentStep);
		currentStep = s1;
		//xpe.setStep(s1);
		
		step s2 = new step();// for a		
		s2.setAxisType(AXIS_DESCENDENT_OR_SELF);		
		s2.o = new AutoPilot(vn);
		nodeTest nt2 = new nodeTest();		
		nt2.setTestType(nodeTest.NAMETEST);
		nt2.qname = "c";		
		s2.setNodeTest(nt2);		
		currentStep.setNextStep(s2);		
		s2.setPrevStep(currentStep);		
		currentStep  = s2;		
		// set null pointer to the end		
		currentStep.setNextStep(null);			
		return xpe;
	
	}
	
    // for the time being, parsing by human :)
	public XPathExpr buildExpr(){
		step currentStep;
		XPathExpr  xpe= new XPathExpr();
		//xpe.setPathType(XPathExpr.ABSOLUTE_PATH);
		
		step s0 = new step();// for a
		
		s0.setAxisType(AXIS_CHILD);
		
		nodeTest nt0 = new nodeTest();
		
		nt0.setTestType(nodeTest.NAMETEST);
		
		nt0.qname = "a";
		
		s0.setNodeTest(nt0);
		
		s0.setPrevStep(null);
		
		currentStep = s0;
		
		xpe.setStep(s0);
		
		// now move on to b
		step s1 = new step();// for a
		
		s1.setAxisType(AXIS_CHILD);
		
		nodeTest nt1 = new nodeTest();
		
		nt1.setTestType(nodeTest.NAMETEST);
		
		nt1.qname = "b";
		
		s1.setNodeTest(nt1);
		
		currentStep.setNextStep(s1);
		
		s1.setPrevStep(currentStep);
		
		currentStep = s1;
		
		// now move on to c
		
		step s2 = new step();// for a
		
		s2.setAxisType(AXIS_CHILD);
		
		nodeTest nt2 = new nodeTest();
		
		nt2.setTestType(nodeTest.NAMETEST);
		
		nt2.qname = "c";
		
		s2.setNodeTest(nt2);
		
		currentStep.setNextStep(s2);
		
		s2.setPrevStep(currentStep);
		
		currentStep  = s2;
		
		// set null pointer to the end
		
		currentStep.setNextStep(null);		
		
		return xpe;
	}
	// will return if the node is found, vn will also
	// navigate accordingly
	// this function will remember the state after exiting
	// so next re-entry will continue 
	public int evalXpathExpr2(VTDNav vn, XPathExpr xpe)
			throws XPathException, NavException {
		AutoPilot ap = null;
		
		boolean b = false;
		if (currentStep == null){
			if (xpe.path_type == XPathExpr.ABSOLUTE_PATH)
				vn.toElement(VTDNav.ROOT);
			currentStep = xpe.s;
		}
		
		while (true) {
			// assuming every step is child
			if (currentStep.axis_type != AXIS_CHILD
					&& currentStep.axis_type != AXIS_DESCENDENT_OR_SELF
					&& currentStep.axis_type != AXIS_DESCENDENT)
				throw new XPathException("axis not supported");
			
			switch(xpe.state){			
			  case XPathExpr.START:
			  	if (currentStep.axis_type == AXIS_CHILD) {
					if (xpe.path_type != XPathExpr.ABSOLUTE_PATH) {
						vn.toElement(VTDNav.FIRST_CHILD);
					}
					xpe.state = XPathExpr.END;
					do {
						if (vn.matchElement(currentStep.nt.qname)) {
							if (currentStep.getNextStep() != null)
								xpe.state = XPathExpr.FORWARD;
							else {
								xpe.state = XPathExpr.TERMINAL;
								return vn.getCurrentIndex();
							}
							break;
						}
					} while (vn.toElement(VTDNav.NS));
				}else if(currentStep.axis_type == AXIS_DESCENDENT_OR_SELF
						||currentStep.axis_type == AXIS_DESCENDENT){
					ap = (AutoPilot)currentStep.o;
					if(currentStep.get_ft()==true){
						if (currentStep.axis_type == AXIS_DESCENDENT_OR_SELF)
						 ap.selectElement(currentStep.nt.qname);
						else 
						 ap.selectElement_D(currentStep.nt.qname);
						currentStep.set_ft(false);
					}
					//System.out.println("iterating element -->"+ap.getElementName());
					xpe.state = XPathExpr.END;
					
					vn.push(); // not the most efficient. good for now
					//if (ap.getElementName().matches("a"))
					//System.out.println("depth before iter "+vn.getCurrentDepth());
					b = ap.iterate();
					//if (ap.getElementName().matches("a"))
					//System.out.println("depth after iter "+vn.getCurrentDepth());
					//System.out.println("iterating result --->"+b);
					if (b== false){
						vn.pop();
						break;
					}
					else {							
						if (currentStep.getNextStep() != null)
							xpe.state = XPathExpr.FORWARD;
						else {
							//vn.pop();
							xpe.state = XPathExpr.TERMINAL;
							return vn.getCurrentIndex();
						}
					}				
				}else if (currentStep.axis_type == AXIS_PARENT){
					vn.push();
					b = vn.toElement(VTDNav.P)&& 
						(currentStep.nt.qname == null || 
						 currentStep.nt.qname.matches("*") ||
						 vn.matchElement(currentStep.nt.qname)) ;
					if (b == false)
						vn.pop();
					xpe.state = XPathExpr.END;
					if (b == true ){
						if (currentStep.getNextStep() != null)
							xpe.state = XPathExpr.FORWARD;
						else {
							xpe.state = XPathExpr.TERMINAL;
							return vn.getCurrentIndex();
						}
					}	
					
				}else if (currentStep.axis_type == AXIS_ANCESTOR){
					
				}else if (currentStep.axis_type == AXIS_ANCESTOR_OR_SELF){
					
				}
			  	
			  	break;
			  	
			  case XPathExpr.END:
			  	return -1;
			  	
			  case XPathExpr.FORWARD: //
			  	//must have a child
			  	currentStep = currentStep.getNextStep();
			  	if (currentStep.axis_type == AXIS_CHILD) {
					
			  		xpe.state = XPathExpr.BACKWARD;
					forward: if (vn.toElement(VTDNav.FC)) {
						do {
							if (vn.matchElement(currentStep.nt.qname)) {
								if (currentStep.getNextStep() != null)
									xpe.state = XPathExpr.FORWARD;
								else {
									xpe.state = XPathExpr.TERMINAL;
									return vn.getCurrentIndex();
								}
								break forward;
							}
						} while (vn.toElement(VTDNav.NS));
						vn.toElement(VTDNav.P);
					}
				} else if (currentStep.axis_type == AXIS_DESCENDENT_OR_SELF
						||currentStep.axis_type == AXIS_DESCENDENT) {
					ap = (AutoPilot)currentStep.o;

					if(currentStep.get_ft()==true){
						if (currentStep.axis_type == AXIS_DESCENDENT_OR_SELF)
							 ap.selectElement(currentStep.nt.qname);
						else 
							 ap.selectElement_D(currentStep.nt.qname);
						currentStep.set_ft(false);
					}					
					//System.out.println("iterating element -->"+ap.getElementName());
					
					vn.push(); // not the most efficient. good for now
					//if (ap.getElementName().matches("a"))
					//System.out.println("depth before iter "+vn.getCurrentDepth());
					b = ap.iterate();
					//if (ap.getElementName().matches("a"))
					//System.out.println("depth after iter "+vn.getCurrentDepth());
					//System.out.println("iterating result --->"+b);
					if (b== false){
						vn.pop();
						xpe.state = XPathExpr.BACKWARD;
					}
					else {						
						if (currentStep.getNextStep() != null)
							xpe.state = XPathExpr.FORWARD;
						else {
							//vn.pop();
							xpe.state = XPathExpr.TERMINAL;
							return vn.getCurrentIndex();
						}
					}
				}else if (currentStep.axis_type == AXIS_PARENT){
					b = vn.toElement(VTDNav.P) && 
					   (currentStep.nt.qname == null || 
					   	currentStep.nt.qname.matches("*") ||
						vn.matchElement(currentStep.nt.qname));
					xpe.state = XPathExpr.BACKWARD;
					
					if (b == true){
						if (currentStep.getNextStep() != null)
							xpe.state = XPathExpr.FORWARD;
						else {
							xpe.state = XPathExpr.TERMINAL;
							return vn.getCurrentIndex();
						}
					}	
				}else if (currentStep.axis_type == AXIS_ANCESTOR){
					
				}else if (currentStep.axis_type == AXIS_ANCESTOR_OR_SELF){
					
				}
				break;
			  
			  case XPathExpr.BACKWARD: // going backforward is ok
			  	currentStep = currentStep.getPrevStep();
			  	if (currentStep.axis_type == AXIS_CHILD) {
					if (vn.toElement(VTDNav.NS, currentStep.nt.qname)) {
						xpe.state = XPathExpr.FORWARD;
					} else if (currentStep.getPrevStep() == null)
						xpe.state = XPathExpr.END;
					else {
						xpe.state = XPathExpr.BACKWARD;
						vn.toElement(VTDNav.P);
					}
				} else if (currentStep.axis_type == AXIS_DESCENDENT_OR_SELF
						||currentStep.axis_type == AXIS_DESCENDENT) {
					ap = (AutoPilot)currentStep.o;
					//System.out.println("iterating element -->"+ap.getElementName());
					//if (ap.getElementName().matches("a"))
					//System.out.println("depth before iter "+vn.getCurrentDepth());
					b = ap.iterate();
					//if (ap.getElementName().matches("a"))
					//System.out.println("depth after iter "+vn.getCurrentDepth());
					//System.out.println("iterating result --->"+b);
					if (b== false){
						if (currentStep.getPrevStep()!=null){
						   vn.pop();
						   xpe.state = XPathExpr.BACKWARD;
						}
						else
							xpe.state = XPathExpr.END;
					}
					else {						
						if (currentStep.getNextStep() != null){
							vn.push();
							xpe.state = XPathExpr.FORWARD;
						}
						else {
							xpe.state = XPathExpr.TERMINAL;
							return vn.getCurrentIndex();
						}
					}
				}else if (currentStep.axis_type == AXIS_PARENT){
					vn.pop();
					
				}else if (currentStep.axis_type == AXIS_ANCESTOR){
					
				}else if (currentStep.axis_type == AXIS_ANCESTOR_OR_SELF){
					
				}

				break;
			  
			  case XPathExpr.TERMINAL:
			  	if (currentStep.axis_type == AXIS_CHILD) {

					while (vn.toElement(VTDNav.NS, currentStep.nt.qname)) {
						xpe.state = XPathExpr.TERMINAL;
						return vn.getCurrentIndex();
					}

					if (currentStep.getPrevStep() == null)
						xpe.state = XPathExpr.END;

					else {
						vn.toElement(VTDNav.P);
						xpe.state = XPathExpr.BACKWARD;
					}
				} else if (currentStep.axis_type == AXIS_DESCENDENT_OR_SELF
						|| currentStep.axis_type == AXIS_DESCENDENT) {
					ap = (AutoPilot)currentStep.o;
					//System.out.println("iterating element -->"+ap.getElementName());
					//if (ap.getElementName().matches("a"))
					//System.out.println("depth before iter "+vn.getCurrentDepth());
					b = ap.iterate();
					//if (ap.getElementName().matches("a"))
					//System.out.println("depth after iter "+vn.getCurrentDepth());
					//System.out.println("iterating result --->"+b);
					if (b==true)
						return vn.getCurrentIndex();					
					if (currentStep.getPrevStep() == null){					
							xpe.state = XPathExpr.END;
					}
					else {
						vn.pop();
						xpe.state = XPathExpr.BACKWARD;
						currentStep.ft = true;
					}						
				}else if (currentStep.axis_type == AXIS_PARENT){
					vn.pop();
					xpe.state = XPathExpr.BACKWARD;
					
				}else if (currentStep.axis_type == AXIS_ANCESTOR){
					
				}else if (currentStep.axis_type == AXIS_ANCESTOR_OR_SELF){
					
				}
			    break;
			  	
			  default: 
			  	throw new XPathException("unknown state");
			
			}
		}

		//return false;
	}
	
	
	
	// will return if the node is found, vn will also
	// navigate accordingly
	// this function will remember the state after exiting
	// so next re-entry will continue 
	public boolean evalXpathExpr(VTDNav vn, XPathExpr xpe)
			throws XPathException, NavException {
		AutoPilot ap = null;
		
		boolean b = false;
		if (currentStep == null){
			if (xpe.path_type == XPathExpr.ABSOLUTE_PATH)
				vn.toElement(VTDNav.ROOT);
			currentStep = xpe.s;
		}
		
		while (true) {
			// assuming every step is child
			if (currentStep.axis_type != AXIS_CHILD
					&& currentStep.axis_type != AXIS_DESCENDENT_OR_SELF
					&& currentStep.axis_type != AXIS_DESCENDENT)
				throw new XPathException("axis not supported");
			
			switch(xpe.state){			
			  case XPathExpr.START:
			  	if (currentStep.axis_type == AXIS_CHILD) {
					if (xpe.path_type != XPathExpr.ABSOLUTE_PATH) {
						vn.toElement(VTDNav.FIRST_CHILD);
					}
					xpe.state = XPathExpr.END;
					do {
						if (vn.matchElement(currentStep.nt.qname)) {
							if (currentStep.getNextStep() != null)
								xpe.state = XPathExpr.FORWARD;
							else {
								xpe.state = XPathExpr.TERMINAL;
								return true;
							}
							break;
						}
					} while (vn.toElement(VTDNav.NS));
				}else if(currentStep.axis_type == AXIS_DESCENDENT_OR_SELF
						||currentStep.axis_type == AXIS_DESCENDENT){
					ap = (AutoPilot)currentStep.o;
					if(currentStep.get_ft()==true){
						if (currentStep.axis_type == AXIS_DESCENDENT_OR_SELF)
						 ap.selectElement(currentStep.nt.qname);
						else 
						 ap.selectElement_D(currentStep.nt.qname);
						currentStep.set_ft(false);
					}
					//System.out.println("iterating element -->"+ap.getElementName());
					xpe.state = XPathExpr.END;
					
					vn.push(); // not the most efficient. good for now
					//if (ap.getElementName().matches("a"))
					//System.out.println("depth before iter "+vn.getCurrentDepth());
					b = ap.iterate();
					//if (ap.getElementName().matches("a"))
					//System.out.println("depth after iter "+vn.getCurrentDepth());
					//System.out.println("iterating result --->"+b);
					if (b== false){
						vn.pop();
						break;
					}
					else {							
						if (currentStep.getNextStep() != null)
							xpe.state = XPathExpr.FORWARD;
						else {
							//vn.pop();
							xpe.state = XPathExpr.TERMINAL;
							return true;
						}
					}				
				}
			  	
			  	break;
			  	
			  case XPathExpr.END:
			  	return false;
			  	
			  case XPathExpr.FORWARD: //
			  	//must have a child
			  	currentStep = currentStep.getNextStep();
			  	if (currentStep.axis_type == AXIS_CHILD) {
					
			  		xpe.state = XPathExpr.BACKWARD;
					forward: if (vn.toElement(VTDNav.FC)) {
						do {
							if (vn.matchElement(currentStep.nt.qname)) {
								if (currentStep.getNextStep() != null)
									xpe.state = XPathExpr.FORWARD;
								else {
									xpe.state = XPathExpr.TERMINAL;
									return true;
								}
								break forward;
							}
						} while (vn.toElement(VTDNav.NS));
						vn.toElement(VTDNav.P);
					}
				} else if (currentStep.axis_type == AXIS_DESCENDENT_OR_SELF
						||currentStep.axis_type == AXIS_DESCENDENT) {
					ap = (AutoPilot)currentStep.o;

					if(currentStep.get_ft()==true){
						if (currentStep.axis_type == AXIS_DESCENDENT_OR_SELF)
							 ap.selectElement(currentStep.nt.qname);
						else 
							 ap.selectElement_D(currentStep.nt.qname);
						currentStep.set_ft(false);
					}					
					//System.out.println("iterating element -->"+ap.getElementName());
					
					vn.push(); // not the most efficient. good for now
					//if (ap.getElementName().matches("a"))
					//System.out.println("depth before iter "+vn.getCurrentDepth());
					b = ap.iterate();
					//if (ap.getElementName().matches("a"))
					//System.out.println("depth after iter "+vn.getCurrentDepth());
					//System.out.println("iterating result --->"+b);
					if (b== false){
						vn.pop();
						xpe.state = XPathExpr.BACKWARD;
					}
					else {						
						if (currentStep.getNextStep() != null)
							xpe.state = XPathExpr.FORWARD;
						else {
							//vn.pop();
							xpe.state = XPathExpr.TERMINAL;
							return true;
						}
					}
				}
				break;
			  
			  case XPathExpr.BACKWARD: // going backforward is ok
			  	currentStep = currentStep.getPrevStep();
			  	if (currentStep.axis_type == AXIS_CHILD) {
					if (vn.toElement(VTDNav.NS, currentStep.nt.qname)) {
						xpe.state = XPathExpr.FORWARD;
					} else if (currentStep.getPrevStep() == null)
						xpe.state = XPathExpr.END;
					else {
						xpe.state = XPathExpr.BACKWARD;
						vn.toElement(VTDNav.P);
					}
				} else if (currentStep.axis_type == AXIS_DESCENDENT_OR_SELF
						||currentStep.axis_type == AXIS_DESCENDENT) {
					ap = (AutoPilot)currentStep.o;
					//System.out.println("iterating element -->"+ap.getElementName());
					//if (ap.getElementName().matches("a"))
					//System.out.println("depth before iter "+vn.getCurrentDepth());
					b = ap.iterate();
					//if (ap.getElementName().matches("a"))
					//System.out.println("depth after iter "+vn.getCurrentDepth());
					//System.out.println("iterating result --->"+b);
					if (b== false){
						if (currentStep.getPrevStep()!=null){
						   vn.pop();
						   xpe.state = XPathExpr.BACKWARD;
						}
						else
							xpe.state = XPathExpr.END;
					}
					else {						
						if (currentStep.getNextStep() != null){
							vn.push();
							xpe.state = XPathExpr.FORWARD;
						}
						else {
							xpe.state = XPathExpr.TERMINAL;
							return true;
						}
					}
				}

				break;
			  
			  case XPathExpr.TERMINAL:
			  	if (currentStep.axis_type == AXIS_CHILD) {

					while (vn.toElement(VTDNav.NS, currentStep.nt.qname)) {
						xpe.state = XPathExpr.TERMINAL;
						return true;
					}

					if (currentStep.getPrevStep() == null)
						xpe.state = XPathExpr.END;

					else {
						vn.toElement(VTDNav.P);
						xpe.state = XPathExpr.BACKWARD;
					}
				} else if (currentStep.axis_type == AXIS_DESCENDENT_OR_SELF
						|| currentStep.axis_type == AXIS_DESCENDENT) {
					ap = (AutoPilot)currentStep.o;
					//System.out.println("iterating element -->"+ap.getElementName());
					//if (ap.getElementName().matches("a"))
					//System.out.println("depth before iter "+vn.getCurrentDepth());
					b = ap.iterate();
					//if (ap.getElementName().matches("a"))
					//System.out.println("depth after iter "+vn.getCurrentDepth());
					//System.out.println("iterating result --->"+b);
					if (b==true)
						return true;					
					if (currentStep.getPrevStep() == null){					
							xpe.state = XPathExpr.END;
					}
					else {
						vn.pop();
						xpe.state = XPathExpr.BACKWARD;
						currentStep.ft = true;
					}						
				}
			    break;
			  	
			  default: 
			  	throw new XPathException("unknown state");
			
			}
		}

		//return false;
	}
	step currentStep;
	
	public static void main(String[] args) {
		
		XPathTest xpt = new XPathTest();
		
		
		try{
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
			ap.selectElement("a");
			int result;
			while(ap.iterate()){
				System.out.println(" index ---> "+ vn.getCurrentIndex());
				System.out.println(" get current depth ==>" + vn.getCurrentDepth());
			}
			
			System.out.println("=================");
			vn.toElement(VTDNav.R);
			XPathExpr expr = xpt.buildExpr2(vn);
			// navigate and perform node test according to 
			// the XPath expression
			
			//vn.toElement(VTDNav.FC);
			
			while((result = xpt.evalXpathExpr2(vn,expr))!=-1){
				//vn.dumpContext();
				System.out.println("element name -->" + vn.toString(result));
				System.out.println("\tindex ---> "+ result+"  depth ==>"+vn.getCurrentDepth());
			}
			System.out.println("\n+++++++++++++++++++++++\n");
			vn.toElement(VTDNav.R);
			expr = xpt.buildExpr3(vn);
			while((result= xpt.evalXpathExpr2(vn,expr))!=-1){
				//vn.dumpContext();
				System.out.println("element name -->" + vn.toString(result));
				System.out.println("\tindex ---> "+ result+"  depth ==>"+vn.getCurrentDepth());
			}
			
		}catch(ParseException e){
			
		}catch(NavException e){
			
		}catch(XPathException e){
			
		}
		catch (java.io.IOException e){
			
		}
		
	}
}
