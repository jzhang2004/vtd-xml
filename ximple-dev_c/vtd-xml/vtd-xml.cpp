/* 
 * Copyright (C) 2002-2007 XimpleWare, info@ximpleware.com
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
// vtd-xml.cpp : Defines the entry point for the console application.
//

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include "xpath1.h"
#include "helper.h"
#include "vtdGen.h"
#include "vtdNav.h"
#include "autoPilot.h"
#include "XMLModifier.h"
//#include "lex.yy.c"
//#include "l8.tab.c"
#define VC

struct exception_context the_exception_context[1];

#if 0
void main(){
	exception e;
	int i=0,j=0,k=0;
	unsigned int temp1 = 0x7ffffff;
	unsigned int temp2 = 0x8000000;
	IntHash *ih = NULL;
	if (temp2> temp1)
		printf("we are good \n");
	Try{
		ih = createIntHash();
		for(i=234;i<599;i++){
			if (isUniqueIntHash(ih,i))
				printf(" yes %d\n",i);
		}
		for (i=234;i<611;i++){
			if (!isUniqueIntHash(ih,i))
				printf(" no %d \n",i);
			else
				printf(" yes %d \n",i);
		}
		for (i = 599;i< 612;i++){
			if (!isUniqueIntHash(ih,i))
				printf(" no %d \n",i);
			else
				printf(" yes %d \n",i);
		}
		freeIntHash(ih);
	}Catch(e){

	}
	return 1;
}
#endif

#if 1
static  VTDNav* parseString(char* ba);
static void test1();
static void test2();
static void test3();
static void xtest1();
static void xtest2();
static void xtest3();
static void xtest4();
static void println(char* ba);
Boolean APTest_BR(char *fn){
	exception e;
	int i = 0;
	int ii;
	FILE *f = NULL;
	UByte *xml = NULL;
	VTDGen *vg = NULL;
	VTDNav *vn = NULL;
	AutoPilot *ap = NULL;

	f = fopen(fn,"r");
	//f = fopen("d://ximple-dev//testcases//VTDGen//bad//nt_8_2.xml","r");
	ii=(int)_filelength(f->_file);
	//printf("size of the file is %d \n",ii);
	xml = (UByte *)malloc(sizeof(UByte)*ii); 
	fread(xml,sizeof(UByte),ii,f);
	Try{
		FastIntBuffer *fib1 = createFastIntBuffer();
		FastIntBuffer *fib2 = createFastIntBuffer();
		int z = 0;
		vg = createVTDGen();
		setDoc_BR(vg,xml,ii);
		parse(vg,TRUE);
		setDoc_BR(vg,xml,ii);
		parse(vg,TRUE);
		vn = getNav(vg);
		if (toElement(vn,FIRST_CHILD)){
			do{
				z++;
				sampleState(vn,fib1);
			}
			while(toElement(vn,NEXT_SIBLING));
		}
		z = 0;
		toElement(vn,ROOT);

		if (toElement(vn,FIRST_CHILD)){
			do{
				if (toElement(vn,FIRST_CHILD)){
					do{
						z++;
						sampleState(vn,fib1);
					}
					while(toElement(vn,NEXT_SIBLING));
					toElement(vn, PARENT);
				}
			}
			while(toElement(vn,NEXT_SIBLING));
		}

		z = 0;
		toElement(vn,ROOT);

		if (toElement(vn,FIRST_CHILD)){
			do{
				if (toElement(vn,FIRST_CHILD)){
					do{
						if (toElement(vn,FIRST_CHILD)){
							do{
								z++;
								sampleState(vn,fib1);
							}
							while(toElement(vn,NEXT_SIBLING));
							toElement(vn, PARENT);
						}
					}
					while(toElement(vn,NEXT_SIBLING));
					toElement(vn, PARENT);
				}
			}
			while(toElement(vn,NEXT_SIBLING));
		}


		z = 0;
		toElement(vn,ROOT);

		if (toElement(vn,FIRST_CHILD)){
			do{
				if (toElement(vn,FIRST_CHILD)){
					do{
						if (toElement(vn,FIRST_CHILD)){
							do{
								if (toElement(vn,FIRST_CHILD)){
									do{
										z++;
										sampleState(vn,fib1);
									}
									while(toElement(vn,NEXT_SIBLING));
									toElement(vn, PARENT);
								}
							}
							while(toElement(vn,NEXT_SIBLING));
							toElement(vn, PARENT);
						}
					}
					while(toElement(vn,NEXT_SIBLING));
					toElement(vn, PARENT);
				}
			}
			while(toElement(vn,NEXT_SIBLING));
		}

		z = 0;
		toElement(vn,ROOT);
		
		ap = createAutoPilot(vn);
		selectElement(ap,L"level1:level1");
		while(iterateAP(ap)){
			z++;
			sampleState(vn,fib2);
		}

		z = 0;
		toElement(vn,ROOT);

		selectElement(ap,L"level2:level2");
		while(iterateAP(ap)){
			z++;
			sampleState(vn,fib2);
		}

		z = 0;
		toElement(vn,ROOT);

		selectElement(ap,L"level3:level3");
		while(iterateAP(ap)){
			z++;
			sampleState(vn,fib2);
		}

		z = 0;
		toElement(vn,ROOT);

		selectElement(ap,L"level4:level4");
		while(iterateAP(ap)){
			z++;
			sampleState(vn,fib2);
		}

		if (fib1->size != fib2->size){
			return FALSE;
		}

		for (i = 0;i < fib1->size;i++){
			if (intAt(fib1,i)!=intAt(fib2,i)){
				return FALSE;
			}
		}

		return TRUE;
}
Catch(e){
	printf("exception %s \n",e.msg);
	return FALSE;
}
}




Boolean APTest(char *fn){
	exception e;
	int i = 0;
	int ii;
	FILE *f = NULL;
	UByte *xml = NULL;
	VTDGen *vg = NULL;
	VTDNav *vn = NULL;
	AutoPilot *ap = NULL;

	f = fopen(fn,"r");
	//f = fopen("d://ximple-dev//testcases//VTDGen//bad//nt_8_2.xml","r");
	ii=(int)_filelength(f->_file);
	//printf("size of the file is %d \n",ii);
	xml = (UByte *)malloc(sizeof(UByte)*ii); 
	fread(xml,sizeof(UByte),ii,f);
	Try{
		FastIntBuffer *fib1 = createFastIntBuffer();
		FastIntBuffer *fib2 = createFastIntBuffer();
		int z = 0;
		vg = createVTDGen();
		setDoc(vg,xml,ii);
		parse(vg,TRUE);
		vn = getNav(vg);
		if (toElement(vn,FIRST_CHILD)){
			do{
				z++;
				sampleState(vn,fib1);
			}
			while(toElement(vn,NEXT_SIBLING));
		}
		z = 0;
		toElement(vn,ROOT);

		if (toElement(vn,FIRST_CHILD)){
			do{
				if (toElement(vn,FIRST_CHILD)){
					do{
						z++;
						sampleState(vn,fib1);
					}
					while(toElement(vn,NEXT_SIBLING));
					toElement(vn, PARENT);
				}
			}
			while(toElement(vn,NEXT_SIBLING));
		}

		z = 0;
		toElement(vn,ROOT);

		if (toElement(vn,FIRST_CHILD)){
			do{
				if (toElement(vn,FIRST_CHILD)){
					do{
						if (toElement(vn,FIRST_CHILD)){
							do{
								z++;
								sampleState(vn,fib1);
							}
							while(toElement(vn,NEXT_SIBLING));
							toElement(vn, PARENT);
						}
					}
					while(toElement(vn,NEXT_SIBLING));
					toElement(vn, PARENT);
				}
			}
			while(toElement(vn,NEXT_SIBLING));
		}


		z = 0;
		toElement(vn,ROOT);

		if (toElement(vn,FIRST_CHILD)){
			do{
				if (toElement(vn,FIRST_CHILD)){
					do{
						if (toElement(vn,FIRST_CHILD)){
							do{
								if (toElement(vn,FIRST_CHILD)){
									do{
										z++;
										sampleState(vn,fib1);
									}
									while(toElement(vn,NEXT_SIBLING));
									toElement(vn, PARENT);
								}
							}
							while(toElement(vn,NEXT_SIBLING));
							toElement(vn, PARENT);
						}
					}
					while(toElement(vn,NEXT_SIBLING));
					toElement(vn, PARENT);
				}
			}
			while(toElement(vn,NEXT_SIBLING));
		}

		z = 0;
		toElement(vn,ROOT);
		
		ap = createAutoPilot(vn);
		selectElement(ap,L"level1:level1");
		while(iterateAP(ap)){
			z++;
			sampleState(vn,fib2);
		}

		z = 0;
		toElement(vn,ROOT);

		selectElement(ap,L"level2:level2");
		while(iterateAP(ap)){
			z++;
			sampleState(vn,fib2);
		}

		z = 0;
		toElement(vn,ROOT);

		selectElement(ap,L"level3:level3");
		while(iterateAP(ap)){
			z++;
			sampleState(vn,fib2);
		}

		z = 0;
		toElement(vn,ROOT);

		selectElement(ap,L"level4:level4");
		while(iterateAP(ap)){
			z++;
			sampleState(vn,fib2);
		}

		if (fib1->size != fib2->size){
			return FALSE;
		}

		for (i = 0;i < fib1->size;i++){
			if (intAt(fib1,i)!=intAt(fib2,i)){
				return FALSE;
			}
		}

		return TRUE;
}
Catch(e){
	printf("exception %s \n",e.msg);
	return FALSE;
}
}


Boolean NavTestNS(char* fn){
	exception e;
	//struct _stat fs;

	int ii = 0;
	FILE *f = NULL;
	UByte *xml = NULL;
	VTDGen *vg = NULL;
	VTDNav *vn = NULL;

	f = fopen(fn,"r");
	//f = fopen("d://ximple-dev//testcases//VTDGen//bad//nt_8_2.xml","r");
	ii=(int)_filelength(f->_file);
	//printf("size of the file is %d \n",ii);
	xml = (UByte *)malloc(sizeof(UByte)*ii); 
	fread(xml,sizeof(UByte),ii,f);
	Try{
		int i,i1;
		vg = createVTDGen();
		setDoc(vg,xml,ii);
		parse(vg,TRUE);
		vn = getNav(vg);
		i= parseInt(vn,getAttrVal(vn,L"attr"));
		i1 = 0;
		if(toElement(vn,FIRST_CHILD)){ // to level 1
			//i1++;
			do {
				int j = parseInt(vn,getAttrVal(vn,L"attr"));
				int j1 = 0;
				if (matchElementNS(vn,L"level1",L"level1")==FALSE){
					return FALSE;
				}
				if (toElement(vn,FIRST_CHILD)){ // to level 2
					//j1++;
					do {						
						int k = parseInt(vn,getAttrVal(vn,L"attr"));
						int k1 = 0;
						if (matchElementNS(vn,L"level2",L"level2")==FALSE)
							return FALSE;
						if (toElement(vn,FC)){ // level 3
							//k1++;
							do{									
								int l = parseInt(vn,getAttrVal(vn,L"attr"));
								int l1 = 0;
								if (matchElementNS(vn,L"level3",L"level3")==FALSE)
									return FALSE;
								if (toElement(vn,FIRST_CHILD)){ // level 4
									//l1++;
									do {											
										int m = parseInt(vn,getAttrVal(vn,L"attr"));
										int m1 = 0;
										if (matchElementNS(vn,L"level4",L"level4")==FALSE)
											return FALSE;
										if (toElement(vn,FC)){ // level 5
											m1++;
											do {
												if (matchElement(vn,L"level5")==FALSE)
													return FALSE;
												m1++;
											}while(toElement(vn,NS));
											toElement(vn,P);
										}
										if (m!=m1)
											return FALSE;
										if (m!=0)
											return FALSE;
										l1++;
									}while(toElement(vn,NS));
									toElement(vn,P);
								}
								if (l!=l1)
									return FALSE;
								k1++;
							}while(toElement(vn,NS));
							toElement(vn,P);
						}
						if (k1 != k)
							return FALSE;
						j1++;
					}while(toElement(vn,NS));
					toElement(vn,P);
				}
				if (j1 != j)
					return FALSE;
				i1++;
			}while(toElement(vn,NS));				
		}
		if (i1!=i)
			return FALSE;
		toElement(vn,ROOT);
		i= parseInt(vn,getAttrVal(vn,L"attr"));
		i1 = 0;
		if(toElement(vn,LAST_CHILD)){ // to level 1
			//i1++;
			do {
				int j = parseInt(vn,getAttrVal(vn,L"attr"));
				int j1 = 0;
				if (matchElementNS(vn,L"level1",L"level1")==FALSE){
					return FALSE;
				}
				if (toElement(vn,LAST_CHILD)){ // to level 2
					//j1++;
					do {						
						int k = parseInt(vn,getAttrVal(vn,L"attr"));
						int k1 = 0;
						if (matchElementNS(vn,L"level2",L"level2")==FALSE)
							return FALSE;
						if (toElement(vn,LC)){ // level 3
							//k1++;
							do{									
								int l = parseInt(vn,getAttrVal(vn,L"attr"));
								int l1 = 0;
								if (matchElementNS(vn,L"level3",L"level3")==FALSE)
									return FALSE;
								if (toElement(vn,LAST_CHILD)){ // level 4
									//l1++;
									do {											
										int m = parseInt(vn,getAttrVal(vn,L"attr"));
										int m1 = 0;
										if (matchElementNS(vn,L"level4",L"level4")==FALSE)
											return FALSE;
										if (toElement(vn,LC)){ // level 5
											m1++;
											do {
												if (matchElement(vn,L"level5")==FALSE)
													return FALSE;
												m1++;
											}while(toElement(vn,PS));
											toElement(vn,P);
										}
										if (m!=m1)
											return FALSE;
										if (m!=0)
											return FALSE;
										l1++;
									}while(toElement(vn,PS));
									toElement(vn,P);
								}
								if (l!=l1)
									return FALSE;
								k1++;
							}while(toElement(vn,PS));
							toElement(vn,P);
						}
						if (k1 != k)
							return FALSE;
						j1++;
					}while(toElement(vn,PS));
					toElement(vn,P);
				}
				if (j1 != j)
					return FALSE;
				i1++;
			}while(toElement(vn,PS));				
		}
		if (i1!=i)
			return FALSE;
	}
	Catch (e){
		if (e.et == parse_exception)
			printf("exception e ==> %s \n %s\n", e.msg,e.sub_msg);
		return FALSE;
	}
	return TRUE;
}
Boolean NavTestNS_BR(char* fn){
	exception e;
	//struct _stat fs;

	int ii = 0;
	FILE *f = NULL;
	UByte *xml = NULL;
	VTDGen *vg = NULL;
	VTDNav *vn = NULL;

	f = fopen(fn,"r");
	//f = fopen("d://ximple-dev//testcases//VTDGen//bad//nt_8_2.xml","r");
	ii=(int)_filelength(f->_file);
	//printf("size of the file is %d \n",ii);
	xml = (UByte *)malloc(sizeof(UByte)*ii); 
	fread(xml,sizeof(UByte),ii,f);
	Try{
		int i,i1;
		vg = createVTDGen();
		setDoc(vg,xml,ii);
		parse(vg,TRUE);
		setDoc_BR(vg,xml,ii);
		parse(vg,TRUE);
		vn = getNav(vg);
		i= parseInt(vn,getAttrVal(vn,L"attr"));
		i1 = 0;
		if(toElement(vn,FIRST_CHILD)){ // to level 1
			//i1++;
			do {
				int j = parseInt(vn,getAttrVal(vn,L"attr"));
				int j1 = 0;
				if (matchElementNS(vn,L"level1",L"level1")==FALSE){
					return FALSE;
				}
				if (toElement(vn,FIRST_CHILD)){ // to level 2
					//j1++;
					do {						
						int k = parseInt(vn,getAttrVal(vn,L"attr"));
						int k1 = 0;
						if (matchElementNS(vn,L"level2",L"level2")==FALSE)
							return FALSE;
						if (toElement(vn,FC)){ // level 3
							//k1++;
							do{									
								int l = parseInt(vn,getAttrVal(vn,L"attr"));
								int l1 = 0;
								if (matchElementNS(vn,L"level3",L"level3")==FALSE)
									return FALSE;
								if (toElement(vn,FIRST_CHILD)){ // level 4
									//l1++;
									do {											
										int m = parseInt(vn,getAttrVal(vn,L"attr"));
										int m1 = 0;
										if (matchElementNS(vn,L"level4",L"level4")==FALSE)
											return FALSE;
										if (toElement(vn,FC)){ // level 5
											m1++;
											do {
												if (matchElement(vn,L"level5")==FALSE)
													return FALSE;
												m1++;
											}while(toElement(vn,NS));
											toElement(vn,P);
										}
										if (m!=m1)
											return FALSE;
										if (m!=0)
											return FALSE;
										l1++;
									}while(toElement(vn,NS));
									toElement(vn,P);
								}
								if (l!=l1)
									return FALSE;
								k1++;
							}while(toElement(vn,NS));
							toElement(vn,P);
						}
						if (k1 != k)
							return FALSE;
						j1++;
					}while(toElement(vn,NS));
					toElement(vn,P);
				}
				if (j1 != j)
					return FALSE;
				i1++;
			}while(toElement(vn,NS));				
		}
		if (i1!=i)
			return FALSE;
		toElement(vn,ROOT);
		i= parseInt(vn,getAttrVal(vn,L"attr"));
		i1 = 0;
		if(toElement(vn,LAST_CHILD)){ // to level 1
			//i1++;
			do {
				int j = parseInt(vn,getAttrVal(vn,L"attr"));
				int j1 = 0;
				if (matchElementNS(vn,L"level1",L"level1")==FALSE){
					return FALSE;
				}
				if (toElement(vn,LAST_CHILD)){ // to level 2
					//j1++;
					do {						
						int k = parseInt(vn,getAttrVal(vn,L"attr"));
						int k1 = 0;
						if (matchElementNS(vn,L"level2",L"level2")==FALSE)
							return FALSE;
						if (toElement(vn,LC)){ // level 3
							//k1++;
							do{									
								int l = parseInt(vn,getAttrVal(vn,L"attr"));
								int l1 = 0;
								if (matchElementNS(vn,L"level3",L"level3")==FALSE)
									return FALSE;
								if (toElement(vn,LAST_CHILD)){ // level 4
									//l1++;
									do {											
										int m = parseInt(vn,getAttrVal(vn,L"attr"));
										int m1 = 0;
										if (matchElementNS(vn,L"level4",L"level4")==FALSE)
											return FALSE;
										if (toElement(vn,LC)){ // level 5
											m1++;
											do {
												if (matchElement(vn,L"level5")==FALSE)
													return FALSE;
												m1++;
											}while(toElement(vn,PS));
											toElement(vn,P);
										}
										if (m!=m1)
											return FALSE;
										if (m!=0)
											return FALSE;
										l1++;
									}while(toElement(vn,PS));
									toElement(vn,P);
								}
								if (l!=l1)
									return FALSE;
								k1++;
							}while(toElement(vn,PS));
							toElement(vn,P);
						}
						if (k1 != k)
							return FALSE;
						j1++;
					}while(toElement(vn,PS));
					toElement(vn,P);
				}
				if (j1 != j)
					return FALSE;
				i1++;
			}while(toElement(vn,PS));				
		}
		if (i1!=i)
			return FALSE;
	}
	Catch (e){
		if (e.et == parse_exception)
			printf("exception e ==> %s \n %s\n", e.msg,e.sub_msg);
		return FALSE;
	}
	return TRUE;
}
Boolean MiscTest_BR(char *fn){
	exception e;
	int ii = 0;
	FILE *f = NULL;
	UByte *xml = NULL;
	VTDGen *vg = NULL;
	VTDNav *vn = NULL;

	f = fopen(fn,"r");
	//f = fopen("d://ximple-dev//testcases//VTDGen//bad//nt_8_2.xml","r");
	ii=(int)_filelength(f->_file);
	//printf("size of the file is %d \n",ii);
	xml = (UByte *)malloc(sizeof(UByte)*ii); 
	fread(xml,sizeof(UByte),ii,f);
	Try{
		int i;
		vg = createVTDGen();
		setDoc_BR(vg,xml,ii);
		parse(vg,TRUE);
		setDoc_BR(vg,xml,ii);
		parse(vg,TRUE);
		vn = getNav(vg);
		i = getText(vn);
		if (toElement(vn,FIRST_CHILD)){
				if (matchElement(vn,L"float")){
					do {
#ifdef VC
						float f1 = (float)_wtof(toString(vn,getText(vn)));
#endif
#ifdef GCC
						float f1 = (float)wcstof(toString(vn,getText(vn)),NULL);
#endif
						float f2 = parseFloat(vn,getText(vn));
						if (f1 != f2)
							return FALSE;						
					}while(toElement(vn,NS));
					return TRUE;
				}else if (matchElement(vn,L"double")){
					do {
#ifdef VC
						double d1 = _wtof(toString(vn,getText(vn)));
#endif
#ifdef GCC
						double d1 = wcstof(toString(vn,getText(vn)),NULL);
#endif
						double d2 = parseDouble(vn,getText(vn));
						if (d1 != d2)
							return FALSE;						
					}while(toElement(vn,NS));
					return TRUE;
				}else if (matchElement(vn,L"int")){
					do {
						int i1 = wcstol(toString(vn,getText(vn)),NULL,10);
						int i2 = parseInt(vn,getText(vn));
						if (i1 != i2)
							return FALSE;						
					}while(toElement(vn,NS));
					return TRUE;
				}else if (matchElement(vn,L"long")){
					do {
#ifdef VC
						Long l1 = _wcstoi64(toString(vn,getText(vn)),NULL,10);
#endif
#ifdef GCC
						Long l1 = wcstoll(toString(vn,getText(vn)),NULL,10);
#endif
						Long l2 = parseLong(vn, getText(vn));
						if (l1 != l2)
							return FALSE;						
					}while(toElement(vn,NS));
					return TRUE;
				}
				return FALSE;				
			}else {
				if (i==-1){
					i = getCurrentIndex(vn)+3;
					if (getTokenType(vn,i)== TOKEN_COMMENT
							|| getTokenLength(vn,i) == parseInt(vn,getAttrVal(vn,L"len")))
						return TRUE;
						
					return FALSE;
				}
				if (getTokenType(vn,i)== TOKEN_CDATA_VAL
						|| getTokenType(vn,i) == TOKEN_CHARACTER_DATA){
					int z = getAttrVal(vn,L"len");
					if (z!=-1){
						int i4 = parseInt(vn,z);
						if (i4!= getTokenLength(vn,i)){
							return FALSE;
						}
						return TRUE;
					}
					return TRUE;
				}
				return FALSE;			
			}
	}Catch (e){

	}
	return FALSE;
}

Boolean MiscTest(char *fn){
	exception e;
	int ii = 0;
	FILE *f = NULL;
	UByte *xml = NULL;
	VTDGen *vg = NULL;
	VTDNav *vn = NULL;

	f = fopen(fn,"r");
	//f = fopen("d://ximple-dev//testcases//VTDGen//bad//nt_8_2.xml","r");
	ii=(int)_filelength(f->_file);
	//printf("size of the file is %d \n",ii);
	xml = (UByte *)malloc(sizeof(UByte)*ii); 
	fread(xml,sizeof(UByte),ii,f);
	Try{
		int i;
		vg = createVTDGen();
		setDoc(vg,xml,ii);
		parse(vg,TRUE);
		vn = getNav(vg);
		i = getText(vn);
		if (toElement(vn,FIRST_CHILD)){
				if (matchElement(vn,L"float")){
					do {
#ifdef VC
						float f1 = (float)_wtof(toString(vn,getText(vn)));
#endif
#ifdef GCC
						float f1 = (float)wcstof(toString(vn,getText(vn)),NULL);
#endif
						float f2 = parseFloat(vn,getText(vn));
						if (f1 != f2)
							return FALSE;						
					}while(toElement(vn,NS));
					return TRUE;
				}else if (matchElement(vn,L"double")){
					do {
#ifdef VC
						double d1 = _wtof(toString(vn,getText(vn)));
#endif
#ifdef GCC
						double d1 = wcstof(toString(vn,getText(vn)),NULL);
#endif
						double d2 = parseDouble(vn,getText(vn));
						if (d1 != d2)
							return FALSE;						
					}while(toElement(vn,NS));
					return TRUE;
				}else if (matchElement(vn,L"int")){
					do {
						int i1 = wcstol(toString(vn,getText(vn)),NULL,10);
						int i2 = parseInt(vn,getText(vn));
						if (i1 != i2)
							return FALSE;						
					}while(toElement(vn,NS));
					return TRUE;
				}else if (matchElement(vn,L"long")){
					do {
#ifdef VC
						Long l1 = _wcstoi64(toString(vn,getText(vn)),NULL,10);
#endif
#ifdef GCC
						Long l1 = wcstoll(toString(vn,getText(vn)),NULL,10);
#endif
						Long l2 = parseLong(vn, getText(vn));
						if (l1 != l2)
							return FALSE;						
					}while(toElement(vn,NS));
					return TRUE;
				}
				return FALSE;				
			}else {
				if (i==-1){
					i = getCurrentIndex(vn)+3;
					if (getTokenType(vn,i)== TOKEN_COMMENT
							|| getTokenLength(vn,i) == parseInt(vn,getAttrVal(vn,L"len")))
						return TRUE;
						
					return FALSE;
				}
				if (getTokenType(vn,i)== TOKEN_CDATA_VAL
						|| getTokenType(vn,i) == TOKEN_CHARACTER_DATA){
					int z = getAttrVal(vn,L"len");
					if (z!=-1){
						int i4 = parseInt(vn,z);
						if (i4!= getTokenLength(vn,i)){
							return FALSE;
						}
						return TRUE;
					}
					return TRUE;
				}
				return FALSE;			
			}
	}Catch (e){

	}
	return FALSE;
}
Boolean NavTest_BR(char* fn){
	exception e;
	//struct _stat fs;

	int ii = 0;
	FILE *f = NULL;
	UByte *xml = NULL;
	VTDGen *vg = NULL;
	VTDNav *vn = NULL;

	f = fopen(fn,"r");
	//f = fopen("d://ximple-dev//testcases//VTDGen//bad//nt_8_2.xml","r");
	ii=(int)_filelength(f->_file);
	//printf("size of the file is %d \n",ii);
	xml = (UByte *)malloc(sizeof(UByte)*ii); 
	fread(xml,sizeof(UByte),ii,f);
	Try{
		int i,i1;
		vg = createVTDGen();
		setDoc_BR(vg,xml,ii);
		parse(vg,TRUE);
		setDoc_BR(vg,xml,ii);
		parse(vg,TRUE);
		vn = getNav(vg);
		i= parseInt(vn,getAttrVal(vn,L"attr"));
		i1 = 0;
		if(toElement(vn,FIRST_CHILD)){ // to level 1
			//i1++;
			do {
				int j = parseInt(vn,getAttrVal(vn,L"attr"));
				int j1 = 0;
				if (matchElement(vn,L"level1")==FALSE){
					return FALSE;
				}
				if (toElement(vn,FIRST_CHILD)){ // to level 2
					//j1++;
					do {						
						int k = parseInt(vn,getAttrVal(vn,L"attr"));
						int k1 = 0;
						if (matchElement(vn,L"level2")==FALSE)
							return FALSE;
						if (toElement(vn,FC)){ // level 3
							//k1++;
							do{									
								int l = parseInt(vn,getAttrVal(vn,L"attr"));
								int l1 = 0;
								if (matchElement(vn,L"level3")==FALSE)
									return FALSE;
								if (toElement(vn,FIRST_CHILD)){ // level 4
									//l1++;
									do {											
										int m = parseInt(vn,getAttrVal(vn,L"attr"));
										int m1 = 0;
										if (matchElement(vn,L"level4")==FALSE)
											return FALSE;
										if (toElement(vn,FC)){ // level 5
											m1++;
											do {
												if (matchElement(vn,L"level5")==FALSE)
													return FALSE;
												m1++;
											}while(toElement(vn,NS));
											toElement(vn,P);
										}
										if (m!=m1)
											return FALSE;
										if (m!=0)
											return FALSE;
										l1++;
									}while(toElement(vn,NS));
									toElement(vn,P);
								}
								if (l!=l1)
									return FALSE;
								k1++;
							}while(toElement(vn,NS));
							toElement(vn,P);
						}
						if (k1 != k)
							return FALSE;
						j1++;
					}while(toElement(vn,NS));
					toElement(vn,P);
				}
				if (j1 != j)
					return FALSE;
				i1++;
			}while(toElement(vn,NS));				
		}
		if (i1!=i)
			return FALSE;
		toElement(vn,ROOT);
		i= parseInt(vn,getAttrVal(vn,L"attr"));
		i1 = 0;
		if(toElement(vn,LAST_CHILD)){ // to level 1
			//i1++;
			do {
				int j = parseInt(vn,getAttrVal(vn,L"attr"));
				int j1 = 0;
				if (matchElement(vn,L"level1")==FALSE){
					return FALSE;
				}
				if (toElement(vn,LAST_CHILD)){ // to level 2
					//j1++;
					do {						
						int k = parseInt(vn,getAttrVal(vn,L"attr"));
						int k1 = 0;
						if (matchElement(vn,L"level2")==FALSE)
							return FALSE;
						if (toElement(vn,LC)){ // level 3
							//k1++;
							do{									
								int l = parseInt(vn,getAttrVal(vn,L"attr"));
								int l1 = 0;
								if (matchElement(vn,L"level3")==FALSE)
									return FALSE;
								if (toElement(vn,LAST_CHILD)){ // level 4
									//l1++;
									do {											
										int m = parseInt(vn,getAttrVal(vn,L"attr"));
										int m1 = 0;
										if (matchElement(vn,L"level4")==FALSE)
											return FALSE;
										if (toElement(vn,LC)){ // level 5
											m1++;
											do {
												if (matchElement(vn,L"level5")==FALSE)
													return FALSE;
												m1++;
											}while(toElement(vn,PS));
											toElement(vn,P);
										}
										if (m!=m1)
											return FALSE;
										if (m!=0)
											return FALSE;
										l1++;
									}while(toElement(vn,PS));
									toElement(vn,P);
								}
								if (l!=l1)
									return FALSE;
								k1++;
							}while(toElement(vn,PS));
							toElement(vn,P);
						}
						if (k1 != k)
							return FALSE;
						j1++;
					}while(toElement(vn,PS));
					toElement(vn,P);
				}
				if (j1 != j)
					return FALSE;
				i1++;
			}while(toElement(vn,PS));				
		}
		if (i1!=i)
			return FALSE;
	}
	Catch (e){
		if (e.et == parse_exception)
			printf("exception e ==> %s \n %s\n", e.msg,e.sub_msg);
		return FALSE;
	}
	return TRUE;
}


Boolean NavTest(char* fn){
	exception e;
	//struct _stat fs;

	int ii = 0;
	FILE *f = NULL;
	UByte *xml = NULL;
	VTDGen *vg = NULL;
	VTDNav *vn = NULL;

	f = fopen(fn,"r");
	//f = fopen("d://ximple-dev//testcases//VTDGen//bad//nt_8_2.xml","r");
	ii=(int)_filelength(f->_file);
	//printf("size of the file is %d \n",ii);
	xml = (UByte *)malloc(sizeof(UByte)*ii); 
	fread(xml,sizeof(UByte),ii,f);
	Try{
		int i,i1;
		vg = createVTDGen();
		setDoc(vg,xml,ii);
		parse(vg,TRUE);
		vn = getNav(vg);
		i= parseInt(vn,getAttrVal(vn,L"attr"));
		i1 = 0;
		if(toElement(vn,FIRST_CHILD)){ // to level 1
			//i1++;
			do {
				int j = parseInt(vn,getAttrVal(vn,L"attr"));
				int j1 = 0;
				if (matchElement(vn,L"level1")==FALSE){
					return FALSE;
				}
				if (toElement(vn,FIRST_CHILD)){ // to level 2
					//j1++;
					do {						
						int k = parseInt(vn,getAttrVal(vn,L"attr"));
						int k1 = 0;
						if (matchElement(vn,L"level2")==FALSE)
							return FALSE;
						if (toElement(vn,FC)){ // level 3
							//k1++;
							do{									
								int l = parseInt(vn,getAttrVal(vn,L"attr"));
								int l1 = 0;
								if (matchElement(vn,L"level3")==FALSE)
									return FALSE;
								if (toElement(vn,FIRST_CHILD)){ // level 4
									//l1++;
									do {											
										int m = parseInt(vn,getAttrVal(vn,L"attr"));
										int m1 = 0;
										if (matchElement(vn,L"level4")==FALSE)
											return FALSE;
										if (toElement(vn,FC)){ // level 5
											m1++;
											do {
												if (matchElement(vn,L"level5")==FALSE)
													return FALSE;
												m1++;
											}while(toElement(vn,NS));
											toElement(vn,P);
										}
										if (m!=m1)
											return FALSE;
										if (m!=0)
											return FALSE;
										l1++;
									}while(toElement(vn,NS));
									toElement(vn,P);
								}
								if (l!=l1)
									return FALSE;
								k1++;
							}while(toElement(vn,NS));
							toElement(vn,P);
						}
						if (k1 != k)
							return FALSE;
						j1++;
					}while(toElement(vn,NS));
					toElement(vn,P);
				}
				if (j1 != j)
					return FALSE;
				i1++;
			}while(toElement(vn,NS));				
		}
		if (i1!=i)
			return FALSE;
		toElement(vn,ROOT);
		i= parseInt(vn,getAttrVal(vn,L"attr"));
		i1 = 0;
		if(toElement(vn,LAST_CHILD)){ // to level 1
			//i1++;
			do {
				int j = parseInt(vn,getAttrVal(vn,L"attr"));
				int j1 = 0;
				if (matchElement(vn,L"level1")==FALSE){
					return FALSE;
				}
				if (toElement(vn,LAST_CHILD)){ // to level 2
					//j1++;
					do {						
						int k = parseInt(vn,getAttrVal(vn,L"attr"));
						int k1 = 0;
						if (matchElement(vn,L"level2")==FALSE)
							return FALSE;
						if (toElement(vn,LC)){ // level 3
							//k1++;
							do{									
								int l = parseInt(vn,getAttrVal(vn,L"attr"));
								int l1 = 0;
								if (matchElement(vn,L"level3")==FALSE)
									return FALSE;
								if (toElement(vn,LAST_CHILD)){ // level 4
									//l1++;
									do {											
										int m = parseInt(vn,getAttrVal(vn,L"attr"));
										int m1 = 0;
										if (matchElement(vn,L"level4")==FALSE)
											return FALSE;
										if (toElement(vn,LC)){ // level 5
											m1++;
											do {
												if (matchElement(vn,L"level5")==FALSE)
													return FALSE;
												m1++;
											}while(toElement(vn,PS));
											toElement(vn,P);
										}
										if (m!=m1)
											return FALSE;
										if (m!=0)
											return FALSE;
										l1++;
									}while(toElement(vn,PS));
									toElement(vn,P);
								}
								if (l!=l1)
									return FALSE;
								k1++;
							}while(toElement(vn,PS));
							toElement(vn,P);
						}
						if (k1 != k)
							return FALSE;
						j1++;
					}while(toElement(vn,PS));
					toElement(vn,P);
				}
				if (j1 != j)
					return FALSE;
				i1++;
			}while(toElement(vn,PS));				
		}
		if (i1!=i)
			return FALSE;
	}
	Catch (e){
		if (e.et == parse_exception)
			printf("exception e ==> %s \n %s\n", e.msg,e.sub_msg);
		return FALSE;
	}
	return TRUE;
}




void vnTester(){
	test1();
	test2();
	test3();
}
void XPathTester(){
	xtest1();
	xtest2();
	xtest3();
	xtest4();
}
static void println(char *ba){
	printf(ba);
	printf("\n");
}
static  VTDNav* parseString(char* ba, int len){
	VTDGen *vg = createVTDGen();
	setDoc(vg,ba,len);
	parse(vg,TRUE);
	return getNav(vg);
}
static void test2(){

	int i1,i2,i3,k;
	char ba[]="<test1> "  
             "<a>good</a>"
             "<b>good&amp;&lt;&gt;&apos;&quot;</b>"
             "<good>good</good>"
             "<c>&#97;&#98;&#99;&#100;&#101;&#102;&#103;</c>"
             "<d>&#x61;&#x62;&#x63;&#x64;&#x65;&#x66;&#x67;&#x100;</d>"
             "<e attr=''></e>"
             "<b><![CDATA[good&amp;&lt;&gt;&apos;&quot;]]></b>"
             "</test1>";
	        VTDNav* vn = parseString(ba,strlen(ba));
        toElement(vn,FIRST_CHILD);
        i1 = getText(vn);
        k = i1;
        if ((compareRawTokenString(vn,i1,L"good")==0)
                && (compareRawTokenString(vn,i1,L"good1")== -1)
                && (compareRawTokenString(vn,i1,L"goo")==1)
                && (compareTokenString(vn,i1,L"good")==0)
                && (compareTokenString(vn,i1,L"good1")== -1)
                && (compareTokenString(vn,i1,L"goo")==1))
            printf("test2 1 succeed\n");
        else
            printf("test2 1 failed\n");
        toElement(vn,NEXT_SIBLING);
        i2 = getText(vn);
        if ((compareRawTokenString(vn,i2,L"good&amp;&lt;&gt;&apos;&quot;")==0)
                && (compareRawTokenString(vn,i2,L"good&amp;&lt;&gt;&apos;&quot")==1)
                && (compareRawTokenString(vn,i2,L"good&amp;&lt;&gt;&apos;&quot;abd")==-1)
                && (compareTokenString(vn,i2,L"good&<>'\"")==0)
                && (compareTokenString(vn,i2,L"good&<>'")==1)
                && (compareTokenString(vn,i2,L"good&<>'\"a")==-1)
                )            
            printf("test2 2 succeed\n");
        else
            printf("test2 2 failed\n");
        i3 = i2;
        toElement(vn,NEXT_SIBLING);
        i2 = getText(vn);
        if ((compareTokens(vn,i2,vn,i1)==0)
                && (compareTokens(vn,i2,vn,i3)== -1 )
                && (compareTokens(vn,i3,vn,i2)== 1)        	
        )
            printf("test2 3 succeed\n");
        else 
            printf("test2 3 failed\n");
        toElement(vn,NEXT_SIBLING);
        i2 = getText(vn);
        if ((compareRawTokenString(vn,i2,L"&#97;&#98;&#99;&#100;&#101;&#102;&#103;")==0)
                && (compareRawTokenString(vn,i2,L"&#97;&#98;&#99;&#100;&#101;&#102;&#103;a")==-1)
                && (compareRawTokenString(vn,i2,L"&#97;&#98;&#99;&#100;&#101;&#102;&#103")==1)
                && (compareTokenString(vn,i2,L"abcdefg")==0)
                && (compareTokenString(vn,i2,L"abcdef")==1)
                && (compareTokenString(vn,i2,L"abcdefg1")==-1))
            printf("test2 4 succeed\n");
        else 
            printf("test2 4 failed\n");
        toElement(vn,NEXT_SIBLING);
        i2 = getText(vn);
        //System.out.printf(toRawString(vn,i2));
        if ((compareRawTokenString(vn,i2,L"&#x61;&#x62;&#x63;&#x64;&#x65;&#x66;&#x67;&#x100;")==0)
                && (compareRawTokenString(vn,i2,L"&#x61;&#x62;&#x63;&#x64;&#x65;&#x66;&#x67;&#x100;a")==-1)
                && (compareRawTokenString(vn,i2,L"&#x61;&#x62;&#x63;&#x64;&#x65;&#x66;&#x67;&#x100")==1)
                && (compareTokenString(vn,i2,L"abcdefg\u0100")==0)
                && (compareTokenString(vn,i2,L"abcdefg")==1)
                && (compareTokenString(vn,i2,L"abcdefg\u0101")==-1))
            printf("test2 5 succeed\n");
        else 
            printf("test2 5 failed\n");
        toElement(vn,NEXT_SIBLING);
        i2 = getAttrVal(vn,L"attr");
        if (compareRawTokenString(vn,i2,L"")==0
                || compareTokenString(vn,i2,L"a")==-1)
            printf("test2 6 succeed\n");
        else
            printf("test2 6 failed\n");
        toElement(vn,NEXT_SIBLING);
        i2 = getText(vn);
        if (compareRawTokenString(vn,i2,L"good&amp;&lt;&gt;&apos;&quot;")==0
                && compareTokenString(vn,i2,L"good&<>")!=0)
            printf("test2 7 succeed\n");
        else
            printf("test2 7 failed\n");
}
static void test1(){
	int i1, i2, k, i3;
	char ba[]="<test1> "  
             "<a>good</a>"
             "<b>good&amp;&lt;&gt;&apos;&quot;</b>"
             "<good>good</good>"
             "<c>&#97;&#98;&#99;&#100;&#101;&#102;&#103;</c>"
             "<d>&#x61;&#x62;&#x63;&#x64;&#x65;&#x66;&#x67;&#x100;</d>"
             "<e attr=''></e>"
             "<b><![CDATA[good&amp;&lt;&gt;&apos;&quot;]]></b>"
             "</test1>";
	VTDNav* vn = parseString(ba,strlen(ba));
    toElement(vn,FIRST_CHILD);
    i1 = getText(vn);
    k = i1;
    if (!matchRawTokenString(vn,i1,L"good")
                || !matchTokenString(vn,i1,L"good"))
            printf("test1 1 failed\n");
        else
            printf("test1 1 succeed\n");
        toElement(vn,NEXT_SIBLING);
        i2 = getText(vn);
        if (!matchRawTokenString(vn,i2,L"good&amp;&lt;&gt;&apos;&quot;")
                || !matchTokenString(vn,i2,L"good&<>'\""))
            printf("test1 2 failed\n");
        else
            printf("test1 2 succeed\n");
        toElement(vn,NEXT_SIBLING);
        i2 = getText(vn);
        if (!matchTokens(vn,i2,vn,i1))
            printf("test1 3 failed\n");
        else 
            printf("test1 3 succeed\n");
        toElement(vn,NEXT_SIBLING);
        i2 = getText(vn);
        if (!matchRawTokenString(vn,i2,L"&#97;&#98;&#99;&#100;&#101;&#102;&#103;")
                || !matchTokenString(vn,i2,L"abcdefg"))
            printf("test1 4 failed\n");
        else 
            printf("test1 4 succeed\n");
        toElement(vn, NEXT_SIBLING);
        i2 = getText(vn);
        if (!matchRawTokenString(vn,i2,L"&#x61;&#x62;&#x63;&#x64;&#x65;&#x66;&#x67;&#x100;")
                || !matchTokenString(vn,i2,L"abcdefg\u0100"))
            printf("test1 5 failed\n");
        else 
            printf("test1 5 succeed\n");
        toElement(vn,NEXT_SIBLING);
        i2 = getAttrVal(vn,L"attr");
        if (!matchRawTokenString(vn,i2,L"")
                || !matchTokenString(vn,i2,L""))
            printf("test1 6 failed\n");
        else
            printf("test1 6 succeed\n");
        toElement(vn,NEXT_SIBLING);
        i2 = getText(vn);
        if (matchRawTokenString(vn,i2,L"good&amp;&lt;&gt;&apos;&quot;")
                && !matchTokenString(vn,i2,L"good&<>"))
            printf("test1 7 succeed\n");
        else
            printf("test1 7 failed\n");
}
static void test3(){
	int i1,i2,i3,k;
	UCSChar* s1;
	char ba[]="<test1> "  
		"<a>good</a>"
		"<b>good&amp;&lt;&gt;&apos;&quot;</b>"
		"<good>good</good>"
		"<c>&#97;&#98;&#99;&#100;&#101;&#102;&#103;</c>"
		"<d>&#x61;&#x62;&#x63;&#x64;&#x65;&#x66;&#x67;&#x100;</d>"
		"<e attr=''></e>"
		"<b><![CDATA[good&amp;&lt;&gt;&apos;&quot;]]></b>"
		"</test1>";

	VTDNav* vn = parseString(ba,strlen(ba));
	toElement(vn,FIRST_CHILD);
	i1 = getText(vn);
	k = i1;
	if ((wcscmp(toRawString(vn,i1),L"good")==0)
		&& (wcscmp(toRawString(vn,i1),L"good1")== -1)
		&& (wcscmp(toRawString(vn,i1),L"goo")==1)
		&& (wcscmp(toString(vn,i1),L"good")==0)
		&& (wcscmp(toString(vn,i1),L"good1")== -1)
		&& (wcscmp(toString(vn,i1),L"goo")==1))
		printf("test3 1 succeed\n");
	else
		printf("test3 1 failed\n");
	toElement(vn,NEXT_SIBLING);
	i2 = getText(vn);
	s1= toRawString(vn,i2);
	if ((wcscmp(toRawString(vn,i2),L"good&amp;&lt;&gt;&apos;&quot;")==0)
		&& (wcscmp(toRawString(vn,i2),L"good&amp;&lt;&gt;&apos;&quot")>0)
		&& (wcscmp(s1,L"good&amp;&lt;&gt;&apos;&quot;abd")<0)
		&& (wcscmp(toString(vn,i2),L"good&<>'\"")==0)
		&& (wcscmp(toString(vn,i2),L"good&<>'")>0)
		&& (wcscmp(toString(vn,i2),L"good&<>'\"a")<0)
		)            
		printf("test3 2 succeed\n");
	else
		printf("test3 2 failed\n");
	i3 = i2;
	toElement(vn,NEXT_SIBLING);
	i2 = getText(vn);
	if ((wcscmp(toString(vn,i2),toString(vn,i1))==0)
		&& (wcscmp(toString(vn,i2),toString(vn,i3))< 0)
		&& (wcscmp(toString(vn,i3),toString(vn,i2))> 0)        	
		)
		printf("test3 3 succeed\n");
	else 
		printf("test3 3 failed\n");
	toElement(vn,NEXT_SIBLING);
	i2 = getText(vn);
	if ((compareRawTokenString(vn,i2,L"&#97;&#98;&#99;&#100;&#101;&#102;&#103;")==0)
		&& (compareRawTokenString(vn,i2,L"&#97;&#98;&#99;&#100;&#101;&#102;&#103;a")==-1)
		&& (compareRawTokenString(vn,i2,L"&#97;&#98;&#99;&#100;&#101;&#102;&#103")==1)
		&& (wcscmp(toString(vn,i2),L"abcdefg")==0)
		&& (wcscmp(toString(vn,i2),L"abcdef")>0)
		&& (wcscmp(toString(vn,i2),L"abcdefg1")<0))
		printf("test3 4 succeed\n");
	else 
		printf("test3 4 failed\n");
	toElement(vn,NEXT_SIBLING);
	i2 = getText(vn);
	//System.out.printf(toRawString(vn,i2));
	if ((compareRawTokenString(vn,i2,L"&#x61;&#x62;&#x63;&#x64;&#x65;&#x66;&#x67;&#x100;")==0)
		&& (compareRawTokenString(vn,i2,L"&#x61;&#x62;&#x63;&#x64;&#x65;&#x66;&#x67;&#x100;a")==-1)
		&& (compareRawTokenString(vn,i2,L"&#x61;&#x62;&#x63;&#x64;&#x65;&#x66;&#x67;&#x100")==1)
		&& (compareTokenString(vn,i2,L"abcdefg\u0100")==0)
		&& (compareTokenString(vn,i2,L"abcdefg")==1)
		&& (compareTokenString(vn,i2,L"abcdefg\u0101")==-1))
		printf("test3 5 succeed\n");
	else 
		printf("test3 5 failed\n");
	toElement(vn,NEXT_SIBLING);
	i2 = getAttrVal(vn,L"attr");
	if (wcscmp(toString(vn,i2),L"")==0
		|| wcscmp(toString(vn,i2),L"a")<0)
		printf("test3 6 succeed\n");
	else
		printf("test3 6 failed\n");
	toElement(vn,NEXT_SIBLING);
	i2 = getText(vn);
	if (wcscmp(toRawString(vn,i2),L"good&amp;&lt;&gt;&apos;&quot;")==0
		&& wcscmp(toString(vn,i2),L"good&<>")!=0)
		printf("test3 7 succeed\n");
	else
		printf("test3 7 failed\n");
}
static void xtest1(){
//int i1,i2,i3,k;
	UCSChar* s1;
	char ba[]="<AAA>"
            "<BBB/>"
            "<CCC/>"
            "<BBB/>"
            "<BBB/>"
            "<DDD>"
            "<BBB/>"
            "</DDD>"
            "<CCC/>" 
            "</AAA>";
	int i;
	VTDNav* vn = parseString(ba,strlen(ba));
	AutoPilot* ap = createAutoPilot(vn);
	if (selectXPath(ap,L"/AAA")){
		while((i=evalXPath(ap))!=-1){
			if (compareTokenString(vn,i,L"AAA")!=0)
				println("test1 1 failed");
			else 
				println("test1 1 succeed");
		}
	}
}
static void xtest2(){
int i1,i2,i3,k;
	UCSChar* s1;
	char ba[]="<test2>"
        		"<e1>good</e1>"
        		"<e1>bad</e1>"
        		"<f1>bad</f1>"
        		"<f1>good</f1>"
        		"</test2>";

	VTDNav* vn = parseString(ba,strlen(ba));
	AutoPilot* ap = createAutoPilot(vn);
	if (selectXPath(ap,L"/test2/e1=/test2/f1")){
		if(evalXPathToBoolean(ap))
			println("test2 1 succeed");
		else 
			println("test2 1 failed");
	}

	if (selectXPath(ap,L"/test2/e1!=/test2/f1")){
		if(evalXPathToBoolean(ap))
			println("test2 2 succeed");
		else 
			println("test2 2 failed");
	}

	if (selectXPath(ap,L"/test2/e1 > /test2/f1")){
		if(evalXPathToBoolean(ap))
			println("test2 3 succeed");
		else 
			println("test2 3 failed");
	}

	if (selectXPath(ap,L"/test2/e1 >= /test2/f1")){
		if(evalXPathToBoolean(ap))
			println("test2 4 succeed");
		else 
			println("test2 4 failed");
	}

	if (selectXPath(ap,L"/test2/e1 <= /test2/f1")){
		if(evalXPathToBoolean(ap))
			println("test2 5 succeed");
		else 
			println("test2 5 failed");
	}

	if (selectXPath(ap,L"/test2/e1 < /test2/f1")){
		if(evalXPathToBoolean(ap))
			println("test2 6 succeed");
		else 
			println("test2 6 failed");
	}

	if (selectXPath(ap,L"/test2/e1 < /test2/e1")){
		if(evalXPathToBoolean(ap))
			println("test2 7 succeed");
		else 
			println("test2 7 failed");
	}
	if (selectXPath(ap,L"/test2/e1 <= /test2/e1")){
		if(evalXPathToBoolean(ap))
			println("test2 8 succeed");
		else 
			println("test2 8 failed");
	}

	if (selectXPath(ap,L"/test2/e1 > /test2/e1")){
		if(evalXPathToBoolean(ap))
			println("test2 9 succeed");
		else 
			println("test2 9 failed");
	}
	if (selectXPath(ap,L"/test2/e1 >= /test2/e1")){
		if(evalXPathToBoolean(ap))
			println("test2 10 succeed");
		else 
			println("test2 10 failed");
	}
	if (selectXPath(ap,L"/test2/e1 = /test2/e1")){
		if(evalXPathToBoolean(ap))
			println("test2 11 succeed");
		else 
			println("test2 11 failed");
	}
	if (selectXPath(ap,L"/test2/e1 != /test2/e1")){
		if(evalXPathToBoolean(ap))
			println("test2 12 succeed");
		else 
			println("test2 12 failed");
	}

	if (selectXPath(ap,L"/test2/*[starts-with(name(.),'e')] !=/test2/f1")){
		if(evalXPathToBoolean(ap))
			println("test2 12 succeed");
		else 
			println("test2 12 failed");
	}
}
static void xtest3(){
int i1,i2,i3,k;
 double d ;
	UCSChar* s1;
	char ba[]="<test2>"
             "<e1 attr='1'>4</e1>" 
             "<e1 attr='2'>44</e1>"
             "<f1 attr='3'>1000</f1>" 
             "<f1 attr='4' attr1='6'>10e5</f1>" 
             "</test2>";

	VTDNav* vn = parseString(ba,strlen(ba));
	AutoPilot* ap = createAutoPilot(vn);

	if (selectXPath(ap,L"/test2/*/@* !=4")){
	if (evalXPathToBoolean(ap))
		println("test3 1 succeed");
	else
		println("test3 1 failed");
	}

	if (selectXPath(ap,L"/test2/*/@*=4"))
	if (evalXPathToBoolean(ap))
		println("test3 2 succeed");
	else
		println("test3 2 failed");

	if (selectXPath(ap,L"/test2/*/@* >4"))
	if (evalXPathToBoolean(ap))
		println("test3 3 succeed");
	else
		println("test3 3 failed");

	if (selectXPath(ap,L"/test2/*/@* <=6"))
	if (evalXPathToBoolean(ap))
		println("test3 4 succeed");
	else
		println("test3 4 failed");

	if (selectXPath(ap,L"/test2/*/@* <4 "))
	if (evalXPathToBoolean(ap))
		println("test3 5 succeed");
	else
		println("test3 5 failed");

	if (selectXPath(ap,L"/test2/*/@* <=6"))
	if (evalXPathToBoolean(ap))
		println("test3 6 succeed");
	else
		println("test3 6 failed");

	if (selectXPath(ap,L"/test2/*/@* =6"))
	if (evalXPathToBoolean(ap))
		println("test3 7 succeed");
	else
		println("test3 7 failed");

	if (selectXPath(ap,L"/test2/*/@* =4"))
	if (evalXPathToBoolean(ap))
		println("test3 8 succeed");
	else
		println("test3 8 failed");

	if (selectXPath(ap,L"/test2/* <=1000"))
	if (evalXPathToBoolean(ap))
		println("test3 9 succeed");
	else
		println("test3 9 failed");

	if (selectXPath(ap,L"/test2/* =1000"))
	if (evalXPathToBoolean(ap))
		println("test3 10 succeed");
	else
		println("test3 10 failed");

	if (selectXPath(ap,L"/test2/* !=1000"))
	if (evalXPathToBoolean(ap))
		println("test3 11 succeed");
	else
		println("test3 11 failed");

	if (selectXPath(ap,L"/test2/* >=1000"))
	if (evalXPathToBoolean(ap))
		println("test3 12 succeed");
	else
		println("test3 12 failed");

	if (selectXPath(ap,L"/test2/* >1000"))
	if (evalXPathToBoolean(ap))
		println("test3 13 succeed");
	else
		println("test3 13 failed");
	//printExprString(ap);
    //if (selectXPath(ap,L"/test2/*/@*[. > 1]")){
	if (selectXPath(ap,L"/test2/*/@*[. > 1]")){
		//printExprString(ap);
		int a=0;
		while((i1=evalXPath(ap))!=-1){
			a++;
		}
	}

	if (selectXPath(ap,L"count(/test2/*/@*[. > 1])")){
		//printExprString(ap);
		d = evalXPathToNumber(ap);
	}
	
	if (selectXPath(ap,L"/test2/* > count(/test2/*/@*[. > 1]) "))
	if (evalXPathToBoolean(ap))
		println("test3 14 succeed");
	else
		println("test3 14 failed");

	if (selectXPath(ap,L"/test2/* = count(/test2/*/@*[. > 1]) "))
	if (evalXPathToBoolean(ap))
		println("test3 15 succeed");
	else
		println("test3 15 failed");

	if (selectXPath(ap,L"/test2/* < count(/test2/*/@*[. > 1]) "))
	if (!evalXPathToBoolean(ap))
		println("test3 16 succeed");
	else
		println("test3 16 failed");  
	if (selectXPath(ap,L"/test2/* > count(/test2/*/@*[ 1 < .]) "))
	if (evalXPathToBoolean(ap))
		println("test3 17 succeed");
	else
		println("test3 17 failed");

	if (selectXPath(ap,L"/test2/* = count(/test2/*/@*[1 < .]) "))
	if (evalXPathToBoolean(ap))
		println("test3 18 succeed");
	else
		println("test3 18 failed");

	if (selectXPath(ap,L"/test2/* < count(/test2/*/@*[1 < .]) "))
		if (!evalXPathToBoolean(ap))
			println("test3 19 succeed");
		else
			println("test3 19 failed");  

	if (selectXPath(ap,L"  count(/test2/*/@*[1 < .])< /test2/* "))
		if (evalXPathToBoolean(ap))
			println("test3 20 succeed");
		else
			println("test3 20 failed");

	if (selectXPath(ap,L" count(/test2/*/@*[1 < .])= /test2/* "))
		if (evalXPathToBoolean(ap))
			println("test3 21 succeed");
		else
			println("test3 21 failed");

	if (selectXPath(ap,L" count(/test2/*/@*[1 < .])> /test2/* "))
		if (!evalXPathToBoolean(ap))
			println("test3 22 succeed");
		else
			println("test3 22 failed"); 
}

