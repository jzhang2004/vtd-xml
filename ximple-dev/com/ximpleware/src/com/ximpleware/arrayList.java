package com.ximpleware;

class arrayList {
	public final static int GROWTH_INC = 16;
	public final static int default_init_capacity = 16;
	public arrayList(){
		oa = new Object[default_init_capacity];
		capacity = default_init_capacity;
	}
	
	public arrayList(int initial_capacity){
		oa = new Object[initial_capacity];
		capacity = initial_capacity;
	}
	
	final public Object get(int i){
		if(i>=size)
		    throw new IndexOutOfBoundsException();
		return oa[i];
	}
	
	final public void add(Object o){
		if (size<capacity){
			oa[size]=o;
			size++;
		}else{
			Object[] oa1 = new Object[capacity+GROWTH_INC];
			System.arraycopy(oa, 0, oa1, 0, capacity);
			oa = oa1;
			capacity = capacity+GROWTH_INC;
			oa[size] = o;
			size++;
		}
	}
	
	final public void clear(){
		size = 0;
	}
	
	final public int size(){
		return size;
	}

	final public int getCapacity(){
		return capacity;
	}
	protected int size;
	protected int capacity;
	protected Object[] oa;
	
}
