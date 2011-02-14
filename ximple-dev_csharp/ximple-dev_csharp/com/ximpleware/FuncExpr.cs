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
using System.Text;
using Alist = com.ximpleware.xpath.Alist;
using Expr = com.ximpleware.Expr;
using FuncName = com.ximpleware.xpath.FuncName;
using UnsupportedException = com.ximpleware.xpath.UnsupportedException;
using XPathEvalException = com.ximpleware.XPathEvalException;
namespace com.ximpleware
{
    /// <summary> FuncExpr implements the function expression defined
    /// in XPath spec
    /// 
    /// </summary>
    public class FuncExpr : Expr
    {
        public override int adjust(int n)
        {
            int i = 0;
            switch (opCode)
            {
                case FuncName.COUNT:
                    i = argumentList.e.adjust(n);
                    return i;
                case FuncName.SUM:
                    i = argumentList.e.adjust(n);
                    return i;
                default:
                    return 0;
            }
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
                return isString_Renamed_Field;
            }

        }
        override public bool Boolean
        {
            get
            {
                return isBoolean_Renamed_Field;
            }

        }
        override public int ContextSize
        {
            set
            {
                if (opCode == FuncName.LAST)
                {
                    contextSize = value;
                    //System.out.println("contextSize: "+size);
                }
                else
                {
                    Alist temp = argumentList;
                    //boolean b = false;
                    while (temp != null)
                    {
                        temp.e.ContextSize = value;
                        temp = temp.next;
                    }
                }
            }

        }
        override public int Position
        {
            set
            {
                if (opCode == FuncName.POSITION)
                {
                    position = value;
                    //System.out.println("PO: "+size);
                }
                else
                {
                    Alist temp = argumentList;
                    //boolean b = false;
                    while (temp != null)
                    {
                        temp.e.Position = value;
                        temp = temp.next;
                    }
                }
            }

        }

        public Alist argumentList;
        public int opCode;
        internal bool isNumerical_Renamed_Field;
        internal bool isBoolean_Renamed_Field;
        internal bool isString_Renamed_Field;
        internal int contextSize;
        //double d;
        internal int position;
        internal int a;
        internal int argCount()
        {
            Alist temp = argumentList;
            int count = 0;
            while (temp != null)
            {
                count++;
                temp = temp.next;
            }
            return count;
        }
        public FuncExpr(int oc, Alist list)
        {
            a = 0;
            opCode = oc;
            argumentList = list;
            isBoolean_Renamed_Field = false;
            isString_Renamed_Field = false;
            position = 0;
            //isNodeSet = false;
            isNumerical_Renamed_Field = false;
            switch (opCode)
            {

                case FuncName.LAST: isNumerical_Renamed_Field = true; break;

                case FuncName.POSITION: isNumerical_Renamed_Field = true; break;

                case FuncName.COUNT: isNumerical_Renamed_Field = true; break;

                case FuncName.LOCAL_NAME: isString_Renamed_Field = true; break;

                case FuncName.NAMESPACE_URI: isString_Renamed_Field = true; break;

                case FuncName.NAME: isString_Renamed_Field = true; break;

                case FuncName.STRING: isString_Renamed_Field = true; break;

                case FuncName.CONCAT: isString_Renamed_Field = true; break;

                case FuncName.STARTS_WITH: isBoolean_Renamed_Field = true; break;

                case FuncName.CONTAINS: isBoolean_Renamed_Field = true; break;

                case FuncName.SUBSTRING_BEFORE: isString_Renamed_Field = true; break;

                case FuncName.SUBSTRING_AFTER: isString_Renamed_Field = true; break;

                case FuncName.SUBSTRING: isString_Renamed_Field = true; break;

                case FuncName.STRING_LENGTH: isNumerical_Renamed_Field = true; break;

                case FuncName.NORMALIZE_SPACE: isString_Renamed_Field = true; break;

                case FuncName.TRANSLATE: isString_Renamed_Field = true; break;

                case FuncName.BOOLEAN: isBoolean_Renamed_Field = true; break;

                case FuncName.NOT: isBoolean_Renamed_Field = true; break;

                case FuncName.TRUE: isBoolean_Renamed_Field = true; break;

                case FuncName.FALSE: isBoolean_Renamed_Field = true; break;

                case FuncName.LANG: isBoolean_Renamed_Field = true; break;

                case FuncName.NUMBER: isNumerical_Renamed_Field = true; break;

                case FuncName.SUM: isNumerical_Renamed_Field = true; break;

                case FuncName.FLOOR: isNumerical_Renamed_Field = true; break;

                case FuncName.CEILING: isNumerical_Renamed_Field = true; break;

                case FuncName.ROUND: isNumerical_Renamed_Field = true; break;
                case FuncName.ABS: isNumerical_Renamed_Field = true; break;
                case FuncName.ROUND_HALF_TO_EVEN:
                    isNumerical_Renamed_Field = true; break;
                case FuncName.ROUND_HALF_TO_ODD:
                    isNumerical_Renamed_Field = true; break;
                case FuncName.CODE_POINTS_TO_STRING:
                    isString_Renamed_Field = true; break;
                case FuncName.COMPARE: isBoolean_Renamed_Field = true; break;
                case FuncName.UPPER_CASE: isString_Renamed_Field = true; break;
                case FuncName.LOWER_CASE: isString_Renamed_Field = true; break;
                case FuncName.ENDS_WITH: isBoolean_Renamed_Field = true; break;
                case FuncName.QNAME: isString_Renamed_Field = true; break;
                case FuncName.LOCAL_NAME_FROM_QNAME:
                    isString_Renamed_Field = true; break;
                case FuncName.NAMESPACE_URI_FROM_QNAME:
                    isString_Renamed_Field = true; break;
                case FuncName.NAMESPACE_URI_FOR_PREFIX:
                    isString_Renamed_Field = true; break;
                case FuncName.RESOLVE_QNAME: isString_Renamed_Field = true; break;
                case FuncName.IRI_TO_URI: isString_Renamed_Field = true; break;
                case FuncName.ESCAPE_HTML_URI: isString_Renamed_Field = true; break;
                default: isString_Renamed_Field = true; break;
            }
        }

