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
//#include "lex.yy.c"
//#include "l8.tab.c"


struct exception_context the_exception_context[1];

//#define _UNICODE
#if 0


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
		int i,i1;
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

int main(int argc, char *argv[])
{   
   int test =11;
  
   if (test ==1){
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
            //System.out.println(""+ia[0]);
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
	   //FastIntBuffer *fib = createFastIntBuffer();

	   }Catch(e){
		   printf("exception occured \n");
	   }
   }
   // fast long buffer test
   if (test == 4){
	   exception e;
	   Long i;
	   Long *ia;
	   Long j;
	   Try{
			FastLongBuffer *flb = createFastLongBuffer2(10);
			for (i=0L;i<1000000L;i++){
				appendLong(flb,i);
			}
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
				Long j= longAt(flb,i);
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


if (test ==10){
	
	exception e;
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
			printf("XML has no header \n");
		fclose(f);
		fclose(fo);
		// remember C has no automatic garbage collector
		// needs to deallocate manually.
		freeVTDNav(vn);
		freeVTDGen(vg);

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

#if 1
void main(){
	AutoPilot *ap = NULL;
	int ii = 0;
	FILE *f = NULL;
	UByte *xml = NULL;
	VTDGen *vg = NULL;
	VTDNav *vn = NULL;
	exception e;
	int a,result;
	expr *expression = NULL;
	double d= 0;
	char* fn = "d:/ximple-dev/testcases/xpath/x21.xml";
	f = fopen(fn,"r");
	ii=(int)_filelength(f->_file);
	xml = (UByte *)malloc(sizeof(UByte)*ii); 
	fread(xml,sizeof(UByte),ii,f);
	

	//selectXPath(ap,L"/ns1:*/ns1:*/ns1:*");
	Try{
		ap = createAutoPilot2();
		vg = createVTDGen();
		setDoc(vg,xml,ii);
		parse(vg,TRUE);
		vn = getNav(vg);
		declareXPathNameSpace(ap,L"a",L"jimmy");
		declareXPathNameSpace(ap,L"c",L"larry");
		selectXPath(ap,L"/a:b/c:d ");
		setVTDNav(ap,vn);

		wprintf(L" *************** \n");

		printExprString(ap);

		wprintf(L" \n *************** \n");
		//d = ap->xpe->evalNumber(ap->xpe,NULL);
		//wprintf(L" dval -> %ls\n",ap->xpe->evalString(ap->xpe,NULL));
		wprintf(L" \n\n ************** \n");
		a = getCurrentIndex(vn);
		while((result=evalXPath(ap))!=-1){
			wprintf(L" result is %d \n",result);
		}
		if (0==getCurrentIndex(vn)){
			wprintf(L"begin matches end \n");
		}
	}Catch(e){
		wprintf(L"%s\n",e.msg);
	}
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
		UCSChar *string = wcsdup(L"2.0");
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

//struct exception_context the_exception_context[1];#ifdef 0#endif