static void xtest4(){
	int i1,i2,i3,k;
	UCSChar* s1;
	char ba[]="<test2>"
		"<e1 attr='1'>4</e1>" 
		"<e1 attr='2'>44</e1>"
		"<f1 attr='3'>1000</f1>" 
		"<f1 attr='4' attr1='6' attr2=''>10e5</f1>" 
		"</test2>";

	VTDNav* vn = parseString(ba,strlen(ba));
	AutoPilot* ap = createAutoPilot(vn);
	if (selectXPath(ap,L"/test2/*/@* !='4'"))
	if (evalXPathToBoolean(ap))
		println("test4 1 succeed");
	else
		println("test4 1 failed");

	if (selectXPath(ap,L"/test2/*/@* ='4'"))
	if (evalXPathToBoolean(ap))
		println("test4 2 succeed");
	else
		println("test4 2 failed");

	if (selectXPath(ap,L"/test2/*/@* >='4'"))
	if (evalXPathToBoolean(ap))
		println("test4 3 succeed");
	else
		println("test4 3 failed");

	if (selectXPath(ap,L"/test2/*/@* >'4'"))
	if (evalXPathToBoolean(ap))
		println("test4 4 succeed");
	else
		println("test4 4 failed");

	if (selectXPath(ap,L"/test2/*/@* <'4'"))
	if (evalXPathToBoolean(ap))
		println("test4 5 succeed");
	else
		println("test4 5 failed");

	if (selectXPath(ap,L"/test2/*/@* <='4'"))
	if (evalXPathToBoolean(ap))
		println("test4 6 succeed");
	else
		println("test4 6 failed");

	if (selectXPath(ap,L"/test2/*/@* <='4'"))
	if (evalXPathToBoolean(ap))
		println("test4 7 succeed");
	else
		println("test4 7 failed");

	if (selectXPath(ap,L"/test2/*/@* =''"))
	if (evalXPathToBoolean(ap))
		println("test4 8 succeed");
	else
		println("test4 8 failed");

	if (selectXPath(ap,L"/test2/*/@* >=''"))
	if (evalXPathToBoolean(ap))
		println("test4 9 succeed");
	else
		println("test4 9 failed");

	if (selectXPath(ap,L"not(/test2/*/@* ='')"))
	if (!evalXPathToBoolean(ap))
		println("test4 10 succeed");
	else
		println("test4 10 failed");

	if (selectXPath(ap,L"not(/test2/*/@* ='')"))
	if (!evalXPathToBoolean(ap))
		println("test4 11 succeed");
	else
		println("test4 11 failed");

	if (selectXPath(ap,L"not(/test2/*/@* ='')"))
	if (!evalXPathToBoolean(ap))
		println("test4 12 succeed");
	else
		println("test4 12 failed");

	if (selectXPath(ap,L"/test2/*/@* = /test2/*"))
	if (evalXPathToBoolean(ap))
		println("test4 13 succeed");
	else
		println("test4 13 failed");

	if (selectXPath(ap,L"/test2/*/@* >= /test2/*"))
	if (evalXPathToBoolean(ap))
		println("test4 14 succeed");
	else
		println("test4 14 failed");

	if (selectXPath(ap,L"/test2/*/@* <= /test2/*"))
	if (evalXPathToBoolean(ap))
		println("test4 15 succeed");
	else
		println("test4 15 failed");

	if (selectXPath(ap,L"'4' = /test2/*/@* "))
	if (evalXPathToBoolean(ap))
		println("test4 16 succeed");
	else
		println("test4 16 failed");

	if (selectXPath(ap,L" '4' = /test2/*/@* "))
	if (evalXPathToBoolean(ap))
		println("test4 17 succeed");
	else
		println("test4 17 failed");

	if (selectXPath(ap,L"'4' <= /test2/*/@* "))
	if (evalXPathToBoolean(ap))
		println("test4 18 succeed");
	else
		println("test4 18 failed");

	if (selectXPath(ap,L"'4' < /test2/*/@* "))
	if (evalXPathToBoolean(ap))
		println("test4 19 succeed");
	else
		println("test4 19 failed");

	if (selectXPath(ap,L"'4' > /test2/*/@* "))
	if (evalXPathToBoolean(ap))
		println("test4 20 succeed");
	else
		println("test4 20 failed");

	if (selectXPath(ap,L"/test2/*/@* <='4'"))
	if (evalXPathToBoolean(ap))
		println("test4 21 succeed");
	else
		println("test4 21 failed");

	if (selectXPath(ap,L"/test2/*/@* <='4'"))
	if (evalXPathToBoolean(ap))
		println("test4 22 succeed");
	else
		println("test4 22 failed");

	if (selectXPath(ap,L"/test2/*/@* =''"))
	if (evalXPathToBoolean(ap))
		println("test4 23 succeed");
	else
		println("test4 23 failed");

	if (selectXPath(ap,L"/test2/*/@* >=''"))
	if (evalXPathToBoolean(ap))
		println("test4 24 succeed");
	else
		println("test4 24 failed");

	if (selectXPath(ap,L"not(/test2/*/@* ='')"))
	if (!evalXPathToBoolean(ap))
		println("test4 25 succeed");
	else
		println("test4 25 failed");

	if (selectXPath(ap,L"not( '' = /test2/*/@*)"))
	if (!evalXPathToBoolean(ap))
		println("test4 26 succeed");
	else
		println("test4 26 failed");

	if (selectXPath(ap,L"not(/test2/*/@* ='')"))
	if (!evalXPathToBoolean(ap))
		println("test4 27 succeed");
	else
		println("test4 27 failed");
}
int main(int argc, char *argv[])
{  
	//char a[0xfffffff];
	//14 parseTest_BR
	//9  parseTest
	//15 NavTestNS_BR
	//16 NavTestNS
	//17 APTest_BR
	//10 NavTest
	//11 APTest
	//12 MiscTest
	//18 MiscTest_BR
	//19 NavTest_BR
	//20 vnTester

   int test =22;
   if (test==22){
	   exception e;
	   int ii = 0;
		FILE *f = NULL; 
		FILE *fout = NULL;
		UByte *xml = NULL;
		VTDGen *vg = NULL;
		VTDNav *vn = NULL;

		f = fopen(argv[0],"rb");
		fout =fopen(argv[1],"wb");
	//f = fopen("d://ximple-dev//testcases//VTDGen//bad//nt_8_2.xml","r");
		ii=(int)_filelength(f->_file);
	//printf("size of the file is %d \n",ii);
		xml = (UByte *)malloc(sizeof(UByte)*ii); 
		fread(xml,sizeof(UByte),ii,f);
		//setDoc(vg,xml,ii);
		Try{
			vg = createVTDGen();
			printf(argv[0]);
			printf("\n");
			printf(argv[1]);
			printf("\n");
			parse(vg,TRUE);
			writeIndex_VTDGen(vg,fout);
			fclose(f);
			fclose(fout);
		}Catch(e){
		}
   }

   if (test == 21){
		XPathTester();
   } 
   else if (test == 20){
		vnTester();
   } 
   else if (test ==1){
	int i,a;
	unsigned int k; 
	Long l;
	UCSChar *string;
	UCSChar *as;
	exception e;
	printf("size of UCSChar is %d \n",sizeof(UCSChar));
	printf("size of wchar_t is %d \n",sizeof(long long));
	printf("size of char is %d \n",(UByte)-1);
	printf("string length is %d \n",wcslen(L"this"));
	as= (UCSChar *)malloc(10*sizeof(UCSChar));
	for (i=0;i<10;i++){
		as[i]=128;
	}
	for (i=0;i<10;i++){
		wprintf(L"%u",as[i]);
	}
	
	/*ArrayList *al = createArrayList();
	for (int i=0;i<1000;i++)
	{
		int *ia = (int*) malloc(1000*sizeof(int));
		for(int k=0;k<1000;k++)
			ia[k]=i;
		add(al,ia);
	}
	
	for (int i=0;i<1000;i++)
	{
		int *ia = (int*)get(al,i);
		for (int k=0;k<1000;k++)
		{
			if (ia[k]!=i)
			{
				printf(" a mismatch %d \n",i);
			}
		}
	}
	recycleArrayList(al);*/
	//printf("a %d \n",the_exception_context->caught);
	Try{
		e.et = out_of_mem;
		e.subtype = 0;
		Throw e;
	}
	Catch (e){
		printf("e.et -->%d \n",e.et);
		printf("e.subtype --> %d \n",e.subtype);
	}


	printf("size of long is %d \n",sizeof(Long));
	//printf("size of boolean is %d \n",true);
	l =  0xffffffffffffffff;
    i = (unsigned int) l;
	a = 1<<-1;
	string = L"this is cool!";
	k = wcslen(string);
	printf(" k equals %d", k);

	printf ("\n ------------------ \n");
	string = L"*";
	if (wcscmp(string, L"*")==0)
		wprintf(L" string value of s is * \n");
   }
   if (test ==2){
	   int i,j,z;
	   exception e;
    Try {
        int* ia = malloc(10*sizeof(int));

        ContextBuffer* cb = createContextBuffer2(4, 10);
        for (i = 0; i < 10; i++) {
            for (j = 0; j < 10; j++) {
                ia[j] = i;
            }
            store(cb,ia);
        }
		/*for ( i=0;i<7;i++){
			int* p = cb->al->storage[i];
			printf("%d%d%d%d %d%d%d%d %d%d%d%d %d%d%d%d \n",
				p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7],p[8],p[9],p[10],p[11],p[12],p[13],p[14],p[15]);
		}*/
        //cb.store(ia);
        for (i = 9; i >= 0; i--) {
			for (z=0;z<10;z++){
				ia[z] = 0;
			}
            load(cb,ia);
            //System.out.printf(""+ia[0]);
            for (j = 9; j >= 0; j--) {
                if (ia[j] != i) {
                    printf(" store error %d %d %d", i , j ,ia[j]);
                }
            }
        }
        printf("success");
    } Catch (e) {
        printf(" exception caught ");
    }
   }
   // fast int buffer test
   if (test == 3){
	   exception e;
	   int i;
	   int *ia;
	   Try{
			FastIntBuffer *fib = createFastIntBuffer2(10);
			for (i=0;i<1000000;i++){
				appendInt(fib,i);
			}
			Throw e;
			for(i=0;i<1000000;i++){
				int j= intAt(fib,i);
				if (j!=i){
					printf("mismatch \n");
				}
			}
			ia = getIntArray(fib,2000,1000);
			for(i = 2000;i<3000;i++)
			{
				int j= intAt(fib,i);
				if (j!=i){
					printf("mismatch \n");
					freeFastIntBuffer(fib);
				}
			}
			freeFastIntBuffer(fib);
			free(ia);
	   //FastIntBuffer *fib = createFastIntBuffer();

	   }Catch(e){
		   printf("exception occured \n");
	   }
   }
   // fast long buffer test
   if (test == 4){
	   exception e;
	   Long i;
	   Long *ia, *ib;
	   Long j;
	   Try{
			FastLongBuffer *flb = createFastLongBuffer2(12);
			ib = malloc(sizeof(Long)*1000000);
			for (i=0L;i<1000000L;i++){
				ib[i]=i;				
			}
			appendLongArray(flb,ib,1000000);
			clearFastLongBuffer(flb);
			appendLongArray(flb,ib,1000000);
			clearFastLongBuffer(flb);
			appendLongArray(flb,ib,1000000);
			//Throw e;
			for(i=0L;i<1000000L;i++){
			    j= longAt(flb,i);
				if (j!=i){
					printf("mismatch \n");
				}
			}
			ia = getLongArray(flb,2000,1000);
			for(i = 2000L;i<3000L;i++)
			{
				j = ia[i-2000];
				if (j!=i){
					printf("mismatch \n");
					//freeFastLongBuffer(flb);
				}
				
			}
			printf("array list size ---> %d, capacity ---->%d ",
				flb->al->size,
				flb->al->capacity); 
			printf("flb size ---> %d, capacity ---->%d ",
				flb->size,
				flb->capacity); 
			freeFastLongBuffer(flb);
			free(ia);
			printf(" long is %I64d\n", 0xffffffffffL);

	   //FastIntBuffer *fib = createFastIntBuffer();

	   }Catch(e){
		   printf("exception occured \n");
	   }
   }
   if (test == 5){
	   exception e;
	   Try{
		   VTDGen *vg = NULL;
		   VTDNav *vn = NULL;
		   char* ca= "<abc> <ab a = '1sfsf' b=\" \"> cde <!-- abcd --></ab> </abc>";
		   Byte *ba = (Byte *)ca;
		   unsigned int len = strlen(ca);
		   while(TRUE){
			   ba = (Byte *)malloc(sizeof(Byte)*len);
			   memcpy(ba,ca,len);
			   vg = createVTDGen();
			   setDoc(vg,ba,(int)strlen(ca));
			   parse(vg,TRUE);
			   vn = getNav(vg);
			   clear(vg);
			   freeVTDGen(vg);
			   freeVTDNav(vn);
		   }

	   }Catch (e){
		   printf(e.msg);
		   printf("\n");
		   if (e.et = parse_exception){
			   printf(e.sub_msg);
		   }
	   }
   }

   if (test == 6){ 
	   exception e;
	   Try{			  
		   int i = Character_digit('a',16);
	   	   printf("i 's val is %d \n", i);
		   printf("i << 2 is %d \n", i<<2);
	   }Catch (e){
		   printf(e.msg);
	   }
   }
   if (test == 7){
	   int i;
	   int a=0x0a0b0c0d;
	   unsigned int d;
	   
	   Byte* b = malloc(4);
	   a= 0x0a0b0c0d;

	   //============================
	   // to encode token type 
	   // type << 4

	   // to decode token type
	   // record & 0xf0 >> 4
	   
	   //============================
	   // to encode nesting depth
	   // ((depth & 0x0f) <<8) | (a&0xf)

	   // to decode nesting depth
	   // (record & 0x0f) | ((record & 0xf000)>>8) 

	   //============================
	   // to encode offset
	   // ((os & 0xff) <<24) | 
	   // ((os & 0xff00)<<8) | 
	   // ((os & 0xff0000)>>8) | 
	   // ((os & 0xff000000) >> 24)

	   // to decode 
	   // first cast record(64 bit) into a 32-bit integer i
	   // then 
	   // os = ((i & 0xff) <<24) |
	   //      ((i & 0xff00)<<8) |
	   //	   ((i & 0xff0000) >> 8) | 
	   //      ((i & 0xff000000) >> 24);

	   //=============================
	   // to encode prefix length and qname length
	   // qname length part
	   // ((qlen & 0xff) << 24) | ((qlen & 0x700) << 8)
	   // prefix length part
	   // ((plen & 0x1f) << 19) |((plen & 0x1e0) << 3
	   
	   // to decode prefix length and qname length
	   // vtd1 = (vtd & 0xff070000);
	   // vtd2 = (vtd & 0xf8070000);
	   // qlen = ((vtd1 & 0xff000000) >> 24) | ((vtd1 & 0x70000) >> 8);
	   // prefix len
	   // plen = ((vtd2 & 0xf00) >> 3) | ((vtd2 & 0xf80000) >> 19); 

	
	   //==============================
	   // to encode ns bits
	   // ns << 34
	   // to decode ns bits
       // (vtd & (0xc0>> 32)) << 34  
	   //d= ((a&0xf0)<<8)|(a&0xf);

	   memcpy(b,(&a),4);
	   for (i=0;i<4;i++){
		   printf(" %x ", (unsigned char) *(b+i));
	   }
	   d = ((a & 0xff) <<24) | 
		   ((a & 0xff00) << 8) |
		   ((a & 0xff0000) >> 8) |
		   ((a & 0xff000000) >> 24);
	   memcpy(b,(&d),4);
	   for (i=0;i<4;i++){
		   printf(" %x ", (unsigned char) *(b+i));
	   }

	   printf("\n");
	   d = 0x3fffffff;
	   //d = (d&0x0f)|((d&0xf000)>>8);
	   d = (d & 0x3f) | ((d & 0x3fffffc0)<<2);
	   d = ((d & 0xff) <<24) | 
		   ((d & 0xff00) << 8) |
		   ((d & 0xff0000) >> 8) |
		   ((d & 0xff000000) >> 24);
	   memcpy(b,(&d),4);
	   for (i=0;i<4;i++){
		   printf(" %x ", (unsigned char) *(b+i));
	   }

	   printf(" \n");
	   d = (d & 0x3f) | ((d& 0xffffff00)>>2);
	   d = ((d & 0xff) <<24) | 
		   ((d & 0xff00) << 8) |
		   ((d & 0xff0000) >> 8) |
		   ((d & 0xff000000) >> 24);
	   memcpy(b,(&d),4);
	   for (i=0;i<4;i++){
		   printf(" %x ", (unsigned char) *(b+i));
	   }
   }

   if (test == 8){
	   unsigned int i = 0xffffffff;
	   Long l=0x8fffffff7fffffff;
	   i=(unsigned int)(l>>32);
	   //i = 0x7fffffff>>31;
	   //i = (unsigned int)l;
	   //i = i>>28;
	   printf("shifted i is %x",i>>1);
   }

if (test==9){
		exception e;
		UByte a = -1;
		int i = a;
		//struct _stat fs;
		FILE *f = NULL;
		UByte *xml = NULL;
		VTDGen *vg = NULL;
		VTDNav *vn = NULL;
		//printf(" i  equals %d \n",i);
		char *dir = "d://ximple-dev//testcases//VTDGen//bad//";
		char *fullname = (char *)malloc(sizeof(char)*50);
		sprintf(fullname,"%s%s",dir,argv[1]);
		f = fopen(fullname,"r");
		//f = fopen("d://ximple-dev//testcases//VTDGen//bad//nt_8_2.xml","r");
		i=(int)_filelength(f->_file);
		printf("size of the file is %d \n",i);
		xml = (UByte *)malloc(sizeof(UByte)*i); 
		fread(xml,sizeof(UByte),i,f);
		Try{
			vg = createVTDGen();
			setDoc(vg,xml,i);
			parse(vg,TRUE);
			vn = getNav(vg);
			for (i=0;i<vn->vtdSize;i++)
			{
				printf("offset is --> %d; ",getTokenOffset(vn,i));
				printf("length is --> %d; ",getTokenLength(vn,i));
				
				printf("type is --> %d; ", getTokenType(vn,i));
				printf("depth is --> %d \n",getTokenDepth(vn,i));
			}
		}
		Catch (e){
			if (e.et == parse_exception)
			printf("exception e ==> %s \n %s\n", e.msg,e.sub_msg);
		}
   }

if (test==14){
		exception e;
		UByte a = -1;
		int i = a;
		//struct _stat fs;
		FILE *f = NULL;
		UByte *xml = NULL;
		VTDGen *vg = NULL;
		VTDNav *vn = NULL;
		//printf(" i  equals %d \n",i);
		char *dir = "d://ximple-dev//testcases//VTDGen//bad//";
		char *fullname = (char *)malloc(sizeof(char)*50);
		//argv[1] = "nt_0_1.xml";
		sprintf(fullname,"%s%s",dir,argv[1]);
		f = fopen(fullname,"r");
		//f = fopen("d://ximple-dev//testcases//VTDGen//bad//nt_8_2.xml","r");
		i=(int)_filelength(f->_file);
		printf("size of the file is %d \n",i);
		xml = (UByte *)malloc(sizeof(UByte)*i); 
		fread(xml,sizeof(UByte),i,f);
		Try{
			vg = createVTDGen();
			setDoc_BR(vg,xml,i);
			parse(vg,TRUE);
			setDoc_BR(vg,xml,i);
			parse(vg, TRUE);
			vn = getNav(vg);
			for (i=0;i<vn->vtdSize;i++)
			{
				printf("offset is --> %d; ",getTokenOffset(vn,i));
				printf("length is --> %d; ",getTokenLength(vn,i));
				
				printf("type is --> %d; ", getTokenType(vn,i));
				printf("depth is --> %d \n",getTokenDepth(vn,i));
			}
			freeVTDGen(vg);
			freeVTDNav(vn);
			free(xml);
		}
		Catch (e){
			if (e.et == parse_exception)
			printf("exception e ==> %s \n %s\n", e.msg,e.sub_msg);
		}
   }
if (test ==15){
	//struct _stat fs;
	
	FILE *f = NULL;
	UByte *xml = NULL;
	VTDGen *vg = NULL;
	VTDNav *vn = NULL;
	char *dir = "d://ximple-dev//testcases//VTDNav//";
	char *fullname = (char *)malloc(sizeof(char)*50);
	sprintf(fullname,"%s%s",dir,argv[1]);
    if (NavTestNS_BR(fullname)==TRUE)
		printf("%s passed \n", fullname);
}

if (test ==16){
	//struct _stat fs;
	
	FILE *f = NULL;
	UByte *xml = NULL;
	VTDGen *vg = NULL;
	VTDNav *vn = NULL;
	char *dir = "d://ximple-dev//testcases//VTDNav//";
	char *fullname = (char *)malloc(sizeof(char)*50);
	sprintf(fullname,"%s%s",dir,argv[1]);
    if (NavTestNS(fullname)==TRUE)
		printf("%s passed \n", fullname);
}

if (test ==17){
	FILE *f = NULL;
	UByte *xml = NULL;
	VTDGen *vg = NULL;
	VTDNav *vn = NULL;
	char *dir = "d://ximple-dev//testcases//AutoPilot//";
	char *fullname = (char *)malloc(sizeof(char)*50);
	sprintf(fullname,"%s%s",dir,argv[1]);
	if (APTest_BR(fullname)==TRUE)
		printf("%s passed \n", fullname);
}

if (test == 18){
	//struct _stat fs;

	FILE *f = NULL;
	UByte *xml = NULL;
	VTDGen *vg = NULL;
	VTDNav *vn = NULL;
	char *dir = "d://ximple-dev//testcases//VTDNav//";
	char *fullname = (char *)malloc(sizeof(char)*50);
	sprintf(fullname,"%s%s",dir,argv[1]);
    if (MiscTest_BR(fullname)==TRUE)
		printf("%s passed \n", fullname);
}

if (test ==19){
	//struct _stat fs;
	
	FILE *f = NULL;
	UByte *xml = NULL;
	VTDGen *vg = NULL;
	VTDNav *vn = NULL;
	char *dir = "d://ximple-dev//testcases//VTDNav//";
	char *fullname = (char *)malloc(sizeof(char)*50);
	sprintf(fullname,"%s%s",dir,argv[1]);
    if (NavTest_BR(fullname)==TRUE)
		printf("%s passed \n", fullname);
}

if (test ==10){
	//struct _stat fs;
	
	FILE *f = NULL;
	UByte *xml = NULL;
	VTDGen *vg = NULL;
	VTDNav *vn = NULL;
	char *dir = "d://ximple-dev//testcases//VTDNav//";
	char *fullname = (char *)malloc(sizeof(char)*50);
	sprintf(fullname,"%s%s",dir,argv[1]);
    if (NavTest(fullname)==TRUE)
		printf("%s passed \n", fullname);
}

if (test == 11){
	FILE *f = NULL;
	UByte *xml = NULL;
	VTDGen *vg = NULL;
	VTDNav *vn = NULL;
	char *dir = "d://ximple-dev//testcases//AutoPilot//";
	char *fullname = (char *)malloc(sizeof(char)*50);
	sprintf(fullname,"%s%s",dir,argv[1]);
	if (APTest(fullname)==TRUE)
		printf("%s passed \n", fullname);
}

if (test==12){
	FILE *f = NULL;
	UByte *xml = NULL;
	VTDGen *vg = NULL;
	VTDNav *vn = NULL;
	char *dir = "d://ximple-dev//testcases//VTDNav//";
	char *fullname = (char *)malloc(sizeof(char)*50);
	sprintf(fullname,"%s%s",dir,argv[1]);
	if (MiscTest(fullname)==TRUE)
		printf("%s passed \n", fullname);
}

if (test == 13){
	exception e;
	FILE *f = NULL;
	FILE *fo = NULL;
	int i = 0;
	
	Long l = 0;
	int len = 0;
	int offset = 0;
	
	char* filename = "c://soap2.xml";

	UByte *xml = NULL; // this is the buffer containing the XML content, UByte means unsigned byte
	VTDGen *vg = NULL; // This is the VTDGen that parses XML
	VTDNav *vn = NULL; // This is the VTDNav that navigates the VTD records
	UByte *sm = "\n================\n";

	// allocate a piece of buffer then reads in the document content
	// assume "c:\soap2.xml" is the name of the file
	f = fopen(filename,"r");
	fo = fopen("c://out.txt","w");

	i = (int) _filelength(f->_file);
	printf("size of thefile is %d \n",i);
	xml = (UByte *)malloc(sizeof(UByte) *i);
	fread(xml,sizeof(UByte),i,f);
	Try{
		vg = createVTDGen();
		setDoc(vg,xml,i);
		parse(vg,TRUE);
		vn = getNav(vg);
		if (toElementNS(vn,FC,L"http://www.w3.org/2003/05/soap-envelope",L"Header")) //nav to soap header
		{
			if (toElement(vn,FC))
			{
				do{
					if (hasAttrNS(vn,L"http://www.w3.org/2003/05/soap-envelope",L"mustUnderstand")){
						wprintf(toString(vn,getCurrentIndex(vn)));

						printf("\n");
						l = getElementFragment(vn);
						offset = (int) l;
						len = (int) (l>>32);
						fwrite((char *)(xml+offset),sizeof(UByte),len,fo);
						fwrite((char *) sm,sizeof(UByte),strlen(sm),fo);
					}
				}
				while(toElement(vn,NS));
			}
			else 
				printf("Header has no child elements \n");
		}
		else
			printf("XML has no header \n");
		fclose(f);
		fclose(fo);

	}
	Catch (e) {
		if (e.et == parse_exception)
			printf("parse exception e ==> %s \n %s\n", e.msg, e.sub_msg);		
	}
	
}

return 0;
}
#endif
#if 0
int main(){

	exception e;
	FILE *f = NULL;
	FILE *fo = NULL;
	int i = 0;
	
	Long l = 0,l2=0;
	int len = 0;
	int offset = 0;
	
	char* filename = "d://newpo.xml";

	UByte *xml = NULL; // this is the buffer containing the XML content, UByte means unsigned byte
	VTDGen *vg = NULL; // This is the VTDGen that parses XML
	VTDNav *vn = NULL; // This is the VTDNav that navigates the VTD records
	AutoPilot *ap = NULL;
	XMLModifier *xm =NULL;
	UByte *sm = "\n================\n";

	l2 = 2LL<<32;
	printf("l2 ==> %l \n",l2);
	// allocate a piece of buffer then reads in the document content
	// assume "c:\soap2.xml" is the name of the file
	f = fopen(filename,"rb");
	fo = fopen("d://out.txt","w");

	i = (int) _filelength(f->_file);
	printf("size of thefile is %d \n",i);
	xml = (UByte *)malloc(sizeof(UByte) *i);
	fread(xml,sizeof(UByte),i,f);
	Try{
		vg = createVTDGen();
		setDoc(vg,xml,i);
		parse(vg,TRUE);
		vn = getNav(vg);
		xm = createXMLModifier();
		
		ap = createAutoPilot2();
				if (selectXPath(ap,L"substring(\"12345\",1.5,2.6)")){
			printExprString(ap);
		    wprintf(L"the result ====> %s \n",evalXPathToString(ap));
		}
		if (selectXPath(ap,L"substring(\"12345\",0,3)")){
			printExprString(ap);
		    wprintf(L"\nthe result ====> %s\n",evalXPathToString(ap));
		}
		if (selectXPath(ap,L"substring(\"12345\",0 div 0, 3)")){
			printExprString(ap);
		    wprintf(L"\nthe result ====> %s\n",evalXPathToString(ap));
		}
		if (selectXPath(ap,L"substring(\"12345\",1,0 div 0)")){
			printExprString(ap);
		    wprintf(L"\nthe result ====> %s\n",evalXPathToString(ap));
		}
		if (selectXPath(ap,L"substring(\"12345\",-42,1 div 0)")){
			printExprString(ap);
		    wprintf(L"\nthe result ====> %s\n",evalXPathToString(ap));
		}
		if (selectXPath(ap,L"substring(\"12345\",-1 div 0,1 div 0)")){
			printExprString(ap);
		    wprintf(L"\nthe result ====> %s\n",evalXPathToString(ap));
		}

		if (selectXPath(ap,L"substring-after('1/6/1999','/6')")){
			printExprString(ap);
			wprintf(L"\n the result ===> %s \n",evalXPathToString(ap));
		}

		if (selectXPath(ap,L"substring-before('1/6/1999','/6')")){
			printExprString(ap);
			wprintf(L"\n the result ===> %s \n",evalXPathToString(ap));
		}
		if (selectXPath(ap,L"normalize-space('    abcd\tabc   ')")){
			printExprString(ap);
			wprintf(L"\n the result ===> %s; \n",evalXPathToString(ap));
		}
		//declareXPathNameSpace(ap,L"ns1",L"http://www.w3.org/2003/05/soap-envelope");
		if (selectXPath(ap,L"/purchaseOrder/items/item[@partNum='872-AA']/USPrice[.>100]")){
			int c=0;
			bind(ap,vn);
			bind4XMLModifier(xm,vn);
			while((i = evalXPath(ap))!= -1){
				UCSChar *c1 = L"<test/>\t";
				UCSChar *c2 = L"200";
				insertBeforeElement(xm,L"<test/>\t");
				updateToken(xm,getText(vn),c2);
				c++;
			}
			output(xm,fo);
		}
		/*
		if (toElementNS(vn,FC,"http://www.w3.org/2003/05/soap-envelope","Header")) //nav to soap header
		{
			if (toElement(vn,FC))
			{
				do{
					if (hasAttrNS(vn,"http://www.w3.org/2003/05/soap-envelope","mustUnderstand")){
						printf(toString(vn,getCurrentIndex(vn)));

						printf("\n");
						l = getElementFragment(vn);
						offset = (int) l;
						len = (int) (l>>32);
						fwrite((char *)(xml+offset),sizeof(UByte),len,fo);
						fwrite((char *) sm,sizeof(UByte),strlen(sm),fo);
					}
				}
				while(toElement(vn,NS));
			}
			else 
				printf("Header has no child elements \n");
		}
		else
			printf("XML has no header \n");*/
		fclose(f);
		fclose(fo);
		if (selectXPath(ap,L"//a | //b |//c")){
			printExprString(ap);
			printf("\n");
		}
		if (selectXPath(ap,L"substring(\"12345\",1.5,2.6)")){
			printExprString(ap);
		    wprintf(L"the result ====> %s \n",evalXPathToString(ap));
		}
		if (selectXPath(ap,L"substring(\"12345\",0,3)")){
			printExprString(ap);
		    wprintf(L"\nthe result ====> %s\n",evalXPathToString(ap));
		}
		if (selectXPath(ap,L"substring(\"12345\",0 div 0, 3)")){
			printExprString(ap);
		    wprintf(L"\nthe result ====> %s\n",evalXPathToString(ap));
		}
		if (selectXPath(ap,L"substring(\"12345\",1,0 div 0)")){
			printExprString(ap);
		    wprintf(L"\nthe result ====> %s\n",evalXPathToString(ap));
		}
		if (selectXPath(ap,L"substring(\"12345\",-42,1 div 0)")){
			printExprString(ap);
		    wprintf(L"\nthe result ====> %s\n",evalXPathToString(ap));
		}
		if (selectXPath(ap,L"substring(\"12345\",-1 div 0,1 div 0)")){
			printExprString(ap);
		    wprintf(L"\nthe result ====> %s\n",evalXPathToString(ap));
		}

		if (selectXPath(ap,L"substring-after('1/6/1999','/6')")){
			printExprString(ap);
			wprintf(L"\n the result ===> %s \n",evalXPathToString(ap));
		}

		if (selectXPath(ap,L"substring-before('1/6/1999','/6')")){
			printExprString(ap);
			wprintf(L"\n the result ===> %s \n",evalXPathToString(ap));
		}
		if (selectXPath(ap,L"normalize-space('    abcd\tabc   ')")){
			printExprString(ap);
			wprintf(L"\n the result ===> %s; \n",evalXPathToString(ap));
		}
		// remember C has no automatic garbage collector
		// needs to deallocate manually.
		freeVTDNav(vn);
		freeVTDGen(vg);
		freeXMLModifier(xm);
		freeAutoPilot(ap);

	}
	Catch (e) {
		if (e.et == parse_exception)
			printf("parse exception e ==> %s \n %s\n", e.msg, e.sub_msg);	
		// manual garbage collection here
		freeVTDGen(vg);
	}
  return 0;
}
#endif
#if 0
int main(){

	exception e;
	FILE *f = NULL;
	//FILE *fo = NULL;
	int i = 0,t=0;
	int result = -1;
	Long l = 0;
	int count = 0;
	//int offset = 0;
	wchar_t *tmpString;
	char* filename = "./servers.xml";

	UByte *xml = NULL; // this is the buffer containing the XML content, UByte means unsigned byte
	VTDGen *vg = NULL; // This is the VTDGen that parses XML
	VTDNav *vn = NULL; // This is the VTDNav that navigates the VTD records
	AutoPilot *ap = NULL;
	UByte *sm = "\n================\n";

	// allocate a piece of buffer then reads in the document content
	// assume "c:\soap2.xml" is the name of the file
	f = fopen(filename,"r");
	//fo = fopen("c://out.txt","w");

	i = (int) _filelength(f->_file);
	printf("size of thefile is %d \n",i);
	xml = (UByte *)malloc(sizeof(UByte) *i);
	fread(xml,sizeof(UByte),i,f);
	Try{
		vg = createVTDGen();
		setDoc(vg,xml,i);
		parse(vg,TRUE);
		vn = getNav(vg);
		ap = createAutoPilot2();
		declareXPathNameSpace(ap,L"ns1",L"http://purl.org/dc/elements/1.1/");
		if (selectXPath(ap,L"//ns1:*")){
			setVTDNav(ap,vn);
			while((result=evalXPath(ap))!= -1){
				wprintf(L"result is %d \n",result);
				tmpString = toString(vn,result);
                wprintf(L"Element name ==> %ls \n",tmpString);
				free(tmpString);
				t = getText(vn);
				if (t!=-1){
					tmpString = toNormalizedString(vn,t);
					wprintf(L" text ==> %ls \n",tmpString);
					free(tmpString);
				}
				wprintf(L"\n =======================\n ");
				count ++;
			}
		}
		wprintf(L"\nTotal number of elements %d \n",count);
		fclose(f);
		//fclose(fo);
		// remember C has no automatic garbage collector
		// needs to deallocate manually.
		freeVTDNav(vn);
		freeVTDGen(vg);
		freeAutoPilot(ap);

	}
	Catch (e) {
		if (e.et == parse_exception)
			printf("parse exception e ==> %s \n %s\n", e.msg, e.sub_msg);	
		// manual garbage collection here
		freeVTDGen(vg);
	}
  return 0;
}	
#endif
#if 0
int main(){

	exception e;
	FILE *f = NULL;
	//FILE *fo = NULL;
	int i = 0, v = 0;
	int t1 =0, t2=0;
	int count=0, par_count=0;
	char* filename = "./bioinfo.xml";
	IntHash *ih = NULL;

	UByte *xml = NULL; // this is the buffer containing the XML content, UByte means unsigned byte
	VTDGen *vg = NULL; // This is the VTDGen that parses XML
	VTDNav *vn = NULL; // This is the VTDNav that navigates the VTD records
	AutoPilot *ap = NULL;
	UByte *sm = "\n================\n";

	// allocate a piece of buffer then reads in the document content
	// assume "c:\soap2.xml" is the name of the file
	f = fopen(filename,"r");
	//fo = fopen("c://out.txt","w");

	i = (int) _filelength(f->_file);
	printf("size of thefile is %d \n",i);
	xml = (UByte *)malloc(sizeof(UByte) *i);
	i = fread(xml,sizeof(UByte),i,f);
	
	Try{
		vg = createVTDGen();
		ap = createAutoPilot2();		

		count = 0;
		if (selectXPath(ap,L"/bix/package/command/parlist")){
			while(t1<10){
				setDoc_BR(vg,xml,i);
				parse(vg,FALSE);
				vn = getNav(vg);			
				bind(ap,vn);
				count = 0;
				while(evalXPath(ap)!= -1){
					count++;
				}
				printf("count is %d \n",count);
				
				resetXPath(ap);
				//freeVTDNav(vn);
				t1++;
			}
			//while(TRUE);
		}
		
		//printf("count is %d \n",count);		
		
		if (selectXPath(ap,L"/bix/package/command/parlist/par")){
			while(evalXPath(ap)!= -1){
				par_count++;
			}
		}
		wprintf(L"count ==> %d \n",count);
		wprintf(L"par_count ==> %d \n",par_count);

		toElement(vn,ROOT);
		selectElement(ap,L"par");
		while(iterateAP(ap)){
			if (getCurrentDepth(vn) == 4){
				v++;
			}
		}
		wprintf(L"verify ==> %d \n",v);
		fclose(f);
		//fclose(fo);
		// remember C has no automatic garbage collector
		// needs to deallocate manually.
		freeVTDNav(vn);
		freeVTDGen(vg);
		freeAutoPilot(ap);
	}
	Catch (e) {
		if (e.et == parse_exception)
			printf("parse exception e ==> %s \n %s\n", e.msg, e.sub_msg);	
		// manual garbage collection here
		freeVTDGen(vg);
	}



#if 0
	Try{
		ih = createIntHash();
		i = 0;
		for (i=0;i<256;i++){
			if (isUniqueIntHash(ih, i) == TRUE){
				printf(" true %d \n",i);
			}else 
				printf(" false %d \n",i);
		}
		for (i=0;i<256;i++){
			if (isUniqueIntHash(ih, i) == TRUE){
				printf(" true %d \n",i);
			}else 
				printf(" false %d \n",i);
		}
		resetIntHash(ih);
		for (i=0;i<256;i++){
			if (isUniqueIntHash(ih, i) == TRUE){
				printf(" true %d \n",i);
			}else 
				printf(" false %d \n",i);
		}
		freeIntHash(ih);
	}
	Catch (e) {
		if (e.et == parse_exception)
			printf("parse exception e ==> %s \n %s\n", e.msg, e.sub_msg);	
		// manual garbage collection here
		freeVTDGen(vg);
	}
  return 0;
#endif
}	
#endif
#if 0
void main(){
	wchar_t* tests[] =	{
L"/descendant::test:*",
L"/descendant::test:*",
L"/descendant::test:e",
L"/descendant::test:f",
L"/root/test:*/text()",
L"a/d[2]/e",
L"*[c or d]",
L"a/c[d]",
L"a/c[d=\"Text for D\"]",
L"a[3][@a1=\"va1\"]",
L"a[@a1=\"va1\"][1]",
L"a[@a1=\"va1\"]",
L"/root/a/c/../@a1",
L"/root/a/..",
L".//c",
L".",
L"//c/d",
L"//c",
L"/root/a//d",
L"/root/a/d",
L"//a",
L"//d",
L"a/d[2]/e",
L"*/d",
L"a[last()]",
L"a[1]",
L"/root/a[1]/@*",
L"/root/a[1]/@a1",
L"text()",
L"*",
L"a",
L"/root",
L"child::*[self::a or self::b][position()=last() - 1]",
L"child::*[self::a or self::b]",
L"child::a/child::c[child::d='Text for D']",
L"child::a/child::c[child::d]",
L"child::a[position()=1][attribute::a1=\"va1\"]",
L"child::a[attribute::a1=\"va1\"][position()=1]",
L"child::a[attribute::a2=\"va2\"]",
L"child::a/child::d[position()=2]/child::e[position()=1]",
L"/descendant::a[position()=2]",
L"child::a[1]/child::d[1]/preceding-sibling::c[position()=1]",
L"child::a[2]/following-sibling::b[position()=1]",
L"child::a[position()=2]",
L"child::a[position()=last()-1]",
L"child::a[position()=last()]",
L"child::a[position()=1]",
L"/descendant::a/child::c",
L"/",
L"/descendant::a",
L"/child::root/child::*/child::d",
L"child::b/descendant::a",
L"self::root",
L"/child::root/child::a/descendant-or-self::a",
L"/child::root/child::a/child::c/ancestor::root",
L"/child::root/descendant::a",
L"/child::root/child::a/attribute::*",
L"/child::root/child::a/attribute::a1",
L"/child::root/child::node()",
L"/child::root/child::a/child::d/child::text()",
L"/child::root/child::*",
L"/child::root/child::a",
L"/root/a[(1+1-1)*2 div 2]",
L"/child::root/child::a/ancestor-or-self::a",
L"-1"
};
					
	unsigned int t1 = 0x7fffffff;
	unsigned int t2 = 1;
	Long t3;
	AutoPilot *ap = NULL;
	int ii = 0,i;
	FILE *f = NULL;
	UByte *xml = NULL;
	VTDGen *vg = NULL;
	VTDNav *vn = NULL;
	wchar_t *aa = NULL;
	exception e;
	int a,result;
	expr *expression = NULL;
	double d= 0;
	char* fn = "d:/ximple-dev/testcases/xpath/x21.xml";
	f = fopen(fn,"r");
	ii=(int)_filelength(f->_file);
	xml = (UByte *)malloc(sizeof(UByte)*ii); 
	fread(xml,sizeof(UByte),ii,f);
	t3 = t1+t2;
	printf(" t3 is %d \n",t3);

	//selectXPath(ap,L"/ns1:*/ns1:*/ns1:*");
	Try{
		ap = createAutoPilot2();
		vg = createVTDGen();
		setDoc(vg,xml,ii);
		parse(vg,TRUE);
		vn = getNav(vg);
		declareXPathNameSpace(ap,L"test",L"jimmy");
		//declareXPathNameSpace(ap,L"c",L"larry");
		for(i=1;i<64;i++){
		//selectXPath(ap,L"/a:b/c:d ");
			wprintf(L"i ==> %d \n",i);
			wprintf(L"input test string ==> %ls\n",tests[i]);
			if (selectXPath(ap,tests[i])){

				bind(ap,vn);
				
				wprintf(L"output string ===>");
				printExprString(ap);
				wprintf(L"\n");
				
			} else {
				wprintf(L" XPath parsing failed \n");
			}

		}
		//d = ap->xpe->evalNumber(ap->xpe,NULL);
		//wprintf(L" dval -> %ls\n",ap->xpe->evalString(ap->xpe,NULL));
		wprintf(L" \n\n ************** \n");
		
		//a = getCurrentIndex(vn);
		//while((result=evalXPath(ap))!=-1){
		//	wprintf(L" result is %d \n",result);
		//}
		//if (0==getCurrentIndex(vn)){
		//	wprintf(L"begin matches end \n");
		//}
	}Catch(e){
		wprintf(L"%ls \n",e.msg);
	}

	aa = malloc(sizeof(wchar_t)*16);
	swprintf(aa,L"%d\n",1234);
	wprintf(L"%ls",aa);

	freeVTDGen(vg);
	freeVTDNav(vn);
	freeAutoPilot(ap);
	
///a:b[/a:*/*/descendant::k]/@d
	//expression = xpathParse(L"/ a[/a/b/c] / b[1+2 div 3] / self:: c");
	//expression->toString(expression,NULL);
	//expression->freeExpr(expression);
	
	//d = expression->evalNumber(expression,NULL);
	//yy_scan_string("/a/b/c");
	//if (yyparse()==0){
		//expression = yylval.expression;
	//}
	
	/*Try {
		while(TRUE){
		UCSChar *string = _wcsdup(L"2.0");
		expr *e1 = createNumberExpr(2);
		expr *e2 = createLiteralExpr(string);
		expr *e3 = createBinaryExpr(e1,OP_ADD,e2);
		//wprintf(L"%s",e3->evalString(e3,NULL));
		e3->freeExpr(e3);
		//free(string);
		string = NULL;
		e3 = NULL;
		e2 = NULL;
		e1 = NULL;
		}
	}Catch (e){
	}*/

}
#endif
//struct exception_context the_exception_context[1];
#if 0