        public override System.String ToString()
        {
            if (argumentList == null)
                return fname() + " (" + ")";
            return fname() + " (" + argumentList + ")";
        }
        private String getString(VTDNav vn)
        {
            if (argCount() == 0)
                try
                {
                    if (vn.atTerminal)
                    {
                        if (vn.getTokenType(vn.LN) == VTDNav.TOKEN_CDATA_VAL)
                            return vn.toRawString(vn.LN);
                        return vn.toString(vn.LN);
                    }
                    return vn.toString(vn.getCurrentIndex());
                }
                catch (NavException e)
                {
                    return null; // this will almost never occur
                }
            else if (argCount() == 1)
            {
                return argumentList.e.evalString(vn);
            }
            else
                throw new System.ArgumentException("String()'s argument count is invalid");
        }
        private String getLocalName(VTDNav vn)
        {
            if (argCount() == 0)
            {
                try
                {
                    int index = vn.getCurrentIndex();
                    int type = vn.getTokenType(index);
                    if (vn.ns && (type == VTDNav.TOKEN_STARTING_TAG
                            || type == VTDNav.TOKEN_ATTR_NAME))
                    {
                        int offset = vn.getTokenOffset(index);
                        int length = vn.getTokenLength(index);
                        if (length < 0x10000)
                        {
                            if (vn.localNameIndex != index)
                            {
                                vn.localNameIndex = index;
                                vn.localName = vn.toRawString(index);
                            }
                            return vn.localName;
                        }
                        else
                        {
                            int preLen = length >> 16;
                            int QLen = length & 0xffff;
                            if (preLen != 0)
                            {
                                if (vn.localNameIndex != index)
                                {
                                    vn.localNameIndex = index;
                                    vn.localName = vn.toRawString(offset + preLen + 1, QLen
                                            - preLen - 1);
                                }
                                return vn.localName;
                            }
                            else
                            {
                                if (vn.localNameIndex != index)
                                {
                                    vn.localNameIndex = index;
                                    vn.localName = vn.toRawString(offset, QLen);
                                }
                                return vn.localName;
                            }
                        }
                    }
                    else
                        return "";
                }
                catch (NavException e)
                {
                    return ""; // this will almost never occur
                }

            }
            else if (argCount() == 1)
            {
                int a = evalFirstArgumentListNodeSet2(vn);

                if (a == -1 || vn.ns == false)
                    return "";
                int type = vn.getTokenType(a);
                if (type != VTDNav.TOKEN_STARTING_TAG && type != VTDNav.TOKEN_ATTR_NAME)
                    return "";
                try
                {
                    int offset = vn.getTokenOffset(a);
                    int length = vn.getTokenLength(a);
                    if (length < 0x10000)
                        return vn.toRawString(a);
                    else
                    {
                        int preLen = length >> 16;
                        int QLen = length & 0xffff;
                        if (preLen != 0)
                            return vn.toRawString(offset + preLen + 1,
                                    QLen - preLen - 1);
                        else
                        {
                            return vn.toRawString(offset, QLen);
                        }
                    }
                }
                catch (NavException e)
                {
                    return ""; // this will almost never occur
                }
            }
            else
                throw new System.ArgumentException
                ("local-name()'s argument count is invalid");
        }

