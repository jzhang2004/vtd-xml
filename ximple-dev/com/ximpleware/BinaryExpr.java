/* 
 * Copyright (C) 2002-2015 XimpleWare, info@ximpleware.com
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
/*VTD-XML is protected by US patent 7133857, 7260652, an 7761459*/
/*All licenses to any parties in litigation with XimpleWare have been expressly terminated. No new license, and no renewal of any revoked license, 
 * is granted to those parties as a result of re-downloading software from this or any other website*/
package com.ximpleware;
/**
 * The parser.java uses this class to contruct the corresponding
 * AST for XPath expression when there are two operands and one
 * operator
 * 
 */
public class BinaryExpr extends Expr {
	public final static int ADD = 0;
	public final static int SUB = 1;
	public final static int MULT = 2;
	public final static int DIV = 3;
	public final static int MOD = 4;
	public final static int OR = 5;
	public final static int AND = 6;
	public final static int EQ = 7;
	public final static int NE = 8;
	public final static int LE = 9;
	public final static int GE = 10;
	public final static int LT = 11;
	public final static int GT = 12;
	//public final static int UNION = 13;

	public final static byte NS_NS =0;
	public final static byte NS_N = 1;
	public final static byte NS_S = 2;
	public final static byte NS_B = 3;
	public final static byte N_NS =4;
	public final static byte N_N = 5;
	public final static byte N_S = 6;
	public final static byte N_B = 7;
	public final static byte S_NS =8;
	public final static byte S_N = 9;
	public final static byte S_S = 10;
	public final static byte S_B = 11;
	public final static byte B_NS =12;
	public final static byte B_N = 13;
	public final static byte B_S = 14;
	public final static byte B_B = 15;
	
	public final static int BUF_SZ_EXP = 7; 
	protected int op;
	boolean isNumerical;
	boolean isBoolean;
	
	protected Expr left;
	protected Expr right;
	
	protected FastIntBuffer fib1;
	protected byte compType;
	/**
	 * constructor
	 * @param l
	 * @param o
	 * @param r
	 */
	public BinaryExpr ( Expr l, int o, Expr r) {
		op = o;
		left = l;
		right = r;
		fib1 =  null;
		//cacheable =false;
		// precompute the comparison type
		switch(op){
		 	case ADD:
			case SUB:
			case MULT:
			case DIV:
			case MOD: isNumerical = true; isBoolean = false; break;
			case OR :
			case AND:
			case EQ:
			case NE:
			case LE:
			case GE:
			case LT:
			case GT: isNumerical = false; isBoolean = true;
			default:
		}
		compType = computeCompType();
	}
	/**
	 * compute the right op code for binary expression evaluation
	 */
	final  public byte computeCompType(){
		if (left.isNodeSet()){
			if (right.isNodeSet())
				return NS_NS;
			if (right.isNumerical())
				return NS_N;
			if (right.isString())
				return NS_S;
			return NS_B;
		}
		if (left.isNumerical()){
			if(right.isNodeSet())
				return N_NS;
			if (right.isNumerical())
				return N_N;
			if (right.isString())
				return N_S;
			return N_B;
		}
		if (left.isString()){
			if(right.isNodeSet())
				return S_NS;
			if (right.isNumerical())
				return S_N;
			if (right.isString())
				return S_S;
			return S_B;
		}
		if(right.isNodeSet())
			return B_NS;
		if (right.isNumerical())
			return B_N;
		if (right.isString())
			return B_S;
		return B_B;
	}
	
	final public String toString(){
		String os;
		switch(op){
			case ADD: os = " + "; break;
			case SUB: os = " - "; break;
			case MULT: os = " * "; break;
			case DIV: os = " / "; break;
			case MOD: os = " mod "; break;
			case OR : os = " or ";break;
			case AND: os = " and "; break;
			case EQ: os = " = "; break;
			case NE: os = " != "; break;
			case LE: os = " <= "; break;
			case GE: os = " >= "; break;
			case LT: os = " < "; break;
			default: os = " > "; break;
			 
		}
		
		return "("+ left + os + right+")";
	}

	
	final public boolean evalBoolean(VTDNav vn){
	    //int i,i1=0;
	    //int stackSize;
	    //Expr e1, e2;
	    //int t;
	    //boolean b = false;
		switch(op){
			case OR: return left.evalBoolean(vn) || right.evalBoolean(vn);
			case AND:return left.evalBoolean(vn) && right.evalBoolean(vn);
			case EQ:
			case NE: 		
			case LE: 
			case GE: 
			case LT: 
			case GT: return computeComp(op,vn);	
			default: double dval = evalNumber(vn);
				 if (dval ==-0.0 || dval ==+0.0 || Double.isNaN(dval))
					 return false;
				 return true;
		}
	}

