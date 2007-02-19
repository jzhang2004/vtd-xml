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
using System;
namespace com.ximpleware
{
	
	/// <summary> XMLModifier offers an easy-to-use interface for users to
	/// take advantage of the incremental update information
	/// The XML modifier assumes there is a master document on which
	/// the modification is applied: users can remove an element, update
	/// a token, or insert new content anywhere in the document
	/// 
	/// </summary>
	public class XMLModifier
	{
		protected internal VTDNav md; // master document
		
		
		public const int XML_DELETE = 0;
		public const int XML_INSERT_BYTE = 1;
		public const int XML_INSERT_SEGMENT_BYTE = 2;
		public const int XML_INSERT_STRING = 3;
		public const int XML_INSERT_SEGMENT_STRING = 4;
		
		public const int DELETE_LIMIT = 0x1ffffff;
		private const long MASK_DELETE = 0x00000000000000000L; //0000
		private const long MASK_INSERT_SEGMENT_BYTE = 0x2000000000000000L; //0010
		private const long MASK_INSERT_BYTE = 0x4000000000000000L; //0100
		private const long MASK_INSERT_SEGMENT_STRING = 0x6000000000000000L; //0110
		//UPGRADE_TODO: Literal detected as an unsigned long can generate compilation errors. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1175'"
		private const ulong MASK_INSERT_STRING = 0x8000000000000000L; //1000
		
		protected internal FastObjectBuffer fob;
		protected internal FastLongBuffer flb;
		internal intHash deleteHash; // one deletion per offset val
		internal intHash insertHash; // one insert per offset val
		protected internal System.String charSet;
        protected internal System.Text.Encoding eg;
		/// <summary> Constructor for XMLModifier that takes VTDNav object as the master document</summary>
		/// <param name="masterDocument">is the document on which the modification is applied
		/// </param>
		public XMLModifier(VTDNav masterDocument)
		{
			bind(masterDocument);
		}
		
		
		/// <summary> Argument-less constructor for XMLModifier,
		/// needs to call bind to attach the master document to an instance
		/// of XMLModifier
		/// 
		/// </summary>
		public XMLModifier()
		{
			md = null;
		}
		/// <summary> Attach master document to this instance of XMLModifier</summary>
		/// <param name="masterDocument">*
		/// </param>
		public void  bind(VTDNav masterDocument)
		{
			if (masterDocument == null)
				throw new System.ArgumentException("MasterDocument can't be null");
			md = masterDocument;
			flb = new FastLongBuffer();
			fob = new FastObjectBuffer();
			int i = intHash.determineHashWidth(md.vtdSize);
			insertHash = new intHash(i);
			deleteHash = new intHash(i);
			//determine encoding charset string here
			int encoding = md.getEncoding();
			switch (encoding)
			{
				
				case VTDNav.FORMAT_ASCII: 
					charSet = "ascii";
					break;
				
				case VTDNav.FORMAT_ISO_8859_1: 
					charSet = "iso-8859-1";
					break;
				
				case VTDNav.FORMAT_UTF8: 
					charSet = "utf-8";
					break;
				
				case VTDNav.FORMAT_UTF_16BE: 
					charSet = "utf-16be";
					break;
				
				case VTDNav.FORMAT_UTF_16LE: 
					charSet = "utf-16le";
					break;
				
				default: 
					throw new ModifyException("Master document encoding not yet supported by XML modifier");
				
			}
            eg = System.Text.Encoding.GetEncoding(charSet);
		}
		/// <summary> Removes content from the master XML document 
		/// It first calls getCurrentIndex() if the result is 
		/// a starting tag, then the entire element referred to
		/// by the starting tag is removed
		/// If the result is an attribute name or ns node, then 
		/// the corresponding attribute name/value pair is removed
		/// If the token type is one of text, CDATA or commment,
		/// then the entire node, including the starting and ending 
		/// delimiting text surrounding the content, is removed
		/// 
		/// </summary>
		public void  remove()
		{
			int i = md.getCurrentIndex();
			int type = md.getTokenType(i);
			if (type == VTDNav.TOKEN_STARTING_TAG)
			{
				long l = md.getElementFragment();
				removeContent((int) l, (int) (l >> 32));
			}
			else if (type == VTDNav.TOKEN_ATTR_NAME || type == VTDNav.TOKEN_ATTR_NS)
			{
				removeAttribute(i);
			}
			else
			{
				removeToken(i);
			}
		}
		