        private String getNameSpaceURI(VTDNav vn)
        {
            if (argCount() == 0)
            {
                try
                {
                    int i = vn.getCurrentIndex();
                    int type = vn.getTokenType(i);

                    if (vn.ns && (type == VTDNav.TOKEN_STARTING_TAG
                            || type == VTDNav.TOKEN_ATTR_NAME))
                    {
                        int a = vn.lookupNS();
                        if (a == 0)
                            return "";
                        else
                            return vn.toString(a);
                    }
                    return "";
                }
                catch (Exception e)
                {
                    return "";
                }
            }
            else if (argCount() == 1)
            {
                vn.push2();
                int size = vn.contextStack2.size;
                int a = -1;
                try
                {
                    a = argumentList.e.evalNodeSet(vn);
                }
                catch (Exception e)
                {
                }
                String s = "";
                // return a;
                try
                {
                    if (a == -1 || vn.ns == false)
                    { }
                    else
                    {
                        int type = vn.getTokenType(a);
                        if (type == VTDNav.TOKEN_STARTING_TAG
                                || type == VTDNav.TOKEN_ATTR_NAME)
                            s = vn.toString(vn.lookupNS());

                    }
                }
                catch (Exception e) { };
                vn.contextStack2.size = size;
                argumentList.e.reset(vn);
                vn.pop2();
                return s;

            }
            else
                throw new System.ArgumentException
                ("namespace-uri()'s argument count is invalid");
        }

        private String getName(VTDNav vn)
        {
            int a;
            if (argCount() == 0)
            {
                a = vn.getCurrentIndex();
                int type = vn.getTokenType(a);

                if ((type == VTDNav.TOKEN_STARTING_TAG
                        || type == VTDNav.TOKEN_ATTR_NAME))
                {
                    try
                    {
                        if (vn.nameIndex != a)
                        {
                            vn.name = vn.toString(a);
                            vn.nameIndex = a;
                        }
                        return vn.name;
                    }
                    catch (Exception e)
                    {
                        return "";
                    }
                }
                else
                    return "";
            }
            else if (argCount() == 1)
            {
                a = evalFirstArgumentListNodeSet2(vn);
                try
                {
                    if (a == -1 || vn.ns == false)
                        return "";
                    else
                    {
                        int type = vn.getTokenType(a);
                        if (type == VTDNav.TOKEN_STARTING_TAG
                                || type == VTDNav.TOKEN_ATTR_NAME)
                            return vn.toString(a);
                        return "";
                    }
                }
                catch (Exception e)
                {
                }
                return "";
            }
            else
                throw new System.ArgumentException
                ("name()'s argument count is invalid");

        }

        public override System.String evalString(VTDNav vn)
        {
            //int d = 0;
            switch (opCode)
            {
                case FuncName.CONCAT:
                    return concat(vn);
                case FuncName.LOCAL_NAME:
                    return getLocalName(vn);
                case FuncName.NAMESPACE_URI:
                    return getNameSpaceURI(vn);
                case FuncName.NAME:
                    return getName(vn);

                case FuncName.STRING:
                    return getString(vn);

                //goto case FuncName.SUBSTRING_BEFORE;

                case FuncName.SUBSTRING_BEFORE: return subStringBefore(vn);
                case FuncName.SUBSTRING_AFTER: return subStringAfter(vn);
                case FuncName.SUBSTRING: return subString(vn);
                case FuncName.TRANSLATE: return translate(vn);
                case FuncName.NORMALIZE_SPACE: return normalizeSpace(vn);
                case FuncName.CODE_POINTS_TO_STRING: 
                    throw new com.ximpleware.xpath.UnsupportedException("not yet implemented");
   			    case FuncName.UPPER_CASE: return upperCase(vn);
                case FuncName.LOWER_CASE: return lowerCase(vn);
   			    case FuncName.QNAME:
   			    case FuncName.LOCAL_NAME_FROM_QNAME:
   			    case FuncName.NAMESPACE_URI_FROM_QNAME:
   			    case FuncName.NAMESPACE_URI_FOR_PREFIX:
   			    case FuncName.RESOLVE_QNAME:
   			    case FuncName.IRI_TO_URI:
   			    case FuncName.ESCAPE_HTML_URI:
   			    case FuncName.ENCODE_FOR_URI:
   			        throw new com.ximpleware.xpath.UnsupportedException("not yet implemented");

                default: if (Boolean)
                    {
                        if (evalBoolean(vn) == true)
                            return "true";
                        else
                            return "false";
                    }
                    else
                    {
                        return "" + evalNumber(vn);
                    }
                //break;

            }
        }

