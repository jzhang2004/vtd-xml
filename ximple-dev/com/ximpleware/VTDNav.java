package com.ximpleware;

/* 
 * Copyright (C) 2002-2004 XimpleWare, info@ximpleware.com
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

import com.ximpleware.NavException;
import com.ximpleware.parser.UTF8Char;
/**
 *
 * To change the template for this generated type comment go to
 * Window&gt;Preferences&gt;Java&gt;Code Generation&gt;Code and Comments
 */
/**
 * VTD navigator class implementation
 */
public class VTDNav {
	// Navigation directions
	public final static int ROOT = 0;
	public final static int PARENT = 1;
	public final static int FIRST_CHILD = 2;
	public final static int LAST_CHILD = 3;
	public final static int NEXT_SIBLING = 4;
	public final static int PREV_SIBLING = 5;

	// Navigation directions
	public final static int R = 0;
	public final static int P = 1;
	public final static int FC = 2;
	public final static int LC = 3;
	public final static int NS = 4;
	public final static int PS = 5;

	// token type definitions
	public final static int TOKEN_STARTING_TAG = 0;
	public final static int TOKEN_ENDING_TAG = 1;
	public final static int TOKEN_ATTR_NAME = 2;
	public final static int TOKEN_ATTR_NS = 3;
	public final static int TOKEN_ATTR_VAL = 4;
	public final static int TOKEN_CHARACTER_DATA = 5;
	public final static int TOKEN_COMMENT = 6;
	public final static int TOKEN_PI_NAME = 7;
	public final static int TOKEN_PI_VAL = 8;
	public final static int TOKEN_DEC_ATTR_NAME = 9;
	public final static int TOKEN_DEC_ATTR_VAL = 10;
	public final static int TOKEN_CDATA_VAL = 11;
	public final static int TOKEN_DTD_VAL = 12;

	// encoding format definition here
	public final static int FORMAT_UTF8 = 2;
	public final static int FORMAT_ASCII = 0;
	public final static int FORMAT_UTF_16LE = 4;
	public final static int FORMAT_UTF_16BE = 3;
	public final static int FORMAT_ISO_8859 = 1;

	// masks for obtaining various fields from a VTD token 
	private final static long MASK_TOKEN_FULL_LEN = 0x000fffff00000000L;
	private final static long MASK_TOKEN_PRE_LEN = 0x000ff80000000000L;
	private final static long MASK_TOKEN_QN_LEN = 0x000007ff00000000L;
	private final static long MASK_TOKEN_OFFSET = 0x000000003fffffffL;
	private final static long MASK_TOKEN_TYPE = 0xf000000000000000L;
	private final static long MASK_TOKEN_DEPTH = 0x0ff0000000000000L;

	// tri-state variable for namespace lookup
	private final static long MASK_TOKEN_NS_MARK = 0x00000000c0000000L;

	protected int rootIndex; // where the root element is at
	protected int nestingLevel;
	protected int[] context; // main navigation tracker aka context object

	// location cache part
	protected int l2upper;
	protected int l2lower;
	protected int l3upper;
	protected int l3lower;
	protected int l2index;
	protected int l3index;
	protected int l1index;

	// containers
	protected ILongBuffer vtdBuffer;
	protected ILongBuffer l1Buffer;
	protected ILongBuffer l2Buffer;
	protected IIntBuffer l3Buffer;
	private IByteBuffer XMLDoc;

	//private int recentNS; // most recently visited NS node, experiment for now
	// Hierarchical representation is an array of integers addressing elements tokens 
	private ContextBuffer2 contextStack;

	// the document encoding	     
	private int encoding;
	// for string to token comparison
	private int currentOffset;

	// whether the navigation is namespace enabled or not. 
	private boolean ns;

