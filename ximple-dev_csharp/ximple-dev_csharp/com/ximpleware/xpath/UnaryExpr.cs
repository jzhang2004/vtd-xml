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
using com.ximpleware;
namespace com.ximpleware.xpath
{
    /// <summary> Parser use this class to represent Unary Expr
    /// 
    /// </summary>
    public class UnaryExpr : Expr
    {
        /// <summary>
        /// 
        /// </summary>
        /// <param name="n"></param>
        /// <returns></returns>
        public override int adjust(int n) { return 0; }
        /// <summary>
        /// test whether expression returns a node set
        /// </summary>
        override public bool NodeSet
        {
            get
            {
                return false;
            }

        }
        /// <summary>
        /// test whether expression returns a Numerical (double)
        /// </summary>
        override public bool Numerical
        {
            get
            {
                return true;
            }

        }
        /// <summary>
        /// test whether the expressoin returns a string
        /// </summary>
        override public bool String
        {
            get
            {
                return false;
            }

        }
        /// <summary>
        /// test whether the expression evalutes to a boolean
        /// </summary>
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
        /// <summary>
        /// 
        /// </summary>
        public Expr operand;
        /// <summary>
        /// 
        /// </summary>
        /// <param name="o"></param>
        /// <param name="e"></param>
        public UnaryExpr(int o, Expr e)
        {
            op = o;
            operand = e;
        }
        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public override System.String ToString()
        {
            return "-" + operand;
        }

        /// <summary>
        /// evaluate a union expression to a boolean
        /// </summary>
        /// <param name="vn"></param>
        /// <returns></returns>
        public override bool evalBoolean(VTDNav vn)
        {

            return operand.evalBoolean(vn);
        }
        /// <summary>
        /// evaluate a union expression to double
        /// </summary>
        /// <param name="vn"></param>
        /// <returns></returns>
        public override double evalNumber(VTDNav vn)
        {
            return (-1) * operand.evalNumber(vn);
        }
        /// <summary>
        /// evaluate a union expression to a node set
        /// </summary>
        /// <param name="vn"></param>
        /// <returns></returns>
        public override int evalNodeSet(VTDNav vn)
        {

            throw new XPathEvalException("UnaryExpr can't eval to a node set!");
        }

        /// <summary>
        /// evaluate a union expression to a string
        /// </summary>
        /// <param name="vn"></param>
        /// <returns></returns>
        public override System.String evalString(VTDNav vn)
        {
            double dval = operand.evalNumber(vn);
            //UPGRADE_WARNING: Data types in Visual C# might be different.  Verify the accuracy of narrowing conversions. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1042'"
            if (dval == (int)dval)
            {
                //UPGRADE_WARNING: Data types in Visual C# might be different.  Verify the accuracy of narrowing conversions. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1042'"
                return "" + ((int)dval);
            }
            return "" + dval;
        }
        /// <summary>
        /// reset the expression
        /// </summary>
        /// <param name="vn"></param>
        public override void reset(VTDNav vn)
        {
            operand.reset(vn);
        }
        // to support computer context size 
        // needs to add 
        /// <summary>
        /// test whether the expression requires contextSize
        /// </summary>
        /// <returns></returns>
        public override bool requireContextSize()
        {
            return operand.requireContextSize();
        }

        /// <summary>
        /// clear internal cache
        /// </summary>
        public override void clearCache()
        {
            operand.clearCache();
        }
        /// <summary>
        /// test whether the expression is a constant
        /// </summary>
        /// <returns></returns>
        public override bool isFinal()
        {
            return operand.isFinal();
        }
        /// <summary>
        /// mark teh expression as cacheable
        /// </summary>
        public override void markCacheable()
        {
            operand.markCacheable();
        }
        /// <summary>
        /// mark the expression as cacheable second type
        /// </summary>
        public override void markCacheable2()
        {
            operand.markCacheable2();
        }
    }
}