        private string translate(VTDNav vn)
        {
            int numArg = argCount();

            if (numArg == 3)
            {
                String resultStr = argumentList.e.evalString(vn);
                String indexStr = argumentList.next.e.evalString(vn);

                if (resultStr == null || resultStr.Length == 0 || indexStr == null || indexStr.Length == 0) return resultStr;

                StringBuilder resultSB = new StringBuilder(resultStr);
                String replace = argumentList.next.next.e.evalString(vn);

                StringBuilder usedCharStr = new StringBuilder();

                int lenRep = (replace != null) ? replace.Length : 0;

                for (int i = 0; i < indexStr.Length; i++)
                {
                    char idxChar = indexStr[i];

                    if (usedCharStr.ToString().IndexOf(idxChar) < 0)
                    {

                        if (i < lenRep)
                        {
                            resultSB = resultSB.Replace(idxChar, replace[i]);
                        }
                        else
                        {
                            resultSB = resultSB.Replace(idxChar.ToString(), "");
                        }

                        usedCharStr.Append(idxChar);
                    }

                }

                return resultSB.ToString();

            }
            else
            {
                throw new System.ArgumentException("Argument count for translate() is invalid. Expected: 3; Actual: " + numArg);
            }
        }

        public override double evalNumber(VTDNav vn)
        {
            int ac = 0;
            switch (opCode)
            {

                case FuncName.LAST: if (argCount() != 0)
                        throw new System.ArgumentException("floor()'s argument count is invalid");
                    return contextSize;

                case FuncName.POSITION: if (argCount() != 0)
                        throw new System.ArgumentException("position()'s argument count is invalid");
                    return position;

                case FuncName.COUNT: return count(vn);

                case FuncName.NUMBER: if (argCount() != 1)
                        throw new System.ArgumentException("number()'s argument count is invalid");
                    return argumentList.e.evalNumber(vn);


                case FuncName.SUM: return sum(vn);

                case FuncName.FLOOR: if (argCount() != 1)
                        throw new System.ArgumentException("floor()'s argument count is invalid");
                    return System.Math.Floor(argumentList.e.evalNumber(vn));


                case FuncName.CEILING: if (argCount() != 1)
                        throw new System.ArgumentException("ceiling()'s argument count is invalid");
                    return System.Math.Ceiling(argumentList.e.evalNumber(vn));


                case FuncName.STRING_LENGTH:
                    //return stringLen
                    ac = argCount();
                    if (ac == 0)
                    {
                        try
                        {
                            if (vn.atTerminal == true)
                            {
                                int type = vn.getTokenType(vn.LN);
                                if (type == VTDNav.TOKEN_ATTR_NAME || type == VTDNav.TOKEN_ATTR_NS)
                                {
                                    return vn.getStringLength(vn.LN + 1);
                                }
                                else
                                {
                                    return vn.getStringLength(vn.LN);
                                }
                            }
                            else
                            {
                                int i = vn.getText();
                                if (i == -1)
                                    return 0;
                                else
                                    return vn.getStringLength(i);
                            }
                        }
                        catch (NavException e)
                        {
                            return 0;
                        }
                    }
                    else if (ac == 1)
                    {
                        return argumentList.e.evalString(vn).Length;
                    }
                    else
                    {
                        throw new System.ArgumentException("string-length()'s argument count is invalid");
                    }
                //goto case FuncName.ROUND;


                case FuncName.ROUND: if (argCount() != 1)
                        throw new System.ArgumentException("round()'s argument count is invalid");
                    //UPGRADE_TODO: Method 'java.lang.Math.round' was converted to 'System.Math.Round' which has a different behavior. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1073_javalangMathround_double'"
                    return (long)System.Math.Round(argumentList.e.evalNumber(vn));

                case FuncName.ABS: if (argCount() != 1)
                        throw new System.ArgumentException("abs()'s argument count is invalid");
                    //UPGRADE_TODO: Method 'java.lang.Math.round' was converted to 'System.Math.Round' which has a different behavior. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1073_javalangMathround_double'"
                    return (long)System.Math.Abs(argumentList.e.evalNumber(vn));
                case FuncName.ROUND_HALF_TO_EVEN :
                    int numArg = argCount();
                    if (numArg < 1 || numArg > 2)
                    {
                        throw new System.ArgumentException("Argument count for roundHalfToEven() is invalid. Expected: 1 or 2; Actual: " + numArg);
                    }

                    double value = argumentList.e.evalNumber(vn);
                    int precision = (numArg == 2) ? (int)Math.Floor(argumentList.next.e.evalNumber(vn) + 0.5d) : 0;
                    return (long)System.Math.Round(value, precision, MidpointRounding.ToEven);

   			    case FuncName.ROUND_HALF_TO_ODD:
   			        throw new com.ximpleware.xpath.UnsupportedException("not yet implemented");

                default: if (isBoolean_Renamed_Field)
                    {
                        if (evalBoolean(vn))
                            return 1;
                        else
                            return 0;
                    }
                    else
                    {
                        try
                        {
                            double dval = System.Double.Parse(evalString(vn));
                            return dval;
                        }
                        catch (System.FormatException e)
                        {
                            return System.Double.NaN;
                        }
                        //return System.Double.Parse(evalString(vn));
                    }
                //break;

            }
        }

