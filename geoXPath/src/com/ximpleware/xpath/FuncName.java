/* 
 * Copyright (C) 2002-2007 XimpleWare, info@ximpleware.com
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
package com.ximpleware.xpath;
/**
 * 
 * This class is used by lexer to detect built-in functions
 * then transport to the parser 
 */
public class FuncName{
	public int i;
	public static final int 	LAST = 0;
	public static final int		POSITION =1;
	public static final int		COUNT = 2;
	public static final int		LOCAL_NAME =3;
	public static final int		NAMESPACE_URI =4;
	public static final int		NAME=5;
	public static final int		STRING=6;
	public static final int		CONCAT = 7;
	public static final int		STARTS_WITH = 8;
	public static final int		CONTAINS = 9;
	public static final int		SUBSTRING_BEFORE = 10;
	public static final int		SUBSTRING_AFTER =11;
	public static final int		SUBSTRING = 12;
	public static final int		STRING_LENGTH =13;
	public static final int		NORMALIZE_SPACE =14;
	public static final int		TRANSLATE =15;
	public static final int		BOOLEAN =16;
	public static final int		NOT = 17;
	public static final int		TRUE =18;
	public static final int		FALSE =19;
	public static final int		LANG = 20;
	public static final int		NUMBER = 21;
	public static final int		SUM = 22;
	public static final int		FLOOR =23;
	public static final int		CEILING = 24;
	public static final int		ROUND = 25;

    //added by yanj at 2007-07-08
    public static final int GP_GEOMETRY_TYPE = 26;
    public static final int GP_ENVELOPE = 27;
    public static final int GP_BOUNDARY = 28;
    public static final int GP_EQUALS = 29;
    public static final int GP_DISJOINT = 30;
    public static final int GP_INTERSECTS = 31;
    public static final int GP_TOUCHES = 32;
    public static final int GP_CROSSES = 33;
    public static final int GP_WITHIN = 34;
    public static final int GP_CONTAINS = 35;
    public static final int GP_OVERLAPS = 36;
    public static final int GP_DISTANCE = 37;
    public static final int GP_BUFFER = 38;
    public static final int GP_CONVEX_HULL = 39;
    public static final int GP_INTERSECTION = 40;
    public static final int GP_UNION = 41;
    public static final int GP_DIFFERENCE = 42;
    public static final int GP_SYM_DIFFERENCE = 43;
    //added end
}
