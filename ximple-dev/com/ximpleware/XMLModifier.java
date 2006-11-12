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
import java.io.*;

/**
 * XMLModifier offers an easy-to-use interface for users to
 * take advantage of the incremental update information
 * The XML modifier assumes there is a master document on which
 * the modification is applied: users can remove an element, update
 * a token, or insert new content anywhere in the document
 *
 */
public class XMLModifier {
    protected VTDNav md; // master document
   
    
    public static final int XML_DELETE = 0;
    public static final int XML_INSERT_BYTE = 1;
    public static final int XML_INSERT_SEGMENT_BYTE = 2;
    public static final int XML_INSERT_STRING = 3;
    public static final int XML_INSERT_SEGMENT_STRING = 4;

    public static final int DELETE_LIMIT = 0x1ffffff;
    private static final long MASK_DELETE = 0x00000000000000000L; //0000
    private static final long MASK_INSERT_SEGMENT_BYTE = 0x2000000000000000L; //0010
    private static final long MASK_INSERT_BYTE = 0x4000000000000000L;//0100
    private static final long MASK_INSERT_SEGMENT_STRING = 0x6000000000000000L; //0110
    private static final long MASK_INSERT_STRING = 0x8000000000000000L; //1000
    
    protected FastObjectBuffer fob;
    protected FastLongBuffer flb;
    protected intHash deleteHash; // one deletion per offset val
    protected intHash insertHash;   // one insert per offset val
    protected String charSet;
    /**
     * Constructor for XMLModifier that takes VTDNav object as the master document
     * @param masterDocument is the document on which the modification is applied
     */
    public XMLModifier(VTDNav masterDocument) throws ModifyException{
        bind(masterDocument);
    }
    

    /**
     * Argument-less constructor for XMLModifier,
     * needs to call bind to attach the master document to an instance
     * of XMLModifier
     *
     */
    public XMLModifier(){
        md = null;
    }
    /**
     * Attach master document to this instance of XMLModifier
     * @param masterDocument
     *
     */
    public void bind(VTDNav masterDocument) throws ModifyException{
        if (masterDocument == null)
            throw new IllegalArgumentException("MasterDocument can't be null");
        md = masterDocument;
        flb = new FastLongBuffer();
        fob = new FastObjectBuffer();
        int i = intHash.determineHashWidth(md.vtdSize);
        insertHash = new intHash(i);
        deleteHash = new intHash(i);    
        //determine encoding charset string here
        int encoding = md.getEncoding();
        switch(encoding){
        	case VTDNav.FORMAT_ASCII:
        	    charSet = "ASCII";
        	    break;
            case VTDNav.FORMAT_ISO_8859_1:
                charSet = "ISO8859_1";
            	break;
            case VTDNav.FORMAT_UTF8:
                charSet = "UTF8";
            	break;
            case VTDNav.FORMAT_UTF_16BE:
                charSet = "UnicodeBigUnmarked";
            	break;
            case VTDNav.FORMAT_UTF_16LE:
                charSet = "UnicodeLittleUnmarked";
            	break;
            default:
                throw new ModifyException
                ("Master document encoding not yet supported by XML modifier");
        }
    }
    /**
     * Removes content from the master XML document 
     * It first calls getCurrentIndex() if the result is 
     * a starting tag, then the entire element referred to
     * by the starting tag is removed
     * If the result is an attribute name or ns node, then 
     * the corresponding attribute name/value pair is removed
     * If the token type is one of text, CDATA or commment,
     * then the entire node, including the starting and ending 
     * delimiting text surrounding the content, is removed
     *
     */
    public void remove() throws NavException,ModifyException{
        if (md == null)
            throw new IllegalArgumentException("MasterDocument can't be null");
        int i = md.getCurrentIndex();
        int type = md.getTokenType(i);
        if (type==VTDNav.TOKEN_STARTING_TAG){
            long l = md.getElementFragment();
            removeContent((int)l, (int)(l>>32));            
        } else if (type == VTDNav.TOKEN_ATTR_NAME 
                || type==VTDNav.TOKEN_ATTR_NS){
            removeAttribute(i);
        } else {
            removeToken(i);
        }
    }
    