        public override int evalNodeSet(VTDNav vn)
        {
            throw new XPathEvalException(" Function Expr can't eval to node set ");
        }

        public override bool evalBoolean(VTDNav vn)
        {
            switch (opCode)
            {
                case FuncName.STARTS_WITH:
                    if (argCount() != 2)
                    {
                        throw new System.ArgumentException("starts-with()'s argument count is invalid");
                    }
                    return startsWith(vn);
                case FuncName.CONTAINS:
                    if (argCount() != 2)
                    {
                        throw new System.ArgumentException("contains()'s argument count is invalid");
                    }
                    return contains(vn);
                case FuncName.TRUE: if (argCount() != 0)
                    {
                        throw new System.ArgumentException("true() doesn't take any argument");
                    }
                    return true;

                case FuncName.FALSE: if (argCount() != 0)
                    {
                        throw new System.ArgumentException("false() doesn't take any argument");
                    }
                    return false;

                case FuncName.BOOLEAN: if (argCount() != 1)
                    {
                        throw new System.ArgumentException("boolean() doesn't take any argument");
                    }
                    return argumentList.e.evalBoolean(vn);

                case FuncName.NOT: if (argCount() != 1)
                    {
                        throw new System.ArgumentException("not() doesn't take any argument");
                    }
                    return !argumentList.e.evalBoolean(vn);
                case FuncName.LANG:
                    if (argCount() != 1)
                    {
                        throw new System.ArgumentException("lang()'s argument count is invalid");
                    }
                    return lang(vn, argumentList.e.evalString(vn));

                case FuncName.COMPARE: throw new com.ximpleware.xpath.UnsupportedException("not yet implemented");
                case FuncName.ENDS_WITH:
                    if (argCount() != 2)
                    {
                        throw new System.ArgumentException("ends-with()'s argument count is invalid");
                    }
                    return endsWith(vn);
          				
                default: if (Numerical)
                    {
                        double d = evalNumber(vn);
                        if (d == 0 || Double.IsNaN(d))
                            return false;
                        return true;
                    }
                    else
                    {
                        return evalString(vn).Length != 0;
                    }
                //break;

            }
        }

        public override void reset(VTDNav vn)
        {
            a = 0;
            //contextSize = 0;
            if (argumentList != null)
                argumentList.reset(vn);
        }

        public System.String fname()
        {
            switch (opCode)
            {

                case FuncName.LAST: return "last";

                case FuncName.POSITION: return "position";

                case FuncName.COUNT: return "count";

                case FuncName.LOCAL_NAME: return "local-name";

                case FuncName.NAMESPACE_URI: return "namespace-uri";

                case FuncName.NAME: return "name";

                case FuncName.STRING: return "string";

                case FuncName.CONCAT: return "concat";

                case FuncName.STARTS_WITH: return "starts-with";

                case FuncName.CONTAINS: return "contains";

                case FuncName.SUBSTRING_BEFORE: return "substring_before";

                case FuncName.SUBSTRING_AFTER: return "substring_after";

                case FuncName.SUBSTRING: return "substring";

                case FuncName.STRING_LENGTH: return "string-length";

                case FuncName.NORMALIZE_SPACE: return "normalize-space";

                case FuncName.TRANSLATE: return "translate";

                case FuncName.BOOLEAN: return "boolean";

                case FuncName.NOT: return "not";

                case FuncName.TRUE: return "true";

                case FuncName.FALSE: return "false";

                case FuncName.LANG: return "lang";

                case FuncName.NUMBER: return "number";

                case FuncName.SUM: return "sum";

                case FuncName.FLOOR: return "floor";

                case FuncName.CEILING: return "ceiling";

                case FuncName.ROUND: return "round";
                    // added for 2.0
                case FuncName.ABS: return "abs";
                case FuncName.ROUND_HALF_TO_EVEN:
                    return "round-half-to-even";
                case FuncName.ROUND_HALF_TO_ODD:
                    return "round-half-to-odd";
                case FuncName.CODE_POINTS_TO_STRING:
                    return "code-points-to-string";
                case FuncName.COMPARE: return "compare";
                case FuncName.UPPER_CASE: return "upper-case";
                case FuncName.LOWER_CASE: return "lower-case";
                case FuncName.ENDS_WITH: return "ends-with";
                case FuncName.QNAME: return "qname";
                case FuncName.LOCAL_NAME_FROM_QNAME:
                    return "local-name-from-QName";
                case FuncName.NAMESPACE_URI_FROM_QNAME:
                    return "namespace-uri-from-QName";
                case FuncName.NAMESPACE_URI_FOR_PREFIX:
                    return "namespace-uri-for-prefix";
                case FuncName.RESOLVE_QNAME: return "resolve-QName";
                case FuncName.IRI_TO_URI: return "iri-to-uri";
                case FuncName.ESCAPE_HTML_URI: return "escape-html-uri";
                default: return "encode-for-uri";

            }
        }


