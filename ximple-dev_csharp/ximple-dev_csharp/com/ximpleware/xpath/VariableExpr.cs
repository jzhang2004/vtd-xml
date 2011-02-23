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

namespace com.ximpleware.xpath
{
    public class VariableExpr:Expr
    {


        private String exprName;
        private Expr exprVal;

        public VariableExpr(String name, Expr e)
        {
            exprName = name;
            exprVal = e;
        }


        override public int adjust(int n)
        {
            // TODO Auto-generated method stub
            return exprVal.adjust(n);
        }


        override public bool evalBoolean(VTDNav vn)
        {
            // TODO Auto-generated method stub
            return exprVal.evalBoolean(vn);
        }


        override public int evalNodeSet(VTDNav vn)
        {
            // TODO Auto-generated method stub
            return exprVal.evalNodeSet(vn);
        }


        override public double evalNumber(VTDNav vn)
        {
            // TODO Auto-generated method stub
            return exprVal.evalNumber(vn);
        }


        override public String evalString(VTDNav vn)
        {
            // TODO Auto-generated method stub
            return exprVal.evalString(vn);
        }


      
        override public bool requireContextSize()
        {
            // TODO Auto-generated method stub
            return exprVal.requireContextSize();
        }


        override public void reset(VTDNav vn)
        {
            // TODO Auto-generated method stub
            exprVal.reset(vn);
        }


        override public String ToString()
        {
            // TODO Auto-generated method stub
            return "$" + exprName;
        }

        override public bool NodeSet
        {
            get
            {
                return exprVal.NodeSet;
            }

        }
        override public bool Numerical
        {
            get
            {
                return exprVal.Numerical;
            }

        }
        override public bool String
        {
            get
            {
                return exprVal.String;
            }

        }
        override public bool Boolean
        {
            get
            {
                return exprVal.Boolean;
            }

        }

        override public int ContextSize
        {
            set
            {
                exprVal.ContextSize = value;
            }

        }
        override public int Position
        {
            set
            {
                exprVal.Position = value;
            }

        }
    }
}