	final public double evalNumber(VTDNav vn){
		switch(op){
			case ADD: return left.evalNumber(vn) + right.evalNumber(vn);
			case SUB: return left.evalNumber(vn) - right.evalNumber(vn);
			case MULT:return left.evalNumber(vn) * right.evalNumber(vn);
			case DIV: return left.evalNumber(vn) / right.evalNumber(vn);
			case MOD: return left.evalNumber(vn) % right.evalNumber(vn);
			default	: if (evalBoolean(vn) == true)
					  return 1;
				  return 0;

		}
	}
		
	final public int evalNodeSet(VTDNav vn) throws XPathEvalException {
		throw new XPathEvalException("BinaryExpr can't eval to a node set!");
	}
	
	final public String evalString(VTDNav vn){
		if(isNumerical()){
		    
		    double d = evalNumber(vn);
		    if (d==(long)d){
		        return ""+(long)d;
		    }
		    else 
		        return ""+d;
		} else {
		    boolean b = evalBoolean(vn);
		    if (b)
		        return "true";
		    else
		        return "false";
		}
	}

	final public void reset(VTDNav vn){
		left.reset(vn); 
		right.reset(vn); 
		//cached = false; 
		/*if (cachedNodeSet != null){
			cachedNodeSet.clear();
		}*/
	};

	final public boolean  isNodeSet(){
		return false;
	}

	final public boolean  isNumerical(){
		return isNumerical;
	}
	
	final public boolean isString(){
	    return false;
	}
	
	final public boolean isBoolean(){
	    return isBoolean;
	}
	// to support computation of context size 
	// needs to add 
	// public boolean needContextSize();
	// public boolean SetContextSize(int contextSize);
	//If both objects to be compared are node-sets, then 
	//the comparison will be true if and only if there is 
	//a node in the first node-set and a node in the second 
	//node-set such that the result of performing the comparison 
	//on the string-values of the two nodes is true. If one 
	//object to be compared is a node-set and the other is a 
	//number, then the comparison will be true if and only if 
	//there is a node in the node-set such that the result of 
	//performing the comparison on the number to be compared and on
	//the result of converting the string-value of that node to a 
	//number using the number function is true. If one object to be 
	//compared is a node-set and the other is a string, then the 
	//comparison will be true if and only if there is a node in 
	//the node-set such that the result of performing the comparison 
	//on the string-value of the node and the other string is true. 
	//If one object to be compared is a node-set and the other is a boolean, 
	//then the comparison will be true if and only if the result of 
	//performing the comparison on the boolean and on the result of 
	//converting the node-set to a boolean using the boolean function is true.

	//When neither object to be compared is a node-set and the operator 
	//is = or !=, then the objects are compared by converting them to a 
	//common type as follows and then comparing them. If at least one object 
	//to be compared is a boolean, then each object to be compared is 
	//converted to a boolean as if by applying the boolean function. 
	//Otherwise, if at least one object to be compared is a number, then 
	//each object to be compared is converted to a number as if by applying 
	//the number function. Otherwise, both objects to be compared are 
	//converted to strings as if by applying the string function. The = 
	//comparison will be true if and only if the objects are equal; the 
	//!= comparison will be true if and only if the objects are not equal. 
	//Numbers are compared for equality according to IEEE 754 [IEEE 754]. Two 
	//booleans are equal if either both are true or both are false. Two strings 
	//are equal if and only if they consist of the same sequence of UCS characters.