void main(){
	unsigned int a = 0x80000001,b1;
	int b = -0x7fffffff, b2;
	int i=0;
    Long ll = (Long) 0xc0000000; 
	Long result;


	FastLongBuffer *flb = createFastLongBuffer(10);
    b2 = (int) b;
	printf(" ll --> %x \n",ll);
	if (a == b)
		printf("a == b \n");
	for(i=0;i<10000;i++){
		appendLong(flb,i);
	}
	clearFastLongBuffer(flb);
	for(i=0;i<10000;i++){
		appendLong(flb,10000);
	}
	if (flb->size!=10000) 
		printf("error !!! \n");

	for(i=0;i<10000;i++){
		if ( longAt(flb,i)!=10000){
			printf("error ----- \n");
		}
	}
}
#endif
#if 0
int main(){

	exception e;
	FILE *f = NULL;
	FILE *fo = NULL;
	int i = 0,j=0;
	
	Long l = 0;
	int len = 0;
	int offset = 0;
	
	char* filename = "c://soap2.xml";

	UByte *xml = NULL; // this is the buffer containing the XML content, UByte means unsigned byte
	VTDGen *vg = NULL; // This is the VTDGen that parses XML
	VTDNav *vn = NULL; // This is the VTDNav that navigates the VTD records
	AutoPilot *ap = NULL;
	UByte *sm = "\n================\n";

	// allocate a piece of buffer then reads in the document content
	// assume "c:\soap2.xml" is the name of the file
	f = fopen(filename,"r");
	fo = fopen("c://out.txt","w");

	i = (int) _filelength(f->_file);
	printf("size of thefile is %d \n",i);
	xml = (UByte *)malloc(sizeof(UByte) *i);
	fread(xml,sizeof(UByte),i,f);
	Try{
		vg = createVTDGen();
		setDoc(vg,xml,i);
		parse(vg,TRUE);
		vn = getNav(vg);
		ap = createAutoPilot2();
		declareXPathNameSpace(ap,L"ns1",L"http://www.w3.org/2003/05/soap-envelope");
		//if (selectXPath(ap,L"//*[namespace-uri()='http://travelcompany.example.org/reservation/travel']")){
		if (selectXPath(ap,L"//*[@*/../@*]")){
			bind(ap,vn);
			wprintf(L"output string ===>");
			printExprString(ap);
			wprintf(L"\n");
			while((j=evalXPath(ap))!= -1){
				UCSChar *temp = NULL;
				temp = toString(vn,j);
				wprintf(L"element name ====> %s \n",temp);
				wprintf(L"element index ===> %d \n",j);
				free(temp);
			}
		}
		if (selectXPath(ap,L"//*/@*/..")){
			bind(ap,vn);
			wprintf(L"output string ===>");
			printExprString(ap);
			wprintf(L"\n");
			while((j=evalXPath(ap))!= -1){
				UCSChar *temp = NULL;
				temp = toString(vn,j);
				wprintf(L"element name ====> %s \n",temp);
				wprintf(L"element index ===> %d \n",j);
				free(temp);
			}
		}
		/*
		if (toElementNS(vn,FC,"http://www.w3.org/2003/05/soap-envelope","Header")) //nav to soap header
		{
			if (toElement(vn,FC))
			{
				do{
					if (hasAttrNS(vn,"http://www.w3.org/2003/05/soap-envelope","mustUnderstand")){
						printf(toString(vn,getCurrentIndex(vn)));

						printf("\n");
						l = getElementFragment(vn);
						offset = (int) l;
						len = (int) (l>>32);
						fwrite((char *)(xml+offset),sizeof(UByte),len,fo);
						fwrite((char *) sm,sizeof(UByte),strlen(sm),fo);
					}
				}
				while(toElement(vn,NS));
			}
			else 
				printf("Header has no child elements \n");
		}
		else
			printf("XML has no header \n");*/
		fclose(f);
		fclose(fo);
		// remember C has no automatic garbage collector
		// needs to deallocate manually.
		freeVTDNav(vn);
		freeVTDGen(vg);
		freeAutoPilot(ap);

	}
	Catch (e) {
		if (e.et == parse_exception)
			printf("parse exception e ==> %s \n %s\n", e.msg, e.sub_msg);	
		// manual garbage collection here
		freeVTDGen(vg);
	}
  return 0;
}
#endif
#if 0 
main(){
		if(1){
	char *a, *b;
	a = b = malloc(10);
	strcpy(a,"good");
	printf("%s \n",a);

	a = realloc(a, 20);

	printf("%s \n",b);

	printf("%s \n",a);
	}
}
#endif


