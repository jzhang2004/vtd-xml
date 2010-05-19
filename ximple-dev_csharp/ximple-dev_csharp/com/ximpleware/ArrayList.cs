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
