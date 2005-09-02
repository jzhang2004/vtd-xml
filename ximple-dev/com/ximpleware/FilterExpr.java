/*
 * Created on Aug 27, 2005
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package com.ximpleware;

import com.ximpleware.xpath.Expr;
import com.ximpleware.xpath.Predicate;
import com.ximpleware.xpath.XPathEvalException;

/**
 * 
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class FilterExpr extends Expr {


	public Expr e;
	public Predicate p;
	FastIntBuffer fib;
	int stackSize;
	//public int position;
	
	public FilterExpr(Expr l, Predicate pr){
		e = l;
		p = pr;
		stackSize = 0;
		//position = 1;
		fib = new FastIntBuffer(8);
	}
	public int getPositon(){
		return fib.size();
	}
	public boolean evalBoolean(VTDNav vn) {
		boolean a = false;
		vn.push2();
		//record stack size
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

	public int evalNodeSet(VTDNav vn) 
	throws XPathEvalException, NavException {
		int a = e.evalNodeSet(vn);
		while (a!=-1){
			if (p.eval(vn)==true){
				//p.reset();
				if (isUnique(a))
					return a;				
			}else {
				//p.reset();
				a = e.evalNodeSet(vn);
			}
			
		}
		return -1;		
	}

	public String evalString(VTDNav vn) {
		vn.push2();
		int size = vn.contextStack2.size;
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
	vn.contextStack2.size = size;
	reset(vn);
	vn.pop2();
	try{
		if (a!=-1) return vn.toString(a);
	}catch (NavException e){
	}
    return null;
	}

	public void reset(VTDNav vn) {
		e.reset(vn);
		p.reset(vn);
		fib.clear();
		vn.contextStack2.size = stackSize; 
		//position = 1;
	}


	public String toString() {
		// TODO Auto-generated method stub
		return "("+e+") "+p;
	}

	public boolean isNumerical() {
		
		return false;
	}


	public boolean isNodeSet() {
		return true;
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

}
