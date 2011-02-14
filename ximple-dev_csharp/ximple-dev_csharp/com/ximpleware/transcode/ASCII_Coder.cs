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
    class ASCII_Coder
    {
        /// <summary>
        /// 
        /// </summary>
        /// <param name="ch"></param>
        /// <returns></returns>
        public static int getLen(int ch)
        {
            if (ch >= 128)
                throw new TranscodeException("Invalid UCS char for ASCII format");
            else
                return 1;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="input"></param>
        /// <param name="offset"></param>
        /// <returns></returns>
        public static long decode(byte[] input, int offset)
        {
            long l = input[offset];
            return (((long)(offset + 1)) << 32) | l;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="output"></param>
        /// <param name="offset"></param>
        /// <param name="ch"></param>
        /// <returns></returns>
        public static int encode(byte[] output, int offset, int ch)
        {
            output[offset] = (byte)ch;
            return offset + 1;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="os"></param>
        /// <param name="ch"></param>
        public static void encodeAndWrite(System.IO.Stream os, int ch)
        {
            if (ch >= 128)
                throw new TranscodeException("Invalid UCS char for ASCII format");
            os.WriteByte((byte)ch);
        }
    }
}
