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
import com.ximpleware.xpath.Alist;
import com.ximpleware.xpath.Expr;
import com.ximpleware.xpath.FuncName;
import com.ximpleware.xpath.UnsupportedException;
import com.ximpleware.xpath.XPathEvalException;

public class FuncExpr extends Expr{

	public Alist argumentList;
	public int opCode;
	boolean isNumerical;
	boolean isBoolean;
	boolean isString;
	double d;
	int a;
	int argCount(){
		Alist temp = argumentList;
		int count = 0;
		while(temp!=null){
			count++;
			temp = temp.next;
		}
		return count;
	}
	public FuncExpr(int oc , Alist list){
		a = 0;
	  opCode = oc;
	  argumentList = list;
	  isBoolean = false;
	  isString  = false;
	  //isNodeSet = false;
	  isNumerical = false;
	  switch(opCode){
			case FuncName.LAST: 			isNumerical = true;break;
			case FuncName.POSITION: 		isNumerical = true;break;
			case FuncName.COUNT: 			isNumerical = true;break;
			case FuncName.LOCAL_NAME: 		isString = true; break;
			case FuncName.NAMESPACE_URI: 	isString = true; break;
			case FuncName.NAME: 			isString = true; break;
			case FuncName.STRING: 			isString = true; break;
			case FuncName.CONCAT: 			isString = true; break;
			case FuncName.STARTS_WITH:		isBoolean= true;break;
			case FuncName.CONTAINS: 		isBoolean= true;break;
			case FuncName.SUBSTRING_BEFORE: isString = true; break;
			case FuncName.SUBSTRING_AFTER: 	isString = true; break;
			case FuncName.SUBSTRING: 		isString = true; break;
			case FuncName.STRING_LENGTH: 	isNumerical = true;break;
			case FuncName.NORMALIZE_SPACE: 	isString = true; break;
			case FuncName.TRANSLATE:	 	isString = true;break;
			case FuncName.BOOLEAN: 			isBoolean =true;break;
			case FuncName.NOT: 			    isBoolean =true;break;
			case FuncName.TRUE: 			isBoolean = true;break;
			case FuncName.FALSE: 			isBoolean = true;break;
			case FuncName.LANG: 			isBoolean = true;break;
			case FuncName.NUMBER:			isNumerical = true;break;
			case FuncName.SUM: 			    isNumerical = true;break;
			case FuncName.FLOOR: 			isNumerical = true;break;
			case FuncName.CEILING: 			isNumerical = true;break;
			default:			isNumerical = true;
	  }	  
	}

	public String toString(){
	  if (argumentList == null)
		  return fname()+" ("+")";
	  return fname()+" ("+argumentList +")";
	}
	public String evalString(VTDNav vn) throws UnsupportedException{
	  switch(opCode){
			case FuncName.LOCAL_NAME: 			
			case FuncName.NAMESPACE_URI: 	
			case FuncName.NAME: 		
			    throw new UnsupportedException("Some functions are not supported");
			case FuncName.STRING:
			    if (argCount()== 0)
			        try{
			            if (vn.atTerminal){
			                if (vn.getTokenType(vn.LN) == VTDNav.TOKEN_CDATA_VAL )
			                    return vn.toRawString(vn.LN);
			                return vn.toString(vn.LN);
			            }
			            return vn.toString(vn.getCurrentIndex());
			        }
			    	catch(NavException e){
			    	    return null; // this will almost never occur
			    	}
			    else if (argCount() == 1){
			        return argumentList.e.evalString(vn);
			    } else 
			        throw new IllegalArgumentException
					("String()'s argument count is invalid");
			case FuncName.SUBSTRING_BEFORE:		
			case FuncName.SUBSTRING_AFTER: 		
			case FuncName.SUBSTRING: 		
			case FuncName.TRANSLATE: 	
			case FuncName.NORMALIZE_SPACE:
			default: throw new UnsupportedException("Some functions are not supported");
	  }
	}	
	public double evalNumber(VTDNav vn) throws UnsupportedException{
	  switch(opCode){
			case FuncName.LAST:  throw new UnsupportedException
									("Some functions are not supported"); 			
			case FuncName.POSITION: return a++;
			case FuncName.COUNT: 	return count(vn);
			case FuncName.NUMBER:   if (argCount()!=1)
										throw new IllegalArgumentException
										("number()'s argument count is invalid");
									return argumentList.e.evalNumber(vn);
									
			case FuncName.SUM:	    return sum(vn);
			case FuncName.FLOOR: 	if (argCount()!=1 )
			    						throw new IllegalArgumentException("floor()'s argument count is invalid");
			    					return Math.floor(argumentList.e.evalNumber(vn));
			    					
			case FuncName.CEILING:	if (argCount()!=1 )
			    						throw new IllegalArgumentException("ceiling()'s argument count is invalid");
			    					return Math.ceil(argumentList.e.evalNumber(vn));
			    					
			case FuncName.STRING_LENGTH:
			    
			case FuncName.ROUND: 	if (argCount()!=1 )
			    						return Math.round(argumentList.e.evalNumber(vn));
			
			default: throw new UnsupportedException("Some functions are not supported");
	  }
	}

