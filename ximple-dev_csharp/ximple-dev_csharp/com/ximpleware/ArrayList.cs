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
using System.Text;

namespace com.ximpleware
{
    class ArrayList
    {
        	public const int GROWTH_INC = 16;
	public const int default_init_capacity = 16;
	public ArrayList(){
		oa = new Object[default_init_capacity];
		capacity = default_init_capacity;
	}
	
	public ArrayList(int initial_capacity){
		oa = new Object[initial_capacity];
		capacity = initial_capacity;
	}
	
	public Object Get(int i){
		if(i>=size)
		    throw new System.IndexOutOfRangeException();
		return oa[i];
	}
	
	public void Add(Object o){
		if (size<capacity){
			oa[size]=o;
			size++;
		}else{
			Object[] oa1 = new Object[capacity+GROWTH_INC];
			Array.Copy(oa, 0, oa1, 0, capacity);
			oa = oa1;
			capacity = capacity+GROWTH_INC;
			oa[size] = o;
			size++;
		}
	}
	
	public void Clear(){
		size = 0;
	}
	
	public int Size(){
		return size;
	}

	public int GetCapacity(){
		return capacity;
	}
	protected internal int size;
	protected internal int capacity;
	protected internal Object[] oa;
    }
}
