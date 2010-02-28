/* 
* Copyright (C) 2002-2010 XimpleWare, info@ximpleware.com
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
	/// <summary> This class represents a literal string XPath expression
	/// 
	/// </summary>
	public class LiteralExpr:Expr
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
				return false;
			}
			
		}
		override public bool String
		{
			get
			{
				return true;
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
		public System.String s;
		public LiteralExpr(System.String st)
		{
			s = st;
		}
		public override System.String ToString()
		{
			bool b = true;
			for (int i = 0; i < s.Length; i++)
			{
				if (s[i] == '\'')
				{
					b = false;
					break;
				}
			}
			if (b == true)
				return "\"" + s + "\"";
			else
				return "'" + s + "'";
		}
		
		public override bool evalBoolean(VTDNav vn)
		{
			return s.Length != 0;
		}
		
		public override double evalNumber(VTDNav vn)
		{
			try
			{
				double dval = System.Double.Parse(s);
				return dval;
			}
			catch (System.FormatException e)
			{
				return System.Double.NaN;
			}
		}
		
		public override int evalNodeSet(VTDNav vn)
		{
			
			throw new XPathEvalException("LiteralExpr can't eval to a node set!");
		}
		
		public override System.String evalString(VTDNav vn)
		{
			return s;
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
	}
}