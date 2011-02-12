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
	/// <summary> LocationPathExpr uses this class to represent a predicate
	/// 
	/// </summary>
	public class Predicate : LocationPathNode
	{
		public double Index
		{
			set
			{
				if (value <= 0)
					throw new XPathEvalException("Invalid index number");
				d = (double) value;
			}
			
		}
		public int ContextSize
		{
			set
			{
				expr.ContextSize = value;
			}
			
		}
		internal double d; // only supports a[1] style of location path for now
		public int count;
		public Expr expr;
		public Predicate()
		{
			nextP = (Predicate) null;
			count = 0;
			d = 0;
		}
		public bool eval(VTDNav vn)
		{
			bool b;
			count++; // increment the position
			expr.Position = count;
			if (expr.Numerical)
			{
				b = (expr.evalNumber(vn) == count);
			}
			else
			{
				b = expr.evalBoolean(vn);
			}
			return b;
		}
		
		public void  reset(VTDNav vn)
		{
			count = 0;
			expr.reset(vn); // is this really needed?
		}
		public Predicate nextP;
		
		public override System.String ToString()
		{
			System.String s = "[" + expr + "]";
			if (nextP == null)
			{
				return s;
			}
			else
			{
				return s + nextP;
			}
		}
		
		// to support computer context size 
		// needs to add 
		
		public bool requireContextSize()
		{
			return expr.requireContextSize();
		}

        public void adjust(int n)
        {
            expr.adjust(n);
        }
	}
}