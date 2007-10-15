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
#include <stdio.h>
#include "elementFragmentNs.h"

/* Create an ElementFragmentNs object */
ElementFragmentNs* createElementFragmentNs(VTDNav *vn, Long l1, FastIntBuffer *fib, int len){
	ElementFragmentNs *ef = (ElementFragmentNs *)malloc(sizeof(ElementFragmentNs));
	if (ef==NULL) {
		throwException2(out_of_mem,
			"ElementFragmentNs allocation failed ");
		return NULL;
	}
	ef->fib = fib;
	ef->vn = vn;
	ef->stLen = len;
	ef->l = l1;
	return ef;	
}

/* Free an ElementFragmentNs object */
void freeElementFragmentNs(ElementFragmentNs *ef){
	free(ef);
}

/* getSize recomputes the fragment size without byte copying 
   the unit is in # of bytes
*/
int getFragmentSize(ElementFragmentNs *ef){
	//int encoding = vn.encoding;
	int len = (int) (ef->l >> 32);
	int i = 0;
	if (ef->stLen != 0)
		for (i = 0; i < ef->fib->size; i++) {
			int k = intAt(ef->fib,i);
			len += (getTokenLength(ef->vn,k) & 0xffff)
				+ getTokenLength(ef->vn, k + 1) + 4;
		}
		return len;
}

/*  */
Long getOffsetLeng(ElementFragmentNs *ef){
	return ef->l;
}

/*  Return a byte array with namespace compensation
    witht the orginal encoding format */
UByte* toFragmentBytes(ElementFragmentNs *ef){
	UByte  *ba = (UByte *)malloc(getFragmentSize(ef));
	if (ba == NULL){
		throwException2(out_of_mem,"byte array allocation failed in toFragmentBytes");
		return NULL;
	} else {
		int os = (int)ef->l;
		int len = (int)(ef->l>>32);
		int os1 = 0;
		encoding enc;
		int temp,i;
		int tos =0,tlen=0;
		UByte *xml = ef->vn->XMLDoc;
		if (ef->stLen==0){
			//System.arraycopy(xml,os,ba,0,len);
			memcpy(ba,xml,len);
			return ba;
		}
		enc = getEncoding(ef->vn);
		temp = 0;

		switch(enc){
			case FORMAT_UTF_16BE: 
			case FORMAT_UTF_16LE: temp= (ef->stLen+1)<<1; break;
			default:
				temp = ef->stLen+1;
		}            
		//System.arraycopy(xml,os,ba,0,temp);
		memcpy(ba,xml,temp);
		//namespace compensation
		os1 += temp;

		
		for (i = 0; i < ef->fib->size; i++) {
			//System.out.println("i ==>"+fib.intAt(i));
			switch (enc) {
			case FORMAT_UTF_16BE:
				//write a 0 and ws
				//System.arraycopy(ws,0,ba,os1,2);
				memcpy(ba+os1,ws,2);
				os1 += 2;
				tos = getTokenOffset(ef->vn,intAt(ef->fib,i))<<1;
				tlen= (getTokenLength(ef->vn,intAt(ef->fib,i)) & 0xffff)<<1;
				//System.arraycopy(xml,tos,ba,os1,tlen);
				memcpy(ba+os1,xml+tos,tlen);

				os1 +=tlen;
				// write a 0 and =
				// System.arraycopy(ws,2,ba,os1,2);
				memcpy(ba+os1,ws+2,2);
				os1	+= 2;
				tos = (getTokenOffset(ef->vn, intAt(ef->fib,i)+1)-1)<<1;
				tlen= ((getTokenLength(ef->vn, intAt(ef->fib,i)+1) & 0xffff)+2)<<1;
				//System.arraycopy(xml,tos,ba,os1,tlen);
				memcpy(ba+os1,xml+tos,tlen);
				os1 +=tlen;
				break;
			case FORMAT_UTF_16LE:
				// write a ws and 0
				//System.arraycopy(ws,1,ba,os1,2);
				memcpy(ba+os1,ws+1,2);
				os1 += 2;
				tos = getTokenOffset(ef->vn, intAt(ef->fib,i))<<1;
				tlen= (getTokenLength(ef->vn, intAt(ef->fib,i)) & 0xffff)<<1;
				//System.arraycopy(xml,tos,ba,os1,tlen);
				memcpy(ba+os1,xml+tos,tlen);
				os1 +=tlen;
				// 	 write a = and 0
				//System.arraycopy(ws,3,ba,os1,2);
				memcpy(ba+os1,ws+3,2);
				os1	+= 2;
				tos = (getTokenOffset(ef->vn,intAt(ef->fib,i)+1)-1)<<1;
				tlen= ((getTokenLength(ef->vn,intAt(ef->fib,i)+1) & 0xffff)+2)<<1;
				//System.arraycopy(xml,tos,ba,os1,tlen);
				memcpy(ba+os1,xml+tos,tlen);
				os1 +=tlen;
				break;
			default:
				// write a ws
				//System.arraycopy(ws, 1, ba, os1, 1);
				memcpy(ba+os1,ws+1,1);
				os1 ++;
				tos = getTokenOffset(ef->vn, intAt(ef->fib,i));
				tlen = (getTokenLength(ef->vn,intAt(ef->fib,i)) & 0xffff);
				//System.arraycopy(xml, tos, ba, os1, tlen);
				memcpy(ba+os1,xml+tos,tlen);
				os1 +=tlen;
				// 	 write a = 
				//System.arraycopy(ws, 3, ba, os1, 1);
				memcpy(ba+os1,ws+3,1);

				os1 ++;
				tos = getTokenOffset(ef->vn, intAt(ef->fib,i) + 1) - 1 ;
				tlen = (getTokenLength(ef->vn, intAt(ef->fib,i) + 1) & 0xffff) + 2;
				//System.arraycopy(xml, tos, ba, os1, tlen);
				memcpy(ba+os1,xml+tos,tlen);
				os1 +=tlen;
			}
		}
		//System.out.println(new String(ba));
		switch(enc){
			case FORMAT_UTF_16BE: 
			case FORMAT_UTF_16LE: 
				temp= (ef->stLen+1)<<1; break;
			default:
				temp = ef->stLen+1;
		}  
		//System.arraycopy(xml, os + temp, ba, os1, len - temp);
		memcpy(ba+os1,xml+os+temp, len-temp);
		//System.out.println(new String(ba));
		return ba;
	}
}