#if 0

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


#include <string.h>
#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "xpath1.h"
#include "helper.h"
#include "vtdGen.h"
#include "indexHandler.h"

struct exception_context the_exception_context[1];

int main(){

	exception e;
	FILE *f = NULL ,*f2 = NULL,*f3 = NULL,*fw = NULL;
	int i = 0,t,result,count=0;
 	wchar_t *tmpString;	
	
	char* filename = "d:/newpo.xml";
	struct stat s;
	UByte *xml = NULL; // this is the buffer containing the XML content, UByte means unsigned byte
	VTDGen *vg = NULL; // This is the VTDGen that parses XML
	VTDNav *vn = NULL; // This is the VTDNav that navigates the VTD records
	AutoPilot *ap = NULL;
	XMLModifier *xm = NULL;  
	char ta[] ="<a> test </a>";
	int tl =  strlen(ta);
	// allocate a piece of buffer then reads in the document content
	// assume "c:\soap2.xml" is the name of the file
	f = fopen(filename,"rb");
	f3 = fopen("d:/soap2.vxl","rb");
	f2 = fopen("d:/newpo2.vxl","wb");
	fw = fopen("d:/newpo_update.xml","wb");
	stat(filename,&s);

	i = (int) s.st_size;	
	wprintf(L"size of the file is %d \n",i);
	xml = (UByte *)malloc(sizeof(UByte) *i);
	i = fread(xml,sizeof(UByte),i,f);
	Try{
		printf("tl ==> %d \n", tl);
		vg = createVTDGen();
		setDoc(vg,ta,tl);
		parse(vg,TRUE);
		vn = getNav(vg);
		if (overWrite(vn,getText(vn),"baddd",1,4)){
			printf(" new XML ==> %s \n", ta);
		}
		setDoc(vg,xml,i);
		parse(vg,TRUE);
		writeIndex_VTDGen(vg,f2);
		loadIndex_VTDGen(vg,f3);
		vn = getNav(vg);
		ap = createAutoPilot2();
		xm = createXMLModifier();
		//if (selectXPath(ap,L"/purchaseOrder/items/item[@partNum='872-AA']")){
		//if (selectXPath(ap,L"/purchaseOrder/shipTo/name/text()[normalize-space()]")){
		if (selectXPath(ap,L"/purchaseOrder/@orderDate[normalize-space()]")){
			bind(ap,vn);
			bind4XMLModifier(xm,vn);
			while((result=evalXPath(ap))!= -1){
				wprintf(L" result !!!! ==> %s \n",toString(vn,result));
				//remove4XMLModifier(xm);
				//insertBeforeElement(xm,L"<something/>");	
			}
		}
		if (selectXPath(ap,L"/purchaseOrder/items/item/USPrice[.<40]/text()")){
			while((result=evalXPath(ap))!= -1){
				updateToken(xm,result,L"200");
			}
		}
		output(xm,fw);
		
		fclose(f);
		fclose(fw);
		// remember C has no automatic garbage collector
		// needs to deallocate manually.
		freeVTDNav(vn);
		freeVTDGen(vg);
		freeXMLModifier(xm);
		freeAutoPilot(ap);
	}
	Catch (e) {
		if (e.et == parse_exception)
			wprintf(L"parse exception e ==> %s \n %s\n", e.msg, e.sub_msg);	
		// manual garbage collection here
		freeVTDGen(vg);
	}
  return 0;
}
#endif