	// intermediate buffer for push and pop purposes  
	private int[] stackTemp;
	protected int offset;
	// length of the document
	protected int docLen;
	protected int vtdSize; //vtd record count
	/**
	 * Initialize the VTD navigation object.
	 * @param RootIndex int
	 * @param maxDepth int
	 * @param encoding int
	 * @param NS  boolean
	 * @param x byte[]
	 * @param vtd com.ximpleware.ILongBuffer
	 * @param l1 com.ximpleware.ILongBuffer
	 * @param l2 com.ximpleware.ILongBuffer
	 * @param l3 com.ximpleware.IIntBuffer
	 * @param so int  starting offset of the document(in byte)
	 * @param length int length of the document (in byte)
	 */
	protected VTDNav(
		int RootIndex,
		int enc,
		boolean NS,
		int depth,
		IByteBuffer x,
		ILongBuffer vtd,
		ILongBuffer l1,
		ILongBuffer l2,
		IIntBuffer l3,
		int so,
	// start offset of the starting offset(in byte) 
	int length) // lengnth of the XML document (in byte)) 
	{
		// initialize all buffers
		if (l1 == null
			|| l2 == null
			|| l3 == null
			|| vtd == null
			|| x == null
			|| depth < 0
			|| RootIndex < 0 //|| encoding <= FORMAT_UTF8
			//|| encoding >= FORMAT_ISO_8859_1
			|| so < 0
			|| length < 0) {
			throw new IllegalArgumentException();
		}

		l1Buffer = l1;
		l2Buffer = l2;
		l3Buffer = l3;
		vtdBuffer = vtd;
		XMLDoc = x;

		encoding = enc;
		//System.out.println("encoding " + encoding);
		rootIndex = RootIndex;
		nestingLevel = depth + 1;
		ns = NS;

		// initialize the context object
		context = new int[nestingLevel];
		//depth value is the first entry in the context because root is singular.
		context[0] = 0;
		//set the value to zero
		for (int i = 1; i < nestingLevel; i++) {
			context[i] = -1;
		}
		currentOffset = 0;
		//contextStack = new ContextBuffer(1024, nestingLevel + 7);
		contextStack = new ContextBuffer2(10, nestingLevel + 7);
		stackTemp = new int[nestingLevel + 7];

		// initial state of LC variables
		l1index = l2index = l3index = -1;
		l2lower = l3lower = -1;
		l2upper = l3upper = -1;
		offset = so;
		docLen = length;
		//System.out.println("offset " + offset + "  length " + length);
		//printL2Buffer();
		vtdSize = vtd.size();

		//recentNS = -1;
	}
	/**
	 * This method print out the current state info of the navigation object.
	 * It is for debugging purpose.
	 */
	public void dumpContext() {
		for (int i = 0; i < context.length; i++) {
			System.out.println("context[" + i + "] ==>" + context[i]);
		}
		try {
			System.out.println(
				"element name -->" + toRawString(getCurrentIndex()));
		} catch (NavException e) {
			System.out.println("toString exception condition occurred " + e);
		}
	}
	/**
	 * Return the attribute count of the element at the cursor position.
	 * when ns is false, ns tokens are considered attributes;
	 * otherwise, ns tokens are not considered attributes
	 * @return int
	 */
	public int getAttrCount() {
		int count = 0;
		int index = getCurrentIndex() + 1;
		while (index < vtdSize) {
			int type = getTokenType(index);
			if (type == TOKEN_ATTR_NAME
				|| type == TOKEN_ATTR_VAL
				|| type == TOKEN_ATTR_NS) {
				if (type == TOKEN_ATTR_NAME
					|| (!ns && (type == TOKEN_ATTR_NS))) {
					count++;
				}
			} else
				break;
			index++;
		}
		return count;
	}
	/**
	 * Get the token index of the attribute value given an attribute name.     
	 * @return int  (-1 if no such attribute name exists)
	 * @param an java.lang.String
	 * @exception com.ximpleware.NavException The exception if the underlying byte 
	 * content contains various errors. Notice that we are being conservative in making little assumption on
	 * the correctness of underlying byte content. This is because the VTD etc can be generated by another
	 * machine from a load-balancer.
	 * @exception IllegalArguementException if an is null
	 */
	public int getAttrVal(String an) throws NavException {
		//int size = vtdBuffer.size();
		int index = (context[0] != 0) ? context[context[0]] + 1 : rootIndex + 1;
		
		int type;
		if (index<vtdSize)
		   type= getTokenType(index);
		else
			return -1;
		if (ns == false) {
			while ((type == TOKEN_ATTR_NAME || type == TOKEN_ATTR_NS)) {
				if (matchRawTokenString(index,
					an)) { // ns node visible only ns is disabled
					return index + 1;
				}
				index += 2;
				if (index >= vtdSize)
					break;
				type = getTokenType(index);
			}
		} else {
			while ((type == TOKEN_ATTR_NAME || type == TOKEN_ATTR_NS)) {
				if (type == TOKEN_ATTR_NAME
					&& matchRawTokenString(
						index,
						an)) { // ns node visible only ns is disabled
					return index + 1;
				}
				index += 2;
				if (index>=vtdSize)
					break;
				type = getTokenType(index);
			}
		}
		return -1;
	}
	/**
	 * Get the token index of the attribute value of given URL and local name.
	 * If ns is not enabled, the lookup will return -1, indicating a no-found.
	 * Also namespace nodes are invisible using this method.
	 * One can't use * to indicate any name space because * is ambiguous!!
	 * @return int (-1 if no matching attribute found)
	 * @param URL java.lang.String  (Name space URL)
	 * @param ln java.lang.String   (local name)
	 * @exception com.ximpleware.NavException The exception if the underlying byte 
	 * content contains various errors. Notice that we are being conservative in making little assumption on
	 * the correctness of underlying byte content. This is because the VTD etc can be generated by another
	 * machine from a load-balancer.
	 * @exception IllegalArguementException if s is null
	 */
	public int getAttrValNS(String URL, String ln) throws NavException {
		if (ns == false)
			return -1;
		if (URL == null)
			return getAttrVal(ln);
		int size = vtdBuffer.size();
		int index = (context[0] != 0) ? context[context[0]] + 1 : rootIndex + 1;
		// point to the token next to the element tag
		int type;
		if (index<vtdSize)
			type = getTokenType(index);
		else 
			return -1;
		while (index < size
			&& (type == TOKEN_ATTR_NAME || type == TOKEN_ATTR_NS)) {
			int i = getTokenLength(index);
			int offset = getTokenOffset(index);
			int preLen = (i >> 16) & 0xffff;
			int fullLen = i & 0xffff;
			if (preLen != 0
				// attribute name without a prefix is not bound to any namespaces
				&& matchRawTokenString(
					offset + preLen + 1,
					fullLen - preLen - 1,
					ln)
				&& resolveNS(URL, offset, preLen)) {
				return index + 1;
			}
			index += 2;
			if (index>=vtdSize)
				break;
			type = getTokenType(index);
		}
		return -1;
	}
	/**
	 * This method decodes the underlying byte array into corresponding UCS2 char representation .
	 * It doesn't resolves built-in entity and character references.
	 * Length will never be zero
	 * Creation date: (11/21/03 6:26:17 PM)
	 * @return int
	 * @exception com.ximpleware.NavException The exception is thrown if the underlying byte 
	 * content contains various errors. Notice that we are being conservative in making little assumption on
	 * the correctness of underlying byte content. This is because the VTD can be generated by another
	 * machine, e.g. from a load-balancer.
	 */
	private int getChar() throws NavException {
		int temp = 0;
		int a, c, d;
		int val;
		int ch;
		a = c = d = val = 0;

		switch (encoding) {
			case FORMAT_ASCII : // ascii is compatible with UTF-8, the offset value is bytes
				temp = XMLDoc.byteAt(currentOffset);
				if (temp == '\r') {
					if (XMLDoc.byteAt(currentOffset + 1) == '\n') {
						currentOffset += 2;
						return '\n';
					} else {
						currentOffset++;
						return '\n';
					}
				}
				currentOffset++;
				return temp;
			case FORMAT_UTF8 :
				temp = XMLDoc.byteAt(currentOffset) & 0xff;

				switch (UTF8Char.byteCount(temp)) {
					case 1 :
						if (temp == '\r') {
							if (XMLDoc.byteAt(currentOffset + 1) == '\n') {
								currentOffset += 2;
								return '\n';
							} else {
								currentOffset++;
								return '\n';
							}
						}
						currentOffset++;
						return temp;
					case 2 :
						c = 0x1f;
						d = 6;
						a = 1;
						break;
					case 3 :
						c = 0x0f;
						d = 12;
						a = 2;
						break;
					case 4 :
						c = 0x07;
						d = 18;
						a = 3;
						break;
					case 5 :
						c = 0x03;
						d = 24;
						a = 4;
						break;
					case 6 :
						c = 0x01;
						d = 30;
						a = 5;
						break;
					default :
						throw new NavException("UTF 8 encoding error: should never happen");
				}

				val = (temp & c) << d;
				int i = a - 1;
				while (i >= 0) {
					temp = XMLDoc.byteAt(currentOffset + a - i);
					if ((temp & 0xc0) != 0x80)
						throw new NavException("UTF 8 encoding error: should never happen");
					val = val | ((temp & 0x3f) << ((i<<2)+(i<<1)));
					i--;
				}
				currentOffset += a + 1;
				return val;

			case FORMAT_ISO_8859 :
				temp = XMLDoc.byteAt(currentOffset);
				if (temp == '\r') {
					if (XMLDoc.byteAt(currentOffset + 1) == '\n') {
						currentOffset += 2;
						return '\n';
					} else {
						currentOffset++;
						return '\n';
					}
				}
				currentOffset++;
				return temp & 0xff;

			case FORMAT_UTF_16BE :
				// implement UTF-16BE to UCS4 conversion
				temp =
					((XMLDoc.byteAt(currentOffset << 1) & 0xff)	<< 8) 
							|(XMLDoc.byteAt((currentOffset << 1) + 1)& 0xff);
				if ((temp < 0xd800)
					|| (temp > 0xdfff)) { // not a high surrogate
					if (temp == '\r') {
						if (XMLDoc.byteAt((currentOffset << 1) + 3) == '\n'
							&& XMLDoc.byteAt((currentOffset << 1) + 2) == 0) {
							currentOffset += 2;
							return '\n';
						} else {
							currentOffset++;
							return '\n';
						}
					}
					currentOffset++;
					return temp;
				} else {
					if (temp<0xd800 || temp>0xdbff)				
						throw new NavException("UTF 16 BE encoding error: should never happen");
					val = temp;
					temp =
						((XMLDoc.byteAt((currentOffset << 1) + 2) & 0xff)
							<< 8) | (XMLDoc.byteAt((currentOffset << 1 )+ 3) & 0xff);
					if (temp < 0xdc00 || temp > 0xdfff) {
						// has to be a low surrogate here
						throw new NavException("UTF 16 BE encoding error: should never happen");
					}
					val = ((temp - 0xd800) << 10) + (val - 0xdc00) + 0x10000;
					currentOffset += 2;
					return val;
				}

			case FORMAT_UTF_16LE :
				// implement UTF-16LE to UCS4 conversion
				temp =
					(XMLDoc.byteAt((currentOffset << 1) + 1 ) & 0xff)
						<< 8 | (XMLDoc.byteAt(currentOffset << 1) & 0xff);
				if (temp < 0xdc00 || temp > 0xdfff) { // check for low surrogate
					if (temp == '\r') {
						if (XMLDoc.byteAt((currentOffset << 1) + 2) == '\n'
							&& XMLDoc.byteAt((currentOffset << 1) + 3) == 0) {
							currentOffset += 2;
							return '\n';
						} else {
							currentOffset++;
							return '\n';
						}
					}
					currentOffset++;
					return temp;
				} else {
					if (temp<0xd800 || temp>0xdbff)				
						throw new NavException("UTF 16 LE encoding error: should never happen");
					val = temp;
					temp =
						(XMLDoc.byteAt((currentOffset << 1) + 3)&0xff)
							<< 8 | (XMLDoc.byteAt((currentOffset << 1) + 2) & 0xff);
					if (temp < 0xdc00 || temp > 0xdfff) {
						// has to be high surrogate
						throw new NavException("UTF 16 LE encoding error: should never happen");
					}
					val = ((temp - 0xd800)<<10) + (val - 0xdc00) + 0x10000;
					currentOffset += 2;
					return val;
				}
				//System.out.println("UTF 16 LE unimplemented for now");

			default :
				throw new NavException("Unknown Encoding");
		}
	}
	/**
	 * This method decodes the underlying byte array into corresponding UCS2 char representation .
	 * Also it resolves built-in entity and character references.
	 * @return int
	 * @exception com.ximpleware.NavException The exception is thrown if the underlying byte 
	 * content contains various errors. Notice that we are being conservative in making little assumption on
	 * the correctness of underlying byte content. This is because the VTD can be generated by another
	 * machine from a load-balancer.
	 */
	private int getCharResolved() throws NavException {
		int ch = 0;
		int val = 0;
		ch = getChar();
		if (ch != '&')
			return ch;

		// let us handle references here
		//currentOffset++;
		ch = getCharUnit(currentOffset);
		currentOffset++;
		switch (ch) {
			case '#' :

				ch = getCharUnit(currentOffset);

				if (ch == 'x') {
					while (true) {
						currentOffset++;
						ch = getCharUnit(currentOffset);

						if (ch >= '0' && ch <= '9') {
							val = (val << 4) + (ch - '0');
						} else if (ch >= 'a' && ch <= 'f') {
							val = (val << 4) + (ch - 'a' + 10);
						} else if (ch >= 'A' && ch <= 'F') {
							val = (val << 4) + (ch - 'A' + 10);
						} else if (ch == ';') {
							currentOffset++;
							break;
						} else
							throw new NavException("Illegal char in a char reference");
					}
				} else {
					while (true) {

						ch = getCharUnit(currentOffset);

						if (ch >= '0' && ch <= '9') {
							val = val * 10 + (ch - '0');
						} else if (ch == ';') {
							currentOffset++;
							break;
						} else
							throw new NavException("Illegal char in char reference");
						currentOffset++;
					}
				}
				break;

			case 'a' :
				ch = getCharUnit(currentOffset);
				if (ch == 'm') {
					if (getCharUnit(currentOffset + 1) == 'p'
						&& getCharUnit(currentOffset + 2) == ';') {
						currentOffset += 3;
						val = '&';
					} else
						throw new NavException("illegal builtin reference");
				} else if (ch == 'p') {
					if (getCharUnit(currentOffset + 1) == 'o'
						&& getCharUnit(currentOffset + 2) == 's'
						&& getCharUnit(currentOffset + 3) == ';') {
						currentOffset += 4;
						val = '\'';
					} else
						throw new NavException("illegal builtin reference");
				} else
					throw new NavException("illegal builtin reference");
				break;

			case 'q' :

				if (getCharUnit(currentOffset) == 'u'
					&& getCharUnit(currentOffset + 1) == 'o'
					&& getCharUnit(currentOffset + 2) == 't'
					&& getCharUnit(currentOffset + 3) == ';') {
					currentOffset += 4;
					val = '\"';
				} else
					throw new NavException("illegal builtin reference");
				break;
			case 'l' :
				if (getCharUnit(currentOffset) == 't'
					&& getCharUnit(currentOffset + 1) == ';') {
					currentOffset += 2;
					val = '<';
				} else
					throw new NavException("illegal builtin reference");
				break;
			case 'g' :
				if (getCharUnit(currentOffset) == 't'
					&& getCharUnit(currentOffset + 1) == ';') {
					currentOffset += 2;
					val = '>';
				} else
					throw new NavException("illegal builtin reference");
				break;

			default :
				throw new NavException("Invalid entity char");

		}

		//currentOffset++;
		return val;
	}
	/**
	 * Get the next char unit which gets decoded automatically
	 * @return int
	 */
	private int getCharUnit(int offset) {
		return (encoding < 3)
			? XMLDoc.byteAt(offset) & 0xff
			: (encoding == FORMAT_UTF_16BE)
			? (XMLDoc.byteAt(offset << 1)
				<< 8 | XMLDoc.byteAt((offset << 1) + 1))
			: (XMLDoc.byteAt((offset << 1) + 1)
				<< 8 | XMLDoc.byteAt(offset << 1));
	}
	/**
	 * Get the depth (>=0) of the current element.
	 * Creation date: (11/16/03 6:58:22 PM)
	 * @return int
	 */
	public int getCurrentDepth() {
		return context[0];
	}
	/**
	 * Get the index value of the current element.
	 * Creation date: (11/16/03 6:40:25 PM)
	 * @return int
	 */
	public int getCurrentIndex() {
		return (context[0] == 0) ? rootIndex : context[context[0]];
	}
	/**
	 * Get the starting offset and length of an element
	 * encoded in a long, upper 32 bit is length; lower 32 bit is offset
	 * Unit is in byte.
	 * Creation date: (3/15/04 1:47:55 PM)
	 */
	public long getElementFragment() throws NavException {
		// a little scanning is needed
		// has next sibling case
		// if not
		int depth = getCurrentDepth();
		int so = getTokenOffset(getCurrentIndex()) - 1;
		int length = 0;

		// for an element with next sibling
		if (toElement(NEXT_SIBLING)) {

			int temp = getCurrentIndex();
			// rewind 
			while (getTokenDepth(temp) < depth) {
				temp--;
			}
			//temp++;
			int so2 = getTokenOffset(temp) - 1;
			// look for the first '>'
			while (getCharUnit(so2) != '>') {
				so2--;
			}
			length = so2 - so + 1;
			toElement(PREV_SIBLING);
			if (encoding < 3)
				return ((long) length) << 32 | so;
			else
				return ((long) length) << 33 | (so << 1);
		}

		// for root element
		if (depth == 0) {
			int temp = vtdBuffer.size() - 1;
			boolean b = false;
			int so2 = 0;
			while (getTokenDepth(temp) == -1) {
				temp--; // backward scan
				b = true;
			}
			if (b == false)
				so2 =
					(encoding < 3)
						? (offset + docLen - 1)
						: ((offset + docLen) << 1) - 1;
			else
				so2 = getTokenOffset(temp + 1);
			while (getCharUnit(so2) != '>') {
				so2--;
			}
			length = so2 - so + 1;
			if (encoding < 3)
				return ((long) length) << 32 | so;
			else
				return ((long) length) << 33 | (so << 1);
		}
		// for a non-root element with no next sibling
		int temp = getCurrentIndex() + 1;
		int size = vtdBuffer.size();
		// temp is not the last entry in VTD buffer
		if (temp < size - 1) {
			while (temp < size && getTokenDepth(temp) >= depth) {
				temp++;
			}
			if (temp != size) {
				int d =
					depth
						- getTokenDepth(temp)
						+ ((getTokenType(temp) == TOKEN_STARTING_TAG) ? 1 : 0);
				int so2 = getTokenOffset(temp) - 1;
				int i = 0;
				// scan backward
				while (i < d) {
					if (getCharUnit(so2) == '>')
						i++;
					so2--;
				}
				length = so2 - so + 2;
				if (encoding < 3)
					return ((long) length) << 32 | so;
				else
					return ((long) length) << 33 | (so << 1);
			}
			/*
			int so2 = getTokenOffset(temp - 1) - 1;
			int d = depth - getTokenDepth(temp - 1);
			int i = 0;
			while (i < d) {
			    if (getCharUnit(so2) == '>') {
			        i++;
			    }
			    so2--;
			}
			length = so2 - so + 2;
			if (encoding < 3)
			    return ((long) length) << 32 | so;
			else
			    return ((long) length) << 33 | (so << 1);*/
		}
		// temp is the last entry
		// scan forward search for /> or </cc>
		int so2 =
			(encoding < 3)
				? (offset + docLen - 1)
				: ((offset + docLen) << 1) - 1;
		int d = depth + 1;
		int i = 0;
		while (i < d) {
			if (getCharUnit(so2) == '>') {
				i++;
			}
			so2--;
		}

		length = so2 - so + 2;

		if (encoding < 3)
			return ((long) length) << 32 | so;
		else
			return ((long) length) << 33 | (so << 1);
	}
	/**
	 * Get the encoding of the XML document.
	 * <pre>   0  ASCII       </pre>
	 * <pre>   1  ISO-8859-1  </pre>
	 * <pre>   2  UTF-8       </pre>
	 * <pre>   3  UTF-16BE    </pre>
	 * <pre>   4  UTF-16LE    </pre>
	 * @return int
	 */
	public int getEncoding() {
		return encoding;
	}
	/**
	 * Get the maximum nesting depth of the XML document (>0).
	 * max depth is nestingLevel -1
	 * @return int
	 */
	public int getNestingLevel() {
		return nestingLevel;
	}
	/**
	 * Get root index value.
	 * @return int
	 */
	public int getRootIndex() {
		return rootIndex;
	}
	/**
	 * This method returns of the token index of the type character data or CDATA.
	 * Notice that it is intended to support data orient XML (not mixed-content XML).
	 * return the index of the text token, or -1 if none exists.
	 * @return int
	 */
	public int getText() {
		int index = (context[0] != 0) ? context[context[0]] + 1 : rootIndex + 1;
		int depth = getCurrentDepth();
		int type; 
		if (index<vtdSize)
			type = getTokenType(index);
		else 
			return -1;

		while (true) {
			if (type == TOKEN_CHARACTER_DATA || type == TOKEN_CDATA_VAL) {
				if (depth == getTokenDepth(index))
					return index;
				else
					return -1;
			} else if (type == TOKEN_ATTR_NS || type == TOKEN_ATTR_NAME) {
				index += 2; // assuming a single token for attr val
			} else if (
				type == TOKEN_PI_NAME
					|| type == TOKEN_PI_VAL
					|| type == TOKEN_COMMENT) {
				if (depth == getTokenDepth(index)) {
					index += 1;
				} else
					return -1;
			} else
				return -1;
			if (index >= vtdSize)
				break;
			type = getTokenType(index);
		}
		return -1;
	}
	/**
	 * Get total number of VTD tokens for the current XML document.
	 * @return int
	 */
	public int getTokenCount() {
		return vtdSize;
	}
	/**
	 * Get the depth value of a token (>=0).
	 * @return int
	 * @param index int
	 */
	public int getTokenDepth(int index) {
		int i = (int) ((vtdBuffer.longAt(index) & MASK_TOKEN_DEPTH) >> 52);
		if (i != 255)
			return i;
		return -1;
	}
	/**
	 * Get the token length at the given index value
	 * please refer to VTD spec for more details
	 * Length is in terms of the UTF char unit
	 * For prefixed tokens, it is the qualified name length.
	 * When ns is not enabled, return the full name length for attribute name and element name
	 * When ns is enabled, return an int with upper 16 bit for prefix length, lower 16 bit for qname length 
	 * @return int
	 * @param index int
	 */
	public int getTokenLength(int index) {
		int type = getTokenType(index);
		int depth;
		int val;
		int len = 0;
		long l = vtdBuffer.longAt(index);
		switch (type) {
			case TOKEN_ATTR_NAME :
			case TOKEN_ATTR_NS :
			case TOKEN_STARTING_TAG :
				return (ns == false)
					? (int) ((l & MASK_TOKEN_QN_LEN) >> 32)
					: ((int) ((l & MASK_TOKEN_QN_LEN)
						>> 32)
						| ((int) ((l & MASK_TOKEN_PRE_LEN)
							>> 32)
							<< 5));
			case TOKEN_CHARACTER_DATA:
			case TOKEN_CDATA_VAL:
			case TOKEN_COMMENT: // make sure this is total length
				depth = getTokenDepth(index);
				do{
					len = len +  (int)
					((l	& MASK_TOKEN_FULL_LEN) >> 32);
					index++;						
					}
				while(index < vtdSize && depth == getTokenDepth(index) 
						&& type == getTokenType(index));
				//if (int k=0)
				return len;
			default :
				return (int)
					((l & MASK_TOKEN_FULL_LEN) >> 32);
		}
		/*if (encoding<3)
		 return val;
		else
		 return val<<1;*/
	}
	/**
	 * Get the starting offset of the token at the given index.
	 * @return int
	 * @param index int
	 * @exception
	 */
	public int getTokenOffset(int index) {
		//return (context[0] != 0)
		//    ? (int) (vtdBuffer.longAt(context[context[0]]) & MASK_TOKEN_OFFSET)
		//    : (int) (vtdBuffer.longAt(rootIndex) & MASK_TOKEN_OFFSET);
		return (int) (vtdBuffer.longAt(index) & MASK_TOKEN_OFFSET);
	}

