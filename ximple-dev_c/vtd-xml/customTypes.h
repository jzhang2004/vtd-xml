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
#ifndef CUSTOMTYPE_H
#define CUSTOMTYPE_H
#include <TCHAR.h>
#include "cexcept.h"
#include <string.h>
//#include <float.h>
// those are for min and max of a float number
#define MAXFLOAT 3.402823466e+38F
#define MINFLOAT 1.175494351e-38F

#define MAXINT  0x7fffffff
#define MININT 0x80000001

#define MAXLONG 0x7fffffffffffffff
#define MINLONG 0x8000000000000001

#define BIG_ENDIAN 0 // big endian   --> 1
					 // small endian --> 0

#define inline __inline
#define _unicode

typedef _TCHAR UCS2Char;
typedef long long Long;
typedef char Byte;
typedef unsigned char UByte;
// VTD-XML's own definition of Boolean
typedef enum Bool {FALSE, 
					  TRUE} 
					Boolean;

// VTD navigation parameters
typedef enum direction {ROOT,
						PARENT,
						FIRST_CHILD,
						LAST_CHILD,
						NEXT_SIBLING,
						PREV_SIBLING} 
					navDir;


typedef enum XMLencoding {FORMAT_ASCII,
						  FORMAT_ISO_8859,
						  FORMAT_UTF8,
						  FORMAT_UTF_16BE,
						  FORMAT_UTF_16LE} 
					encoding;

typedef enum VTDtokentype {TOKEN_STARTING_TAG,
						   TOKEN_ENDING_TAG,
						   TOKEN_ATTR_NAME,
						   TOKEN_ATTR_NS,
						   TOKEN_ATTR_VAL,
						   TOKEN_CHARACTER_DATA,
						   TOKEN_COMMENT,
						   TOKEN_PI_NAME,
						   TOKEN_PI_VAL,
						   TOKEN_DEC_ATTR_NAME,
						   TOKEN_DEC_ATTR_VAL,
						   TOKEN_CDATA_VAL,
						   TOKEN_DTD_VAL}
					tokenType;

					enum exception_type {out_of_mem, 
					  					 invalid_argument,
										 array_out_of_bound,
										 parse_exception,
										 nav_exception,
										 pilot_exception,
										 number_format_exception,
										 other};

					typedef struct vtd_exception {
						enum exception_type et;
						int subtype; // subtype to be defined later
						const char* msg;
						const char* sub_msg;
					} exception;
					
					define_exception_type(exception);
					extern struct exception_context the_exception_context[1];

					inline int swap_bytes(int i){
						return (((i & 0xff) << 24) |
							((i & 0xff00) <<8) |
							((i & 0xff0000) >> 8) |
							((i & 0xff000000) >> 24)&0xff);
					}
#endif