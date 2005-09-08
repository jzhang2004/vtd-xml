/* 
 * Copyright (C) 2002-2004 XimpleWare, info@ximpleware.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

package com.ximpleware;
import com.ximpleware.xpath.*;

public class LocationPathExpr extends Expr{

		public static final int ABSOLUTE_PATH =0,
					RELATIVE_PATH =1;
		Step s;
		Step currentStep;
		int pathType;
		int state;
	        FastIntBuffer fib; // for uniqueness checking
		public static final int START = 0, // initial state
					   END= 1,   // return to begin
				      TERMINAL= 2, // no more next step
				      FORWARD = 3, // 
				      BACKWARD= 4, //
					  DOWN  = 5; // 
								
		public LocationPathExpr(){
			state = START;
			s = null;
			pathType = RELATIVE_PATH;
			currentStep = null;
			fib = new FastIntBuffer(8);// page size 256 = 2^ 8
		}
		
		public void setStep(Step st){
			s = st;
		}
		
		public void setPathType(int ptype){
			pathType = ptype;
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
		
		public void reset(VTDNav vn){
			state = START;
			Step temp = s;
			fib.clear();
			currentStep = null;
			while(temp!=null){
				temp.reset(vn);
				temp = temp.nextS;
			}	
		}

		public String toString(){
			String st = "";
			Step ts = s;
			if (pathType == LocationPathExpr.ABSOLUTE_PATH){
				st = st+"/";
			}
			if (ts == null)
				return st;
			else 
				return st + ts;
		}

		public boolean evalBoolean(VTDNav vn){
			boolean a = false;
			vn.push2();
			// record stack size
			int size = vn.contextStack2.size;
		        try{	
				a = (evalNodeSet(vn) != -1);
			vn.contextStack2.size = size;
			}catch (Exception e){
			}
			//rewind stack
			reset(vn);
			vn.pop2();
			return a;
		}

		public double evalNumber(VTDNav vn){
			double d;
			int a = -1;
			vn.push2();
		        try {
			  a =evalNodeSet(vn);
			  if (a!=-1){
			  	if (vn.getTokenType(a)== VTDNav.TOKEN_ATTR_NAME){
				  a ++;
			  	}else if (vn.getTokenType(a)== VTDNav.TOKEN_STARTING_TAG) {
				  a = vn.getText();
			  	}
			  }			  
			} catch (Exception e){
				
			}
			reset(vn);
			vn.pop2();
			try{
				if (a!=-1) return vn.parseDouble(a);
			}catch (NavException e){
			}
			return Double.NaN;
		}
	
		public String evalString(VTDNav vn){ 	
			vn.push2();
		        int a = -1;	
			try {
			  a =evalNodeSet(vn);
			  if (a!=-1){
			  if (vn.getTokenType(a)== VTDNav.TOKEN_ATTR_NAME){
				  a ++;
			  }
			  if (vn.getTokenType(a)== VTDNav.TOKEN_STARTING_TAG) {
				  a = vn.getText();
			  }
			  }
			  
			} catch (Exception e){
			}
			reset(vn);
			vn.pop2();
			try{
				if (a!=-1) return vn.toString(a);
			}catch (NavException e){
			}
	        return null;	
		}

		public boolean  isNodeSet(){
			return true;
		}

		public boolean  isNumerical(){
			return false;
		}
		
		public int evalNodeSet2(VTDNav vn) throws XPathEvalException, NavException
		{

		AutoPilot ap = null;
		int result;
		int temp;

		boolean b = false;
		if (currentStep == null) {
			if ( pathType ==  ABSOLUTE_PATH){
				vn.toElement(VTDNav.ROOT);
				vn.toElement(VTDNav.PARENT);
			}
			currentStep =  s;
			if (currentStep == null){
				if (  state ==  START){
					 state =  END;
					return 0;
				}
				else{
					return -1;
				}
			}
		}
//		if (s!=null){
//			System.out.println("<<<<<<>>>>>>");
//			System.out.println(s.toString());
//			System.out.println("<<<<<<>>>>>>");
//		}
		while (true) {
			switch (currentStep.axis_type) {

			case AxisType.CHILD:
				switch ( state) {
				case  START:
					if (currentStep.nt.testType != NodeTest.TEXT){
						 b = vn.toElement(VTDNav.FIRST_CHILD);
						 state =  END;
						 if (b == true){
						do {
							if (currentStep.eval(vn)) {
								if (currentStep.getNextStep() != null){
									//currentStep.position++;
									 state =  FORWARD;
									currentStep = currentStep.getNextStep();
								}
								else {
									 state =  TERMINAL;
									result = vn.getCurrentIndex();
									if ( isUnique(result))
										return result;
								}
							break;
							}
						} while (vn.toElement(VTDNav.NS));
						if (state == END)
						 vn.toElement(VTDNav.PARENT);
						 }
					
					} else {
						// for text() right now predicates are not evaled
						if (vn.getAtTerminal()==true){
							state = END;
						}else {
							result = vn.getText();
							if (result != -1){
								vn.setAtTerminal(true);
								if (currentStep.getNextStep() != null){
									state =  FORWARD;
									currentStep = currentStep.getNextStep();
								} else {
									state =  TERMINAL;
									//result = vn.getText();
									if ( isUnique(result)){
										//vn.setAtTerminal(true);
										return result;
									}
								}					
							}else {							
								state = END;							
							}
						}
					}
					break;

				case  END:
					currentStep =null;
					// reset();
					return -1;

				case  FORWARD:
					if (currentStep.nt.testType != NodeTest.TEXT) {
						//currentStep = currentStep.getNextStep();
						 state =  BACKWARD;
						forward: if (vn.toElement(VTDNav.FC)) {
							do {
								if (currentStep.eval(vn)) {
									if (currentStep.getNextStep() != null) {
										 state =  FORWARD;
										currentStep = currentStep.getNextStep();
									} else {
										 state =  TERMINAL;
										result = vn.getCurrentIndex();
										if ( isUnique(result))
											return result;
									}
									break forward;
								}
							} while (vn.toElement(VTDNav.NS));
							vn.toElement(VTDNav.P);
							currentStep.resetP(vn);
							currentStep = currentStep.getPrevStep();
						} else {
							//vn.toElement(VTDNav.P);
							currentStep = currentStep.getPrevStep();
						}
					} else {
						if (vn.getAtTerminal() == true){
							state = BACKWARD;
							currentStep = currentStep.getPrevStep();
						}else {
							result = vn.getText();
							if (result != -1){
								vn.setAtTerminal(true);
								if (currentStep.getNextStep() != null){
									state =  FORWARD;
									currentStep = currentStep.getNextStep();
								} else {
									state =  TERMINAL;
									//result = vn.getText();
									if (isUnique(result)){
										return result;
									}
								}					
							}else {
									state = BACKWARD;
									currentStep = currentStep.getPrevStep();
							}
						}						
					}
					break;

				case  BACKWARD:
					if (currentStep.nt.testType != NodeTest.TEXT) {
						//currentStep = currentStep.getPrevStep();
						b = false;
						while (vn.toElement(VTDNav.NS)) {
							if (currentStep.eval(vn)) {
								b = true;
								break;
							}
						}
						if (b == true) {
							 state =  FORWARD;
							currentStep = currentStep.getNextStep();
						} else if (currentStep.getPrevStep() == null){
							currentStep.resetP(vn);
							vn.toElement(VTDNav.P);
							 state =  END;
						}
						else {
							currentStep.resetP(vn);
							 state =  BACKWARD;
							vn.toElement(VTDNav.P);
							currentStep = currentStep.getPrevStep();
						}
					}else {
						vn.setAtTerminal(false);
						if (currentStep.getPrevStep() == null)
							 state =  END;
						else {
							 state =  BACKWARD;
							 //vn.setAtTerminal(false);
							currentStep = currentStep.getPrevStep();
						}
					}
					break;

				case  TERMINAL:
					if (currentStep.nt.testType != NodeTest.TEXT) {
						while (vn.toElement(VTDNav.NS)) {
							if (currentStep.eval(vn)) {
								// state =  TERMINAL;
								result = vn.getCurrentIndex();
								if ( isUnique(result))
									return result;
							}
						}
						currentStep.resetP(vn);
						if (currentStep.getPrevStep() == null){
							 state =  END;
							 vn.toElement(VTDNav.P);
						}
						else {
							vn.toElement(VTDNav.P);
							
							 state =  BACKWARD;
							currentStep = currentStep.getPrevStep();
						}
					}else {
						currentStep.resetP(vn);
						vn.setAtTerminal(false);
						if (currentStep.getPrevStep() == null)
							 state =  END;

						else {
							 state =  BACKWARD;
							currentStep = currentStep.getPrevStep();
						}
					}
					break;

				default:
					throw new XPathEvalException("unknown state");
				}
				break;

			case AxisType.DESCENDANT_OR_SELF:
			case AxisType.DESCENDANT:
			case AxisType.PRECEDING:								
			case AxisType.FOLLOWING:
				switch ( state) {
				case  START:
				case  FORWARD:
					// currentStep.o;
					String helper = null;
					if (currentStep.nt.testType == NodeTest.NODE){
						helper = "*";
					}else {
						helper = currentStep.nt.nodeName;
					}
					if (currentStep.o == null)
						currentStep.o = ap = new AutoPilot(vn);
					else
						ap = (AutoPilot) currentStep.o;
					if (currentStep.get_ft() == true) {

						if (currentStep.axis_type == AxisType.DESCENDANT_OR_SELF )
							if (currentStep.nt.testType == NodeTest.NODE)
								ap.setSpecial(true);
							else
								ap.setSpecial(false);
						//currentStep.o = ap = new AutoPilot(vn);
					    if (currentStep.axis_type == AxisType.DESCENDANT_OR_SELF)
							ap.selectElement(helper);
						else if (currentStep.axis_type == AxisType.DESCENDANT)
							ap.selectElement_D(helper);
						else if (currentStep.axis_type == AxisType.PRECEDING)
							ap.selectElement_P(helper);
						else 
							ap.selectElement_F(helper);
						currentStep.set_ft(false);
					}
					if ( state ==  START)
						 state =  END;

					vn.push2(); // not the most efficient. good for now
					//System.out.println("  --++ push in //");
					b = false;
					while(ap.iterate()){
						if (currentStep.evalPredicates(vn)){
							b = true;
							break;
						}
					}
					if (b == false) {
						vn.pop2();
						//System.out.println("  --++ pop in //");
						currentStep.set_ft(true);
						currentStep.resetP(vn);
						if ( state ==  FORWARD){
							 state =  BACKWARD;
							currentStep = currentStep.getPrevStep();							
						}						
					} else {
						if (currentStep.getNextStep() != null){
							 state =  FORWARD;
							currentStep = currentStep.getNextStep();
						}
						else {
							//vn.pop();
							 state =  TERMINAL;
							result = vn.getCurrentIndex();
							if ( isUnique(result))
								return result;
						}
					}
					break;
					
				case  END:
					currentStep = null;
					// reset();
					return -1;

				case  BACKWARD:
					//currentStep = currentStep.getPrevStep();
					ap = (AutoPilot) currentStep.o;
					//vn.push();
					b = false;
					while(ap.iterate()){
						if (currentStep.evalPredicates(vn)){
							b = true;
							break;
						}
					}
					if (b == false) {
						vn.pop2();
						currentStep.set_ft(true);
						currentStep.resetP(vn);
						//System.out.println("  --++ pop in //");
						if (currentStep.getPrevStep() != null) {
							 state =  BACKWARD;
							currentStep = currentStep.getPrevStep();
						} else
							 state =  END;
					} else {
						if (currentStep.getNextStep() != null) {
							//vn.push();
							//System.out.println("  --++ push in //");
							 state =  FORWARD;
							currentStep = currentStep.getNextStep();
						} else {
							 state =  TERMINAL;
							result = vn.getCurrentIndex();
							if ( isUnique(result))
								return result;
						}
					}
					break;

				case  TERMINAL:
					ap = (AutoPilot) currentStep.o;
					b = false;
					while(ap.iterate()){
						if (currentStep.evalPredicates(vn)){
							b = true;
							break;
						}
					}
					if (b == true){
						if (currentStep.evalPredicates(vn)){
							result = vn.getCurrentIndex();
							if ( isUnique(result))
								return result;
						}
					}
					else if (currentStep.getPrevStep() == null) {
						currentStep.resetP(vn);
						vn.pop2();
						 state =  END;
					} else {
						vn.pop2();
						currentStep.set_ft(true);
						currentStep.resetP(vn);
						//System.out.println("  --++ pop in //");
						 state =  BACKWARD;
						//currentStep.ft = true;
						currentStep = currentStep.getPrevStep();
					}
					break;

				default:
					throw new  XPathEvalException("unknown state");
				}
				break;

			case AxisType.PARENT:
				switch ( state) {
				case  START:
				case  FORWARD:
					// assuming .. without specifying element name
					// makes the qname = null
					//if (vn.matchElement("b")){
					//	System.out.println(" b encountered ==> "+ vn.getCurrentDepth());
					//}
					if (vn.getCurrentDepth() == -1) {
						if ( state ==  START)
							 state =  END;
						else {
							//vn.pop();
							 state =  BACKWARD;
							currentStep = currentStep.getPrevStep();
						}
					} else {
						vn.push2();
						vn.toElement(VTDNav.P); // must return true
						if (currentStep.eval(vn)){
						    if (currentStep.getNextStep() != null) {
							    state =  FORWARD;
							   currentStep = currentStep.getNextStep();
						    } else {
							    state =  TERMINAL;
							   result = vn.getCurrentIndex();
								if ( isUnique(result))
									return result;
						    }
						}else{
							vn.pop2();
							currentStep.resetP(vn);
							if ( state ==  START)
								 state =  END;
							else {								
								 state =  BACKWARD;
								currentStep = currentStep.getPrevStep();
							}
						}
					}

					break;				
					
				case  END:
					currentStep = null;
					// reset();
				    return -1;
					
				case  BACKWARD:
				case  TERMINAL:
					if (currentStep.getPrevStep() == null) {
					    vn.pop2();
						 state =  END;
						break;
					}else {
						vn.pop2();
						 state =  BACKWARD;
						currentStep = currentStep.getPrevStep();
						break;
					}
					
				default:
					throw new  XPathEvalException("unknown state");
				}

				break;

			case AxisType.ANCESTOR: // reverse document order
				switch ( state) {
				case  START:
					    
				    state =  END;
				   if (vn.getCurrentDepth()!=-1){
				   		vn.push2();
						
				   		while(vn.toElement(VTDNav.P)){
				   			if (currentStep.eval(vn)){
				   				if (currentStep.getNextStep() != null){
				   					 state =  FORWARD;
				   					currentStep = currentStep.getNextStep();
				   					break;
				   				}
				   				else {
								    //vn.pop();
				   					 state =  TERMINAL;
				   					result = vn.getCurrentIndex();
									if ( isUnique(result))
										return result;
				   				}
				   			}							
				   		}
				   		if ( state == END){
				   			currentStep.resetP(vn);
				   			vn.pop2();
				   		}
				   }
				   break;
				  	
				case  FORWARD:
				    state =  BACKWARD;
				   	vn.push2();
						
				   	while(vn.toElement(VTDNav.P)){
				   		if (currentStep.eval(vn)){
				   			if (currentStep.getNextStep() != null){
				   				 state =  FORWARD;
				   				currentStep = currentStep.getNextStep();
				   				break;
				   			}
				   			else {
				   				//vn.pop();
				   				 state =  TERMINAL;
				   				result = vn.getCurrentIndex();
								if ( isUnique(result))
									return result;
				   			}
				   		}							
				   	}
				   	if ( state == BACKWARD){
				   		currentStep.resetP(vn);
						vn.pop2();
				   		currentStep=currentStep.getPrevStep();
				   	}
				    
				  	break;
				
				case  END:
					currentStep =null;
					// reset();
				    return -1;
				
				case  BACKWARD:
					b = false;
					vn.push2();

					while (vn.toElement(VTDNav.P)) {
						if (currentStep.eval(vn)) {
							if (currentStep.getNextStep()!= null) {
								 state =  FORWARD;
								currentStep = currentStep.getNextStep();
								b = true;
								break;
							} else {
								//vn.pop();
								 state =  TERMINAL;
								result = vn.getCurrentIndex();
								if ( isUnique(result))
									return result;
							}
						}
					}
					if (b==false){
						vn.pop2();
						if (currentStep.getPrevStep()!=null) {
							currentStep.resetP(vn);
							state =  BACKWARD;
							currentStep = currentStep.getPrevStep();
						}
						else {
							 state =  END;
						}
					}
					break;
				
				case  TERMINAL:
					while (vn.toElement(VTDNav.P)) {
						if (currentStep.eval(vn)) {
							result = vn.getCurrentIndex();
							if ( isUnique(result))
								return result;
						}
					}
					vn.pop2();
					
					if (currentStep.getPrevStep()!=null) {
						currentStep.resetP(vn);
						 state =  BACKWARD;
						currentStep = currentStep.getPrevStep();
					}
					else {
						 state =  END;
					}
					break;
				
				default:
					throw new  XPathEvalException("unknown state");
				}

				break;
				
			case AxisType.ANCESTOR_OR_SELF: // reverse doc order
				switch ( state) {
				case  START:
					
					 state =  END;
					vn.push2();
					
					if (currentStep.get_ft()== true){						
						currentStep.set_ft(false);
						if (currentStep.eval(vn)) {
							if (currentStep.getNextStep() != null) {
								 state =  FORWARD;
								currentStep = currentStep.getNextStep();
								break;
							} else {
								//vn.pop();
								 state =  TERMINAL;
								result = vn.getCurrentIndex();
								if ( isUnique(result))
									return result;
							}
						}
					}
					else {
						while (vn.toElement(VTDNav.P)) {
							if (currentStep.eval(vn)) {
								if (currentStep.getNextStep() != null) {
									 state =  FORWARD;
									currentStep = currentStep.getNextStep();
									break;
								} else {
									//vn.pop();
									 state =  TERMINAL;
									result = vn.getCurrentIndex();
									if ( isUnique(result))
										return result;
								}
							}
						}
					}
					if ( state ==  END) {
						currentStep.resetP(vn);
						vn.pop2();
					}

					break;
					
				case  FORWARD:
					 state =  BACKWARD;
					vn.push2();
					if (currentStep.get_ft() == true) {
						currentStep.set_ft(false);
						if (currentStep.eval(vn)) {
							if (currentStep.getNextStep() != null) {
								 state =  FORWARD;
								currentStep = currentStep.getNextStep();
								break;
							} else {
								//vn.pop();
								 state =  TERMINAL;
								result = vn.getCurrentIndex();
								if ( isUnique(result))
									return result;
							}
						}
					} else {
						while (vn.toElement(VTDNav.P)) {
							if (currentStep.eval(vn)) {
								if (currentStep.getNextStep() != null) {
									 state =  FORWARD;
									currentStep = currentStep.getNextStep();
									break;
								} else {
									//vn.pop();
									 state =  TERMINAL;
									result = vn.getCurrentIndex();
									if ( isUnique(result))
										return result;
								}
							}
						}
					}
					if ( state ==  BACKWARD) {
						currentStep.resetP(vn);
						currentStep.set_ft(true);
						vn.pop2();
						currentStep = currentStep.getPrevStep();
					}
					break;
				
				case  END:
					currentStep = null;
					// reset();
			    	return -1;
					
				
				case  BACKWARD:
					b = false;
					vn.push2();

					while (vn.toElement(VTDNav.P)) {
						if (currentStep.eval(vn)) {
							if (currentStep.getNextStep() != null) {
								 state =  FORWARD;
								currentStep = currentStep.getNextStep();
								b = true;
								break;
							} else {
								//vn.pop();
								 state =  TERMINAL;
								result = vn.getCurrentIndex();
								if ( isUnique(result))
									return result;
							}
						}
					}
					if (b == false) {
						vn.pop2();
						currentStep.resetP(vn);
						if (currentStep.getPrevStep() != null) {
							currentStep.set_ft(true);
							 state =  BACKWARD;
							currentStep = currentStep.getPrevStep();
						} else {
							 state =  END;
						}
					}
					break;
				
				case  TERMINAL:
					while (vn.toElement(VTDNav.P)) {
						if (currentStep.eval(vn)) {
							result = vn.getCurrentIndex();
							if ( isUnique(result))
								return result;
						}
					}
					vn.pop2();
					currentStep.resetP(vn);
					if (currentStep.getPrevStep()!=null) {
						currentStep.set_ft(true);
						 state =  BACKWARD;
						currentStep = currentStep.getPrevStep();
					}
					else {
						 state =  END;
					}
					break;
					
				
				default:
					throw new  XPathEvalException("unknown state");
				}

				break;
				
			case AxisType.SELF:
				switch( state){
				  case  START:
				  case  FORWARD:	
				  	if (currentStep.eval(vn)){
				  		if (currentStep.getNextStep()!=null){
				  			 state =  FORWARD;
				  			currentStep = currentStep.getNextStep();
				  		}
				  		else{
				  			 state =  TERMINAL;
				  			result = vn.getCurrentIndex();
							if ( isUnique(result))
								return result;
				  		}
				  	}else {
				  		currentStep.resetP(vn);
				  		if ( state ==  START)
				  			 state =  END;
				  		else 
				  			 state =  BACKWARD;
				  	}
				    break;
				  	
				  case  END:
				  	currentStep = null;
				  	// reset();
				  	return -1;
				  	
				  case  BACKWARD:
				  	/*if (currentStep.eval(vn)){
				  		if (currentStep.getPrevStep()!=null){
				  			 state =  BACKWARD;
				  			currentStep = currentStep.getPrevStep();
				  		}
				  		else{
				  			 state =  TERMINAL;
				  			result = vn.getCurrentIndex();
							if ( isUnique(result))
								return result;
				  		}
				  	}else {
				  		currentStep.resetP();
				  		if (currentStep.getPrevStep()!=null){
				  			 state =  BACKWARD;
				  			currentStep= currentStep.getPrevStep();
				  		}else{
				  			 state =  END;				  			
				  		}
				  	}
				  	break;*/
				  
				  case  TERMINAL:
				  	if (currentStep.getPrevStep()!=null){
			  			 state =  BACKWARD;
			  			currentStep= currentStep.getPrevStep();
			  		}else{
			  			 state =  END;				  			
			  		}
				  	break;
				  
				  default:
					throw new  XPathEvalException("unknown state");
				}
				break;
				
			case AxisType.FOLLOWING_SIBLING: 
				
				switch( state){
				  case  START:
				  case  FORWARD:
				  	if ( state ==  START)
				  		 state =  END;
				  	else
				  		 state =  BACKWARD;
				  	vn.push2();
				  	while (vn.toElement(VTDNav.NS)){
				  		if (currentStep.eval(vn)){
				  			if (currentStep.getNextStep()!=null){
				  				 state =  FORWARD;
				  				currentStep = currentStep.getNextStep();
				  				break;
				  			} else {
				  				 state =  TERMINAL;
				  				result = vn.getCurrentIndex();
								if ( isUnique(result))
									return result;
				  			}
				  		}
				  	}
				  	
				  	if ( state ==  END){
				  		currentStep.resetP(vn);
				  		vn.pop2();
				  	}else if ( state ==  BACKWARD){
				  		currentStep.resetP(vn);
				  		vn.pop2();
				  		currentStep = currentStep.getPrevStep();				  		
				  	}
				    break;
				  	 
				  case  END:
				  	currentStep = null;
				  	// reset();
				  	return -1;
				  	
				  case  BACKWARD:
				  	while (vn.toElement(VTDNav.NS)){
				  		if (currentStep.eval(vn)){
				  			if (currentStep.getNextStep()!=null){
				  				 state =  FORWARD;
				  				currentStep = currentStep.getNextStep();
				  				b = true;
				  				break;
				  			} else {
				  				 state =  TERMINAL;
				  				result = vn.getCurrentIndex();
								if ( isUnique(result))
									return result;
				  			}
				  		}
				  	}
				    if (b==false){
				    	vn.pop2();
				    	currentStep.resetP(vn);
				    	if (currentStep.getPrevStep()==null){
				    		 state =  END;
				    	}else{
				    		 state =  BACKWARD;
				    		currentStep = currentStep.getPrevStep();
				    	}
				    }
				  	break;
				  
				  case  TERMINAL:
				  	while (vn.toElement(VTDNav.NS)){
				  		if (currentStep.eval(vn)){
				  			// state =  TERMINAL;
				  			result = vn.getCurrentIndex();
							if ( isUnique(result))
								return result;
				  		}
				  	}
				  	vn.pop2();
				  	currentStep.resetP(vn);
				  	if(currentStep.getPrevStep()!=null){
				  		currentStep = currentStep.getPrevStep();
				  		 state =  BACKWARD;
				  	}else{
				  		 state =  END;
				  	}
				  	break;

				  default:
					throw new  XPathEvalException("unknown state");
				}
				break;
				
			case AxisType.PRECEDING_SIBLING:
				switch( state){
				  case  START:
				  case  FORWARD:
				  	if ( state ==  START)
				  		 state =  END;
				  	else
				  		 state =  BACKWARD;
				  	vn.push2();
				  	while (vn.toElement(VTDNav.PS)){
				  		if (currentStep.eval(vn)){
				  			if (currentStep.getNextStep()!=null){
				  				 state =  FORWARD;
				  				currentStep = currentStep.getNextStep();
				  				break;
				  			} else {
				  				 state =  TERMINAL;
				  				result = vn.getCurrentIndex();
								if ( isUnique(result))
									return result;
				  			}
				  		}
				  	}
				  	
				  	if ( state ==  END){
				  		currentStep.resetP(vn);
				  		vn.pop2();
				  	}else if ( state ==  BACKWARD){
				  		currentStep.resetP(vn);
				  		vn.pop2();
				  		currentStep = currentStep.getPrevStep();				  		
				  	}
				  	 break;
				  	 
				  case  END:
				  	currentStep = null;
				  	// reset();
				  	return -1;
				  
				  case  BACKWARD:
				  	while (vn.toElement(VTDNav.PS)){
				  		if (currentStep.eval(vn)){
				  			if (currentStep.getNextStep()!=null){
				  				 state =  FORWARD;
				  				currentStep = currentStep.getNextStep();
				  				b = true;
				  				break;
				  			} else {
				  				 state =  TERMINAL;
				  				result = vn.getCurrentIndex();
								if ( isUnique(result))
									return result;
				  			}
				  		}
				  	}
				    if (b==false){
				    	vn.pop2();
				    	currentStep.resetP(vn);
				    	if (currentStep.getPrevStep()==null){
				    		 state =  END;
				    	}else{
				    		 state =  BACKWARD;
				    		currentStep = currentStep.getPrevStep();
				    	}
				    }
				  	break;
				  
				  case  TERMINAL:
				  	while (vn.toElement(VTDNav.PS)){
				  		if (currentStep.eval(vn)){
				  			// state =  TERMINAL;
				  			result = vn.getCurrentIndex();
							if ( isUnique(result))
								return result;
				  		}
				  	}
				  	vn.pop2();
				  	if(currentStep.getPrevStep()!=null){
				  		currentStep = currentStep.getPrevStep();
				  		 state =  BACKWARD;
				  	}else{
				  		 state =  END;
				  	}
				  	break;
				  
				  default:
					throw new  XPathEvalException("unknown state");
				}
				break;
			

				
			case AxisType.ATTRIBUTE:
				switch( state){
					case  START:
					case  FORWARD:
						if (vn.getAtTerminal()==true){
							if (state ==START)
								state = END;
							else {
								state = BACKWARD;
								currentStep  = currentStep.getPrevStep();
							}
						} else {
						if (currentStep.get_ft() == true) {
							currentStep.o = ap = new AutoPilot(vn);
						    ap.selectAttr(currentStep.nt.nodeName);
							currentStep.set_ft(false);
						}
						if ( state ==  START)
							 state =  END;
						vn.setAtTerminal(true);
						while( (temp = ap.iterateAttr()) != -1){
							
							if (currentStep.evalPredicates(vn)){
								break;
							}							
						}
						if (temp == -1){
							currentStep.set_ft(true);
							currentStep.resetP(vn);
							vn.setAtTerminal(false);
							if ( state ==  FORWARD){
								state =  BACKWARD;
								currentStep = currentStep.getPrevStep();							
							}	
						}else {
							
							if (currentStep.getNextStep() != null){
								 state =  FORWARD;
								currentStep = currentStep.getNextStep();
							}
							else {
								//vn.pop();
								 state =  TERMINAL;
								if ( isUnique(temp))
									return temp;
							}
						
						}
						}
						break;
						
					case  END:
						currentStep = null;
						// reset();
				  		return -1;
				  		
					case  BACKWARD:
						ap = (AutoPilot) currentStep.o;
						//vn.push();
						while( (temp = ap.iterateAttr()) != -1){
							if (currentStep.evalPredicates(vn)){
								break;
							}							
						}
						if (temp == -1) {
							currentStep.set_ft(true);
							currentStep.resetP(vn);
							vn.setAtTerminal(false);
							if (currentStep.getPrevStep() != null) {
								state =  BACKWARD;
								currentStep = currentStep.getPrevStep();
							} else
								state =  END;
						} else {
							if (currentStep.getNextStep() != null) {
								state =  FORWARD;
								currentStep = currentStep.getNextStep();
							} else {
								state =  TERMINAL;
								if ( isUnique(temp))
									return temp;
							}
						}
						break;
						
					case  TERMINAL:
						ap = (AutoPilot) currentStep.o;
						while( (temp = ap.iterateAttr()) != -1){
							if (currentStep.evalPredicates(vn)){
								break;
							}							
						}
						if (temp != -1) 
							if (isUnique(temp))
								return temp;
						vn.setAtTerminal(false);
						currentStep.resetP(vn);
						if (currentStep.getPrevStep() == null) {
							currentStep.set_ft(true);
							 state =  END;
						} else {
							 state =  BACKWARD;
							currentStep.set_ft(true);
							currentStep = currentStep.getPrevStep();
						}
						
						break;					
					
					default:
						throw new  XPathEvalException("unknown state");
				}
				break;
			

			default:
				throw new  XPathEvalException("axis not supported");

			}

		}

	}
		
	int process_child(VTDNav vn)
		throws XPathEvalException,NavException{
	    
	    int result;
	    boolean b = false;
		switch ( state) {
		case  START:
			if (currentStep.nt.testType != NodeTest.TEXT){
			    // if this step contains any predicate that
			    // require context size
			    // needs to precompute the context size
			    // vn.push2();
			    // computerContext();
			    // set contxt();
			    // vn.pop2()
			    // if the context size is zero
			    // get immediately set teh state to end
			    // or backward
				 b = vn.toElement(VTDNav.FIRST_CHILD);
				 state =  END;
				 if (b == true){
				do {
					if (currentStep.eval(vn)) {
						if (currentStep.getNextStep() != null){
							//currentStep.position++;
							 state =  FORWARD;
							currentStep = currentStep.getNextStep();
						}
						else {
							 state =  TERMINAL;
							result = vn.getCurrentIndex();
							if ( isUnique(result)){
								return result;
							}
						}
					break;
					}
				} while (vn.toElement(VTDNav.NS));
				if (state == END)
				 vn.toElement(VTDNav.PARENT);
				 }
			
			} else {
				// for text() right now predicates are not evaled
				if (vn.getAtTerminal()==true){
					state = END;
				}else {
					result = vn.getText();
					if (result != -1){
						vn.setAtTerminal(true);
						if (currentStep.getNextStep() != null){
						    vn.LN = result;
							state =  FORWARD;
							currentStep = currentStep.getNextStep();
						} else {
							state =  TERMINAL;
							//result = vn.getText();
							if ( isUnique(result)){
								//vn.setAtTerminal(true);
							    vn.LN = result;
								return result;
							}
						}					
					}else {							
						state = END;							
					}
				}
			}
			break;

		case  END:
			currentStep =null;
			// reset();
			return -1;

		case  FORWARD:
			if (currentStep.nt.testType != NodeTest.TEXT) {
			    // if this step contains any predicate that
			    // require context size
			    // needs to precompute the context size
			    // vn.push2();
			    // computerContext();
			    // set contxt();
			    // vn.pop2()
			    // if the context size is zero
			    // get immediately set teh state to end
			    // or backward
				//currentStep = currentStep.getNextStep();
				 state =  BACKWARD;
				forward: if (vn.toElement(VTDNav.FC)) {
					do {
						if (currentStep.eval(vn)) {
							if (currentStep.getNextStep() != null) {
								 state =  FORWARD;
								currentStep = currentStep.getNextStep();
							} else {
								 state =  TERMINAL;
								result = vn.getCurrentIndex();
								if ( isUnique(result))
									return result;
							}
							break forward;
						}
					} while (vn.toElement(VTDNav.NS));
					vn.toElement(VTDNav.P);
					currentStep.resetP(vn);
					currentStep = currentStep.getPrevStep();
				} else {
					//vn.toElement(VTDNav.P);
					currentStep = currentStep.getPrevStep();
				}
			} else {
				if (vn.getAtTerminal() == true){
					state = BACKWARD;
					currentStep = currentStep.getPrevStep();
				}else {
					result = vn.getText();
					if (result != -1){
						vn.setAtTerminal(true);
						if (currentStep.getNextStep() != null){
						    vn.LN = result;
							state =  FORWARD;
							currentStep = currentStep.getNextStep();
						} else {
							state =  TERMINAL;
							//result = vn.getText();
							if (isUnique(result)){
							    vn.LN = result;
								return result;
							}
						}					
					}else {
							state = BACKWARD;
							currentStep = currentStep.getPrevStep();
					}
				}						
			}
			break;

		case  BACKWARD:
			if (currentStep.nt.testType != NodeTest.TEXT) {
				//currentStep = currentStep.getPrevStep();
				b = false;
				while (vn.toElement(VTDNav.NS)) {
					if (currentStep.eval(vn)) {
						b = true;
						break;
					}
				}
				if (b == true) {
					 state =  FORWARD;
					currentStep = currentStep.getNextStep();
				} else if (currentStep.getPrevStep() == null){
					currentStep.resetP(vn);
					vn.toElement(VTDNav.P);
					 state =  END;
				}
				else {
					currentStep.resetP(vn);
					 state =  BACKWARD;
					vn.toElement(VTDNav.P);
					currentStep = currentStep.getPrevStep();
				}
			}else {
				vn.setAtTerminal(false);
				if (currentStep.getPrevStep() == null)
					 state =  END;
				else {
					 state =  BACKWARD;
					 //vn.setAtTerminal(false);
					currentStep = currentStep.getPrevStep();
				}
			}
			break;

		case  TERMINAL:
			if (currentStep.nt.testType != NodeTest.TEXT) {
				while (vn.toElement(VTDNav.NS)) {
					if (currentStep.eval(vn)) {
						// state =  TERMINAL;
						result = vn.getCurrentIndex();
						if ( isUnique(result))
							return result;
					}
				}
				currentStep.resetP(vn);
				if (currentStep.getPrevStep() == null){
					 state =  END;
					 vn.toElement(VTDNav.P);
				}
				else {
					vn.toElement(VTDNav.P);
					
					 state =  BACKWARD;
					currentStep = currentStep.getPrevStep();
				}
			}else {
				currentStep.resetP(vn);
				vn.setAtTerminal(false);
				if (currentStep.getPrevStep() == null)
					 state =  END;

				else {
					 state =  BACKWARD;
					currentStep = currentStep.getPrevStep();
				}
			}
			break;

		default:
			throw new XPathEvalException("unknown state");
		}
		return -2;
	}
	
	int process_DDFP(VTDNav vn) 
		throws XPathEvalException, NavException {
	    AutoPilot ap;
	    boolean b = false;
	    int result;
		switch ( state) {
		case  START:
		case  FORWARD:
			// currentStep.o;
		    // if this step contains any predicate that
		    // require context size
		    // needs to precompute the context size
		    // vn.push2();
		    // computerContext();
		    // set contxt();
		    // vn.pop2()
		    // if the context size is zero
		    // get immediately set teh state to end
		    // or backward
			String helper = null;
			if (currentStep.nt.testType == NodeTest.NODE){
				helper = "*";
			}else {
				helper = currentStep.nt.nodeName;
			}
			if (currentStep.o == null)
				currentStep.o = ap = new AutoPilot(vn);
			else
				ap = (AutoPilot) currentStep.o;
			if (currentStep.get_ft() == true) {

				if (currentStep.axis_type == AxisType.DESCENDANT_OR_SELF )
					if (currentStep.nt.testType == NodeTest.NODE)
						ap.setSpecial(true);
					else
						ap.setSpecial(false);
				//currentStep.o = ap = new AutoPilot(vn);
			    if (currentStep.axis_type == AxisType.DESCENDANT_OR_SELF)
					ap.selectElement(helper);
				else if (currentStep.axis_type == AxisType.DESCENDANT)
					ap.selectElement_D(helper);
				else if (currentStep.axis_type == AxisType.PRECEDING)
					ap.selectElement_P(helper);
				else 
					ap.selectElement_F(helper);
				currentStep.set_ft(false);
			}
			if ( state ==  START)
				 state =  END;

			vn.push2(); // not the most efficient. good for now
			//System.out.println("  --++ push in //");
			b = false;
			while(ap.iterate()){
				if (currentStep.evalPredicates(vn)){
					b = true;
					break;
				}
			}
			if (b == false) {
				vn.pop2();
				//System.out.println("  --++ pop in //");
				currentStep.set_ft(true);
				currentStep.resetP(vn);
				if ( state ==  FORWARD){
					 state =  BACKWARD;
					currentStep = currentStep.getPrevStep();							
				}						
			} else {
				if (currentStep.getNextStep() != null){
					 state =  FORWARD;
					currentStep = currentStep.getNextStep();
				}
				else {
					//vn.pop();
					 state =  TERMINAL;
					result = vn.getCurrentIndex();
					if ( isUnique(result))
						return result;
				}
			}
			break;
			
		case  END:
			currentStep = null;
			// reset();
			return -1;

		case  BACKWARD:
			//currentStep = currentStep.getPrevStep();
			ap = (AutoPilot) currentStep.o;
			//vn.push();
			b = false;
			while(ap.iterate()){
				if (currentStep.evalPredicates(vn)){
					b = true;
					break;
				}
			}
			if (b == false) {
				vn.pop2();
				currentStep.set_ft(true);
				currentStep.resetP(vn);
				//System.out.println("  --++ pop in //");
				if (currentStep.getPrevStep() != null) {
					 state =  BACKWARD;
					currentStep = currentStep.getPrevStep();
				} else
					 state =  END;
			} else {
				if (currentStep.getNextStep() != null) {
					//vn.push();
					//System.out.println("  --++ push in //");
					 state =  FORWARD;
					currentStep = currentStep.getNextStep();
				} else {
					 state =  TERMINAL;
					result = vn.getCurrentIndex();
					if ( isUnique(result))
						return result;
				}
			}
			break;

		case  TERMINAL:
			ap = (AutoPilot) currentStep.o;
			b = false;
			while(ap.iterate()){
				if (currentStep.evalPredicates(vn)){
					b = true;
					break;
				}
			}
			if (b == true){
				if (currentStep.evalPredicates(vn)){
					result = vn.getCurrentIndex();
					if ( isUnique(result))
						return result;
				}
			}
			else if (currentStep.getPrevStep() == null) {
				currentStep.resetP(vn);
				vn.pop2();
				 state =  END;
			} else {
				vn.pop2();
				currentStep.set_ft(true);
				currentStep.resetP(vn);
				//System.out.println("  --++ pop in //");
				 state =  BACKWARD;
				//currentStep.ft = true;
				currentStep = currentStep.getPrevStep();
			}
			break;

		default:
			throw new  XPathEvalException("unknown state");
		}
		return -2;
	}
	
	int process_parent(VTDNav vn)
		throws XPathEvalException, NavException{
	    int result;
		switch ( state) {
		case  START:
		case  FORWARD:
			// assuming .. without specifying element name
			// makes the qname = null
			//if (vn.matchElement("b")){
			//	System.out.println(" b encountered ==> "+ vn.getCurrentDepth());
			//}
		    
		    // if this step contains any predicate that
		    // require context size
		    // needs to precompute the context size
		    // vn.push2();
		    // computerContext();
		    // set contxt();
		    // vn.pop2()
		    // if the context size is zero
		    // get immediately set teh state to end
		    // or backward
			if (vn.getCurrentDepth() == -1) {
				if ( state ==  START)
					 state =  END;
				else {
					//vn.pop();
					 state =  BACKWARD;
					currentStep = currentStep.getPrevStep();
				}
			} else {
				vn.push2();
				vn.toElement(VTDNav.P); // must return true
				if (currentStep.eval(vn)){
				    if (currentStep.getNextStep() != null) {
					    state =  FORWARD;
					   currentStep = currentStep.getNextStep();
				    } else {
					    state =  TERMINAL;
					   result = vn.getCurrentIndex();
						if ( isUnique(result))
							return result;
				    }
				}else{
					vn.pop2();
					currentStep.resetP(vn);
					if ( state ==  START)
						 state =  END;
					else {								
						 state =  BACKWARD;
						currentStep = currentStep.getPrevStep();
					}
				}
			}

			break;				
			
		case  END:
			currentStep = null;
			// reset();
		    return -1;
			
		case  BACKWARD:
		case  TERMINAL:
			if (currentStep.getPrevStep() == null) {
			    vn.pop2();
				 state =  END;
				break;
			}else {
				vn.pop2();
				 state =  BACKWARD;
				currentStep = currentStep.getPrevStep();
				break;
			}
			
		default:
			throw new  XPathEvalException("unknown state");
		}

		return -2;
	    
	}
	
	int process_ancestor( VTDNav vn)
		throws XPathEvalException, NavException{
	    int result;
	    boolean b = false;
		switch ( state) {
		case  START:
		    // if this step contains any predicate that
		    // require context size
		    // needs to precompute the context size
		    // vn.push2();
		    // computerContext();
		    // set contxt();
		    // vn.pop2()
		    // if the context size is zero
		    // get immediately set teh state to end
		    // or backward    
		    state =  END;
		   if (vn.getCurrentDepth()!=-1){
		   		vn.push2();
				
		   		while(vn.toElement(VTDNav.P)){
		   			if (currentStep.eval(vn)){
		   				if (currentStep.getNextStep() != null){
		   					 state =  FORWARD;
		   					currentStep = currentStep.getNextStep();
		   					break;
		   				}
		   				else {
						    //vn.pop();
		   					 state =  TERMINAL;
		   					result = vn.getCurrentIndex();
							if ( isUnique(result))
								return result;
		   				}
		   			}							
		   		}
		   		if ( state == END){
		   			currentStep.resetP(vn);
		   			vn.pop2();
		   		}
		   }
		   break;
		  	
		case  FORWARD:
		    // if this step contains any predicate that
		    // require context size
		    // needs to precompute the context size
		    // vn.push2();
		    // computerContext();
		    // set contxt();
		    // vn.pop2()
		    // if the context size is zero
		    // get immediately set teh state to end
		    // or backward
		    state =  BACKWARD;
		   	vn.push2();
				
		   	while(vn.toElement(VTDNav.P)){
		   		if (currentStep.eval(vn)){
		   			if (currentStep.getNextStep() != null){
		   				 state =  FORWARD;
		   				currentStep = currentStep.getNextStep();
		   				break;
		   			}
		   			else {
		   				//vn.pop();
		   				 state =  TERMINAL;
		   				result = vn.getCurrentIndex();
						if ( isUnique(result))
							return result;
		   			}
		   		}							
		   	}
		   	if ( state == BACKWARD){
		   		currentStep.resetP(vn);
				vn.pop2();
		   		currentStep=currentStep.getPrevStep();
		   	}
		    
		  	break;
		
		case  END:
			currentStep =null;
			// reset();
		    return -1;
		
		case  BACKWARD:
			b = false;
			vn.push2();

			while (vn.toElement(VTDNav.P)) {
				if (currentStep.eval(vn)) {
					if (currentStep.getNextStep()!= null) {
						 state =  FORWARD;
						currentStep = currentStep.getNextStep();
						b = true;
						break;
					} else {
						//vn.pop();
						 state =  TERMINAL;
						result = vn.getCurrentIndex();
						if ( isUnique(result))
							return result;
					}
				}
			}
			if (b==false){
				vn.pop2();
				if (currentStep.getPrevStep()!=null) {
					currentStep.resetP(vn);
					state =  BACKWARD;
					currentStep = currentStep.getPrevStep();
				}
				else {
					 state =  END;
				}
			}
			break;
		
		case  TERMINAL:
			while (vn.toElement(VTDNav.P)) {
				if (currentStep.eval(vn)) {
					result = vn.getCurrentIndex();
					if ( isUnique(result))
						return result;
				}
			}
			vn.pop2();
			
			if (currentStep.getPrevStep()!=null) {
				currentStep.resetP(vn);
				 state =  BACKWARD;
				currentStep = currentStep.getPrevStep();
			}
			else {
				 state =  END;
			}
			break;
		
		default:
			throw new  XPathEvalException("unknown state");
		}

		return -2;	
	}
	
	int process_ancestor_or_self(VTDNav vn)
		throws XPathEvalException,NavException{
	    boolean b = false;
	    int result;
		switch ( state) {
		case  START:
		    // if this step contains any predicate that
		    // require context size
		    // needs to precompute the context size
		    // vn.push2();
		    // computerContext();
		    // set contxt();
		    // vn.pop2()
		    // if the context size is zero
		    // get immediately set teh state to end
		    // or backward
			 state =  END;
			vn.push2();
			
			if (currentStep.get_ft()== true){						
				currentStep.set_ft(false);
				if (currentStep.eval(vn)) {
					if (currentStep.getNextStep() != null) {
						 state =  FORWARD;
						currentStep = currentStep.getNextStep();
						break;
					} else {
						//vn.pop();
						 state =  TERMINAL;
						result = vn.getCurrentIndex();
						if ( isUnique(result))
							return result;
					}
				}
			}
			else {
				while (vn.toElement(VTDNav.P)) {
					if (currentStep.eval(vn)) {
						if (currentStep.getNextStep() != null) {
							 state =  FORWARD;
							currentStep = currentStep.getNextStep();
							break;
						} else {
							//vn.pop();
							 state =  TERMINAL;
							result = vn.getCurrentIndex();
							if ( isUnique(result))
								return result;
						}
					}
				}
			}
			if ( state ==  END) {
				currentStep.resetP(vn);
				vn.pop2();
			}

			break;
			
		case  FORWARD:
		    // if this step contains any predicate that
		    // require context size
		    // needs to precompute the context size
		    // vn.push2();
		    // computerContext();
		    // set contxt();
		    // vn.pop2()
		    // if the context size is zero
		    // get immediately set teh state to end
		    // or backward
			 state =  BACKWARD;
			vn.push2();
			if (currentStep.get_ft() == true) {
				currentStep.set_ft(false);
				if (currentStep.eval(vn)) {
					if (currentStep.getNextStep() != null) {
						 state =  FORWARD;
						currentStep = currentStep.getNextStep();
						break;
					} else {
						//vn.pop();
						 state =  TERMINAL;
						result = vn.getCurrentIndex();
						if ( isUnique(result))
							return result;
					}
				}
			} else {
				while (vn.toElement(VTDNav.P)) {
					if (currentStep.eval(vn)) {
						if (currentStep.getNextStep() != null) {
							 state =  FORWARD;
							currentStep = currentStep.getNextStep();
							break;
						} else {
							//vn.pop();
							 state =  TERMINAL;
							result = vn.getCurrentIndex();
							if ( isUnique(result))
								return result;
						}
					}
				}
			}
			if ( state ==  BACKWARD) {
				currentStep.resetP(vn);
				currentStep.set_ft(true);
				vn.pop2();
				currentStep = currentStep.getPrevStep();
			}
			break;
		
		case  END:
			currentStep = null;
			// reset();
	    	return -1;
			
		
		case  BACKWARD:
			b = false;
			vn.push2();

			while (vn.toElement(VTDNav.P)) {
				if (currentStep.eval(vn)) {
					if (currentStep.getNextStep() != null) {
						 state =  FORWARD;
						currentStep = currentStep.getNextStep();
						b = true;
						break;
					} else {
						//vn.pop();
						 state =  TERMINAL;
						result = vn.getCurrentIndex();
						if ( isUnique(result))
							return result;
					}
				}
			}
			if (b == false) {
				vn.pop2();
				currentStep.resetP(vn);
				if (currentStep.getPrevStep() != null) {
					currentStep.set_ft(true);
					 state =  BACKWARD;
					currentStep = currentStep.getPrevStep();
				} else {
					 state =  END;
				}
			}
			break;
		
		case  TERMINAL:
			while (vn.toElement(VTDNav.P)) {
				if (currentStep.eval(vn)) {
					result = vn.getCurrentIndex();
					if ( isUnique(result))
						return result;
				}
			}
			vn.pop2();
			currentStep.resetP(vn);
			if (currentStep.getPrevStep()!=null) {
				currentStep.set_ft(true);
				 state =  BACKWARD;
				currentStep = currentStep.getPrevStep();
			}
			else {
				 state =  END;
			}
			break;
			
		
		default:
			throw new  XPathEvalException("unknown state");
		}

		return -2;
	}
	int process_self(VTDNav vn)
		throws XPathEvalException,NavException{
	    //boolean b = false;
	    int result;
		switch( state){
		  case  START:
		  case  FORWARD:
		      
			    // if this step contains any predicate that
			    // require context size
			    // needs to precompute the context size
			    // vn.push2();
			    // computerContext();
			    // set contxt();
			    // vn.pop2()
			    // if the context size is zero
			    // get immediately set teh state to end
			    // or backward
		      
		  	if (currentStep.eval(vn)){
		  		if (currentStep.getNextStep()!=null){
		  			 state =  FORWARD;
		  			currentStep = currentStep.getNextStep();
		  		}
		  		else{
		  			 state =  TERMINAL;
		  			 if (vn.atTerminal == true)
		  			     result = vn.LN;
		  			 else 
		  			     result = vn.getCurrentIndex();
					if ( isUnique(result))
						return result;
		  		}
		  	}else {
		  		currentStep.resetP(vn);
		  		if ( state ==  START)
		  			 state =  END;
		  		else 
		  			 state =  BACKWARD;
		  	}
		    break;
		  	
		  case  END:
		  	currentStep = null;
		  	// reset();
		  	return -1;
		  	
		  case  BACKWARD:
		  case  TERMINAL:
		  	if (currentStep.getPrevStep()!=null){
	  			 state =  BACKWARD;
	  			currentStep= currentStep.getPrevStep();
	  		}else{
	  			 state =  END;				  			
	  		}
		  	break;
		  
		  default:
			throw new  XPathEvalException("unknown state");
		}
		return -2;
	}
	
	int process_following_sibling(VTDNav vn)
		throws XPathEvalException,NavException{
	    boolean b = false;
	    int result;
	    
		switch( state){
		  case  START:
		  case  FORWARD:
		  	if ( state ==  START)
		  		 state =  END;
		  	else
		  		 state =  BACKWARD;
		  	vn.push2();
		  	while (vn.toElement(VTDNav.NS)){
		  		if (currentStep.eval(vn)){
		  			if (currentStep.getNextStep()!=null){
		  				 state =  FORWARD;
		  				currentStep = currentStep.getNextStep();
		  				break;
		  			} else {
		  				 state =  TERMINAL;
		  				result = vn.getCurrentIndex();
						if ( isUnique(result))
							return result;
		  			}
		  		}
		  	}
		  	
		  	if ( state ==  END){
		  		currentStep.resetP(vn);
		  		vn.pop2();
		  	}else if ( state ==  BACKWARD){
		  		currentStep.resetP(vn);
		  		vn.pop2();
		  		currentStep = currentStep.getPrevStep();				  		
		  	}
		    break;
		  	 
		  case  END:
		  	currentStep = null;
		  	// reset();
		  	return -1;
		  	
		  case  BACKWARD:
		  	while (vn.toElement(VTDNav.NS)){
		  		if (currentStep.eval(vn)){
		  			if (currentStep.getNextStep()!=null){
		  				 state =  FORWARD;
		  				currentStep = currentStep.getNextStep();
		  				b = true;
		  				break;
		  			} else {
		  				 state =  TERMINAL;
		  				result = vn.getCurrentIndex();
						if ( isUnique(result))
							return result;
		  			}
		  		}
		  	}
		    if (b==false){
		    	vn.pop2();
		    	currentStep.resetP(vn);
		    	if (currentStep.getPrevStep()==null){
		    		 state =  END;
		    	}else{
		    		 state =  BACKWARD;
		    		currentStep = currentStep.getPrevStep();
		    	}
		    }
		  	break;
		  
		  case  TERMINAL:
		  	while (vn.toElement(VTDNav.NS)){
		  		if (currentStep.eval(vn)){
		  			// state =  TERMINAL;
		  			result = vn.getCurrentIndex();
					if ( isUnique(result))
						return result;
		  		}
		  	}
		  	vn.pop2();
		  	currentStep.resetP(vn);
		  	if(currentStep.getPrevStep()!=null){
		  		currentStep = currentStep.getPrevStep();
		  		 state =  BACKWARD;
		  	}else{
		  		 state =  END;
		  	}
		  	break;

		  default:
			throw new  XPathEvalException("unknown state");
		}
		return -2;
	}
	
	int process_preceding_sibling(VTDNav vn)
		throws XPathEvalException,NavException {
	    boolean b = false;
	    int result;
	    switch(state){
		  case  START:
		  case  FORWARD:
		      
			    // if this step contains any predicate that
			    // require context size
			    // needs to precompute the context size
			    // vn.push2();
			    // computerContext();
			    // set contxt();
			    // vn.pop2()
			    // if the context size is zero
			    // get immediately set teh state to end
			    // or backward
		      
		  	if ( state ==  START)
		  		 state =  END;
		  	else
		  		 state =  BACKWARD;
		  	vn.push2();
		  	while (vn.toElement(VTDNav.PS)){
		  		if (currentStep.eval(vn)){
		  			if (currentStep.getNextStep()!=null){
		  				 state =  FORWARD;
		  				currentStep = currentStep.getNextStep();
		  				break;
		  			} else {
		  				 state =  TERMINAL;
		  				result = vn.getCurrentIndex();
						if ( isUnique(result))
							return result;
		  			}
		  		}
		  	}
		  	
		  	if ( state ==  END){
		  		currentStep.resetP(vn);
		  		vn.pop2();
		  	}else if ( state ==  BACKWARD){
		  		currentStep.resetP(vn);
		  		vn.pop2();
		  		currentStep = currentStep.getPrevStep();				  		
		  	}
		  	 break;
		  	 
		  case  END:
		  	currentStep = null;
		  	// reset();
		  	return -1;
		  
		  case  BACKWARD:
		  	while (vn.toElement(VTDNav.PS)){
		  		if (currentStep.eval(vn)){
		  			if (currentStep.getNextStep()!=null){
		  				 state =  FORWARD;
		  				currentStep = currentStep.getNextStep();
		  				b = true;
		  				break;
		  			} else {
		  				 state =  TERMINAL;
		  				result = vn.getCurrentIndex();
						if ( isUnique(result))
							return result;
		  			}
		  		}
		  	}
		    if (b==false){
		    	vn.pop2();
		    	currentStep.resetP(vn);
		    	if (currentStep.getPrevStep()==null){
		    		 state =  END;
		    	}else{
		    		 state =  BACKWARD;
		    		currentStep = currentStep.getPrevStep();
		    	}
		    }
		  	break;
		  
		  case  TERMINAL:
		  	while (vn.toElement(VTDNav.PS)){
		  		if (currentStep.eval(vn)){
		  			// state =  TERMINAL;
		  			result = vn.getCurrentIndex();
					if ( isUnique(result))
						return result;
		  		}
		  	}
		  	vn.pop2();
		  	if(currentStep.getPrevStep()!=null){
		  		currentStep = currentStep.getPrevStep();
		  		 state =  BACKWARD;
		  	}else{
		  		 state =  END;
		  	}
		  	break;
		  
		  default:
			throw new  XPathEvalException("unknown state");
		}
		return -2;
	}
	
	int process_attribute(VTDNav vn)
		throws XPathEvalException,NavException {
	    AutoPilot ap = null;
	    boolean b = false;
	    int result;
	    int temp;
		switch( state){
		case  START:
		case  FORWARD:
		    // if this step contains any predicate that
		    // require context size
		    // needs to precompute the context size
		    // vn.push2();
		    // computerContext();
		    // set contxt();
		    // vn.pop2()
		    // if the context size is zero
		    // get immediately set teh state to end
		    // or backward
			if (vn.getAtTerminal()==true){
				if (state ==START)
					state = END;
				else {
					state = BACKWARD;
					currentStep  = currentStep.getPrevStep();
				}
			} else {
			if (currentStep.get_ft() == true) {
				currentStep.o = ap = new AutoPilot(vn);
			    ap.selectAttr(currentStep.nt.nodeName);
				currentStep.set_ft(false);
			}
			if ( state ==  START)
				 state =  END;
			vn.setAtTerminal(true);
			while( (temp = ap.iterateAttr()) != -1){
				
				if (currentStep.evalPredicates(vn)){
					break;
				}							
			}
			if (temp == -1){
				currentStep.set_ft(true);
				currentStep.resetP(vn);
				vn.setAtTerminal(false);
				if ( state ==  FORWARD){
					state =  BACKWARD;
					currentStep = currentStep.getPrevStep();							
				}	
			}else {
				
				if (currentStep.getNextStep() != null){
				    vn.LN = temp;
   				    state =  FORWARD;
					currentStep = currentStep.getNextStep();
				}
				else {
					//vn.pop();
					 state =  TERMINAL;
					if ( isUnique(temp)){
					    vn.LN = temp;
						return temp;
					}
				}
			
			}
			}
			break;
			
		case  END:
			currentStep = null;
			// reset();
	  		return -1;
	  		
		case  BACKWARD:
			ap = (AutoPilot) currentStep.o;
			//vn.push();
			while( (temp = ap.iterateAttr()) != -1){
				if (currentStep.evalPredicates(vn)){
					break;
				}							
			}
			if (temp == -1) {
				currentStep.set_ft(true);
				currentStep.resetP(vn);
				vn.setAtTerminal(false);
				if (currentStep.getPrevStep() != null) {
					state =  BACKWARD;
					currentStep = currentStep.getPrevStep();
				} else
					state =  END;
			} else {
				if (currentStep.getNextStep() != null) {
					state =  FORWARD;
					currentStep = currentStep.getNextStep();
				} else {
					state =  TERMINAL;
					if ( isUnique(temp)){
					    vn.LN = temp;
						return temp;
					}
				}
			}
			break;
			
		case  TERMINAL:
			ap = (AutoPilot) currentStep.o;
			while( (temp = ap.iterateAttr()) != -1){
				if (currentStep.evalPredicates(vn)){
					break;
				}							
			}
			if (temp != -1) 
				if (isUnique(temp)){
				    vn.LN = temp;
					return temp;
				}
			vn.setAtTerminal(false);
			currentStep.resetP(vn);
			if (currentStep.getPrevStep() == null) {
				currentStep.set_ft(true);
				 state =  END;
			} else {
				 state =  BACKWARD;
				currentStep.set_ft(true);
				currentStep = currentStep.getPrevStep();
			}
			
			break;					
		
		default:
			throw new  XPathEvalException("unknown state");
	}
	return -2;
	}
	
    public int evalNodeSet(VTDNav vn) 
    	throws NavException,XPathEvalException{
        int result;
		if (currentStep == null) {
			if ( pathType ==  ABSOLUTE_PATH){
				vn.toElement(VTDNav.ROOT);
				vn.toElement(VTDNav.PARENT);
			}
			currentStep =  s;
			if (currentStep == null){
				if (  state ==  START){
					 state =  END;
					return 0;
				}
				else{
					return -1;
				}
			}
		}
		
		while (true) {
			switch (currentStep.axis_type) {

			case AxisType.CHILD:
			    if ( (result = process_child(vn))!=-2)
				   return result;
			    break;
			case AxisType.DESCENDANT_OR_SELF:
			case AxisType.DESCENDANT:
			case AxisType.PRECEDING:								
			case AxisType.FOLLOWING:
			    if ((result = process_DDFP(vn))!= -2)
			        return result;
			    break;
			case AxisType.PARENT:
			    if ((result = process_parent(vn))!= -2)
			        return result;
			    break;
			case AxisType.ANCESTOR:
			    if ((result = process_ancestor(vn))!= -2)
			        return result;
			    break;
			case AxisType.ANCESTOR_OR_SELF:
			    if ((result = process_ancestor_or_self(vn))!= -2)
			        return result;
			    break;
			case AxisType.SELF:
			    if ((result = process_self(vn))!= -2)
			        return result;
			    break;
			case AxisType.FOLLOWING_SIBLING:
			    if ((result = process_following_sibling(vn))!= -2)
			        return result;
			    break;
			case AxisType.PRECEDING_SIBLING:
			    if ((result = process_preceding_sibling(vn))!= -2)
			        return result;
			    break;
			case AxisType.ATTRIBUTE:
			    if ((result = process_attribute(vn))!= -2)
			        return result;
			    break;
			default:
			    throw new XPathEvalException("axis not supported");
			}
		}
        
    }
    
	public boolean isString(){
	    return false;
	}
	
	public boolean isBoolean(){
	    return false;
	}
	
	// to support computer context size 
	// needs to add 
	// public boolean needContextSize();
	// public boolean SetContextSize(int contextSize);
}

