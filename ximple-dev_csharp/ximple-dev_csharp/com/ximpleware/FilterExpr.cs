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
using Expr = com.ximpleware.Expr;
using Predicate = com.ximpleware.xpath.Predicate;
using XPathEvalException = com.ximpleware.XPathEvalException;
namespace com.ximpleware
{
	
	/// <summary> 
	/// TODO To change the template for this generated type comment go to
	/// Window - Preferences - Java - Code Style - Code Templates
	/// </summary>
	public class FilterExpr:Expr
	{
        public override int adjust(int n) {
            return e.adjust(n);
        }
		override public bool Numerical
		{
			get
			{
				
				return false;
			}
			
		}
		override public bool NodeSet
		{
			get
			{
				return true;
			}
			
		}
		override public bool String
		{
			/*public boolean isUnique(int i){
			int size = fib.size_Renamed_Field;
			for (int j=0; j<size;j++){
			if (i == fib.intAt(j))
			return false;
			}
			fib.append(i);
			return true;
			}*/
			
			
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
		
		
		public Expr e;
		public Predicate p;
		//FastIntBuffer fib;
		//int stackSize;
		internal bool first_time;
		//public int position;
		
		public FilterExpr(Expr l, Predicate pr)
		{
			e = l;
			p = pr;
			//stackSize = 0;
			//position = 1;
			//fib = new FastIntBuffer(8);
			first_time = true;
		}
		/*public int getPositon(){
		return fib.size_Renamed_Field;
		}*/
		public override bool evalBoolean(VTDNav vn)
		{
			bool a = false;
			vn.push2();
			//record stack size
			int size = vn.contextStack2.size;
			try
			{
				a = (evalNodeSet(vn) != - 1);
			}
			catch (System.Exception e)
			{
			}
			//rewind stack
			vn.contextStack2.size = size;
			reset(vn);
			vn.pop2();
			return a;
		}
		
		
		public override double evalNumber(VTDNav vn)
		{
            int a = getStringIndex(vn);
            try
            {
                if (a != -1) return vn.parseDouble(a);
            }
            catch (NavException e)
            {
            }
            return Double.NaN;
		}
		
		public override int evalNodeSet(VTDNav vn)
		{
			// if tne predicate require context size
			// needs to precompute the context size
			// vn.push2();
			// computerContext();
			// set contxt();
			// vn.pop2()
			// if the context size is zero
			// get immediately set teh state to end
			// or backward
			if (first_time && p.requireContextSize())
			{
				first_time = false;
				int i = 0;
				//vn.push2();
                e.adjust(vn.getTokenCount());
				while (e.evalNodeSet(vn) != - 1)
					i++;
				//vn.pop2();
				p.ContextSize = i;
				reset2(vn);
			}
			int a = e.evalNodeSet(vn);
			while (a != - 1)
			{
				if (p.eval(vn) == true)
				{
					//p.reset();
					return a;
				}
				else
				{
					//p.reset();
					a = e.evalNodeSet(vn);
				}
			}
			return - 1;
		}
		
		public override System.String evalString(VTDNav vn)
		{
            //if (e.isString())
            //     return e.evalString(vn);
            int a = getStringIndex(vn);
            try
            {
                if (a != -1)
                    return vn.toString(a);
            }
            catch (NavException e)
            {
            }
            return "";
		}
		
		public override void  reset(VTDNav vn)
		{
			reset2(vn);
			//vn.contextStack2.size = stackSize; 
			//position = 1;
			first_time = true;
		}
		
		public void  reset2(VTDNav vn)
		{
			e.reset(vn);
			p.reset(vn);
			//fib.size_Renamed_Field=0;
		}
		
		
		public override System.String ToString()
		{
			
			return "(" + e + ") " + p;
		}
		// to support computer context size 
		// needs to add 
		public override bool requireContextSize()
		{
			return false;
		}
	}
}