/* 
 * Copyright (C) 2002-2006 XimpleWare, info@ximpleware.com
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
package com.ximpleware;

/**
 *
 * This is the implementation of a hash table for integers
 * It is used by various XPath expressions to check for uniqueness
 * of a node
 */
class intHash {
    private int mask1 = 0x7ff;
     //2048
    private int mask2 = 0xfffff800;
    private int pageSizeE = 5; // 32 * 4 bytes
    protected FastIntBuffer[] storage;
    private int hashWidth = 1<<11;
    private int maxDepth;
    /**
     * Test whether the input i is unique; 
     * if not, insert into the hash table and return false
     * otherwise, return true
     */
    public boolean isUnique(int i){
        int temp = i & mask1;
        if (temp>maxDepth){
            maxDepth = temp;
        }
        if (storage[temp]==null) {
            storage[temp]= new FastIntBuffer(pageSizeE);
            storage[temp].append(i);
            return true;
        }        
        else{
            int size = storage[temp].size();
            for (int j=0;j<size;j++){
                if (i == storage[temp].intAt(j)){
                    return false;
                }
            }
            storage[temp].append(i);
            return true;            
        }
    }

    /**
     * Clear all entries in the hash table
     * This method sets the size of member FastIntBuffer
     * to zero
     */
    public void reset(){
        for (int i=0;i<=maxDepth;i++){
            if (storage[i]!=null){
                storage[i].clear();
            }
        }
    }
    /**
     * Constructor
     *
     */
    public intHash(){
        storage = new FastIntBuffer[hashWidth];
    }
    
    public intHash(int hashWidthExpo){
        hashWidth = 1<<hashWidthExpo;
        mask1 = (hashWidth) -1;
        mask2 = (~mask1) & 0xffffffff;    
        storage = new FastIntBuffer[hashWidth];
    }
    public static void main(String[] args) {
        intHash a = new intHash(6);
        for(int i=0;i<667;i++)
           System.out.println("" + i + " " + a.isUnique(i));
        for(int i=0;i<667;i++)
        System.out.println("" + i + " " + a.isUnique(i));
    }
}
