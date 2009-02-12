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
#include "textIter.h"
/* increment the index value to account for ultra long tokens */
static int increment(TextIter *ti, int sp);
/* Test whether a give token type is a TEXT.*/
static int isText(TextIter *ti, int i);

// create text iterator
TextIter *createTextIter(){
	TextIter *ti = NULL;
	ti = (TextIter *)malloc(sizeof(TextIter));
	
	if (ti == NULL){
		throwException2(out_of_mem,
			"createTextIter failed");
		return NULL;
	}
	ti->vn = NULL;
	
	return ti;
}
/* free TextIter */
void freeTextIter(TextIter* ti){
	free(ti);
}
/* increment the index value to account for ultra long tokens */
int increment(TextIter *ti, int sp){
	int type = getTokenType(ti->vn,sp);
    int vtdSize = ti->vn->vtdBuffer->size;
    int i=sp+1;
    while(i<vtdSize && 
    	ti->depth == getTokenDepth(ti->vn,i) && 
		type == getTokenType(ti->vn,i)&&
			(getTokenOffset(ti->vn,i-1)+ (int)((longAt(ti->vn->vtdBuffer, i-1) & MASK_TOKEN_FULL_LEN)>>32) 
			        == getTokenOffset(ti->vn,i))){
		i++;
    }      	
    return i;
}

/* Test whether a give token type is a TEXT.*/
int isText(TextIter *ti, int index){
	int type = getTokenType(ti->vn,index);
    return (type == TOKEN_CHARACTER_DATA
    //|| type == TOKEN_COMMENT
    || type == TOKEN_CDATA_VAL);
}

/* Obtain the current navigation position and element info from VTDNav.
 * So one can instantiate it once and use it for many different elements */
void touch(TextIter *ti, VTDNav *v){
	/*if (v == NULL || ti==NULL){
		e.et = invalid_argument;
		e.msg = "Touch failed VTDNav instance can't be null";
		Throw e;
	}*/     

	 ti->depth = v->context[0];
	 if (ti->depth == -1)
		 ti->index = 0;
	 else 
         ti->index = (ti->depth != 0) ? v->context[ti->depth] : v->rootIndex;

     ti->vn = v;
     ti->prevLocation = -1;
     ti->lcIndex = -1;
     ti->lcUpper = -1;
     ti->lcLower = -1;
}