		/// <summary> Remove the token content, if the token type is text, CDATA
		/// or comment, then the entire node, including the starting and 
		/// ending delimiting text, will be removed as well
		/// </summary>
		/// <param name="i">*
		/// </param>
		public void  removeToken(int i)
		{		
			int type = md.getTokenType(i);
            int os = md.getTokenOffset(i);
            int len = md.getTokenLength(i);
			switch (type)
			{
				
				case VTDNav.TOKEN_CDATA_VAL:					
					if (md.getEncoding() < VTDNav.FORMAT_UTF_16BE)
						removeContent(os - 9, len + 12);
					else
						removeContent((os - 9) << 1, (len + 12) << 1);
					return ;
				
				
				case VTDNav.TOKEN_COMMENT:					
					if (md.getEncoding() < VTDNav.FORMAT_UTF_16BE)
						removeContent(os - 4, len + 7);
					else
						removeContent((os - 4) << 1, (len + 7) << 1);
					return ;
				
				
				default: 
                    if (md.getEncoding() < VTDNav.FORMAT_UTF_16BE)
						removeContent(os, len);
					else
						removeContent((os) << 1, (len) << 1);
					return ;
				
			}
		}
		/// <summary> remove an attribute name value pair from the master document</summary>
		/// <param name="attrNameIndex">*
		/// </param>
		public void  removeAttribute(int attrNameIndex)
		{
			int type = md.getTokenType(attrNameIndex);
			if (type != VTDNav.TOKEN_ATTR_NAME && type != VTDNav.TOKEN_ATTR_NS)
				throw new ModifyException("token type should be attribute name");
			int os1 = md.getTokenOffset(attrNameIndex);
			int os2 = md.getTokenOffset(attrNameIndex + 1);
			int len2 = md.getTokenLength(attrNameIndex + 1);
			if (md.getEncoding() < VTDNav.FORMAT_UTF_16BE)
				removeContent(os1, os2 + len2 - os1 + 1);
			else
				removeContent(os1 << 1, (os2 + len2 - os1 + 1) << 1);
		}
		
		/// <summary> </summary>
		/// <param name="offset">
		/// </param>
		/// <param name="len">*
		/// </param>
		private void  removeContent(int offset, int len)
		{
			if (offset < md.docOffset || len > md.docLen || offset + len > md.docOffset + md.docLen)
			{
				throw new ModifyException("Invalid offset or length for removeContent");
			}
			if (deleteHash.isUnique(offset) == false)
				throw new ModifyException("There can be only one deletion per offset value");
			
			flb.append(((long) len) << 32 | offset | MASK_DELETE);
			fob.append((System.Object) null);
		}
		
		/// <summary> </summary>
		/// <param name="offset">
		/// </param>
		/// <param name="content">*
		/// </param>
		private void  insertBytesAt(int offset, byte[] content)
		{
			
			if (insertHash.isUnique(offset) == false)
			{
				throw new ModifyException("There can be only one insert per offset");
			}
			flb.append((long) offset | MASK_INSERT_BYTE);
			fob.append(content);
		}

