package com.ximpleware;

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
/**
 * XimpleWare's AutoPilot implementation.
 * 
 * 
 * Added various functions for following, preceding, attribute, descendent
 * axis (7/2005)
 * 
 * Added selectXPath
 * 
 */

import com.ximpleware.xpath.*;
import java.io.*;
public class AutoPilot {
    private int depth;
    // the depth of the element at the starting point will determine when to stop iteration
    private int iter_type; // see selectElement
    private VTDNav vn; // the navigator object
    private int index;
    private boolean ft; // a helper variable for 
    private boolean special;   // This helps distinguish between
    				   		   // the case of node() and * for preceding axis
    						   // of xpath evaluation
    private String name; // Store element name after selectElement
    private String localName; // Store local name after selectElemntNS
    private String URL; // Store URL name after selectElementNS
    private int size; // for iterateAttr
    
    private Expr xpe;
    
    private int[] contextCopy;
    
    
    // defines the type of "iteration"
    public final static int UNDEFINED = 0;
    // set the mode corresponding to DOM's getElemetnbyName(string)
    public final static int SIMPLE = 1;
    // set the mode corresponding to DOM's getElementbyNameNS(string)
    public final static int SIMPLE_NS = 2;
    public final static int DESCENDENT = 3;
    public final static int DESCENDENT_NS = 4;
    public final static int FOLLOWING = 5;
    public final static int FOLLOWING_NS=6;
    public final static int PRECEDING = 7;
    public final static int PRECEDING_NS=8;
    public final static int ATTR = 9;
    public final static int ATTR_NS = 10;
    
    
 public String getName(){
   	return name;
 }
/**
 * AutoPilot constructor comment.
 * @exception IllegalArgumentException If the VTDNav object is null 
 */
public AutoPilot(VTDNav v) {
    if (v == null)
        throw new IllegalArgumentException(" instance of VTDNav can't be null ");
    name = null;
    vn = v;
    //depth = v.getCurrentDepth();
    iter_type = UNDEFINED; // not defined
    ft = true;
    size = 0;
    special = false;
    xpe = null;
       
}

public void rebind(VTDNav v){
    name = null;
    vn = v;
    //depth = v.getCurrentDepth();
    iter_type = UNDEFINED; // not defined
    ft = true;
    size = 0;
    special = false;
    contextCopy = (int[])vn.context.clone();
}
/**
 * Iterate over all the selected element nodes in document order.
 * Null element name allowed, corresponding to node() in xpath
 * Creation date: (12/4/03 5:25:42 PM)
 * @return boolean
 * @exception com.ximpleware.NavException See description in method toElement() in VTDNav class.
 */
public boolean iterate() throws PilotException, NavException {
    switch (iter_type) {
        case SIMPLE :
        	//System.out.println("iterating ---> "+name+ " depth ---> "+depth);
            /*if (elementName == null)
                throw new PilotException(" Element name not set ");*/
        	
            if (ft == false)
                return vn.iterate(depth, name);
            else {
            	ft = false;
                if (special || 
                		vn.matchElement(name)) {                	
                    return true;
                } else
                    return vn.iterate(depth, name);
            }
            
        case SIMPLE_NS :
            if (localName == null) //|| URL == null)
                throw new PilotException(" URL or Localname not set properly");

            if (ft == false)
                return vn.iterateNS(depth, URL, localName);
            else {
            	ft = false;
                if (vn.matchElementNS(URL, localName)) {
                	return true;
                } else
                    return vn.iterateNS(depth, URL, localName);
            }
            
         case DESCENDENT:
         	if (name == null)
                throw new PilotException(" Element name not set ");
         	
         	return vn.iterate(depth, name);
         	
         case DESCENDENT_NS:
         	if (localName == null) //|| URL == null)
                throw new PilotException(" URL or Localname not set properly");
         	
         	return vn.iterateNS(depth, URL, localName);
         	
         case FOLLOWING:
            if (ft == false)
                return vn.iterate_following(name);
            else {
            	ft = false;
            	// find the first next sibling of 
            	while(true){
            		while (vn.toElement(VTDNav.NS)){
            			 if (vn.matchElement(name)) {                	
                            return true;
            			 }
            		}
                    if (vn.toElement(VTDNav.P)==false){
                    	return vn.iterate_following(name);
                    } 
            	}
            }
            
         case FOLLOWING_NS:
         	if (ft == false)
                return vn.iterate_followingNS(URL,localName);
            else {
            	ft = false;
            	// find the first next sibling of 
            	while(true){
            		while (vn.toElement(VTDNav.NS)){
            			 if (vn.matchElementNS(URL,localName)) {                	
                            return true;
            			 }
            		}
                    if (vn.toElement(VTDNav.P)==false){
                    	return vn.iterate_followingNS(URL,localName);
                    } 
            	}
            }
           
         case PRECEDING: 
         	return vn.iterate_preceding(name, contextCopy,special);

         case PRECEDING_NS:
         	return vn.iterate_precedingNS(URL, contextCopy,localName);
                    	
        default :
            throw new PilotException(" iteration action type undefined");
    }
}
/**
 * 
 * @return
 * @throws PilotException
 */
   public int iterateAttr() throws PilotException,NavException{
   	    switch(iter_type){
   	    	case ATTR:
   	    		if (name.compareTo("*")==0){
   	    			if (ft != false){
   	    				ft = false;
   	    				index = vn.getCurrentIndex()+1;
   	    			} else
   	    				index +=2;
   	    			if (vn.ns == false){
   	    				while(index<=size){
   	    					int type = vn.getTokenType(index);
   	    					if (type == VTDNav.TOKEN_ATTR_NAME
   	    						|| type == VTDNav.TOKEN_ATTR_NS){
   	    						return index;
   	    					}else{   	    				
   	    						return -1;
   	    					}
   	    				}
   	    				return -1;
   	    			}else {
   	    				int type = vn.getTokenType(index);
   	    				while(index<=size){
	    					if (type == VTDNav.TOKEN_ATTR_NAME
	    						|| type == VTDNav.TOKEN_ATTR_NS){
	    						if (type == VTDNav.TOKEN_ATTR_NAME){
	    							return index;
	    						}
	    						else 
	    							index += 2;	    						
	    					}else{   	    				
	    						return -1;
	    					}
   	    				}
   	    				return -1;
   	    			}
   	    		}else{
   	    			if (ft == false){
   	    				return -1;
   	    			} else {
   	    				ft = false;
   	    				int i = vn.getAttrVal(name);
   	    				if(i!=-1)
   	    					return i-1;
   	    				else 
   	    					return -1;
   	    			}   	    			
   	    		}
   	        case ATTR_NS:
	    			if (ft == false){
   	    				return -1;
   	    			} else {
   	    				ft = false;
   	    				int i = vn.getAttrValNS(URL,localName);
   	    				if(i!=-1)
   	    					return i-1;
   	    				else 
   	    					return -1;
   	    			} 
   	        default:
   	        	throw new PilotException("invalid iteration type");
   	    }
   	
   }
/**
 * Select the element name before iterating.
 * "*" matches every element
 * Creation date: (12/4/03 5:51:31 PM)
 * @param en java.lang.String
 */
	public void selectElement(String en) {
		iter_type = SIMPLE;
		depth = vn.getCurrentDepth();
		//startIndex = vn.getCurrentIndex();
		name = en;
		ft = true;
	}
/**
 * Select the element name (name space version) before iterating. URL, if set to *,
 * matches every namespace URL, if set to null, indicates the namespace is
 * undefined. localname, if set to *, matches any localname Creation date:
 * (12/4/03 6:05:19 PM)
 * 
 * @param URL
 *            java.lang.String
 * @param ln
 *            java.lang.String
 */
public void selectElementNS(String ns_URL, String ln) {
    iter_type = SIMPLE_NS;
    depth = vn.getCurrentDepth();
    //startIndex = vn.getCurrentIndex();
    localName = ln;
    URL = ns_URL;
    ft = true;
}

/**
 * Select all descendent elements along the descendent axis, without ns awareness
 * @param en
 */
public void selectElement_D(String en) {
	iter_type = DESCENDENT;
	depth = vn.getCurrentDepth();
	//startIndex = vn.getCurrentIndex();
	name = en;
	ft = true;
}

/**
 * Select all descendent elements along the Descendent axis, withns awareness
 * @param ns_URL
 * @param ln
 */
public void selectElementNS_D(String ns_URL, String ln){
    iter_type = DESCENDENT_NS;
    depth = vn.getCurrentDepth();
    //startIndex = vn.getCurrentIndex();
    localName = ln;
    URL = ns_URL;
    ft = true;
}

/**
 * Select all elements along the following axis, without ns,
 * null selects every elements and documents
 * @param en
 */
public void selectElement_F(String en) {
	iter_type = FOLLOWING;
	ft = true;
	name = en;
}

/**
 * Select all elements along the preceding axis as defined in XPath
 * The namespace-aware version
 * @param en
 */
public void selectElementNS_F(String ns_URL, String ln){
	iter_type = FOLLOWING_NS;
    ft = true;
    localName = ln;
    URL = ns_URL;
}

/**
 * Select all elements along the preceding axis as defined in XPath
 * @param en
 */
public void selectElement_P(String en) {
	depth = vn.getCurrentDepth();
	iter_type = PRECEDING;
    ft = true;	
    name = en;
    contextCopy = (int[])vn.context.clone();
    contextCopy[0]=vn.rootIndex;
}

/**
 * Select all elements along the preceding axis as defined in XPath
 * This is the namespace aware version
 * @param ns_URL
 * @param ln
 */
public void selectElementNS_P(String ns_URL, String ln){
	depth = vn.getCurrentDepth();
	iter_type = PRECEDING_NS;
    ft = true;
    localName = ln;
    URL = ns_URL;
    contextCopy = (int[])vn.context.clone();
    contextCopy[0]=vn.rootIndex;
}

/**
 * Select an attribute name for iteration, * choose all attributes of an element
 * @param en
 */
public void selectAttr(String en) {
	iter_type = ATTR;
    ft = true;
    size = vn.getTokenCount();
    name = en;
}

/**
 * Select an attribute name, both local part and namespace URL part
 * @param ns_URL
 * @param ln
 */
public void selectAttrNS(String ns_URL, String ln){
	iter_type = ATTR_NS;
    ft = true;
    localName = ln;
    URL = ns_URL;
}

/**
 * 
 * @param s
 * @throws XPathParseException
 */
public void selectXPath(String s) throws XPathParseException{
	
	try{
		parser p = new parser(new StringReader(s));
		xpe = (com.ximpleware.xpath.Expr) p.parse().value;
	}
	catch(Exception e){
		throw new XPathParseException(e.toString());
	}
}

/**
 * Reset teh XPath so it 
 *
 */
public void resetXPath(){
	if (xpe!=null){
		xpe.reset();
		vn.clearStack2();
	}
}

/**
 * 
 * @return
 */
public int evalXPath() throws XPathEvalException, NavException{
	if (xpe!=null){
			return xpe.evalNodeSet(vn);
	}
	throw new PilotException(" Null XPath expression "); 
}
/**
 * Setspecial is used by XPath evaluator to distinguish between
 * node() and *
 * @param b
 */

public void setSpecial(boolean b ){
	special = b;
}

public String getExprString(){
	return xpe.toString();
}
}
