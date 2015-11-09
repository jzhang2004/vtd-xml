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
using LocationPathNode = com.ximpleware.xpath.LocationPathNode;
namespace com.ximpleware
{
	/// <summary> This class is used within LocationPathExpr to represent 
	/// Nodetest
	/// 
	/// </summary>
	public class NodeTest : LocationPathNode
	{
		public int TestType
		{
			/*public void setNsEnabled(boolean b){
			nsEnabled = b;
			}*/
			
			set
			{
				testType = value;
			}
			
		}
		public System.String NodeName
		{
			set
			{
                nodeName = value;
                if (value.Equals("*"))
                    type = 0;
                else
                    type = 1;
			}
			
		}
		public System.String nodeName;
		public System.String prefix;
		public System.String localName;
		public System.String URL;
		internal bool nsEnabled;
		public int testType;
        public int type;

		public const int NAMETEST = 0;
		public const int NODE = 1;
		public const int TEXT = 2;
		public const int PI0 = 3;
		public const int PI1 = 4;
		public const int COMMENT = 5;
		public NodeTest()
		{
			nsEnabled = false;
			localName = null;
		}
		public void  setNodeNameNS(System.String p, System.String ln)
		{
            prefix = p;
            localName = ln;
            type = 2;
		}
		public  bool eval(VTDNav vn)
		{
            if (vn.atTerminal)
                return false;
            switch (type)
            {
                case 0: return true;
                case 1: return vn.matchElement(nodeName);
                case 2: return vn.matchElementNS(URL, localName);
            }
            //}
            return false;
		}

        public bool eval2(VTDNav vn) {
            switch (testType)
            {
                case NAMETEST:
                    if (vn.atTerminal)
                        return false;
                    switch (type)
                    {
                        case 0: return true;
                        case 1: return vn.matchElement(nodeName);
                        case 2: return vn.matchElementNS(URL, localName);
                    }
                    return false;
                case NODE:
                    return true;
                case TEXT:
                    if (!vn.atTerminal)
                        return false;
                    int t = vn.getTokenType(vn.LN);
                    if (t == VTDNav.TOKEN_CHARACTER_DATA
                            || t == VTDNav.TOKEN_CDATA_VAL)
                    {
                        return true;
                    }
                    return false;

                case PI0:
                    if (!vn.atTerminal)
                        return false;
                    if (vn.getTokenType(vn.LN) == VTDNav.TOKEN_PI_NAME)
                    {
                        return true;
                    }
                    return false;
                case PI1:
                    if (!vn.atTerminal)
                        return false;
                    if (vn.getTokenType(vn.LN) == VTDNav.TOKEN_PI_NAME)
                    {
                        return vn.matchTokenString(vn.LN, nodeName);
                    }
                    return false;

                default: // comment
                    if (!vn.atTerminal)
                        return false;
                    if (vn.getTokenType(vn.LN) == VTDNav.TOKEN_COMMENT)
                    {
                        return true;
                    }
                    return false;
            }
        
        }
		public override System.String ToString()
		{
			switch (testType)
			{
				
				case NAMETEST: 
					if (localName == null)
						return nodeName;
					else
						return prefix + ":" + localName;
					//goto case NODE;
				
				case NODE:  return "node()";
				
				case TEXT:  return "text()";

                case PI0: return "processing-instruction()";
                case PI1: return (nodeName.IndexOf('"') > 0) ?
                                    "processing-instruction('" + nodeName + "')"
                                  : "processing-instruction(\"" + nodeName + "\")";
				
				default:  return "comment()";
				
			}
		}
	}
}