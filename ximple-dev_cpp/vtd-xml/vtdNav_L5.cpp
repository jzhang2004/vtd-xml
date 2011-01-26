/* 
* Copyright (C) 2002-2011 XimpleWare, info@ximpleware.com
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
#include "vtdNav_L5.h"
//#include "decoder.h"
#include "bookMark.h"
#include "elementFragmentNs.h"

//#include <exception>
using namespace com_ximpleware;
using namespace std;

void VTDNav_L5::resolveLC_l3(){
		int temp = l2Buffer->lower32At(l2index);
		if (l3lower != temp) {
			l3lower = temp;
			// l2lower shouldn't be -1 !!!! l2lower and l2upper always get
			// resolved simultaneously
			l3index = l3lower;
			l3upper = l3Buffer->size - 1;
			for (int i = l2index + 1; i < l2Buffer->size; i++) {
				temp = l2Buffer->lower32At(i);
				if (temp != (int)0xffffffff) {
					l3upper = temp - 1;
					break;
				}
			}
		} // intelligent guess again ??

		if (l3index < 0 || l3index >= l3Buffer->size
				|| context[3] != l3Buffer->upper32At(l3index)) {
			
			if (l3index >= l3Buffer->size || l3index<0)
				l3index = l3lower;
			if (l3index+1< l3Buffer->size&& context[3] == l3Buffer->upper32At(l3index + 1))
				l3index = l3index + 1;
			else if (l3upper - l3lower >= 16) {
				int init_guess = l3lower
						+ (int) ((l3upper - l3lower)
								* ((float) context[3] - l3Buffer
										->upper32At(l3lower)) / (l3Buffer->upper32At(l3upper) - l3Buffer->upper32At(l3lower)));
				if (l3Buffer->upper32At(init_guess) > context[3]) {
					while (context[3] != l3Buffer->upper32At(init_guess))
						init_guess--;
				} else if (l3Buffer->upper32At(init_guess) < context[3]) {
					while (context[3] != l3Buffer->upper32At(init_guess))
						init_guess++;
				}
				l3index = init_guess;
			} else if (context[3]<l3Buffer->upper32At(l3index)){
				
				while ( context[3] != l3Buffer->upper32At(l3index)) {
					l3index--;
				}
			}
			else { 
				while(context[3]!=l3Buffer->upper32At(l3index))
					l3index++;
			}	
		}
}

void VTDNav_L5::resolveLC_l4(){
		int temp = l3Buffer->lower32At(l3index);
		if (l4lower != temp) {
			l4lower = temp;
			// l2lower shouldn't be -1 !!!! l2lower and l2upper always get
			// resolved simultaneously
			l4index = l4lower;
			l4upper = l4Buffer->size - 1;
			for (int i = l3index + 1; i < l3Buffer->size; i++) {
				temp = l3Buffer->lower32At(i);
				if (temp != (int)0xffffffff) {
					l4upper = temp - 1;
					break;
				}
			}
		} // intelligent guess again ??

		if (l4index < 0 || l4index >= l4Buffer->size
				|| context[4] != l4Buffer->upper32At(l4index)) {
			
			if (l4index >= l4Buffer->size || l4index<0)
				l4index = l4lower;
			if (l4index+1< l4Buffer->size&& context[4] == l4Buffer->upper32At(l4index + 1))
				l4index = l4index + 1;
			else if (l4upper - l4lower >= 16) {
				int init_guess = l4lower
						+ (int) ((l4upper - l4lower)
								* ((float) context[4] - l4Buffer->upper32At(l4lower)) / (l4Buffer->upper32At(l4upper) 
								- l4Buffer->upper32At(l4lower)));
				if (l4Buffer->upper32At(init_guess) > context[4]) {
					while (context[4] != l4Buffer->upper32At(init_guess))
						init_guess--;
				} else if (l4Buffer->upper32At(init_guess) < context[4]) {
					while (context[4] != l4Buffer->upper32At(init_guess))
						init_guess++;
				}
				l4index = init_guess;
			} else if (context[4]<l4Buffer->upper32At(l4index)){
				
				while ( context[4] != l4Buffer->upper32At(l4index)) {
					l4index--;
				}
			}
			else { 
				while(context[4]!=l4Buffer->upper32At(l4index))
					l4index++;
			}	
		}
}

void VTDNav_L5::resolveLC_l5(){
		int temp = l4Buffer->lower32At(l4index);
		if (l5lower != temp) {
			//l3lower and l3upper are always together
			l5lower = temp;
			// l3lower shouldn't be -1
			l5index = l5lower;
			l5upper = l5Buffer->size - 1;
			for (int i = l4index + 1; i < l4Buffer->size; i++) {
				temp = l4Buffer->lower32At(i);
				if (temp != (int)0xffffffff) {
					l5upper = temp - 1;
					break;
				}
			}
		}

		if (l5index < 0 || l5index >= l5Buffer->size
				|| context[5] != l5Buffer->intAt(l5index)) {
			if (l5index >= l5Buffer->size || l5index <0)
				l5index = l5lower;
			if (l5index+1 < l5Buffer->size &&
					context[5] == l5Buffer->intAt(l5index + 1))
				l5index = l5index + 1;
			else if (l5upper - l5lower >= 16) {
				int init_guess = l5lower
						+ (int) ((l5upper - l5lower) * ((float) (context[5] - l5Buffer->intAt(l5lower)) / (l5Buffer->intAt(l5upper) 
						- l5Buffer->intAt(l5lower))));
				if (l5Buffer->intAt(init_guess) > context[5]) {
					while (context[5] != l5Buffer->intAt(init_guess))
						init_guess--;
				} else if (l5Buffer->intAt(init_guess) < context[5]) {
					while (context[5] != l5Buffer->intAt(init_guess))
						init_guess++;
				}
				l5index = init_guess;
			} else if (context[5]<l5Buffer->intAt(l5index)){
				while (context[5] != l5Buffer->intAt(l5index)) {
					l5index--;
				}
			} else {
				while (context[5] != l5Buffer->intAt(l5index)) {
					l5index++;
				}
			}
		}
}

void VTDNav_L5::recoverNode_l4(int i){}

void VTDNav_L5::recoverNode_l5(int i){}

void VTDNav_L5::resolveLC(){

	if (context[0]<=0)
		return;
	resolveLC_l1();
	if (context[0] == 1)
		return;
	resolveLC_l2();
	if (context[0] == 2)
		return;	
	resolveLC_l3();
	if (context[0] == 3)
		return;	
	resolveLC_l4();
	if (context[0] == 4)
		return;	
	resolveLC_l5();
}


VTDNav_L5::VTDNav_L5(int r, 
					 encoding_t enc, 
					 bool ns1, 
					 int depth,
					 UByte *x, 
					 int xLen, 
					 FastLongBuffer *vtd, 
					 FastLongBuffer *l1,
					 FastLongBuffer *l2, 
					 FastLongBuffer *l3, 
					 FastLongBuffer *l4, 
					 FastIntBuffer *l5, 
					 int so, 
					 int len,
					 bool br1):
/*rootIndex(r),
nestingLevel(depth+1),
//context (new int[nestingLevel]),
atTerminal(false),
l2upper(-1),
l2lower(-1),
l3upper(-1),
l3lower(-1),
l2index(-1),
l3index(-1),
l1index(-1),
vtdBuffer(vtd),
l1Buffer(l1),
l2Buffer(l2),
//l3Buffer(l3),
XMLDoc(x),
offsetMask((ns1)? MASK_TOKEN_OFFSET1:MASK_TOKEN_OFFSET2),
LN(0),// record txt and attrbute for XPath eval purposes
encoding(enc),
ns(ns1),
docOffset(so), // starting offset of the XML doc wrt XMLDoc
docLen(len),   // size of XML document
vtdSize(vtd->size),// # of entries in vtdBuffer equvalent 
		// to calling size(FastLongBuffer *flb) defined in fastLongBuffer.h
bufLen(xLen), // size of XMLDoc in bytes
br(br1), // buffer reuse flag*/
VTDNav(r, enc,ns1,depth,x, xLen,vtd, l1,l2,
					  so, 
					  len,
					  br1),
