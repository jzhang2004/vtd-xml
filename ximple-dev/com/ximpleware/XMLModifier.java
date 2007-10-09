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
package com.ximpleware;
import java.io.*;

/**
 * XMLModifier offers an easy-to-use interface for users to
 * take advantage of the incremental update of VTD-XML
 * The XML modifier assumes there is a master document on which
 * the modification is applied: users can remove an element, update
 * a token, or insert new content anywhere in the document
 * 
 * The process:
 * * The modification operations are recorded first
 * * The output() is called to generate output document
 *
 */
public class XMLModifier {
    protected VTDNav md; // master document
    
    private static final long MASK_DELETE = 0x00000000000000000L; //0000
    private static final long MASK_INSERT_SEGMENT_BYTE = 0x2000000000000000L; //0010
    private static final long MASK_INSERT_BYTE = 0x4000000000000000L;//0100
    private static final long MASK_INSERT_SEGMENT_STRING = 0x6000000000000000L; //0110
    private static final long MASK_INSERT_STRING = 0x8000000000000000L; //1000
    private static final long MASK_INSERT_FRAGMENT_NS = 0xa000000000000000L; //1010
    
    protected FastObjectBuffer fob;
    protected FastLongBuffer flb;
    protected intHash deleteHash; // one deletion per offset val
    protected intHash insertHash;   // one insert per offset val
    protected String charSet;
    int encoding;
    
    public class ByteSegment{
        byte[] ba;
        int offset;
        int len;
    }
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
     * so all the operations occuring aftewards are based on this instance
     * of VTDNav
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
        encoding = md.getEncoding();
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
     * Remove a byte segment from XML.
     * l's upper 32 bits is length in # of bytes
     * l's lower 32 bits is byte offset 
     * @param l
     * @throws NavException
     * @throws ModifyException
     *
     */
    public void remove(long l) throws NavException,ModifyException{
        removeContent((int)l, (int)(l>>32));
    }
    
    /**
     * Remove the token content. If the token type is text, CDATA
     * or comment, then the entire node, including the starting and 
     * ending delimiting text, will be removed as well
     * @param i the index for the content
     *
     */
    public void removeToken(int i) throws ModifyException{        
        int type = md.getTokenType(i);
        int os = md.getTokenOffset(i);
		int len = md.getTokenLength(i);
        switch(type){
        	case VTDNav.TOKEN_CDATA_VAL:        	   
        		if (encoding < VTDNav.FORMAT_UTF_16BE)
        		    removeContent(os - 9, len + 12 );
        		else
        		    removeContent((os - 9)<<1,(len+12)<<1);
        		return;
        		 
        	case VTDNav.TOKEN_COMMENT:
           	    if (encoding < VTDNav.FORMAT_UTF_16BE)
           	        removeContent(os-4, len+7);
           	    else
           	        removeContent((os-4) << 1, (len+7) << 1);
           	    return;
        		
        	default:
    			if (encoding < VTDNav.FORMAT_UTF_16BE)
        	        removeContent(os, len);
        	    else
        	        removeContent((os) << 1, (len) << 1);
        	    return;        	    
        }
    }
    /**
     * Remove an attribute name value pair from the master document.
     * @param attrNameIndex
     *
     */
    public void removeAttribute(int attrNameIndex) throws ModifyException{
         int type = md.getTokenType(attrNameIndex);
        if (type != VTDNav.TOKEN_ATTR_NAME&& type != VTDNav.TOKEN_ATTR_NS)
            throw new ModifyException("token type should be attribute name");
        int os1 = md.getTokenOffset(attrNameIndex);
        int os2 = md.getTokenOffset(attrNameIndex+1);
        int len2 = md.getTokenLength(attrNameIndex+1);
   	    if (encoding < VTDNav.FORMAT_UTF_16BE)
   	        removeContent(os1,os2+len2-os1+1); 
	    else 
	        removeContent(os1<<1,(os2+len2-os1+1)<<1); 
		    
    }
    
    /**
     * Remove a segment of byte content from master XML doc.
     * The segment is denoted by its offset and len. 
     * @param offset
     * @param len
     *
     */
    public void removeContent(int offset, int len) throws ModifyException{

        if (offset < md.docOffset || len > md.docLen 
                || offset + len > md.docOffset + md.docLen){
            throw new ModifyException("Invalid offset or length for removeContent");
        }
        if (deleteHash.isUnique(offset)==false)
            throw new ModifyException("There can be only one deletion per offset value");
            
        flb.append(((long)len)<<32 | offset | MASK_DELETE);
        fob.append((Object)null);
    }
    