        /// <summary> Update the token with the byte array content,
        /// according to the encoding of the master document
        /// </summary>
        /// <param name="offset">
        /// </param>
        /// <param name="newContentBytes">*
        /// </param>
        public void updateToken(int index, byte[] newContentBytes)
        {
            if (newContentBytes == null)
                throw new System.ArgumentException("String newContent can't be null");
            int offset = md.getTokenOffset(index);
            int len = md.getTokenLength(index);
            int type = md.getTokenType(index);
            // one insert
            switch (type)
            {

                case VTDNav.TOKEN_CDATA_VAL:
                    if (md.getEncoding() < VTDNav.FORMAT_UTF_16BE)
                        insertBytesAt(offset - 9, newContentBytes);
                    else
                        insertBytesAt((offset - 9) << 1, newContentBytes);
                    break;

                case VTDNav.TOKEN_COMMENT:
                    if (md.getEncoding() < VTDNav.FORMAT_UTF_16BE)
                    {
                        //UPGRADE_TODO: Method 'java.lang.String.getBytes' was converted to 'System.Text.Encoding.GetEncoding(string).GetBytes(string)' which has a different behavior. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1073_javalangStringgetBytes_javalangString'"
                        insertBytesAt(offset - 4, newContentBytes);
                    }
                    else
                    {
                        //UPGRADE_TODO: Method 'java.lang.String.getBytes' was converted to 'System.Text.Encoding.GetEncoding(string).GetBytes(string)' which has a different behavior. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1073_javalangStringgetBytes_javalangString'"
                        insertBytesAt((offset - 4) << 1, newContentBytes);
                    }
                    break;


                default:
                    insertBytesAt(offset, newContentBytes);
                    break;

            }
            // one delete
            removeToken(index);
        }
		/// <summary> Update the token with the given string value,
		/// notice that string will be converted into byte array
		/// according to the encoding of the master document
		/// </summary>
		/// <param name="offset">
		/// </param>
		/// <param name="newContent">*
		/// </param>
		public void  updateToken(int index, System.String newContent)
		{
			if (newContent == null)
				throw new System.ArgumentException("String newContent can't be null");
			int offset = md.getTokenOffset(index);
			int len = md.getTokenLength(index);
			int type = md.getTokenType(index);
			// one insert
			switch (type)
			{
				
				case VTDNav.TOKEN_CDATA_VAL: 
					if (md.getEncoding() < VTDNav.FORMAT_UTF_16BE)
                        insertBytesAt(offset - 9, eg.GetBytes(newContent));
					else
                        insertBytesAt((offset - 9) << 1, eg.GetBytes(newContent));
					break;
				
				case VTDNav.TOKEN_COMMENT: 
					if (md.getEncoding() < VTDNav.FORMAT_UTF_16BE)
					{
						//UPGRADE_TODO: Method 'java.lang.String.getBytes' was converted to 'System.Text.Encoding.GetEncoding(string).GetBytes(string)' which has a different behavior. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1073_javalangStringgetBytes_javalangString'"
                        insertBytesAt(offset - 4, eg.GetBytes(newContent));
					}
					else
					{
						//UPGRADE_TODO: Method 'java.lang.String.getBytes' was converted to 'System.Text.Encoding.GetEncoding(string).GetBytes(string)' which has a different behavior. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1073_javalangStringgetBytes_javalangString'"
                        insertBytesAt((offset - 4) << 1, eg.GetBytes(newContent));
					}
					break;
				
				
				default:
                    insertBytesAt(offset, eg.GetBytes(newContent));
					break;
				
			}
			// one delete
			removeToken(index);
		}
		
		/// <summary> 
		/// 
		/// 
		/// </summary>
		protected internal void  sort()
		{
            if (flb.size() > 0) 
			quickSort(0, flb.size() - 1);
		}
		
		/// <summary> 
		/// This function will do the range checking and make
		/// sure there is no overlaping or invalid deletion 
		/// There can be only one deletion at one offset value
		/// Delete can't overlap with, nor contains, another delete
		/// 
		/// </summary>
		protected internal void  check()
		{
			int os1, os2, temp;
			int size = flb.size();
			
			for (int i = 0; i < size; i++)
			{
				os1 = flb.lower32At(i);
				os2 = flb.lower32At(i) + (flb.upper32At(i) & 0x1fffffff) - 1;
				if (i + 1 < size)
				{
					temp = flb.lower32At(i + 1);
					if (temp != os1 && temp <= os2)
						throw new ModifyException("Invalid insertion/deletion condition detected between offset " + os1 + " and offset " + os2);
				}
			}
		}

