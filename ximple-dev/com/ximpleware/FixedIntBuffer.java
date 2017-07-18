
/* 
 * Copyright (C) 2002-2017 XimpleWare, info@ximpleware.com
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
/*All licenses to any parties in litigation with XimpleWare have been expressly terminated. No new license, and no renewal of any revoked license, 
 * is granted to those parties as a result of re-downloading software from this or any other website*/

package com.ximpleware;

public class FixedIntBuffer extends FastIntBuffer {
	protected int[] storage;
	//protected int size;
	public FixedIntBuffer(int cap){
		storage = new int[cap];
		size =0;
	}
	public void append(int i){
		storage[size]=i;
		size++;
	}
	public void append(int[] ia){
		
	}
	
	public int[] getIntArray(int startingOffset, int len){
		int[] ia = new int[len];
		System.arraycopy(storage, startingOffset, ia, 0, len);
		return ia;
	}
	
	public int[] toIntArray(){
		return storage.clone();
	}
	
	public int getCapacity(){
		return storage.length;
	}
	
	public int intAt(int index){
		return storage[index];
	}
	
	public void modifyEntry(int index, int i){
		storage[index] = i;
	}
}
