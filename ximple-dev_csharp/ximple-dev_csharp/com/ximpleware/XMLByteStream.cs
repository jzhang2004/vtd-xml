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
    public class XMLByteStream:System.IO.Stream
    {
        protected Byte[] XMLDoc;
        private int offset;
        public XMLByteStream(int size)
        {
            XMLDoc = new Byte[size];
            offset = 0;
        }

        public override void Write(byte[] ba, int os, int len) 
        {
            if (offset+len > XMLDoc.Length)
                throw new System.IndexOutOfRangeException("XMLDoc size exceeds maximum size");
            Array.Copy(ba, os, XMLDoc, offset, len);
            offset += len;
        }

        public override void WriteByte(byte value)
        {
            if (offset + 1 > XMLDoc.Length)
                throw new System.IndexOutOfRangeException("XMLDoc size exceeds maximum size");
            XMLDoc[offset + 1] = value;
            offset++;
        }

        public void Write(byte[] ba)
        {
            if (ba.Length > XMLDoc.Length)
                throw new System.IndexOutOfRangeException("XMLDoc size exceeds maximum size");
            Write(ba, 0, ba.Length);
            offset += ba.Length;
        }

        public override void Flush()
        {
        }

        public override long Length
        {
            get
            {
                return offset - 1;
            }
        }

        public override bool CanRead
        {
            get
            {
                return false;
            }
        }

        public override int Read(byte[] buffer, int offset, int count)
        {
            throw new NotImplementedException();
        }

        public override void SetLength(long value)
        {
            throw new NotImplementedException();
        }

        public override bool CanSeek
        {
            get { throw new NotImplementedException(); }
        }

        public override bool CanWrite
        {
            get { throw new NotImplementedException(); }
        }


        public override long Position
        {
            get
            {
                throw new NotImplementedException();
            }
            set
            {
                throw new NotImplementedException();
            }
        }

        public override long Seek(long offset, System.IO.SeekOrigin origin)
        {
            throw new NotImplementedException();
        }

        public byte[] getXML()
        {
            return XMLDoc;
        }
             

    }    
}