/* Write ns compensated fragments (bytes in original encoding format) to outputstream */
void writeFragmentToFile(ElementFragmentNs *ef, FILE *f){
	int os = (int)ef->l;
	int len = (int)(ef->l>>32);
	int os1 = 0;
	encoding enc;
	int temp,i;
	size_t temp2;
	int tos =0,tlen=0;
	UByte *xml = ef->vn->XMLDoc;
	if (ef->stLen==0){
		//System.arraycopy(xml,os,ba,0,len);
		//memcpy(ba,xml,sizeof(UByte),len);
		temp2=fwrite(xml,sizeof(UByte),len,f);
		if (temp2<len){
			throwException2(io_exception,"fwrite incomplete in writeToFile");
		}
		return;
	}
	enc = getEncoding(ef->vn);
	temp = 0;

	switch(enc){
			case FORMAT_UTF_16BE: 
			case FORMAT_UTF_16LE: temp= (ef->stLen+1)<<1; break;
			default:
				temp = ef->stLen+1;
	}            
	//System.arraycopy(xml,os,ba,0,temp);
	//memcpy(ba,xml,sizeof(UByte),temp);
	temp2 = fwrite(xml,sizeof(UByte),temp,f); 
	if (temp2<temp){
		throwException2(io_exception,"fwrite incomplete in writeToFile");
	}
	//namespace compensation
	


	for (i = 0; i < ef->fib->size; i++) {
		//System.out.println("i ==>"+fib.intAt(i));
		switch (enc) {
			case FORMAT_UTF_16BE:
				//write a 0 and ws
				//System.arraycopy(ws,0,ba,os1,2);
				//memcpy(ba+os1,ws,sizeof(UByte),2);
				temp2 = fwrite(ws,sizeof(UByte),2,f);
				if (temp2<2){
					throwException2(io_exception,"fwrite incomplete in writeToFile");
				}
				
				tos = getTokenOffset(ef->vn,intAt(ef->fib,i))<<1;
				tlen= (getTokenLength(ef->vn,intAt(ef->fib,i)) & 0xffff)<<1;
				//System.arraycopy(xml,tos,ba,os1,tlen);
				//memcpy(ba+os1,xml+tos,sizeof(UByte),tlen);
				temp2 = fwrite(xml+tos,sizeof(UByte),tlen,f);
				if (temp2<tlen){
					throwException2(io_exception,"fwrite incomplete in writeToFile");
				}
				
				// write a 0 and =
				// System.arraycopy(ws,2,ba,os1,2);
				//memcpy(ba+os1,ws+2,2);
				temp2 = fwrite(ws+2,sizeof(UByte),2,f);
				if (temp2<2){
					throwException2(io_exception,"fwrite incomplete in writeToFile");
				}
				
				tos = (getTokenOffset(ef->vn, intAt(ef->fib,i)+1)-1)<<1;
				tlen= ((getTokenLength(ef->vn, intAt(ef->fib,i)+1) & 0xffff)+2)<<1;
				//System.arraycopy(xml,tos,ba,os1,tlen);
				//memcpy(ba+os1,xml+tos,tlen);
				temp2 = fwrite(xml+tos,sizeof(UByte),tlen,f);
				if (temp2<tlen){
					throwException2(io_exception,"fwrite incomplete in writeToFile");
				}
				
				break;
			case FORMAT_UTF_16LE:
				// write a ws and 0
				//System.arraycopy(ws,1,ba,os1,2);
				//memcpy(ba+os1,ws+1,2);
				temp2 = fwrite(ws+1,sizeof(UByte),2,f);
				if (temp2<tlen){
					throwException2(io_exception,"fwrite incomplete in writeToFile");
				}
				
				tos = getTokenOffset(ef->vn, intAt(ef->fib,i))<<1;
				tlen= (getTokenLength(ef->vn, intAt(ef->fib,i)) & 0xffff)<<1;
				//System.arraycopy(xml,tos,ba,os1,tlen);
				//memcpy(ba+os1,xml+tos,tlen);
				temp2 = fwrite(xml+tos,sizeof(UByte),tlen,f);
				if (temp2<tlen){
					throwException2(io_exception,"fwrite incomplete in writeToFile");
				}
				
				// 	 write a = and 0
				//System.arraycopy(ws,3,ba,os1,2);
				//memcpy(ba+os1,ws+3,2);
				temp2 = fwrite(ws+3,sizeof(UByte),2,f);
				if (temp2<2){
					throwException2(io_exception,"fwrite incomplete in writeToFile");
				}
				
				tos = (getTokenOffset(ef->vn,intAt(ef->fib,i)+1)-1)<<1;
				tlen= ((getTokenLength(ef->vn,intAt(ef->fib,i)+1) & 0xffff)+2)<<1;
				//System.arraycopy(xml,tos,ba,os1,tlen);
				//memcpy(ba+os1,xml+tos,tlen);
				temp2 = fwrite(xml+tos,sizeof(UByte),tlen,f);
				if (temp2<tlen){
					throwException2(io_exception,"fwrite incomplete in writeToFile");
				}
				
				break;
			default:
				// write a ws
				//System.arraycopy(ws, 1, ba, os1, 1);
				//memcpy(ba+os1,ws+1,1);
				temp2 = fwrite(ws+1,sizeof(UByte),1,f);
				if (temp2<1){
					throwException2(io_exception,"fwrite incomplete in writeToFile");
				}
				
				tos = getTokenOffset(ef->vn, intAt(ef->fib,i));
				tlen = (getTokenLength(ef->vn,intAt(ef->fib,i)) & 0xffff);
				//System.arraycopy(xml, tos, ba, os1, tlen);
				//memcpy(ba+os1,xml+tos,tlen);
				temp2 = fwrite(xml+tos,sizeof(UByte),tlen,f);
				if (temp2<tlen){
					throwException2(io_exception,"fwrite incomplete in writeToFile");
				}
				
				// 	 write a = 
				//System.arraycopy(ws, 3, ba, os1, 1);
				//memcpy(ba+os1,ws+3,1);
				temp2 = fwrite(ws+3,sizeof(UByte),1,f);
				if (temp2<1){
					throwException2(io_exception,"fwrite incomplete in writeToFile");
				}
				
				tos = getTokenOffset(ef->vn, intAt(ef->fib,i) + 1) - 1 ;
				tlen = (getTokenLength(ef->vn, intAt(ef->fib,i) + 1) & 0xffff) + 2;
				//System.arraycopy(xml, tos, ba, os1, tlen);
				//memcpy(ba+os1,xml+tos,tlen);
				temp2 = fwrite(xml+tos,sizeof(UByte),tlen,f);
				if (temp2<tlen){
					throwException2(io_exception,"fwrite incomplete in writeToFile");
				}
				
		}
	}
	//System.out.println(new String(ba));
	switch(enc){
			case FORMAT_UTF_16BE: 
			case FORMAT_UTF_16LE: 
				temp= (ef->stLen+1)<<1; break;
			default:
				temp = ef->stLen+1;
	}  
	//System.arraycopy(xml, os + temp, ba, os1, len - temp);
	//memcpy(ba+os1,xml+os+temp, len-temp);
	temp2 = fwrite(xml+os+temp,sizeof(UByte),len-temp,f);
	if (temp2<len-temp){
		throwException2(io_exception,"fwrite incomplete in writeToFile");
	}
}