/* 
* Copyright (C) 2002-2011 XimpleWare, info@ximpleware.com
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
using com.ximpleware.transcode;
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
        private const long MASK_DELETE = 0x00000000000000000L; //0000
        private const long MASK_INSERT_SEGMENT_BYTE = 0x2000000000000000L; //0010
        private const long MASK_INSERT_BYTE = 0x4000000000000000L; //0100
        private const long MASK_INSERT_SEGMENT_BYTE_ENCLOSED = 0x6000000000000000L; //0110
        private const ulong MASK_INSERT_BYTE_ENCLOSED = 0x8000000000000000L; //1000
        //private const long MASK_INSERT_SEGMENT_STRING = 0x6000000000000000L; //0110

        //UPGRADE_TODO: Literal detected as an unsigned long can generate compilation errors. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1175'"
        //private const ulong MASK_INSERT_STRING = 0x8000000000000000L; //1000        
        private const ulong MASK_INSERT_FRAGMENT_NS = 0xa000000000000000L; //1010
        private const ulong MASK_INSERT_FRAGMENT_NS_ENCLOSED = 0x8000000000000000L; //1110
        private static byte[] ba1;// = null;// = {0x3e,0};
        private static byte[] ba2;// = null;// = {0x3c,0};

        private static byte[] ba3;// = null;// = {0,0x3e};
        private static byte[] ba4;// = null;// = {0,0x3c};

        protected internal FastObjectBuffer fob;
        protected internal FastLongBuffer flb;
        internal intHash deleteHash; // one deletion per offset val
        internal intHash insertHash; // one insert per offset val
        protected internal System.String charSet;
        protected internal System.Text.Encoding eg;
        int encoding;
        /// <summary> Constructor for XMLModifier that takes VTDNav object as the master document</summary>
        /// <param name="masterDocument">is the document on which the modification is applied
        /// </param>
        /// 
        static XMLModifier()
        {
            ba1 = new byte[2];
            ba1[0] = 0x3e; ba1[1] = 0;
            ba2 = new byte[2];
            ba2[0] = 0x3c; ba2[1] = 0;

            ba3 = new byte[2];
            ba3[0] = 0; ba3[1] = 0x3e;
            ba4 = new byte[2];
            ba4[0] = 0; ba4[1] = 0x3c;
        }
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
        public void bind(VTDNav masterDocument)
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
            encoding = md.getEncoding();
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
                case VTDNav.FORMAT_ISO_8859_2:
                    charSet = "iso-8859-2";
                    break;
                case VTDNav.FORMAT_ISO_8859_3:
                    charSet = "iso-8859-3";
                    break;
                case VTDNav.FORMAT_ISO_8859_4:
                    charSet = "iso-8859-4";
                    break;
                case VTDNav.FORMAT_ISO_8859_5:
                    charSet = "iso-8859-5";
                    break;
                case VTDNav.FORMAT_ISO_8859_6:
                    charSet = "iso-8859-6";
                    break;
                case VTDNav.FORMAT_ISO_8859_7:
                    charSet = "iso-8859-7";
                    break;
                case VTDNav.FORMAT_ISO_8859_8:
                    charSet = "iso-8859-8";
                    break;
                case VTDNav.FORMAT_ISO_8859_9:
                    charSet = "iso-8859-9";
                    break;
                case VTDNav.FORMAT_ISO_8859_10:
                    charSet = "iso-8859-10";
                    break;
                case VTDNav.FORMAT_ISO_8859_11:
                    charSet = "iso-8859-11";
                    break;
                case VTDNav.FORMAT_ISO_8859_12:
                    charSet = "iso-8859-12";
                    break;
                case VTDNav.FORMAT_ISO_8859_13:
                    charSet = "iso-8859-13";
                    break;
                case VTDNav.FORMAT_ISO_8859_14:
                    charSet = "iso-8859-14";
                    break;
                case VTDNav.FORMAT_ISO_8859_15:
                    charSet = "iso-8859-15";
                    break;
                case VTDNav.FORMAT_WIN_1250:
                    charSet = "windows-1250";
                    break;
                case VTDNav.FORMAT_WIN_1251:
                    charSet = "windows-1251";
                    break;
                case VTDNav.FORMAT_WIN_1252:
                    charSet = "windows-1252";
                    break;
                case VTDNav.FORMAT_WIN_1253:
                    charSet = "windows-1253";
                    break;
                case VTDNav.FORMAT_WIN_1254:
                    charSet = "windows-1254";
                    break;
                case VTDNav.FORMAT_WIN_1255:
                    charSet = "windows-1255";
                    break;
                case VTDNav.FORMAT_WIN_1256:
                    charSet = "windows-1256";
                    break;
                case VTDNav.FORMAT_WIN_1257:
                    charSet = "windows-1257";
                    break;
                case VTDNav.FORMAT_WIN_1258:
                    charSet = "windows-1258";
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
        public void remove()
        {
            int i = md.getCurrentIndex();
            int type = md.getTokenType(i);
            if (type == VTDNav.TOKEN_STARTING_TAG)
            {
                long l = md.getElementFragment();
                removeContent((int)l, (int)(l >> 32));
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
        public void removeToken(int i)
        {
            int type = md.getTokenType(i);
            int os = md.getTokenOffset(i);
            int len =
            (type == VTDNav.TOKEN_STARTING_TAG
                || type == VTDNav.TOKEN_ATTR_NAME
                || type == VTDNav.TOKEN_ATTR_NS)
                ? md.getTokenLength(i) & 0xffff
                : md.getTokenLength(i);
            switch (type)
            {

                case VTDNav.TOKEN_CDATA_VAL:
                    if (md.getEncoding() < VTDNav.FORMAT_UTF_16BE)
                        removeContent(os - 9, len + 12);
                    else
                        removeContent((os - 9) << 1, (len + 12) << 1);
                    return;


                case VTDNav.TOKEN_COMMENT:
                    if (md.getEncoding() < VTDNav.FORMAT_UTF_16BE)
                        removeContent(os - 4, len + 7);
                    else
                        removeContent((os - 4) << 1, (len + 7) << 1);
                    return;


                default:
                    if (md.getEncoding() < VTDNav.FORMAT_UTF_16BE)
                        removeContent(os, len);
                    else
                        removeContent((os) << 1, (len) << 1);
                    return;

            }
        }
        /// <summary> remove an attribute name value pair from the master document</summary>
        /// <param name="attrNameIndex">*
        /// </param>
        public void removeAttribute(int attrNameIndex)
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

        /// <summary> Remove a segment from the byte content denoted by the offset and length 
        /// </summary>
        /// <param name="offset">
        /// </param>
        /// <param name="len">*
        /// </param>
        public void removeContent(int offset, int len)
        {
            if (offset < md.docOffset || len > md.docLen || offset + len > md.docOffset + md.docLen)
            {
                throw new ModifyException("Invalid offset or length for removeContent");
            }
            if (deleteHash.isUnique(offset) == false)
                throw new ModifyException("There can be only one deletion per offset value");

            flb.append(((long)len) << 32 | offset | MASK_DELETE);
            fob.append((System.Object)null);
        }

        /// <summary> Insert a byte array in the document</summary>
        /// <param name="offset">
        /// </param>
        /// <param name="content">*
        /// </param>
        public void insertBytesAt(int offset, byte[] content)
        {

            if (insertHash.isUnique(offset) == false)
            {
                throw new ModifyException("There can be only one insert per offset");
            }
            flb.append((long)offset | MASK_INSERT_BYTE);
            fob.append(content);
        }

        /// <summary>
        ///  Insert a segment in a byte array in the document
        /// </summary>
        /// <param name="offset"></param>
        /// <param name="content"></param>
        /// <param name="contentOffset"></param>
        /// <param name="contentLen"></param>
        public void insertBytesAt(int offset, byte[] content, int contentOffset, int contentLen)
        {
            if (insertHash.isUnique(offset) == false)
            {
                throw new ModifyException("There can be only one insert per offset");
            }
            if (contentOffset < 0
                    || contentLen < 0
                    || contentOffset + contentLen > content.Length)
            {
                throw new ModifyException("Invalid contentOffset and/or contentLen");
            }
            flb.append((long)offset | MASK_INSERT_SEGMENT_BYTE);
            ByteSegment bs = new ByteSegment();
            bs.ba = content;
            bs.len = contentLen;
            bs.offset = contentOffset;

            fob.append(bs);
        }


        private void insertBytesAt(int offset, byte[] content, long l)
        {
            if (insertHash.isUnique(offset) == false)
            {
                throw new ModifyException("There can be only one insert per offset");
            }
            int contentOffset = (int)l;
            int contentLen = (int)(l >> 32);
            if (contentOffset < 0
                    || contentLen < 0
                    || contentOffset + contentLen > content.Length)
            {
                throw new ModifyException("Invalid contentOffset and/or contentLen");
            }
            flb.append((long)offset | MASK_INSERT_SEGMENT_BYTE);
            ByteSegment bs = new ByteSegment();
            bs.ba = content;
            bs.len = contentLen;
            bs.offset = contentOffset;
            fob.append(bs);
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="index"></param>
        /// <param name="newContentBytes"></param>
        /// <param name="contentOffset"></param>
        /// <param name="contentLen"></param>
        /// <param name="src_encoding"></param>
        public void updateToken(int index, byte[] newContentBytes,
        int contentOffset, int contentLen, int src_encoding)
        {

            if (src_encoding == encoding)
            {
                updateToken(index, newContentBytes, contentOffset, contentLen);
                return;
            }
            if (newContentBytes == null)
                throw new System.ArgumentException("newContentBytes can't be null");

            int offset = md.getTokenOffset(index);
            //int len = md.getTokenLength(index);
            int type = md.getTokenType(index);
            int len = (type == VTDNav.TOKEN_STARTING_TAG
                    || type == VTDNav.TOKEN_ATTR_NAME || type == VTDNav.TOKEN_ATTR_NS) ? md
                    .getTokenLength(index) & 0xffff
                    : md.getTokenLength(index);

            // one insert
            byte[] bo = Transcoder.transcode(newContentBytes, contentOffset,
                    contentLen, src_encoding, encoding);

            switch (type)
            {
                case VTDNav.TOKEN_CDATA_VAL:
                    if (encoding < VTDNav.FORMAT_UTF_16BE)
                        insertBytesAt(offset - 9, bo);
                    else
                        insertBytesAt((offset - 9) << 1, bo);
                    break;
                case VTDNav.TOKEN_COMMENT:
                    if (encoding < VTDNav.FORMAT_UTF_16BE)
                        insertBytesAt(offset - 4, bo);
                    else
                        insertBytesAt((offset - 4) << 1, bo);
                    break;

                default:
                    if (encoding < VTDNav.FORMAT_UTF_16BE)
                        insertBytesAt(offset, bo);
                    else
                        insertBytesAt(offset << 1, bo);
                    break;
            }
            // one delete
            removeToken(index);
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="index"></param>
        /// <param name="newContentBytes"></param>
        /// <param name="src_encoding"></param>
        public void updateToken(int index, byte[] newContentBytes, int src_encoding)
        {
            if (src_encoding == encoding)
            {
                updateToken(index, newContentBytes);
                return;
            }
            if (newContentBytes == null)
                throw new System.ArgumentException
                ("newContentBytes can't be null");
            int offset = md.getTokenOffset(index);

            int type = md.getTokenType(index);
            int len =
               (type == VTDNav.TOKEN_STARTING_TAG
                   || type == VTDNav.TOKEN_ATTR_NAME
                   || type == VTDNav.TOKEN_ATTR_NS)
                   ? md.getTokenLength(index) & 0xffff
                   : md.getTokenLength(index);
            // one insert
            byte[] bo = Transcoder.transcode(newContentBytes, 0,
                    newContentBytes.Length, src_encoding, encoding);
            switch (type)
            {
                case VTDNav.TOKEN_CDATA_VAL:
                    if (encoding < VTDNav.FORMAT_UTF_16BE)
                        insertBytesAt(offset - 9, bo);
                    else
                        insertBytesAt((offset - 9) >> 1, bo);
                    break;
                case VTDNav.TOKEN_COMMENT:
                    if (encoding < VTDNav.FORMAT_UTF_16BE)
                        insertBytesAt(offset - 4, bo);
                    else
                        insertBytesAt((offset - 4) >> 1, bo);
                    break;

                default:
                    if (encoding < VTDNav.FORMAT_UTF_16BE)
                        insertBytesAt(offset, bo);
                    else
                        insertBytesAt(offset << 1, bo);
                    break;
            }
            // one delete
            removeToken(index);
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
            //int len = md.getTokenLength(index);
            int type = md.getTokenType(index);
            int len =
            (type == VTDNav.TOKEN_STARTING_TAG
                || type == VTDNav.TOKEN_ATTR_NAME
                || type == VTDNav.TOKEN_ATTR_NS)
                ? md.getTokenLength(index) & 0xffff
                : md.getTokenLength(index);
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
                    if (md.getEncoding() < VTDNav.FORMAT_UTF_16BE)
                        insertBytesAt(offset, newContentBytes);
                    else
                        insertBytesAt(offset << 1, newContentBytes);
                    break;
            }
            // one delete
            removeToken(index);
        }


        public void updateToken(int index, byte[] newContentBytes, int contentOffset, int contentLen)
        {
            if (newContentBytes == null)
                throw new System.ArgumentException
                ("newContentBytes can't be null");

            int offset = md.getTokenOffset(index);
            //int len = md.getTokenLength(index);
            int type = md.getTokenType(index);
            int len =
            (type == VTDNav.TOKEN_STARTING_TAG
                || type == VTDNav.TOKEN_ATTR_NAME
                || type == VTDNav.TOKEN_ATTR_NS)
                ? md.getTokenLength(index) & 0xffff
                : md.getTokenLength(index);
            // one insert
            switch (type)
            {
                case VTDNav.TOKEN_CDATA_VAL:
                    if (encoding < VTDNav.FORMAT_UTF_16BE)
                        insertBytesAt(offset - 9, newContentBytes, contentOffset, contentLen);
                    else
                        insertBytesAt((offset - 9) << 1, newContentBytes, contentOffset, contentLen);
                    break;
                case VTDNav.TOKEN_COMMENT:
                    if (encoding < VTDNav.FORMAT_UTF_16BE)
                        insertBytesAt(offset - 4, newContentBytes, contentOffset, contentLen);
                    else
                        insertBytesAt((offset - 4) << 1, newContentBytes, contentOffset, contentLen);
                    break;

                default:
                    if (encoding < VTDNav.FORMAT_UTF_16BE)
                        insertBytesAt(offset, newContentBytes, contentOffset, contentLen);
                    else
                        insertBytesAt(offset << 1, newContentBytes, contentOffset, contentLen);
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
        public void updateToken(int index, System.String newContent)
        {
            if (newContent == null)
                throw new System.ArgumentException("String newContent can't be null");
            int offset = md.getTokenOffset(index);
            //int len = md.getTokenLength(index);
            int type = md.getTokenType(index);
            int len =
            (type == VTDNav.TOKEN_STARTING_TAG
                || type == VTDNav.TOKEN_ATTR_NAME
                || type == VTDNav.TOKEN_ATTR_NS)
                ? md.getTokenLength(index) & 0xffff
                : md.getTokenLength(index);
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
        protected internal void sort()
        {
            if (flb.size_Renamed_Field > 0)
                quickSort(0, flb.size_Renamed_Field - 1);
        }

        /// <summary> 
        /// This function will do the range checking and make
        /// sure there is no overlaping or invalid deletion 
        /// There can be only one deletion at one offset value
        /// Delete can't overlap with, nor contains, another delete
        /// 
        /// </summary>
        protected internal void check()
        {
            int os1, os2, temp;
            int size = flb.size_Renamed_Field;
            int inc = (md.encoding < VTDNav.FORMAT_UTF_16BE) ? 2 : 4;
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
        /// <summary>
        /// 
        /// </summary>
        /// <param name="src_encoding"></param>
        /// <param name="b"></param>
        public void insertAfterElement(int src_encoding, byte[] b)
        {
            if (src_encoding == encoding)
            {
                insertAfterElement(b);
            }
            else
            {
                int startTagIndex = md.getCurrentIndex();
                int type = md.getTokenType(startTagIndex);
                if (type != VTDNav.TOKEN_STARTING_TAG)
                    throw new ModifyException("Token type is not a starting tag");
                long l = md.getElementFragment();
                int offset = (int)l;
                int len = (int)(l >> 32);
                // transcoding logic
                byte[] bo = Transcoder.transcode(b, 0, b.Length, src_encoding, encoding);
                insertBytesAt(offset + len, bo);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="src_encoding"></param>
        /// <param name="b"></param>
        /// <param name="contentOffset"></param>
        /// <param name="contentLen"></param>
        public void insertAfterElement(int src_encoding, byte[] b, int contentOffset, int contentLen)
        {
            if (src_encoding == encoding)
            {
                insertAfterElement(b, contentOffset, contentLen);
            }
            else
            {
                int startTagIndex = md.getCurrentIndex();
                int type = md.getTokenType(startTagIndex);
                if (type != VTDNav.TOKEN_STARTING_TAG)
                    throw new ModifyException("Token type is not a starting tag");
                long l = md.getElementFragment();
                int offset = (int)l;
                int len = (int)(l >> 32);
                // transcode in here
                byte[] bo = Transcoder.transcode(b, contentOffset, contentLen, src_encoding, encoding);
                insertBytesAt(offset + len, bo);
            }
        }
        /// <summary>
        /// Insert  segment of bytes after the element
        /// </summary>
        /// <param name="b"></param>
        /// <param name="l1"></param>
        public void insertAfterElement(byte[] b, long l1)
        {
            int startTagIndex = md.getCurrentIndex();
            int type = md.getTokenType(startTagIndex);
            if (type != VTDNav.TOKEN_STARTING_TAG)
                throw new ModifyException("Token type is not a starting tag");
            long l = md.getElementFragment();
            int offset = (int)l;
            int len = (int)(l >> 32);
            insertBytesAt(offset + len, b, l1);
        }

        /// <summary>
        /// Insert a segment of bytes after the element
        /// </summary>
        /// <param name="src_encoding"></param>
        /// <param name="b"></param>
        /// <param name="l1"></param>
        public void insertAfterElement(int src_encoding, byte[] b, long l1)
        {
            if (src_encoding == encoding)
            {
                insertAfterElement(b, l1);
            }
            else
            {
                int startTagIndex = md.getCurrentIndex();
                int type = md.getTokenType(startTagIndex);
                if (type != VTDNav.TOKEN_STARTING_TAG)
                    throw new ModifyException("Token type is not a starting tag");
                long l = md.getElementFragment();
                int offset = (int)l;
                int len = (int)(l >> 32);
                byte[] bo = Transcoder.transcode(b, (int)l, (int)l >> 32, src_encoding, encoding);
                insertBytesAt(offset + len, bo, l1);
            }
        }
        /// <summary> This method will first call getCurrentIndex() to get the cursor index value
        /// then insert a segment of the byte array content after the element
        /// </summary>
        /// <param name="b">*
        /// </param>
        public void insertAfterElement(byte[] b, int contentOffset, int contentLen)
        {
            int startTagIndex = md.getCurrentIndex();
            int type = md.getTokenType(startTagIndex);
            if (type != VTDNav.TOKEN_STARTING_TAG)
                throw new ModifyException("Token type is not a starting tag");
            long l = md.getElementFragment();
            int offset = (int)l;
            int len = (int)(l >> 32);
            //UPGRADE_TODO: Method 'java.lang.String.getBytes' was converted to 'System.Text.Encoding.GetEncoding(string).GetBytes(string)' which has a different behavior. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1073_javalangStringgetBytes_javalangString'"
            insertBytesAt(offset + len, b, contentOffset, contentLen);
        }


        /// <summary> This method will first call getCurrentIndex() to get the cursor index value
        /// then insert the byte value of s before the element
        /// </summary>
        /// <param name="startTagIndex">
        /// </param>
        /// <param name="s">*
        /// </param>
        public void insertAfterElement(System.String s)
        {
            int startTagIndex = md.getCurrentIndex();
            int type = md.getTokenType(startTagIndex);
            if (type != VTDNav.TOKEN_STARTING_TAG)
                throw new ModifyException("Token type is not a starting tag");
            long l = md.getElementFragment();
            int offset = (int)l;
            int len = (int)(l >> 32);
            //UPGRADE_TODO: Method 'java.lang.String.getBytes' was converted to 'System.Text.Encoding.GetEncoding(string).GetBytes(string)' which has a different behavior. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1073_javalangStringgetBytes_javalangString'"
            insertBytesAt(offset + len, eg.GetBytes(s));
        }


        /// <summary>
        /// Insert a namespace compensated element fragment after cursor element
        /// </summary>
        /// <param name="ef"></param>
        public void insertAfterElement(ElementFragmentNs ef)
        {
            int startTagIndex = md.getCurrentIndex();
            int type = md.getTokenType(startTagIndex);
            if (type != VTDNav.TOKEN_STARTING_TAG)
                throw new ModifyException("Token type is not a starting tag");
            long l = md.getElementFragment();
            int offset = (int)l;
            int len = (int)(l >> 32);
            insertElementFragmentNsAt(offset + len, ef);
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
            //int encoding = md.getTokenType(startTagIndex);
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
        /// <summary>
        /// 
        /// </summary>
        /// <param name="src_encoding"></param>
        /// <param name="b"></param>
        public void insertBeforeElement(int src_encoding, byte[] b)
        {
            if (src_encoding == md.encoding)
            {
                insertBeforeElement(b);
            }
            else
            {
                int startTagIndex = md.getCurrentIndex();
                int type = md.getTokenType(startTagIndex);
                if (type != VTDNav.TOKEN_STARTING_TAG)
                    throw new ModifyException("Token type is not a starting tag");

                int offset = md.getTokenOffset(startTagIndex) - 1;
                byte[] bo = Transcoder.transcode(b, 0, b.Length, src_encoding, encoding);
                if (encoding < VTDNav.FORMAT_UTF_16BE)
                    insertBytesAt(offset, bo);
                else
                    insertBytesAt((offset) << 1, bo);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="b"></param>
        /// <param name="l1"></param>
        public void insertBeforeElement(byte[] b, long l1)
        {
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
        /// <summary>
        /// 
        /// </summary>
        /// <param name="src_encoding"></param>
        /// <param name="b"></param>
        /// <param name="l1"></param>
        public void insertBeforeElement(int src_encoding, byte[] b, long l1)
        {
            if (src_encoding == md.encoding)
            {
                insertBeforeElement(b, l1);
            }
            else
            {
                int startTagIndex = md.getCurrentIndex();
                int type = md.getTokenType(startTagIndex);
                if (type != VTDNav.TOKEN_STARTING_TAG)
                    throw new ModifyException("Token type is not a starting tag");

                int offset = md.getTokenOffset(startTagIndex) - 1;
                byte[] bo = Transcoder.transcode(b, (int)l1, (int)(l1 >> 32), src_encoding, encoding);
                if (encoding < VTDNav.FORMAT_UTF_16BE)
                    insertBytesAt(offset, bo);
                else
                    insertBytesAt((offset) << 1, bo);
            }
        }
        /// <summary>
        /// insertBeforeElement inserts a segment of a byte array right before an XML element
        /// </summary>
        /// <param name="b"></param>
        public void insertBeforeElement(byte[] b, int contentOffset, int contentLen)
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
                insertBytesAt(offset, b, contentOffset, contentLen);
            }
            else
            {
                //UPGRADE_TODO: Method 'java.lang.String.getBytes' was converted to 'System.Text.Encoding.GetEncoding(string).GetBytes(string)' which has a different behavior. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1073_javalangStringgetBytes_javalangString'"
                insertBytesAt((offset) << 1, b, contentOffset, contentLen);
            }
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="src_encoding"></param>
        /// <param name="b"></param>
        /// <param name="contentOffset"></param>
        /// <param name="contentLen"></param>
        public void insertBeforeElement(int src_encoding, byte[] b, int contentOffset, int contentLen)
        {
            if (src_encoding == encoding)
            {
                insertBeforeElement(b, contentOffset, contentLen);
            }
            else
            {
                int startTagIndex = md.getCurrentIndex();
                int type = md.getTokenType(startTagIndex);
                if (type != VTDNav.TOKEN_STARTING_TAG)
                    throw new ModifyException("Token type is not a starting tag");

                int offset = md.getTokenOffset(startTagIndex) - 1;
                // do transcoding here
                byte[] bo = Transcoder.transcode(b, contentOffset, contentLen, src_encoding, encoding);
                if (encoding < VTDNav.FORMAT_UTF_16BE)
                    insertBytesAt(offset, bo);
                else
                    insertBytesAt((offset) << 1, bo);
            }
        }
        /// <summary> This method will first call getCurrentIndex() to get the cursor index value
        /// then insert the byte value of s before the element
        /// </summary>
        /// <param name="startTagIndex">
        /// </param>
        /// <param name="s">*
        /// </param>
        public void insertBeforeElement(System.String s)
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
            int encoding = md.getTokenType(startTagIndex) & 0xffff;

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
        public void insertAttribute(System.String attr)
        {
            int startTagIndex = md.getCurrentIndex();
            int type = md.getTokenType(startTagIndex);
            if (type != VTDNav.TOKEN_STARTING_TAG)
                throw new ModifyException("Token type is not a starting tag");
            int offset = md.getTokenOffset(startTagIndex);
            int len = md.getTokenLength(startTagIndex);
            int encoding = md.getTokenType(startTagIndex) & 0xffff;

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


        /// <summary>
        /// Insert the transcoded representation of a byte arry of an attribute 
        /// after the starting tag This method will first call getCurrentIndex() 
        /// to get the cursor index value if the index is of type "starting tag", 
        /// then teh attribute is inserted after the starting tag
        /// </summary>
        /// <param name="src_encoding"></param>
        /// <param name="b"></param>
        public void insertAttribute(int src_encoding, byte[] b)
        {
            int startTagIndex = md.getCurrentIndex();
            int type = md.getTokenType(startTagIndex);
            if (type != VTDNav.TOKEN_STARTING_TAG)
                throw new ModifyException("Token type is not a starting tag");
            int offset = md.getTokenOffset(startTagIndex);
            int len = md.getTokenLength(startTagIndex) & 0xffff;
            byte[] bo = Transcoder.transcode(b, 0, b.Length, src_encoding, encoding);
            if (encoding < VTDNav.FORMAT_UTF_16BE)
                insertBytesAt(offset + len, bo);
            else
                insertBytesAt((offset + len) << 1, bo);
            //insertBytesAt()
        }
        /// <summary> This method applys the modification to the XML document
        /// and generate output byte content accordingly
        /// Notice that output is not guaranteed to be well-formed 
        /// </summary>
        /// <param name="os">*
        /// </param>
        public void output(System.IO.Stream os)
        {
            if (os == null)
                throw new System.ArgumentException("OutputStream can't be null");
            sort();
            check();
            long l;
            byte[] ba = md.getXML().getBytes();
            //for (int i = 0; i < flb.size_Renamed_Field; i++)
            //{
            //	System.Console.Out.WriteLine(" offset value is ==>" + flb.lower32At(i));
            //}
            int t = md.vtdBuffer.lower32At(0);
            int start = (t == 0) ?
                    md.docOffset : 32;
            int len = (t == 0) ?
                    md.docLen : (md.docLen - 32);
            if (flb.size_Renamed_Field == 0)
            {
                os.Write(ba, start, len);
            }
            else if (md.encoding < VTDNav.FORMAT_UTF_16BE)
            {
                int offset = start;
                int inc = 1;
                for (int i = 0; i < flb.size_Renamed_Field; i = i + inc)
                {
                    if (i + 1 == flb.size_Renamed_Field)
                    {
                        inc = 1;
                    }
                    else if (flb.lower32At(i) == flb.lower32At(i + 1))
                    {
                        inc = 2; // both insert and remove 
                    }
                    else
                        inc = 1; // either insert or remove
                    l = flb.longAt(i);
                    
                    if (inc == 1)
                    {
                        if ((l & (~0x1fffffffffffffffL)) == XML_DELETE)
                        {
                            os.Write(ba, offset, flb.lower32At(i) - offset);
                            offset = flb.lower32At(i) + (flb.upper32At(i) & 0x1fffffff);
                        }
                        else if ((l & (~0x1fffffffffffffffL)) == MASK_INSERT_BYTE)
                        {
                            // insert
                            os.Write(ba, offset, flb.lower32At(i) - offset);
                            byte[] temp_byteArray = (byte[])fob.objectAt(i);
                            os.Write((byte[])fob.objectAt(i), 0, temp_byteArray.Length);
                            offset = flb.lower32At(i);
                        }
                        else if ((l & (~0x1fffffffffffffffL)) == MASK_INSERT_SEGMENT_BYTE)
                        {
                            os.Write(ba, offset, flb.lower32At(i) - offset);
                            ByteSegment bs = (ByteSegment)fob.objectAt(i);
                            os.Write(bs.ba, bs.offset, bs.len);
                            offset = flb.lower32At(i);
                        }
                        else if ((ulong)(l & (~0x1fffffffffffffffL)) == MASK_INSERT_FRAGMENT_NS)
                        {
                            //ElementFragmentNs
                            os.Write(ba, offset, flb.lower32At(i) - offset);
                            ElementFragmentNs ef = (ElementFragmentNs)fob.objectAt(i);
                            ef.writeToOutputStream(os, md.encoding);
                            offset = flb.lower32At(i);
                        }
                        else if ((ulong)(l & (~0x1fffffffffffffffL)) == MASK_INSERT_BYTE_ENCLOSED)
                        { // insert
                            os.Write(ba, offset, flb.lower32At(i) - offset);
                            os.WriteByte(0x3e);
                            byte[] temp = (byte[])fob.objectAt(i);
                            os.Write(temp,0,temp.Length);                            
                            os.WriteByte(0x3c);
                            offset = flb.lower32At(i);
                        }
                        else if ((l & (~0x1fffffffffffffffL)) == MASK_INSERT_SEGMENT_BYTE_ENCLOSED)
                        {
                            // XML_INSERT_SEGMENT_BYTE
                            os.Write(ba, offset, flb.lower32At(i) - offset);
                            ByteSegment bs = (ByteSegment)fob.objectAt(i);
                            os.WriteByte(0x3e);
                            
                            os.Write(bs.ba, bs.offset, bs.len);
                            os.WriteByte(0x3c);
                            offset = flb.lower32At(i);
                        }
                        else if ((ulong)(l & (~0x1fffffffffffffffL)) == MASK_INSERT_FRAGMENT_NS_ENCLOSED)
                        {
                            //ElementFragmentNs
                            os.Write(ba, offset, flb.lower32At(i) - offset);
                            ElementFragmentNs ef = (ElementFragmentNs)fob.objectAt(i);
                            os.WriteByte(0x3e);
                            ef.writeToOutputStream(os, md.encoding);
                            os.WriteByte(0x3c);
                            offset = flb.lower32At(i);
                        }
                    
                    }
                    else
                    {
                        long k = flb.longAt(i + 1), temp;
                        int i1 = i, temp2;
                        int i2 = i + 1;
                        if ((l & (~0x1fffffffffffffffL)) != MASK_DELETE)
                        {
                            temp = l;
                            l = k;
                            k = temp;
                            temp2 = i1;
                            i1 = i2;
                            i2 = temp2;
                        }

                        os.Write(ba, offset, flb.lower32At(i) - offset);

                        if ((k & (~0x1fffffffffffffffL)) == MASK_INSERT_BYTE)
                        {
                            //os.Write(ba, offset, flb.lower32At(i + 1) - offset);
                            byte[] temp_byteArray3;
                            temp_byteArray3 = (byte[])fob.objectAt(i2);
                            os.Write(temp_byteArray3, 0, temp_byteArray3.Length);
                            offset = flb.lower32At(i1) + (flb.upper32At(i1) & 0x1fffffff);
                        }
                        else if ((k & (~0x1fffffffffffffffL)) == MASK_INSERT_SEGMENT_BYTE)
                        {
                            //os.Write(ba, offset, flb.lower32At(i + 1) - offset);
                            ByteSegment bs = (ByteSegment)fob.objectAt(i2);
                            os.Write(bs.ba, bs.offset, bs.len);
                            offset = flb.lower32At(i1) + (flb.upper32At(i1) & 0x1fffffff);
                        }
                        else if ((ulong)(k & (~0x1fffffffffffffffL)) == MASK_INSERT_FRAGMENT_NS)
                        {
                            //ElementFragmentNs
                            //os.Write(ba, offset, flb.lower32At(i + 1) - offset);
                            ElementFragmentNs ef = (ElementFragmentNs)fob.objectAt(i2);
                            //ef.writeToOutputStream(os);
                            ef.writeToOutputStream(os, md.encoding);
                            offset = flb.lower32At(i1) + (flb.upper32At(i1) & 0x1fffffff);
                        }
                        else if ((ulong)(k & (~0x1fffffffffffffffL)) == MASK_INSERT_BYTE_ENCLOSED)
                        { // insert
                            byte[] temp_byteArray3 = (byte[])fob.objectAt(i2);
                            os.WriteByte(0x3e);
                            os.Write(temp_byteArray3,0,temp_byteArray3.Length);
                            os.WriteByte(0x3c);
                            offset = flb.lower32At(i1) + (flb.upper32At(i1) & 0x1fffffff);
                        }
                        else if ((k & (~0x1fffffffffffffffL)) == MASK_INSERT_SEGMENT_BYTE_ENCLOSED)
                        {
                            // XML_INSERT_SEGMENT_BYTE
                            ByteSegment bs = (ByteSegment)fob.objectAt(i2);
                            os.WriteByte(0x3e);
                            os.Write(bs.ba, bs.offset, bs.len);
                            os.WriteByte(0x3c);
                            offset = flb.lower32At(i1) + (flb.upper32At(i1) & 0x1fffffff);
                        }
                        else if ((ulong)(k & (~0x1fffffffffffffffL)) == MASK_INSERT_FRAGMENT_NS_ENCLOSED)
                        {
                            //ElementFragmentNs
                            ElementFragmentNs ef = (ElementFragmentNs)fob.objectAt(i2);
                            os.WriteByte(0x3e);
                            ef.writeToOutputStream(os, md.encoding);
                            os.WriteByte(0x3c);
                            offset = flb.lower32At(i1) + (flb.upper32At(i1) & 0x1fffffff);
                        }
                    }
                }
                os.Write(ba, offset, start + len - offset);
            }
            else
            {
                byte[] b1 = ba1;
                byte[] b2 = ba2;
                if (md.encoding == VTDNav.FORMAT_UTF_16BE)
                {
                    b1 = ba3;
                    b2 = ba4;
                }
                int offset = start;
                int inc = 1;
                for (int i = 0; i < flb.size_Renamed_Field; i = i + inc)
                {
                    if (i + 1 == flb.size_Renamed_Field)
                    {
                        inc = 1;
                    }
                    else if (flb.lower32At(i) == flb.lower32At(i + 1))
                    {
                        inc = 2; // both insert and remove 
                    }
                    else
                        inc = 1; // either insert or remove
                    l = flb.longAt(i);
                    if (inc == 1)
                    {
                        if ((l & (~0x1fffffffffffffffL)) == XML_DELETE)
                        {
                            os.Write(ba, offset, (flb.lower32At(i) << 1) - offset);
                            offset = (flb.lower32At(i) + (flb.upper32At(i) & 0x1fffffff)) << 1;
                        }
                        else if ((l & (~0x1fffffffffffffffL)) == MASK_INSERT_BYTE)
                        {
                            // insert
                            os.Write(ba, offset, (flb.lower32At(i) << 1) - offset);
                            byte[] temp_byteArray = (byte[])fob.objectAt(i);
                            os.Write((byte[])fob.objectAt(i), 0, temp_byteArray.Length);
                            offset = flb.lower32At(i) << 1;
                        }
                        else if ((l & (~0x1fffffffffffffffL)) == MASK_INSERT_SEGMENT_BYTE)
                        {
                            os.Write(ba, offset, (flb.lower32At(i) << 1) - offset);
                            ByteSegment bs = (ByteSegment)fob.objectAt(i);
                            os.Write(bs.ba, bs.offset, bs.len);
                            offset = flb.lower32At(i) << 1;
                        }
                        else if ((ulong)(l & (~0x1fffffffffffffffL)) == MASK_INSERT_FRAGMENT_NS)
                        {
                            os.Write(ba, offset, (flb.lower32At(i) << 1) - offset);
                            ElementFragmentNs ef = (ElementFragmentNs)fob.objectAt(i);
                            //ef.writeToOutputStream(os);
                            ef.writeToOutputStream(os, md.encoding);
                            offset = flb.lower32At(i) << 1;
                        }
                        else if ((ulong)(l & (~0x1fffffffffffffffL)) == MASK_INSERT_BYTE_ENCLOSED)
                        { // insert
                            // XML_INSERT_SEGMENT_BYTE
                            byte[] tempBA = (byte[])fob.objectAt(i);
                            os.Write(ba, offset, (flb.lower32At(i) << 1) - offset);
                            os.Write(b1,0,b1.Length);
                            os.Write(tempBA,0,tempBA.Length);
                            os.Write(b2,0,b1.Length);
                            offset = flb.lower32At(i) << 1;
                        }
                        else if ((l & (~0x1fffffffffffffffL)) == MASK_INSERT_SEGMENT_BYTE_ENCLOSED)
                        {
                            // XML_INSERT_SEGMENT_BYTE
                            os.Write(ba, offset, (flb.lower32At(i) << 1) - offset);
                            ByteSegment bs = (ByteSegment)fob.objectAt(i);
                            os.Write(b1,0,b1.Length);
                            os.Write(bs.ba, bs.offset, bs.len);
                            os.Write(b2,0,b2.Length);
                            offset = flb.lower32At(i) << 1;
                        }
                        else if ((ulong)(l & (~0x1fffffffffffffffL)) == MASK_INSERT_FRAGMENT_NS_ENCLOSED)
                        {
                            //ElementFragmentNs
                            os.Write(ba, offset, (flb.lower32At(i) << 1) - offset);
                            ElementFragmentNs ef = (ElementFragmentNs)fob.objectAt(i);
                            os.Write(b1,0,b1.Length);
                            ef.writeToOutputStream(os, md.encoding);
                            os.Write(b2,0,b2.Length);
                            offset = flb.lower32At(i) << 1;
                        }
                    }
                    else
                    {
                        long k = flb.longAt(i + 1), temp;
                        int i1 = i, temp2;
                        int i2 = i + 1;
                        if ((l & (~0x1fffffffffffffffL)) != MASK_DELETE)
                        {
                            temp = l;
                            l = k;
                            k = temp;
                            temp2 = i1;
                            i1 = i2;
                            i2 = temp2;
                        }

                        os.Write(ba, offset, (flb.lower32At(i) << 1) - offset);

                        if ((k & (~0x1fffffffffffffffL)) == MASK_INSERT_BYTE)
                        {
                            //os.Write(ba, offset, flb.lower32At(i + 1) - offset);
                            byte[] temp_byteArray3;
                            temp_byteArray3 = (byte[])fob.objectAt(i2);
                            os.Write(temp_byteArray3, 0, temp_byteArray3.Length);
                            offset = (flb.lower32At(i1) + (flb.upper32At(i1) & 0x1fffffff)) << 1;
                        }
                        else if ((k & (~0x1fffffffffffffffL)) == MASK_INSERT_SEGMENT_BYTE)
                        {
                            //os.Write(ba, offset, flb.lower32At(i + 1) - offset);
                            ByteSegment bs = (ByteSegment)fob.objectAt(i2);
                            os.Write(bs.ba, bs.offset, bs.len);
                            offset = (flb.lower32At(i1) + (flb.upper32At(i1) & 0x1fffffff)) << 1;
                        }
                        else if ((ulong)(k & (~0x1fffffffffffffffL)) == MASK_INSERT_FRAGMENT_NS)
                        {
                            //ElementFragmentNs
                            //os.Write(ba, offset, flb.lower32At(i + 1) - offset);
                            ElementFragmentNs ef = (ElementFragmentNs)fob.objectAt(i2);
                            //ef.writeToOutputStream(os);
                            ef.writeToOutputStream(os, md.encoding);
                            offset = (flb.lower32At(i1) + (flb.upper32At(i1) & 0x1fffffff)) << 1;
                        }
                        else if ((ulong)(k & (~0x1fffffffffffffffL)) == MASK_INSERT_BYTE_ENCLOSED)
                        { // insert
                            // XML_INSERT_SEGMENT_BYTE
                            //os.write(ba,offset, flb.lower32At(i2)-offset);
                            byte[] tempBA = (byte[])fob.objectAt(i2);
                            os.Write(b1,0,b1.Length);
                            os.Write(tempBA,0,tempBA.Length);
                            os.Write(b2,0,b2.Length);
                            offset = (flb.lower32At(i1) + (flb.upper32At(i1) & 0x1fffffff)) << 1;
                        }
                        else if ((k & (~0x1fffffffffffffffL)) == MASK_INSERT_SEGMENT_BYTE_ENCLOSED)
                        {
                            // XML_INSERT_SEGMENT_BYTE
                            ByteSegment bs = (ByteSegment)fob.objectAt(i2);
                            os.Write(b1,0,b1.Length);
                            os.Write(bs.ba, bs.offset, bs.len);
                            os.Write(b2,0,b2.Length);
                            offset = (flb.lower32At(i1) + (flb.upper32At(i1) & 0x1fffffff)) << 1;
                        }
                        else if ((ulong)(k & (~0x1fffffffffffffffL)) == MASK_INSERT_FRAGMENT_NS_ENCLOSED)
                        {
                            //ElementFragmentNs
                            //os.write(ba,offset, flb.lower32At(i2)-offset);
                            ElementFragmentNs ef = (ElementFragmentNs)fob.objectAt(i2);
                            os.Write(b1,0,b1.Length);
                            ef.writeToOutputStream(os, md.encoding);
                            os.Write(b2,0,b2.Length);
                            offset = (flb.lower32At(i1) + (flb.upper32At(i1) & 0x1fffffff)) << 1;
                        }
                    }
                }
                os.Write(ba, offset, start + len - offset);
            }
        }

        /// <summary>
        /// Generate updated output XML document and write it into 
        /// a file of given name
        /// </summary>
        /// <param name="fileName"></param>
        public void output(String fileName)
        {
            System.IO.FileStream fs = new System.IO.FileStream(fileName, System.IO.FileMode.Create);
            output(fs);
            fs.Close();
        }

        internal void quickSort(int lo, int hi)
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

        /// <summary>
        /// Reset the internal state of XMLModifer object so it
        /// can be reused
        /// </summary>
        public void reset()
        {
            if (flb != null)
                flb.size_Renamed_Field = 0;
            if (fob != null)
                fob.size_Renamed_Field = 0;
            if (insertHash != null)
                insertHash.reset();
            if (deleteHash != null)
                deleteHash.reset();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="ef"></param>

        public void insertBeforeElement(ElementFragmentNs ef)
        {
            int startTagIndex = md.getCurrentIndex();
            int type = md.getTokenType(startTagIndex);
            if (type != VTDNav.TOKEN_STARTING_TAG)
                throw new ModifyException("Token type is not a starting tag");

            int offset = md.getTokenOffset(startTagIndex) - 1;

            if (encoding < VTDNav.FORMAT_UTF_16BE)
                insertElementFragmentNsAt(offset, ef);
            else
                insertElementFragmentNsAt((offset) << 1, ef);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="offset"></param>
        /// <param name="ef"></param>
        private void insertElementFragmentNsAt(int offset, ElementFragmentNs ef)
        {
            if (insertHash.isUnique(offset) == false)
            {
                throw new ModifyException("There can be only one insert per offset");
            }
            unchecked
            {
                flb.append(((long)offset) | (long)MASK_INSERT_FRAGMENT_NS);
                fob.append(ef);
            }

        }
        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public int getUpdatedDocumentSize()
        {
            int size = flb.size_Renamed_Field;
            int docSize = md.getXML().getBytes().Length;
            int inc = (md.encoding < VTDNav.FORMAT_UTF_16BE) ? 2 : 4;
            long l;
            for (int i = 0; i < size; i++)
            {
                l = flb.longAt(i);
                if ((l & (~0x1fffffffffffffffL)) == MASK_DELETE)
                {
                    docSize -= (int)((l & (0x1fffffffffffffffL)) >> 32);
                }
                else if ((l & (~0x1fffffffffffffffL)) == MASK_INSERT_BYTE)
                {
                    docSize += ((byte[])fob.objectAt(i)).Length;
                }
                else if ((l & (~0x1fffffffffffffffL)) == MASK_INSERT_SEGMENT_BYTE)
                { // MASK_INSERT_SEGMENT_BYTE
                    docSize += ((ByteSegment)fob.objectAt(i)).len;
                }
                else if ((ulong)(l & (~0x1fffffffffffffffL)) == MASK_INSERT_FRAGMENT_NS)
                {
                    docSize += ((ElementFragmentNs)fob.objectAt(i)).getSize(md.encoding);
                }
                else if ((ulong)(l & (~0x1fffffffffffffffL)) == MASK_INSERT_BYTE_ENCLOSED)
                {
                    docSize += ((byte[])fob.objectAt(i)).Length + inc;
                }
                else if ((l & (~0x1fffffffffffffffL)) == MASK_INSERT_SEGMENT_BYTE_ENCLOSED)
                {
                    docSize += ((ByteSegment)fob.objectAt(i)).len + inc;
                }
                else /*if ((l & (~0x1fffffffffffffffL)) == MASK_INSERT_FRAGMENT_NS_ENCLOSED)*/
                {
                    docSize += ((ElementFragmentNs)fob.objectAt(i)).getSize(md.encoding) + inc;
                }
            }
            return docSize;
        }
        /// <summary>
        /// update the cursor element with a new name
        /// </summary>
        /// <param name="newElementName"></param>

        public void updateElementName(String newElementName)
        {
            int i = md.getCurrentIndex();
            int type = md.getTokenType(i);
            if (type != VTDNav.TOKEN_STARTING_TAG)
            {
                throw new ModifyException("You can only update a element name");
            }
            int offset = md.getTokenOffset(i);
            int len = md.getTokenLength(i) & 0xffff;
            updateToken(i, newElementName);
            long l = md.getElementFragment();
            int encoding = md.getEncoding();
            byte[] xml = md.getXML().getBytes();
            int temp = (int)l + (int)(l >> 32);
            if (encoding < VTDNav.FORMAT_UTF_16BE)
            {
                //scan backwards for />
                //int temp = (int)l+(int)(l>>32);
                if (xml[temp - 2] == (byte)'/')
                    return;
                //look for </
                temp--;
                while (xml[temp] != (byte)'/')
                {
                    temp--;
                }
                insertBytesAt(temp + 1, eg.GetBytes(newElementName));
                removeContent(temp + 1, len);
                return;
                //
            }
            else if (encoding == VTDNav.FORMAT_UTF_16BE)
            {

                //scan backwards for />
                if (xml[temp - 3] == (byte)'/' && xml[temp - 4] == 0)
                    return;

                temp -= 2;
                while (!(xml[temp + 1] == (byte)'/' && xml[temp] == 0))
                {
                    temp -= 2;
                }
                insertBytesAt(temp + 2, eg.GetBytes(newElementName));
                removeContent(temp + 2, len << 1);
            }
            else
            {
                //scan backwards for />
                if (xml[temp - 3] == 0 && xml[temp - 4] == '/')
                    return;

                temp -= 2;
                while (!(xml[temp] == (byte)'/' && xml[temp + 1] == 0))
                {
                    temp -= 2;
                }
                insertBytesAt(temp + 2, eg.GetBytes(newElementName));
                removeContent(temp + 2, len << 1);
            }
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="vn"></param>
        /// <param name="contentOffset"></param>
        /// <param name="contentLen"></param>
        public void insertBeforeElement(VTDNav vn, int contentOffset, int contentLen)
        {
            insertBeforeElement(vn.encoding, vn.XMLDoc.getBytes(), contentOffset, contentLen);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="vn"></param>
        /// <param name="l"></param>
        public void insertBeforeElement(VTDNav vn, long l)
        {
            insertBeforeElement(vn.encoding, vn.XMLDoc.getBytes(), l);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="vn"></param>
        /// <param name="contentOffset"></param>
        /// <param name="contentLen"></param>
        public void insertAfterElement(VTDNav vn, int contentOffset, int contentLen)
        {
            insertAfterElement(vn.encoding, vn.XMLDoc.getBytes(), contentOffset, contentLen);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="vn"></param>
        /// <param name="l1"></param>
        public void insertAfterElement(VTDNav vn, long l1)
        {
            insertAfterElement(vn.encoding, vn.XMLDoc.getBytes(), l1);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="index"></param>
        /// <param name="vn"></param>
        /// <param name="contentOffset"></param>
        /// <param name="contentLen"></param>
        public void updateToken(int index, VTDNav vn,
            int contentOffset, int contentLen)
        {
            updateToken(index, vn.XMLDoc.getBytes(), contentOffset, contentLen, vn.encoding);
        }


        /// <summary>
        /// This method will insert byte array b after the head of cursor element
        /// </summary>
        /// <param name="b"></param>
        public void insertAfterHead(byte[] b)
        {
            long i = md.getOffsetAfterHead();
            if (i < 0)
            {
                //throw new ModifyException("Insertion failed");
                // handle empty element case
                // <a/> would become <a>b's content</a>
                // so there are two insertions there
                insertBytesEnclosedAt((int)i - 1, b);
                insertEndingTag(i);
                return;
            }
            insertBytesAt((int)i, b);
        }

        /// <summary>
        /// This method will insert a segment of the byte array b after 
        /// the head of cursor element,
        /// </summary>
        /// <param name="b"></param>
        /// <param name="offset"></param>
        /// <param name="len"></param>
        public void insertAfterHead(byte[] b, int offset, int len)
        {
            long i = md.getOffsetAfterHead();
            if (i < 0)
            {
                //throw new ModifyException("Insertion failed");
                insertBytesEnclosedAt((int)i - 1, b, offset, len);
                insertEndingTag(i);
                return;
            }
            insertBytesAt((int)i, b, offset, len);
        }

        /// <summary>
        /// This method will insert a segment of the byte array b after 
        /// the head of cursor element
        /// </summary>
        /// <param name="b"></param>
        /// <param name="l"></param>
        public void insertAfterHead(byte[] b, long l)
        {
            long i = md.getOffsetAfterHead();
            if (i < 0)
            {
                //throw new ModifyException("Insertion failed");
                insertBytesEnclosedAt((int)i - 1, b, (int)l, (int)(l << 32));
                insertEndingTag(i);
                return;
            }
            insertBytesAt((int)i, b, l);
        }

        /// <summary>
        /// This method will insert an ElementFragmentNs instance 
        /// after the head of cursor element, 
        /// </summary>
        /// <param name="ef"></param>
        public void insertAfterHead(ElementFragmentNs ef)
        {
            long i = md.getOffsetAfterHead();
            if (i < 0)
            {
                //throw new ModifyException("Insertion failed");
                insertElementFragmentNsEnclosedAt((int)i - 1, ef);
                insertEndingTag(i);
                return;
            }
            insertElementFragmentNsAt((int)i, ef);
        }


        /// <summary>
        /// This method will insert the transcoded representation of 
        /// byte array b after the head of cursor element, 
        /// </summary>
        /// <param name="src_encoding"></param>
        /// <param name="b"></param>
        public void insertAfterHead(int src_encoding, byte[] b)
        {
            if (src_encoding == encoding)
            {
                insertAfterHead(b);
            }
            else
            {
                long i = md.getOffsetAfterHead();
                byte[] bo = null;
                if (i < 0)
                {
                    //throw new ModifyException("Insertion failed");
                    bo = Transcoder.transcode(b, 0, b.Length, src_encoding, encoding);
                    insertBytesEnclosedAt((int)i - 1, bo);
                    insertEndingTag(i);
                    return;
                }
                bo = Transcoder.transcode(b, 0, b.Length, src_encoding, encoding);
                insertBytesAt((int)i, bo);
            }
        }
        /// <summary>
        /// This method will insert the transcoded representation of 
        /// a segment of the byte array b after the head of cursor element, 
        /// </summary>
        /// <param name="src_encoding"></param>
        /// <param name="b"></param>
        /// <param name="offset"></param>
        /// <param name="length"></param>
        public void insertAfterHead(int src_encoding, byte[] b, int offset, int length)
        {
            if (src_encoding == encoding)
            {
                insertAfterHead(b, offset, length);
            }
            else
            {
                long i = md.getOffsetAfterHead();
                byte[] bo = null;
                if (i < 0)
                {
                    //throw new ModifyException("Insertion failed");
                    bo = Transcoder.transcode(b, offset, length, src_encoding, encoding);
                    insertBytesEnclosedAt((int)i - 1, bo);
                    insertEndingTag(i);
                    return;
                }
                bo = Transcoder.transcode(b, offset, length, src_encoding, encoding);
                insertBytesAt((int)i, bo, offset, length);
            }
        }

        /// <summary>
        /// This method will insert the transcoded representation of 
        /// a segment of the byte array b  after the head of cursor element, 
        /// </summary>
        /// <param name="src_encoding"></param>
        /// <param name="b"></param>
        /// <param name="l"></param>
        public void insertAfterHead(int src_encoding, byte[] b, long l)
        {
            if (src_encoding == encoding)
            {
                insertAfterHead(b, l);
            }
            else
            {
                long i = md.getOffsetAfterHead();
                byte[] bo = null;
                if (i < 0)
                {
                    //throw new ModifyException("Insertion failed");
                    bo = Transcoder.transcode(b, (int)l, (int)l >> 32, src_encoding, encoding);
                    insertBytesEnclosedAt((int)i - 1, bo, l);
                    insertEndingTag(i);
                    return;
                }
                bo = Transcoder.transcode(b, (int)l, (int)l >> 32, src_encoding, encoding);
                insertBytesAt((int)i, bo, l);
            }
        }

        /// <summary>
        /// This method will insert s' byte array 
        /// representation of the string after the head of cursor element, 
        /// </summary>
        /// <param name="s"></param>
        public void insertAfterHead(String s)
        {
            long i = md.getOffsetAfterHead();
            if (i < 0)
            {
                //throw new ModifyException("Insertion failed");
                insertBytesEnclosedAt((int)i - 1, eg.GetBytes(s));
                insertEndingTag(i);
                return;
            }
            insertBytesAt((int)i, eg.GetBytes(s));
        }


        /// <summary>
        /// This method will insert s' byte array representation after 
        /// the head of cursor element, 
        /// </summary>
        /// <param name="vn"></param>
        /// <param name="contentOffset"></param>
        /// <param name="contentLen"></param>
        public void insertAfterHead(VTDNav vn, int contentOffset, int contentLen)
        {
            insertAfterHead(vn.encoding, vn.XMLDoc.getBytes(), contentOffset, contentLen);
        }
        /// <summary>
        /// This method will insert a segment of the byte array b (contained in vn, and 
        /// transcode into a byte array) after the head of cursor element, 
        /// </summary>
        /// <param name="vn"></param>
        /// <param name="l1"></param>
        public void insertAfterHead(VTDNav vn, long l1)
        {
            insertAfterHead(vn.encoding, vn.XMLDoc.getBytes(), l1);
        }

        public VTDNav outputAndReparse()
        {
            XMLByteStream xbos = new XMLByteStream(getUpdatedDocumentSize());
            output(xbos);
            VTDGen vg = new VTDGen();
            vg.setDoc(xbos.getXML());
            vg.parse(this.md.ns);
            return vg.getNav();
        }

        private void insertBytesEnclosedAt(int offset, byte[] content, int contentOffset, int contentLen)
        {
            if (insertHash.isUnique(offset) == false)
            {
                throw new ModifyException("There can be only one insert per offset");
            }
            if (contentOffset < 0
                    || contentLen < 0
                    || contentOffset + contentLen > content.Length)
            {
                throw new ModifyException("Invalid contentOffset and/or contentLen");
            }
            flb.append((long)offset | MASK_INSERT_SEGMENT_BYTE_ENCLOSED);
            ByteSegment bs = new ByteSegment();
            bs.ba = content;
            bs.len = contentLen;
            bs.offset = contentOffset;
            fob.append(bs);
        }

        private void insertBytesEnclosedAt(int offset, byte[] content, long l)
        {
            if (insertHash.isUnique(offset) == false)
            {
                throw new ModifyException("There can be only one insert per offset");
            }
            int contentOffset = (int)l;
            int contentLen = (int)(l >> 32);
            if (contentOffset < 0
                    || contentLen < 0
                    || contentOffset + contentLen > content.Length)
            {
                throw new ModifyException("Invalid contentOffset and/or contentLen");
            }
            flb.append((long)offset | MASK_INSERT_SEGMENT_BYTE_ENCLOSED);
            ByteSegment bs = new ByteSegment();
            bs.ba = content;
            bs.len = contentLen;
            bs.offset = contentOffset;
            fob.append(bs);
        }
        private void insertBytesEnclosedAt(int offset, byte[] content)
        {

            if (insertHash.isUnique(offset) == false)
            {
                throw new ModifyException("There can be only one insert per offset");
            }
            unchecked
            {
                flb.append((long)offset | (long)MASK_INSERT_BYTE_ENCLOSED);
                fob.append(content);
            }
        }
        private void insertElementFragmentNsEnclosedAt(int offset, ElementFragmentNs ef)
        {
            if (insertHash.isUnique(offset) == false)
            {
                throw new ModifyException("There can be only one insert per offset");
            }
            unchecked
            {
                flb.append((long)offset | (long)MASK_INSERT_FRAGMENT_NS_ENCLOSED);
                fob.append(ef);
            }
        }

        private void insertEndingTag(long l)
        {
            int i = md.getCurrentIndex();
            int offset = md.getTokenOffset(i);
            int length = md.getTokenLength(i) & 0xffff;
            byte[] xml = md.getXML().getBytes();
            if (md.encoding < VTDNav.FORMAT_UTF_16BE)
                insertBytesAt((int)l, xml, offset, length);
            else
                insertBytesAt((int)l, xml, offset << 1, length << 1);
        }
    }
}