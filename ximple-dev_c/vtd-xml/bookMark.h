/*
 * Copyright (C) 2002-2009 XimpleWare, info@ximpleware.com
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
#ifndef BM_H
#define BN_H

#include "customTypes.h"
#include "vtdNav.h"
#include "cexcept.h"
#include "xpath.h"

typedef struct bookMark{
	int* ba;
	int ba_len;
	VTDNav *vn1;
} BookMark;

BookMark *createBookMark();
BookMark *createBookMark2(VTDNav *vn);
void freeBookMark(BookMark *bm);
void unbind4BookMark(BookMark *bm);
void bind4BookMark(BookMark *bm,VTDNav *vn);
VTDNav* getNav4BookMark(BookMark *bm);
Boolean setCursorPosition(BookMark *bm, VTDNav *vn);
Boolean setCursorPosition2(BookMark *bm);
Boolean recordCursorPosition(BookMark *bm, VTDNav *vn);
Boolean recordCursorPosition2(BookMark *bm);

#endif