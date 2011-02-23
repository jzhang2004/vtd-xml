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
using com.ximpleware;
namespace com.ximpleware.xpath
{
	/// <summary> This class represent an XPath that is a double
	/// 
	/// </summary>
	public class NumExpr:Expr
	{
		override public bool NodeSet
		{
			get
			{
				return false;
			}
			
		}
		override public bool Numerical
		{
			get
			{
				return true;
			}
			
		}
		override public bool String
		{
			get
			{
				return false;
			}
			
		}
		override public bool Boolean
		{
			get
			{
				return false;
			}
			
		}
		override public int ContextSize
		{
			set
			{
			}
			
		}
		override public int Position
		{
			set
			{
				
			}
			
		}
		public double dval;
		public NumExpr(double d)
		{
			dval = d;
		}
		public override System.String ToString()
		{
			//UPGRADE_WARNING: Data types in Visual C# might be different.  Verify the accuracy of narrowing conversions. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1042'"
			if (dval == (long) dval)
			{
				//UPGRADE_WARNING: Data types in Visual C# might be different.  Verify the accuracy of narrowing conversions. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1042'"
				return "" + (long) dval;
			}
			return "" + dval;
		}
		
		public double eval()
		{
			return dval;
		}
		
		public override bool evalBoolean(VTDNav vn)
		{
			if (dval == 0.0 || System.Double.IsNaN(dval))
				return false;
			return true;
		}
		
		public override double evalNumber(VTDNav vn)
		{
			return dval;
		}
		
		public override int evalNodeSet(VTDNav vn)
		{
			
			throw new XPathEvalException("NumExpr can't eval to a node set!");
		}
		
		public override System.String evalString(VTDNav vn)
		{
			//UPGRADE_WARNING: Data types in Visual C# might be different.  Verify the accuracy of narrowing conversions. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1042'"
			if (dval == (int) dval)
			{
				//UPGRADE_WARNING: Data types in Visual C# might be different.  Verify the accuracy of narrowing conversions. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1042'"
				return "" + ((int) dval);
			}
			return "" + dval;
		}
		
		public override void  reset(VTDNav vn)
		{
		}
		
		// to support computer context size 
		// needs to add 
		public override bool requireContextSize()
		{
			return false;
		}

        public override int adjust(int n) { return 0; }
	}
}