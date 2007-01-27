/* 
* Copyright (C) 2002-2006 XimpleWare, info@ximpleware.com
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
using Expr = com.ximpleware.xpath.Expr;
using XPathEvalException = com.ximpleware.xpath.XPathEvalException;
namespace com.ximpleware
{
    /// <summary> The parser.java uses this class to contruct the corresponding
    /// AST for XPath expression when there are two operands and one
    /// operator
    /// 
    /// </summary>
    public class BinaryExpr : Expr
    {
        public override void adjust(int n)
        {
            left.adjust(n);
            right.adjust(n);
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

        protected internal FastIntBuffer fib1, fib2;
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
            fib1 = fib2 = null;
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
                case NE: return computeEQNE(op, vn);

                case LE: return left.evalNumber(vn) <= right.evalNumber(vn);

                case GE: return left.evalNumber(vn) >= right.evalNumber(vn);

                case LT: return left.evalNumber(vn) < right.evalNumber(vn);

                case GT: return left.evalNumber(vn) > right.evalNumber(vn);

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
        private bool computeEQNE(int op, VTDNav vn)
        {
            String st1, st2;
            if (left.NodeSet && right.NodeSet)
            {
                return compNodeSetNodeSet(left, right, vn, op);
            }
            else
            {
                if (left.Numerical && right.NodeSet){
                    return compNumericalNodeSet(left, right, vn, op);
                }
                if (left.NodeSet && right.Numerical)
                {
                    return compNumericalNodeSet(right, left, vn, op);
                }
                if (left.String && right.NodeSet)
                {
                    return compStringNodeSet(left, right, vn, op);
                }
                if (left.NodeSet && right.String)
                {
                    return compStringNodeSet(right, left, vn, op);
                }
            }

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

            if (st1 == null || st2 == null)
                if (op == EQ)
                    return false;
                else
                    return true;

            return (op == EQ) ? (st1.Equals(st2))
                    : (!st1.Equals(st2));

        }

        public override bool requireContextSize()
        {
            return left.requireContextSize() || right.requireContextSize();
        }

        private bool compStringNodeSet(Expr left, Expr right, VTDNav vn, int op)
        {
            int i, t, i1 = 0, stackSize;
            String st1;
            try
            {
                vn.push2();
                stackSize = vn.contextStack2.size;
                st1 = left.evalString(vn);

                if (st1 != null)
                    while ((i = right.evalNodeSet(vn)) != -1)
                    {
                        t = vn.getTokenType(i);
                        if (t == VTDNav.TOKEN_STARTING_TAG)
                        {
                            i1 = vn.getText();
                            if (i1 == -1)
                                break;
                            t = vn.getTokenType(i1);
                            if (i1 == VTDNav.TOKEN_CHARACTER_DATA)
                            {
                                if (vn.matchTokenString(i1, st1))
                                {
                                    vn.contextStack2.size = stackSize;
                                    vn.pop2();
                                    left.reset(vn);
                                    right.reset(vn);
                                    if (op == EQ)
                                        return true;
                                    else
                                        return false;
                                }
                            }
                            else
                            {
                                if (vn.matchRawTokenString(i1, st1))
                                {
                                    vn.contextStack2.size = stackSize;
                                    vn.pop2();
                                    left.reset(vn);
                                    right.reset(vn);
                                    if (op == EQ)
                                        return true;
                                    else
                                        return false;
                                }
                            }
                        }
                        else if (t == VTDNav.TOKEN_ATTR_NAME || t == VTDNav.TOKEN_ATTR_NS)
                        {
                            if (vn.matchTokenString(i + 1, st1))
                            {
                                vn.contextStack2.size = stackSize;
                                vn.pop2();
                                left.reset(vn);
                                right.reset(vn);
                                if (op == EQ)
                                    return true;
                                else
                                    return false;
                            }
                        }
                        else if (t == VTDNav.TOKEN_CHARACTER_DATA || t == VTDNav.TOKEN_CDATA_VAL)
                        {
                            if (vn.matchTokenString(i, st1))
                            {
                                vn.contextStack2.size = stackSize;
                                vn.pop2();
                                left.reset(vn);
                                right.reset(vn);
                                if (op == EQ)
                                    return true;
                                else
                                    return false;
                            }
                        }
                        else if (t == VTDNav.TOKEN_CDATA_VAL)
                        {
                            if (vn.matchRawTokenString(i, st1))
                            {
                                vn.contextStack2.size = stackSize;
                                vn.pop2();
                                left.reset(vn);
                                right.reset(vn);
                                if (op == EQ)
                                    return true;
                                else
                                    return false;
                            }
                        }
                    }
                vn.contextStack2.size = stackSize;
                vn.pop2();
                left.reset(vn);
                right.reset(vn);
                if (op == EQ)
                    return false;
                else
                    return true;

            }
            catch (System.Exception e)
            {
                //fib1.clear();
                //fib2.clear();
                throw new System.SystemException("Undefined behavior");
            }
        }

        private bool compNodeSetNodeSet(Expr left, Expr right, VTDNav vn, int op)
        {
            int i, t, i1 = 0, s1, s2;
            try
            {
                if (fib1 == null)
                    fib1 = new FastIntBuffer(BUF_SZ_EXP);
                if (fib2 == null)
                    fib2 = new FastIntBuffer(BUF_SZ_EXP);
                vn.push2();
                while ((i = left.evalNodeSet(vn)) != -1)
                {
                    t = vn.getTokenType(i);
                    if (t == VTDNav.TOKEN_STARTING_TAG)
                        i1 = vn.getText();
                    if (i1 != -1)
                        fib1.append(i1);
                    else if (t == VTDNav.TOKEN_ATTR_NAME || t == VTDNav.TOKEN_ATTR_NS)
                        fib1.append(i + 1);
                    else if (t == VTDNav.TOKEN_CHARACTER_DATA || t == VTDNav.TOKEN_CDATA_VAL)
                        fib1.append(i);
                }
                vn.pop2();
                vn.push2();
                while ((i = right.evalNodeSet(vn)) != -1)
                {
                    t = vn.getTokenType(i);
                    if (t == VTDNav.TOKEN_STARTING_TAG)
                        fib2.append(vn.getText());
                    else if (t == VTDNav.TOKEN_ATTR_NAME || t == VTDNav.TOKEN_ATTR_NS)
                        fib2.append(i + 1);
                    else if (t == VTDNav.TOKEN_CHARACTER_DATA || t == VTDNav.TOKEN_CDATA_VAL)
                        fib2.append(i);
                }
                vn.pop2();
                s1 = fib1.size();
                s2 = fib2.size();

                // start a while loop comparison
                for (int j = 0; j < s1; j++)
                {
                    for (int k = 0; k < s2; k++)
                    {
                        if (vn.matchTokens(fib1.intAt(j), vn, fib2.intAt(k)))
                        {
                            fib1.clear();
                            fib2.clear();
                            if (op == EQ)
                                return true;
                            else
                                return false;
                        }
                    }
                }
                fib1.clear();
                fib2.clear();
                if (op == EQ)
                    return false;
                else
                    return true;
            }
            catch (System.Exception e)
            {
                fib1.clear();
                fib2.clear();
                throw new System.SystemException("Undefined behavior");
            }
        }

        private bool compNumericalNodeSet(Expr left, Expr right, VTDNav vn, int op)
        {
            int i, t, i1 = 0, stackSize;
            try
            {

                vn.push2();
                stackSize = vn.contextStack2.size;
                while ((i = right.evalNodeSet(vn)) != -1)
                {
                    t = vn.getTokenType(i);
                    if (t == VTDNav.TOKEN_STARTING_TAG)
                    {
                        i1 = vn.getText();
                        if (i1 == -1)
                            break;
                        if (vn.parseDouble(i1) == left.evalNumber(vn))
                        {
                            vn.contextStack2.size = stackSize;
                            vn.pop2();
                            left.reset(vn);
                            right.reset(vn);
                            if (op == EQ)
                                return true;
                            else
                                return false;
                        }
                    }
                    else if (t == VTDNav.TOKEN_ATTR_NAME || t == VTDNav.TOKEN_ATTR_NS)
                    {
                        if (vn.parseDouble(i + 1) == left.evalNumber(vn))
                        {
                            vn.contextStack2.size = stackSize;
                            vn.pop2();
                            left.reset(vn);
                            right.reset(vn);
                            if (op == EQ)
                                return true;
                            else
                                return false;
                        }
                    }
                    else if (t == VTDNav.TOKEN_CHARACTER_DATA || t == VTDNav.TOKEN_CDATA_VAL)
                    {
                        if (vn.parseDouble(i) == left.evalNumber(vn))
                        {
                            vn.contextStack2.size = stackSize;
                            vn.pop2();
                            left.reset(vn);
                            right.reset(vn);
                            if (op == EQ)
                                return true;
                            else
                                return false;
                        }
                    }
                }
                vn.contextStack2.size = stackSize;
                vn.pop2();
                left.reset(vn);
                right.reset(vn);
                if (op == EQ)
                    return false;
                else
                    return true;

            }
            catch (System.Exception e)
            {
                //fib1.clear();
                //fib2.clear();
                throw new System.SystemException("Undefined behavior");
            }
        }
    }
}