        private System.String subStringAfter(VTDNav vn)
        {
            if (argCount() == 2)
            {
                String s1 = argumentList.e.evalString(vn);
                String s2 = argumentList.next.e.evalString(vn);
                int len1 = s1.Length;
                int len2 = s2.Length;
                for (int i = 0; i < len1; i++)
                {
                    //System.String.Compare(s1,i,s2,0,len2);
                    if (System.String.Compare(s1, i, s2, 0, len2) == 0)
                        return s1.Substring(i + len2);
                }
                return "";
            }
            throw new System.ArgumentException
            ("substring-after()'s argument count is invalid");
        }

        private System.String subStringBefore(VTDNav vn)
        {
            if (argCount() == 2)
            {
                String s1 = argumentList.e.evalString(vn);
                String s2 = argumentList.next.e.evalString(vn);
                int len1 = s1.Length;
                int len2 = s2.Length;
                for (int i = 0; i < len1; i++)
                {
                    //System.String.Compare(s1,i,s2,0,len2);
                    if (System.String.Compare(s1, i, s2, 0, len2) == 0)
                        return s1.Substring(0, i);
                }
                return "";
            }
            throw new System.ArgumentException
            ("substring-after()'s argument count is invalid");
        }

        private System.String subString(VTDNav vn)
        {
            if (argCount() == 2)
            {
                System.String s = argumentList.e.evalString(vn);
                if (s != null)
                {
                    //UPGRADE_WARNING: Data types in Visual C# might be different.  Verify the accuracy of narrowing conversions. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1042'"
                    //s.Substring((int)argumentList.next.e.evalNumber(vn)+1);
                    double d1 = Math.Floor(argumentList.next.e.evalNumber(vn) + 0.5d);
                    if (d1 != d1 || d1 > s.Length)
                        return "";
                    return s.Substring(Math.Max((int)(d1 - 1), 0));
                }
                return null;
            }
            else if (argCount() == 3)
            {
                System.String s = argumentList.e.evalString(vn);
                if (s != null)
                {
                    // double d1 = argumentList.next.next.e.evalNumber(vn);
                    // double d2 = argumentList.next.e.evalNumber(vn);
                    double d1 = Math.Floor(argumentList.next.e.evalNumber(vn) + 0.5d);
                    double d2 = Math.Floor(argumentList.next.next.e.evalNumber(vn) + 0.5d);
                    int i = Math.Max(0, (int)d1 - 1);
                    if ((d1 + d2) != (d1 + d2) || d1 > s.Length)
                        return "";
                    return s.Substring(i, Math.Min(s.Length, (int)d2 - 1 + (int)d1) - i);
                    //UPGRADE_WARNING: Data types in Visual C# might be different.  Verify the accuracy of narrowing conversions. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1042'"
                    //s.Substring((int)d1-1, ((int)d1) - ((int)d2));
                }
                return null;
            }
            throw new System.ArgumentException("substring()'s argument count is invalid");
        }

