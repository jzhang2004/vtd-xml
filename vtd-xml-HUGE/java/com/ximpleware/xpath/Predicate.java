/* 
 * Copyright (C) 2002-2007 XimpleWare, info@ximpleware.com
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
 * LocationPathExpr uses this class to represent a predicate
 *
 */
public class Predicate implements LocationPathNode{
	double d; // only supports a[1] style of location path for now
	public int count;
	public Expr expr;
	public Predicate(){
		nextP = (Predicate) null;
		count = 0;
		d = 0;
	}
	public boolean eval(VTDNav vn) {
		boolean b;		
		count++; // increment the position
		expr.setPosition(count);
		if (expr.isNumerical()){		    
			b = (expr.evalNumber(vn)== count);
		}
		else{ 
			b = expr.evalBoolean(vn);
		}
		return b;
	}
	
	public void setIndex(double index) throws XPathEvalException{
		if (index<=0)
			throw new XPathEvalException("Invalid index number");
		d = (double) index;
	}
	
	public void reset(VTDNav vn){
		count = 0;
		expr.reset(vn); // is this really needed?
	}
	public Predicate nextP;

	public String toString(){
		String s = "["+expr+"]";
		if (nextP==null){
			return s;
		} else {
			return s+nextP;
		}
	}
	
	// to support computer context size 
	// needs to add 
	
	public boolean requireContextSize(){
	    return expr.requireContextSize();
	}
	
	public void setContextSize(int size){
	    expr.setContextSize(size);
	}
}

