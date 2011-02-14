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
using System.Collections.Generic;
using System.Text;

namespace com.ximpleware.transcode
{
    class Transcoder
    {

       /// <summary>
       /// 
       /// </summary>
       /// <param name="input"></param>
       /// <param name="offset"></param>
       /// <param name="length"></param>
       /// <param name="input_encoding"></param>
       /// <param name="output_encoding"></param>
       /// <returns></returns>
        public static byte[] transcode(byte[] input, int offset, int length,
                int input_encoding, int output_encoding)
        {
            //check input and output encoding

            // calculate the length of the output byte array
            int i = getOutLength(input, offset, length, input_encoding,
                    output_encoding);
            // allocate the byte array
            byte[] output = new byte[i];
            // fill the byte array with output encoding
            transcodeAndFill(input, output, offset, length, input_encoding,
                    output_encoding);
            return output;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="input"></param>
        /// <param name="offset"></param>
        /// <param name="length"></param>
        /// <param name="input_encoding"></param>
        /// <param name="output_encoding"></param>
        /// <returns></returns>
        public static int getOutLength(byte[] input, int offset, int length,
                int input_encoding, int output_encoding)
        {
            int len = 0;
            int k = offset;
            int c;
            while (k < offset + length)
            {
                long l = decode(input, k, input_encoding);
                k = (int)(l >> 32);
                c = (int)l;
                len = len + getLen(c, output_encoding);
            }
            return len;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="input"></param>
        /// <param name="output"></param>
        /// <param name="offset"></param>
        /// <param name="length"></param>
        /// <param name="input_encoding"></param>
        /// <param name="output_encoding"></param>
        public static void transcodeAndFill(byte[] input, byte[] output,
                int offset, int length, int input_encoding, int output_encoding)
        {
            int k = offset;
            int c, i = 0;
            while (k < offset + length)
            {
                long l = decode(input, k, input_encoding);
                k = (int)(l >> 32);
                c = (int)l;
                i = encode(output, i, c, output_encoding);
            }
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="initOutPosition"></param>
        /// <param name="input"></param>
        /// <param name="output"></param>
        /// <param name="offset"></param>
        /// <param name="length"></param>
        /// <param name="input_encoding"></param>
        /// <param name="output_encoding"></param>
        /// <returns></returns>
        public static int transcodeAndFill2(int initOutPosition,
                byte[] input,
                byte[] output,
                int offset, int length, int input_encoding, int output_encoding)
        {
            int k = offset;
            int c, i = initOutPosition;
            while (k < offset + length)
            {
                long l = decode(input, k, input_encoding);
                k = (int)(l >> 32);
                c = (int)l;
                i = encode(output, i, c, output_encoding);
            }
            return i;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="input"></param>
        /// <param name="os"></param>
        /// <param name="offset"></param>
        /// <param name="length"></param>
        /// <param name="input_encoding"></param>
        /// <param name="output_encoding"></param>
        public static void transcodeAndWrite(byte[] input,
                System.IO.Stream os,
                int offset, int length, int input_encoding, int output_encoding)
        {
            int k = offset;
            int c;
            while (k < offset + length)
            {
                long l = decode(input, k, input_encoding);
                k = (int)(l >> 32);
                c = (int)l;
                //Console.WriteLine("c ==>" + (char) c+ "  "+c);
                encodeAndWrite(os, c, output_encoding);
            }
        }

       /// <summary>
       /// 
       /// </summary>
       /// <param name="ch"></param>
       /// <param name="output_encoding"></param>
       /// <returns></returns>
        public static int getLen(int ch, int output_encoding)
        {
            if (ch == 0xbe)
            {
                Console.WriteLine("0xbe");
            }
            switch (output_encoding)
            {
                case VTDNav.FORMAT_ASCII:
                    return ASCII_Coder.getLen(ch);
                case VTDNav.FORMAT_UTF8:
                    return UTF8_Coder.getLen(ch);
                case VTDNav.FORMAT_ISO_8859_1:
                    return ISO8859_1Coder.getLen(ch);
                case VTDNav.FORMAT_UTF_16LE:
                    return UTF16LE_Coder.getLen(ch);
                case VTDNav.FORMAT_UTF_16BE:
                    return UTF16BE_Coder.getLen(ch);
                default:
                    throw new com.ximpleware.TranscodeException("Unsupported encoding");
            }
        }

       /// <summary>
       /// 
       /// </summary>
       /// <param name="input"></param>
       /// <param name="offset"></param>
       /// <param name="input_encoding"></param>
       /// <returns></returns>
        public static long decode(byte[] input, int offset, int input_encoding)
        {
            switch (input_encoding)
            {
                case VTDNav.FORMAT_ASCII:
                    return ASCII_Coder.decode(input, offset);
                case VTDNav.FORMAT_UTF8:
                    return UTF8_Coder.decode(input, offset);
                case VTDNav.FORMAT_ISO_8859_1:
                    return ISO8859_1Coder.decode(input, offset);
                case VTDNav.FORMAT_UTF_16LE:
                    return UTF16LE_Coder.decode(input, offset);
                case VTDNav.FORMAT_UTF_16BE:
                    return UTF16BE_Coder.decode(input, offset);
                default:
                    throw new com.ximpleware.TranscodeException("Unsupported encoding");
            }
        }
/// <summary>
/// 
/// </summary>
/// <param name="output"></param>
/// <param name="offset"></param>
/// <param name="ch"></param>
/// <param name="output_encoding"></param>
/// <returns></returns>
        public static int encode(byte[] output, int offset, int ch,
                int output_encoding)
        {
            switch (output_encoding)
            {
                case VTDNav.FORMAT_ASCII:
                    return ASCII_Coder.encode(output, offset, ch);
                case VTDNav.FORMAT_UTF8:
                    return UTF8_Coder.encode(output, offset, ch);
                case VTDNav.FORMAT_ISO_8859_1:
                    return ISO8859_1Coder.encode(output, offset, ch);
                case VTDNav.FORMAT_UTF_16LE:
                    return UTF16LE_Coder.encode(output, offset, ch);
                case VTDNav.FORMAT_UTF_16BE:
                    return UTF16BE_Coder.encode(output, offset, ch);
                default:
                    throw new com.ximpleware.TranscodeException("Unsupported encoding");
            }
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="os"></param>
        /// <param name="ch"></param>
        /// <param name="output_encoding"></param>
        public static void encodeAndWrite(System.IO.Stream os, int ch,
                int output_encoding)
        {
            switch (output_encoding)
            {
                case VTDNav.FORMAT_ASCII:
                    ASCII_Coder.encodeAndWrite(os, ch);
                    return;
                case VTDNav.FORMAT_UTF8:
                    UTF8_Coder.encodeAndWrite(os, ch);
                    return;
                case VTDNav.FORMAT_ISO_8859_1:
                    ISO8859_1Coder.encodeAndWrite(os, ch);
                    return;
                case VTDNav.FORMAT_UTF_16LE:
                    UTF16LE_Coder.encodeAndWrite(os, ch);
                    return;
                case VTDNav.FORMAT_UTF_16BE:
                    UTF16BE_Coder.encodeAndWrite(os, ch);
                    return;
                default:
                    throw new com.ximpleware.TranscodeException("Unsupported encoding");
            }
        }
    }
}
