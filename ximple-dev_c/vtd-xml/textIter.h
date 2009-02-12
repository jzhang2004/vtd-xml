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
#ifndef TEXTITER_H
#define TEXTITER_H

#include "customTypes.h"
#include "vtdNav.h"
#include "cexcept.h"

typedef struct textIter{
	int prevLocation;
	int depth;
	int index;
	VTDNav *vn;
	int lcIndex;
	int lcLower;
	int lcUpper;
} TextIter;

/* create TextIter */
TextIter *createTextIter();
/* free TextIter */
void freeTextIter(TextIter* ti);
/* Obtain the current navigation position and element info from VTDNav.
 * So one can instantiate it once and use it for many different elements */
void touch(TextIter *ti, VTDNav *vn);
/* Get the index vals for the text nodes in document order.*/
int getNext(TextIter *ti);

#endif