    /**
     * Remove the token content, if the token type is text, CDATA
     * or comment, then the entire node, including the starting and 
     * ending delimiting text, will be removed as well
     * @param i
     *
     */
    public void removeToken(int i) throws ModifyException{
        if (md == null)
            throw new IllegalArgumentException("MasterDocument can't be null");
       
        int type = md.getTokenType(i);
        int os,len;
        switch(type){
        	case VTDNav.TOKEN_CDATA_VAL:
        	    os = md.getTokenOffset(i);
        		len = md.getTokenLength(i);
        		if (md.getEncoding() < VTDNav.FORMAT_UTF_16BE)
        		    removeContent(os - 9, len + 12 );
        		else
        		    removeContent((os - 9)<<1,(len+12)<<1);
        		return;
        		 
        	case VTDNav.TOKEN_COMMENT:
           	    
        	    os = md.getTokenOffset(i);            
        		len = md.getTokenLength(i);
           	    if (md.getEncoding() < VTDNav.FORMAT_UTF_16BE)
           	        removeContent(os-4, len+7);
           	    else
           	        removeContent((os-4) << 1, (len+7) << 1);
           	    return;
        		
        	default:
        	    os = md.getTokenOffset(i);            
    			len = md.getTokenLength(i);
    			if (md.getEncoding() < VTDNav.FORMAT_UTF_16BE)
        	        removeContent(os, len);
        	    else
        	        removeContent((os) << 1, (len) << 1);
        	    return;        	    
        }
    }
    /**
     * remove an attribute name value pair from the master document
     * @param attrNameIndex
     *
     */
    public void removeAttribute(int attrNameIndex) throws ModifyException{
        if (md == null)
            throw new IllegalArgumentException("MasterDocument can't be null");
        int type = md.getTokenType(attrNameIndex);
        if (type != VTDNav.TOKEN_ATTR_NAME&& type != VTDNav.TOKEN_ATTR_NS)
            throw new ModifyException("token type should be attribute name");
        int os1 = md.getTokenOffset(attrNameIndex);
        int os2 = md.getTokenOffset(attrNameIndex+1);
        int len2 = md.getTokenLength(attrNameIndex+1);
   	    if (md.getEncoding() < VTDNav.FORMAT_UTF_16BE)
   	        removeContent(os1,os2+len2-os1+1); 
	    else 
	        removeContent(os1<<1,(os2+len2-os1+1)<<1); 
		    
    }
    
    /**
     * 
     * @param offset
     * @param len
     *
     */
    private void removeContent(int offset, int len) throws ModifyException{
        if (md == null)
            throw new IllegalArgumentException("MasterDocument can't be null");
        if (offset < md.docOffset || len > md.docLen 
                || offset + len > md.docOffset + md.docLen){
            throw new ModifyException("Invalid offset or length for removeContent");
        }
        if (deleteHash.isUnique(offset)==false)
            throw new ModifyException("There can be only one insertion per offset value");
            
        flb.append(((long)len)<<32 | offset | MASK_DELETE);
        fob.append((Object)null);
    }
    
