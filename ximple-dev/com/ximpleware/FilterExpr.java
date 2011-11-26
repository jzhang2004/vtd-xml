/* 
 * Copyright (C) 2002-2011 XimpleWare, info@ximpleware.com
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

import com.ximpleware.xpath.Predicate;

/**
 * 
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class FilterExpr extends Expr {


	public Expr e;
	public Predicate p;
	//FastIntBuffer fib;
	//int stackSize;
	boolean first_time;
	//public int position;
	
	public FilterExpr(Expr l, Predicate pr){
		e = l;
		p = pr;
		//stackSize = 0;
		//position = 1;
		//fib = new FastIntBuffer(8);
		first_time = true;
	}
	/*public int getPositon(){
		return fib.size();
	}*/
	public boolean evalBoolean(VTDNav vn) {
	    //if (e.isBoolean())
	    //    return e.evalBoolean(vn);
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
		//String s = "";
		double d = Double.NaN;
		int a = -1;
        vn.push2();
        int size = vn.contextStack2.size;
        try {
            a = evalNodeSet(vn);
            if (a != -1) {
            	int t = vn.getTokenType(a);
                if (t == VTDNav.TOKEN_ATTR_NAME) {
                	d = vn.parseDouble(a+1);
                } else if (t == VTDNav.TOKEN_STARTING_TAG || t ==VTDNav.TOKEN_DOCUMENT) {
                    String s = vn.getXPathStringVal();
                    d  = Double.parseDouble(s);
                }else if (t == VTDNav.TOKEN_PI_NAME) {
                	if (a+1 < vn.vtdSize || vn.getTokenType(a+1)==VTDNav.TOKEN_PI_VAL)
	                	//s = vn.toString(a+1); 	
                	d = vn.parseDouble(a+1);               
                }else 
                	d = vn.parseDouble(a);
            }
        } catch (Exception e) {

        }
        vn.contextStack2.size = size;
        reset(vn);
        vn.pop2();
        //return s;
		return d;
	}

	public int evalNodeSet(VTDNav vn) 
	throws XPathEvalException, NavException {
	    // if tne predicate require context size
	    // needs to precompute the context size
	    // vn.push2();
	    // computerContext();
	    // set contxt();
	    // vn.pop2()
	    // if the context size is zero
	    // get immediately set teh state to end
	    // or backward
	    if (first_time && p.requireContextSize()){
	        first_time = false;
	        int i = 0;
	        //vn.push2();
	        e.adjust(vn.getTokenCount());
	        while(e.evalNodeSet(vn)!=-1)
	            i++;
	        //vn.pop2();
	        p.setContextSize(i);
	        reset2(vn);
	    }
		int a = e.evalNodeSet(vn);
		while (a!=-1){
			if (p.eval(vn)==true){
				//p.reset();
				return a;				
			}else {
				//p.reset();
				a = e.evalNodeSet(vn);
			}			
		}
		return -1;		
	}

	public String evalString(VTDNav vn) {
	    //if (e.isString())
	   //     return e.evalString(vn);
		/*int a = getStringIndex(vn);
        try {
        	if (a !=-2)
        		return vn.getXPathStringVal();
            if (a != -1)
                return vn.toString(a);
        } catch (NavException e) {
        }
        return "";*/
		String s = "";
		int a = -1;
        vn.push2();
        int size = vn.contextStack2.size;
        try {
            a = evalNodeSet(vn);
            if (a != -1) {
            	int t = vn.getTokenType(a);
                if (t == VTDNav.TOKEN_ATTR_NAME) {
                    s = vn.toString(a+1);
                } else if (t == VTDNav.TOKEN_STARTING_TAG || t ==VTDNav.TOKEN_DOCUMENT) {
                    s = vn.getXPathStringVal();
                }else if (t == VTDNav.TOKEN_PI_NAME) {
                	if (a+1 < vn.vtdSize || vn.getTokenType(a+1)==VTDNav.TOKEN_PI_VAL)
	                	s = vn.toString(a+1);              
                }
            }
        } catch (Exception e) {

        }
        vn.contextStack2.size = size;
        reset(vn);
        vn.pop2();
        return s;
	}

	public void reset(VTDNav vn) {
		reset2(vn);
		//vn.contextStack2.size = stackSize; 
		//position = 1;
		first_time = true;
	}
	
	public void reset2(VTDNav vn){
		e.reset(vn);
		p.reset(vn);
		//fib.clear();
	}


	public String toString() {
		
		return "("+e+") "+p;
	}

	public boolean isNumerical() {
		
		return false;
	}


	public boolean isNodeSet() {
		return true;
	}
	
	/*public boolean isUnique(int i){
		int size = fib.size();
		for (int j=0; j<size;j++){
			if (i == fib.intAt(j))
				return false;
		}
		fib.append(i);
		return true;
	}*/
	
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
	public int adjust(int n){
	    return e.adjust(n);
	    //p.adjust(n);
	}
	
	
}
