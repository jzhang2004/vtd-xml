/* 
* Copyright (C) 2002-2013 XimpleWare, info@ximpleware.com
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
/*VTD-XML is protected by US patent 7133857, 7260652, an 7761459*/
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
		/// <summary>
		/// 
		/// </summary>
		/// <param name="f"></param>
		/// <param name="l"></param>
		public PathExpr(Expr f, LocationPathExpr l)
		{
			fe = f;
			lpe = l;
			//first_time = true;
			evalState = 0;
			//fib = new FastIntBuffer(8);
			ih = new intHash();
		}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="vn"></param>
		/// <returns></returns>
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
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="vn"></param>
		/// <returns></returns>
		public override double evalNumber(VTDNav vn)
		{
            double d = Double.NaN;
            int a = -1;
            vn.push2();
            int size = vn.contextStack2.size;
            try
            {
                a = evalNodeSet(vn);
                if (a != -1)
                {
                    int t = vn.getTokenType(a);
                    if (t == VTDNav.TOKEN_ATTR_NAME)
                    {
                        d = vn.parseDouble(a + 1);
                    }
                    else if (t == VTDNav.TOKEN_STARTING_TAG || t == VTDNav.TOKEN_DOCUMENT)
                    {
                        String s = vn.getXPathStringVal();
                        d = Double.Parse(s);
                    }
                    else if (t == VTDNav.TOKEN_PI_NAME)
                    {
                        if (a + 1 < vn.vtdSize || vn.getTokenType(a + 1) == VTDNav.TOKEN_PI_VAL)
                            //s = vn.toString(a+1); 	
                            d = vn.parseDouble(a + 1);
                        else
                            d = Double.NaN;
                    }
                    else
                        d = vn.parseDouble(a);
                }
            }
            catch (Exception e)
            {

            }
            vn.contextStack2.size = size;
            reset(vn);
            vn.pop2();
            //return s;
            return d;
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
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="vn"></param>
		/// <returns></returns>
		public override System.String evalString(VTDNav vn)
		{
            String s = "";
            int a = -1;
            vn.push2();
            int size = vn.contextStack2.size;
            try
            {
                a = evalNodeSet(vn);
                if (a != -1)
                {
                    int t = vn.getTokenType(a);
                    switch (t)
                    {
                        case VTDNav.TOKEN_STARTING_TAG:
                        case VTDNav.TOKEN_DOCUMENT:
                            s = vn.getXPathStringVal();
                            break;
                        case VTDNav.TOKEN_ATTR_NAME:
                            s = vn.toString(a + 1);
                            break;
                        case VTDNav.TOKEN_PI_NAME:
                            //if (a + 1 < vn.vtdSize
                            //		|| vn.getTokenType(a + 1) == VTDNav.TOKEN_PI_VAL)
                            s = vn.toString(a + 1);
                            break;
                        default:
                            s = vn.toString(a);
                            break;
                    }
                }
            }
            catch (Exception e)
            {

            }
            vn.contextStack2.size = size;
            reset(vn);
            vn.pop2();
            return s;
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

        public override bool isFinal(){
		    return fe.isFinal();
	    }

        
        /// <summary>
        /// 
        /// </summary>
        public override void markCacheable()
        {
            fe.markCacheable();
            lpe.markCacheable();
        }
        /// <summary>
        /// 
        /// </summary>
        public override void markCacheable2()
        {
            fe.markCacheable2();
            lpe.markCacheable2();
        }
        /// <summary>
        /// 
        /// </summary>
        public override void clearCache()
        {
            fe.clearCache();
            lpe.clearCache();
        }
	}
}