	/** Get the XML document 
	 * @return IByteBuffer
	 */
	public IByteBuffer getXML() {
		return XMLDoc;
	}
	/**
	 * Get the token type of the token at the given index value.
	 * Creation date: (11/16/03 6:41:51 PM)
	 * @return int
	 * @param index int
	 * @exception
	 */
	public int getTokenType(int index) {
		return (int) ((vtdBuffer.longAt(index) & MASK_TOKEN_TYPE) >> 60) & 0xf;
	}
	/**
	 * Test whether current element has an attribute with the matching name.
	 * "*" will match any attribute name, therefore is a test whether there is any attribute at all
	 * if namespace is disabled, this function will not distinguish between ns declaration and attribute 
	 * otherwise, ns tokens are invisible
	 * Creation date: (11/16/03 5:50:26 PM)
	 * @return boolean (true if such an attribute exists)
	 * @param an java.lang.String
	 * @exception com.ximpleware.NavException The exception if the underlying byte 
	 * content contains various errors. Notice that we are being conservative in making little assumption on
	 * the correctness of underlying byte content. This is because the VTD etc can be generated by another
	 * machine from a load-balancer.
	 * @exception IllegalArguementException if an is null
	 */
	public boolean hasAttr(String an) throws NavException {
		int size = vtdBuffer.size();
		int index = (context[0] != 0) ? context[context[0]] + 1 : rootIndex + 1;
		if (index >= size)
			return false;
		int type = getTokenType(index);
		if (ns == false) {
			if (an.equals("*")) {
				if (type == TOKEN_ATTR_NAME || type == TOKEN_ATTR_NS)
					return true;
				else
					return false;
			} else {
				while (index < size
					&& (type == TOKEN_ATTR_NAME
						|| type == TOKEN_ATTR_NS)) { // ns tokens becomes visible
					if (matchRawTokenString(index, an))
						return true;
					index += 2;
					type = getTokenType(index);
				}
				return false;
			}
		} else {
			if (an.equals("*")) {
				while (index < size
					&& (getTokenType(index) == TOKEN_ATTR_NAME
						|| getTokenType(index) == TOKEN_ATTR_NS)) {
					if (type == TOKEN_ATTR_NAME)
						// TOKEN_ATTR_NS is invisible when ns == true
						return true;
					index += 2;
					type = getTokenType(index);
				}
				return false;

			} else {
				while (index < size
					&& (type == TOKEN_ATTR_NAME || type == TOKEN_ATTR_NS)) {
					if (type == TOKEN_ATTR_NAME
						&& matchRawTokenString(index, an))
						return true;
					index += 2;
					type = getTokenType(index);
				}
				return false;
			}
		}
	}
	/**
	 * Test whether the current element has an attribute with 
	 * matching namespace URL and localname.
	 * If ns is false, return false immediately
	 * @return boolean
	 * @param URL java.lang.String (namespace URL)
	 * @param ln java.lang.String  (localname )
	 * @exception com.ximpleware.NavException The exception if the underlying byte 
	 * content contains various errors. Notice that we are being conservative in making little assumption on
	 * the correctness of underlying byte content. This is because the VTD  can be generated by another
	 * machine such as a load-balancer.
	 * @exception IllegalArguementException if ln is null
	 */
	public boolean hasAttrNS(String URL, String ln) throws NavException {
		return (getAttrValNS(URL, ln) != -1);
	}
	/**
	 * Test the token type, to see if it is a starting tag.
	 * @return boolean
	 * @param index int
	 */
	private final boolean isElement(int index) {
		return (((vtdBuffer.longAt(index) & MASK_TOKEN_TYPE) >> 60) & 0xf)
			== TOKEN_STARTING_TAG;
	}
	/**
	 * Test whether ch is a white space character or not.
	 * @return boolean
	 * @param ch int
	 */
	final private boolean isWS(int ch) {
		return (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r');
	}
	/**
	 * This method is similar to getElementByName in DOM except it doesn't
	 * return the nodeset, instead it iterates over those nodes. Notice that this method
	 * is called by the "iterate" method in the Autopilot class.
	 * "*" will match any element
	 * Creation date: (12/2/03 2:31:20 PM)
	 * @return boolean
	 * @param dp int    (The depth of the starting position before iterating)
	 * @param en java.lang.String
	 * @exception com.ximpleware.NavException The exception is signaled if the underlying byte 
	 * content contains various errors. Notice that we are being conservative in making little assumption on
	 * the correctness of underlying byte content. This is because VTD records can be generated by another
	 * machine from a load-balancer.
	 * @exception IllegalArguementException   When en is null
	 */
	protected boolean iterate(int dp, String en)
		throws NavException { // the navigation doesn't rely on LC
		// get the current depth
		int index = getCurrentIndex() + 1;
		//int size = vtdBuffer.size();
		while (index < vtdSize) {
			if (isElement(index)) {
				int depth = getTokenDepth(index);
				if (depth > dp) {
					context[0] = depth;
					context[depth] = index;
					if (matchElement(en)) {
						if (depth < 4)
							resolveLC();
						return true;
					}
				} else {
					return false;
				}
			}
			index++;
		}
		return false;
	}
	/**
	 * This method is similar to getElementByName in DOM except it doesn't
	 * return the nodeset, instead it iterates over those nodes .
	 * When URL is "*" it will match any namespace
	 * if ns is false, return false immediately
	 * @return boolean
	 * @param dp int    (The depth of the starting position before iterating)
	 * @param URL  java.lang.String
	 * @param ln  java.lang.String
	 * @exception com.ximpleware.NavException The exception if the underlying byte 
	 * content contains various errors. Notice that we are being conservative in making little assumption on
	 * the correctness of underlying byte content. This is because VTD records can be generated by another
	 * machine from a load-balancer..
	 * @exception IllegalArguementException if ln is null
	 * example
	 * 
	 *  int depth = nv.getCurrentDepth()
	 *  while(iterateNS(depth, "www.url.com","node_name")){
	 *		push(); // store the current position
	 *		//move position safely
	 *		pop();  // load the position
	 * }
	 */
	protected boolean iterateNS(int dp, String URL, String ln)
		throws NavException {
		if (ns == false)
			return false;
		if (ln == null)
			throw new IllegalArgumentException("local name can't be null");
		int index = getCurrentIndex() + 1;
		while (index < vtdSize) {
			if (isElement(index)) {
				int depth = getTokenDepth(index);
				if (depth > dp) {
					context[0] = depth;
					context[depth] = index;
					if (matchElementNS(URL, ln)) {
						if (depth < 4)
							resolveLC();
						return true;
					}
				} else {
					return false;
				}
			}
			index++;
		}
		return false;
	}

	/**
	 * Test if the current element matches the given name.
	 * Creation date: (11/26/03 2:09:43 PM)
	 * @return boolean
	 * @param en java.lang.String
	 * @exception com.ximpleware.NavException If the underlying raw char representation has errors.
	 * @exception IllegalArguementException if en is null
	 */
	public boolean matchElement(String en) throws NavException {
		if (en == null)
			throw new IllegalArgumentException(" Element name can't be null ");
		if (en.equals("*"))
			return true;
		return matchRawTokenString(
			(context[0] == 0) ? rootIndex : context[context[0]],
			en);
	}
	/**
	 * Test whether the current element matches the given namespace URL and localname.
	 * URL, when set to "*", matches any namespace (including null), when set to null, defines a "always-no-match"
	 * ln is the localname that, when set to *, matches any localname
	 * @return boolean
	 * @param URL java.lang.String
	 * @param ln java.lang.String
	 * @exception com.ximpleware.NavException When there is any encoding conversion error or unknown entity.
	 * @exception java.lang.IllegalArgumentException  if ln == null
	 */
	public boolean matchElementNS(String URL, String ln) throws NavException {

		int i =
			getTokenLength((context[0] != 0) ? context[context[0]] : rootIndex);
		int offset =
			getTokenOffset((context[0] != 0) ? context[context[0]] : rootIndex);
		int preLen = (i >> 16) & 0xffff;
		int fullLen = i & 0xffff;

		if (ln.equals("*")
			|| ((preLen != 0)
				? matchRawTokenString(
					offset + preLen + 1,
					fullLen - preLen - 1,
					ln)
				: matchRawTokenString(
					offset,
					fullLen,
					ln))) { // no prefix, search for xmlns
			if (((URL != null) ? URL.equals("*") : false)
				|| (resolveNS(URL, offset, preLen) == true))
				return true;
		}
		return false;
	}
	/**
	 * Match a string against a token with given offset and len, entities doesn't get resolved.
	 * @return boolean
	 * @param offset int
	 * @param len int
	 * @param s java.lang.String
	 * @exception com.ximpleware.NavException The exception if the underlying byte 
	 * content contains various errors. Notice that we are being conservative in making little assumption on
	 * the correctness of underlying byte content. This is because the VTD  can be generated by another
	 * machine such as a load-balancer.
	 * @exception IllegalArguementException if s is null
	 */
	private boolean matchRawTokenString(int offset, int len, String s)
		throws NavException {
		if (s == null)
			throw new IllegalArgumentException("string can't be null");
		int i, l;
		this.currentOffset = offset;
		int endOffset = offset + len;
		if (encoding < 2) {
			if (s.length() != len)
				return false;
			l = s.length();
			for (i = 0; i < l && currentOffset < endOffset; i++) {
				if (s.charAt(i) != (XMLDoc.byteAt(currentOffset) & 0xff))
					return false;
				currentOffset++;
			}
			return true;
		} else {
			//       System.out.print("currentOffset :" + currentOffset);
			l = s.length();
			//System.out.println(s);
			for (i = 0; i < l && currentOffset < endOffset; i++) {
				if (s.charAt(i) != getChar()) {
					return false;
				}
			}
		}
		if (i == l && currentOffset == endOffset)
			return true;
		return false;
	}
	/**
	 * Match the string against the token at the given index value. When a token
	 * is an attribute name or starting tag, qualified name is what gets matched against
	 * This method has to take care of the underlying encoding conversion
	 * but it <em> doesn't </em> resolve entity reference in the underlying document
	 * @return boolean
	 * @param index int   (index into the VTD token buffer)
	 * @param s java.lang.String
	 * @exception com.ximpleware.NavException When if the underlying byte 
	 * content contains various errors. Notice that we are being conservative in making little assumption on
	 * the correctness of underlying byte content. This is because the VTD  can be generated by another
	 * machine such as a load-balancer.
	 * @exception IllegalArguementException if s is null
	**/
	public boolean matchRawTokenString(int index, String s)
		throws NavException {
		if (s == null)
			throw new IllegalArgumentException("string can't be null");
		int type = getTokenType(index);
		int len =
			(type == TOKEN_STARTING_TAG
				|| type == TOKEN_ATTR_NAME
				|| type == TOKEN_ATTR_NS)
				? getTokenLength(index) & 0xffff
				: getTokenLength(index);
		// upper 16 bit is zero or for prefix

		currentOffset = getTokenOffset(index);
		// point currentOffset to the beginning of the token
		// for UTF 8 and ISO, the performance is a little better by avoid calling getChar() everytime
		return matchRawTokenString(currentOffset, len, s);
	}
	/**
	 * Match a string with a token represented by a long (upper 32 len, lower 32 offset).
	 * @return boolean
	 * @param l long
	 * @param s java.lang.String
	 * @exception com.ximpleware.NavException When if the underlying byte 
	 * content contains various errors. Notice that we are being conservative in making little assumption on
	 * the correctness of underlying byte content. This is because the VTD  can be generated by another
	 * machine such as a load-balancer.
	 * @exception IllegalArguementException if s is null
	 * 
	 */
	private boolean matchRawTokenString(long l, String s) throws NavException {
		if (s == null)
			throw new IllegalArgumentException("string can't be null");
		int len = (int) ((l & MASK_TOKEN_FULL_LEN) >> 32);
		// a little hardcode is always bad
		currentOffset = (int) l;
		return matchRawTokenString(currentOffset, len, s);
	}
	/**
	 * Match a string against a token with given offset and len, entities get resolved properly.
	 * Creation date: (11/24/03 1:37:42 PM)
	 * @return boolean
	 * @param offset int
	 * @param len int
	 * @param s java.lang.String
	 * @exception com.ximpleware.NavException The exception if the underlying byte 
	 * content contains various errors. Notice that we are being conservative in making little assumption on
	 * the correctness of underlying byte content. This is because the VTD  can be generated by another
	 * machine such as a load-balancer.
	 * @exception IllegalArguementException if s is null
	 */
	private boolean matchTokenString(int offset, int len, String s)
		throws NavException {
		if (s == null)
			throw new IllegalArgumentException("string can't be null");

		currentOffset = offset;
		int endOffset = offset + len;
		int l;
		if (encoding < FORMAT_UTF8) {
			int i = 0;
			l = s.length();
			for (i = 0; i < l && currentOffset < endOffset; i++) {
				if ((XMLDoc.byteAt(currentOffset) & 0xff) != '&') {
					if (s.charAt(i) != (XMLDoc.byteAt(currentOffset) & 0xff))
						return false;
					currentOffset++;
				} else {
					if (s.charAt(i) != getCharResolved()) {
						return false;
					}
				}
			}
			if (i == l && currentOffset == endOffset)
				return true;
			else
				return false;
		} else {
			int i = 0;
			l = s.length();
			for (i = 0; i < l && currentOffset < endOffset; i++) {
				if (s.charAt(i) != getCharResolved()) {
					return false;
				}
			}
			if (i == l && currentOffset == endOffset)
				return true;
			else
				return false;
		}
	}
	/**
	 * Match the string against the token at the given index value. When a token
	 * is an attribute name or starting tag, qualified name is what gets matched against
	 * This method has to take care of the underlying encoding conversion
	 * as well as entity reference comparison
	 * @return boolean
	 * @param index int
	 * @param s java.lang.String
	 * @exception com.ximpleware.NavException When if the underlying byte 
	 * content contains various errors. Notice that we are being conservative in making little assumption on
	 * the correctness of underlying byte content. This is because the VTD  can be generated by another
	 * machine such as a load-balancer.
	 * @exception IllegalArguementException if s is null
	**/
	public boolean matchTokenString(int index, String s) throws NavException {
		if (s == null)
			throw new IllegalArgumentException("string can't be null");
		int type = getTokenType(index);
		int len =
			(type == TOKEN_STARTING_TAG
				|| type == TOKEN_ATTR_NAME
				|| type == TOKEN_ATTR_NS)
				? getTokenLength(index) & 0xffff
				: getTokenLength(index);
		// upper 16 bit is zero or for prefix

		currentOffset = getTokenOffset(index);
		// point currentOffset to the beginning of the token
		// for UTF 8 and ISO, the performance is a little better by avoid calling getChar() everytime
		return matchTokenString(currentOffset, len, s);
	}
	/**
	 * Match a string against a "non-extractive" token represented by a long (upper 32 len, lower 32 offset).
	 * @return boolean
	 * @param l long
	 * @param s java.lang.String
	 * @exception com.ximpleware.NavException When the underlying byte 
	 * content contains various errors. Notice that we are being conservative in making little assumption on
	 * the correctness of underlying byte content. This is because the VTD  can be generated by another
	 * machine such as a load-balancer.
	 * @exception IllegalArguementException if s is null
	 * 
	 */

	private boolean matchTokenString(long l, String s) throws NavException {
		if (s == null)
			throw new IllegalArgumentException("string can't be null");
		int len = (int) (l >> 32) & 0xffff;
		currentOffset = (int) l;
		return matchTokenString(currentOffset, len, s);
	}


	/**
	 * Evaluate the namespace indicator in bit 31 and bit 30.
	 * Creation date: (11/27/03 5:38:51 PM)
	 * @return int
	 * @param i int
	 */
	final private int NSval(int i) {

		return (int) (vtdBuffer.longAt(i) & MASK_TOKEN_NS_MARK);
	}
	/**
	 * Convert a vtd token into a double.
	 * Creation date: (12/8/03 2:28:31 PM)
	 * @return double
	 * @exception com.ximpleware.NavException The exception if the underlying byte 
	 * content contains various errors. Notice that we are being conservative in making little assumption on
	 * the correctness of underlying byte content. This is because the VTD  can be generated by another
	 * machine such as a load-balancer.
	 */
	public double parseDouble(int index) throws NavException {
		currentOffset = getTokenOffset(index);
		int end = currentOffset + getTokenLength(index);
		//past the last one by one

		int ch = getCharResolved();

		while (currentOffset < end) { // trim leading whitespaces
			if (!isWS(ch))
				break;
			ch = getCharResolved();
		}

		if (currentOffset > end) // all whitespace
			throw new NavException("Empty string");

		boolean neg = (ch == '-');

		if (ch == '-' || ch == '+')
			ch = getCharResolved(); //get another one if it is sign.

		//left part of decimal
		double left = 0;
		while (currentOffset <= end) {
			//must be <= since we get the next one at last.

			int dig = Character.digit((char) ch, 10); //only consider decimal
			if (dig < 0)
				break;

			left = left * 10 + dig;

			ch = getCharResolved();
		}

		//right part of decimal
		double right = 0;
		double scale = 1;
		if (ch == '.') {
			ch = getCharResolved();

			while (currentOffset <= end) {
				//must be <= since we get the next one at last.

				int dig = Character.digit((char) ch, 10);
				//only consider decimal
				if (dig < 0)
					break;

				right = right * 10 + dig;
				scale *= 10;

				ch = getCharResolved();
			}
		}

		//exponent
		long exp = 0;
		if (ch == 'E' || ch == 'e') {
			ch = getCharResolved();
			boolean expneg = (ch == '-'); //sign for exp
			if (ch == '+' || ch == '-')
				ch = getCharResolved(); //skip the +/- sign

			int cur = currentOffset;
			//remember the indx, used to find a invalid number like 1.23E

			while (currentOffset <= end) {
				//must be <= since we get the next one at last.

				int dig = Character.digit((char) ch, 10);
				//only consider decimal
				if (dig < 0)
					break;

				exp = exp * 10 + dig;

				ch = getCharResolved();
			}
			if (cur == currentOffset)
				throw new NavException(toString(index));
			//found a invalid number like 1.23E

			if (expneg)
				exp = (-exp);
		}

		//anything left must be space
		while (currentOffset <= end) {
			if (!isWS(ch))
				throw new NavException(toString(index));

			ch = getCharResolved();
		}

		double v = (double) left;
		if (right != 0)
			v += ((double) right) / (double) scale;

		if (exp != 0)
			v = v * Math.pow(10, exp);

		return ((neg) ? (-v) : v);
	}
	/**
	 * Convert a vtd token into a float.
	 * we assume token type to be attr val or character data
	 * Creation date: (12/8/03 2:28:18 PM)
	 * @return float
	 * @exception com.ximpleware.NavException The exception if the underlying byte 
	 * content contains various errors. Notice that we are being conservative in making little assumption on
	 * the correctness of underlying byte content. This is because the VTD  can be generated by another
	 * machine such as a load-balancer.
	 */
	public float parseFloat(int index) throws NavException {

		currentOffset = getTokenOffset(index);
		int end = currentOffset + getTokenLength(index);
		//past the last one by one

		int ch = getCharResolved();

		while (currentOffset <= end) { // trim leading whitespaces
			if (!isWS(ch))
				break;
			ch = getCharResolved();
		}

		if (currentOffset > end) // all whitespace
			throw new NavException("Empty string");

		boolean neg = (ch == '-');

		if (ch == '-' || ch == '+')
			ch = getCharResolved(); //get another one if it is sign.

		//left part of decimal
		long left = 0;
		while (currentOffset <= end) {
			//must be <= since we get the next one at last.

			int dig = Character.digit((char) ch, 10); //only consider decimal
			if (dig < 0)
				break;

			left = left * 10 + dig;

			ch = getCharResolved();
		}

		//right part of decimal
		long right = 0;
		long scale = 1;
		if (ch == '.') {
			ch = getCharResolved();

			while (currentOffset <= end) {
				//must be <= since we get the next one at last.

				int dig = Character.digit((char) ch, 10);
				//only consider decimal
				if (dig < 0)
					break;

				right = right * 10 + dig;
				scale *= 10;

				ch = getCharResolved();
			}
		}

		//exponent
		long exp = 0;
		if (ch == 'E' || ch == 'e') {
			ch = getCharResolved();
			boolean expneg = (ch == '-'); //sign for exp
			if (ch == '+' || ch == '-')
				ch = getCharResolved(); //skip the +/- sign

			int cur = currentOffset;
			//remember the indx, used to find a invalid number like 1.23E

			while (currentOffset <= end) {
				//must be <= since we get the next one at last.

				int dig = Character.digit((char) ch, 10);
				//only consider decimal
				if (dig < 0)
					break;

				exp = exp * 10 + dig;

				ch = getCharResolved();
			}

			if (cur == currentOffset)
				throw new NavException(toString(index));
			//found a invalid number like 1.23E

			if (expneg)
				exp = (-exp);
		}

		//anything left must be space
		while (currentOffset <= end) {
			if (!isWS(ch))
				throw new NavException(toString(index));

			ch = getCharResolved();
		}

		double v = (double) left;
		if (right != 0)
			v += ((double) right) / (double) scale;

		if (exp != 0)
			v = v * Math.pow(10, exp);
		if (neg)
			v = -v;

		float f = (float) v;

		//try to handle overflow/underflow
		if (v >= Float.MAX_VALUE)
			f = Float.MAX_VALUE;
		else if (v <= Float.MIN_VALUE)
			f = Float.MIN_VALUE;

		return f;
	}
	/**
	 * Convert a vtd token into an int.
	 * This method will automatically strip off the leading and trailing
	 * we assume token type to be attr val or character data
	 * zero, unlike Integer.parseInt(int index)
	 * 
	 * Creation date: (12/8/03 2:32:22 PM)
	 * @return int
	 * @param index int
	 * @exception com.ximpleware.NavException The exception if the underlying byte 
	 * content contains various errors. Notice that we are being conservative in making little assumption on
	 * the correctness of underlying byte content. This is because the VTD  can be generated by another
	 * machine such as a load-balancer.
	 */
	public int parseInt(int index) throws NavException {
		return parseInt(index, 10);
	}
	/**
	 * Convert a vtd token into an int, with the given radix.
	 * we assume token type to be attr val or character data
	 * the first char can be either '+' or '-'
	 * Creation date: (12/16/03 1:21:20 PM)
	 * @return int
	 * @param index int
	 * @param radix int
	 * @exception com.ximpleware.NavException The exception if the underlying byte 
	 * content contains various errors. Notice that we are being conservative in making little assumption on
	 * the correctness of underlying byte content. This is because the VTD  can be generated by another
	 * machine such as a load-balancer.
	 */
	protected int parseInt(int index, int radix) throws NavException {
		if (radix < 2 || radix > 36)
			throw new NumberFormatException(
				"radix " + radix + " out of valid range");

		currentOffset = getTokenOffset(index);
		int endOffset = currentOffset + getTokenLength(index);

		int c = getCharResolved();

		// trim leading whitespaces
		while ((c == ' ' || c == '\n' || c == '\t' || c == '\r')
			&& (currentOffset <= endOffset))
			c = getCharResolved();

		if (currentOffset > endOffset) // all whitespace
			throw new NumberFormatException(" empty string");

		boolean neg = (c == '-');
		if (neg || c == '+')
			c = getCharResolved(); //skip sign

		long result = 0;
		long pos = 1;
		while (currentOffset <= endOffset) {
			int digit = Character.digit((char) c, radix);
			if (digit < 0)
				break;

			//Note: for binary we can simply shift to left to improve performance
			result = result * radix + digit;
			//pos *= radix;

			c = getCharResolved();
		}

		if (result > Integer.MAX_VALUE)
			throw new NumberFormatException("Overflow: " + toString(index));

		// take care of the trailing
		while (currentOffset <= endOffset && isWS(c)) {
			c = getCharResolved();
		}
		if (currentOffset == (endOffset + 1))
			return (int) ((neg) ? (-result) : result);
		else
			throw new NumberFormatException(toString(index));
	}
	/**
	 * Convert a vtd token into a long.
	 * we assume token type to be attr val or character data
	 * Creation date: (12/8/03 2:32:59 PM)
	 * @return long
	 * @param index int
	 * @exception com.ximpleware.NavException The exception if the underlying byte 
	 * content contains various errors. Notice that we are being conservative in making little assumption on
	 * the correctness of underlying byte content. This is because the VTD  can be generated by another
	 * machine such as a load-balancer.
	 */
	public long parseLong(int index) throws NavException {
		return parseLong(index, 10);
	}
	/**
	 * Convert a vtd token into a long, with the given radix.
	 * the first char can be either '+' or '-', leading and trailing will be stripped
	 * we assume token type to be attr val or character data
	 * Creation date: (12/17/03 1:51:06 PM)
	 * @return long
	 * @param index int
	 * @param radix int
	 * @exception com.ximpleware.NavException The exception if the underlying byte 
	 * content contains various errors. Notice that we are being conservative in making little assumption on
	 * the correctness of underlying byte content. This is because the VTD  can be generated by another
	 * machine such as a load-balancer.
	 */
	protected long parseLong(int index, int radix) throws NavException {
		if (radix < 2 || radix > 36)
			throw new NumberFormatException(
				"radix " + radix + " out of valid range");

		currentOffset = getTokenOffset(index);
		int endOffset = currentOffset + getTokenLength(index);

		int c = getCharResolved();

		// trim leading whitespaces
		while ((c == ' ' || c == '\n' || c == '\t' || c == '\r')
			&& (currentOffset <= endOffset))
			c = getCharResolved();

		if (currentOffset > endOffset) // all whitespace
			throw new NumberFormatException(" empty string");

		boolean neg = (c == '-');
		if (neg || c == '+')
			c = getCharResolved(); //skip sign

		long result = 0;
		long pos = 1;
		while (currentOffset <= endOffset) {
			int digit = Character.digit((char) c, radix);
			if (digit < 0)
				break;

			//Note: for binary we can simply shift to left to improve performance
			result = result * radix + digit;
			//pos *= radix;

			c = getCharResolved();
		}

		if (result > Long.MAX_VALUE)
			throw new NumberFormatException("Overflow: " + toString(index));

		// take care of the trailing
		while (currentOffset <= endOffset && isWS(c)) {
			c = getCharResolved();
		}
		if (currentOffset == (endOffset + 1))
			return (int) ((neg) ? (-result) : result);
		else
			throw new NumberFormatException(toString(index));
	}
	/**
	 * Load the context info from ContextBuffer.
	 * Info saved including LC and current state of the context 
	 * Creation date: (11/16/03 6:59:20 PM)
	 * @ret boolean
	 */
	public boolean pop() {
		boolean b = contextStack.load(stackTemp);
		if (b == false)
			return false;
		for (int i = 0; i < nestingLevel; i++) {
			context[i] = stackTemp[i];
		}
		l1index = stackTemp[nestingLevel];
		l2index = stackTemp[nestingLevel + 1];
		l3index = stackTemp[nestingLevel + 2];
		l2lower = stackTemp[nestingLevel + 3];
		l2upper = stackTemp[nestingLevel + 4];
		l3lower = stackTemp[nestingLevel + 5];
		l3upper = stackTemp[nestingLevel + 6];

		return true;

	}
	/**
	 * Insert the method's description here.
	 * Creation date: (3/11/04 1:46:06 PM)*/

	public void printL2Buffer() {

		for (int i = 0; i < l1Buffer.size(); i++)
			System.out.println(Long.toHexString(l1Buffer.longAt(i)));

		System.out.println("==================");
		for (int i = 0; i < l2Buffer.size(); i++)
			System.out.println(Long.toHexString(l2Buffer.longAt(i)));
		System.out.println(
			" l1index :"
				+ l1index
				+ "  l2index :"
				+ l2index
				+ " l3index :"
				+ l3index);
		System.out.println(
			"          " + "l2lower :" + l2lower + " l3lower: " + l3lower);
		System.out.println(
			"          " + "l2upper :" + l2upper + " l3upper: " + l3upper);

	}
	/**
	 * printLC for debugging purpose.
	 * Creation date: (3/12/04 6:07:40 PM)
	 */
	public void printLC() {
		int a = 0, b = 0, c = 0;
		int depth = getCurrentDepth();
		if (depth == 1) {
			System.out.println("l1index --->" + l1index);
		} else if (depth == 2) {
			System.out.println("l2index --->" + l2index);
			System.out.println("l2lower --->" + l2lower);
			System.out.println("l2upper --->" + l2upper);
		} else if (depth == 3) {
			System.out.println("l3index --->" + l3index);
			System.out.println("l3lower --->" + l3lower);
			System.out.println("l3upper --->" + l3upper);
		}
	}
	/**
	 * Store the context info into the ContextBuffer.
	 * Info saved including LC and current state of the context 
	 * Creation date: (11/16/03 7:00:27 PM)
	 */
	public void push() {
		for (int i = 0; i < nestingLevel; i++) {
			stackTemp[i] = context[i];
		}
		stackTemp[nestingLevel] = l1index;
		stackTemp[nestingLevel + 1] = l2index;
		stackTemp[nestingLevel + 2] = l3index;
		stackTemp[nestingLevel + 3] = l2lower;
		stackTemp[nestingLevel + 4] = l2upper;
		stackTemp[nestingLevel + 5] = l3lower;
		stackTemp[nestingLevel + 6] = l3upper;

		contextStack.store(stackTemp);
	}
	/**
	 * Sync up the current context with location cache.
	 * This operation includes finding out l1index, l2index, 
	 * l3index and restores upper and lower bound info
	 * To improve efficieny this method employs some heuristic search algorithm.
	 * The result is that it is quite close to direct access.
	 * Creation date: (11/16/03 7:44:53 PM)
	 * @return int  The index of the NS URL
	 */
	private void resolveLC() {
		int temp;
		switch (context[0]) {
			case 1 :
			case 2 :
			case 3 :
				if (l1index < 0
					|| l1index >= l1Buffer.size()
					|| context[1] != l1Buffer.upper32At(l1index)) {
					if (l1index >= l1Buffer.size()) {
						l1index = 0;
					}
					if (context[1] != l1Buffer.upper32At(l1index + 1)) {
						int init_guess =
							(int) (l1Buffer.size()
								* ((float) context[1] / vtdBuffer.size()));
						if (l1Buffer.upper32At(init_guess) > context[1]) {
							while (l1Buffer.upper32At(init_guess)
								!= context[1]) {
								init_guess--;
							}
						} else if (
							l1Buffer.upper32At(init_guess) < context[1]) {
							while (l1Buffer.upper32At(init_guess)
								!= context[1]) {
								init_guess++;
							}
						}
						l1index = init_guess;
					} else
						l1index = l1index + 1;
					// for iterations, l1index+1 is the logical next value for l1index
				}
				if (context[0] == 1)
					break;

				temp = l1Buffer.lower32At(l1index);
				if (l2lower != temp) {
					l2lower = temp;
					// l2lower shouldn't be -1 !!!!  l2lower and l2upper always get resolved simultaneously
					l2index = l2lower;
					l2upper = l2Buffer.size() - 1;
					for (int i = l1index + 1; i < l1Buffer.size(); i++) {
						temp = l1Buffer.lower32At(i);
						if (temp != 0xffffffff) {
							l2upper = temp - 1;
							break;
						}
					}
				} // intelligent guess again ??

				if (l2index < 0
					|| l2index >= l2Buffer.size()
					|| context[2] != l2Buffer.upper32At(l2index)) {
					if (l2index >= l2Buffer.size())
						l2index = l2lower;
					if (context[2] == l2Buffer.upper32At(l2index + 1))
						l2index = l2index + 1;
					else if (l2upper - l2lower >= 16) {
						int init_guess =
							l2lower
								+ (int) ((l2upper - l2lower)
									* ((float) context[2]
										- l2Buffer.upper32At(l2lower))
									/ (l2Buffer.upper32At(l2upper)
										- l2Buffer.upper32At(l2lower)));
						if (l2Buffer.upper32At(init_guess) > context[2]) {
							while (context[2]
								!= l2Buffer.upper32At(init_guess))
								init_guess--;
						} else if (
							l2Buffer.upper32At(init_guess) < context[2]) {
							while (context[2]
								!= l2Buffer.upper32At(init_guess))
								init_guess++;
						}
						l2index = init_guess;
					} else
						while (context[2] != l2Buffer.upper32At(l2index)) {
							l2index++;
						}
				}

				if (context[0] == 2)
					break;
				temp = l2Buffer.lower32At(l2index);
				if (l3lower != temp) {
					//l3lower and l3upper are always together
					l3lower = temp;
					// l3lower shouldn't be -1
					l3index = l3lower;
					l3upper = l3Buffer.size() - 1;
					for (int i = l2index + 1; i < l2Buffer.size(); i++) {
						temp = l2Buffer.lower32At(i);
						if (temp != 0xffffffff) {
							l3upper = temp - 1;
							break;
						}
					}
				}

				if (l3index < 0
					|| l3index >= l3Buffer.size()
					|| context[3] != l3Buffer.intAt(l3index)) {
					if (l3index >= l3Buffer.size())
						l3index = l3lower;
					if (context[3] == l3Buffer.intAt(l3index + 1))
						l3index = l3index + 1;
					else if (l3upper - l3lower >= 16) {
						int init_guess =
							l3lower
								+ (int) ((l3upper - l3lower)
									* ((float) (context[3]
										- l3Buffer.intAt(l3lower))
										/ (l3Buffer.intAt(l3upper)
											- l3Buffer.intAt(l3lower))));
						if (l3Buffer.intAt(init_guess) > context[3]) {
							while (context[3] != l3Buffer.intAt(init_guess))
								init_guess--;
						} else if (l3Buffer.intAt(init_guess) < context[3]) {
							while (context[3] != l3Buffer.intAt(init_guess))
								init_guess++;
						}
						l3index = init_guess;
					} else
						while (context[3] != l3Buffer.intAt(l3index)) {
							l3index++;
						}
				}

				/*if (context[0] == 3)
				    break;*/
			default : // do nothing here
		}
	}
	/**
	 * Test whether the URL is defined in the document.
	 * Null is allowed to indicate the name space is undefined.
	 * Creation date: (11/16/03 7:54:01 PM)
	 * @param URL java.lang.String
	 * @exception com.ximpleware.NavException When there is any encoding conversion error or unknown entity.
	 */
	private boolean resolveNS(String URL) throws NavException {
		int i =
			getTokenLength((context[0] != 0) ? context[context[0]] : rootIndex);
		int offset =
			getTokenOffset((context[0] != 0) ? context[context[0]] : rootIndex);
		int preLen = (i >> 16) & 0xffff;

		return resolveNS(URL, offset, preLen);
	}
	/**
	 * Test whether the URL is defined in the document.
	 * Null is allowed to indicate the name space should be undefined.
	 * Creation date: (11/16/03 7:54:01 PM)
	 * @return boolean
	 * @param URL java.lang.String
	 * @param offset (offset of the prefix)
	 * @param len    (length of the prefix)
	 * @exception com.ximpleware.NavException When there is any encoding conversion error or unknown entity.
	 */
	private boolean resolveNS(String URL, int offset, int len)
		throws NavException {
		long l;
		boolean hasNS = false;
		int size = vtdBuffer.size();
		// look for a match in the current hiearchy and return true
		for (int i = context[0]; i >= 0; i--) {
			int s = (i != 0) ? context[i] : rootIndex;
			switch (NSval(s)) { // checked the ns marking
				case 0xc0000000 :
					s = s + 1;
					if (s>=size)
						break;
					int type = getTokenType(s);

					while ((type == TOKEN_ATTR_NAME || type == TOKEN_ATTR_NS)) {
						if (type == TOKEN_ATTR_NS) {
							// Get the token length
							int temp = getTokenLength(s);
							int preLen = ((temp >> 16) & 0xffff);
							int fullLen = temp & 0xffff;
							int os = getTokenOffset(s);
							// xmlns found
							if (temp == 5 && len == 0) {
								if (URL != null) {
									return matchTokenString(s + 1, URL);
								} else { //xmlns is found but shouldn't be
									return false;
								}
							} else if ((fullLen - preLen - 1) == len) {
								// prefix length identical to local part of ns declaration
								boolean a = true;
								for (int j = 0; j < len; j++) {
									if (getCharUnit(os + preLen + 1 + j)
										!= getCharUnit(offset + j)) {
										a = false;
										break;
									}
								}
								if (a == true) {
									return (URL != null)
										? matchTokenString(s + 1, URL)
										: false;
								}
							}
						}
						//return (URL != null) ? true : false;
						s += 2;
						if (s>=size)
							break;
						type = getTokenType(s);
					}
					break;
				case 0x80000000 :
					break;
				default : // check the ns existence, mark bit 31:30 to 11 or 10
					int k = s + 1;
				    if (k>=size)
				    	break;
					type = getTokenType(k);

					while ( (type == TOKEN_ATTR_NAME || type == TOKEN_ATTR_NS)) {
						if (type == TOKEN_ATTR_NS) {
							// Get the token length
							hasNS = true;
							int temp = getTokenLength(k);
							int preLen = ((temp >> 16) & 0xffff);
							int fullLen = temp & 0xffff;
							int os = getTokenOffset(k);
							// xmlns found
							if (temp == 5 && len == 0) {
								l = vtdBuffer.longAt(s);
								hasNS = false;
								vtdBuffer.modifyEntry(
									s,
									l | 0x00000000c0000000L);
								if (URL != null) {
									return matchRawTokenString(k + 1, URL);
								} else { //xmlns is found but shouldn't be
									return false;
								}
							} else if ((fullLen - preLen - 1) == len) {
								// prefix length identical to local part of ns declaration
								boolean a = true;
								for (int j = 0; j < len; j++) {
									if (getCharUnit(os + preLen + 1 + j)
										!= getCharUnit(offset + j)) {
										a = false;
										break;
									}
								}
								if (a == true) {
									l = vtdBuffer.longAt(s);
									//hasNS = false;
									vtdBuffer.modifyEntry(
										s,
										l | 0x00000000c0000000L);
									return (URL != null)
										? matchTokenString(k + 1, URL)
										: false;
								}
							}
						}
						//return (URL != null) ? true : false;
						k += 2;
						if (k>=size) 
							break;
						type = getTokenType(k);
					}
					l = vtdBuffer.longAt(s);
					if (hasNS) {
						hasNS = false;
						vtdBuffer.modifyEntry(s, l | 0x00000000c0000000L);
					} else {
						vtdBuffer.modifyEntry(s, l | 0x0000000080000000L);
					}
					break;
			}
		}
		return (URL != null) ? false : true;
	}
	/**
	 * A generic navigation method.
	 * Move the current to the element according to the direction constants
	 * If no such element, no position change and return false.
	 * Creation date: (12/2/03 1:43:50 PM)
	 * Legal direction constants are
	 *<pre>   			ROOT               0 </pre>	
	 *<pre> 		    PARENT  		   1 </pre>
	 *<pre>       	    FIRST_CHILD		   2 </pre>  
	 *<pre> 		    LAST_CHILD 		   3 </pre>
	 *<pre>    	  	    NEXT_SIBLING       4 </pre>
	 *<pre>      	    PREV_SIBLING       5 </pre>
	 * @return boolean
	 * @param direction int
	 * @exception com.ximpleware.NavException  When direction value is illegal.
	 */
	public boolean toElement(int direction) throws NavException {
		int size;
		switch (direction) {
			case ROOT :
				if (context[0] != 0) {
					for (int i = 1; i <= context[0]; i++) {
						context[i] = 0xffffffff;
					}
					context[0] = 0;
				}
				l1index = l2index = l3index = -1;
			case PARENT :
				if (context[0] != 0) {
					//context[context[0]] = context[context[0] + 1] = 0xffffffff;
					context[context[0]] = -1;
					context[0]--;
					return true;
				} else {
					return false;
				}
			case FIRST_CHILD :
			case LAST_CHILD :

				switch (context[0]) {
					case 0 :
						if (l1Buffer.size() > 0) {
							context[0] = 1;
							l1index =
								(direction == FIRST_CHILD)
									? 0
									: (l1Buffer.size() - 1);
							context[1] = l1Buffer.upper32At(l1index);
							//(int) (vtdToken >> 32);
							return true;
						} else
							return false;
					case 1 :
						l2lower = l1Buffer.lower32At(l1index);
						if (l2lower == -1) {
							return false;
						}
						context[0] = 2;
						l2upper = l2Buffer.size() - 1;
						size = l1Buffer.size();
						for (int i = l1index + 1; i < size; i++) {
							int temp = l1Buffer.lower32At(i);
							if (temp != 0xffffffff) {
								l2upper = temp - 1;
								break;
							}
						}
						//System.out.println(" l2 upper: " + l2upper + " l2 lower : " + l2lower);
						l2index =
							(direction == FIRST_CHILD) ? l2lower : l2upper;
						context[2] = l2Buffer.upper32At(l2index);
						return true;

					case 2 :
						l3lower = l2Buffer.lower32At(l2index);
						if (l3lower == -1) {
							return false;
						}
						context[0] = 3;

						l3upper = l3Buffer.size() - 1;
						size = l2Buffer.size();
						for (int i = l2index + 1; i < size; i++) {
							int temp = l2Buffer.lower32At(i);
							if (temp != 0xffffffff) {
								l3upper = temp - 1;
								break;
							}
						}
						//System.out.println(" l3 upper : " + l3upper + " l3 lower : " + l3lower);
						l3index =
							(direction == FIRST_CHILD) ? l3lower : l3upper;
						context[3] = l3Buffer.intAt(l3index);

						return true;

					default :
						if (direction == FIRST_CHILD) {
							size = vtdBuffer.size();
							int index = context[context[0]] + 1;
							while (index < size) {
								long temp = vtdBuffer.longAt(index);
								int token_type =
									(int) ((MASK_TOKEN_TYPE & temp) >> 60)
										& 0xf;

								if (token_type == TOKEN_STARTING_TAG) {
									int depth =
										(int) ((MASK_TOKEN_DEPTH & temp) >> 52);
									if (depth <= context[0]) {
										return false;
									} else if (depth == (context[0] + 1)) {
										context[0] += 1;
										context[context[0]] = index;
										return true;
									}
								}

								index++;
							} // what condition  
							return false;
						} else {
							int index = context[context[0]] + 1;
							int last_index = -1;
							size = vtdBuffer.size();
							while (index < size) {
								long temp = vtdBuffer.longAt(index);
								int depth =
									(int) ((MASK_TOKEN_DEPTH & temp) >> 52);
								int token_type =
									(int) ((MASK_TOKEN_TYPE & temp) >> 60)
										& 0xf;

								if (token_type == TOKEN_STARTING_TAG) {
									if (depth <= context[0]) {
										break;
									} else if (depth == (context[0] + 1)) {
										last_index = index;
									}
								}

								index++;
							}
							if (last_index == -1) {
								return false;
							} else {
								context[0] += 1;
								context[context[0]] = last_index;
								return true;
							}
						}
				}

			case NEXT_SIBLING :
			case PREV_SIBLING :
				switch (context[0]) {
					case 0 :
						return false;
					case 1 :
						if (direction == NEXT_SIBLING) {
							if (l1index + 1 >= l1Buffer.size()) {
								return false;
							}

							l1index++; // global incremental
						} else {
							if (l1index - 1 < 0) {
								return false;
							}
							l1index--; // global incremental
						}
						context[1] = l1Buffer.upper32At(l1index);
						return true;
					case 2 :
						if (direction == NEXT_SIBLING) {
							if (l2index + 1 > l2upper) {
								return false;
							}
							l2index++;
						} else {
							if (l2index - 1 < l2lower) {
								return false;
							}
							l2index--;
						}
						context[2] = l2Buffer.upper32At(l2index);
						return true;
					case 3 :
						if (direction == NEXT_SIBLING) {
							if (l3index + 1 > l3upper) {
								return false;
							}
							l3index++;
						} else {
							if (l3index - 1 < l3lower) {
								return false;
							}
							l3index--;
						}
						context[3] = l3Buffer.intAt(l3index);
						return true;
					default :
						//int index = context[context[0]] + 1;

						if (direction == NEXT_SIBLING) {
							int index = context[context[0]] + 1;
							while (index < vtdBuffer.size()) {
								long temp = vtdBuffer.longAt(index);
								int token_type =
									(int) ((MASK_TOKEN_TYPE & temp) >> 60)
										& 0xf;

								if (token_type == TOKEN_STARTING_TAG) {
									int depth =
										(int) ((MASK_TOKEN_DEPTH & temp) >> 52);
									if (depth < context[0]) {
										return false;
									} else if (depth == (context[0])) {
										context[context[0]] = index;
										return true;
									}
								}
								index++;
							}
							return false;
						} else {
							int index = context[context[0]] - 1;
							while (index > context[context[0] - 1]) {
								// scan backforward
								long temp = vtdBuffer.longAt(index);
								int token_type =
									(int) ((MASK_TOKEN_TYPE & temp) >> 60)
										& 0xf;

								if (token_type == TOKEN_STARTING_TAG) {
									int depth =
										(int) ((MASK_TOKEN_DEPTH & temp) >> 52);
									/*if (depth < context[0]) {
									    return false;
									} else */
									if (depth == (context[0])) {
										context[context[0]] = index;
										return true;
									}
								}
								index--;
							} // what condition          	    
							return false;
						}
				}

			default :
				throw new NavException("illegal navigation options");
		}

	}
	/**
	 * A generic navigation method.
	 * Move the current to the element according to the direction constants and the element name
	 * If no such element, no position change and return false.
	 * "*" matches any element
	 * Creation date: (12/2/03 1:43:50 PM)
	 * Legal direction constants are 	<br>
	 * <pre>		ROOT            0  </pre>
	 * <pre>		PARENT          1  </pre>
	 * <pre>		FIRST_CHILD     2  </pre>
	 * <pre>		LAST_CHILD      3  </pre>
	 * <pre>		NEXT_SIBLING    4  </pre>
	 * <pre>		PREV_SIBLING    5  </pre>
	 * <br>
	 * for ROOT and PARENT, element name will be ignored.
	 * @return boolean
	 * @param direction int
	 * @param en String     
	 * @exception com.ximpleware.NavException  When direction value is illegal. Or there are errors 
	 * in underlying byte representation of the document
	 * @exception IllegalArguementException if en is null
	 */
	public boolean toElement(int direction, String en) throws NavException {
		int size;
		int temp;
		int d;
		int val=0;
		if (en == null)
			throw new IllegalArgumentException(" Element name can't be null ");
		if (en.equals("*"))
			return toElement(direction);
		switch (direction) {
			case ROOT :
				return toElement(ROOT);

			case PARENT :
				return toElement(PARENT);

			case FIRST_CHILD :
				if (toElement(FIRST_CHILD) == false)
					return false;
				// check current element name
				if (matchElement(en) == false) {
					if (toElement(NEXT_SIBLING, en) == true)
						return true;
					else {
						//toParentElement();
						context[context[0]] = 0xffffffff;
						context[0]--;
						return false;
					}
				} else
					return true;

			case LAST_CHILD :
				if (toElement(LAST_CHILD) == false)
					return false;
				if (matchElement(en) == false) {
					if (toElement(PREV_SIBLING, en) == true)
						return true;
					else {
						context[context[0]] = 0xffffffff;
						context[0]--;
						//toParentElement();
						return false;
					}
				} else
					return true;

			case NEXT_SIBLING :
				d = context[0];
				switch(d)
				{
				  case 1: val = l1index; break;
				  case 2: val = l2index; break;
				  case 3: val = l3index; break;
				  	default:
				}
				temp = context[d]; // store the current position
				if (d == 0)
					return false;
				while (toElement(NEXT_SIBLING)) {
					if (matchElement(en)) {
						return true;
					}
				}
				switch(d)
				{
				  case 1: l1index = val; break;
				  case 2: l2index = val; break;
				  case 3: l3index = val; break;
				  	default:
				}
				context[d] = temp;
				return false;

			case PREV_SIBLING :
				d = context[0];
				switch(d)
				{
				  case 1: val = l1index; break;
				  case 2: val = l2index; break;
				  case 3: val = l3index; break;
				  	default:
				}
				temp = context[d]; // store the current position
				if (d == 0)
					return false;
				while (toElement(PREV_SIBLING)) {
					if (matchElement(en)) {
						return true;
					}
				}
				switch(d)
				{
				  case 1: l1index = val; break;
				  case 2: l2index = val; break;
				  case 3: l3index = val; break;
				  	default:
				}
				context[d] = temp;
				return false;

			default :
				throw new NavException("illegal navigation options");
		}
	}
	/**
	 * A generic navigation method with namespace support.
	 * Move the current to the element according to the direction constants and the prefix and local names
	 * If no such element, no position change and return false.
	 * URL * matches any namespace, including undefined namespaces
	 * a null URL means hte namespace prefix is undefined for the element
	 * ln *  matches any localname
	 * Creation date: (12/2/03 1:43:50 PM)
	 * Legal direction constants are<br>
	 * <pre>		ROOT            0  </pre>
	 * <pre>		PARENT          1  </pre>
	 * <pre>		FIRST_CHILD     2  </pre>
	 * <pre>		LAST_CHILD      3  </pre>
	 * <pre>		NEXT_SIBLING    4  </pre>
	 * <pre>		PREV_SIBLING    5  </pre>
	 * <br>
	 * for ROOT and PARENT, element name will be ignored.
	 * If not ns enabled, return false immediately with no position change.
	 * @return boolean
	 * @param direction int
	 * @param en String     
	 * @exception com.ximpleware.NavException  When direction value is illegal. Or there are errors 
	 * in underlying byte representation of the document
	 * @exception IllegalArguementException if ln is null
	 */
	public boolean toElementNS(int direction, String URL, String ln)
		throws NavException {
		int size;
		int temp;
		int val=0;
		int d; // temp location
		if (ns == false)
			return false;
		switch (direction) {
			case ROOT :
				return toElement(ROOT);

			case PARENT :
				return toElement(PARENT);

			case FIRST_CHILD :
				if (toElement(FIRST_CHILD) == false)
					return false;
				// check current element name
				if (matchElementNS(URL, ln) == false) {
					if (toElementNS(NEXT_SIBLING, URL, ln) == true)
						return true;
					else {
						//toParentElement();
						context[context[0]] = 0xffffffff;
						context[0]--;
						return false;
					}
				} else
					return true;

			case LAST_CHILD :
				if (toElement(LAST_CHILD) == false)
					return false;
				if (matchElementNS(URL, ln) == false) {
					if (toElementNS(PREV_SIBLING, URL, ln) == true)
						return true;
					else {
						context[context[0]] = 0xffffffff;
						context[0]--;
						//toParentElement();
						return false;
					}
				} else
					return true;

			case NEXT_SIBLING :
				d = context[0];
				temp = context[d]; // store the current position
				switch(d)
				{
				  case 1: val = l1index; break;
				  case 2: val = l2index; break;
				  case 3: val = l3index; break;
				  	default:
				}
				if (d == 0)
					return false;
				while (toElement(NEXT_SIBLING)) {
					if (matchElementNS(URL, ln)) {
						return true;
					}
				}
				switch(d)
				{
				  case 1: l1index = val; break;
				  case 2: l2index = val; break;
				  case 3: l3index = val; break;
				  	default:
				}
				context[d] = temp;
				return false;

			case PREV_SIBLING :
				d = context[0];
				temp = context[d]; // store the current position
				switch(d)
				{
				  case 1: val = l1index; break;
				  case 2: val = l2index; break;
				  case 3: val = l3index; break;
				  	default:
				}
				if (d == 0)
					return false;
				while (toElement(PREV_SIBLING)) {
					if (matchElementNS(URL, ln)) {
						return true;
					}
				}
				switch(d)
				{
				  case 1: l1index = val; break;
				  case 2: l2index = val; break;
				  case 3: l3index = val; break;
				  	default:
				}
				context[d] = temp;
				return false;

			default :
				throw new NavException("illegal navigation options");
		}

	}
	/**
	 * This method normalizes a token into a string in a way that resembles DOM.
	 * The leading and trailing white space characters will be stripped.
	 * The entity and character references will be resolved
	 * Multiple whitespaces char will be collapsed into one.
	 * Whitespaces via entities will nonetheless be preserved.
	 * Creation date: (12/8/03 1:57:10 PM)
	 * @return java.lang.String
	 * @param index int
	 * @exception NavException When the encoding has errors
	 */
	public String toNormalizedString(int index) throws NavException {
		int type = getTokenType(index);
		if (type!=TOKEN_CHARACTER_DATA &&
				type!= TOKEN_ATTR_VAL)
			return toRawString(index); 
		
		int len;
		if (type == TOKEN_STARTING_TAG
			|| type == TOKEN_ATTR_NAME
			|| type == TOKEN_ATTR_NS)
			len = getTokenLength(index) & 0xffff;
		else
			len = getTokenLength(index);
		if (len == 0)
			return "";
		currentOffset = getTokenOffset(index);
		int endOffset = len + currentOffset - 1; // point to the last character
		StringBuffer sb = new StringBuffer(len);
		String s = null;
		int ch;

		// trim off the leading whitespaces

		while (true) {
			int temp = currentOffset;
			ch = getChar();

			if (!isWS(ch)) {
				currentOffset = temp;
				break;
			}
		}

		boolean d = false;
		while (currentOffset <= endOffset) {
			ch = getCharResolved();
			if (isWS(ch) && getCharUnit(currentOffset - 1) != ';') {
				d = true;
			} else {
				if (d == false)
					sb.append((char) ch); // java only supports 16 bit unicode
				else {
					sb.append(' ');
					sb.append((char) ch);
					d = false;
				}
			}
		}

		return sb.toString();
	}

	/**
	 * Convert a token at the given index to a String, (built-in entity and char references not resolved)
	 * (entities and char references not expanded).
	 * Creation date: (11/16/03 7:28:49 PM)
	 * @return java.lang.String
	 * @param index int
	 * @exception NavException When the encoding has errors
	 */
	public String toRawString(int index) throws NavException {
		int type = getTokenType(index);
		int len;
		if (type == TOKEN_STARTING_TAG
			|| type == TOKEN_ATTR_NAME
			|| type == TOKEN_ATTR_NS)
			len = getTokenLength(index) & 0xffff;
		else
			len = getTokenLength(index);
		int offset = getTokenOffset(index);

		currentOffset = getTokenOffset(index);
		int endOffset = len + currentOffset;
		StringBuffer sb = new StringBuffer(len);

		while (currentOffset < endOffset) {
			char c = (char) getChar();
			sb.append(c); // java only support 16 bit unit code
		}
		return sb.toString();

	}
	/**
	 * Convert a token at the given index to a String, (entities and char references resolved).
	 * An attribute name or an element name will get the UCS2 string of qualified name 
	 * Creation date: (11/16/03 7:27:19 PM)
	 * @return java.lang.String
	 * @param int index
	 * @exception NavException
	 */
	public String toString(int index) throws NavException {
		int type = getTokenType(index);
		if (type!=TOKEN_CHARACTER_DATA &&
				type!= TOKEN_ATTR_VAL)
			return toRawString(index); 
		int len;
		if (type == TOKEN_STARTING_TAG
			|| type == TOKEN_ATTR_NAME
			|| type == TOKEN_ATTR_NS)
			len = getTokenLength(index) & 0xffff;
		else
			len = getTokenLength(index);
		currentOffset = getTokenOffset(index);
		int endOffset = len + currentOffset;
		StringBuffer sb = new StringBuffer(len);
		String s = null;

		while (currentOffset < endOffset) {
			char c = (char) getCharResolved();
			sb.append(c); // java only support 16 bit unit code
		}

		return sb.toString();
	}
}