l4index(-1),
l5index(-1),
l4lower(-1),
l4upper(-1),
l5lower(-1),
l5upper(-1),
l3Buffer(l3),
l4Buffer(l4),
l5Buffer(l5)
{								
								   if (l3 == NULL ||
									   l4 == NULL ||
									   l5 == NULL )
								   {
									   throw InvalidArgumentException(
										   "Invalid argument when creating VTDGen failed ");
								   }
								   

								   /*vn = (VTDNav *) malloc(sizeof(VTDNav));
								   if (vn==NULL){
								   throwException2(out_of_mem,							 
								   "VTDNav allocation failed ");
								   return NULL;
								   }*/


								   try{
									   contextBuf = new ContextBuffer(10, nestingLevel+15);
									   contextBuf2 = new ContextBuffer(10,nestingLevel+15);
								   }catch(std::bad_alloc&){
									   delete contextBuf;
									   delete contextBuf2;
									   //free(stackTemp);
									   delete[] context;
									   //free();
									   throw OutOfMemException(							 
										   "VTDNav allocation failed ");
									   //return NULL;
								   }

								   //stackTemp = (int *)malloc((nestingLevel+9)*sizeof(int));
								   stackTemp = new int[nestingLevel+15];
								   if (contextBuf == NULL 
									   || stackTemp == NULL){
										   delete contextBuf;
										   delete contextBuf2;
										   delete[] stackTemp;
										   delete[] context;
										   //free();
										   throw OutOfMemException(							 
											   "VTDNav allocation failed ");
										   //return NULL;
								   }

}

