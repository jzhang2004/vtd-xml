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
	/// <summary> 
	/// This class is used by Yylex to detect axis type
	/// and transport to the parser
	/// 
	/// </summary>
	public class AxisType
	{
		
		public int i;
		public const int CHILD = 0;
		public const int ANCESTOR = 3;
		public const int ANCESTOR_OR_SELF = 12;
		public const int ATTRIBUTE = 8;
		
		public const int DESCENDANT = 1;
		public const int DESCENDANT_OR_SELF = 11;
		public const int FOLLOWING = 6;
		public const int FOLLOWING_SIBLING = 4;
		public const int NAMESPACE = 9;
		public const int PARENT = 2;
		public const int PRECEDING = 7;
		public const int PRECEDING_SIBLING = 5;
		public const int SELF = 10;
		
		public AxisType()
		{
		}

        public String getAxisString()
        {
            switch (i)
            {
                case 0: return "child::";
                case 1: return "descendent::";
                case 2: return "parent::";
                case 3: return "ancestor::";
                case 4: return "following-sibling::";
                case 5: return "preceding-sibling::";
                case 6: return "following::";
                case 7: return "preceding::";
                case 8: return "attribute::";
                case 9: return "namespace::";
                case 10: return "self::";
                case 11: return "descendant-or-self::";
                default: return "ancestor-or-self::";
            }
        }

	}
}