	final private boolean computeComp(int op, VTDNav vn){
	  //int i, t, i1 = 0, stackSize, s1, s2;
        String st1, st2;
        switch(compType){
        case NS_NS:return compNodeSetNodeSet(left, right, vn, op);
        case NS_N:return compNodeSetNumerical(left, right, vn, op);
        case NS_S:return compNodeSetString(left, right, vn, op);
        //case NS_B:
        case N_NS:return compNumericalNodeSet(left, right, vn, op);
        //case N_N:   break;
        //case N_S:   break;
        //case N_B:
        case S_NS:return compStringNodeSet(left, right, vn, op);
        //case S_N:
        //case S_S:
        //case S_B:
        //case B_NS:
        //case B_N:
        //case B_S:
        //default:break;
        }
        /*if (left.isNodeSet() && right.isNodeSet()) {
            return compNodeSetNodeSet(left, right, vn, op);
        } else {
            if (left.isNumerical() && right.isNodeSet()){
                return compNumericalNodeSet(left, right, vn, op);
            }
            if (left.isNodeSet() && right.isNumerical()) {
                //return compNumericalNodeSet(right, left, vn, op);
                return compNodeSetNumerical(left, right, vn, op);
            }
            if (left.isString() && right.isNodeSet()){
                return compStringNodeSet(left, right, vn, op);
            }
            if (left.isNodeSet() && right.isString()) {
                //return compStringNodeSet(right, left, vn, op);
                return compNodeSetString(left, right, vn, op);
            }
        }*/
        if (op==EQ || op==NE){
            if (left.isBoolean() || right.isBoolean()) {
                if (op == EQ)
                    return left.evalBoolean(vn) == right.evalBoolean(vn);
                else
                    return left.evalBoolean(vn) != right.evalBoolean(vn);
            }

            if (left.isNumerical() || right.isNumerical()) {
                if (op == EQ)
                    return left.evalNumber(vn) == right.evalNumber(vn);
                else
                    return left.evalNumber(vn) != right.evalNumber(vn);
            }

            st1 = left.evalString(vn);
            st2 = right.evalString(vn);
            /*if (st1 == null || st2 == null)
                if (op == EQ)
                    return false;
                else
                    return true;*/

            return (op == EQ) ? (st1.equals(st2)) : (!st1.equals(st2));
        }
        return compNumbers(left.evalNumber(vn),right.evalNumber(vn),op);
        
	}
	
	final public boolean requireContextSize(){
	    return left.requireContextSize() || right.requireContextSize();
	}
	
	final public void setContextSize(int size){
	    left.setContextSize(size);
	    right.setContextSize(size);
	}
	final public void setPosition(int pos){
	    left.setPosition(pos);
	    right.setPosition(pos);
	}
	
	// this function computes the case where one expr is a node set, the other is a string
	
	final private boolean compNodeSetString(Expr left, Expr right, VTDNav vn,int op){
	     int i, i1 = 0, stackSize;
	     String s;	     
	     
       try {
           s = right.evalString(vn);
           vn.push2();
           stackSize = vn.contextStack2.size;
           while ((i = left.evalNodeSet(vn)) != -1) {
        	   int t=vn.getTokenType(i);
        	   if (t!= VTDNav.TOKEN_STARTING_TAG
        			   && t!=VTDNav.TOKEN_DOCUMENT){
        		   i1 = getStringVal(vn,i); 
        		   // if (i1==-1 && s.length()==0)
        		   //return true;
        		   if (i1 != -1) {
        			   boolean b = compareVString1(i1,vn,s,op);
        			   if (b){
        				   left.reset(vn);
        				   vn.contextStack2.size = stackSize;
        				   vn.pop2();
        				   return b;
        			   }
        		   }
        	   }else{        		   
        		   boolean b= vn.XPathStringVal_Matches(i, s);
        		   if (b){
        			   left.reset(vn);
        			   vn.contextStack2.size = stackSize;
        			   vn.pop2();
        			   return b;
        		   }
        	   }
           }           
           vn.contextStack2.size = stackSize;
           vn.pop2();
           left.reset(vn);            
           return false; //compareEmptyNodeSet(op, s); 
       } catch (Exception e) {
           throw new RuntimeException("Undefined behavior");
       }
	}
	
