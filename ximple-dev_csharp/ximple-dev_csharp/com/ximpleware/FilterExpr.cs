/* 
* Copyright (C) 2002-2015 XimpleWare, info@ximpleware.com
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
using Predicate = com.ximpleware.xpath.Predicate;
using XPathEvalException = com.ximpleware.XPathEvalException;
namespace com.ximpleware
{

    /// <summary> 
    /// TODO To change the template for this generated type comment go to
    /// Window - Preferences - Java - Code Style - Code Templates
    /// </summary>
    public class FilterExpr : Expr
    {
        public override int adjust(int n)
        {
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
        public bool out_of_range;
        //public int position;

        public FilterExpr(Expr l, Predicate pr)
        {
            e = l;
            p = pr;
            //stackSize = 0;
            //position = 1;
            //fib = new FastIntBuffer(8);
            first_time = true;
            out_of_range = false;
            pr.fe = this;
            needReordering = l.needReordering;
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
                a = (evalNodeSet(vn) != -1);
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
            //String s = "";
            double d = Double.NaN;
            int a = 0x7fffffff, k = -1;
            vn.push2();
            int size = vn.contextStack2.size;
            try
            {
                if (needReordering)
                {
                    while ((k = evalNodeSet(vn)) != -1)
                    {
                        // a = evalNodeSet(vn);
                        if (k < a)
                            a = k;
                    }
                    if (a == 0x7fffffff)
                        a = -1;
                }
                else
                {
                    a = evalNodeSet(vn);
                }
                if (a != -1)
                {
                    int t = vn.getTokenType(a);
                    if (t == VTDNav.TOKEN_ATTR_NAME)
                    {
                        d = vn.parseDouble(a + 1);
                    }
                    else if (t == VTDNav.TOKEN_STARTING_TAG || t == VTDNav.TOKEN_DOCUMENT)
                    {
                        d = vn.XPathStringVal2Double(a);// Double.parseDouble(s);
                    }
                    else if (t == VTDNav.TOKEN_PI_NAME)
                    {
                        if (a + 1 < vn.vtdSize || vn.getTokenType(a + 1) == VTDNav.TOKEN_PI_VAL)
                            //s = vn.toString(a+1); 	
                            d = vn.parseDouble(a + 1);
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
            // if tne predicate require context size
            // needs to precompute the context size
            // vn.push2();
            // computerContext();
            // set contxt();
            // vn.pop2()
            // if the context size is zero
            // get immediately set teh state to end
            // or backward
            if (first_time && p.requireContext)
            {
                first_time = false;
                int i = 0;
                //vn.push2();
                e.adjust(vn.getTokenCount());
                while (e.evalNodeSet(vn) != -1)
                    i++;
                //vn.pop2();
                p.ContextSize = i;
                reset2(vn);
            }
            if (out_of_range)
                return -1;
            int a = e.evalNodeSet(vn);
            while (a != -1)
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
            return -1;
        }

        public override System.String evalString(VTDNav vn)
        {
            String s = "";
            int a = 0x7fffffff, k = -1;
            vn.push2();
            int size = vn.contextStack2.size;
            try
            {
                //a = evalNodeSet(vn);
                if (needReordering)
                {
                    while ((k = evalNodeSet(vn)) != -1)
                    {
                        // a = evalNodeSet(vn);
                        if (k < a)
                            a = k;
                    }
                    if (a == 0x7fffffff)
                        a = -1;
                }
                else
                {
                    a = evalNodeSet(vn);
                }
                if (a != -1)
                {
                    int t = vn.getTokenType(a);
                    switch (t)
                    {
                        case VTDNav.TOKEN_STARTING_TAG:
                        case VTDNav.TOKEN_DOCUMENT:
                            s = vn.getXPathStringVal2(a,(short)0);
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

        public override void reset(VTDNav vn)
        {
            reset2(vn);
            //vn.contextStack2.size = stackSize; 
            //position = 1;
            first_time = true;
        }

        public void reset2(VTDNav vn)
        {
            out_of_range = false;
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

        public override bool isFinal()
        {
            return e.isFinal();
        }

        public override void clearCache()
        {
            e.clearCache();
            if (p.expr != null)
            {
                p.expr.clearCache();
            }
        }

	
	    public override void markCacheable2(){
		e.markCacheable2();	
		if (p.expr!=null){
			if (p.expr.isFinal()&&p.expr.NodeSet){
				CachedExpr ce = new CachedExpr(p.expr);
				p.expr = ce;
			}
			p.expr.markCacheable2();
		}
	}
	
	 public override void markCacheable(){
		e.markCacheable();
		if (p.expr!=null){
			if (p.expr.isFinal()&&p.expr.NodeSet){
				CachedExpr ce = new CachedExpr(p.expr);
				p.expr = ce;
			}
			p.expr.markCacheable2();
		}
	}	
	
	
    }
}