/* 
 * Copyright (C) 2002-2009 XimpleWare, info@ximpleware.com
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
import com.ximpleware.*;
/**
 * 
 * This is the abstract class on which all XPath expressions 
 * are based
 */

abstract public class Expr {

	abstract public boolean evalBoolean(VTDNav vn);

	abstract public double evalNumber(VTDNav vn);
		
	abstract public int evalNodeSet(VTDNav vn) throws XPathEvalException, NavException;
	
	abstract public String evalString(VTDNav vn);

	abstract public void reset(VTDNav vn);
	abstract public String toString();

	abstract public boolean isNumerical();
	abstract public boolean isNodeSet();
	abstract public boolean isString();
	abstract public boolean isBoolean();
	
	abstract public boolean requireContextSize();
	abstract public void setContextSize(int size);
	
	abstract public void setPosition(int pos);
	abstract public int adjust(int n);
	// to support computer context size 
	// needs to add 
	//abstract public boolean needContextSize();
	//abstract public boolean SetContextSize(int contextSize);
    protected int getStringIndex(VTDNav vn){
    	int a = -1;
        vn.push2();
        int size = vn.contextStack2.size;
        try {
            a = evalNodeSet(vn);
            if (a != -1) {
            	int t = vn.getTokenType(a);
                if (t == VTDNav.TOKEN_ATTR_NAME) {
                    a++;
                } else if (vn.getTokenType(a) == VTDNav.TOKEN_STARTING_TAG) {
                    a = vn.getText();
                }else if (t == VTDNav.TOKEN_PI_NAME) {
                    if (a+1<vn.vtdSize && vn.getTokenType(a+1)== VTDNav.TOKEN_PI_VAL)
                    	a=a+1;
                    else
                    	a = -1;                    
                }
            }
        } catch (Exception e) {

        }
        vn.contextStack2.size = size;
        reset(vn);
        vn.pop2();
        return a;
    }
}
