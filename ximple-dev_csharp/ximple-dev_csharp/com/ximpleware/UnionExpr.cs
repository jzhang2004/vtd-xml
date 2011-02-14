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
//UPGRADE_TODO: The 'com.ximpleware.xpath' package was found, but not necessarily converted. References to it may cause compile errors. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1280'"
using com.ximpleware.xpath;
namespace com.ximpleware
{
	
	public class UnionExpr:Expr
	{
		internal intHash ih;
		
		internal Expr e;
		
		internal UnionExpr next;
		
		internal UnionExpr current;
		
		internal int state;

        public override int adjust(int n)
        {
            int i = e.adjust(n);
            if (ih != null && i == ih.e)
            { }
            else
                ih = new intHash(i);
            UnionExpr tmp = this.next;
            while (tmp != null)
            {
                tmp.e.adjust(n);
                tmp = tmp.next;
            }
            return i;
        }
		public UnionExpr(Expr e1)
		{
			e = e1;
			next = null;
			current = this;
			ih = null;
			state = 0;
		}
		
		/*
		* (non-Javadoc)
		* 
		* @see com.ximpleware.xpath.Expr#evalBoolean(com.ximpleware.VTDNav)
		*/
		public override bool evalBoolean(VTDNav vn)
		{
            if (e.NodeSet==false)
            {
                return e.evalBoolean(vn);
            }
            else
            {
                bool a = false;
                vn.push2();
                // record teh stack size
                int size = vn.contextStack2.size;
                try
                {
                    a = (evalNodeSet(vn) != -1);
                }
                catch (System.Exception ee)
                {
                }
                //rewind stack
                vn.contextStack2.size = size;
                reset(vn);
                vn.pop2();
                return a;
            }
            /*if (e.Numerical)
            {
                double dval = e.evalNumber(vn);

                if (dval == 0.0 || System.Double.IsNaN(dval))
                    return false;
                return true;
            }

            String s = e.evalString(vn);
            if (s == null || s.Length == 0)
                return false;
            return true;*/

		}
		
		/*
		* (non-Javadoc)
		* 
		* @see com.ximpleware.xpath.Expr#evalNumber(com.ximpleware.VTDNav)
		*/
		public override double evalNumber(VTDNav vn)
		{
            if (e.NodeSet==false)
                return e.evalNumber(vn);
            int a = getStringIndex(vn);
            try
            {
                if (a != -1)
                    return vn.parseDouble(a);
            }
            catch (NavException ee)
            {
            }
            return Double.NaN;
		}
		
		/*
		* (non-Javadoc)
		* 
		* @see com.ximpleware.xpath.Expr#evalNodeSet(com.ximpleware.VTDNav)
		*/
		public override int evalNodeSet(VTDNav vn)
		{
			int a;
			if (this.next == null)
			{
				return e.evalNodeSet(vn);
			}
			else
			{
				while (true)
				{
					switch (state)
					{
						
						case 0: 
							if (ih == null)
							  ih = new intHash();
							if (current != null)
							{
								vn.push2();
								while ((a = current.e.evalNodeSet(vn)) != - 1)
								{
									if (isUnique(a))
									{
										state = 1;
										return a;
									}
								}
								state = 2;
								vn.pop2();
								break;
							}
							else
								state = 3;
							break;
						
						
						case 1: 
							while ((a = current.e.evalNodeSet(vn)) != - 1)
							{
								if (isUnique(a))
								{
									state = 1;
									return a;
								}
							}
							state = 2;
							vn.pop2();
							break;
						
						
						case 2: 
							current = current.next;
							if (current != null)
							{
								vn.push2();
								while ((a = current.e.evalNodeSet(vn)) != - 1)
								{
									if (isUnique(a))
									{
										state = 1;
										return a;
									}
								}
								vn.pop2();
								break;
							}
							else
								state = 3;
							break;
						
						
						case 3: 
							return - 1;
						
						
						default: 
							throw new XPathEvalException("Invalid state evaluating PathExpr");
						
					}
				}
			}
			
			/*
			* default: throw new XPathEvalException( "Invalid state evaluating
			* PathExpr");
			*/
		}
		