        private System.String normalizeSpace(VTDNav vn)
        {
            if (argCount() == 0)
            {
                String s = null;
                try
                {
                    if (vn.atTerminal)
                    {
                        int ttype = vn.getTokenType(vn.LN);
                        if (ttype == VTDNav.TOKEN_CDATA_VAL)
                            s = vn.toRawString(vn.LN);
                        else if (ttype == VTDNav.TOKEN_ATTR_NAME ||
                             ttype == VTDNav.TOKEN_ATTR_NS)
                        {
                            s = vn.toString(vn.LN + 1);
                        }
                        else
                            s = vn.toString(vn.LN);
                    }
                    else
                        s = vn.toString(vn.getCurrentIndex());
                    return normalize(s);
                }
                catch (NavException e)
                {
                    return ""; // this will almost never occur
                }
            }
            else if (argCount() == 1)
            {
                String s = argumentList.e.evalString(vn);
                return normalize(s);
            }
            throw new System.ArgumentException("normalize-space()'s argument count is invalid");
            //return null;
        }
        private String normalize(String s)
        {
            int len = s.Length;
            StringBuilder sb = new StringBuilder(len);
            int i = 0;
            // strip off leading ws
            for (i = 0; i < len; i++)
            {
                if (isWS(s[i]))
                {
                }
                else
                {
                    break;
                }
            }
            while (i < len)
            {
                char c = s[i];
                if (!isWS(c))
                {
                    sb.Append(c);
                    i++;
                }
                else
                {
                    while (i < len)
                    {
                        c = s[i];
                        if (isWS(c))
                            i++;
                        else
                            break;
                    }
                    if (i < len)
                        sb.Append(' ');
                }
            }
            return sb.ToString();
        }

        private bool isWS(char c)
        {
            if (c == ' ' || c == '\t' || c == '\r' || c == '\n')
                return true;
            return false;
        }

        private System.String concat(VTDNav vn)
        {
            StringBuilder sb = new StringBuilder();
            if (argCount() >= 2)
            {
                Alist temp = argumentList;
                while (temp != null)
                {
                    sb.Append(temp.e.evalString(vn));
                    temp = temp.next;
                }
                return sb.ToString();
            }
            else
                throw new System.ArgumentException("concat()'s argument count is invalid");
        }

        private int count(VTDNav vn)
        {
            int a = -1;
            if (argCount() != 1 || argumentList.e.NodeSet == false)
                throw new System.ArgumentException("Count()'s argument count is invalid");
            vn.push2();
            try
            {
                a = 0;
                argumentList.e.adjust(vn.getTokenCount());
                while (argumentList.e.evalNodeSet(vn) != -1)
                {
                    a++;
                }
                argumentList.e.reset(vn);
                vn.pop2();
            }
            catch (System.Exception e)
            {
                argumentList.e.reset(vn);
                vn.pop2();
            }
            return a;
        }