	final private boolean compareEmptyNodeSet(int op, String s){
	    if (op == NE ){
	        if (s.length()==0) {
	            return false;
	        } else 
	            return true;	        
	    }else{
	        if (s.length()==0) {
	            return true;
	        } else 
	            return false;
	    }	        
	}
	final private boolean compStringNodeSet(Expr left, Expr right, VTDNav vn,int op){
	     int i, i1 = 0, stackSize;
	     String s;
        try {
            s = left.evalString(vn);
            vn.push2();
            stackSize = vn.contextStack2.size;
            while ((i = right.evalNodeSet(vn)) != -1) {
            	int t = vn.getTokenType(i);
				if (t!= VTDNav.TOKEN_STARTING_TAG 
						&& t!= VTDNav.TOKEN_DOCUMENT) {
					
					i1 = getStringVal(vn, i);
					if (i1 != -1) {
						boolean b = compareVString2(i1, vn, s, op);
						if (b) {
							right.reset(vn);
							vn.contextStack2.size = stackSize;
							vn.pop2();
							return b;
						}
					}
				}else{
					boolean b = vn.XPathStringVal_Matches(i, s);
					if (b){
						right.reset(vn);
						vn.contextStack2.size = stackSize;
						vn.pop2();
						return b;
					}
				}
            }    
            vn.contextStack2.size = stackSize;
            vn.pop2();
            right.reset(vn);            
            return false; //compareEmptyNodeSet(op, s); 
        } catch (Exception e) {
            throw new RuntimeException("Undefined behavior");
        }
	}
	
	final private boolean compNumbers(double d1, double d2, int op) {
        switch (op) {
        case LE:
            return d1 <= d2;
        case GE:
            return d1 >= d2;
        case LT:
            return d1 < d2;
        case GT:
            return d1 > d2;
        }
        return false;
    }
	// this function computes the boolean when one expression is node set
	// the other is numerical
	final private boolean compNumericalNodeSet(Expr left, Expr right, VTDNav vn, int op ){
	     int i, i1 = 0, stackSize;
	     double d;
        try {
            d = left.evalNumber(vn);
            vn.push2();
            stackSize = vn.contextStack2.size;
            while ((i = right.evalNodeSet(vn)) != -1) {
                //i1 = getStringVal(vn,i); 
                if (compareVNumber1(i,vn,d,op)){
                    right.reset(vn);
                    vn.contextStack2.size = stackSize;
                    vn.pop2();
                    return true;
                }
            }    
            vn.contextStack2.size = stackSize;
            vn.pop2();
            right.reset(vn);            
            return false; 
        } catch (Exception e) {
            throw new RuntimeException("Undefined behavior");
        }
	}
	final private boolean compNodeSetNumerical(Expr left, Expr right, VTDNav vn, int op ){
	     int i,i1 = 0, stackSize;
	     double d;
       try {
           d = right.evalNumber(vn);
           vn.push2();
           stackSize = vn.contextStack2.size;
           while ((i = left.evalNodeSet(vn)) != -1) {
                
               if (compareVNumber2(i,vn,d,op)){
                   left.reset(vn);
                   vn.contextStack2.size = stackSize;
                   vn.pop2();
                   return true;
               }
           }    
           vn.contextStack2.size = stackSize;
           vn.pop2();
           left.reset(vn);            
           return false; 
       } catch (Exception e) {
           throw new RuntimeException("Undefined behavior");
       }
	}
	
	final private int getStringVal(VTDNav vn,int i){
        int i1,t = vn.getTokenType(i);
        if (t == VTDNav.TOKEN_STARTING_TAG){
            i1 = vn.getText();
            return i1;
        }
        else if (t == VTDNav.TOKEN_ATTR_NAME
                || t == VTDNav.TOKEN_ATTR_NS || t==VTDNav.TOKEN_PI_NAME)
        	return i+1;
        else 
            return i;
	}
	
	final private boolean compareVNumber1(int k, VTDNav vn, double d, int op)
	throws NavException {
		double d1;
		int t = vn.getTokenType(k);
		if (t==VTDNav.TOKEN_STARTING_TAG || t==VTDNav.TOKEN_DOCUMENT){
			d1 =vn.XPathStringVal2Double(k);
		}else {
			k = getStringVal(vn,k);
			d1 = vn.parseDouble(k);
		}
	    switch (op){
	    	case EQ:
	    	    return d == d1;
	    	case NE:
	    		return d != d1;
	    	case GE:
	    	    return d >= d1;
	    	case LE:
	    	    return d <= d1;
	    	case GT:
	    	    return d > d1;
	    	default:
	    	    return d < d1;	    	
	    }
	}
	
