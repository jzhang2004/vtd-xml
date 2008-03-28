/* 
* Copyright (C) 2002-2008 XimpleWare, info@ximpleware.com
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
	/// This class is used by lexer to detect built-in functions
	/// then transport to the parser 
	/// </summary>
	public class FuncName
	{
		public int i;
		public const int LAST = 0;
		public const int POSITION = 1;
		public const int COUNT = 2;
		public const int LOCAL_NAME = 3;
		public const int NAMESPACE_URI = 4;
		public const int NAME = 5;
		public const int STRING = 6;
		public const int CONCAT = 7;
		public const int STARTS_WITH = 8;
		public const int CONTAINS = 9;
		public const int SUBSTRING_BEFORE = 10;
		public const int SUBSTRING_AFTER = 11;
		public const int SUBSTRING = 12;
		public const int STRING_LENGTH = 13;
		public const int NORMALIZE_SPACE = 14;
		public const int TRANSLATE = 15;
		public const int BOOLEAN = 16;
		public const int NOT = 17;
		public const int TRUE = 18;
		public const int FALSE = 19;
		public const int LANG = 20;
		public const int NUMBER = 21;
		public const int SUM = 22;
		public const int FLOOR = 23;
		public const int CEILING = 24;
		public const int ROUND = 25;
	}
}