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

public class UnaryExpr extends Expr {

	public int op;
	public Expr operand;
	public UnaryExpr (int o, Expr e){
		op = o;
		operand = e;
	}
	
	public String toString(){
		return "-"+operand;
	}

		
	public boolean evalBoolean(VTDNav vn){
		
		return operand.evalBoolean(vn);
	}

	public double evalNumber(VTDNav vn){ return -1*operand.evalNumber(vn);}
		
	public int evalNodeSet(VTDNav vn) throws XPathEvalException{
		
		throw new XPathEvalException("UnaryExpr can't eval to a node set!");
	}
	
        public String evalString(VTDNav vn){
		double dval = operand.evalNumber(vn);
		if (dval == (int) dval){
			return ""+((int) dval);
		}
		return ""+dval;
	}

	public void reset(){
		operand.reset();
	}

	public boolean  isNodeSet(){
		return false;
	}

	public boolean  isNumerical(){
		return true;
	}
}
