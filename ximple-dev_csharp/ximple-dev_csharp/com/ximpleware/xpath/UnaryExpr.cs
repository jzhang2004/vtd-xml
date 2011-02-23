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
	/// <summary> Parser use this class to represent Unary Expr
	/// 
	/// </summary>
	public class UnaryExpr:Expr
	{
        public override int adjust(int n) { return 0; }
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
				operand.ContextSize = value;
			}
			
		}
		override public int Position
		{
			set
			{
				operand.Position = value;
			}
			
		}
		
		public int op;
		public Expr operand;
		public UnaryExpr(int o, Expr e)
		{
			op = o;
			operand = e;
		}
		
		public override System.String ToString()
		{
			return "-" + operand;
		}
		
		
		public override bool evalBoolean(VTDNav vn)
		{
			
			return operand.evalBoolean(vn);
		}
		
		public override double evalNumber(VTDNav vn)
		{
			return (- 1) * operand.evalNumber(vn);
		}
		
		public override int evalNodeSet(VTDNav vn)
		{
			
			throw new XPathEvalException("UnaryExpr can't eval to a node set!");
		}
		
		public override System.String evalString(VTDNav vn)
		{
			double dval = operand.evalNumber(vn);
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
			operand.reset(vn);
		}
		// to support computer context size 
		// needs to add 
		public override bool requireContextSize()
		{
			return operand.requireContextSize();
		}
	}
}