    /**
     * insert the byte content into XML
     * @param offset
     * @param content
     *
     */
    public void insertBytesAt(int offset, byte[] content) throws ModifyException{

        if (insertHash.isUnique(offset)==false){
            throw new ModifyException("There can be only one insert per offset");
        }
        flb.append( (long)offset | MASK_INSERT_BYTE);
        fob.append(content);
    }
    /**
     * Insert ns compensated element fragment into the document
     * @param ef
     *
     */
    private void insertElementFragmentNsAt(int offset, ElementFragmentNs ef) throws ModifyException{
        if (insertHash.isUnique(offset)==false){
            throw new ModifyException("There can be only one insert per offset");
        }
        flb.append( (long)offset | MASK_INSERT_FRAGMENT_NS);
        fob.append(ef);
    }
    
    /**
     * Insert a segment of the byte content into XML
     * @param offset
     * @param content
     * @param contentOffset
     * @param contentLen
     * @throws ModifyException
     *
     */
    public  void insertBytesAt(int offset, byte[] content, int contentOffset, int contentLen) 
    throws ModifyException {
        if (insertHash.isUnique(offset)==false){
            throw new ModifyException("There can be only one insert per offset");
        }
        if (contentOffset < 0 
                || contentLen <0 
                || contentOffset+contentLen >= content.length){
            throw new ModifyException("Invalid contentOffset and/or contentLen");
        }
        flb.append( (long)offset | MASK_INSERT_SEGMENT_BYTE);
        ByteSegment bs = new ByteSegment();
        bs.ba = content;
        bs.len = contentLen;
        bs.offset = contentOffset;
       
        fob.append(bs);
    }
    /**
     * Insert a segment of content into XML
     * l (a long)'s upper 32 bit is length, lower 32 bit is offset
     * @param offset
     * @param content
     * @param l
     * @throws ModifyException
     *
     */
    private void insertBytesAt(int offset, byte[] content, long l)
    throws ModifyException {
        if (insertHash.isUnique(offset)==false){
            throw new ModifyException("There can be only one insert per offset");
        }
        int contentOffset = (int)l;
        int contentLen = (int)(l>>32); 
        if (contentOffset < 0 
                || contentLen <0 
                || contentOffset+contentLen >= content.length){
            throw new ModifyException("Invalid contentOffset and/or contentLen");
        }
        flb.append( (long)offset | MASK_INSERT_SEGMENT_BYTE);
        ByteSegment bs = new ByteSegment();
        bs.ba = content;
        bs.len = contentLen;
        bs.offset = contentOffset;
        fob.append(bs);
    }
   
   /**
    * Update the token with the given byte array content,
    * @param index
    * @param newContentBytes
    * @throws ModifyException
    * @throws UnsupportedEncodingException
    *
    */
    public void updateToken(int index, byte[] newContentBytes) 
    	throws ModifyException,UnsupportedEncodingException{
        if (newContentBytes==null)
            throw new IllegalArgumentException
            ("newContentBytes can't be null");
        int offset = md.getTokenOffset(index);
        int len = md.getTokenLength(index);
        int type = md.getTokenType(index);
        // one insert
        switch(type){
        	case VTDNav.TOKEN_CDATA_VAL:
        	    if (encoding < VTDNav.FORMAT_UTF_16BE)
        	        insertBytesAt(offset-9,newContentBytes);
        	    else 
        	        insertBytesAt((offset-9)<<1,newContentBytes);
        		break;
        	case VTDNav.TOKEN_COMMENT:
           	    if (encoding < VTDNav.FORMAT_UTF_16BE)
        	        insertBytesAt(offset-4,newContentBytes);
        	    else 
        	        insertBytesAt((offset-4)<<1,newContentBytes);
        		break;
        	    
        	default: 
        	    insertBytesAt(offset,newContentBytes);
        }
        // one delete
        removeToken(index);        	
    }
    
    /**
    * Update the token with the given string value,
    * notice that string will be converted into byte array
    * according to the encoding of the master document
    * @param index
    * @param newContent
    * @throws ModifyException
    * @throws UnsupportedEncodingException
    *
    */
    
