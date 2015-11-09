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
namespace com.ximpleware.xpath
{
	/// <summary> 
	/// This class is used by Yylex to detect axis type
	/// and transport to the parser
	/// 
	/// </summary>
	public class AxisType
	{
		
		public int i;
		public const int CHILD0 = 0;
        public const int CHILD = 1;
        public const int DESCENDANT_OR_SELF0 = 2;
        public const int DESCENDANT0 = 3;
        public const int PRECEDING0 = 4;
        public const int FOLLOWING0 = 5;
        public const int DESCENDANT_OR_SELF = 6;
        public const int DESCENDANT = 7;

        public const int PRECEDING = 8;
        public const int FOLLOWING = 9;

        public const int PARENT = 10;
        public const int ANCESTOR = 11;
		
		public const int ANCESTOR_OR_SELF = 12;
		public const int SELF = 13;
		
		
		
		
		public const int FOLLOWING_SIBLING = 14;
        public const int FOLLOWING_SIBLING0 = 15;
		
		public const int PRECEDING_SIBLING = 16;
        public const int PRECEDING_SIBLING0 = 17;

        public const int ATTRIBUTE = 18;
        public const int NAMESPACE = 19;
		
		public AxisType()
		{
		}

        public String getAxisString()
        {
            switch (i)
            {
                case CHILD0:
                case CHILD: return "child::";
                case DESCENDANT_OR_SELF0: return "descendant-or-self::";
                case DESCENDANT0: return "descendent::";
                case PRECEDING0: return "preceding::";
                case FOLLOWING0: return "following::";
                case DESCENDANT_OR_SELF: return "descendant-or-self::";
                case DESCENDANT: return "descendent::";
                case PRECEDING: return "preceding::";
                case FOLLOWING: return "following::";
                case PARENT: return "parent::";
                case ANCESTOR: return "ancestor::";
                case ANCESTOR_OR_SELF: return "ancestor-or-self::";
                case SELF: return "self::";
                case FOLLOWING_SIBLING: return "following-sibling::";
                case FOLLOWING_SIBLING0: return "following-sibling::";
                case PRECEDING_SIBLING: return "preceding-sibling::";
                case PRECEDING_SIBLING0: return "preceding-sibling::";
                case ATTRIBUTE: return "attribute::";
                default: return "namespace::";		
            }
        }

	}
}