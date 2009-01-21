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
#include "bookMark.h"

BookMark *createBookMark(){
	BookMark *bm = (BookMark*) malloc(sizeof(BookMark));
	if (bm==NULL) {
		throwException2(out_of_mem,
			"BookMark allocation failed ");
	}
	bm->ba = NULL;
	bm->ba_len = -1;
	bm->vn1 = NULL;
	return bm;
}
BookMark *createBookMark2(VTDNav *vn){
	BookMark *bm = (BookMark*) malloc(sizeof(BookMark));
	if (bm==NULL) {
		throwException2(out_of_mem,
			"BookMark allocation failed ");
	}
	bm->ba = NULL;
	bm->ba_len = -1;
	bm->vn1 = NULL;

	bind4BookMark(bm,vn);
	recordCursorPosition2(bm);

	return bm;
}

void freeBookMark(BookMark *bm){
	free(bm->ba);
	free(bm);
}
void unbind4BookMark(BookMark *bm){
	bm->vn1 = NULL;
}
void bind4BookMark(BookMark *bm, VTDNav *vn){
		if (vn==NULL)
            throwException2(invalid_argument,"vn can't be null");
        bm->vn1 = vn;
		if (bm->ba == NULL || vn->nestingLevel+8 != bm->ba_len){
			if (vn->nestingLevel+8 != bm->ba_len){
				free(bm->ba);
			}
            bm->ba = malloc(sizeof(int)*(vn->nestingLevel + 8)); 
			if (bm->ba == NULL){
				throwException2(out_of_mem,
			"BookMark.ba allocation failed ");
			}
		}
        bm->ba[0]= -2 ; // this would never happen in a VTDNav obj's context
}
VTDNav* getNav4BookMark(BookMark *bm){
	return bm->vn1;
}
Boolean setCursorPosition(BookMark *bm, VTDNav *vn){
	int i=0;
	if (bm->vn1 != vn || bm->ba == NULL || bm->ba[0] == -2)
		return FALSE;
	for (i = 0; i < vn->nestingLevel; i++) {
		vn->context[i] = bm->ba[i];
	}

	vn->l1index = bm->ba[vn->nestingLevel];
	vn->l2index = bm->ba[vn->nestingLevel + 1];
	vn->l3index = bm->ba[vn->nestingLevel + 2];
	vn->l2lower = bm->ba[vn->nestingLevel + 3];
	vn->l2upper = bm->ba[vn->nestingLevel + 4];
	vn->l3lower = bm->ba[vn->nestingLevel + 5];
	vn->l3upper = bm->ba[vn->nestingLevel + 6];
	if (bm->ba[vn->nestingLevel+7] < 0){
		vn->atTerminal = TRUE;		    
	} else
		vn->atTerminal = FALSE;

	vn->LN = bm->ba[vn->nestingLevel+7] & 0x7fffffff;
	return TRUE;
}
Boolean setCursorPosition2(BookMark *bm){
	return setCursorPosition(bm,bm->vn1);
}

Boolean recordCursorPosition(BookMark *bm, VTDNav *vn){
	int i;
	if (vn == NULL)
		return FALSE;
	if (vn== bm->vn1){
	}else {
		bind4BookMark(bm,vn);
	}
	for (i = 0; i < vn->nestingLevel; i++) {
		bm->ba[i] = bm->vn1->context[i];
	}

	bm->ba[vn->nestingLevel]= vn->l1index ;
	bm->ba[vn->nestingLevel + 1]= vn->l2index ;
	bm->ba[vn->nestingLevel + 2]= vn->l3index ;
	bm->ba[vn->nestingLevel + 3]= vn->l2lower ;
	bm->ba[vn->nestingLevel + 4]= vn->l2upper ;
	bm->ba[vn->nestingLevel + 5]= vn->l3lower ;
	bm->ba[vn->nestingLevel + 6]= vn->l3upper ;
	//ba[vn.nestingLevel + 7]=(vn.atTerminal == true)?1:0;
	bm->ba[vn->nestingLevel + 7]= 
		(vn->atTerminal == TRUE)? 
		(vn->LN | 0x80000000) : vn->LN ;
	return TRUE;
}
Boolean recordCursorPosition2(BookMark *bm){
	return recordCursorPosition(bm,bm->vn1);
}