/* Get the index vals for the text nodes in document order.*/
int getNext(TextIter *ti){
	int temp1, temp2, size;
	int sp,i,d, type;
	FastLongBuffer *l1Buffer = ti->vn->l1Buffer;
	FastLongBuffer *l2Buffer = ti->vn->l2Buffer;
	FastIntBuffer *l3Buffer = ti->vn->l3Buffer;

    int vtdSize = ti->vn->vtdBuffer->size;
    switch (ti->depth) {
		case -1: return -1;
        case 0 :
            // scan forward, if none found, jump to level 1 elements and scan backward until one is found
            // if there isn't a level-one element, jump to the end of vtd buffer and scan backward
            sp = (ti->prevLocation != -1) ? increment(ti,ti->prevLocation): (ti->index + 1);
			if (l1Buffer->size != 0) {
				//l1Buffer = ti->vn->l1Buffer;
                temp1 = upper32At(l1Buffer,0);
                temp2 = upper32At(l1Buffer,l1Buffer->size - 1);
                ti->lcIndex = (ti->lcIndex != -1) ? ti->lcIndex : 0;
                while (sp < vtdSize) {
                    if (sp >= temp1 && sp < temp2) {
                        int s = upper32At(l1Buffer,ti->lcIndex);
                        if (sp == s) { // get to the next l1 element then do a rewind
                            ti->lcIndex++;
                            sp = upper32At(l1Buffer,ti->lcIndex)-1;
                            while (getTokenDepth(ti->vn,sp) == 0
                                && getTokenType(ti->vn,sp) != TOKEN_STARTING_TAG) { //probe depth in here
                                sp--;
                            }
                            sp++; // point to the first possible node  
                        }
                        if (isText(ti,sp) == TRUE && getTokenDepth(ti->vn,sp)==0) {
                            ti->prevLocation = sp;
                            return sp;
                        }
                        sp++;
                    } else if (sp < temp1) {
                        if (isText(ti,sp) == TRUE && getTokenDepth(ti->vn,sp)==0) {
                            ti->prevLocation = sp;
                            return sp;
                        }
                        sp++;
                    } else {
                        if (sp == temp2) { // get to the end of the document and do a rewind
                            sp = ti->vn->vtdBuffer->size - 1;
                            while (getTokenDepth(ti->vn,sp) <= 0 ) {
                                sp--;
                            }
                            sp++;
                            //continue;
                        }
                        if (sp>=vtdSize)
							return -1;
                        else if (isText(ti,sp) == TRUE && getTokenDepth(ti->vn,sp)==0) {
                            ti->prevLocation = sp;
                            return sp;
                        }
                        else if (getTokenDepth(ti->vn,sp)>1 ) {
                            break;
                        }
                        sp++;
                    }
                }
                //ti->prevLocation = vtdSize-1;
                return -1;
                // found nothing
            } else {
                // no child element for root, just scan right forward
                while (sp < vtdSize) {
                    if (isText(ti,sp) == TRUE && getTokenDepth(ti->vn,sp)==0) {
                        ti->prevLocation = sp;
                        return sp;
                    }
                    sp++;
                }
                return -1;
            }
        case 1 :
            if (ti->prevLocation != -1) {
                sp = increment(ti,ti->prevLocation) ;
            } else {
                // fetch lclower and lcupper
                ti->lcLower = lower32At(l1Buffer,ti->vn->l1index);
                if (ti->lcLower != -1) {
                    ti->lcUpper = l2Buffer->size - 1;
                    size = l1Buffer->size;
                    for (i = ti->vn->l1index + 1; i < size ; i++) {
                        int temp = lower32At(l1Buffer, i);
                        if (temp != 0xffffffff) {
                            ti->lcUpper = temp - 1;
                            break;
                        }
                    }
                }
                sp = ti->index + 1;
            } // check for l2lower and l2upper

            if (ti->lcLower != -1) { // have at least one child element
                temp1 = upper32At(l2Buffer,ti->lcLower);
                temp2 = upper32At(l2Buffer, ti->lcUpper);
                ti->lcIndex = (ti->lcIndex != -1) ? ti->lcIndex : ti->lcLower;
                while (sp < vtdSize) {
                    int s = upper32At(l2Buffer, ti->lcIndex);
                    if (sp >= temp1 && sp < temp2) {
                        if (sp == s) {
                            ti->lcIndex++;
                            sp = upper32At(l2Buffer, ti->lcIndex) - 1;
                            while (getTokenDepth(ti->vn, sp) == 1) {
                                sp--;
                            }
                            sp++;
                            //continue;
                        }
                        if (isText(ti,sp) == TRUE && getTokenDepth(ti->vn,sp)==1) {
                            ti->prevLocation = sp;
                            return sp;
                        }
                        sp++;
                    } else if (sp < temp1) {
                        if (isText(ti,sp) == TRUE && getTokenDepth(ti->vn,sp)==1) {
                            ti->prevLocation = sp;
                            return sp;
                        }
                        sp++;
                    } else {
                        //if (sp == temp2) { // last child element
                        //} else
						
                        if (isText(ti,sp) == TRUE && getTokenDepth(ti->vn,sp) == 1){
                        	//System.out.println("depth ->"+nr->vn->getTokenDepth(sp));
                            ti->prevLocation = sp;
                            return sp;
                        } else if ((getTokenType(ti->vn,sp)== TOKEN_STARTING_TAG
                                && getTokenDepth(ti->vn,sp) < 2 ) || getTokenDepth(ti->vn,sp)<1) {
                            break;
                        }
                        sp++;
                    }                    
                }
                //ti->prevLocation = vtdSize-1;
                return -1;
            } else { // no child element
				if (sp>=vtdSize) return -1;{
                int d = getTokenDepth(ti->vn,sp);
                int type = getTokenType(ti->vn,sp);
                while (sp < vtdSize
                    && d >= 1
                    && !(d == 1 && type == TOKEN_STARTING_TAG)) {
                    if (isText(ti,sp) == TRUE) {
                        ti->prevLocation = sp;
                        return sp;
                    }
                    sp++;
                    d = getTokenDepth(ti->vn, sp);
                    type = getTokenType(ti->vn,sp);                    
                }
                //ti->prevLocation = vtdSize-1;
                return -1;
				}
            }

        case 2 :
            if (ti->prevLocation != -1) {
                sp = increment(ti,ti->prevLocation);
            } else {
                // fetch lclower and lcupper
                ti->lcLower = lower32At(l2Buffer, ti->vn->l2index);
                if (ti->lcLower != -1) {
                    ti->lcUpper = l3Buffer->size - 1;
                    size = l2Buffer->size;
                    for (i = ti->vn->l2index + 1; i < size ; i++) {
                        int temp = lower32At(l2Buffer,i);
                        if (temp != 0xffffffff) {
                            ti->lcUpper = temp - 1;
                            break;
                        }
                    }
                }
                sp = ti->index + 1;
            } // check for l3lower and l3upper

            if (ti->lcLower != -1) { // at least one child element
                temp1 = intAt(l3Buffer,ti->lcLower);
                temp2 = intAt(l3Buffer,ti->lcUpper);
                ti->lcIndex = (ti->lcIndex != -1) ? ti->lcIndex : ti->lcLower;
                while (sp < vtdSize) {
                    int s = intAt(l3Buffer, ti->lcIndex);
                    //int s = vn.l2Buffer.upper32At(ti->lcIndex);
                    if (sp >= temp1 && sp < temp2) {
                        if (sp == s) {
                            ti->lcIndex++;
                            sp = intAt(l3Buffer, ti->lcIndex) - 1;
                            while (getTokenDepth(ti->vn, sp) == 2) {
                                sp--;
                            }
                            sp++;
                            //continue;
                        }
                        if (isText(ti,sp) == TRUE && getTokenDepth(ti->vn,sp)==2) {
                            ti->prevLocation = sp;
                            return sp;
                        }
                        sp++;
                    } else if (sp < temp1) {
                        if (isText(ti,sp) == TRUE && getTokenDepth(ti->vn,sp)==2) {
                            ti->prevLocation = sp;
                            return sp;
                        }
                        sp++;
                    } else {
                        //if (sp == temp2) { // last child element
                        //} else 
						if (isText(ti,sp) == TRUE && getTokenDepth(ti->vn,sp) == 2) {
                            ti->prevLocation = sp;
                            return sp;
                        } else if ((getTokenType(ti->vn,sp)== TOKEN_STARTING_TAG
                                && getTokenDepth(ti->vn,sp) < 3 ) || getTokenDepth(ti->vn,sp)<2) {
                            break;
                        }
                        sp++;
                    }
                }
                //ti->prevLocation = vtdSize-1;
                return -1;
            } else { // no child elements
				if (sp>=vtdSize) return -1;{
                int d = getTokenDepth(ti->vn,sp);
                int type = getTokenType(ti->vn,sp);
                while (sp < vtdSize
                    && d >= 2
                    && !(d == 2 && type == TOKEN_STARTING_TAG)) {
                    // the last condition indicates the start of the next sibling element
                    if (isText(ti,sp) == TRUE) {
                        ti->prevLocation = sp;
                        return sp;
                    }
                    sp++;
                    d = getTokenDepth(ti->vn,sp);
                    type = getTokenType(ti->vn,sp);
                    
                }
                //ti->prevLocation = vtdSize-1;
                return -1;
				}
            }

        default :
            //int curDepth = vn.context[0];
            sp = (ti->prevLocation != -1) ? increment(ti,ti->prevLocation): ti->index + 1;
			if (sp>=vtdSize) return -1;
            d = getTokenDepth(ti->vn, sp);
            type = getTokenType(ti->vn, sp);
            while (d >= ti->depth
                && !(d == ti->depth && type == TOKEN_STARTING_TAG)) {
                if (isText(ti, sp) == TRUE&& d == ti->depth) {
                    ti->prevLocation = sp;
                    return sp;
                }
                sp++;
				if(sp >= vtdSize)
				return -1;

                d = getTokenDepth(ti->vn, sp);
                type = getTokenType(ti->vn, sp);
             }
    }
    //ti->prevLocation = vtdSize-1;
    return -1;
}
