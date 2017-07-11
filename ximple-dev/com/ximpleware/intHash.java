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

/**
 *
 * This is the implementation of a hash table for integers
 * It is used by various XPath expressions to check for uniqueness
 * of a node. It has been modified to improve memory usage for large XML files.
 */
class intHash {
	private int mask1 = 0x7ff;
    //2048
	private int mask2 = 0xfffff800;
	private int pageSizeE = 3; // 32 * 4 bytes
	protected Object[] storage;
	private int hashWidth = 1<<11;
	private int maxDepth;
	protected int e;
	/**
     * Constructor
     *
     */
    public intHash(){
        hashWidth = 1<<0;
        mask1 = (hashWidth) -1;
        mask2 = (~mask1) & 0xffffffff;    
        storage = new FastIntBuffer[hashWidth];
        e=0;
    }
	 public intHash(int hashWidthExpo){
	        e=hashWidthExpo;
	        hashWidth = 1<<hashWidthExpo;
	        mask1 = (hashWidth) -1;
	        mask2 = (~mask1) & 0xffffffff;    
	        storage = new Object[hashWidth];
	    }
	 
	    public boolean isUnique(int i){
	        int temp = i & mask1;
	        if (temp>maxDepth){
	            maxDepth = temp;
	        }
	        if (storage[temp]==null) {
	            int[] ia= new int[1<<pageSizeE];
	            ia[0]=1;
	            ia[1]=i;
	            storage[temp]=ia;
	            return true;
	        }        
	        else{
	        	int[] ia = (int [])storage[temp];
	            int size = ia[0];
	            for (int j=1;j<=size;j++){
	                if (i == ia[j]){
	                    return false;
	                }
	            }
	            if (size < ia.length-1){
	               ia[0]++;
	               ia[size+1]=i;
	            }else{
	            	int[] ia_new= new int[ia.length+8];
	            	System.arraycopy(ia, 0, ia_new, 0, ia.length);
	            	ia_new[0]++;
	            	ia_new[ia_new[0]]=i;
	            	storage[temp] = ia_new;
	            }
	            return true;            
	        }
	    }
	    
	    /**
	     * This function differs from isUnique(int i) in that it doesn't insert i into
	     * intHash if it is unique, use to implement intersection or difference of nodesets
	     * @param i
	     * @return
	     */
	    public boolean _isUnique(int i){
	        int temp = i & mask1;
	        if (temp>maxDepth){
	            maxDepth = temp;
	        }
	        if (storage[temp]==null) {
	            //storage[temp]= new FastIntBuffer(pageSizeE);
	            //storage[temp].append(i);
	            return true;
	        }        
	        else{
	            int size = ((int [])storage[temp])[0];
	            for (int j=1;j<=size;j++){
	                if (i == ((int []) storage[temp])[j]){
	                    return false;
	                }
	            }
	            //storage[temp].append(i);
	            return true;            
	        }
	    }

	    
	    final public int totalSize(){
	    	int total = 0;
	    	for (int i=0;i<storage.length;i++){
	    		if (storage[i]!=null){
	    			int[] ia= (int [])storage[i];
	    			total+= ia[0];
	    		}
	    	}
	    	return total;
	    }
	 final public void reset(){
	        for (int i=0;i<=maxDepth;i++){
	            if (storage[i]!=null){
	                ((int [])storage[i])[0]=0;
	            }
	        }
	    }
	   public static int determineHashWidth(int i){  
	    	
	    	// can we do better than this?
	        if (i<(1<<8))
	            return 3;
	    	if (i<(1<<9))
	    		return 4;
	    	if (i<(1<<10))
	    		return 5;
	    	if (i<(1<<11))
	    		return 6;
	    	if (i<(1<<12))
	    		return 7;
	    	if (i<(1<<13))
	    		return 8;
	    	if (i<(1<<14))
	    		return 9;
	    	if (i<(1<<15))
	    		return 10;
	    	if (i<(1<<16))
	    		return 11;
	    	if (i<(1<<17))
	    	    return 12;
	       	if (i<(1<<18))
	    	    return 13;
	       	if (i<(1<<19))
	    	    return 14;
	       	if (i<(1<<20))
	    	    return 15;
	       	if (i<(1<<21))
	    	    return 16;
	       	if (i<(1<<22))
	    	    return 17;
	       	if (i<(1<<23))
	    	    return 18;
	       	if (i<(1<<24))
	    	    return 19;
	       	if (i<(1<<25))
	       	    return 20;
	       	if (i<(1<<26))
	       	    return 21;
	    	if (i<(1<<27))
	       	    return 22;
	    	if (i<(1<<28))
	       	    return 23;
	       	return 24;
	    }
}