        /// <summary> This method will first call getCurrentIndex() to get the cursor index value
        /// then insert the byte array content after the element
        /// </summary>
        /// <param name="b">*
        /// </param>
        public void insertAfterElement(byte[] b)
        {
            int startTagIndex = md.getCurrentIndex();
            int type = md.getTokenType(startTagIndex);
            if (type != VTDNav.TOKEN_STARTING_TAG)
                throw new ModifyException("Token type is not a starting tag");
            long l = md.getElementFragment();
            int offset = (int)l;
            int len = (int)(l >> 32);
            //UPGRADE_TODO: Method 'java.lang.String.getBytes' was converted to 'System.Text.Encoding.GetEncoding(string).GetBytes(string)' which has a different behavior. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1073_javalangStringgetBytes_javalangString'"
            insertBytesAt(offset + len, b);
        }
		/// <summary> This method will first call getCurrentIndex() to get the cursor index value
		/// then insert the byte value of s before the element
		/// </summary>
		/// <param name="startTagIndex">
		/// </param>
		/// <param name="s">*
		/// </param>
		public void  insertAfterElement(System.String s)
		{
			int startTagIndex = md.getCurrentIndex();
			int type = md.getTokenType(startTagIndex);
			if (type != VTDNav.TOKEN_STARTING_TAG)
				throw new ModifyException("Token type is not a starting tag");
			long l = md.getElementFragment();
			int offset = (int) l;
			int len = (int) (l >> 32);
			//UPGRADE_TODO: Method 'java.lang.String.getBytes' was converted to 'System.Text.Encoding.GetEncoding(string).GetBytes(string)' which has a different behavior. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1073_javalangStringgetBytes_javalangString'"
            insertBytesAt(offset + len, eg.GetBytes(s));
		}

        /// <summary> This method will first call getCurrentIndex() to get the cursor index value
        /// then insert the byte content before the element
        /// </summary>
        /// <param name="b">*
        /// </param>
        public void insertBeforeElement(byte[] b)
        {
            int startTagIndex = md.getCurrentIndex();
            int type = md.getTokenType(startTagIndex);
            if (type != VTDNav.TOKEN_STARTING_TAG)
                throw new ModifyException("Token type is not a starting tag");

            int offset = md.getTokenOffset(startTagIndex) - 1;
            int encoding = md.getTokenType(startTagIndex);
            if (encoding < VTDNav.FORMAT_UTF_16BE)
            {
                //UPGRADE_TODO: Method 'java.lang.String.getBytes' was converted to 'System.Text.Encoding.GetEncoding(string).GetBytes(string)' which has a different behavior. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1073_javalangStringgetBytes_javalangString'"
                insertBytesAt(offset, b);
            }
            else
            {
                //UPGRADE_TODO: Method 'java.lang.String.getBytes' was converted to 'System.Text.Encoding.GetEncoding(string).GetBytes(string)' which has a different behavior. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1073_javalangStringgetBytes_javalangString'"
                insertBytesAt((offset) << 1, b);
            }
        }
		/// <summary> This method will first call getCurrentIndex() to get the cursor index value
		/// then insert the byte value of s before the element
		/// </summary>
		/// <param name="startTagIndex">
		/// </param>
		/// <param name="s">*
		/// </param>
		public void  insertBeforeElement(System.String s)
		{
			int startTagIndex = md.getCurrentIndex();
			int type = md.getTokenType(startTagIndex);
			if (type != VTDNav.TOKEN_STARTING_TAG)
				throw new ModifyException("Token type is not a starting tag");
			
			int offset = md.getTokenOffset(startTagIndex) - 1;
			int encoding = md.getTokenType(startTagIndex);
			if (encoding < VTDNav.FORMAT_UTF_16BE)
			{
				//UPGRADE_TODO: Method 'java.lang.String.getBytes' was converted to 'System.Text.Encoding.GetEncoding(string).GetBytes(string)' which has a different behavior. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1073_javalangStringgetBytes_javalangString'"
				insertBytesAt(offset, eg.GetBytes(s));
			}
			else
			{
				//UPGRADE_TODO: Method 'java.lang.String.getBytes' was converted to 'System.Text.Encoding.GetEncoding(string).GetBytes(string)' which has a different behavior. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1073_javalangStringgetBytes_javalangString'"
                insertBytesAt((offset) << 1, eg.GetBytes(s));
			}
		}

