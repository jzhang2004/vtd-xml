/* 
* Copyright (C) 2002-2011 XimpleWare, info@ximpleware.com
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
using System;
using com.ximpleware.xpath;
namespace com.ximpleware
{
	/// <summary> XimpleWare's AutoPilot implementation encapsulating node iterator
	/// and XPath.
	/// 
	/// </summary>
	public class AutoPilot
	{
		protected internal System.String Name
		{
			get
			{
				return name;
			}
			
		}
		/// <summary> Setspecial is used by XPath evaluator to distinguish between
		/// node() and *
		/// node() corresponding to b= true;
		/// </summary>
		/// <param name="b">
		/// </param>
		protected internal bool Special
		{			
			set
			{
				special = value;
			}
			
		}
		/// <summary> Convert the expression to a string
		/// For debugging purpose
		/// </summary>
		/// <returns>
		/// </returns>
		public System.String getExprString()
		{
			return xpe.ToString();
		}
		private int depth;
		// the depth of the element at the starting point will determine when to stop iteration
		private int iter_type; // see selectElement
		private VTDNav vn; // the navigator object
		private int index; // for iterAttr
		private bool ft; // a helper variable for 
		private bool special; // This helps distinguish between
		// the case of node() and * for preceding axis
		// of xpath evaluation
		private System.String name; // Store element name after selectElement
        private System.String name2; //xmlns:+name
        private System.String localName; // Store local name after selectElemntNS
		private System.String URL; // Store URL name after selectElementNS
		private int size; // for iterateAttr
        private FastIntBuffer fib; 
		private Expr xpe; // for evalXPath
		
		private int[] contextCopy; //for preceding axis
		private int stackSize; // the stack size for xpath evaluation
		static private System.Collections.Hashtable nsHash;
        static private System.Collections.Hashtable symbolHash;
		//private parser p;
		// defines the type of "iteration"
		public const int UNDEFINED = 0;
		// set the mode corresponding to DOM's getElemetnbyName(string)
		public const int SIMPLE = 1;
		// set the mode corresponding to DOM's getElementbyNameNS(string)
		public const int SIMPLE_NS = 2;
		public const int DESCENDANT = 3;
		public const int DESCENDANT_NS = 4;
		public const int FOLLOWING = 5;
		public const int FOLLOWING_NS = 6;
		public const int PRECEDING = 7;
		public const int PRECEDING_NS = 8;
		public const int ATTR = 9;
		public const int ATTR_NS = 10;
        public const int NAME_SPACE = 11;
		/// <summary> AutoPilot constructor comment.</summary>
		/// <exception cref="IllegalArgumentException">If the VTDNav object is null 
		/// </exception>
		public AutoPilot(VTDNav v)
		{
			if (v == null)
				throw new System.ArgumentException(" instance of VTDNav can't be null ");
			name = null;
			vn = v;
			//depth = v.getCurrentDepth();
			iter_type = UNDEFINED; // not defined
			ft = true;
			size = 0;
			special = false;
			xpe = null;
            symbolHash = new System.Collections.Hashtable();
			//p = null;       
		}
		
		/// <summary> Use this constructor for delayed binding to VTDNav
		/// which allows the reuse of XPath expression 
		/// 
		/// </summary>
		public AutoPilot()
		{
			name = null;
			//vn = v;
			//depth = v.getCurrentDepth();
			iter_type = UNDEFINED; // not defined
			ft = true;
			size = 0;
			special = false;
			xpe = null;
            symbolHash = new System.Collections.Hashtable();
		}
		/// <summary>This function creates URL ns prefix 
		/// and is intended to be called prior to selectXPath
		/// </summary>
		/// <param name="String">prefix
		/// </param>
		/// <param name="String">URL
		/// </param>
		
		public void  declareXPathNameSpace(System.String prefix, System.String URL)
		{
			if (nsHash == null)
				nsHash = System.Collections.Hashtable.Synchronized(new System.Collections.Hashtable());
			nsHash[prefix] = URL;
			//System.out.println(ht); 
		}

        /// <summary>
        /// declare variable references
        /// </summary>
        /// <param name="varName"></param>
        /// <param name="varExpr"></param>
        public void declareVariableExpr(String varName, String varExpr)
        {
           
            try
            {
                com.ximpleware.xpath.parser p = new com.ximpleware.xpath.parser(new System.IO.StringReader(varExpr));
                p.nsHash = nsHash;
                p.symbolHash = symbolHash;
                xpe = (com.ximpleware.Expr)p.parse().value;
                symbolHash[varName] = xpe;
                ft = true;
            }
            catch (XPathParseException e)
            {
                //UPGRADE_TODO: The equivalent in .NET for method 'java.lang.Throwable.toString' may return a different value. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1043'"
                Console.WriteLine("Syntax error after  ==>" + varExpr.Substring(0, e.getOffset()));
                throw new XPathParseException(e.ToString());
            }
            catch (System.Exception e)
            {
                Console.WriteLine("error occurred");
                throw new XPathParseException(e.ToString());
            }
        }
		
		/// <summary> Bind is to replace rebind() and setVTDNav()
		/// It resets the internal state of AutoPilot
		/// so one can attach a VTDNav object to the autopilot
		/// </summary>
		/// <param name="vnv">*
		/// </param>
		public void  bind(VTDNav vnv)
		{
			name = null;
			if (vnv == null)
				throw new System.ArgumentException(" instance of VTDNav can't be null ");
			vn = vnv;
			//depth = v.getCurrentDepth();
			iter_type = UNDEFINED; // not defined
			ft = true;
			size = 0;
			special = false;
			//resetXPath();
		}
		/// <summary> Iterate over all the selected element nodes in document order.
		/// Null element name allowed, corresponding to node() in xpath
		/// Creation date: (12/4/03 5:25:42 PM)
		/// </summary>
		/// <returns> boolean
		/// </returns>
		/// <exception cref="com.ximpleware.NavException">See description in method toElement() in VTDNav class.
		/// </exception>
		public bool iterate()
		{
			switch (iter_type)
			{
				
				case SIMPLE: 
					//System.out.println("iterating ---> "+name+ " depth ---> "+depth);
					/*if (elementName == null)
					throw new PilotException(" Element name not set ");*/
					if (vn.atTerminal)
						return false;
					if (ft == false)
						return vn.iterate(depth, name, special);
					else
					{
						ft = false;
						if (special || vn.matchElement(name))
						{
							return true;
						}
						else
							return vn.iterate(depth, name, special);
					}
					//goto case SIMPLE_NS;
				
				
				case SIMPLE_NS: 
					if (vn.atTerminal)
						return false;
					if (ft == false)
						return vn.iterateNS(depth, URL, localName);
					else
					{
						ft = false;
						if (vn.matchElementNS(URL, localName))
						{
							return true;
						}
						else
							return vn.iterateNS(depth, URL, localName);
					}
					//goto case DESCENDANT;
				
				
				case DESCENDANT: 
					if (vn.atTerminal)
						return false;
					return vn.iterate(depth, name, special);
				
				
				case DESCENDANT_NS: 
					if (vn.atTerminal)
						return false;
					return vn.iterateNS(depth, URL, localName);
				
				
				case FOLLOWING: 
					if (vn.atTerminal)
						return false;
					if (ft == false)
						return vn.iterate_following(name, special);
					else
					{
						ft = false;
						// find the first next sibling of 
						while (true)
						{
							while (vn.toElement(VTDNav.NS))
							{
								if (special || vn.matchElement(name))
								{
									return true;
								}
								return vn.iterate_following(name, special);
							}
							if (vn.toElement(VTDNav.P) == false)
							{
								//return vn.iterate_following(name, special);
								return false;
							}
						}
					}
					//goto case FOLLOWING_NS;
				
				
				case FOLLOWING_NS: 
					if (vn.atTerminal)
						return false;
					if (ft == false)
						return vn.iterate_followingNS(URL, localName);
					else
					{
						ft = false;
						// find the first next sibling of 
						while (true)
						{
							while (vn.toElement(VTDNav.NS))
							{
								if (vn.matchElementNS(URL, localName))
								{
									return true;
								}
								return vn.iterate_followingNS(URL, localName);
							}
							if (vn.toElement(VTDNav.P) == false)
							{
								return false;
							}
						}
					}
					//goto case PRECEDING;
				
				
				case PRECEDING: 
					if (vn.atTerminal)
						return false;
					return vn.iterate_preceding(name, contextCopy, special);
				
				
				case PRECEDING_NS: 
					if (vn.atTerminal)
						return false;
					return vn.iterate_precedingNS(URL, localName, contextCopy);
				
				
				default: 
					throw new PilotException(" iteration action type undefined");
				
			}
		}
