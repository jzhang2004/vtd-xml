/* 
 * Copyright (C) 2002-2006 XimpleWare, info@ximpleware.com
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
import com.ximpleware.xpath.Expr;
import com.ximpleware.xpath.XPathEvalException;
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
	public final static int UNION = 13;

	public final static int BUF_SZ_EXP = 7; 
	protected int op;
	boolean isNumerical;
	boolean isBoolean;
	
	protected Expr left;
	protected Expr right;
	
	protected FastIntBuffer fib1, fib2;
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
		fib1 = fib2 = null;
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
	}
	public String toString(){
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

	
	public boolean evalBoolean(VTDNav vn){
	    int i,i1=0, s1, s2;
	    int stackSize;
	    Expr e1, e2;
	    int t;
	    boolean b = false;
		switch(op){
			case OR: return left.evalBoolean(vn) || right.evalBoolean(vn);
			case AND:return left.evalBoolean(vn) && right.evalBoolean(vn);
			case EQ:
			case NE: return computeEQNE(op,vn);			
			case LE: return left.evalNumber(vn) <= right.evalNumber(vn);
			case GE: return left.evalNumber(vn) >= right.evalNumber(vn);
			case LT: return left.evalNumber(vn) < right.evalNumber(vn);
			case GT: return left.evalNumber(vn) > right.evalNumber(vn);
			default: double dval = evalNumber(vn);
				 if (dval ==-0.0 || dval ==+0.0 || Double.isNaN(dval))
					 return false;
				 return true;
		}
	}

	public double evalNumber(VTDNav vn){
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
		
	public int evalNodeSet(VTDNav vn) throws XPathEvalException {
		throw new XPathEvalException("BinaryExpr can't eval to a node set!");
	}
	
    public String evalString(VTDNav vn){
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

	public void reset(VTDNav vn){left.reset(vn); right.reset(vn);};

	public boolean  isNodeSet(){
		return false;
	}

	public boolean  isNumerical(){
		return isNumerical;
	}
	
	public boolean isString(){
	    return false;
	}
	
	public boolean isBoolean(){
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


	private boolean computeEQNE(int op, VTDNav vn){
	  //int i, t, i1 = 0, stackSize, s1, s2;
        String st1, st2;
        if (left.isNodeSet() && right.isNodeSet()) {
            return compNodeSetNodeSet(left, right, vn, op);
        } else {
            if (left.isNumerical() && right.isNodeSet()){
                return compNumericalNodeSet(left, right, vn, op);
            }
            if (left.isNodeSet() && right.isNumerical()) {
                return compNumericalNodeSet(right, left, vn, op);
            }
            if (left.isString() && right.isNodeSet()){
                return compStringNodeSet(left, right, vn, op);
            }
            if (left.isNodeSet() && right.isString()) {
                return compStringNodeSet(right, left, vn, op);
            }
        }
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
        if (st1 == null || st2 == null)
            if (op == EQ)
                return false;
            else
                return true;

        return (op == EQ) ? (st1.equals(st2))
                : (!st1.equals(st2));
	}
	
	public boolean requireContextSize(){
	    return left.requireContextSize() || right.requireContextSize();
	}
	
	public void setContextSize(int size){
	    left.setContextSize(size);
	    right.setContextSize(size);
	}
	public void setPosition(int pos){
	    left.setPosition(pos);
	    right.setPosition(pos);
	}
	
	// this function computes the case where one expr is a node set, the other is a string
	private boolean compStringNodeSet(Expr left, Expr right, VTDNav vn,int op){
	     int i,t,i1 = 0,stackSize, s1,s2;
		 String st1,st2;
	     try {	     
	        vn.push2();
            stackSize = vn.contextStack2.size;
            st1 = left.evalString(vn);
            if (st1 != null)
                while ((i = right.evalNodeSet(vn)) != -1) {
                    t = vn.getTokenType(i);
                    if (t == VTDNav.TOKEN_STARTING_TAG) {
                        i1 = vn.getText();
                        if (i1 == -1)
                            break;
                        t = vn.getTokenType(i1);
                        if (i1 == VTDNav.TOKEN_CHARACTER_DATA) {
                            if (vn.matchTokenString(i1, st1)) {
                                vn.contextStack2.size = stackSize;
                                vn.pop2();
                                left.reset(vn);
                                right.reset(vn);
                                if (op == EQ)
                                    return true;
                                else
                                    return false;
                            }
                        } else {
                            if (vn.matchRawTokenString(i1, st1)) {
                                vn.contextStack2.size = stackSize;
                                vn.pop2();
                                left.reset(vn);
                                right.reset(vn);
                                if (op == EQ)
                                    return true;
                                else
                                    return false;
                            }
                        }
                    } else if (t == VTDNav.TOKEN_ATTR_NAME
                            || t == VTDNav.TOKEN_ATTR_NS) {
                        if (vn.matchTokenString(i + 1, st1)) {
                            vn.contextStack2.size = stackSize;
                            vn.pop2();
                            left.reset(vn);
                            right.reset(vn);
                            if (op == EQ)
                                return true;
                            else
                                return false;
                        }
                    } else if (t == VTDNav.TOKEN_CHARACTER_DATA
                            || t == VTDNav.TOKEN_CDATA_VAL) {
                        if (vn.matchTokenString(i, st1)) {
                            vn.contextStack2.size = stackSize;
                            vn.pop2();
                            left.reset(vn);
                            right.reset(vn);
                            if (op == EQ)
                                return true;
                            else
                                return false;
                        }
                    } else if (t == VTDNav.TOKEN_CDATA_VAL) {
                        if (vn.matchRawTokenString(i, st1)) {
                            vn.contextStack2.size = stackSize;
                            vn.pop2();
                            left.reset(vn);
                            right.reset(vn);
                            if (op == EQ)
                                return true;
                            else
                                return false;
                        }
                    }
                }
            vn.contextStack2.size = stackSize;
            vn.pop2();
            left.reset(vn);
            right.reset(vn);
            if (op == EQ)
                return false;
            else
                return true;
	               
	      } catch (Exception e) {
	          //fib1.clear();
	          //fib2.clear();
	          throw new RuntimeException("Undefined behavior");
	      }
	}
	
	// this function computes the boolean when one expression is node set
	// the other is numerical
	private boolean compNumericalNodeSet(Expr left, Expr right, VTDNav vn, int op ){
	     int i, t, i1 = 0, stackSize, s1, s2;
        try {
            vn.push2();
            stackSize = vn.contextStack2.size;
            while ((i = right.evalNodeSet(vn)) != -1) {
                t = vn.getTokenType(i);
                if (t == VTDNav.TOKEN_STARTING_TAG) {
                    i1 = vn.getText();
                    if (i1 == -1)
                        break;
                    if (vn.parseDouble(i1) == left.evalNumber(vn)) {
                        vn.contextStack2.size = stackSize;
                        vn.pop2();
                        left.reset(vn);
                        right.reset(vn);
                        if (op == EQ)
                            return true;
                        else
                            return false;
                    }
                } else if (t == VTDNav.TOKEN_ATTR_NAME
                        || t == VTDNav.TOKEN_ATTR_NS) {
                    if (vn.parseDouble(i + 1) == left.evalNumber(vn)) {
                        vn.contextStack2.size = stackSize;
                        vn.pop2();
                        left.reset(vn);
                        right.reset(vn);
                        if (op == EQ)
                            return true;
                        else
                            return false;
                    }
                } else if (t == VTDNav.TOKEN_CHARACTER_DATA
                        || t == VTDNav.TOKEN_CDATA_VAL) {
                    if (vn.parseDouble(i) == left.evalNumber(vn)) {
                        vn.contextStack2.size = stackSize;
                        vn.pop2();
                        left.reset(vn);
                        right.reset(vn);
                        if (op == EQ)
                            return true;
                        else
                            return false;
                    }
                }
            }
            vn.contextStack2.size = stackSize;
            vn.pop2();
            left.reset(vn);
            right.reset(vn);
            if (op == EQ)
                return false;
            else
                return true;
        } catch (Exception e) {
            //fib1.clear();
            //fib2.clear();
            throw new RuntimeException("Undefined behavior");
        }
	}
	
	// this method compare node set with another node set
	private boolean compNodeSetNodeSet(Expr left, Expr right, VTDNav vn, int op){
	    int i,t,i1 = 0,stackSize, s1,s2; 
	    try {
	          if (fib1 == null)
	              fib1 = new FastIntBuffer(BUF_SZ_EXP);
	          if (fib2 == null)
	              fib2 = new FastIntBuffer(BUF_SZ_EXP);
	          vn.push2();
	          while ((i = left.evalNodeSet(vn)) != -1) {
	              t = vn.getTokenType(i);
	              if (t == VTDNav.TOKEN_STARTING_TAG)
	                  i1 = vn.getText();
	                  if (i1!=-1)
	                  fib1.append(i1);
	              else if (t == VTDNav.TOKEN_ATTR_NAME
	                      || t == VTDNav.TOKEN_ATTR_NS)
	              	fib1.append(i+1);
	              else if (t == VTDNav.TOKEN_CHARACTER_DATA
	                      || t == VTDNav.TOKEN_CDATA_VAL)
	                  fib1.append(i);
	          }
	          vn.pop2();
	          vn.push2();
	          while ((i = right.evalNodeSet(vn)) != -1) {
	              t = vn.getTokenType(i);
	              if (t == VTDNav.TOKEN_STARTING_TAG)
	                  fib2.append(vn.getText());
	              else if (t == VTDNav.TOKEN_ATTR_NAME
	                      || t == VTDNav.TOKEN_ATTR_NS)
	              	fib2.append(i+1);
	              else if (t == VTDNav.TOKEN_CHARACTER_DATA
	                      || t == VTDNav.TOKEN_CDATA_VAL)
	                  fib2.append(i);
	          }
	          vn.pop2();
	          s1 = fib1.size();
	          s2 = fib2.size();

	          // start a while loop comparison
	          for (int j = 0; j < s1; j++) {
	              for (int k = 0; k < s2; k++) {
	                  if (vn.matchTokens(fib1.intAt(j), vn, fib2.intAt(k))) {
	                      fib1.clear();
	                      fib2.clear();
	                      if (op == EQ)
	                          return true;
	                      else 
	                          return false;
	                  }
	              }
	          }
	          fib1.clear();
	          fib2.clear();
	          if (op == EQ)
	              return false;
	          else
	              return true;

	      } catch (Exception e) {
	          fib1.clear();
	          fib2.clear();
	          throw new RuntimeException("Undefined behavior");
	      }
	}
	public void adjust(int n){}
}
