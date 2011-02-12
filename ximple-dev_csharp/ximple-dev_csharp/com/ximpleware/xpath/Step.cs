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

namespace com.ximpleware.xpath {
    using com.ximpleware;
    //using com.ximpleware.xpath;

	/// <summary> A step is a part of location path as defined in 
	/// Xpath spec
	/// 
	/// </summary>
	public class Step : LocationPathNode
	{
		public NodeTest NodeTest
		{
			get
			{
				return this.nt;
			}
			
			set
			{
				nt = value;
			}
			
		}
		public Step NextStep
		{
			get
			{
				return nextS;
			}
			
			set
			{
				nextS = value;
			}
			
		}
		public Step PrevStep
		{
			get
			{
				return prevS;
			}
			
			set
			{
				prevS = value;
			}
			
		}
		public Predicate Predicate
		{
			set
			{
				if (p == null)
				{
					p = pt = value;
				}
				else
				{
					pt.nextP = value;
					pt = pt.nextP;
				}
			}
			
		}
		public int AxisType
		{
			set
			{
				axis_type = value;
			}
			
		}
		public int axis_type;
		public NodeTest nt;
		public Predicate p, pt; // linked list
		public Step nextS; // points to next step
		public int position; // position
		public Step prevS; // points to the prev step
		public System.Object o; //AutoPilot goes here
		internal bool ft; // first time
		public Step()
		{
			nextS = prevS = (Step) null;
			p = pt = null;
			nt = null;
			ft = true;
			position = 1;
		}
		
		public void  reset(VTDNav vn)
		{
			ft = true;
			resetP(vn);
			position = 1;
		}
		
		public void  resetP(VTDNav vn)
		{
			Predicate temp = p;
			while (temp != null)
			{
				temp.reset(vn);
				temp = temp.nextP;
			}
		}
		
		public void  resetP(VTDNav vn, Predicate p1)
		{
			Predicate temp = p;
			while (temp != p1)
			{
				temp.reset(vn);
				temp = temp.nextP;
			}
		}

        public void adjust(int n)
        {
            Predicate temp = p;
            while (temp != null)
            {
                temp.adjust(n);
                temp = temp.nextP;
            }
        }
		
		public bool get_ft()
		{
			return ft;
		}
		
		public void  set_ft(bool b)
		{
			ft = b;
		}
		
		public bool eval(VTDNav vn)
		{
			/*boolean result = this.nt.eval(vn);
			if (result == false)
			return false;
			return evalPredicates(vn);*/
			return nt.eval(vn) && evalPredicates(vn);
		}
		
		public bool eval(VTDNav vn, Predicate p)
		{
			return nt.eval(vn) && evalPredicates(vn, p);
		}
		
		public bool evalPredicates(VTDNav vn)
		{
			Predicate temp = this.p;
			while (temp != null)
			{
				if (temp.eval(vn) == false)
					return false;
				temp = temp.nextP;
			}
			
			return true;
		}
		
		public bool evalPredicates(VTDNav vn, Predicate p)
		{
			Predicate temp = this.p;
			while (temp != p)
			{
				if (temp.eval(vn) == false)
					return false;
				temp = temp.nextP;
			}
			return true;
		}
		
		public override System.String ToString()
		{
			System.String s;
			if (p == null)
			{
				s = axisName(axis_type) + nt;
			}
			else
			{
				s = axisName(axis_type) + nt + " " + p;
			}
			
			if (nextS == null)
				return s;
			else
			{
				return s + "/" + nextS.ToString();
			}
		}
		
		public System.String axisName(int i)
		{
			switch (i)
			{
				
				case com.ximpleware.xpath.AxisType.CHILD:  return "child::";

                case com.ximpleware.xpath.AxisType.DESCENDANT: return "descendant::";

                case com.ximpleware.xpath.AxisType.PARENT: return "parent::";

                case com.ximpleware.xpath.AxisType.FOLLOWING_SIBLING: return "following-sibling::";

                case com.ximpleware.xpath.AxisType.PRECEDING_SIBLING: return "preceding-sibling::";

                case com.ximpleware.xpath.AxisType.FOLLOWING: return "following::";

                case com.ximpleware.xpath.AxisType.PRECEDING: return "preceding::";

                case com.ximpleware.xpath.AxisType.ATTRIBUTE: return "attribute::";

                case com.ximpleware.xpath.AxisType.NAMESPACE: return "namespace::";

                case com.ximpleware.xpath.AxisType.SELF: return "self::";

                case com.ximpleware.xpath.AxisType.DESCENDANT_OR_SELF: return "descendant-or-self::";

                case com.ximpleware.xpath.AxisType.ANCESTOR: return "ancestor::";
				
				default:  return "ancestor-or-self::";
				
			}
		}
	}
}