        /// <summary> Insert byte array of an attribute after the starting tag
        /// This method will first call getCurrentIndex() to get the cursor index value
        /// if the index is of type "starting tag", then teh attribute is inserted
        /// after the starting tag
        /// </summary>
        /// <param name="attrBytes">e.g. " attrName='attrVal' ",notice the starting and ending 
        /// white space
        /// 
        /// </param>
        public void insertAttribute(byte[] attrBytes)
        {
            int startTagIndex = md.getCurrentIndex();
            int type = md.getTokenType(startTagIndex);
            if (type != VTDNav.TOKEN_STARTING_TAG)
                throw new ModifyException("Token type is not a starting tag");
            int offset = md.getTokenOffset(startTagIndex);
            int len = md.getTokenLength(startTagIndex);
            int encoding = md.getTokenType(startTagIndex);

            if (encoding < VTDNav.FORMAT_UTF_16BE)
            {
                //UPGRADE_TODO: Method 'java.lang.String.getBytes' was converted to 'System.Text.Encoding.GetEncoding(string).GetBytes(string)' which has a different behavior. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1073_javalangStringgetBytes_javalangString'"
                insertBytesAt(offset + len, attrBytes);
            }
            else
            {
                //UPGRADE_TODO: Method 'java.lang.String.getBytes' was converted to 'System.Text.Encoding.GetEncoding(string).GetBytes(string)' which has a different behavior. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1073_javalangStringgetBytes_javalangString'"
                insertBytesAt((offset + len) << 1, attrBytes);
            }
            //insertBytesAt()
        }
		
