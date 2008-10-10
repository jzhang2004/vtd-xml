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
package com.ximpleware.extended;
import java.io.*;
/**
 * XMLBuffer is a class that reads in an XML doc into chunk-based memory buffers
 * the file size is equal to n*1GB + m (where m < 1GB)
 * 
 */

public class XMLBuffer implements IByteBuffer {
    byte[][] bufferArray;
    long length;
    public XMLBuffer(){
        //ArrayList al = new ArrayList();
    }
    
    /**
     * 
     * @param fileName
     * @throws java.io.IOException
     * @throws ParseException
     *
     */
    public void readFile(String fileName) throws java.io.IOException, ParseException {
        // get file size
        File f = new File(fileName);
        long l = f.length();
        length = l;
        if (l>= (1L<< 38)){
            throw new ParseException("document too big > 256 Gbyte");
        }
        // calculate # of buffers needed and each buffer size
        int pageNumber = (int)(l>>30)+(((l & 0x3fffffffL)==0)?0:1);
        
        bufferArray = new byte[pageNumber][];
        
        //fill the buffers with doc content
        FileInputStream fis = new FileInputStream(f);
        
        for (int i=0;i<pageNumber;i++){
            if (l > (1<<30)){
                bufferArray[i] = new byte[1<<30];       
            }
            else{
                bufferArray[i] = new byte[(int)l];
            }
            fis.read(bufferArray[i]);
            l = l -(1<<30);
        }        
    }

    /**
     * 
     */
	public final byte byteAt(long index){
	    return bufferArray[(int)(index>>30)][(int)(index & 0x3fffffff)];
	}
	
	/**
	 * Return a byte array filled with content from underlying byte storage.
	 * @return byte[]
	 * @param offset int  bytes offset (not UTF char unit)
	 * @param len int
	 */
	public byte[] getBytes(int offset, int len){
	    return (byte[]) null;
	}
	/**
	 * Total size in terms of # of bytes.
	 * @return int
	 */
	public long length(){
	    return length;
	}
	// get the whole XML 
	public byte[] getBytes(){
	    return null;
	}
}
