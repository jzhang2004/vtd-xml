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

/**
 * Exception class for document size exceeding maximum supported 
 * 1GB for UTF-8, ASCII and ISO-8859-1; 2GB for UTF-16BE, UTF-16LE
 * Creation date: (7/17/03 6:13:55 PM)
 * @author: 
 */
public class XMLDocumentSizeException extends RuntimeException {
/**
 * XMLDocumentSizeException constructor comment.
 */
public XMLDocumentSizeException() {
	super();
}
/**
 * XMLDocumentSizeException constructor comment.
 * @param s java.lang.String
 */
public XMLDocumentSizeException(String s) {
	super(s);
}
}
