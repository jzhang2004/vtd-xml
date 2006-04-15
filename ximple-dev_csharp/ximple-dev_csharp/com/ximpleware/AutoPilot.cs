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
using System;
//UPGRADE_TODO: The 'com.ximpleware.xpath' package was found, but not necessarily converted. References to it may cause compile errors. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1280'"

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
		private System.String localName; // Store local name after selectElemntNS
		private System.String URL; // Store URL name after selectElementNS
		private int size; // for iterateAttr
		
		
		private int[] contextCopy; //for preceding axis
		private int stackSize; // the stack size for xpath evaluation
		//private parser p;
		// defines the type of "iteration"
		public const int UNDEFINED = 0;
		// set the mode corresponding to DOM's getElemetnbyName(string)
		public const int SIMPLE = 1;
		// set the mode corresponding to DOM's getElementbyNameNS(string)
		public const int SIMPLE_NS = 2;
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
		}
		
		/// <summary> Bind is to replace rebind() and setVTDNav()
		/// It resets the internal state of AutoPilot
		/// so one can attach a VTDNav object to the autopilot
		/// </summary>
		/// <param name="vnv">*
		/// </param>
		public  void  bind(VTDNav vnv)
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
		}
		/// <summary> Iterate over all the selected element nodes in document order.
		/// Null element name allowed, corresponding to node() in xpath
		/// Creation date: (12/4/03 5:25:42 PM)
		/// </summary>
		/// <returns> boolean
		/// </returns>
		/// <exception cref="com.ximpleware.NavException">See description in method toElement() in VTDNav class.
		/// </exception>
		public  bool iterate()
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
					//goto default;
				
				
				default: 
					throw new PilotException(" iteration action type undefined");
				
			}
		}
		/// <summary> Select the element name before iterating.
		/// "*" matches every element
		/// Creation date: (12/4/03 5:51:31 PM)
		/// </summary>
		/// <param name="en">java.lang.String
		/// </param>
		public  void  selectElement(System.String en)
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
		public  void  selectElementNS(System.String ns_URL, System.String ln)
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
	}
}