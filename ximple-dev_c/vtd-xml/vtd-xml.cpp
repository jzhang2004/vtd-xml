/* 
 * Copyright (C) 2002-2004 XimpleWare, info@ximpleware.com
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

#include "stdafx.h"
#include "string.h"
struct exception_context the_exception_context[1];
int main()
{   
   int test =7;
   if (test ==1){
	int i,a,k; 
	Long l;
	UCS2Char *string;
	exception e;
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
	string = "this is cool!";
	k = strlen(string);
	printf(" k equals %d", k);

	printf ("\n ------------------ \n");
	string = "*";
	if (strcmp(string, "*")==0)
		printf(" string value of s is * \n");
   }
   if (test ==2){
	   int i,j;
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
        //cb.store(ia);
        for (i = 9; i >= 0; i--) {
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
		   int len = strlen(ca);
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
	   }Catch (e){
		   printf(e.msg);
	   }
   }
   if (test == 7){
	   int i;
	   int a=0x0a0b0c0d;
	   Byte* b = malloc(4);
	   memcpy(b,(&a),4);
	   for (i=0;i<4;i++){
		   printf(" %x \n", *(b+i));
	   }
   }
	return 0;
}