	public int evalNodeSet(VTDNav vn) throws XPathEvalException{
	  throw new XPathEvalException(" Function Expr can't eval to node set ");
	}
	
	public boolean evalBoolean(VTDNav vn){
	  	  switch(opCode){
			case FuncName.TRUE: if (argCount()!=0){
									throw new IllegalArgumentException("true() doesn't take any argument");
								}
								return true;			
			case FuncName.FALSE:if (argCount()!=0){
									throw new IllegalArgumentException("false() doesn't take any argument");
								}
								return false;	
			case FuncName.BOOLEAN: if (argCount()!=1){
										throw new IllegalArgumentException("boolean() doesn't take any argument");
								   }
									return argumentList.e.evalBoolean(vn);	
			case FuncName.NOT:	if (argCount()!=1){
										throw new IllegalArgumentException("not() doesn't take any argument");
			   					}
								return !argumentList.e.evalBoolean(vn);
			case FuncName.CONTAINS:	
			default: throw new UnsupportedException("Some functions are not supported");
		  }
	}
	
	public void reset(VTDNav vn){
	    a = 0;
		if (argumentList!=null)
			argumentList.reset(vn);
	}

	public String fname(){
		switch(opCode){
			case FuncName.LAST: 			return "last";
			case FuncName.POSITION: 		return "position";
			case FuncName.COUNT: 			return "count";
			case FuncName.LOCAL_NAME: 		return "local-name";
			case FuncName.NAMESPACE_URI: 		return "namespace-uri";
			case FuncName.NAME: 			return "name";
			case FuncName.STRING: 			return "string";
			case FuncName.CONCAT: 			return "concat";
			case FuncName.STARTS_WITH:		return "starts-with";
			case FuncName.CONTAINS: 		return "contains";
			case FuncName.SUBSTRING_BEFORE: 	return "substring_before";
			case FuncName.SUBSTRING_AFTER: 		return "substring_after";
			case FuncName.SUBSTRING: 		return "substring";
			case FuncName.STRING_LENGTH: 		return "string-length";
			case FuncName.NORMALIZE_SPACE: 		return "normalize-space";
			case FuncName.TRANSLATE:	 	return "translate";
			case FuncName.BOOLEAN: 			return "boolean";
			case FuncName.NOT: 			return "not";
			case FuncName.TRUE: 			return "true";
			case FuncName.FALSE: 			return "false";
			case FuncName.LANG: 			return "lang";
			case FuncName.NUMBER:			return "number";
			case FuncName.SUM: 			return "sum";
			case FuncName.FLOOR: 			return "floor";
			case FuncName.CEILING: 			return "ceiling";
			default:			return "round";
		}
	}
	public boolean  isNodeSet(){
		return false;
	}

	public boolean  isNumerical(){
		return isNumerical;
	}
	
	public boolean isString(){
	    return isString;
	}
	
	public boolean isBoolean(){
	    return isBoolean;
	}
	
	private int count(VTDNav vn){
	    int a = -1;
	    if (argCount()!=1 || argumentList.e.isNodeSet()==false)
			throw new IllegalArgumentException
				("Count()'s argument count is invalid");
		vn.push2();
		try{
			a = 0;
			while(argumentList.e.evalNodeSet(vn)!=-1){
				a ++;
			}
			argumentList.e.reset(vn);
			vn.pop2();
			
		}catch(Exception e){
			argumentList.e.reset(vn);
			vn.pop2();
		}
		return a;
	}
	
	private double sum(VTDNav vn){
	    
	    if (argCount() != 1 || argumentList.e.isNodeSet() == false)
	        throw new IllegalArgumentException("sum()'s argument count is invalid");
    	vn.push2();
    	try {
    	    a = 0;
    	    int i1;
    	    while ((a =argumentList.e.evalNodeSet(vn)) != -1) {
    	        int t = vn.getTokenType(a);
                if (t == VTDNav.TOKEN_STARTING_TAG){
                    i1 = vn.getText();
                    if (i1!=-1)
                        d += vn.parseDouble(i1);
                    if (d== Double.NaN)
                        break;
                }
                else if (t == VTDNav.TOKEN_ATTR_NAME
                        || t == VTDNav.TOKEN_ATTR_NS){
                    d += vn.parseDouble(a+1);
                    if (d== Double.NaN)
                        break;
                }
                else if (t == VTDNav.TOKEN_CHARACTER_DATA
                        || t == VTDNav.TOKEN_CDATA_VAL){
                    d += vn.parseDouble(a);
                    if (d== Double.NaN)
                        break;
                }
                //    fib1.append(i);
    	    }
    	    argumentList.e.reset(vn);
    	    vn.pop2();
    	    return a;
    	} catch (Exception e) {
    	    argumentList.e.reset(vn);
    	    vn.pop2();
    	    return Double.NaN;
    	}
	    
	}
	// to support computer context size 
	// needs to add 
	// public boolean needContextSize();
	// public boolean SetContextSize(int contextSize);
}