	final private boolean compareVString1(int k, VTDNav vn, String s, int op)
	throws NavException {
	    int i = vn.compareTokenString(k, s);
        switch (i) {
        case -1:
            if (op == NE || op == LT || op == LE) {
                return true;
            }
            break;
        case 0:
            if (op == EQ || op == LE || op == GE) {
                return true;
            }
            break;
        case 1:
            if (op == NE || op == GE || op == GT) {
                return true;
            }       
        }
        return false;
	}
	final private boolean compareVString2(int k, VTDNav vn, String s, int op)
	throws NavException {
	    int i = vn.compareTokenString(k, s);
        switch(i){        	
        	case -1:
        	    if (op== NE || op == GT || op == GE){
        	        return true;
        	    }
        	    break;
        	case 0: 
        	    if (op==EQ || op == LE || op == GE ){
        	        return true;
        	    }
        	    break;        	    
        	case 1:
        	    if (op == NE || op==LE  || op == LT ){
        	        return true;
        	    }
        }
        return false;
	}
	
	final private boolean compareVNumber2(int k, VTDNav vn, double d, int op)
	throws NavException {
		double d1;
		int t = vn.getTokenType(k);
		if (t==VTDNav.TOKEN_STARTING_TAG || t==VTDNav.TOKEN_DOCUMENT){
			d1 =vn.XPathStringVal2Double(k);
		}else {
			k = getStringVal(vn,k);
			d1 = vn.parseDouble(k);
		}
	    //double d1 = vn.parseDouble(k);
	    switch (op){
	    	case EQ:
	    	    return d1 == d;
	    	case NE:
	    		return d1 != d;
	    	case GE:
	    	    return d1 >= d;
	    	case LE:
	    	    return d1 <= d;
	    	case GT:
	    	    return d1 > d;
	    	default:
	    	    return d1 < d;	    	
	    }
	}
	final private boolean compareVV(int k,  VTDNav vn, int j,int op) 
	throws NavException {
	    int i = vn.XPathStringVal_Matches(k, vn, j);
        switch(i){        	    
        	case 1:
        	    if (op == NE || op==GE  || op == GT ){
        	        return true;
        	    }
        	    break;
        	case 0: 
        	    if (op==EQ || op == LE || op == GE ){
        	        return true;
        	    }
        	    break;
        	case -1:
        	    if (op== NE || op == LT || op == LE){
        	        return true;
        	    }
        }
        return false;
	}
	
	// this method compare node set with another node set
	final private boolean compNodeSetNodeSet(Expr left, Expr right, VTDNav vn, int op){
	    int i,i1,stackSize,s1; 
	    try {
	          if (fib1 == null)
	              fib1 = new FastIntBuffer(BUF_SZ_EXP);
	          vn.push2();
	          stackSize = vn.contextStack2.size;
	          while ((i = left.evalNodeSet(vn)) != -1) {
	              i1 = getStringVal(vn,i);
	              if (i1 != -1)
	              fib1.append(i1);
	          }
	          left.reset(vn);
	          vn.contextStack2.size = stackSize; 
	          vn.pop2();
	          vn.push2();
	          stackSize = vn.contextStack2.size;
	          while ((i = right.evalNodeSet(vn)) != -1) {
	              i1 = getStringVal(vn,i);
	              if (i1 != -1){
	                  s1 = fib1.size;
	                  for (int k = 0; k < s1; k++) {
		                  boolean b = compareVV(fib1.intAt(k),vn,i1,op);
		                  if (b){
		                      fib1.clear();
		                      vn.contextStack2.size = stackSize; 
		        	          vn.pop2();
		        	          right.reset(vn);
		                      return true;
		                  }
		              }
	              }
	          }
	          vn.contextStack2.size = stackSize; 
	          vn.pop2();
	          right.reset(vn);	         
	          fib1.clear();
	          return false;
	      } catch (Exception e) {
	          fib1.clear();
	          throw new RuntimeException("Undefined behavior");
	      }
	}
	final public int adjust(int n){
	    int i = left.adjust(n);
	    int j = right.adjust(n);
	    if (i>j)return i; else return j;
	}
	
	final public boolean isFinal(){
		return left.isFinal() && right.isFinal();
	}
	
	final public void markCacheable(){
		left.markCacheable();
		right.markCacheable();			
	}
	
	final public void markCacheable2(){
		if (left.isFinal() && left.isNodeSet()){
			CachedExpr ce = new CachedExpr(left);
			left = ce;
		} 
		left.markCacheable2();
		if (right.isFinal() && right.isNodeSet()){
			CachedExpr ce = new CachedExpr(right);
			right = ce;
		} 
		right.markCacheable2();
	}
	
	final public void clearCache(){
		left.clearCache();
		right.clearCache();
	}
}
