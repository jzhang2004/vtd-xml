/* 
* Copyright (C) 2002-2012 XimpleWare, info@ximpleware.com
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
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace com.ximpleware
{
    public class CachedExpr : Expr
    {
        Expr e;
        bool cached;
        bool eb;
        double en;
        String es;
        FastIntBuffer ens;
        int count;
        VTDNav vn1;

        public CachedExpr(Expr e1)
        {
            e = e1;
            cached = false;
            ens = null;
            count = 0;
            vn1 = null;
        }

        public override bool evalBoolean(VTDNav vn)
        {
            // TODO Auto-generated method stub
            if (cached)
            {
                return eb;
            }
            else
            {
                eb = e.evalBoolean(vn);
                return eb;
            }
        }


        public override double evalNumber(VTDNav vn)
        {
            // TODO Auto-generated method stub
            if (cached)
            {
                return en;
            }
            else
            {
                cached = true;
                en = e.evalNumber(vn);
                return en;
            }
        }


        public override int evalNodeSet(VTDNav vn)
        {
            // TODO Auto-generated method stub
            int i = -1;
            if (cached)
            {
                if (count < ens.size_Renamed_Field)
                {
                    i = ens.intAt(count);
                    vn.recoverNode(i);
                    count++;
                    return i;
                }
                else
                    return -1;

            }
            else
            {
                cached = true;

                if (ens == null)
                {
                    ens = new FastIntBuffer(8);//page size 64
                }
                //record node set
                while ((i = e.evalNodeSet(vn)) != -1)
                {
                    ens.append(i);
                }
                e.reset(vn);
                if (ens.size_Renamed_Field > 0)
                {
                    i = ens.intAt(count);//count should be zero
                    vn.recoverNode(i);
                    count++;
                    return i;
                }
                else
                    return -1;
            }
        }


        public override String evalString(VTDNav vn)
        {
            if (cached)
            {
                return es;
            }
            else
            {
                cached = true;
                es = e.evalString(vn);
                return es;
            }
        }


        public override void reset(VTDNav vn)
        {
            count = 0;
            /*if (vn1!=vn){
                cached = false;
                if (ens!=null)
                    ens.clear();
                e.reset(vn);
            }*/
            // TODO Auto-generated method stub
        }


        public override String ToString()
        {
            // TODO Auto-generated method stub
            return "cached(" + e.ToString() + ")";
        }


        public override bool Numerical
        {
            get
            { // TODO Auto-generated method stub
                return e.Numerical;
            }
        }


        public override bool NodeSet
        {
            // TODO Auto-generated method stub
            get { return e.NodeSet; }
        }


        public override bool String
        {
            // TODO Auto-generated method stub

            get { return e.String; }

        }


        public override bool Boolean
        {
            get
            {
                // TODO Auto-generated method stub
                return e.Boolean;
            }
        }


        public override bool requireContextSize()
        {
            // TODO Auto-generated method stub
            return e.requireContextSize();
        }


        public override int ContextSize
        {
            // TODO Auto-generated method stub
            set { e.ContextSize = value; }
        }


        public override int Position
        {
            // TODO Auto-generated method stub
            set { e.Position=value; }
        }


        public override int adjust(int n)
        {
            // TODO Auto-generated method stub
            return e.adjust(n);
        }


        public override bool isFinal()
        {
            // TODO Auto-generated method stub
            return e.isFinal();
        }


        public override void markCacheable()
        {
            // TODO Auto-generated method stub
            e.markCacheable();
        }


        public override void markCacheable2()
        {
            // TODO Auto-generated method stub
            e.markCacheable2();
        }

        public override void clearCache()
        {
            cached = false;
            if (ens != null)
                ens.clear();
            e.clearCache();
        }

    }
}
