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
namespace com.ximpleware
{

    public class IndexHandler
    {
        public const int OFFSET_ADJUSTMENT = 32;
        /// <summary>
        /// This function is called within VTDGen and 
        /// VTDNav's writeIndex
        /// </summary>
        /// <param name="version"></param>
        /// <param name="encodingType"></param>
        /// <param name="ns"></param>
        /// <param name="byteOrder"></param>
        /// <param name="nestDepth"></param>
        /// <param name="LCLevel"></param>
        /// <param name="rootIndex"></param>
        /// <param name="xmlDoc"></param>
        /// <param name="docOffset"></param>
        /// <param name="docLen"></param>
        /// <param name="vtdBuffer"></param>
        /// <param name="l1Buffer"></param>
        /// <param name="l2Buffer"></param>
        /// <param name="l3Buffer"></param>
        /// <param name="os"></param>
        /// <returns></returns>
        public static void writeIndex_L3(byte version,
            int encodingType,
            bool ns,
            bool byteOrder,
            int nestDepth,
            int LCLevel,
            int rootIndex,
            byte[] xmlDoc,
            int docOffset,
            int docLen,
            FastLongBuffer vtdBuffer,
            FastLongBuffer l1Buffer,
            FastLongBuffer l2Buffer,
            FastIntBuffer l3Buffer,
            System.IO.Stream os)
        {
            if (xmlDoc == null
                || docLen <= 0
                || vtdBuffer == null
                || l1Buffer == null
                || l2Buffer == null
                || l3Buffer == null
                || LCLevel != 3)
            {
                throw new System.ArgumentException("Invalid argument for writeIndex ");
            }
            if (vtdBuffer.size_Renamed_Field == 0)
            {
                throw new IndexWriteException("VTDBuffer can't be zero length");
            }
            int i;

            System.IO.BinaryWriter dos = new System.IO.BinaryWriter(os);
            // first 4 bytes
            byte[] ba = new byte[4];
            ba[0] = (byte)version; // version # is 1 
            ba[1] = (byte)encodingType;
            if (BitConverter.IsLittleEndian == false)
                ba[2] = (byte)(ns ? 0xe0 : 0xa0); // big endien
            else
                ba[2] = (byte)(ns ? 0xc0 : 0x80);
            ba[3] = (byte)nestDepth;
            dos.Write(ba);
            // second 4 bytes
            ba[0] = 0;
            ba[1] = 4;
            ba[2] = (byte)((rootIndex & 0xff00) >> 8);
            ba[3] = (byte)(rootIndex & 0xff);
            dos.Write(ba);
            // 2 reserved 32-bit words set to zero
            ba[1] = ba[2] = ba[3] = 0;
            dos.Write(ba);
            dos.Write(ba);
            dos.Write(ba);
            dos.Write(ba);
            // write XML doc in bytes
            dos.Write((long)docLen);
            dos.Write(xmlDoc, docOffset, docLen);
            //dos.Write(xmlDoc, docOffset, docLen);
            // zero padding to make it integer multiple of 64 bits
            if ((docLen & 0x07) != 0)
            {
                int t = (((docLen >> 3) + 1) << 3) - docLen;
                for (; t > 0; t--)
                    dos.Write((System.Byte)0);
            }
            // write VTD

            dos.Write((long)vtdBuffer.size_Renamed_Field);
            if (docOffset != 0)
            {
                for (i = 0; i < vtdBuffer.size_Renamed_Field; i++)
                {
                    dos.Write(adjust(vtdBuffer.longAt(i), -docOffset));
                }
            }
            else
            {
                for (i = 0; i < vtdBuffer.size_Renamed_Field; i++)
                {
                    dos.Write(vtdBuffer.longAt(i));
                }
            }
            // write L1 
            dos.Write((long)l1Buffer.size_Renamed_Field);
            for (i = 0; i < l1Buffer.size_Renamed_Field; i++)
            {
                dos.Write(l1Buffer.longAt(i));
            }
            // write L2
            dos.Write((long)l2Buffer.size_Renamed_Field);
            for (i = 0; i < l2Buffer.size_Renamed_Field; i++)
            {
                dos.Write(l2Buffer.longAt(i));
            }
            // write L3
            dos.Write((long)l3Buffer.size_Renamed_Field);
            for (i = 0; i < l3Buffer.size_Renamed_Field; i++)
            {
                dos.Write(l3Buffer.intAt(i));
            }
            // pad zero if # of l3 entry is odd
            if ((l3Buffer.size_Renamed_Field & 1) != 0)
                dos.Write(0);
            dos.Close();
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="version"></param>
        /// <param name="encodingType"></param>
        /// <param name="ns"></param>
        /// <param name="byteOrder"></param>
        /// <param name="nestDepth"></param>
        /// <param name="LCLevel"></param>
        /// <param name="rootIndex"></param>
        /// <param name="xmlDoc"></param>
        /// <param name="docOffset"></param>
        /// <param name="docLen"></param>
        /// <param name="vtdBuffer"></param>
        /// <param name="l1Buffer"></param>
        /// <param name="l2Buffer"></param>
        /// <param name="l3Buffer"></param>
        /// <param name="os"></param>
        public static void writeIndex_L5(byte version,
    int encodingType,
    bool ns,
    bool byteOrder,
    int nestDepth,
    int LCLevel,
    int rootIndex,
    byte[] xmlDoc,
    int docOffset,
    int docLen,
    FastLongBuffer vtdBuffer,
    FastLongBuffer l1Buffer,
    FastLongBuffer l2Buffer,
    FastLongBuffer l3Buffer,
    FastLongBuffer l4Buffer,
    FastIntBuffer l5Buffer,
    System.IO.Stream os)
        {
            if (xmlDoc == null
                || docLen <= 0
                || vtdBuffer == null
                || l1Buffer == null
                || l2Buffer == null
                || l3Buffer == null
                || l4Buffer == null
                || l5Buffer == null
                || LCLevel != 5
                )
            {
                throw new System.ArgumentException("Invalid argument for writeIndex ");
            }
            if (vtdBuffer.size_Renamed_Field == 0)
            {
                throw new IndexWriteException("VTDBuffer can't be zero length");
            }
            int i;

            System.IO.BinaryWriter dos = new System.IO.BinaryWriter(os);
            // first 4 bytes
            byte[] ba = new byte[4];
            ba[0] = (byte)version; // version # is 1 
            ba[1] = (byte)encodingType;
            if (BitConverter.IsLittleEndian == false)
                ba[2] = (byte)(ns ? 0xe0 : 0xa0); // big endien
            else
                ba[2] = (byte)(ns ? 0xc0 : 0x80);
            ba[3] = (byte)nestDepth;
            dos.Write(ba);
            // second 4 bytes
            ba[0] = 0;
            ba[1] = 6;
            ba[2] = (byte)((rootIndex & 0xff00) >> 8);
            ba[3] = (byte)(rootIndex & 0xff);
            dos.Write(ba);
            // 2 reserved 32-bit words set to zero
            ba[1] = ba[2] = ba[3] = 0;
            dos.Write(ba);
            dos.Write(ba);
            dos.Write(ba);
            dos.Write(ba);
            // write XML doc in bytes
            dos.Write((long)docLen);
            dos.Write(xmlDoc, docOffset, docLen);
            //dos.Write(xmlDoc, docOffset, docLen);
            // zero padding to make it integer multiple of 64 bits
            if ((docLen & 0x07) != 0)
            {
                int t = (((docLen >> 3) + 1) << 3) - docLen;
                for (; t > 0; t--)
                    dos.Write((System.Byte)0);
            }
            // write VTD

            dos.Write((long)vtdBuffer.size_Renamed_Field);
            if (docOffset != 0)
            {
                for (i = 0; i < vtdBuffer.size_Renamed_Field; i++)
                {
                    dos.Write(adjust(vtdBuffer.longAt(i), -docOffset));
                }
            }
            else
            {
                for (i = 0; i < vtdBuffer.size_Renamed_Field; i++)
                {
                    dos.Write(vtdBuffer.longAt(i));
                }
            }
            // write L1 
            dos.Write((long)l1Buffer.size_Renamed_Field);
            for (i = 0; i < l1Buffer.size_Renamed_Field; i++)
            {
                dos.Write(l1Buffer.longAt(i));
            }
            // write L2
            dos.Write((long)l2Buffer.size_Renamed_Field);
            for (i = 0; i < l2Buffer.size_Renamed_Field; i++)
            {
                dos.Write(l2Buffer.longAt(i));
            }
            // write L3
            dos.Write((long)l3Buffer.size_Renamed_Field);
            for (i = 0; i < l3Buffer.size_Renamed_Field; i++)
            {
                dos.Write(l3Buffer.longAt(i));
            }
            // write L4
            dos.Write((long)l4Buffer.size_Renamed_Field);
            for (i = 0; i < l4Buffer.size_Renamed_Field; i++)
            {
                dos.Write(l4Buffer.longAt(i));
            }
            // write L5
            dos.Write((long)l5Buffer.size_Renamed_Field);
            for (i = 0; i < l5Buffer.size_Renamed_Field; i++)
            {
                dos.Write(l5Buffer.intAt(i));
            }
            // pad zero if # of l3 entry is odd
            if ((l5Buffer.size_Renamed_Field & 1) != 0)
                dos.Write(0);
            dos.Close();
        }
        /// <summary>
        /// This function is called within VTDGen's loadIndex
        /// </summary>
        /// <param name="is_Renamed"></param>
        /// <param name="vg"></param>
        /// <returns></returns>
        public static void readIndex(System.IO.Stream is_Renamed, VTDGen vg)
        {
            if (is_Renamed == null || vg == null)
            {
                throw new System.ArgumentException("Invalid argument(s) for readIndex()");
            }
            //UPGRADE_TODO: Class 'java.io.DataInputStream' was converted to 'System.IO.BinaryReader' 
            //which has a different behavior. 
            //"ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1073_javaioDataInputStream'"
            System.IO.BinaryReader dis = new System.IO.BinaryReader(is_Renamed);
            byte b = dis.ReadByte(); // first byte
            // no check on version number for now
            // second byte
            vg.encoding = dis.ReadByte();
            int intLongSwitch;
            int endian;
            // third byte
            b = dis.ReadByte();
            if ((b & 0x80) != 0)
                intLongSwitch = 1;
            //use ints
            else
                intLongSwitch = 0;
            if ((b & 0x40) != 0)
                vg.ns = true;
            else
                vg.ns = false;
            if ((b & 0x20) != 0)
                endian = 1;
            else
                endian = 0;
            if ((b & 0x1f) != 0)
                throw new IndexReadException("Last 5 bits of the third byte should be zero");

            // fourth byte
            vg.VTDDepth = dis.ReadByte();

            // 5th and 6th byte
            int LCLevel = (((int)dis.ReadByte()) << 8) | dis.ReadByte();
            if (LCLevel != 4 &&  LCLevel != 6)
            {
                throw new IndexReadException("LC levels must be at least 3");
            }
            if (LCLevel == 4)
                vg.shallowDepth = true;
            else
                vg.shallowDepth = false;
            // 7th and 8th byte
            vg.rootIndex = (((int)dis.ReadByte()) << 8) | dis.ReadByte();

            // skip a long
            long l = dis.ReadInt64();
            //Console.WriteLine(" l ==>" + l);
            l = dis.ReadInt64();
            //Console.WriteLine(" l ==>" + l);
            l = dis.ReadInt64();
            //Console.WriteLine(" l ==>" + l);
            int size;
            // read XML size
            if (BitConverter.IsLittleEndian && endian == 0
                || BitConverter.IsLittleEndian == false && endian == 1)
                size = (int)l;
            else
                size = (int)reverseLong(l);


            // read XML bytes
            byte[] XMLDoc = new byte[size];
            dis.Read(XMLDoc, 0, size);
            if ((size & 0x7) != 0)
            {
                int t = (((size >> 3) + 1) << 3) - size;
                while (t > 0)
                {
                    dis.ReadByte();
                    t--;
                }
            }

            vg.setDoc(XMLDoc);

            if (BitConverter.IsLittleEndian && endian == 0
                || BitConverter.IsLittleEndian == false && endian == 1)
            {
                // read vtd records
                int vtdSize = (int)dis.ReadInt64();
                while (vtdSize > 0)
                {
                    vg.VTDBuffer.append(dis.ReadInt64());
                    vtdSize--;
                }
                // read L1 LC records
                int l1Size = (int)dis.ReadInt64();
                while (l1Size > 0)
                {
                    vg.l1Buffer.append(dis.ReadInt64());
                    l1Size--;
                }
                // read L2 LC records
                int l2Size = (int)dis.ReadInt64();
                while (l2Size > 0)
                {
                    vg.l2Buffer.append(dis.ReadInt64());
                    l2Size--;
                }
                // read L3 LC records
                int l3Size = (int)dis.ReadInt64();
                if (vg.shallowDepth)
                {
                    if (intLongSwitch == 1)
                    {
                        //l3 uses ints
                        while (l3Size > 0)
                        {
                            vg.l3Buffer.append(dis.ReadInt32());
                            l3Size--;
                        }
                    }
                    else
                    {
                        while (l3Size > 0)
                        {
                            vg.l3Buffer.append((int)(dis.ReadInt64() >> 32));
                            l3Size--;
                        }
                    }
                }
                else
                {
                    while (l3Size > 0)
                    {
                        vg._l3Buffer.append(dis.ReadInt64());
                        l3Size--;
                    }

                    int l4Size = (int)dis.ReadInt64();
                    while (l4Size > 0)
                    {
                        vg._l4Buffer.append(dis.ReadInt64());
                        l4Size--;
                    }

                    int l5Size = (int)dis.ReadInt64();
                    if (intLongSwitch == 1)
                    {
                        while (l5Size > 0)
                        {
                            vg._l5Buffer.append(dis.ReadInt32());
                            l5Size--;
                        }
                    }
                    else
                    {
                        while (l5Size > 0)
                        {
                            vg._l5Buffer.append((int)(dis.ReadInt64() >> 32));
                            l5Size--;
                        }
                    }
                }
            }
            else
            {
                // read vtd records
                int vtdSize = (int)reverseLong(dis.ReadInt64());
                while (vtdSize > 0)
                {
                    vg.VTDBuffer.append(reverseLong(dis.ReadInt64()));
                    vtdSize--;
                }
                // read L1 LC records
                int l1Size = (int)reverseLong(dis.ReadInt64());
                while (l1Size > 0)
                {
                    vg.l1Buffer.append(reverseLong(dis.ReadInt64()));
                    l1Size--;
                }
                // read L2 LC records
                int l2Size = (int)reverseLong(dis.ReadInt64());
                while (l2Size > 0)
                {
                    vg.l2Buffer.append(reverseLong(dis.ReadInt64()));
                    l2Size--;
                }
                // read L3 LC records
                int l3Size = (int)reverseLong(dis.ReadInt64());
                if (vg.shallowDepth)
                {
                    if (intLongSwitch == 1)
                    {
                        //l3 uses ints
                        while (l3Size > 0)
                        {
                            vg.l3Buffer.append(reverseInt(dis.ReadInt32()));
                            l3Size--;
                        }
                    }
                    else
                    {
                        while (l3Size > 0)
                        {
                            vg.l3Buffer.append(reverseInt((int)(dis.ReadInt64() >> 32)));
                            l3Size--;
                        }
                    }
                }
                else
                {
                    while (l3Size > 0)
                    {
                        vg._l3Buffer.append(reverseLong(dis.ReadInt64()));
                        l3Size--;
                    }

                    int l4Size = (int)reverseLong(dis.ReadInt64());
                    {
                        vg._l4Buffer.append(reverseLong(dis.ReadInt64()));
                        l4Size--;
                    }

                    int l5Size = (int)reverseLong(dis.ReadInt64());
                    if (intLongSwitch == 1)
                    {
                        //l3 uses ints
                        while (l5Size > 0)
                        {
                            vg._l5Buffer.append(reverseInt(dis.ReadInt32()));
                            l5Size--;
                        }
                    }
                    else
                    {
                        while (l5Size > 0)
                        {
                            vg._l5Buffer.append(reverseInt((int)(dis.ReadInt64() >> 32)));
                            l5Size--;
                        }
                    }
                }
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="ba"></param>
        /// <param name="vg"></param>
        /// <returns></returns>
        public static void readIndex(byte[] ba, VTDGen vg)
        {
            if (ba == null || vg == null)
            {
                throw new System.ArgumentException("Invalid argument(s) for readIndex()");
            }
            //UPGRADE_TODO: Class 'java.io.DataInputStream' was converted to 'System.IO.BinaryReader' 
            //which has a different behavior. 
            //"ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1073_javaioDataInputStream'"
            System.IO.BinaryReader dis = new System.IO.BinaryReader(new System.IO.MemoryStream(ba));
            byte b = dis.ReadByte(); // first byte
            // no check on version number for now
            // second byte
            vg.encoding = dis.ReadByte();
            int adj = OFFSET_ADJUSTMENT;
            if (vg.encoding >= VTDGen.FORMAT_UTF_16BE)
            {
                adj = OFFSET_ADJUSTMENT >> 1;
            }
            int intLongSwitch;
            int endian;
            // third byte
            b = dis.ReadByte();
            if ((b & 0x80) != 0)
                intLongSwitch = 1;
            //use ints
            else
                intLongSwitch = 0;
            if ((b & 0x40) != 0)
                vg.ns = true;
            else
                vg.ns = false;
            if ((b & 0x20) != 0)
                endian = 1;
            else
                endian = 0;
            if ((b & 0x1f) != 0)
                throw new IndexReadException("Last 5 bits of the third byte should be zero");

            // fourth byte
            vg.VTDDepth = dis.ReadByte();

            // 5th and 6th byte
            int LCLevel = (((int)dis.ReadByte()) << 8) | dis.ReadByte();
            if (LCLevel != 4 && LCLevel != 6)
            {
                throw new IndexReadException("LC levels must be at least 3");
            }
            if (LCLevel == 4)
                vg.shallowDepth = true;
            else
                vg.shallowDepth = false;
            // 7th and 8th byte
            vg.rootIndex = (((int)dis.ReadByte()) << 8) | dis.ReadByte();

            // skip a long
            long l = dis.ReadInt64();
            //Console.WriteLine(" l ==>" + l);
            l = dis.ReadInt64();
            //Console.WriteLine(" l ==>" + l);
            l = dis.ReadInt64();
            //Console.WriteLine(" l ==>" + l);
            int size;
            // read XML size
            if (BitConverter.IsLittleEndian && endian == 0
                || BitConverter.IsLittleEndian == false && endian == 1)
                size = (int)l;
            else
                size = (int)reverseLong(l);


            // read XML bytes
            //byte[] XMLDoc = new byte[size];
            //dis.Read(XMLDoc, 0, size);
            vg.setDoc(ba, OFFSET_ADJUSTMENT, size);
            int t = 0;

            if ((size & 0x7) != 0)
            {
                t = (((size >> 3) + 1) << 3) - size;
            }

            dis = new System.IO.BinaryReader(new System.IO.MemoryStream(ba, 32 + size + t, ba.Length - 32 - size - t));

            if (BitConverter.IsLittleEndian && endian == 0
    || BitConverter.IsLittleEndian == false && endian == 1)
            {
                // read vtd records
                int vtdSize = (int)dis.ReadInt64();
                while (vtdSize > 0)
                {
                    vg.VTDBuffer.append(adjust(dis.ReadInt64(), adj));
                    vtdSize--;
                }
                // read L1 LC records
                int l1Size = (int)dis.ReadInt64();
                while (l1Size > 0)
                {
                    vg.l1Buffer.append(dis.ReadInt64());
                    l1Size--;
                }
                // read L2 LC records
                int l2Size = (int)dis.ReadInt64();
                while (l2Size > 0)
                {
                    vg.l2Buffer.append(dis.ReadInt64());
                    l2Size--;
                }
                // read L3 LC records
                int l3Size = (int)dis.ReadInt64();
                if (vg.shallowDepth)
                {
                    if (intLongSwitch == 1)
                    {
                        //l3 uses ints
                        while (l3Size > 0)
                        {
                            vg.l3Buffer.append(dis.ReadInt32());
                            l3Size--;
                        }
                    }
                    else
                    {
                        while (l3Size > 0)
                        {
                            vg.l3Buffer.append((int)(dis.ReadInt64()>>32));
                            l3Size--;
                        }
                    }
                }
                else
                {
                    while (l3Size > 0)
                    {
                        vg._l3Buffer.append(dis.ReadInt64());
                        l3Size--;
                    }

                    int l4Size = (int)dis.ReadInt64();
                    while (l4Size > 0)
                    {
                        vg._l4Buffer.append(dis.ReadInt64());
                        l4Size--;
                    }

                    int l5Size = (int)dis.ReadInt64();
                    if (intLongSwitch == 1)
                    {
                        while (l5Size > 0)
                        {
                            vg._l5Buffer.append(dis.ReadInt32());
                            l5Size--;
                        }
                    }
                    else
                    {
                        while (l5Size > 0)
                        {
                            vg._l5Buffer.append((int)(dis.ReadInt64()>>32));
                            l5Size--;
                        }
                    }
                }
            }
            else
            {
                // read vtd records
                int vtdSize = (int)reverseLong(dis.ReadInt64());
                while (vtdSize > 0)
                {
                    vg.VTDBuffer.append(adjust(reverseLong(dis.ReadInt64()), adj));
                    vtdSize--;
                }
                // read L1 LC records
                int l1Size = (int)reverseLong(dis.ReadInt64());
                while (l1Size > 0)
                {
                    vg.l1Buffer.append(reverseLong(dis.ReadInt64()));
                    l1Size--;
                }
                // read L2 LC records
                int l2Size = (int)reverseLong(dis.ReadInt64());
                while (l2Size > 0)
                {
                    vg.l2Buffer.append(reverseLong(dis.ReadInt64()));
                    l2Size--;
                }
                // read L3 LC records
                int l3Size = (int)reverseLong(dis.ReadInt64());
                if (vg.shallowDepth)
                {
                    if (intLongSwitch == 1)
                    {
                        //l3 uses ints
                        while (l3Size > 0)
                        {
                            vg.l3Buffer.append(reverseInt(dis.ReadInt32()));
                            l3Size--;
                        }
                    }
                    else
                    {
                        while (l3Size > 0)
                        {
                            vg.l3Buffer.append(reverseInt((int)(dis.ReadInt64() >> 32)));
                            l3Size--;
                        }
                    }
                }
                else
                {
                    while (l3Size > 0)
                    {
                        vg._l3Buffer.append(reverseLong(dis.ReadInt64()));
                        l3Size--;
                    }

                    int l4Size = (int)reverseLong(dis.ReadInt64());
                    {
                        vg._l4Buffer.append(reverseLong(dis.ReadInt64()));
                        l4Size--;
                    }

                    int l5Size = (int)reverseLong(dis.ReadInt64());
                    if (intLongSwitch == 1)
                    {
                        //l3 uses ints
                        while (l5Size > 0)
                        {
                            vg._l5Buffer.append(reverseInt(dis.ReadInt32()));
                            l5Size--;
                        }
                    }
                    else
                    {
                        while (l5Size > 0)
                        {
                            vg._l5Buffer.append(reverseInt((int)(dis.ReadInt64() >> 32)));
                            l5Size--;
                        }
                    }
                }

            }
        }

        private static long reverseLong(long l)
        {
            //UPGRADE_TODO: Literal detected as an unsigned long can generate compilation errors. "ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1175'"
            long t = (((l & -0x0100000000000000L) >> 56) & 0xffL)
                | ((l & 0xff000000000000L) >> 40)
                | ((l & 0xff0000000000L) >> 24)
                | ((l & 0xff00000000L) >> 8)
                | ((l & 0xff000000L) << 8)
                | ((l & 0xff0000L) << 24)
                | ((l & 0xff00L) << 40)
                | ((l & 0xffL) << 56);
            return t;
        }

        private static int reverseInt(int i)
        {
            int t = (((i & -0x01000000) >> 24) & 0xff)
                | ((i & 0xff0000) >> 8)
                | ((i & 0xff00) << 8)
                | ((i & 0xff) << 24);
            return t;
        }

        private static long adjust(long l, int i)
        {
            unchecked
            {
                long l1 = (l & 0xffffffffL) + i;
                long l2 = l & (long)0xffffffff00000000L;
                return l1 | l2;
            }
        }

        public static void readSeparateIndex(System.IO.Stream index, System.IO.Stream XMLBytes, int XMLSize, VTDGen vg)
        {
            if (index == null || vg == null || XMLBytes == null)
            {
                throw new System.ArgumentException("Invalid argument(s) for readIndex()");
            }
            //UPGRADE_TODO: Class 'java.io.DataInputStream' was converted to 'System.IO.BinaryReader' 
            //which has a different behavior. 
            //"ms-help://MS.VSCC.v80/dv_commoner/local/redirect.htm?index='!DefaultContextWindowIndex'&keyword='jlca1073_javaioDataInputStream'"
            System.IO.BinaryReader dis = new System.IO.BinaryReader(index);
            byte b = dis.ReadByte(); // first byte
            // no check on version number for now
            // second byte
            vg.encoding = dis.ReadByte();
            int intLongSwitch;
            int endian;
            // third byte
            b = dis.ReadByte();
            if ((b & 0x80) != 0)
                intLongSwitch = 1;
            //use ints
            else
                intLongSwitch = 0;
            if ((b & 0x40) != 0)
                vg.ns = true;
            else
                vg.ns = false;
            if ((b & 0x20) != 0)
                endian = 1;
            else
                endian = 0;
            if ((b & 0x1f) != 0)
                throw new IndexReadException("Last 5 bits of the third byte should be zero");

            // fourth byte
            vg.VTDDepth = dis.ReadByte();

            // 5th and 6th byte
            int LCLevel = (((int)dis.ReadByte()) << 8) | dis.ReadByte();
            if (LCLevel != 4 && LCLevel != 6)
            {
                throw new IndexReadException("LC levels must be at least 3");
            }
            // 7th and 8th byte
            vg.rootIndex = (((int)dis.ReadByte()) << 8) | dis.ReadByte();

            // skip a long
            dis.ReadInt64();
            //Console.WriteLine(" l ==>" + l);
            dis.ReadInt64();
            //Console.WriteLine(" l ==>" + l);
            long l = dis.ReadInt64();

            int size;
            // read XML size
            if (BitConverter.IsLittleEndian && endian == 0
                || BitConverter.IsLittleEndian == false && endian == 1)
                size = (int)l;
            else
                size = (int)reverseLong(l);


            // read XML bytes
            byte[] XMLDoc = new byte[size];
            XMLBytes.Read(XMLDoc, 0, size);

            //dis.Read(XMLDoc, 0, size);
            /*if ((size & 0x7) != 0)
            {
                int t = (((size >> 3) + 1) << 3) - size;
                while (t > 0)
                {
                    dis.ReadByte();
                    t--;
                }
            }*/

            vg.setDoc(XMLDoc);
            // skip a long
            dis.ReadInt64();
            //Console.WriteLine(" l ==>" + l);
            dis.ReadInt64();
            //Console.WriteLine(" l ==>" + l);
            if (BitConverter.IsLittleEndian && endian == 0
                || BitConverter.IsLittleEndian == false && endian == 1)
            {
                // read vtd records
                int vtdSize = (int)dis.ReadInt64();
                while (vtdSize > 0)
                {
                    vg.VTDBuffer.append(dis.ReadInt64());
                    vtdSize--;
                }
                // read L1 LC records
                int l1Size = (int)dis.ReadInt64();
                while (l1Size > 0)
                {
                    vg.l1Buffer.append(dis.ReadInt64());
                    l1Size--;
                }
                // read L2 LC records
                int l2Size = (int)dis.ReadInt64();
                while (l2Size > 0)
                {
                    vg.l2Buffer.append(dis.ReadInt64());
                    l2Size--;
                }
                // read L3 LC records
                int l3Size = (int)dis.ReadInt64();
                if (vg.shallowDepth)
                {
                    if (intLongSwitch == 1)
                    {
                        //l3 uses ints
                        while (l3Size > 0)
                        {
                            vg.l3Buffer.append(dis.ReadInt32());
                            l3Size--;
                        }
                    }
                    else
                    {
                        while (l3Size > 0)
                        {
                            vg.l3Buffer.append((int)(dis.ReadInt64() >> 32));
                            l3Size--;
                        }
                    }
                }
                else
                {
                    while (l3Size > 0)
                    {
                        vg._l3Buffer.append(dis.ReadInt64());
                        l3Size--;
                    }

                    int l4Size = (int)dis.ReadInt64();
                    while (l4Size > 0)
                    {
                        vg._l4Buffer.append(dis.ReadInt64());
                        l4Size--;
                    }

                    int l5Size = (int)dis.ReadInt64();
                    if (intLongSwitch == 1)
                    {
                        while (l5Size > 0)
                        {
                            vg._l5Buffer.append(dis.ReadInt32());
                            l5Size--;
                        }
                    }
                    else
                    {
                        while (l5Size > 0)
                        {
                            vg._l5Buffer.append((int)(dis.ReadInt64() >> 32));
                            l5Size--;
                        }
                    }
                }
            }
            else
            {
                // read vtd records
                int vtdSize = (int)reverseLong(dis.ReadInt64());
                while (vtdSize > 0)
                {
                    vg.VTDBuffer.append(reverseLong(dis.ReadInt64()));
                    vtdSize--;
                }
                // read L1 LC records
                int l1Size = (int)reverseLong(dis.ReadInt64());
                while (l1Size > 0)
                {
                    vg.l1Buffer.append(reverseLong(dis.ReadInt64()));
                    l1Size--;
                }
                // read L2 LC records
                int l2Size = (int)reverseLong(dis.ReadInt64());
                while (l2Size > 0)
                {
                    vg.l2Buffer.append(reverseLong(dis.ReadInt64()));
                    l2Size--;
                }
                // read L3 LC records
                int l3Size = (int)reverseLong(dis.ReadInt64());
                if (vg.shallowDepth)
                {
                    if (intLongSwitch == 1)
                    {
                        //l3 uses ints
                        while (l3Size > 0)
                        {
                            vg.l3Buffer.append(reverseInt(dis.ReadInt32()));
                            l3Size--;
                        }
                    }
                    else
                    {
                        while (l3Size > 0)
                        {
                            vg.l3Buffer.append(reverseInt((int)(dis.ReadInt64() >> 32)));
                            l3Size--;
                        }
                    }
                }
                else
                {
                    while (l3Size > 0)
                    {
                        vg._l3Buffer.append(reverseLong(dis.ReadInt64()));
                        l3Size--;
                    }

                    int l4Size = (int)reverseLong(dis.ReadInt64());
                    {
                        vg._l4Buffer.append(reverseLong(dis.ReadInt64()));
                        l4Size--;
                    }

                    int l5Size = (int)reverseLong(dis.ReadInt64());
                    if (intLongSwitch == 1)
                    {
                        //l3 uses ints
                        while (l5Size > 0)
                        {
                            vg._l5Buffer.append(reverseInt(dis.ReadInt32()));
                            l5Size--;
                        }
                    }
                    else
                    {
                        while (l5Size > 0)
                        {
                            vg._l5Buffer.append(reverseInt((int)(dis.ReadInt64() >> 32)));
                            l5Size--;
                        }
                    }
                }
            }
        }

        public static void writeSeparateIndex_L3(byte version,
            int encodingType,
            bool ns,
            bool byteOrder, // true is big endien
            int nestDepth,
            int LCLevel,
            int rootIndex,
            //byte[] xmlDoc,
            int docOffset,
            int docLen,
            FastLongBuffer vtdBuffer,
            FastLongBuffer l1Buffer,
            FastLongBuffer l2Buffer,
            FastIntBuffer l3Buffer,
            System.IO.Stream os
            )
        {
            if (docLen <= 0
                || vtdBuffer == null
                || l1Buffer == null
                || l2Buffer == null
                || l3Buffer == null
                || LCLevel != 3)
            {
                throw new System.ArgumentException("Invalid argument for writeIndex ");
            }
            if (vtdBuffer.size_Renamed_Field == 0)
            {
                throw new IndexWriteException("VTDBuffer can't be zero length");
            }
            int i;

            System.IO.BinaryWriter dos = new System.IO.BinaryWriter(os);
            // first 4 bytes
            byte[] ba = new byte[4];
            ba[0] = (byte)version; // version # is 1 
            ba[1] = (byte)encodingType;
            if (BitConverter.IsLittleEndian == false)
                ba[2] = (byte)(ns ? 0xe0 : 0xa0); // big endien
            else
                ba[2] = (byte)(ns ? 0xc0 : 0x80);
            ba[3] = (byte)nestDepth;
            dos.Write(ba);
            // second 4 bytes
            ba[0] = 0;
            ba[1] = 4;
            ba[2] = (byte)((rootIndex & 0xff00) >> 8);
            ba[3] = (byte)(rootIndex & 0xff);
            dos.Write(ba);
            // 2 reserved 32-bit words set to zero
            ba[1] = ba[2] = ba[3] = 0;
            dos.Write(ba);
            dos.Write(ba);
            dos.Write(ba);
            dos.Write(ba);
            // write XML doc in bytes
            dos.Write((long)docLen);
            dos.Write(ba);
            dos.Write(ba);
            dos.Write(ba);
            dos.Write(ba);
            //dos.Write(xmlDoc, docOffset, docLen);
            // zero padding to make it integer multiple of 64 bits
            /*if ((docLen & 0x07) != 0)
            {
                int t = (((docLen >> 3) + 1) << 3) - docLen;
                for (; t > 0; t--)
                    dos.Write((System.Byte)0);
            }*/
            // write VTD

            dos.Write((long)vtdBuffer.size_Renamed_Field);
            if (docOffset != 0)
            {
                for (i = 0; i < vtdBuffer.size_Renamed_Field; i++)
                {
                    dos.Write(adjust(vtdBuffer.longAt(i), -docOffset));
                }
            }
            else
            {
                for (i = 0; i < vtdBuffer.size_Renamed_Field; i++)
                {
                    dos.Write(vtdBuffer.longAt(i));
                }
            }
            // write L1 
            dos.Write((long)l1Buffer.size_Renamed_Field);
            for (i = 0; i < l1Buffer.size_Renamed_Field; i++)
            {
                dos.Write(l1Buffer.longAt(i));
            }
            // write L2
            dos.Write((long)l2Buffer.size_Renamed_Field);
            for (i = 0; i < l2Buffer.size_Renamed_Field; i++)
            {
                dos.Write(l2Buffer.longAt(i));
            }
            // write L3
            dos.Write((long)l3Buffer.size_Renamed_Field);
            for (i = 0; i < l3Buffer.size_Renamed_Field; i++)
            {
                dos.Write(l3Buffer.intAt(i));
            }
            // pad zero if # of l3 entry is odd
            if ((l3Buffer.size_Renamed_Field & 1) != 0)
                dos.Write(0);
            dos.Close();
        }

        public static void writeSeparateIndex_L5(byte version,
    int encodingType,
    bool ns,
    bool byteOrder, // true is big endien
    int nestDepth,
    int LCLevel,
    int rootIndex,
            //byte[] xmlDoc,
    int docOffset,
    int docLen,
    FastLongBuffer vtdBuffer,
    FastLongBuffer l1Buffer,
    FastLongBuffer l2Buffer,
    FastLongBuffer l3Buffer,
    FastLongBuffer l4Buffer,
    FastIntBuffer l5Buffer,
    System.IO.Stream os
    )
        {
            if (docLen <= 0
                || vtdBuffer == null
                || l1Buffer == null
                || l2Buffer == null
                || l3Buffer == null
                || LCLevel != 5)
            {
                throw new System.ArgumentException("Invalid argument for writeIndex ");
            }
            if (vtdBuffer.size_Renamed_Field == 0)
            {
                throw new IndexWriteException("VTDBuffer can't be zero length");
            }
            int i;

            System.IO.BinaryWriter dos = new System.IO.BinaryWriter(os);
            // first 4 bytes
            byte[] ba = new byte[4];
            ba[0] = (byte)version; // version # is 1 
            ba[1] = (byte)encodingType;
            if (BitConverter.IsLittleEndian == false)
                ba[2] = (byte)(ns ? 0xe0 : 0xa0); // big endien
            else
                ba[2] = (byte)(ns ? 0xc0 : 0x80);
            ba[3] = (byte)nestDepth;
            dos.Write(ba);
            // second 4 bytes
            ba[0] = 0;
            ba[1] = 6;
            ba[2] = (byte)((rootIndex & 0xff00) >> 8);
            ba[3] = (byte)(rootIndex & 0xff);
            dos.Write(ba);
            // 2 reserved 32-bit words set to zero
            ba[1] = ba[2] = ba[3] = 0;
            dos.Write(ba);
            dos.Write(ba);
            dos.Write(ba);
            dos.Write(ba);
            // write XML doc in bytes
            dos.Write((long)docLen);
            dos.Write(ba);
            dos.Write(ba);
            dos.Write(ba);
            dos.Write(ba);
            //dos.Write(xmlDoc, docOffset, docLen);
            // zero padding to make it integer multiple of 64 bits
            /*if ((docLen & 0x07) != 0)
            {
                int t = (((docLen >> 3) + 1) << 3) - docLen;
                for (; t > 0; t--)
                    dos.Write((System.Byte)0);
            }*/
            // write VTD

            dos.Write((long)vtdBuffer.size_Renamed_Field);
            if (docOffset != 0)
            {
                for (i = 0; i < vtdBuffer.size_Renamed_Field; i++)
                {
                    dos.Write(adjust(vtdBuffer.longAt(i), -docOffset));
                }
            }
            else
            {
                for (i = 0; i < vtdBuffer.size_Renamed_Field; i++)
                {
                    dos.Write(vtdBuffer.longAt(i));
                }
            }
            // write L1 
            dos.Write((long)l1Buffer.size_Renamed_Field);
            for (i = 0; i < l1Buffer.size_Renamed_Field; i++)
            {
                dos.Write(l1Buffer.longAt(i));
            }
            // write L2
            dos.Write((long)l2Buffer.size_Renamed_Field);
            for (i = 0; i < l2Buffer.size_Renamed_Field; i++)
            {
                dos.Write(l2Buffer.longAt(i));
            }
            // write L3
            dos.Write((long)l3Buffer.size_Renamed_Field);
            for (i = 0; i < l3Buffer.size_Renamed_Field; i++)
            {
                dos.Write(l3Buffer.longAt(i));
            }
            // write L4
            dos.Write((long)l4Buffer.size_Renamed_Field);
            for (i = 0; i < l4Buffer.size_Renamed_Field; i++)
            {
                dos.Write(l4Buffer.longAt(i));
            }
            // write L5
            dos.Write((long)l5Buffer.size_Renamed_Field);
            for (i = 0; i < l5Buffer.size_Renamed_Field; i++)
            {
                dos.Write(l5Buffer.intAt(i));
            }
            // pad zero if # of l3 entry is odd
            if ((l5Buffer.size_Renamed_Field & 1) != 0)
                dos.Write(0);
            dos.Close();
        }
    }
}