		/*
		* (non-Javadoc)
		* 
		* @see com.ximpleware.xpath.Expr#evalString(com.ximpleware.VTDNav)
		*/
		public override System.String evalString(VTDNav vn)
		{
              if (e.NodeSet == false){
                return e.evalString(vn);                
              }
              int a = getStringIndex(vn);
              try
              {
                  if (a != -1)
                      return vn.toString(a);
              }
              catch (NavException ee)
              {
              }
              return "";
		}
		
		/*
		* (non-Javadoc)
		* 
		* @see com.ximpleware.xpath.Expr#reset(com.ximpleware.VTDNav)
		*/
		public override void  reset(VTDNav vn)
		{
			// travese el list and reset every expression
			e.reset(vn);
			current = this;
			UnionExpr tmp = this.next;
			while (tmp != null)
			{
				tmp.e.reset(vn);
				tmp = tmp.next;
			}
			if (ih != null)
				ih.reset();
			state = 0;
		}
		
		/*
		* (non-Javadoc)
		* 
		* @see java.lang.Object#toString()
		*/
		public override System.String ToString()
		{
			// TODO Auto-generated method stub
			
			if (this.next == null)
			{
				return this.e.ToString();
			}
			else
			{
				return this.e.ToString() + " | " + this.next.ToString();
			}
		}
		
		/*
		* (non-Javadoc)
		* 
		* @see com.ximpleware.xpath.Expr#isNumerical()
		*/
		public override bool Numerical
		{
            get
            {
                // TODO Auto-generated method stub
                return e.Numerical;
            }
		}
		
		/*
		* (non-Javadoc)
		* 
		* @see com.ximpleware.xpath.Expr#isNodeSet()
		*/
		public override bool NodeSet
		{
            get
            {
                // TODO Auto-generated method stub
                return e.NodeSet;
            }
		}
		
		/*
		* (non-Javadoc)
		* 
		* @see com.ximpleware.xpath.Expr#isString()
		*/
		public override bool String
		{
            get
            {
                // TODO Auto-generated method stub
                return e.String;
            }
		}
		
		/*
		* (non-Javadoc)
		* 
		* @see com.ximpleware.xpath.Expr#isBoolean()
		*/
		public override bool Boolean
		{
            get
            {
                // TODO Auto-generated method stub
                return false;
            }
		}
		
		/*
		* (non-Javadoc)
		* 
		* @see com.ximpleware.xpath.Expr#requireContextSize()
		*/
		public override bool requireContextSize()
		{
            //bool b = false;
            UnionExpr tmp = this;
            while (tmp != null)
            {
                if (tmp.e.requireContextSize() == true)
                {
                    return true;
                }
                tmp = tmp.next;
            }
            return false;
		}
		
		/*
		* (non-Javadoc)
		* 
		* @see com.ximpleware.xpath.Expr#setContextSize(int)
		*/
        override public int ContextSize
        {
            set
            {
                current = this;
                current.e.ContextSize = value;
                UnionExpr tmp = this.next;
                while (tmp != null)
                {
                    tmp.e.ContextSize = value;
                    tmp = tmp.next;
                }
            }

        }
		
		/*
		* (non-Javadoc)
		* 
		* @see com.ximpleware.xpath.Expr#setPosition(int)
		*/
        override public int Position
        {
            set
            {
                current = this;
                current.e.Position = value;
                UnionExpr tmp = this.next;
                while (tmp != null)
                {
                    tmp.e.Position = value;
                    tmp = tmp.next;
                }

            }

        }
		
		public virtual bool isUnique(int i)
		{
			return ih.isUnique(i);
		}
	}
}