    /**
     * 
     * @param offset
     * @param content
     *
     */
    private void insertBytesAt(int offset, byte[] content) throws ModifyException{
        if (md == null)
            throw new IllegalArgumentException("MasterDocument can't be null");
        if (insertHash.isUnique(offset)==false){
            throw new ModifyException("There can be only one insert per offset");
        }
        flb.append( (long)offset | MASK_INSERT_BYTE);
        fob.append(content);
    }
    /**
     * Update the token with the given string value,
     * notice that string will be converted into byte array
     * according to the encoding of the master document
     * @param offset
     * @param newContent
     *
     */
    public void updateToken(int index, String newContent) 
    	throws ModifyException,UnsupportedEncodingException{
        if (md == null || newContent==null)
            throw new IllegalArgumentException
            ("MasterDocument or newContent can't be null");
        int offset = md.getTokenOffset(index);
        int len = md.getTokenLength(index);
        int type = md.getTokenType(index);
        // one insert
        switch(type){
        	case VTDNav.TOKEN_CDATA_VAL:
        	    if (md.getEncoding() < VTDNav.FORMAT_UTF_16BE)
        	        insertBytesAt(offset-9,newContent.getBytes());
        	    else 
        	        insertBytesAt((offset-9)<<1,newContent.getBytes());
        		break;
        	case VTDNav.TOKEN_COMMENT:
           	    if (md.getEncoding() < VTDNav.FORMAT_UTF_16BE)
        	        insertBytesAt(offset-4,newContent.getBytes(charSet));
        	    else 
        	        insertBytesAt((offset-4)<<1,newContent.getBytes(charSet));
        		break;
        	    
        	default: 
        	    insertBytesAt(offset,newContent.getBytes());
        }
        // one delete
        removeToken(index);        	
    }
    
    /**
     * 
     * 
     *
     */
    protected void sort(){
        quickSort(0,flb.size()-1);
        
    }
    
    /**
     * 
     * This function will do the range checking and make
     * sure there is no overlaping or invalid deletion 
     * There can be only one deletion at one offset value
     * Delete can't overlap with, nor contains, another delete
     *
     */
    protected void check()  throws ModifyException{
        int os1, os2, temp;
        int size = flb.size();
        
        for (int i=0;i<size;i++){
            os1 = flb.lower32At(i);
            os2 = flb.lower32At(i)+ (flb.upper32At(i)& 0x1fffffff)-1;
            if (i+1<size){
                temp = flb.lower32At(i+1);
                if (temp!= os1 && temp<=os2)
                    throw new ModifyException
                    ("Invalid insertion/deletion condition detected between offset "
                            +os1 + " and offset "+os2);
            }
        }
    
        
    }
    /**
     * This method will first call getCurrentIndex() to get the cursor index value
     * then insert the byte value of s before the element
     * @param startTagIndex
     * @param s
     *
     */
    public void insertAfterElement(String s)
    	throws ModifyException,UnsupportedEncodingException,NavException{
        int startTagIndex =md.getCurrentIndex();
        int type = md.getTokenType(startTagIndex);
        if (type!=VTDNav.TOKEN_STARTING_TAG)
            throw new ModifyException("Token type is not a starting tag");
        long l = md.getElementFragment();
        int offset = (int)l;
        int len = (int)(l>>32);
        insertBytesAt(offset+len,s.getBytes(charSet));
        
    }
    /**
     * This method will first call getCurrentIndex() to get the cursor index value
     * then insert the byte value of s before the element
     * @param startTagIndex
     * @param s
     *
     */
    public void insertBeforeElement(String s)
    	throws ModifyException,UnsupportedEncodingException{
        int startTagIndex =md.getCurrentIndex();
        int type = md.getTokenType(startTagIndex);
        if (type!=VTDNav.TOKEN_STARTING_TAG)
            throw new ModifyException("Token type is not a starting tag");
        
        int offset = md.getTokenOffset(startTagIndex)-1;
        int encoding = md.getTokenType(startTagIndex);
        if (encoding < VTDNav.FORMAT_UTF_16BE)
            insertBytesAt(offset,s.getBytes(charSet));
        else
            insertBytesAt((offset)<<1,s.getBytes(charSet));
        
    }
    