    public void updateToken(int index, String newContent) 
    	throws ModifyException,UnsupportedEncodingException{
        if (newContent==null)
            throw new IllegalArgumentException
            ("String newContent can't be null");
        int offset = md.getTokenOffset(index);
        int len = md.getTokenLength(index);
        int type = md.getTokenType(index);
        // one insert
        switch(type){
        	case VTDNav.TOKEN_CDATA_VAL:
        	    if (encoding < VTDNav.FORMAT_UTF_16BE)
        	        insertBytesAt(offset-9,newContent.getBytes(charSet));
        	    else 
        	        insertBytesAt((offset-9)<<1,newContent.getBytes(charSet));
        		break;
        	case VTDNav.TOKEN_COMMENT:
           	    if (encoding < VTDNav.FORMAT_UTF_16BE)
        	        insertBytesAt(offset-4,newContent.getBytes(charSet));
        	    else 
        	        insertBytesAt((offset-4)<<1,newContent.getBytes(charSet));
        		break;
        	    
        	default: 
        	    insertBytesAt(offset,newContent.getBytes(charSet));
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
        if (flb.size()>0)
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
     * Compute the size of the updated XML document
     * @return
     *
     */
    public int getUpdatedDocumentSize(){
        int size = flb.size();
        int docSize = md.getXML().getBytes().length;
        long l;
        for (int i=0;i<size;i++){
            l= flb.longAt(i);
            if ((l & (~0x1fffffffffffffffL)) == MASK_DELETE) {
                docSize -= (int) ((l & (0x1fffffffffffffffL))>> 32);
            } else if ((l & (~0x1fffffffffffffffL)) == MASK_INSERT_BYTE){
                docSize += ((byte[])fob.objectAt(i)).length;
            } else { // MASK_INSERT_SEGMENT_BYTE
                docSize += ((ByteSegment)fob.objectAt(i)).len;
            }
        }
        return docSize;
    }
    /**
     * This method will first call getCurrentIndex() to get the cursor index value
     * then insert the byte array b after the element
     * @param b  the byte array to be inserted into the master document
     *
     */
    public void insertAfterElement(byte[] b)
		throws ModifyException,UnsupportedEncodingException,NavException{
        int startTagIndex =md.getCurrentIndex();
        int type = md.getTokenType(startTagIndex);
        if (type!=VTDNav.TOKEN_STARTING_TAG)
            throw new ModifyException("Token type is not a starting tag");
        long l = md.getElementFragment();
        int offset = (int)l;
        int len = (int)(l>>32);
        insertBytesAt(offset+len,b);
    }
    /**
     * Insert a namespace compensated element after cursor element
     * @param ef (an ElementFragmentNs object)
     * @throws ModifyException
     * @throws NavException
     *
     */
    public void insertAfterElement(ElementFragmentNs ef)
            throws ModifyException, NavException {
        int startTagIndex = md.getCurrentIndex();
        int type = md.getTokenType(startTagIndex);
        if (type != VTDNav.TOKEN_STARTING_TAG)
            throw new ModifyException("Token type is not a starting tag");
        long l = md.getElementFragment();
        int offset = (int) l;
        int len = (int) (l >> 32);
        insertElementFragmentNsAt(offset + len, ef);
    }
    
    /**
     * This method will first call getCurrentIndex() to get the cursor index value
     * then insert a segment of the byte array b after the element
     * @param b
     * @param contentOffset
     * @param contentLen
     * @throws ModifyException
     * @throws UnsupportedEncodingException
     * @throws NavException
     *
     */
    public void insertAfterElement(byte[] b, int contentOffset, int contentLen)
            throws ModifyException, UnsupportedEncodingException, NavException {
        int startTagIndex = md.getCurrentIndex();
        int type = md.getTokenType(startTagIndex);
        if (type != VTDNav.TOKEN_STARTING_TAG)
            throw new ModifyException("Token type is not a starting tag");
        long l = md.getElementFragment();
        int offset = (int) l;
        int len = (int) (l >> 32);
        insertBytesAt(offset + len, b, contentOffset, contentLen);
    }
    /**
     * This method will first call getCurrentIndex() to get the cursor index value
     * then insert a segment of the byte array b after the element,
     * l1 (a long)'s upper 32 bit is length, lower 32 bit is offset
     * @param b
     * @param contentOffset
     * @param contentLen
     * @throws ModifyException
     * @throws UnsupportedEncodingException
     * @throws NavException
     *
     */
    public void insertAfterElement(byte[] b, long l1) throws ModifyException,
            UnsupportedEncodingException, NavException {
        int startTagIndex = md.getCurrentIndex();
        int type = md.getTokenType(startTagIndex);
        if (type != VTDNav.TOKEN_STARTING_TAG)
            throw new ModifyException("Token type is not a starting tag");
        long l = md.getElementFragment();
        int offset = (int) l;
        int len = (int) (l >> 32);
        insertBytesAt(offset + len, b, l1);
    }
    /**
     * This method will first call getCurrentIndex() to get the cursor index value
     * then insert the byte value of s after the element
     * @param s  the string whose byte content will be inserted into the master document
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
     * then insert the byte array b before the element
     * @param b the byte array to be inserted into the master document
     * @throws ModifyException
     *
     */
    public void insertBeforeElement(byte[] b)
    	throws ModifyException{
        int startTagIndex =md.getCurrentIndex();
        int type = md.getTokenType(startTagIndex);
        if (type!=VTDNav.TOKEN_STARTING_TAG)
            throw new ModifyException("Token type is not a starting tag");
        
        int offset = md.getTokenOffset(startTagIndex)-1;
        
        if (encoding < VTDNav.FORMAT_UTF_16BE)
            insertBytesAt(offset,b);
        else
            insertBytesAt((offset)<<1,b);        
    }
    
   /**
    * Insert a namespace compensated fragment before the cursor element
    * @param ef
    * @throws ModifyException
    * 
    *
    */
    public void insertBeforeElement(ElementFragmentNs ef)
    	throws ModifyException{
        int startTagIndex =md.getCurrentIndex();
        int type = md.getTokenType(startTagIndex);
        if (type!=VTDNav.TOKEN_STARTING_TAG)
            throw new ModifyException("Token type is not a starting tag");
        
        int offset = md.getTokenOffset(startTagIndex)-1;
        
        if (encoding < VTDNav.FORMAT_UTF_16BE)
            insertElementFragmentNsAt(offset,ef);
        else
            insertElementFragmentNsAt((offset)<<1,ef);        
    }
    
    
    /**
     * This method will first call getCurrentIndex() to get the cursor index value
     * then insert a segment of the byte array b before the element
     * @param b
     * @param contentOffset
     * @param contentLen
     * @throws ModifyException
     * @throws UnsupportedEncodingException
     *
     */
    public void insertBeforeElement(byte[] b,int contentOffset, int contentLen) throws ModifyException,
            UnsupportedEncodingException {
        int startTagIndex = md.getCurrentIndex();
        int type = md.getTokenType(startTagIndex);
        if (type != VTDNav.TOKEN_STARTING_TAG)
            throw new ModifyException("Token type is not a starting tag");

        int offset = md.getTokenOffset(startTagIndex) - 1;

        if (encoding < VTDNav.FORMAT_UTF_16BE)
            insertBytesAt(offset, b, contentOffset, contentLen);
        else
            insertBytesAt((offset) << 1, b, contentOffset, contentLen);
    }
    /**
     * This method will first call getCurrentIndex() to get the cursor index value
     * then insert a segment of the byte array b before the element
     * l1 (a long)'s upper 32 bit is length, lower 32 bit is offset
     * @param b
     * @param l1
     * @throws ModifyException
     * @throws UnsupportedEncodingException
     *
     */
    public void insertBeforeElement(byte[] b, long l1) throws ModifyException,
            UnsupportedEncodingException {
        int startTagIndex = md.getCurrentIndex();
        int type = md.getTokenType(startTagIndex);
        if (type != VTDNav.TOKEN_STARTING_TAG)
            throw new ModifyException("Token type is not a starting tag");

        int offset = md.getTokenOffset(startTagIndex) - 1;

        if (encoding < VTDNav.FORMAT_UTF_16BE)
            insertBytesAt(offset, b, l1);
        else
            insertBytesAt((offset) << 1, b, l1);
    }
    /**
     * This method will first call getCurrentIndex() to get the cursor index value
     * then insert the byte value of s before the element
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
        
        if (encoding < VTDNav.FORMAT_UTF_16BE)
            insertBytesAt(offset+len,attr.getBytes(charSet));
        else
            insertBytesAt((offset+len)<<1,attr.getBytes(charSet));
        //insertBytesAt()
    }
    
    /**
     * Insert a byte arry of an attribute after the starting tag
     * This method will first call getCurrentIndex() to get the cursor index value
     * if the index is of type "starting tag", then teh attribute is inserted
     * after the starting tag
     * @param b the byte content of e.g. " attrName='attrVal' ",notice the starting and ending 
     * white space
     *
     */
    public void insertAttribute(byte[] b) 
    	throws ModifyException,UnsupportedEncodingException{
        int startTagIndex =md.getCurrentIndex();
        int type = md.getTokenType(startTagIndex);
        if (type!=VTDNav.TOKEN_STARTING_TAG)
            throw new ModifyException("Token type is not a starting tag");
        int offset = md.getTokenOffset(startTagIndex);
        int len = md.getTokenLength(startTagIndex);
        
        if (encoding < VTDNav.FORMAT_UTF_16BE)
            insertBytesAt(offset+len,b);
        else
            insertBytesAt((offset+len)<<1,b);
        //insertBytesAt()
    }
    /**
     * This method applies the modification to the XML document
     * and write the output byte content accordingly to an outputStream
     * Notice that output is not guaranteed to be well-formed 
     * @param os
     *
     */
    public void output(OutputStream os) throws IOException, ModifyException{
        if (os == null)
            throw new IllegalArgumentException("OutputStream can't be null");
        sort();
        check();
        long l;
        byte[] ba = md.getXML().getBytes();
        //for(int i=0;i<flb.size();i++){
        //    System.out.println(" offset value is ==>"+flb.lower32At(i));
        //}
        int t = md.vtdBuffer.lower32At(0);
        int start = (t==0)?
                md.docOffset:32;
        int len = (t==0)?
                md.docLen:(md.docLen-32);
                    
        if (flb.size()==0){
            os.write(ba,start,len);
        }else{
            int offset = start;
            int inc=1;
            for(int i=0;i<flb.size();i=i+inc){
                if (flb.lower32At(i)==flb.lower32At(i+1)){
                    inc  = 2;
                } else 
                    inc = 1;
                l = flb.longAt(i);
                if (inc == 1){                    
                    if ((l & (~0x1fffffffffffffffL)) == MASK_DELETE){
                        os.write(ba,offset, flb.lower32At(i)-offset);
                        offset = flb.lower32At(i) + (flb.upper32At(i) & 0x1fffffff);
                    }else if ((l & (~0x1fffffffffffffffL)) == MASK_INSERT_BYTE ) { // insert
                        os.write(ba,offset, flb.lower32At(i)-offset);
                        os.write((byte[])fob.objectAt(i));                       
                        offset=flb.lower32At(i);
                    } else if ((l & (~0x1fffffffffffffffL)) == MASK_INSERT_SEGMENT_BYTE) { 
                        // XML_INSERT_SEGMENT_BYTE
                        os.write(ba,offset, flb.lower32At(i)-offset);
                        ByteSegment bs = (ByteSegment) fob.objectAt(i);
                        os.write(bs.ba,bs.offset,bs.len);
                        offset=flb.lower32At(i);
                    } else {
                        //ElementFragmentNs
                        os.write(ba,offset, flb.lower32At(i)-offset);
                        ElementFragmentNs ef = (ElementFragmentNs)fob.objectAt(i);
                        ef.writeToOutputStream(os);
                        offset=flb.lower32At(i);
                    }
                } else {
                    if ((l & (~0x1fffffffffffffffL)) == MASK_DELETE){
                        os.write(ba,offset, flb.lower32At(i)-offset);
                        os.write((byte[])fob.objectAt(i+1));
                        offset = flb.lower32At(i) + (flb.upper32At(i) & 0x1fffffff);
                    }else if ((l & (~0x1fffffffffffffffL)) == MASK_INSERT_BYTE ){ // insert
                        os.write(ba,offset, flb.lower32At(i+1)-offset);
                        os.write((byte[])fob.objectAt(i));
                        offset = flb.lower32At(i+1) + (flb.upper32At(i+1) & 0x1fffffff);
                    } else if ((l & (~0x1fffffffffffffffL)) == MASK_INSERT_SEGMENT_BYTE){
                        // XML_INSERT_SEGMENT_BYTE
                        os.write(ba,offset, flb.lower32At(i+1)-offset);
                        ByteSegment bs = (ByteSegment) fob.objectAt(i);
                        os.write(bs.ba,bs.offset,bs.len);
                        offset = flb.lower32At(i+1) + (flb.upper32At(i+1) & 0x1fffffff);
                    } else {
                        //ElementFragmentNs
                        os.write(ba,offset, flb.lower32At(i+1)-offset);
                        ElementFragmentNs ef = (ElementFragmentNs)fob.objectAt(i);
                        ef.writeToOutputStream(os);
                        offset = flb.lower32At(i+1) + (flb.upper32At(i+1) & 0x1fffffff);
                    }
                }
            }  
            os.write(ba,offset,start+len-offset);
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

    /**
     * This method reset the internal state of XMLModify instance so 
     * it can be reused
     * 
     *
     */
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