/// <summary>
/// 
/// </summary>
/// <param name="en"></param>
        protected internal void selectNameSpace(String en)
        {
            if (en == null)
                throw new ArgumentException("namespace name can't be null");
            iter_type = NAME_SPACE;
            ft = true;
            size = vn.getTokenCount();
            name = en;
            if (!en.Equals("*"))
                name2 = "xmlns:" + en;
            if (fib == null)
                fib = new FastIntBuffer(4);
            else
                fib.size_Renamed_Field=0;
        }
    /// <summary>
    /// 
    /// </summary>
    /// <returns></returns>
	protected internal int iterateNameSpace(){
		if (vn.ns == false)
			return -1;
		if (ft != false) {
			ft = false;
			index = vn.getCurrentIndex2() + 1;
		} else
			index += 2;

		while (index < size) {
			int type = vn.getTokenType(index);
			if (type == VTDNav.TOKEN_ATTR_NAME || type == VTDNav.TOKEN_ATTR_NS) {
				if (type == VTDNav.TOKEN_ATTR_NS){ 
				    if  (name.Equals("*")  
				    		|| vn.matchRawTokenString(index, name2)
				    ){
                        if (checkNsUniqueness(index))
                        {
                            vn.LN = index;
                            vn.atTerminal = true;
                            return index;
                        }
				    }
				} 
				index += 2;
			} else {
				vn.atTerminal = false;
				if (vn.toElement(VTDNav.P) == false) {
					return -1;
				} else {
					index = vn.getCurrentIndex2() + 1;
				}
			}
		}

		return -1;
	}

    protected internal bool checkNsUniqueness(int i){
		for (int j=0;j<fib.size_Renamed_Field;j++){
			if (vn.compareTokens(fib.intAt(j), vn, i)==0)
				return false;
		}
			
		fib.append(i);
		return true;
	}


    /// <summary> This method implements the attribute axis for XPath</summary>
    /// <returns> the integer of the selected VTD index for attribute name
    /// </returns>
    /// <throws>  PilotException </throws>
    public int iterateAttr()
    {

        switch (iter_type)
        {

            case ATTR:
                if (String.CompareOrdinal(name, "*") == 0)
                {
                    if (ft != false)
                    {
                        ft = false;
                        index = vn.getCurrentIndex2() + 1;
                    }
                    else
                        index += 2;
                    if (vn.ns == false)
                    {
                        while (index < size)
                        {
                            int type = vn.getTokenType(index);
                            if (type == VTDNav.TOKEN_ATTR_NAME || type == VTDNav.TOKEN_ATTR_NS)
                            {
                                //vn.LN = index;
                                return index;
                            }
                            else
                            {
                                return -1;
                            }
                        }
                        return -1;
                    }
                    else
                    {

                        while (index < size)
                        {
                            int type = vn.getTokenType(index);
                            if (type == VTDNav.TOKEN_ATTR_NAME || type == VTDNav.TOKEN_ATTR_NS)
                            {
                                if (type == VTDNav.TOKEN_ATTR_NAME)
                                {
                                    //vn.LN = index;
                                    return index;
                                }
                                else
                                    index += 2;
                            }
                            else
                            {
                                return -1;
                            }
                        }
                        return -1;
                    }
                }
                else
                {
                    if (ft == false)
                    {
                        return -1;
                    }
                    else
                    {
                        ft = false;
                        int i = vn.getAttrVal(name);
                        if (i != -1)
                        {
                            //vn.LN = i - 1;
                            return i - 1;
                        }
                        else
                            return -1;
                    }
                }
            //goto case ATTR_NS;

            case ATTR_NS:
                if (ft == false)
                {
                    return -1;
                }
                else
                {
                    ft = false;
                    int i = vn.getAttrValNS(URL, localName);
                    if (i != -1)
                    {
                        //vn.LN = i - 1;
                        return i - 1;
                    }
                    else
                        return -1;
                }
            //goto default;

            default:
                throw new PilotException("invalid iteration type");

        }
    }

		/// <summary> This method implements the attribute axis for XPath</summary>
		/// <returns> the integer of the selected VTD index for attribute name
		/// </returns>
		/// <throws>  PilotException </throws>
		protected internal int iterateAttr2()
		{
			
			switch (iter_type)
			{
				
				case ATTR: 
					if (String.CompareOrdinal(name, "*") == 0)
					{
						if (ft != false)
						{
							ft = false;
							index = vn.getCurrentIndex2() + 1;
						}
						else
							index += 2;
						if (vn.ns == false)
						{
							while (index < size)
							{
								int type = vn.getTokenType(index);
								if (type == VTDNav.TOKEN_ATTR_NAME || type == VTDNav.TOKEN_ATTR_NS)
								{
                                    vn.LN = index;
									return index;
								}
								else
								{
									return - 1;
								}
							}
							return - 1;
						}
						else
						{
							
							while (index < size)
                            {
                                int type = vn.getTokenType(index);
								if (type == VTDNav.TOKEN_ATTR_NAME || type == VTDNav.TOKEN_ATTR_NS)
								{
									if (type == VTDNav.TOKEN_ATTR_NAME)
									{
                                        vn.LN = index;
										return index;
									}
									else
										index += 2;
								}
								else
								{
									return - 1;
								}
							}
							return - 1;
						}
					}
					else
					{
						if (ft == false)
						{
							return - 1;
						}
						else
						{
							ft = false;
							int i = vn.getAttrVal(name);
                            if (i != -1)
                            {
                                vn.LN = i - 1;
                                return i - 1;
                            }
                            else
                                return -1;
						}
					}
					//goto case ATTR_NS;
				
				case ATTR_NS: 
					if (ft == false)
					{
						return - 1;
					}
					else
					{
						ft = false;
						int i = vn.getAttrValNS(URL, localName);
                        if (i != -1)
                        {
                            vn.LN = i - 1;
                            return i - 1;
                        }
                        else
                            return -1;
					}
					//goto default;
				
				default: 
					throw new PilotException("invalid iteration type");
				
			}
		}
		/// <summary> Select the element name before iterating.
		/// "*" matches every element
		/// Creation date: (12/4/03 5:51:31 PM)
		/// </summary>
		/// <param name="en">java.lang.String
		/// </param>
		public void  selectElement(System.String en)
		{
			if (en == null)
				throw new System.ArgumentException("element name can't be null");
			iter_type = SIMPLE;
			depth = vn.getCurrentDepth();
			//startIndex = vn.getCurrentIndex();
			name = en;
			ft = true;
		}
		/// <summary> Select the element name (name space version) before iterating. URL, if set to *,
		/// matches every namespace URL, if set to null, indicates the namespace is
		/// undefined. localname, if set to *, matches any localname Creation date:
		/// (12/4/03 6:05:19 PM)
		/// 
		/// </summary>
		/// <param name="URL">java.lang.String
		/// </param>
		/// <param name="ln">java.lang.String
		/// </param>
		public void  selectElementNS(System.String ns_URL, System.String ln)
		{
			if (ln == null)
				throw new System.ArgumentException("local name can't be null");
			iter_type = SIMPLE_NS;
			depth = vn.getCurrentDepth();
			//startIndex = vn.getCurrentIndex();
			localName = ln;
			URL = ns_URL;
			ft = true;
		}
		
		/// <summary> Select all descendent elements along the descendent axis, without ns awareness</summary>
		/// <param name="en">
		/// </param>
		protected internal void  selectElement_D(System.String en)
		{
			if (en == null)
				throw new System.ArgumentException("element name can't be null");
			iter_type = DESCENDANT;
			depth = vn.getCurrentDepth();
			//startIndex = vn.getCurrentIndex();
			name = en;
			ft = true;
		}
		
		/// <summary> Select all descendent elements along the Descendent axis, withns awareness</summary>
		/// <param name="ns_URL">
		/// </param>
		/// <param name="ln">
		/// </param>
		protected internal void  selectElementNS_D(System.String ns_URL, System.String ln)
		{
			if (ln == null)
				throw new System.ArgumentException("local name can't be null");
			iter_type = DESCENDANT_NS;
			depth = vn.getCurrentDepth();
			//startIndex = vn.getCurrentIndex();
			localName = ln;
			URL = ns_URL;
			ft = true;
		}
		
		/// <summary> Select all elements along the following axis, without ns,
		/// null selects every elements and documents
		/// </summary>
		/// <param name="en">
		/// </param>
		protected internal void  selectElement_F(System.String en)
		{
			if (en == null)
				throw new System.ArgumentException("element name can't be null");
			iter_type = FOLLOWING;
			ft = true;
			name = en;
		}
		
		/// <summary> Select all elements along the preceding axis as defined in XPath
		/// The namespace-aware version
		/// </summary>
		/// <param name="en">
		/// </param>
		protected internal void  selectElementNS_F(System.String ns_URL, System.String ln)
		{
			if (ln == null)
				throw new System.ArgumentException("local name can't be null");
			iter_type = FOLLOWING_NS;
			ft = true;
			localName = ln;
			URL = ns_URL;
		}
		
		/// <summary> Select all elements along the preceding axis as defined in XPath</summary>
		/// <param name="en">
		/// </param>
		protected internal void  selectElement_P(System.String en)
		{
			if (en == null)
				throw new System.ArgumentException("element name can't be null");
			depth = vn.getCurrentDepth();
			iter_type = PRECEDING;
			ft = true;
			name = en;
			contextCopy = new int[vn.context.Length];
			vn.context.CopyTo(contextCopy, 0);
			for (int i = vn.context[0] + 1; i < vn.context.Length; i++)
			{
				contextCopy[i] = - 1;
			}
			contextCopy[0] = vn.rootIndex;
		}
		
		/// <summary> Select all elements along the preceding axis as defined in XPath
		/// This is the namespace aware version
		/// </summary>
		/// <param name="ns_URL">
		/// </param>
		/// <param name="ln">
		/// </param>
		protected internal void  selectElementNS_P(System.String ns_URL, System.String ln)
		{
			if (ln == null)
				throw new System.ArgumentException("local name can't be null");
			depth = vn.getCurrentDepth();
			iter_type = PRECEDING_NS;
			ft = true;
			localName = ln;
			URL = ns_URL;
			contextCopy = new int[vn.context.Length];
			vn.context.CopyTo(contextCopy, 0);
			for (int i = vn.context[0] + 1; i < vn.context.Length; i++)
			{
				vn.context[i] = - 1;
			}
			contextCopy[0] = vn.rootIndex;
		}
		
		/// <summary> Select an attribute name for iteration, * choose all attributes of an element</summary>
		/// <param name="en">
		/// </param>
		public void  selectAttr(System.String en)
		{
			if (en == null)
				throw new System.ArgumentException("attribute name can't be null");
			iter_type = ATTR;
			ft = true;
			size = vn.getTokenCount();
			name = en;
		}
		
		/// <summary> Select an attribute name, both local part and namespace URL part</summary>
		/// <param name="ns_URL">
		/// </param>
		/// <param name="ln">
		/// </param>
		public void  selectAttrNS(System.String ns_URL, System.String ln)
		{
			if (ln == null)
				throw new System.ArgumentException("local name of an attribute can't be null");
			iter_type = ATTR_NS;
			ft = true;
			localName = ln;
			URL = ns_URL;
		}
		
		/// <summary> This method selects the string representing XPath expression
		/// Usually evalXPath is called afterwards
		/// </summary>
		/// <param name="s">
		/// </param>
		/// <throws>  XPathParseException </throws>
		public void  selectXPath(System.String s)
		{			
			try
			{
				com.ximpleware.xpath.parser p = new com.ximpleware.xpath.parser(new System.IO.StringReader(s));
				p.nsHash = nsHash;
                p.symbolHash = symbolHash;
				xpe = (com.ximpleware.Expr) p.parse().value;
                ft = true;
			}
			catch (XPathParseException e)
			{
				//UPGRADE_TODO: The equivalent in .NET for method 'java.lang.Throwable.toString' may return a different value. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1043'"
                Console.WriteLine("Syntax error after  ==>" + s.Substring(0, e.getOffset()));
                throw new XPathParseException(e.ToString());
			}
            catch (System.Exception e){
                throw new XPathParseException(e.ToString());
            }
		}
        /// <summary>
        /// Remove all namespaces bindings
        /// </summary>
        public void clearVariableExprs()
        {
            symbolHash.Clear();
        }

        /// <summary>
        /// Remove all namespace bindings
        /// </summary>
        public void clearXPathNameSpaces()
        {
            nsHash.Clear();
        }
		
		/// <summary> 
        /// Reset the XPath so the XPath Expression can 
		/// be reused and revaluated in anther context position
		/// 
		/// </summary>
		
		public void  resetXPath()
		{
			if (xpe != null && vn != null)
			{
				xpe.reset(vn);
				ft = true;
				vn.contextStack2.size = stackSize;
			}
		}

 
        /// <summary>
        ///  evalXPathToNumber() evaluates the xpath expression to a double
        /// </summary>
        /// <returns>return the result as a double</returns>
        public double evalXPathToNumber()
        {
            return xpe.evalNumber(vn);
        }
        
       
       /// <summary>
       /// evalXPathToString() evaluates the xpath expression to a String
       /// </summary>
       /// <returns>return the result as a string</returns>
        
        public String evalXPathToString()
        {
            return xpe.evalString(vn);
        }
       
        /// <summary>
        ///  evalXPathToBoolean() evaluates the xpath expression to a boolean
        /// </summary>
        /// <returns> return the result as a boolean</returns>
        public bool evalXPathToBoolean()
        {
            return xpe.evalBoolean(vn);
        }
		/// <summary> This method returns the next node in the nodeset
		/// it returns -1 if there is no more node
		/// Afer finishing evaluating, don't forget to <em> reset the xpath </em>
		/// </summary>
		/// <returns> int corresponding to the VTD index
		/// </returns>
		public int evalXPath()
		{
			if (xpe != null)
			{
				if (ft == true)
				{
					if (vn != null)
					{
						stackSize = vn.contextStack2.size;
					}
					ft = false;
                    xpe.adjust(vn.getTokenCount());
				}
				return xpe.evalNodeSet(vn);
			}
			throw new PilotException(" Null XPath expression ");
		}
	}
}