    /**
     * Insert an attribute after the starting tag
     * This method will first call getCurrentIndex() to get the cursor index value
     * if the index is of type "starting tag", then teh attribute is inserted
     * after the starting tag
     * @param attr e.g. " attrName='attrVal' ",notice the starting and ending 
     * white space
     *
     */
    public void insertAttribute(String attr) 
    	throws ModifyException,UnsupportedEncodingException{
        int startTagIndex =md.getCurrentIndex();
        int type = md.getTokenType(startTagIndex);
        if (type!=VTDNav.TOKEN_STARTING_TAG)
            throw new ModifyException("Token type is not a starting tag");
        int offset = md.getTokenOffset(startTagIndex);
        int len = md.getTokenLength(startTagIndex);
        int encoding = md.getTokenType(startTagIndex);
        
        if (encoding < VTDNav.FORMAT_UTF_16BE)
            insertBytesAt(offset+len,attr.getBytes(charSet));
        else
            insertBytesAt((offset+len)<<1,attr.getBytes(charSet));
        //insertBytesAt()
    }
    /**
     * This method applys the modification to the XML document
     * and generate output byte content accordingly
     * Notice that output is not guaranteed to be well-formed 
     * @param os
     *
     */
    public void output(OutputStream os) throws IOException, ModifyException{
        if (md == null)
            throw new IllegalArgumentException("MasterDocument can't be null");
        sort();
        check();
        long l;
        byte[] ba = md.getXML().getBytes();
        for(int i=0;i<flb.size();i++){
            System.out.println(" offset value is ==>"+flb.lower32At(i));
        }
        if (flb.size()==0){
            os.write(ba,md.docOffset,md.docLen);
        }else{
            int offset = md.docOffset;
            int inc=1;
            for(int i=0;i<flb.size();i=i+inc){
                if (flb.lower32At(i)==flb.lower32At(i+1)){
                    inc  = 2;
                } else 
                    inc = 1;
                l = flb.longAt(i);
                if (inc == 1){                    
                    if ((l & (~0x1fffffffffffffffL)) == XML_DELETE){
                        os.write(ba,offset, flb.lower32At(i)-offset);
                        offset = flb.lower32At(i) + (flb.upper32At(i) & 0x1fffffff);
                    }else { // insert
                        os.write(ba,offset, flb.lower32At(i)-offset);
                        os.write((byte[])fob.objectAt(i));                       
                        offset=flb.lower32At(i);
                    }
                } else {
                    if ((l & (~0x1fffffffffffffffL)) == XML_DELETE){
                        os.write(ba,offset, flb.lower32At(i)-offset);
                        os.write((byte[])fob.objectAt(i+1));
                        offset = flb.lower32At(i) + (flb.upper32At(i) & 0x1fffffff);
                    }else{
                        os.write(ba,offset, flb.lower32At(i+1)-offset);
                        os.write((byte[])fob.objectAt(i));
                        offset = flb.lower32At(i+1) + (flb.upper32At(i+1) & 0x1fffffff);
                    }                    
                }
            }  
            os.write(ba,offset,md.docOffset+md.docLen-offset);
        }
 
    }    
    
    void quickSort (int lo, int hi)
    {
//      lo is the lower index, hi is the upper index
//      of the region of array a that is to be sorted
        //System.out.println("lo ==>"+lo);
        //System.out.println("hi ==>"+hi);
        int i=lo, j=hi; 
        long h;
        Object o;
        int x=flb.lower32At((lo+hi)/2);

        //  partition
        do
        {    
            while (flb.lower32At(i)<x) i++; 
            while (flb.lower32At(j)>x) j--;
            if (i<=j)
            {
                h=flb.longAt(i); 
                o = fob.objectAt(i);
                flb.modifyEntry(i,flb.longAt(j)); 
                fob.modifyEntry(i,fob.objectAt(j));
                flb.modifyEntry(j,h);
                fob.modifyEntry(j,o);
                i++; 
                j--;
            }
        } while (i<=j);

        //  recursion
        if (lo<j) quickSort(lo, j);
        if (i<hi) quickSort(i, hi);
    }

    
    public void reset(){
        if (flb!=null)
            flb.clear();
        if (fob!=null)
            fob.clear();
        if (insertHash!=null)
            insertHash.reset();
        if (deleteHash!=null)
            deleteHash.reset();
    }

}
