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

	protected int op;
	boolean isNumerical;
	
	protected Expr left;
	protected Expr right;
	public BinaryExpr ( Expr l, int o, Expr r) {
		op = o;
		left = l;
		right = r;
		switch(op){
		 	case ADD:
			case SUB:
			case MULT:
			case DIV:
			case MOD: isNumerical = true; break;
			default: isNumerical = false;
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
			case GT: os = " > "; break;
			default: os = " | "; 
		}
		
		return "("+ left + os + right+")";
	}

	
	public boolean evalBoolean(VTDNav vn){
		switch(op){
			case OR: return left.evalBoolean(vn) || right.evalBoolean(vn);
			case AND:return left.evalBoolean(vn) && right.evalBoolean(vn);
			case EQ: {
					  if (left.isNodeSet() && right.isNodeSet()){
					  	
					  }
					  return left.evalNumber(vn) == right.evalNumber(vn);
					 }
			case NE: return left.evalNumber(vn) != right.evalNumber(vn);
			case LE: return left.evalNumber(vn) <= right.evalNumber(vn);
			case GE: return left.evalNumber(vn) >= right.evalNumber(vn);
			case LT: return left.evalNumber(vn) < right.evalNumber(vn);
			case GT: return left.evalNumber(vn) > right.evalNumber(vn);
			default: double dval = evalNumber(vn);
				 if (dval ==-0.0 || dval ==+0.0 || Double.isNaN(dval))
					 return true;
				 return false;
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
		throw new XPathEvalException("LiteralExpr can't eval to a node set!");
	}
	
        public String evalString(VTDNav vn){
		return "this";
	}

	public void reset(VTDNav vn){left.reset(vn); right.reset(vn);};

	public boolean  isNodeSet(){
		return false;
	}

	public boolean  isNumerical(){
		return isNumerical;
	}
}
