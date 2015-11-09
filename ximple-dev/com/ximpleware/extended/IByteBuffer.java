/* 
 * Copyright (C) 2002-2015 XimpleWare, info@ximpleware.com
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
/*VTD-XML is protected by US patent 7133857, 7260652, an 7761459*/
package com.ximpleware.extended;

import java.io.IOException;

/**
 * Abstract XML document storage interface.
 */
public interface IByteBuffer {

	/**
	 * Get the byte at the given index of the underlying byte storage.
	 * @return byte
	 */
	byte byteAt(long index);
	/**
	 * Return a byte array filled with content from underlying byte storage.
	 * @return byte[]
	 * @param offset int  bytes offset (not UTF char unit)
	 * @param len int
	 */
	byte[] getBytes(int offset, int len);
	/**
	 * Total size in terms of # of bytes.
	 * @return long
	 */
	long length();
	// get the whole XML 
	byte[] getBytes();
	/**
	 * If teh underlying byte representation is IO related, close the IO stream
	 * @throws IOException 
	 */
	void close();
	void writeToFileOutputStream(java.io.FileOutputStream ost, long offset, long length) throws java.io.IOException;
}
