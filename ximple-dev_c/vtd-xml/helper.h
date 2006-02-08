/* 
* Copyright (C) 2002-2006 XimpleWare, info@ximpleware.com
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
#ifndef HELPER_H
#define HELPER_H

#include "customTypes.h"
#include <stdlib.h>
struct obj {
   void* object;
   struct obj *next;
} ;
struct obj *objList;
struct obj *ptr1;
struct obj *ptr2;
   
void addObj(void *obj); // add to a global linked list during yyparse, to prevent mem leak
void freeAllObj();  // if there is anything wrong during parsing
void resetObj(); // this function is called if yyparse return 0

#endif