VTDNav_L5::~VTDNav_L5(){

	//delete (contextBuf);
	//delete (contextBuf2);
	if (br == false && master){
		//delete (vtdBuffer);
		//delete (l1Buffer);
		//delete (l2Buffer);
		delete (l3Buffer);
		delete (l4Buffer);
		delete (l5Buffer);
	}
	//delete[] context;
	//delete[] stackTemp;
	//free(XMLDoc);
}

//This method is similar to getElementByName in DOM except it doesn't
//return the nodeset, instead it iterates over those nodes.
bool VTDNav_L5::iterate(int dp, UCSChar *en, bool special){
// get the current depth
		int index = getCurrentIndex() + 1;
		int tokenType;
		//int size = vtdBuffer.size;
		while (index < vtdSize) {
		    tokenType = getTokenType(index);
			if (tokenType==TOKEN_ATTR_NAME
			        || tokenType == TOKEN_ATTR_NS){			  
			    index = index+2;
			    continue;
			}
			if (isElementOrDocument(index)) {
				int depth = getTokenDepth(index);
				if (depth > dp) {
					context[0] = depth;
					if (depth>0)
						context[depth] = index;
					if (special || matchElement(en)) {
						if (dp< 6)
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

//This method is similar to getElementByName in DOM except it doesn't
//return the nodeset, instead it iterates over those nodes .
//When URL is "*" it will match any namespace
//if ns is false, return false immediately
bool VTDNav_L5::iterateNS(int dp, UCSChar *URL, UCSChar *ln){
		if (ns == false)
			return false;
		int tokenType;
		int index = getCurrentIndex() + 1;
		while (index < vtdSize) {
		    tokenType = getTokenType(index);
			if(tokenType==TOKEN_ATTR_NAME
			        || tokenType == TOKEN_ATTR_NS){
			    index = index+2;
			    continue;
			}
			if (isElementOrDocument(index)) {
				int depth = getTokenDepth(index);
				if (depth > dp) {
					context[0] = depth;
					if (depth>0)
						context[depth] = index;
					if (matchElementNS(URL, ln)) {
						if (dp < 6)
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

// This function is called by selectElement_P in autoPilot
bool VTDNav_L5::iterate_preceding(UCSChar *en, int* a, bool special){
		int index = getCurrentIndex() - 1;
		int t,d;
		//int depth = getTokenDepth(index);
		//int size = vtdBuffer.size;
		while (index >  0) {
			if (isElementOrDocument(index)) {
				int depth = getTokenDepth(index);
				context[0] = depth;
				//context[depth]=index;
				if (depth>0){
					context[depth] = index;
					t = index -1;
					for (int i=depth-1;i>0;i--){
						if (context[i]>index || context[i] == -1){
							while(t>0){
								d = getTokenDepth(t);
								if ( d == i && isElement(t)){
									context[i] = t;
									break;
								}
								t--;
							}							
						}else
							break;
					}
				}
				//dumpContext();
				if (index!= a[depth] && (special || matchElement(en))) {
					if (depth <6)
						resolveLC();
					return true;
				}
			} 
			index--;
		}
		return false;	
}

// This function is called by selectElementNS_P in autoPilot
bool VTDNav_L5::iterate_precedingNS(UCSChar *URL, UCSChar *ln, int* a){
		int index = getCurrentIndex() - 1;
		int t,d;
		//int depth = getTokenDepth(index);
		//int size = vtdBuffer.size;
		while (index > 0 ) {
			if (isElementOrDocument(index)) {
				int depth = getTokenDepth(index);
				context[0] = depth;
				//context[depth]=index;
				if (depth>0){
					context[depth] = index;
					t = index -1;
					for (int i=depth-1;i>0;i--){
						if (context[i]>index || context[i]==-1){
							while(t>0){
								d = getTokenDepth(t);
								if ( d == i && isElement(t)){
									context[i] = t;
									break;
								}
								t--;
							}							
						}else
							break;
					}
				}
				//dumpContext();
				if (index != a[depth] && matchElementNS(URL,ln)) {	
					if (depth <6)
						resolveLC();
					return true;
				}
			} 
			index--;
		}
		return false;
}

// This function is called by selectElement_F in autoPilot
bool VTDNav_L5::iterate_following(UCSChar *en, bool special){
		int index = getCurrentIndex() + 1;
		//int size = vtdBuffer.size;
		while (index < vtdSize) {
			if (isElementOrDocument(index)) {
				int depth = getTokenDepth(index);
				context[0] = depth;
				if (depth>0)
					context[depth] = index;
				if (special || matchElement(en)) {	
					if (depth <6)
					  resolveLC();
					return true;
				}
			} 
			index++;
		}
		return false;	
}


// This function is called by selectElementNS_F in autoPilot
bool VTDNav_L5::iterate_followingNS( UCSChar *URL, UCSChar *ln){
		int index = getCurrentIndex() + 1;
		//int size = vtdBuffer.size;
		while (index < vtdSize) {
			if (isElementOrDocument(index)) {
				int depth = getTokenDepth(index);
				context[0] = depth;
				if (depth>0)
					context[depth] = index;
				if (matchElementNS(URL,ln)) {	
					if (depth <6)
						resolveLC();
					return true;
				}
			} 
			index++;
		}
		return false;
}

//void resolveLC_l3();
//void resolveLC_l4();
//void resolveLC_l5();
void VTDNav_L5::recoverNode_l3(int i){}




//Load the context info from ContextBuffer.
//Info saved including LC and current state of the context 
bool VTDNav_L5::pop(){
		bool b = contextBuf->load(stackTemp);
		if (b == false)
			return false;
		for (int i = 0; i < nestingLevel; i++) {
			context[i] = stackTemp[i];
		}

		l1index = stackTemp[nestingLevel];
		l2index = stackTemp[nestingLevel + 1];
		l3index = stackTemp[nestingLevel + 2];
		l4index = stackTemp[nestingLevel + 3];
		l5index = stackTemp[nestingLevel + 4];
		l2lower = stackTemp[nestingLevel + 5];
		l2upper = stackTemp[nestingLevel + 6];
		l3lower = stackTemp[nestingLevel + 7];
		l3upper = stackTemp[nestingLevel + 8];
		l4lower = stackTemp[nestingLevel + 9];
		l4upper = stackTemp[nestingLevel + 10];
		l5lower = stackTemp[nestingLevel + 11];
		l5upper = stackTemp[nestingLevel + 12];
		atTerminal = (stackTemp[nestingLevel + 13] == 1);
		LN = stackTemp[nestingLevel+14];
		return true;
}

bool VTDNav_L5::pop2(){
		bool b = contextBuf2->load(stackTemp);
		if (b == false)
			return false;
		for (int i = 0; i < nestingLevel; i++) {
			context[i] = stackTemp[i];
		}
		l1index = stackTemp[nestingLevel];
		l2index = stackTemp[nestingLevel + 1];
		l3index = stackTemp[nestingLevel + 2];
		l4index = stackTemp[nestingLevel + 3];
		l5index = stackTemp[nestingLevel + 4];
		l2lower = stackTemp[nestingLevel + 5];
		l2upper = stackTemp[nestingLevel + 6];
		l3lower = stackTemp[nestingLevel + 7];
		l3upper = stackTemp[nestingLevel + 8];
		l4lower = stackTemp[nestingLevel + 9];
		l4upper = stackTemp[nestingLevel + 10];
		l5lower = stackTemp[nestingLevel + 11];
		l5upper = stackTemp[nestingLevel + 12];
		atTerminal = (stackTemp[nestingLevel + 13] == 1);
		LN = stackTemp[nestingLevel+14];
		return true;
}
//Store the context info into the ContextBuffer.
//Info saved including LC and current state of the context 
bool VTDNav_L5::push(){
		for (int i = 0; i < nestingLevel; i++) {
			stackTemp[i] = context[i];
		}
		stackTemp[nestingLevel] = l1index;
		stackTemp[nestingLevel + 1] = l2index;
		stackTemp[nestingLevel + 2] = l3index;
		stackTemp[nestingLevel + 3] = l4index;
		stackTemp[nestingLevel + 4] = l5index;
		stackTemp[nestingLevel + 5] = l2lower;
		stackTemp[nestingLevel + 6] = l2upper;
		stackTemp[nestingLevel + 7] = l3lower;
		stackTemp[nestingLevel + 8] = l3upper;
		stackTemp[nestingLevel + 9] = l4lower;
		stackTemp[nestingLevel + 10] = l4upper;
		stackTemp[nestingLevel + 11] = l5lower;
		stackTemp[nestingLevel + 12] = l5upper;
		
		if (atTerminal)
			stackTemp[nestingLevel + 13] =1;
		else
			stackTemp[nestingLevel + 13] =0;
		stackTemp[nestingLevel+14] = LN; 
		contextBuf->store(stackTemp);
		return true;
}

bool VTDNav_L5::push2(){
		for (int i = 0; i < nestingLevel; i++) {
			stackTemp[i] = context[i];
		}
		stackTemp[nestingLevel] = l1index;
		stackTemp[nestingLevel + 1] = l2index;
		stackTemp[nestingLevel + 2] = l3index;
		stackTemp[nestingLevel + 3] = l4index;
		stackTemp[nestingLevel + 4] = l5index;
		stackTemp[nestingLevel + 5] = l2lower;
		stackTemp[nestingLevel + 6] = l2upper;
		stackTemp[nestingLevel + 7] = l3lower;
		stackTemp[nestingLevel + 8] = l3upper;
		stackTemp[nestingLevel + 9] = l4lower;
		stackTemp[nestingLevel + 10] = l4upper;
		stackTemp[nestingLevel + 11] = l5lower;
		stackTemp[nestingLevel + 12] = l5upper;
		
		if (atTerminal)
			stackTemp[nestingLevel + 13] =1;
		else
			stackTemp[nestingLevel + 13] =0;
		stackTemp[nestingLevel+14] = LN; 
		contextBuf2->store(stackTemp);
		return true;
}
void VTDNav_L5::sampleState( FastIntBuffer *fib){}

// A generic navigation method.
// Move the current to the element according to the direction constants
// If no such element, no position change and return false (0).
/* Legal direction constants are 	<br>
* <pre>		ROOT            0  </pre>
* <pre>		PARENT          1  </pre>
* <pre>		FIRST_CHILD     2  </pre>
* <pre>		LAST_CHILD      3  </pre>
* <pre>		NEXT_SIBLING    4  </pre>
* <pre>		PREV_SIBLING    5  </pre>
* <br>
*/
bool VTDNav_L5::toElement( navDir direction){
		int size;
		switch (direction) {
			case ROOT : // to document element!
				if (context[0] != 0) {
					/*
                     * for (int i = 1; i <= context[0]; i++) { context[i] =
                     * 0xffffffff; }
                     */
					context[0] = 0;
				}
				atTerminal = false;
				l1index = l2index = l3index = -1;
				return true;
			case PARENT :
				if (atTerminal == true){
					atTerminal = false;
					return true;
				}
				if (context[0] > 0) {
					//context[context[0]] = context[context[0] + 1] =
                    // 0xffffffff;
					context[context[0]] = -1;
					context[0]--;
					return true;
				}else if (context[0]==0){
					context[0]=-1; //to be compatible with XPath Data model
					return true;
 				}
				else {
					return false;
				}
			case FIRST_CHILD :
			case LAST_CHILD :
				if (atTerminal) return false;
				switch (context[0]) {
				    case -1:
				    	context[0] = 0;
				    	return true;
					case 0 :
						if (l1Buffer->size > 0) {
							context[0] = 1;
							l1index =
								(direction == FIRST_CHILD)
									? 0
									: (l1Buffer->size - 1);
							context[1] = l1Buffer->upper32At(l1index);
							//(int) (vtdToken >> 32);
							return true;
						} else
							return false;
					case 1 :
						l2lower = l1Buffer->lower32At(l1index);
						if (l2lower == -1) {
							return false;
						}
						context[0] = 2;
						l2upper = l2Buffer->size - 1;
						size = l1Buffer->size;
						for (int i = l1index + 1; i < size; i++) {
							int temp = l1Buffer->lower32At(i);
							if (temp != (int)0xffffffff) {
								l2upper = temp - 1;
								break;
							}
						}
						//System.out.println(" l2 upper: " + l2upper + " l2
                        // lower : " + l2lower);
						l2index =
							(direction == FIRST_CHILD) ? l2lower : l2upper;
						context[2] = l2Buffer->upper32At(l2index);
						return true;
						
					case 2 :
						l3lower = l2Buffer->lower32At(l2index);
						if (l3lower == -1) {
							return false;
						}
						context[0] = 3;
						l3upper = l3Buffer->size - 1;
						size = l2Buffer->size;
						for (int i = l2index + 1; i < size; i++) {
							int temp = l2Buffer->lower32At(i);
							if (temp != (int)0xffffffff) {
								l3upper = temp - 1;
								break;
							}
						}
						//System.out.println(" l2 upper: " + l2upper + " l2
                        // lower : " + l2lower);
						l3index =
							(direction == FIRST_CHILD) ? l3lower : l3upper;
						context[3] = l3Buffer->upper32At(l3index);
						return true;
						
					case 3 :
						l4lower = l3Buffer->lower32At(l3index);
						if (l4lower == -1) {
							return false;
						}
						context[0] = 4;
						l4upper = l4Buffer->size - 1;
						size = l3Buffer->size;
						for (int i = l3index + 1; i < size; i++) {
							int temp = l3Buffer->lower32At(i);
							if (temp != (int)0xffffffff) {
								l4upper = temp - 1;
								break;
							}
						}
						//System.out.println(" l2 upper: " + l2upper + " l2
                        // lower : " + l2lower);
						l4index =
							(direction == FIRST_CHILD) ? l4lower : l4upper;
						context[4] = l4Buffer->upper32At(l4index);
						return true;

					case 4 :
						l5lower = l4Buffer->lower32At(l4index);
						if (l5lower == -1) {
							return false;
						}
						context[0] = 5;

						l5upper = l5Buffer->size - 1;
						size = l4Buffer->size;
						for (int i = l4index + 1; i < size; i++) {
							int temp = l4Buffer->lower32At(i);
							if (temp != (int)0xffffffff) {
								l5upper = temp - 1;
								break;
							}
						}
						//System.out.println(" l3 upper : " + l3upper + " l3
                        // lower : " + l3lower);
						l5index =
							(direction == FIRST_CHILD) ? l5lower : l5upper;
						context[5] = l5Buffer->intAt(l5index);

						return true;

					default :
						if (direction == FIRST_CHILD) {
							size = vtdBuffer->size;
							int index = context[context[0]] + 1;
							while (index < size) {
								Long temp = vtdBuffer->longAt(index);
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
							size = vtdBuffer->size;
							while (index < size) {
								Long temp = vtdBuffer->longAt(index);
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
				if(atTerminal)return false;
				switch (context[0]) {
					case -1:
					case 0 :
						return false;
					case 1 :
						if (direction == NEXT_SIBLING) {
							if (l1index + 1 >= l1Buffer->size) {
								return false;
							}

							l1index++; // global incremental
						} else {
							if (l1index - 1 < 0) {
								return false;
							}
							l1index--; // global incremental
						}
						context[1] = l1Buffer->upper32At(l1index);
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
						context[2] = l2Buffer->upper32At(l2index);
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
						context[3] = l3Buffer->upper32At(l3index);
						return true;
						
					case 4 :
						if (direction == NEXT_SIBLING) {
							if (l4index + 1 > l4upper) {
								return false;
							}
							l4index++;
						} else {
							if (l4index - 1 < l4lower) {
								return false;
							}
							l4index--;
						}
						context[4] = l4Buffer->upper32At(l4index);
						return true;
					case 5 :
						if (direction == NEXT_SIBLING) {
							if (l5index + 1 > l5upper) {
								return false;
							}
							l5index++;
						} else {
							if (l5index - 1 < l5lower) {
								return false;
							}
							l5index--;
						}
						context[5] = l5Buffer->intAt(l5index);
						return true;
					default :
						//int index = context[context[0]] + 1;

						if (direction == NEXT_SIBLING) {
							int index = context[context[0]] + 1;
							size = vtdBuffer->size;
							while (index < size) {
								Long temp = vtdBuffer->longAt(index);
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
								Long temp = vtdBuffer->longAt(index);
								int token_type =
									(int) ((MASK_TOKEN_TYPE & temp) >> 60)
										& 0xf;

								if (token_type == TOKEN_STARTING_TAG) {
									int depth =
										(int) ((MASK_TOKEN_DEPTH & temp) >> 52);
									/*
                                     * if (depth < context[0]) { return false; }
                                     * else
                                     */
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
				throw NavException("illegal navigation options");
		}


}

/**
* A generic navigation method.
* Move the current to the element according to the direction 
* constants and the element name
* If no such element, no position change and return false (0).
* "*" matches any element
* Legal direction constants are 	<br>
* <pre>		ROOT            0  </pre>
* <pre>		PARENT          1  </pre>
* <pre>		FIRST_CHILD     2  </pre>
* <pre>		LAST_CHILD      3  </pre>
* <pre>		NEXT_SIBLING    4  </pre>
* <pre>		PREV_SIBLING    5  </pre>
* <br>
* for ROOT and PARENT, element name will be ignored.
*/
bool VTDNav_L5::toElement( navDir direction, UCSChar *en){
		int temp;
		int d;
		int val=0;
		if (en == NULL)
			throw InvalidArgumentException(" Element name can't be null ");
		if (wcscmp(en,L"*"))
			return toElement(direction);
		switch (direction) {
			case ROOT :
				return toElement(ROOT);

			case PARENT :
				return toElement(PARENT);

			case FIRST_CHILD :
				if (atTerminal)return false;
				if (toElement(FIRST_CHILD) == false)
					return false;
				// check current element name
				if (matchElement(en) == false) {
					if (toElement(NEXT_SIBLING, en) == true)
						return true;
					else {
						//toParentElement();
						//context[context[0]] = 0xffffffff;
						context[0]--;
						return false;
					}
				} else
					return true;

			case LAST_CHILD :
				if (atTerminal)return false;
				if (toElement(LAST_CHILD) == false)
					return false;
				if (matchElement(en) == false) {
					if (toElement(PREV_SIBLING, en) == true)
						return true;
					else {
						//context[context[0]] = 0xffffffff;
						context[0]--;
						//toParentElement();
						return false;
					}
				} else
					return true;

			case NEXT_SIBLING :
				if (atTerminal)return false;
				d = context[0];
				
				switch(d)
				{
				  case -1:
				  case 0: return false;
				  case 1: val = l1index; break;
				  case 2: val = l2index; break;
				  case 3: val = l3index; break;
				  case 4: val = l4index; break;
				  case 5: val = l5index; break;
				}
				temp = context[d]; // store the current position
				
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
				  case 4: l4index = val; break;
				  case 5: l5index = val; break;
				  	
				}
				context[d] = temp;
				return false;

			case PREV_SIBLING :
				if (atTerminal) return false;
				d = context[0];
				switch(d)
				{
				  case -1:
				  case 0: return false;
				  case 1: val = l1index; break;
				  case 2: val = l2index; break;
				  case 3: val = l3index; break;
				  case 4: val = l4index; break;
				  case 5: val = l5index; break;
				  	
				}
				temp = context[d]; // store the current position
				
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
				  case 4: l4index = val; break;
				  case 5: l5index = val; break;
				  	
				}
				context[d] = temp;
				return false;

			default :
				throw NavException("illegal navigation options");
		}
}
/*	
* A generic navigation function with namespace support.
* Move the current to the element according to the direction constants and the prefix and local names
* If no such element, no position change and return false(0).
* URL * matches any namespace, including undefined namespaces
* a null URL means hte namespace prefix is undefined for the element
* ln *  matches any localname
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
*/
bool VTDNav_L5::toElementNS( navDir direction, UCSChar *URL, UCSChar *ln){
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
				if (atTerminal)return false;
				if (toElement(FIRST_CHILD) == false)
					return false;
				// check current element name
				if (matchElementNS(URL, ln) == false) {
					if (toElementNS(NEXT_SIBLING, URL, ln) == true)
						return true;
					else {
						//toParentElement();
						//context[context[0]] = 0xffffffff;
						context[0]--;
						return false;
					}
				} else
					return true;

			case LAST_CHILD :
				if (atTerminal)return false;
				if (toElement(LAST_CHILD) == false)
					return false;
				if (matchElementNS(URL, ln) == false) {
					if (toElementNS(PREV_SIBLING, URL, ln) == true)
						return true;
					else {
						//context[context[0]] = 0xffffffff;
						context[0]--;
						//toParentElement();
						return false;
					}
				} else
					return true;

			case NEXT_SIBLING :
				if (atTerminal)return false;
				d = context[0];
				temp = context[d]; // store the current position
				switch(d)
				{
				  case -1:
				  case 0: return false;
				  case 1: val = l1index; break;
				  case 2: val = l2index; break;
				  case 3: val = l3index; break;
				  case 4: val = l4index; break;
				  case 5: val = l5index; break;
				  	//default:
				}
				//if (d == 0)
				//	return false;
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
				  case 4: val = l4index; break;
				  case 5: val = l5index; break;
				  	//default:
				}
				context[d] = temp;
				return false;

			case PREV_SIBLING :
				if (atTerminal)return false;
				d = context[0];
				temp = context[d]; // store the current position
				switch(d)
				{
				  case -1:
				  case 0: return false;
				  case 1: val = l1index; break;
				  case 2: val = l2index; break;
				  case 3: val = l3index; break;
				  case 4: val = l4index; break;
				  case 5: val = l5index; break;
				  //	default:
				}
				//if (d == 0)
				//	return false;
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
				  case 4: val = l4index; break;
				  case 5: val = l5index; break;
				  //	default:
				}
				context[d] = temp;
				return false;

			default :
				throw NavException("illegal navigation options");
		}
}

/* Write VTD+XML into a FILE pointer */
bool VTDNav_L5::writeIndex(FILE *f){
	return IndexHandler::_writeIndex_L5(1, 
                encoding, 
                ns, 
                true, 
				nestingLevel-1, 
                5, 
                rootIndex, 
                XMLDoc, 
                docOffset, 
                docLen, 
				vtdBuffer, 
                l1Buffer, 
                l2Buffer, 
                l3Buffer,
				l4Buffer,
				l5Buffer,
                f);
}

/* Write VTD+XML into a file of given name */
bool VTDNav_L5::writeIndex(char *fileName){
	FILE *f = NULL;
	bool b = false;
	f = fopen(fileName,"wb");
	
	if (f==NULL){
		throw InvalidArgumentException("fileName not valid");
		//return false;
	}
	b = writeIndex(f);
	fclose(f);
	return b;
}

/* Write the VTDs and LCs into an file*/
bool VTDNav_L5::writeSeparateIndex( char *vtdIndexFileName){
FILE *f = NULL;
	bool b = false;
	f = fopen(vtdIndexFileName,"wb");
	
	if (f==NULL){
		throw InvalidArgumentException("fileName not valid");
		//return false;
	}
	b = writeSeparateIndex(f);
	fclose(f);
	return b;
}

bool VTDNav_L5::writeSeparateIndex( FILE *f){
	return IndexHandler::_writeSeparateIndex_L5(1, 
                encoding, 
                ns, 
                true, 
				nestingLevel-1, 
                3, 
                rootIndex, 
               // XMLDoc, 
                docOffset, 
                docLen, 
				vtdBuffer, 
                l1Buffer, 
                l2Buffer, 
                l3Buffer,
				l4Buffer,
				l5Buffer,
                f);
}