        private double sum(VTDNav vn)
        {
            double d = 0;
            if (argCount() != 1 || argumentList.e.NodeSet == false)
                throw new System.ArgumentException("sum()'s argument count is invalid");
            vn.push2();
            try
            {
                a = 0;
                int i1;
                while ((a = argumentList.e.evalNodeSet(vn)) != -1)
                {
                    int t = vn.getTokenType(a);
                    if (t == VTDNav.TOKEN_STARTING_TAG)
                    {
                        i1 = vn.getText();
                        if (i1 != -1)
                            d = d + vn.parseDouble(i1);
                        if (System.Double.IsNaN(d))
                            break;
                    }
                    else if (t == VTDNav.TOKEN_ATTR_NAME || t == VTDNav.TOKEN_ATTR_NS)
                    {
                        d = d + vn.parseDouble(a + 1);
                        if (System.Double.IsNaN(d))
                            break;
                    }
                    else if (t == VTDNav.TOKEN_CHARACTER_DATA 
                        || t == VTDNav.TOKEN_CDATA_VAL
                        || t== VTDNav.TOKEN_COMMENT)
                    {
                        d = d + vn.parseDouble(a);
                        if (System.Double.IsNaN(d))
                            break;
                    }
                    else if (t == VTDNav.TOKEN_PI_NAME)
                    {
                        if (a + 1 < vn.vtdSize && vn.getTokenType(a + 1) == VTDNav.TOKEN_PI_VAL)
                        {
                            d += vn.parseDouble(a + 1);
                        }
                        else
                        {
                            d = Double.NaN;
                            break;
                        }
                    }
                    //    fib1.append(i);
                }
                argumentList.e.reset(vn);
                vn.pop2();
                return d;
            }
            catch (System.Exception e)
            {
                argumentList.e.reset(vn);
                vn.pop2();
                return System.Double.NaN;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="vn"></param>
        /// <param name="s"></param>
        /// <returns></returns>
        private bool lang(VTDNav vn, String s)
        {
            // check the length of s 
            bool b = false;
            vn.push2();
            try
            {
                while (vn.getCurrentDepth() >= 0)
                {
                    int i = vn.getAttrVal("xml:lang");
                    if (i != -1)
                    {
                        b = vn.matchTokenString(i, s);
                        break;
                    }
                    vn.toElement(VTDNav.P);
                }
            }
            catch (NavException e)
            {

            }
            vn.pop2();
            return b;
        }
        // to support computer context size 
        // needs to add 

        public override bool requireContextSize()
        {
            if (opCode == FuncName.LAST)
                return true;
            else
            {
                Alist temp = argumentList;
                //boolean b = false;
                while (temp != null)
                {
                    if (temp.e.requireContextSize())
                    {
                        return true;
                    }
                    temp = temp.next;
                }
            }
            return false;
        }

        private int evalFirstArgumentListNodeSet(VTDNav vn)
        {
            vn.push2();
            int size = vn.contextStack2.size;
            int a = -1;
            try
            {
                a = argumentList.e.evalNodeSet(vn);
                if (a != -1)
                {
                    if (vn.getTokenType(a) == VTDNav.TOKEN_ATTR_NAME)
                    {
                        a++;
                    }
                    if (vn.getTokenType(a) == VTDNav.TOKEN_STARTING_TAG)
                    {
                        a = vn.getText();
                    }
                }
            }
            catch (Exception e)
            {
            }
            vn.contextStack2.size = size;
            argumentList.e.reset(vn);
            vn.pop2();
            return a;
        }

        private int evalFirstArgumentListNodeSet2(VTDNav vn)
        {
            vn.push2();
            int size = vn.contextStack2.size;
            int a = -1;
            try
            {
                a = argumentList.e.evalNodeSet(vn);
            }
            catch (Exception e)
            {
            }
            vn.contextStack2.size = size;
            argumentList.e.reset(vn);
            vn.pop2();
            return a;
        }

        private bool endsWith(VTDNav vn)
        {
            String s2 = argumentList.next.e.evalString(vn);
            if (argumentList.e.NodeSet)
            {
                int a = evalFirstArgumentListNodeSet(vn);
                if (a == -1)
                    return "".EndsWith(s2);
                else
                {
                    try
                    {
                        return vn.endsWith(a, s2);
                    }
                    catch (Exception e)
                    {
                    }
                    return false;
                }
            }
            String s1 = argumentList.e.evalString(vn);
            return s1.EndsWith(s2);
        }


        private bool contains(VTDNav vn)
        {
            String s2 = argumentList.next.e.evalString(vn);
            if (argumentList.e.NodeSet)
            {
                int a = evalFirstArgumentListNodeSet(vn);
                if (a == -1)
                    return false;
                try
                {
                    return vn.contains(a, s2);
                }
                catch (Exception e)
                {
                    return false;
                }
            }
            String s1 = argumentList.e.evalString(vn);
            //return s1.contains(s2);
            return s1.IndexOf(s2) != -1;
            //return (s1.i))
        }

        private bool startsWith(VTDNav vn)
        {
            String s2 = argumentList.next.e.evalString(vn);
            if (argumentList.e.NodeSet)
            {
                //boolean b = false;
                int a = evalFirstArgumentListNodeSet(vn);
                if (a == -1)
                    return "".StartsWith(s2);
                else
                {
                    try
                    {
                        return vn.startsWith(a, s2);
                    }
                    catch (Exception e)
                    {
                    }
                    return false;
                }
            }
            String s1 = argumentList.e.evalString(vn);
            return s1.StartsWith(s2);
        }

        private String upperCase(VTDNav vn)
        {
            if (argCount() == 1)
            {
                if (argumentList.e.NodeSet)
                {
                    int a = evalFirstArgumentListNodeSet(vn);
                    if (a == -1)
                        return "";
                    else
                    {
                        try
                        {
                            return vn.toStringUpperCase(a);
                        }
                        catch (Exception e)
                        {
                        }
                        return "";
                    }
                }
                else
                {
                    return (argumentList.e.evalString(vn)).ToUpper();
                }
            }
            else
                throw new System.ArgumentException
                ("upperCase()'s argument count is invalid");

        }

        private String lowerCase(VTDNav vn)
        {
            if (argCount() == 1)
            {
                if (argumentList.e.NodeSet)
                {
                    int a = evalFirstArgumentListNodeSet(vn);
                    if (a == -1)
                        return "";
                    else
                    {
                        try
                        {
                            return vn.toStringLowerCase(a);
                        }
                        catch (Exception e)
                        {
                        }
                        return "";
                    }
                }
                else
                {
                    return (argumentList.e.evalString(vn)).ToLower();
                }
            }
            else
                throw new System.ArgumentException
                ("lowerCase()'s argument count is invalid");
        }
    }
}
