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
using XPathEvalException = com.ximpleware.XPathEvalException;
namespace com.ximpleware
{
	
	/// <summary> PathExpr implements the Path expression as defined
	/// in XPath spec
	/// 
	/// </summary>
	public class PathExpr:Expr
	{
        public override int adjust(int n)
        {
            int i = fe.adjust(n);
            lpe.adjust(n);

            if (ih != null && i == ih.e)
            { }
            else
                ih = new intHash(i);
            return i;
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
				return false;
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
		
		internal Expr fe;
		internal LocationPathExpr lpe;
		//boolean first_time;
		internal int evalState;
		//FastIntBuffer fib;
		internal intHash ih;
		
		//public int getPositon(){
		//	return fib.size_Renamed_Field;
		//}
		
		public PathExpr(Expr f, LocationPathExpr l)
		{
			fe = f;
			lpe = l;
			//first_time = true;
			evalState = 0;
			//fib = new FastIntBuffer(8);
			ih = new intHash();
		}
		
		public override bool evalBoolean(VTDNav vn)
		{
			bool a = false;
			vn.push2();
			// record teh stack size
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
			int a;
			while (true)
			{
				switch (evalState)
				{
					
					case 0:  //this state is teh initial state;
						a = fe.evalNodeSet(vn);
						if (a == - 1)
						{
							evalState = 4;
						}
						else
							evalState = 1;
						break;
					
					case 1:  // fe returns valid value, then iterate the locationPath
						vn.push2();
						a = lpe.evalNodeSet(vn);
						if (a == - 1)
						{
							lpe.reset(vn);
							evalState = 3;
						}
						else
						{
							evalState = 2;
							if (isUnique(a))
								return a;
						}
						break;
					
					case 2: 
						a = lpe.evalNodeSet(vn);
						if (a == - 1)
						{
							lpe.reset(vn);
							evalState = 3;
						}
						else
						{
							if (isUnique(a))
								return a;
							//return a;
						}
						break;
					
					case 3: 
						vn.pop2();
						a = fe.evalNodeSet(vn);
						if (a == - 1)
							evalState = 4;
						else
						{
							vn.push2();
							evalState = 2;
						}
						break;
					
					case 4: 
						return - 1;
					
					default: 
						throw new XPathEvalException("Invalid state evaluating PathExpr");
					
				}
			}
			//return -1;
		}
		
		
		public override System.String evalString(VTDNav vn)
		{
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
		// The improved version, use hashtable to check for uniqueness
		public bool isUnique(int i)
		{
			return ih.isUnique(i);
		}
		
		public override void  reset(VTDNav vn)
		{
			
			fe.reset(vn);
			lpe.reset(vn);
			//fib.size_Renamed_Field=0;
			ih.reset();
			evalState = 0;
		}
		
		
		public override System.String ToString()
		{
			
			return "(" + fe + ")/" + lpe;
		}
		
		// to support computer context size 
		// needs to add 
		public override bool requireContextSize()
		{
			return false;
		}
	}
}