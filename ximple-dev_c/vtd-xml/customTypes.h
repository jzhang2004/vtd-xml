#ifndef CUSTOMTYPE_H
#define CUSTOMTYPE_H
#include <TCHAR.h>
#include "cexcept.h"
#include <string.h>

#define inline __inline
#define _unicode

typedef _TCHAR UCS2Char;
typedef long long Long;
typedef char Byte;
typedef enum Bool {FALSE, 
					  TRUE} 
					Boolean;

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
										 navigation_exception,
										 other};

					typedef struct vtd_exception {
						enum exception_type et;
						int subtype; // subtype to be defined later
						const char* msg;
						const char* sub_msg;
					} exception;
					
					define_exception_type(exception);
					extern struct exception_context the_exception_context[1];

#endif