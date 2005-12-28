/* 
 * Copyright (C) 2002-2005 XimpleWare, info@ximpleware.com
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
/* intHash implements a simple hash table that speeds up the 
   uniqueness checking */
#include "xpath1.h"

/* the constructor */
IntHash* createIntHash(){
	exception e;
	int i=0;
	IntHash *ih = (IntHash *) malloc(sizeof(IntHash));
	if (ih==NULL) {
		e.et = out_of_mem;
		e.msg = "IntHash allocation failed ";
		Throw e;
	}
	ih->storage = (FastIntBuffer **) malloc(sizeof(FastIntBuffer*)*256);
	/* initialize everything to null */
	for (i=0;i<hashWidth;i++){
		ih->storage[i]= NULL;
	}

	return ih;
}
/* free intHash */
void freeIntHash(IntHash *ih){
	int i=0;
	if (ih !=NULL){
		for (i=0;i<hashWidth;i++){
			freeFastIntBuffer(ih->storage[i]);
		}
	}
}


/* Test whether the input i is unique; 
   if so, insert into the hash table and return false
   otherwise, return true */
Boolean isUniqueIntHash(IntHash *ih,int i){
	exception e;
	int j,size;
	int temp = i & mask1;
	if (ih->storage[temp]==NULL) {
		ih->storage[temp]= createFastIntBuffer2(pageSizeE);
		if (ih->storage[temp]==NULL) {
			e.et = out_of_mem;
			e.msg = "FastIntBuffer allocation failed ";
			Throw e;
		}
		appendInt(ih->storage[temp],i);
		return TRUE;
	}        
	else{
		size = ih->storage[temp]->size;
		for (j=0;j<size;j++){
			if (i == intAt(ih->storage[temp],j)){
				return FALSE;
			}
		}
		appendInt(ih->storage[temp],i);
		return TRUE;            
	}
}

/* reset intHash */
void resetIntHash(IntHash *ih){
	int i=0;
	for (i=0;i<hashWidth;i++){
		if (ih->storage[i]!=NULL){
			clearFastIntBuffer(ih->storage[i]);
		}
	}
}