		/// <summary> Insert an attribute after the starting tag
		/// This method will first call getCurrentIndex() to get the cursor index value
		/// if the index is of type "starting tag", then teh attribute is inserted
		/// after the starting tag
		/// </summary>
		/// <param name="attr">e.g. " attrName='attrVal' ",notice the starting and ending 
		/// white space
		/// 
		/// </param>
		public void  insertAttribute(System.String attr)
		{
			int startTagIndex = md.getCurrentIndex();
			int type = md.getTokenType(startTagIndex);
			if (type != VTDNav.TOKEN_STARTING_TAG)
				throw new ModifyException("Token type is not a starting tag");
			int offset = md.getTokenOffset(startTagIndex);
			int len = md.getTokenLength(startTagIndex);
			int encoding = md.getTokenType(startTagIndex);
			
			if (encoding < VTDNav.FORMAT_UTF_16BE)
			{
				//UPGRADE_TODO: Method 'java.lang.String.getBytes' was converted to 'System.Text.Encoding.GetEncoding(string).GetBytes(string)' which has a different behavior. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1073_javalangStringgetBytes_javalangString'"
                insertBytesAt(offset + len, eg.GetBytes(attr));
			}
			else
			{
				//UPGRADE_TODO: Method 'java.lang.String.getBytes' was converted to 'System.Text.Encoding.GetEncoding(string).GetBytes(string)' which has a different behavior. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1073_javalangStringgetBytes_javalangString'"
                insertBytesAt((offset + len) << 1, eg.GetBytes(attr));
			}
			//insertBytesAt()
		}
		/// <summary> This method applys the modification to the XML document
		/// and generate output byte content accordingly
		/// Notice that output is not guaranteed to be well-formed 
		/// </summary>
		/// <param name="os">*
		/// </param>
		public void  output(System.IO.Stream os)
		{
			if (os == null)
				throw new System.ArgumentException("OutputStream can't be null");
			sort();
			check();
			long l;
			byte[] ba = md.getXML().getBytes();
			//for (int i = 0; i < flb.size(); i++)
			//{
			//	System.Console.Out.WriteLine(" offset value is ==>" + flb.lower32At(i));
			//}
            int t = md.vtdBuffer.lower32At(0);
            int start = (t == 0) ?
                    md.docOffset : 32;
            int len = (t == 0) ?
                    md.docLen : (md.docLen - 32);
			if (flb.size() == 0)
			{
				os.Write(ba, start,len);
			}
			else
			{
				int offset = start;
				int inc = 1;
				for (int i = 0; i < flb.size(); i = i + inc)
				{
					if (flb.lower32At(i) == flb.lower32At(i + 1))
					{
						inc = 2;
					}
					else
						inc = 1;
					l = flb.longAt(i);
					if (inc == 1)
					{
						if ((l & (~ 0x1fffffffffffffffL)) == XML_DELETE)
						{
							os.Write(ba, offset, flb.lower32At(i) - offset);
							offset = flb.lower32At(i) + (flb.upper32At(i) & 0x1fffffff);
						}
						else
						{
							// insert
							os.Write(ba, offset, flb.lower32At(i) - offset);
							byte[] temp_byteArray;
							temp_byteArray = (byte[]) fob.objectAt(i);
							os.Write(temp_byteArray,0,temp_byteArray.Length);
							offset = flb.lower32At(i);
						}
					}
					else
					{
						if ((l & (~ 0x1fffffffffffffffL)) == XML_DELETE)
						{
							os.Write(ba, offset, flb.lower32At(i) - offset);
							byte[] temp_byteArray2;
							temp_byteArray2 = (byte[]) fob.objectAt(i + 1);
							os.Write(temp_byteArray2, 0, temp_byteArray2.Length);
							offset = flb.lower32At(i) + (flb.upper32At(i) & 0x1fffffff);
						}
						else
						{
							os.Write(ba, offset, flb.lower32At(i + 1) - offset);
							byte[] temp_byteArray3;
							temp_byteArray3 = (byte[]) fob.objectAt(i);
							os.Write(temp_byteArray3, 0, temp_byteArray3.Length);
							offset = flb.lower32At(i + 1) + (flb.upper32At(i + 1) & 0x1fffffff);
						}
					}
				}
                os.Write(ba, offset, start + len - offset);
			}
		}
		
		internal void  quickSort(int lo, int hi)
		{
			//      lo is the lower index, hi is the upper index
			//      of the region of array a that is to be sorted
			//System.out.println("lo ==>"+lo);
			//System.out.println("hi ==>"+hi);
			int i = lo, j = hi;
			long h;
			System.Object o;
			int x = flb.lower32At((lo + hi) / 2);
			
			//  partition
			do 
			{
				while (flb.lower32At(i) < x)
					i++;
				while (flb.lower32At(j) > x)
					j--;
				if (i <= j)
				{
					h = flb.longAt(i);
					o = fob.objectAt(i);
					flb.modifyEntry(i, flb.longAt(j));
					fob.modifyEntry(i, fob.objectAt(j));
					flb.modifyEntry(j, h);
					fob.modifyEntry(j, o);
					i++;
					j--;
				}
			}
			while (i <= j);
			
			//  recursion
			if (lo < j)
				quickSort(lo, j);
			if (i < hi)
				quickSort(i, hi);
		}
		
		
		public void  reset()
		{
			if (flb != null)
				flb.clear();
			if (fob != null)
				fob.clear();
			if (insertHash != null)
				insertHash.reset();
			if (deleteHash != null)
				deleteHash.reset();
		}
	}
}