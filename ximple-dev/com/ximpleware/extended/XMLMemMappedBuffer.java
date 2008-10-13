/*
 * Created on Oct 8, 2008
 *
 * TODO To change the template for this generated file go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
package com.ximpleware.extended;
import java.io.*;
import java.nio.*;
import java.nio.channels.*;
/**
 * @author jimmy zhang
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
/**
 * @author jimmy zhang
 *
 * TODO To change the template for this generated type comment go to
 * Window - Preferences - Java - Code Style - Code Templates
 */
public class XMLMemMappedBuffer implements IByteBuffer {
    MappedByteBuffer input[];
    long length;
    public XMLMemMappedBuffer(){
        
    }
    
    public long length(){
        return length;
    }
    
    public byte byteAt(long index){
        return input[(int)(index>>30)].get((int)(index & 0x3fffffff));
    }
    
    public void readFile(String fileName) throws java.io.IOException, ParseExceptionHuge {
        File f = new File(fileName);
        long l = f.length();
        length = l;
        if (l>= (1L<< 38)){
            throw new ParseExceptionHuge("document too big > 256 Gbyte");
        }
        RandomAccessFile raf = new RandomAccessFile(fileName, "r");
        FileChannel fc  = raf.getChannel();
        int pageNumber = (int)(l>>30)+(((l & 0x3fffffffL)==0)?0:1);
                
        input = new MappedByteBuffer[pageNumber];
        long l2 =0;
        for (int i=0;i<pageNumber;i++){
            if (l > (1<<30)){
                //bufferArray[i] = new byte[1<<30];
                input[i]= fc.map(FileChannel.MapMode.READ_ONLY, l2 ,1<<30);
                l2 = l2+(1<<30);
            }
            else{
                //bufferArray[i] = new byte[(int)l];
                input[i]= fc.map(FileChannel.MapMode.READ_ONLY, l2, l - (i<<30));
            }
            //input[i] = new RandomAccessFile(fileName, "r").getChannel()
            //.map(FileChannel.MapMode.READ_ONLY, 0,(1<<32)-1);
        }
        if (fc!=null)
        fc.close();
        if (raf!=null)
        raf.close();
    }
    
	/**
	 * NOt implemented yet
	 */
    public byte[] getBytes(){
	    return null;
	}
	
	/**
	 * not implemented yet
	 */
    public byte[] getBytes(int offset, int len){
	    return (byte[]) null;
	}    
}
