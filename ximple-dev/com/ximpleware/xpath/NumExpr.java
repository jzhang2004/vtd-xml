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
public class NumExpr extends Expr{
	public double dval;
	public NumExpr( double d){
		dval = d;
	}
	public String toString(){
		if (dval == (int) dval){
			return ""+(int) dval;
		}
		return  ""+dval;
	} 

	public double eval(){
		return dval;
	}
	public boolean  isNodeSet(){
		return false;
	}

	public boolean  isNumerical(){
		return true;
	}
		
	public boolean evalBoolean(VTDNav vn){
		if (dval == 0.0 || Double.isNaN(dval) )
			return false;
		return true;
	}

	public double evalNumber(VTDNav vn){ return dval;}
		
	public int evalNodeSet(VTDNav vn) throws XPathEvalException{
		
		throw new XPathEvalException("NumExpr can't eval to a node set!");
	}
	
        public String evalString(VTDNav vn){
		if (dval == (int) dval){
			return ""+((int) dval);
		}
		return ""+dval;
	}

	public void reset(){};
}
