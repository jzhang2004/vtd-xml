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
package com.ximpleware.xpath;
import com.ximpleware.*;

public class FuncExpr extends Expr{

	public Alist argumentList;
	public int opCode;
	boolean isNumerical;
	public FuncExpr(int oc , Alist list){
	  opCode = oc;
	  argumentList = list;
	  switch(opCode){
			case FuncName.LAST: 			isNumerical = true;break;
			case FuncName.POSITION: 		isNumerical = true;break;
			case FuncName.COUNT: 			isNumerical = true;break;
			case FuncName.LOCAL_NAME: 		isNumerical = false;break;
			case FuncName.NAMESPACE_URI: 		isNumerical = false;break;
			case FuncName.NAME: 			isNumerical = false;break;
			case FuncName.STRING: 			isNumerical = false;break;
			case FuncName.CONCAT: 			isNumerical = false;break;
			case FuncName.STARTS_WITH:		isNumerical = false;break;
			case FuncName.CONTAINS: 		isNumerical = false;break;
			case FuncName.SUBSTRING_BEFORE: 	isNumerical = false;break;
			case FuncName.SUBSTRING_AFTER: 		isNumerical = false;break;
			case FuncName.SUBSTRING: 		isNumerical = false;break;
			case FuncName.STRING_LENGTH: 		isNumerical = true;break;
			case FuncName.NORMALIZE_SPACE: 		isNumerical = false;break;
			case FuncName.TRANSLATE:	 	isNumerical = false;break;
			case FuncName.BOOLEAN: 			isNumerical = false;break;
			case FuncName.NOT: 			isNumerical = false;break;
			case FuncName.TRUE: 			isNumerical = false;break;
			case FuncName.FALSE: 			isNumerical = false;break;
			case FuncName.LANG: 			isNumerical = false;break;
			case FuncName.NUMBER:			isNumerical = true;break;
			case FuncName.SUM: 			isNumerical = true;break;
			case FuncName.FLOOR: 			isNumerical = true;break;
			case FuncName.CEILING: 			isNumerical = true;break;
			default:			isNumerical = true;;
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
			case FuncName.LAST: 			
			case FuncName.POSITION: 	
			case FuncName.COUNT: 			
			case FuncName.NUMBER:		
			case FuncName.SUM: 		
			case FuncName.FLOOR: 	
			case FuncName.CEILING:
			case FuncName.STRING_LENGTH:
			case FuncName.ROUND: 
			default: throw new UnsupportedException("Some functions are not supported");
	  }
	}

	public int evalNodeSet(VTDNav vn) throws XPathEvalException{
	  throw new XPathEvalException(" Function Expr can't eval to node set ");
	}
	
	public boolean evalBoolean(VTDNav vn){
	  	  switch(opCode){
			case FuncName.TRUE: return true;			
			case FuncName.FALSE: return false;	
			case FuncName.BOOLEAN: 			
			case FuncName.CONTAINS:	
			default: throw new UnsupportedException("Some functions are not supported");
		  }
	}
	
	public void reset(){
		if (argumentList!=null)
			argumentList.reset();
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
}
