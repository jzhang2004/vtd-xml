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
namespace com.ximpleware
{
    /// <summary> 
    /// This is the abstract class on which all XPath expressions 
    /// are based
    /// </summary>

    abstract public class Expr
    {
        abstract public bool Numerical { get; }
        abstract public bool NodeSet { get; }
        abstract public bool String { get; }
        abstract public bool Boolean { get; }
        abstract public int ContextSize { set; }
        abstract public int Position { set; }

        abstract public bool evalBoolean(VTDNav vn);

        abstract public double evalNumber(VTDNav vn);

        abstract public int evalNodeSet(VTDNav vn);

        abstract public System.String evalString(VTDNav vn);

        abstract public void reset(VTDNav vn);
        abstract public override System.String ToString();

        abstract public bool requireContextSize();
        abstract public int adjust(int n);
        protected int getStringIndex(VTDNav vn)
        {
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
                        a++;
                    }
                    else if (vn.getTokenType(a) == VTDNav.TOKEN_STARTING_TAG)
                    {
                        a = vn.getText();
                    }
                    else if (t == VTDNav.TOKEN_PI_NAME)
                    {
                        a++;
                    }
                }
            }
            catch (Exception e)
            {

            }
            vn.contextStack2.size = size;
            reset(vn);
            vn.pop2();
            return a;
        }
    }
}