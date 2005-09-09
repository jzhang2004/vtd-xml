/*
 * Created on Aug 27, 2005
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package com.ximpleware;

import com.ximpleware.xpath.Expr;
import com.ximpleware.xpath.XPathEvalException;

/**
 * @author jimmy zhang
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class PathExpr extends Expr {

	Expr fe;
	LocationPathExpr lpe;
	boolean first_time;
	int evalState;
	FastIntBuffer fib;
	
	public int getPositon(){
		return fib.size();
	}
	public PathExpr(Expr f, LocationPathExpr l){
		fe = f;
		lpe = l;
		first_time = true;
		evalState = 0;
		fib = new FastIntBuffer(8);
	}
	
	public boolean evalBoolean(VTDNav vn) {
		boolean a = false;
		vn.push2();
		// record teh stack size
		int size = vn.contextStack2.size;
        try{	
			a = (evalNodeSet(vn) != -1);
		}catch (Exception e){
		}
		//rewind stack
		vn.contextStack2.size = size;
		reset(vn);
		vn.pop2();
		return a;
	}

	/* (non-Javadoc)
	 * @see com.ximpleware.xpath.Expr#evalNumber(com.ximpleware.VTDNav)
	 */
	public double evalNumber(VTDNav vn) {
		double d;
		int a = -1;
		vn.push2();
		int size = vn.contextStack2.size;
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
		vn.contextStack2.size = size;
		reset(vn);
		vn.pop2();
		try{
			if (a!=-1) return vn.parseDouble(a);
		}catch (NavException e){
		}
		return Double.NaN;
	}

	public int evalNodeSet(VTDNav vn) throws XPathEvalException, NavException {
		int a,b;
		while (true) {
			switch (evalState) {
			case 0: //this state is teh initial state;
				a = fe.evalNodeSet(vn);
				if (a == -1){
					evalState =4;
				}
				else
					evalState = 1;
				break;
			case 1: // fe returns valid value
				vn.push2();
				a = lpe.evalNodeSet(vn);
				if (a == -1) {
					lpe.reset(vn);					
					evalState = 3;
				} else {
					evalState = 2;
					if (isUnique(a))
					return a;
				}
				break;
			case 2:
				a = lpe.evalNodeSet(vn);
				if (a == -1) {
					lpe.reset(vn);
					evalState = 3;
				} else{
					if (isUnique(a))
						return a;
					//return a;
				}
				break;
			case 3:
				vn.pop2();
				a = fe.evalNodeSet(vn);
				if (a == -1)
					evalState = 4;
				else
					evalState = 2;
				break;
			case 4:
				return -1;
			default:
				throw new XPathEvalException(
						"Invalid state evaluating PathExpr");
			}
		}
		//return -1;
	}


	public String evalString(VTDNav vn) {
		vn.push2();
		int size = vn.contextStack2.size;
		int a = -1;
		try {
			a = evalNodeSet(vn);
			if (a != -1) {
				if (vn.getTokenType(a) == VTDNav.TOKEN_ATTR_NAME) {
					a++;
				}
				if (vn.getTokenType(a) == VTDNav.TOKEN_STARTING_TAG) {
					a = vn.getText();
				}
			}
		} catch (Exception e) {
		}
		vn.contextStack2.size = size;
		reset(vn);
		vn.pop2();
		try {
			if (a != -1)
				return vn.toString(a);
		} catch (NavException e) {
		}
		return null;	
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
	
	public void reset(VTDNav vn) {
		
		fe.reset(vn);
		lpe.reset(vn);
		fib.clear();
		evalState = 0;

	}


	public String toString() {
		
		return "("+fe +")/" + lpe;
	}


	public boolean isNumerical() {
		return false;
	}


	public boolean isNodeSet() {
		return false;
	}
	
	public boolean isString(){
	    return false;
	}
	
	public boolean isBoolean(){
	    return false;
	}
	
	// to support computer context size 
	// needs to add 
	public boolean requireContextSize(){
	    return false;
	}
	
	public void setContextSize(int size){	    
	}
	
	public void setPosition(int pos){
	    
	}
}
