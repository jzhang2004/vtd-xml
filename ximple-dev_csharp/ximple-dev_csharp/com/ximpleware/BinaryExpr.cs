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
    /// <summary> The parser.java uses this class to contruct the corresponding
    /// AST for XPath expression when there are two operands and one
    /// operator
    /// 
    /// </summary>
    public class BinaryExpr : Expr
    {
        public override int adjust(int n)
        {
            int i = left.adjust(n);
            int j = right.adjust(n);
            if (i > j) return i; else return j;
        }
        override public bool NodeSet
        {
            get
            {
                return false;
            }

        }
        override public bool Numerical
        {
            get
            {
                return isNumerical_Renamed_Field;
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
                return isBoolean_Renamed_Field;
            }
            // to support computer context size 
            // needs to add 
            // public boolean needContextSize();
            // public boolean SetContextSize(int contextSize);

        }
        override public int ContextSize
        {
            set
            {
                left.ContextSize = value;
                right.ContextSize = value;
            }

        }
        override public int Position
        {
            set
            {
                left.Position = value;
                right.Position = value;
            }

        }
        public const int ADD = 0;
        public const int SUB = 1;
        public const int MULT = 2;
        public const int DIV = 3;
        public const int MOD = 4;
        public const int OR = 5;
        public const int AND = 6;
        public const int EQ = 7;
        public const int NE = 8;
        public const int LE = 9;
        public const int GE = 10;
        public const int LT = 11;
        public const int GT = 12;
        public const int UNION = 13;

        public const int BUF_SZ_EXP = 7;
        protected internal int op;
        internal bool isNumerical_Renamed_Field;
        internal bool isBoolean_Renamed_Field;

        protected internal Expr left;
        protected internal Expr right;

        protected internal FastIntBuffer fib1;
        /// <summary> constructor</summary>
        /// <param name="l">
        /// </param>
        /// <param name="o">
        /// </param>
        /// <param name="r">
        /// </param>
        public BinaryExpr(Expr l, int o, Expr r)
        {
            op = o;
            left = l;
            right = r;
            fib1 = null;
            switch (op)
            {

                case ADD:
                case SUB:
                case MULT:
                case DIV:
                case MOD: isNumerical_Renamed_Field = true; isBoolean_Renamed_Field = false; break;

                case OR:
                case AND:
                case EQ:
                case NE:
                case LE:
                case GE:
                case LT:
                case GT: isNumerical_Renamed_Field = false; isBoolean_Renamed_Field = true;
                    goto default;

                default:
                    break;

            }
        }
        public override System.String ToString()
        {
            System.String os;
            switch (op)
            {

                case ADD: os = " + "; break;

                case SUB: os = " - "; break;

                case MULT: os = " * "; break;

                case DIV: os = " / "; break;

                case MOD: os = " mod "; break;

                case OR: os = " or "; break;

                case AND: os = " and "; break;

                case EQ: os = " = "; break;

                case NE: os = " != "; break;

                case LE: os = " <= "; break;

                case GE: os = " >= "; break;

                case LT: os = " < "; break;

                default: os = " > "; break;

            }

            return "(" + left + os + right + ")";
        }


        public override bool evalBoolean(VTDNav vn)
        {
            //int s1, s2;
            //int stackSize;
            //Expr e1, e2;
            //int t;
            //bool b = false;
            switch (op)
            {

                case OR: return left.evalBoolean(vn) || right.evalBoolean(vn);

                case AND: return left.evalBoolean(vn) && right.evalBoolean(vn);

                case EQ:
                case NE:

                case LE:

                case GE:

                case LT:
                case GT: return computeComp(op, vn);

                default: double dval = evalNumber(vn);
                    if (dval == -0.0 || dval == +0.0 || System.Double.IsNaN(dval))
                        return false;
                    return true;

            }
        }

        public override double evalNumber(VTDNav vn)
        {
            switch (op)
            {

                case ADD: return left.evalNumber(vn) + right.evalNumber(vn);

                case SUB: return left.evalNumber(vn) - right.evalNumber(vn);

                case MULT: return left.evalNumber(vn) * right.evalNumber(vn);

                case DIV: return left.evalNumber(vn) / right.evalNumber(vn);

                case MOD: return left.evalNumber(vn) % right.evalNumber(vn);

                default: if (evalBoolean(vn) == true)
                        return 1;
                    return 0;

            }
        }

        public override int evalNodeSet(VTDNav vn)
        {
            throw new XPathEvalException("BinaryExpr can't eval to a node set!");
        }

        public override System.String evalString(VTDNav vn)
        {
            if (Numerical)
            {

                double d = evalNumber(vn);
                //UPGRADE_WARNING: Data types in Visual C# might be different.  Verify the accuracy of narrowing conversions. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1042'"
                if (d == (long)d)
                {
                    //UPGRADE_WARNING: Data types in Visual C# might be different.  Verify the accuracy of narrowing conversions. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1042'"
                    return "" + (long)d;
                }
                else
                    return "" + d;
            }
            else
            {
                bool b = evalBoolean(vn);
                if (b)
                    return "true";
                else
                    return "false";
            }
        }

        public override void reset(VTDNav vn)
        {
            left.reset(vn); right.reset(vn);
        }

        /// <summary>
        /// compute the binary expr whose operand is either = or !=
        /// </summary>
        /// <param name="op"></param>
        /// <param name="vn"></param>
        /// <returns></returns>
        /// 
        /// 
        private bool computeComp(int op, VTDNav vn)
        {
            //int i, t, i1 = 0, stackSize, s1, s2;
            String st1, st2;
            if (left.NodeSet && right.NodeSet)
            {
                return compNodeSetNodeSet(left, right, vn, op);
            }
            else
            {
                if (left.Numerical && right.NodeSet)
                {
                    return compNumericalNodeSet(left, right, vn, op);
                }
                if (left.NodeSet && right.Numerical)
                {
                    return compNodeSetNumerical(left, right, vn, op);
                }
                if (left.String && right.NodeSet)
                {
                    return compStringNodeSet(left, right, vn, op);
                }
                if (left.NodeSet && right.String)
                {
                    return compNodeSetString(left, right, vn, op);
                }
            }
            if (op == EQ || op == NE)
            {
                if (left.Boolean || right.Boolean)
                {
                    if (op == EQ)
                        return left.evalBoolean(vn) == right.evalBoolean(vn);
                    else
                        return left.evalBoolean(vn) != right.evalBoolean(vn);
                }

                if (left.Numerical || right.Numerical)
                {
                    if (op == EQ)
                        return left.evalNumber(vn) == right.evalNumber(vn);
                    else
                        return left.evalNumber(vn) != right.evalNumber(vn);
                }

                st1 = left.evalString(vn);
                st2 = right.evalString(vn);
                /*if (st1 == null || st2 == null)
                    if (op == EQ)
                        return false;
                    else
                        return true;*/

                return (op == EQ) ? (st1.Equals(st2)) : (!st1.Equals(st2));
            }
            return compNumbers(left.evalNumber(vn), right.evalNumber(vn), op);

        }
        private bool compNumbers(double d1, double d2, int op)
        {
            switch (op)
            {
                case LE:
                    return d1 <= d2;
                case GE:
                    return d1 >= d2;
                case LT:
                    return d1 < d2;
                case GT:
                    return d1 > d2;
            }
            return false;
        }
        public override bool requireContextSize()
        {
            return left.requireContextSize() || right.requireContextSize();
        }



        private int getStringVal(VTDNav vn, int i)
        {
            int i1, t = vn.getTokenType(i);
            if (t == VTDNav.TOKEN_STARTING_TAG)
            {
                i1 = vn.getText();
                return i1;
            }
            else if (t == VTDNav.TOKEN_ATTR_NAME
                    || t == VTDNav.TOKEN_ATTR_NS || t==VTDNav.TOKEN_PI_NAME)
                return i + 1;
            else /*if (t == VTDNav.TOKEN_CHARACTER_DATA
                || t == VTDNav.TOKEN_CDATA_VAL)
            return i;*/
                return i;
        }

        private bool compNodeSetNodeSet(Expr left, Expr right, VTDNav vn, int op)
        {
            int i, i1 = 0, stackSize, s1;
            try
            {
                if (fib1 == null)
                    fib1 = new FastIntBuffer(BUF_SZ_EXP);
                vn.push2();
                stackSize = vn.contextStack2.size;
                while ((i = left.evalNodeSet(vn)) != -1)
                {
                    i1 = getStringVal(vn, i);
                    if (i1 != -1)
                        fib1.append(i1);
                }
                left.reset(vn);
                vn.contextStack2.size = stackSize;
                vn.pop2();
                vn.push2();
                stackSize = vn.contextStack2.size;
                while ((i = right.evalNodeSet(vn)) != -1)
                {
                    i1 = getStringVal(vn, i);
                    if (i1 != -1)
                    {
                        s1 = fib1.size_Renamed_Field;
                        for (int k = 0; k < s1; k++)
                        {
                            bool b = compareVV(fib1.intAt(k), vn, i1, op);
                            if (b)
                            {
                                fib1.size_Renamed_Field=0;
                                vn.contextStack2.size = stackSize;
                                vn.pop2();
                                right.reset(vn);
                                return true;
                            }
                        }
                    }
                }
                vn.contextStack2.size = stackSize;
                vn.pop2();
                right.reset(vn);
                fib1.size_Renamed_Field=0;
                return false;
            }
            catch (Exception e)
            {
                fib1.size_Renamed_Field=0;
                throw new System.SystemException("Undefined behavior");
            }
        }
        private bool compareVNumber1(int k, VTDNav vn, double d, int op)
        {
            double d1 = vn.parseDouble(k);
            switch (op)
            {
                case EQ:
                    return d == d1;
                case NE:
                    return d != d1;
                case GE:
                    return d >= d1;
                case LE:
                    return d <= d1;
                case GT:
                    return d > d1;
                default:
                    return d < d1;
            }
        }

        private bool compareVString1(int k, VTDNav vn, String s, int op)
        {
            int i = vn.compareTokenString(k, s);
            switch (i)
            {
                case -1:
                    if (op == NE || op == LT || op == LE)
                    {
                        return true;
                    }
                    break;
                case 0:
                    if (op == EQ || op == LE || op == GE)
                    {
                        return true;
                    }
                    break;
                case 1:
                    if (op == NE || op == GE || op == GT)
                    {
                        return true;
                    }
                    break;
            }
            return false;
        }
        private bool compareVString2(int k, VTDNav vn, String s, int op)
        {
            int i = vn.compareTokenString(k, s);
            switch (i)
            {
                case -1:
                    if (op == NE || op == GT || op == GE)
                    {
                        return true;
                    }
                    break;
                case 0:
                    if (op == EQ || op == LE || op == GE)
                    {
                        return true;
                    }
                    break;
                case 1:
                    if (op == NE || op == LE || op == LT)
                    {
                        return true;
                    }
                    break;
            }
            return false;
        }

        private bool compareVNumber2(int k, VTDNav vn, double d, int op)
        {
            double d1 = vn.parseDouble(k);
            switch (op)
            {
                case EQ:
                    return d1 == d;
                case NE:
                    return d1 != d;
                case GE:
                    return d1 >= d;
                case LE:
                    return d1 <= d;
                case GT:
                    return d1 > d;
                default:
                    return d1 < d;
            }
        }
        private bool compareVV(int k, VTDNav vn, int j, int op)
        {
            int i = vn.compareTokens(k, vn, j);
            switch (i)
            {

                case -1:
                    if (op == NE || op == LT || op == LE)
                    {

                        return true;
                    }
                    break;
                case 0:
                    if (op == EQ || op == LE || op == GE)
                    {
                        return true;
                    }
                    break;
               
                case 1:
                    if (op == NE || op == GE || op == GT)
                    {

                        return true;
                    }
                    break;
            }
            return false;
        }

        private bool compNumericalNodeSet(Expr left, Expr right, VTDNav vn, int op)
        {
            int i, i1 = 0, stackSize;
            double d;
            try
            {
                d = left.evalNumber(vn);
                vn.push2();
                stackSize = vn.contextStack2.size;
                while ((i = right.evalNodeSet(vn)) != -1)
                {
                    i1 = getStringVal(vn, i);
                    if (i1 != -1 && compareVNumber1(i1, vn, d, op))
                    {
                        right.reset(vn);
                        vn.contextStack2.size = stackSize;
                        vn.pop2();
                        return true;
                    }
                }
                vn.contextStack2.size = stackSize;
                vn.pop2();
                right.reset(vn);
                return false;
            }
            catch (Exception e)
            {
                throw new System.SystemException("Undefined behavior");
            }
        }
	private bool compNodeSetNumerical(Expr left, Expr right, VTDNav vn, int op ){
	     int i, i1 = 0, stackSize;
	     double d;
       try {
           d = right.evalNumber(vn);
           vn.push2();
           stackSize = vn.contextStack2.size;
           while ((i = left.evalNodeSet(vn)) != -1) {
               i1 = getStringVal(vn,i); 
               if (i1!=-1 && compareVNumber2(i1,vn,d,op)){
                   left.reset(vn);
                   vn.contextStack2.size = stackSize;
                   vn.pop2();
                   return true;
               }
           }    
           vn.contextStack2.size = stackSize;
           vn.pop2();
           left.reset(vn);            
           return false; 
       } catch (Exception e) {
           throw new System.SystemException("Undefined behavior");
       }
	}

    private bool compNodeSetString(Expr left, Expr right, VTDNav vn, int op)
    {
        int i, i1 = 0, stackSize;
        String s;
        try
        {
            s = right.evalString(vn);
            vn.push2();
            stackSize = vn.contextStack2.size;
            while ((i = left.evalNodeSet(vn)) != -1)
            {
                i1 = getStringVal(vn, i);
                if (i1 != -1 )
                {
                    bool b = compareVString1(i1, vn, s, op);
                    if (b)
                    {
                        left.reset(vn);
                        vn.contextStack2.size = stackSize;
                        vn.pop2();
                        return b;
                    }
                }
            }
            vn.contextStack2.size = stackSize;
            vn.pop2();
            left.reset(vn);
            return false; // compareEmptyNodeSet(op, s);
        }
        catch (Exception e)
        {
            throw new System.SystemException("Undefined behavior");
        }
    }
        private bool compareEmptyNodeSet(int op, String s)
        {
            if (op == NE)
            {
                if (s.Length == 0)
                {
                    return false;
                }
                else
                    return true;
            }
            else
            {
                if (s.Length == 0)
                {
                    return true;
                }
                else
                    return false;
            }
        }
    private bool compStringNodeSet(Expr left, Expr right, VTDNav vn, int op)
    {
        int i, i1 = 0, stackSize;
        String s;
        try
        {
            s = left.evalString(vn);
            vn.push2();
            stackSize = vn.contextStack2.size;
            while ((i = right.evalNodeSet(vn)) != -1)
            {
                i1 = getStringVal(vn, i);
                if (i1 != -1)
                {
                    bool b = compareVString2(i1, vn, s, op);
                    if (b)
                    {
                        right.reset(vn);
                        vn.contextStack2.size = stackSize;
                        vn.pop2();
                        return b;
                    }
                }
            }
            vn.contextStack2.size = stackSize;
            vn.pop2();
            right.reset(vn);
            return false;// compareEmptyNodeSet(op, s);
        }
        catch (Exception e)
        {
            throw new System.SystemException("Undefined behavior");
        }
    }
	
	}
}