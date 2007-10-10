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
    /// <summary> ElementFragmentNs is class representing namespace compensated elementFragment
    /// its constructor is protected, only intended to be called by getElementFragmentNs(). 
    /// 
    /// </summary>
    public class ElementFragmentNs
    {
        /// <summary> getSize gets the fragment with byte copying</summary>
        /// <returns>
        /// 
        /// </returns>
        public int Size
        {
            get
            {
                //int encoding = vn.encoding;
                int len = (int)(l >> 32);
                if (stLen != 0)
                    for (int i = 0; i < fib.size(); i++)
                    {
                        int k = fib.intAt(i);
                        len += (vn.getTokenLength(k) & 0xffff) + vn.getTokenLength(k + 1) + 4;
                    }
                return len;
            }

        }
        /// <summary> Get the long encoding the len and offset of uncompensated element fragment</summary>
        /// <returns>
        /// 
        /// </returns>
        public long OffsetLen
        {
            get
            {
                return l;
            }

        }
        internal VTDNav vn;
        internal long l;
        internal FastIntBuffer fib;
        internal int stLen; // length of starting tag 
        internal static byte[] ws; // a few byte patterns for white space and '='
        protected internal ElementFragmentNs(VTDNav vn1, long l1, FastIntBuffer fib1, int len)
        {
            vn = vn1;
            l = l1;
            fib = fib1;
            stLen = len;
        }
        /// <summary> Return a byte array with namespace compensation
        /// witht the orginal encoding format
        /// </summary>
        /// <returns>
        /// 
        /// </returns>
        public byte[] toBytes()
        {
            byte[] ba = new byte[Size];

            int os = (int)l;
            int len = (int)(l >> 32);
            int os1 = 0;
            byte[] xml = vn.getXML().getBytes();
            if (stLen == 0)
            {
                Array.Copy(xml, os, ba, 0, len);
                return ba;
            }
            int enc = vn.getEncoding();
            int temp = 0;

            switch (enc)
            {

                case VTDNav.FORMAT_UTF_16BE:
                case VTDNav.FORMAT_UTF_16LE: temp = (stLen + 1) << 1; break;

                default:
                    temp = stLen + 1;
                    break;

            }
            Array.Copy(xml, os, ba, 0, temp);

            //namespace compensation
            os1 += temp;

            int tos = 0, tlen = 0;
            for (int i = 0; i < fib.size(); i++)
            {
                //System.out.println("i ==>"+fib.intAt(i));
                switch (enc)
                {

                    case VTDNav.FORMAT_UTF_16BE:
                        
                        Array.Copy(ws, 0, ba, os1, 2);
                        os1 += 2;
                        tos = vn.getTokenOffset(fib.intAt(i)) << 1;
                        tlen = (vn.getTokenLength(fib.intAt(i)) & 0xffff) << 1;
                        Array.Copy(xml, tos, ba, os1, tlen);
                        os1 += tlen;
                        
                        Array.Copy(ws, 2, ba, os1, 2);
                        os1 += 2;
                        tos = (vn.getTokenOffset(fib.intAt(i) + 1) - 1) << 1;
                        tlen = ((vn.getTokenLength(fib.intAt(i) + 1) & 0xffff) + 2) << 1;
                        Array.Copy(xml, tos, ba, os1, tlen);
                        os1 += tlen;
                        break;

                    case VTDNav.FORMAT_UTF_16LE:
                        
                        Array.Copy(ws, 1, ba, os1, 2);
                        os1 += 2;
                        tos = vn.getTokenOffset(fib.intAt(i)) << 1;
                        tlen = (vn.getTokenLength(fib.intAt(i)) & 0xffff) << 1;
                        Array.Copy(xml, tos, ba, os1, tlen);
                        os1 += tlen;
                        
                        Array.Copy(ws, 3, ba, os1, 2);
                        os1 += 2;
                        tos = (vn.getTokenOffset(fib.intAt(i) + 1) - 1) << 1;
                        tlen = ((vn.getTokenLength(fib.intAt(i) + 1) & 0xffff) + 2) << 1;
                        Array.Copy(xml, tos, ba, os1, tlen);
                        os1 += tlen;
                        break;

                    default:
                        
                        Array.Copy(ws, 1, ba, os1, 1);
                        os1++;
                        tos = vn.getTokenOffset(fib.intAt(i));
                        tlen = (vn.getTokenLength(fib.intAt(i)) & 0xffff);
                        Array.Copy(xml, tos, ba, os1, tlen);
                        os1 += tlen;
                        Array.Copy(ws, 3, ba, os1, 1);

                        os1++;
                        tos = vn.getTokenOffset(fib.intAt(i) + 1) - 1;
                        tlen = (vn.getTokenLength(fib.intAt(i) + 1) & 0xffff) + 2;
                        Array.Copy(xml, tos, ba, os1, tlen);
                        os1 += tlen;
                        break;

                }
            }
            switch (enc)
            {

                case VTDNav.FORMAT_UTF_16BE:
                case VTDNav.FORMAT_UTF_16LE:
                    temp = (stLen + 1) << 1; break;

                default:
                    temp = stLen + 1;
                    break;

            }
            Array.Copy(xml, os + temp, ba, os1, len - temp);
            return ba;
        }

        /// <summary> Write ns compensated fragments (bytes in original encoding format) to outputstream</summary>
        /// <param name="ost">
        /// </param>
        /// <throws>  IOException </throws>
        /// <summary> 
        /// </summary>
        public void writeToOutputStream(System.IO.Stream ost)
        {
            int os = (int)l;
            int len = (int)(l >> 32);
            int os1 = 0;
            byte[] xml = vn.getXML().getBytes();
            if (stLen == 0)
            {
                ost.Write(xml, os, len);                
            }
            int enc = vn.getEncoding();
            int temp = 0;

            switch (enc)
            {

                case VTDNav.FORMAT_UTF_16BE:
                case VTDNav.FORMAT_UTF_16LE: temp = (stLen + 1) << 1; break;

                default:
                    temp = stLen + 1;
                    break;

            }
            ost.Write(xml, os, temp);
            int tos = 0, tlen = 0;
            for (int i = 0; i < fib.size(); i++)
            {
                switch (enc)
                {

                    case VTDNav.FORMAT_UTF_16BE:
                        ost.Write(ws, 0, 2);
                        tos = vn.getTokenOffset(fib.intAt(i)) << 1;
                        tlen = (vn.getTokenLength(fib.intAt(i)) & 0xffff) << 1;
                        ost.Write(xml, tos, tlen);
                        ost.Write(ws, 2, 2);
                        tos = (vn.getTokenOffset(fib.intAt(i) + 1) - 1) << 1;
                        tlen = ((vn.getTokenLength(fib.intAt(i) + 1) & 0xffff) + 2) << 1;
                        ost.Write(xml, tos, tlen);
                        break;

                    case VTDNav.FORMAT_UTF_16LE:
                        ost.Write(ws, 1, 2);
                        tos = vn.getTokenOffset(fib.intAt(i)) << 1;
                        tlen = (vn.getTokenLength(fib.intAt(i)) & 0xffff) << 1;
                        ost.Write(xml, tos, tlen);
                        ost.Write(ws, 3, 2);
                        tos = (vn.getTokenOffset(fib.intAt(i) + 1) - 1) << 1;
                        tlen = ((vn.getTokenLength(fib.intAt(i) + 1) & 0xffff) + 2) << 1;
                        ost.Write(xml, tos, tlen);
                        break;

                    default:
                        ost.Write(ws, 1, 1);
                        tos = vn.getTokenOffset(fib.intAt(i));
                        tlen = (vn.getTokenLength(fib.intAt(i)) & 0xffff);
                        ost.Write(xml, tos, tlen);
                        ost.Write(ws, 3, 1);
                        tos = vn.getTokenOffset(fib.intAt(i) + 1) - 1;
                        tlen = (vn.getTokenLength(fib.intAt(i) + 1) & 0xffff) + 2;
                        ost.Write(xml, tos, tlen);
                        break;

                }
            }
            
            switch (enc)
            {

                case VTDNav.FORMAT_UTF_16BE:
                case VTDNav.FORMAT_UTF_16LE:
                    temp = (stLen + 1) << 1; break;

                default:
                    temp = stLen + 1;
                    break;

            }
            
            ost.Write(xml, os + temp, len - temp);
        }

        static ElementFragmentNs()
        {
            {
                ws = new byte[5];
                ws[0] = 0;
                ws[1] = (byte)' ';
                ws[2] = 0;
                ws[3] = (byte)'=';
                ws[4] = 0;
            }
        }
    }
}