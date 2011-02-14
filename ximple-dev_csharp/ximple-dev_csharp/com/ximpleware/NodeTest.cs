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
			}
			
		}
		public System.String nodeName;
		public System.String prefix;
		public System.String localName;
		public System.String URL;
		internal bool nsEnabled;
		public int testType;
		
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
		}
		public  bool eval(VTDNav vn)
		{
			if (testType == NODE)
				return true;
			else if (testType == NAMETEST)
			{
				if (vn.atTerminal == true)
					return false;
				if (localName != null)
					return vn.matchElementNS(URL, localName);
				else
					return vn.matchElement(nodeName);
			}
			return false;
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