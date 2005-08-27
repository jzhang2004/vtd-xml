import com.ximpleware.*;

class LocationPathExpr extends Expr{

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
		
		public void reset(){
			state = START;
			Step temp = s;
			fib.clear();
			while(temp!=null){
				temp.reset();
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
		        try{	
				a = (evalNodeSet(vn) != -1);
			}catch (Exception e){
			}
			reset();
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
			reset();
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
			reset();
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
		
		public int evalNodeSet(VTDNav vn) throws XPathEvalException, NavException
		{

		AutoPilot ap = null;
		int result;

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

		while (true) {
			switch (currentStep.axis_type) {

			case AxisType.CHILD:
				switch ( state) {
				case  START:
					if (currentStep.nt.testType != NodeTest.TEXT){
						vn.toElement(VTDNav.FIRST_CHILD);
						 state =  END;
						do {
							if (currentStep.eval(vn)) {
								if (currentStep.getNextStep() != null){
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
					} else {
						if (currentStep.getNextStep() != null){
							state =  FORWARD;
							currentStep = currentStep.getNextStep();
						} else {
							state =  TERMINAL;
							result = vn.getText();
							if (result!=-1){
								return result;
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
							currentStep = currentStep.getPrevStep();
						} else {
							//vn.toElement(VTDNav.P);
							currentStep = currentStep.getPrevStep();
						}
					} else {
						if (currentStep.getNextStep() != null){
							 state =  FORWARD;
							currentStep = currentStep.getNextStep();
						} else {
							 state =  TERMINAL;
							result = vn.getText();
							if (result!=-1){
								return result;
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
						} else if (currentStep.getPrevStep() == null)
							 state =  END;
						else {
							 state =  BACKWARD;
							vn.toElement(VTDNav.P);
							currentStep = currentStep.getPrevStep();
						}
					}else {
						if (currentStep.getPrevStep() == null)
							 state =  END;
						else {
							 state =  BACKWARD;
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

						if (currentStep.getPrevStep() == null)
							 state =  END;

						else {
							vn.toElement(VTDNav.P);
							 state =  BACKWARD;
							currentStep = currentStep.getPrevStep();
						}
					}else {
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
						 state =  END;
					} else {
						vn.pop2();
						currentStep.set_ft(true);
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
				  		if ( state ==  START)
				  			 state =  END;
				  		else 
				  			 state =  BACKWARD;
				  	}
				    break;
				  	
				  case  END:
				  	currentStep = null;
				  	// reset();
				  	break;
				  	
				  case  BACKWARD:
				  	if (currentStep.eval(vn)){
				  		if (currentStep.getNextStep()!=null){
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
				  		if (currentStep.getPrevStep()!=null){
				  			 state =  BACKWARD;
				  			currentStep= currentStep.getPrevStep();
				  		}else{
				  			 state =  END;				  			
				  		}
				  	}
				  	break;
				  
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
				  		vn.pop2();
				  	}else if ( state ==  BACKWARD){
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
				  		vn.pop2();
				  	}else if ( state ==  BACKWARD){
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
						if (currentStep.get_ft() == true) {
							currentStep.o = ap = new AutoPilot(vn);
						    ap.selectAttr(currentStep.nt.nodeName);
							currentStep.set_ft(false);
						}
						if ( state ==  START)
							 state =  END;
						int temp;
						while( (temp = ap.iterateAttr()) != -1){
							if (currentStep.evalPredicates(vn)){
								break;
							}							
						}
						if (temp == -1){
							currentStep.set_ft(true);
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
							return temp;
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
	
}

