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
// vtd-xml.cpp : Defines the entry point for the console application.
//

#include "everything.h"
#include <iostream>

/*#include <mfc\afx.h>
#include <crtdbg.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE static char THIS_FILE[] = __FILE__;
#endif*/

using namespace com_ximpleware;
//1
int main1(){
	VTDNav *vn = NULL;	
	try{
		VTDGen vg;
		if (vg.parseFile(true,"c:/xml/input.xml")){
			vn = vg.getNav();
			if (vn->toElementNS(FIRST_CHILD,L"someURL",L"b")){
				int i = vn->getText();
				if (i!=-1){
					UCSChar *string = vn->toString(i);
					wprintf(L"the text node value is %d ==> %s \n",i,string);
					delete(string);
				}
			}
			delete vn->getXML();
		} 
		else
			delete (vg.getXML());
		
	}catch(...){// handle various types of exceptions here
	}
	delete(vn);
	return 0;
}

int main2(){
	VTDNav *vn = NULL;
	try{
		AutoPilot ap;
		ap.selectXPath(L"/a/b/text()");
		VTDGen vg;
		if (vg.parseFile(false,"input.xml")){
			vn = vg.getNav();
			ap.bind(vn);
			int i;
			while((i=ap.evalXPath())!=-1){
				UCSChar *string = vn->toString(i);
				wprintf(L"the text node value is %d ==> %s \n",i,string);
				delete(string);
			}	
			delete vn->getXML();	
		} else 
			delete vg.getXML();
		
	}catch(...){// handle various types of exceptions here
	}
	delete(vn);
	return 0;
}

int main3(){
	VTDNav *vn = NULL;
	UCSChar *string = NULL;    
	try{
		AutoPilot ap;
		ap.declareXPathNameSpace(L"ns1",L"someURL");
		ap.selectXPath(L"/ns1:a/ns1:b/text()");
		VTDGen vg;
		if (vg.parseFile(true,"input.xml")){
			vn = vg.getNav();
			ap.bind(vn);
			int i;
			while((i=ap.evalXPath())!=-1){
				string = vn->toString(i);
				wprintf(L"the text node value is %d ==> %s \n",i,string);
				delete(string);
			}	
			delete(vn->getXML());		
		} 
		else
			delete (vg.getXML());
		
	}catch(...){// handle various types of exceptions here
	}
	delete (vn);
	return 0;
}

int main4(){
    try{
		VTDGen vg;
		if (vg.parseFile(true,"input.xml")){
			vg.writeIndex("input.vxl");
		}
		delete(vg.getXML());
	}catch(...){
	}
	return 0;
}

int main5(){
	VTDNav *vn = NULL;
	FILE *f = NULL;
	UCSChar *string = NULL; 
	int i;
   	f = fopen("d:/ximpleware_2.2_c/vtd-xml/codeGuru/5/input.vxl","rb");
	if (f==NULL)
		return 0;
	try{
		AutoPilot ap;
		ap.selectXPath(L"/a/b/text()");
		VTDGen vg;		
		vn = vg.loadIndex(f);
		ap.bind(vn);
		while((i=ap.evalXPath())!=-1){
			UCSChar *string = vn->toString(i);
			wprintf(L"the text node value is %d ==> %s \n",i,string);
			delete(string);
		}
		delete(vn->getXML());		
	}catch(...){// handle various types of exceptions here
	}
	fclose(f);		
	delete(vn);
	return 0;
}

int main6(){
	VTDNav *vn = NULL;
	FILE *f = NULL;
	UCSChar *string = NULL; 
	int i;
    f = fopen("d:/ximpleware_2.2_c/vtd-xml/codeGuru/6/input.vxl","rb");
	if (f==NULL)
		return 0;
	try{
		XMLModifier xm;
		AutoPilot ap;
		ap.selectXPath(L"/a/b");
		VTDGen vg;		
		vn = vg.loadIndex(f);
		ap.bind(vn);
		xm.bind(vn);
		while((i=ap.evalXPath())!=-1){
			xm.insertAttribute(L" attr1='val'");
		}
		xm.output("new.xml");
		delete(vn->getXML());		
	}catch(...){// handle various types of exceptions here
	}
	fclose(f);		
	
	delete (vn);
	return 0;
}

int main7(){
	VTDNav *vn = NULL;
	FILE *f = NULL;
	int i;
	try{
		XMLModifier xm;
		AutoPilot ap;
		ap.selectXPath(L"/root/b");
		VTDGen vg;
		if (vg.parseFile(false,"input.xml")){
			vn = vg.getNav();
			ap.bind(vn);
			xm.bind(vn);
			while((i=ap.evalXPath())!=-1){
				xm.remove();
			}
			xm.output("new.xml");
			delete(vn->getXML());
		}
		else
			delete(vg.getXML());
	}catch(...){// handle various types of exceptions here
	}
	
	delete(vn);
	return 0;
}

int main8(){
	VTDNav *vn = NULL;
	FILE *f = NULL;

    
	try{
		XMLModifier xm;
		AutoPilot ap;
		ap.selectXPath(L"/root/b");
		VTDGen vg;
		if (vg.parseFile(false,"d:/ximpleware_2.2_c/vtd-xml/codeGuru/8/input.xml")){
			vn = vg.getNav();
			ap.bind(vn);
			xm.bind(vn);
			int i;
			while((i=ap.evalXPath())!=-1){
				xm.insertBeforeElement("<lala/>\n",8);
				xm.insertAfterElement("\n<lala/>",8);
			}
			xm.output("new.xml");
			delete(vn->getXML());
		}
		else
			delete(vg.getXML());
	}catch(...){// handle various types of exceptions here
	}
	delete (vn);
	return 0;
}


int main9(){
	
	VTDNav *vn = NULL, *vn2=NULL; // This is the VTDNav that navigates the VTD records
	//AutoPilot *ap = NULL;
	UByte *ba1 = NULL, *ba2=NULL;
	Long l;
	try{
		VTDGen vg;
		// test2.xml is encoded in UTF-16LE
		if (vg.parseFile(false,"test2.xml")==false){
			// parsing failed
			delete(vg.getXML());
			return 0; 
		}
		//parse(vg,TRUE);
		vn = vg.getNav();	
		ba1 =vn->getXML();
		l = vn->getElementFragment();

		if (vg.parseFile(false,"test.xml")==false){
			// parsing failed
			delete(ba1);
			delete(vn);
			delete(vn->getXML());			
			return 0;
		}
		vn2 = vg.getNav();
		XMLModifier xm(vn2);
		vn2->toElement(FIRST_CHILD);
		xm.insertAfterElement(FORMAT_UTF_16LE,ba1,(int)l,(int)(l>>32));
		xm.insertBeforeElement(FORMAT_UTF_16LE,ba1,(int)l,(int)(l>>32));
		xm.output("output.xml");
		delete(vn2->getXML());
		delete (ba1);
		delete (vn2);
		delete (vn);
	}
	catch (...) {
		
	}
	return 0;
}

int main10(){
	VTDNav *vn = NULL; // This is the VTDNav that navigates the VTD record
	UByte *ba = NULL;
	int result;
	try{
		VTDGen vg;
		if (vg.parseFile(false,"test.xml")){
			//parse(vg,TRUE);
			vn = vg.getNav();
			AutoPilot ap;
			XMLModifier xm;
			if (ap.selectXPath(L"//*")){
				ap.bind(vn);
				xm.bind(vn);
				while((result=ap.evalXPath())!= -1){
					xm.updateElementName(L"lalala");
				}
				xm.output("out.xml");
			}
			ba =vn->getXML();
			delete (vn);			
		} else {
			ba =vg.getXML();
		}		
		delete (ba);		
	}
	catch (...) {
	}
	return 0;
}

int main11(){
	VTDNav *vn1 = NULL, *vn2=NULL;
	ElementFragmentNs *efn = NULL;
	int i;
	try{
		XMLModifier xm;
		AutoPilot ap;
		ap.selectXPath(L"/root/b");
		VTDGen vg1;
		if (vg1.parseFile(true,"input2.xml")==false){
			delete(vg1.getXML());
			return 0;
		}
		VTDGen vg2;
		if (vg2.parseFile(true,"input1.xml")==false){
			delete(vg2.getXML());
			delete(vg1.getXML());
			return 0;
		}
		vn1 = vg1.getNav();
		if (vn1->toElementNS(FIRST_CHILD,L"someURL2",L"b")){
			efn =vn1->getElementFragmentNs();
			vn2 = vg2.getNav();
			ap.bind(vn2);
			xm.bind(vn2);
			while((i=ap.evalXPath())!=-1){
				xm.insertBeforeElement(efn);
				xm.insertAfterElement(efn);
			}
			xm.output("output.xml");
		}
		delete(vn1->getXML());
		delete(vn2->getXML());	
	}catch(...){// handle various types of exceptions here
	}
	delete(efn);	
	delete(vn1);
	delete(vn2);
	return 0;
}

int main12(){
	VTDNav *vn = NULL, *vn2=NULL; // This is the VTDNav that navigates the VTD records
	UByte *ba1 = NULL, *ba2=NULL;
	ElementFragmentNs *efs = NULL;
	//Long l;
	try{
		VTDGen vg;
		// test2.xml is encoded in UTF-16LE
		if (vg.parseFile(true,"test2.xml")==false){
			// parsing failed
			delete(vg.getXML());
			return 1;
		}
		//parse(vg,TRUE);
		vn = vg.getNav();	
		ba1 =vn->getXML();
		efs = vn->getElementFragmentNs();

		if (vg.parseFile(true,"test.xml")==false){
			// parsing failed
			delete(ba1);
			delete(vn);
			delete(vg.getXML());
			return 0;
		}
		vn2 = vg.getNav();
		XMLModifier xm(vn2);
		vn2->toElement( FIRST_CHILD);
		xm.insertAfterElement(efs);
		xm.insertBeforeElement(efs);
		xm.output("output.xml");
		
		delete(vn2->getXML());
		free(ba1);
		delete(vn2);
		delete(efs);
		free(vn);
	}
	catch (...) {
	}
	return 0;
}

int main13(){
	VTDNav *vn = NULL;
	FILE *f = NULL;
	UCSChar *string = NULL; 
	int i;
	Long l;    
	try{
		AutoPilot ap1 ; 
		AutoPilot ap2; 
		AutoPilot ap3;
		ap1.selectXPath(L"/root/a");
		ap2.selectXPath(L"/root/b");
		ap3.selectXPath(L"/root/c");
		f = fopen("output.xml","wb");
		VTDGen vg;		
		if (vg.parseFile(true,"input.xml")==false){
			delete(vg.getXML());
			return 0;
		}		
		vn = vg.getNav();
		ap1.bind(vn); 
		ap2.bind(vn);
		ap3.bind(vn);
		fwrite("<root>",1,6,f);
		while((i=ap1.evalXPath())!=-1){
			l = vn->getElementFragment();
			fwrite("\n",1,1,f);
			fwrite(vn->getXML()+(int)l,1,(int)(l>>32),f);
		}
		while((i=ap2.evalXPath())!=-1){
			l = vn->getElementFragment();
			fwrite("\n",1,1,f);
			fwrite(vn->getXML()+(int)l,1,(int)(l>>32),f);
		}
		while((i=ap3.evalXPath())!=-1){
			l = vn->getElementFragment();
			fwrite("\n",1,1,f);
			fwrite(vn->getXML()+(int)l,1,(int)(l>>32),f);
		}
		fwrite("\n</root>",1,8,f);		
		free(vn->getXML());
	}catch(...){// handle various types of exceptions here
	}
	delete (vn);
	return 0;
}

void fillCD(VTDNav *vn, char *title, 
			char *artist, char *country, 
			char *company, float price, int year){
				int i = -1;
				char a[10],b[10];
				sprintf(a,"%4.2f",price);
				sprintf(b,"%d",year);
				
				if (vn->toElement(FIRST_CHILD))
				{
					vn->overWrite(vn->getText(), title, 0, strlen(title)); // the assumption --getText won't return 1
					vn->toElement(NEXT_SIBLING);
					vn->overWrite(vn->getText(), artist,0, strlen(artist));
					vn->toElement(NEXT_SIBLING);
					vn->overWrite(vn->getText(), country,0, strlen(country));
					vn->toElement(NEXT_SIBLING);
					vn->overWrite(vn->getText(), company, 0, strlen(company));
					vn->toElement(NEXT_SIBLING);
					vn->overWrite(vn->getText(), a, 0, strlen((char *)a));
					vn->toElement(NEXT_SIBLING);
					vn->overWrite(vn->getText(), b, 0,strlen((char *)b));
				}
				vn->toElement(PARENT);
}

int main15(){
	
	VTDGen *vg = NULL;
	VTDNav *vn = NULL;
	AutoPilot *ap1 = NULL;
	FILE *f = NULL;
	UCSChar *string = NULL;

	int i;

	try{			
		VTDGen vg;		
		if (vg.parseFile(true,"cd_template.xml")==false){
			delete(vg.getXML());
			return 0;
		}	
		AutoPilot ap1;
		ap1.selectXPath(L"/CATALOG/CD");
		vn = vg.getNav();
		ap1.bind(vn);
		if((i=ap1.evalXPath())!=-1){
			fillCD(vn,"Empire Burlesque", "Bob Dylan", "USA", "Columbia", 10.90, 1985);		
		}	
		if((i=ap1.evalXPath())!=-1){
			fillCD(vn,"Still Got the Blues", "Gary More", "UK", "Virgin Records", 10.20, 1990);		
		}
		vn->dumpXML("new_cd.xml");
		delete(vn->getXML());
	}catch(...){// handle various types of exceptions here
	}
	
	delete(vn);
	return 0;
}

/*int main15(){

	return 0;
}*/

int main16(){

	VTDNav *vn = NULL;
	FILE *f = NULL;
	int i;
	try{			
		VTDGen vg;		
		if (vg.parseFile(true,"old_cd.xml")==false){
			delete(vg.getXML());
			return 0;
		}	
		AutoPilot ap1;
		ap1.selectXPath(L"/CATALOG/CD[PRICE=10.2]/*/text()");
		vn = vg.getNav();
		ap1.bind(vn);
		while((i=ap1.evalXPath())!=-1){
			vn->overWrite(i,"",0,0);		
		}
		vn->dumpXML("new_cd.xml");
		delete(vn->getXML());
	}catch(...){// handle various types of exceptions here
	}	
	delete(vn);
	return 0;
}

UCSChar *ba0,*ba1,*ba2,*ba3,*ba4,*ba5;
void convert(VTDNav *vn, XMLModifier& xm){
	
	int i = -1;
	vn->toElement(FIRST_CHILD);
	i = vn->getText();
	xm.updateToken(i,ba0);

	vn->toElement(NEXT_SIBLING);
	i = vn->getText();
	xm.updateToken(i,ba1);

	vn->toElement(NEXT_SIBLING);
	i = vn->getText();
	xm.updateToken(i,ba2);

	vn->toElement(NEXT_SIBLING);
	i = vn->getText();
	xm.updateToken(i,ba3);

	vn->toElement(NEXT_SIBLING);
	i = vn->getText();
	xm.updateToken(i,ba4);

	vn->toElement(NEXT_SIBLING);
	i = vn->getText();
	xm.updateToken(i,ba5);

	vn->toElement(PARENT);
}

int main14(){
	VTDNav *vn = NULL;

	int i;
	Long l;   
	ba0=L"                      ";
	ba1=L"              ";
	ba2=L"       ";
	ba3=L"              ";
	ba4=L"        ";
	ba5=L"        ";
	try{			
		VTDGen vg;		
		if (vg.parseFile(true,"old_cd.xml")==false){
			delete(vg.getXML());
			return 0;
		}	
		XMLModifier xm;
		AutoPilot ap1 ;
		ap1.selectXPath(L"/CATALOG/CD");
		vn = vg.getNav();
		ap1.bind(vn);
		xm.bind(vn);		
		while((i=ap1.evalXPath())!=-1){
			convert(vn,xm);		
		}	
		xm.output("cd_template.xml");
		delete(vn->getXML());
	}catch(...){// handle various types of exceptions here
	}
	delete(vn);
	return 0;
}



int main17(){
	VTDGen *vg = NULL;
	VTDNav *vn = NULL;
	AutoPilot *ap1 = NULL;
	FILE *f = NULL;
	UCSChar *string = NULL;

	try{
		printf("size of int is %d \n",sizeof(int));
		VTDGen vg;
		if (vg.parseFile(false,"c:/xml/x22.xml")){
			vn = vg.getNav();
			XMLModifier xm(vn);
			xm.insertAfterHead(L"good");
			xm.output("c:/xml/x22222.xml");
			delete vn;
		}

		if (vg.parseFile(false,"c:/xml/x11.xml")){
			vn = vg.getNav();
			XMLModifier xm(vn);
			xm.insertAfterHead(L"good");
			xm.output("c:/xml/x11111.xml");
			delete vn;
		}
	}catch(...){
		printf("exception occurred\n");
	}
	return 0;
}


bool test2(string s, int d){
	VTDNav *vn = NULL;
	VTDGen vg ;
	try{
		//VTDGen *vg = new VTDGen();
		vg.selectLcDepth(d);
		if (vg.parseFile(true, s.c_str()) == false)
			return false;
		vn =vg.getNav();
		int i= vn->parseInt(vn->getAttrVal(L"attr"));
		int i1 = 0;
		if(vn->toElement(VTDNav::FC)){ // to level 1
			//i1++;
			do {
				if (!vn->matchElementNS(L"level1",L"level1")){
					if (vn!=NULL){
						delete vn->getXML();
						delete vn;
					}
					return false;
				}
				if (vn->matchElementNS(L"top",L"level1")){
					if (vn!=NULL){
						delete vn->getXML();
						delete vn;
					}
					return false;
				}
				int j = vn->parseInt(vn->getAttrVal(L"attr"));
				int j1 = 0;
				if (vn->toElement(VTDNav::FC)){ // to level 2
					//j1++;
					do {
						if (!vn->matchElementNS(L"level2",L"level2")){
							if (vn!=NULL){
								delete vn->getXML();
								delete vn;
							}
							return false;
						}
						if (vn->matchElementNS(L"top",L"level2")){
							if (vn!=NULL){
								delete vn->getXML();
								delete vn;
							}
							return false;
						}
						int k = vn->parseInt(vn->getAttrVal(L"attr"));
						int k1 = 0;
						if (vn->toElement(VTDNav::FC)){ // level 3
							//k1++;
							do{
								if (!vn->matchElementNS(L"level3",L"level3")){
									if (vn!=NULL){
										delete vn->getXML();
										delete vn;
									}
									return false;
								}
								if (vn->matchElementNS(L"top",L"level3")){
									if (vn!=NULL){
										delete vn->getXML();
										delete vn;
									}
									return false;
								}
								int l = vn->parseInt(vn->getAttrVal(L"attr"));
								int l1 = 0;
								if (vn->toElement(VTDNav::FC)){ // level 4
									//l1++;
									do {
										if (!vn->matchElementNS(L"level4",L"level4")){
											if (vn!=NULL){
												delete vn->getXML();
												delete vn;
											}
											return false;
										}
										if (vn->matchElementNS(L"top",L"level4")){
											if (vn!=NULL){
												delete vn->getXML();
												delete vn;
											}
											return false;
										}
										int m = vn->parseInt(vn->getAttrVal(L"attr"));
										int m1 = 0;
										if (vn->toElement(VTDNav::FC)){ // level 5
											//m1++;
											do {
												if (!vn->matchElementNS(L"level5",L"level5")){
													if (vn!=NULL){
														delete vn->getXML();
														delete vn;
													}
													return false;
												}
												if (vn->matchElementNS(L"top",L"level5")){
													if (vn!=NULL){
														delete vn->getXML();
														delete vn;
													}
													return false;
												}
												int n = vn->parseInt(vn->getAttrVal(L"attr"));
												int n1 = 0;
												if (vn->toElement(VTDNav::FC)){ // level 5
													//m1++;
													do {
														if (!vn->matchElementNS(L"level6",L"level6"))
															return false;
														if (vn->matchElementNS(L"top",L"level6"))
															return false;
														int o = vn->parseInt(vn->getAttrVal(L"attr"));
														int o1 = 0;
														if (vn->toElement(VTDNav::FC)){ // level 5
															do{
																o1++;
															}while(vn->toElement(VTDNav::NS));
															vn->toElement(VTDNav::P);
															if (o1!=0){
																if (vn!=NULL){
																	delete vn->getXML();
																	delete vn;
																}
																return false;
															}
															if (o1!=o){
																if (vn!=NULL){
																	delete vn->getXML();
																	delete vn;
																}
																return false;
															}
														}
														n1++;
													}while(vn->toElement(VTDNav::NS));
													vn->toElement(VTDNav::P);
												}
												if (n!=n1){
													if (vn!=NULL){
														delete vn->getXML();
														delete vn;
													}
													return false;
												}
												/*if (m!=0)
												return false;*/
												m1++;
											}while(vn->toElement(VTDNav::NS));
											vn->toElement(VTDNav::P);
										}
										if (m!=m1){
											if (vn!=NULL){
												delete vn->getXML();
												delete vn;
											}
											return false;
										}
										/*if (m!=0)
										return false;*/
										l1++;
									}while(vn->toElement(VTDNav::NS));
									vn->toElement(VTDNav::P);
								}
								if (l!=l1){
									if (vn!=NULL){
										delete vn->getXML();
										delete vn;
									}
									return false;
								}
								k1++;
							}while(vn->toElement(VTDNav::NS));
							vn->toElement(VTDNav::P);
						}
						if (k1 != k){
							if (vn!=NULL){
								delete vn->getXML();
								delete vn;
							}							
							return false;
						}
						j1++;
					}while(vn->toElement(VTDNav::NS));
					vn->toElement(VTDNav::P);
				}
				if (j1 != j){
					if (vn!=NULL){
						delete vn->getXML();
						delete vn;
					}
					return false;
				}
				i1++;
			}while(vn->toElement(VTDNav::NS));				
		}
		if (i1!=i){
			if (vn!=NULL){
				delete vn->getXML();
				delete vn;
			}
			return false;
		}
		vn->toElement(VTDNav::R);

			i= vn->parseInt(vn->getAttrVal(L"attr"));
			i1 = 0;
			if(vn->toElement(VTDNav::LC)){ // to level 1
				//i1++;
				do {
					if (!vn->matchElementNS(L"level1",L"level1")){
						if (vn!=NULL){
							delete vn->getXML();
							delete vn;
						}
						return false;
					}
					if (vn->matchElementNS(L"top",L"level1")){
						if (vn!=NULL){
							delete vn->getXML();
							delete vn;
						}
						return false;
					}
					int j = vn->parseInt(vn->getAttrVal(L"attr"));
					int j1 = 0;
					if (vn->toElement(VTDNav::LC)){ // to level 2
						//j1++;
						do {
							if (!vn->matchElementNS(L"level2",L"level2")){
								if (vn!=NULL){
									delete vn->getXML();
									delete vn;
								}
								return false;
							}
							if (vn->matchElementNS(L"top",L"level2")){
								if (vn!=NULL){
									delete vn->getXML();
									delete vn;
								}
								return false;
							}
							int k = vn->parseInt(vn->getAttrVal(L"attr"));
							int k1 = 0;
							if (vn->toElement(VTDNav::LC)){ // level 3
								//k1++;
								do{
									if (!vn->matchElementNS(L"level3",L"level3")){
										if (vn!=NULL){
											delete vn->getXML();
											delete vn;
										}
										return false;
									}
									if (vn->matchElementNS(L"top",L"level3")){
										if (vn!=NULL){
											delete vn->getXML();
											delete vn;
										}
										return false;
									}
									int l = vn->parseInt(vn->getAttrVal(L"attr"));
									int l1 = 0;
									if (vn->toElement(VTDNav::LC)){ // level 4
										//l1++;
										do {	
											if (!vn->matchElementNS(L"level4",L"level4")){
												if (vn!=NULL){
													delete vn->getXML();
													delete vn;
												}
												return false;
											}
											if (vn->matchElementNS(L"top",L"level4")){
												if (vn!=NULL){
													delete vn->getXML();
													delete vn;
												}
												return false;
											}
											int m = vn->parseInt(vn->getAttrVal(L"attr"));
											int m1 = 0;
											if (vn->toElement(VTDNav::LC)){ // level 5
												//m1++;
												do {
													if (!vn->matchElementNS(L"level5",L"level5")){
														if (vn!=NULL){
															delete vn->getXML();
															delete vn;
														}
														return false;
													}
													if (vn->matchElementNS(L"top",L"level5")){
														if (vn!=NULL){
															delete vn->getXML();
															delete vn;
														}													
														return false;
													}
													int n = vn->parseInt(vn->getAttrVal(L"attr"));
													int n1 = 0;
													if (vn->toElement(VTDNav::LC)){ // level 5
														//m1++;
														do {
															if (!vn->matchElementNS(L"level6",L"level6")){
																if (vn!=NULL){
																	delete vn->getXML();
																	delete vn;
																}
																return false;
															}
															if (vn->matchElementNS(L"top",L"level6")){
																if (vn!=NULL){
																	delete vn->getXML();
																	delete vn;
																}
																return false;
															}
															int o = vn->parseInt(vn->getAttrVal(L"attr"));
															int o1 = 0;
															if (vn->toElement(VTDNav::LC)){ // level 5
																do{
																	o1++;
																}while(vn->toElement(VTDNav::PS));
																vn->toElement(VTDNav::P);
																if (o1!=0){
																	if (vn!=NULL){
																		delete vn->getXML();
																		delete vn;
																	}
																	return false;
																}
																if (o1!=o){
																	if (vn!=NULL){
																		delete vn->getXML();
																		delete vn;
																	}
																	return false;
																}
															}
															n1++;
														}while(vn->toElement(VTDNav::PS));
														vn->toElement(VTDNav::P);
													}
													if (n!=n1){
														if (vn!=NULL){
															delete vn->getXML();
															delete vn;
														}
														return false;
													}
													m1++;
												}while(vn->toElement(VTDNav::PS));
												vn->toElement(VTDNav::P);
											}
											if (m!=m1){
												if (vn!=NULL){
													delete vn->getXML();
													delete vn;
												}
												return false;
											}
											/*if (m!=0)
											return false;*/
											l1++;
										}while(vn->toElement(VTDNav::PS));
										vn->toElement(VTDNav::P);
									}
									if (l!=l1){
										if (vn!=NULL){
											delete vn->getXML();
											delete vn;
										}
										return false;
									}
									k1++;
								}while(vn->toElement(VTDNav::PS));
								vn->toElement(VTDNav::P);
							}
							if (k1 != k){
								if (vn!=NULL){
									delete vn->getXML();
									delete vn;
								}
								return false;
							}
							j1++;
						}while(vn->toElement(VTDNav::PS));
						vn->toElement(VTDNav::P);
					}
					if (j1 != j){
						if (vn!=NULL){
							delete vn->getXML();
							delete vn;
						}
						return false;
					}
					i1++;
				}while(vn->toElement(VTDNav::PS));				
			}
			if (i1!=i){
				if (vn!=NULL){
					delete vn->getXML();
					delete vn;
				}
				return false;
			}
			if (vn!=NULL){
				delete vn->getXML();
				delete vn;
			}
			return true;
			//System.out.println("vtdgen ok");
		}
		catch (...){
			//e.printStackTrace();
			//Console.WriteLine("other exceptions");
			//return false;
		}

		//delete vg;
		if (vn!=NULL){
			delete vn->getXML();
			delete vn;
		}
		return false;
		//return true;
	}
bool testIndex2(string s, int d){
		 VTDNav *vn = NULL;
		 try{
			 VTDGen vg ;//= new VTDGen();
			 vg.selectLcDepth(d);
			 if (vg.parseFile( true, s.c_str()) == false)
				 return false;
			 vg.writeIndex("tmp.vxl");
			 delete vg.getXML();
			 //delete vg;
			 VTDGen vg2;
			 VTDNav *vn = vg2.loadIndex("tmp.vxl");
			 int i= vn->parseInt(vn->getAttrVal(L"attr"));
			 int i1 = 0;
			 if(vn->toElement(VTDNav::FC)){ // to level 1
				 //i1++;
				 do {
					 if (!vn->matchElementNS(L"level1",L"level1")){
						 if (vn!=NULL){
							 delete vn->getXML();
							 delete vn;
						 }
						 return false;
					 }
					 if (vn->matchElementNS(L"top",L"level1")){
						 if (vn!=NULL){
							 delete vn->getXML();
							 delete vn;
						 }
						 return false;
					 }
					 int j = vn->parseInt(vn->getAttrVal(L"attr"));
					 int j1 = 0;
					 if (vn->toElement(VTDNav::FC)){ // to level 2
						 //j1++;
							do {
								if (!vn->matchElementNS(L"level2",L"level2")){
									if (vn!=NULL){
					delete vn->getXML();
					delete vn;
				}
									return false;
								}
								if (vn->matchElementNS(L"top",L"level2")){
									if (vn!=NULL){
					delete vn->getXML();
					delete vn;
				}
									return false;
								}
								int k = vn->parseInt(vn->getAttrVal(L"attr"));
								int k1 = 0;
								if (vn->toElement(VTDNav::FC)){ // level 3
									//k1++;
									do{
										if (!vn->matchElementNS(L"level3",L"level3")){
											if (vn!=NULL){
												delete vn->getXML();
												delete vn;
											}
											return false;
										}
										if (vn->matchElementNS(L"top",L"level3")){
											if (vn!=NULL){
												delete vn->getXML();
												delete vn;
											}
											return false;
										}
										int l = vn->parseInt(vn->getAttrVal(L"attr"));
										int l1 = 0;
										if (vn->toElement(VTDNav::FC)){ // level 4
											//l1++;
											do {
												if (!vn->matchElementNS(L"level4",L"level4")){
													if (vn!=NULL){
														delete vn->getXML();
														delete vn;
													}
													return false;
												}
												if (vn->matchElementNS(L"top",L"level4")){
													if (vn!=NULL){
														delete vn->getXML();
														delete vn;
													}
													return false;
												}
												int m = vn->parseInt(vn->getAttrVal(L"attr"));
												int m1 = 0;
												if (vn->toElement(VTDNav::FC)){ // level 5
													//m1++;
													do {
														if (!vn->matchElementNS(L"level5",L"level5")){
															if (vn!=NULL){
																delete vn->getXML();
																delete vn;
															}
															return false;
														}
														if (vn->matchElementNS(L"top",L"level5")){
															if (vn!=NULL){
																delete vn->getXML();
																delete vn;
															}
															return false;
														}
														int n = vn->parseInt(vn->getAttrVal(L"attr"));
														int n1 = 0;
														if (vn->toElement(VTDNav::FC)){ // level 5
															//m1++;
															do {
																if (!vn->matchElementNS(L"level6",L"level6")){
																	if (vn!=NULL){
																		delete vn->getXML();
																		delete vn;
																	}
																	return false;
																}
																if (vn->matchElementNS(L"top",L"level6")){
																	if (vn!=NULL){
																		delete vn->getXML();
																		delete vn;
																	}
																	return false;
																}
																int o = vn->parseInt(vn->getAttrVal(L"attr"));
																int o1 = 0;
																if (vn->toElement(VTDNav::FC)){ // level 5
																	do{
																		o1++;
																	}while(vn->toElement(VTDNav::NS));
																	vn->toElement(VTDNav::P);
																	if (o1!=0){
																		if (vn!=NULL){
																			delete vn->getXML();
																			delete vn;
																		}
																		return false;
																	}
																	if (o1!=o){
																		if (vn!=NULL){
																			delete vn->getXML();
																			delete vn;
																		}
																		return false;
																	}
																}
																n1++;
															}while(vn->toElement(VTDNav::NS));
															vn->toElement(VTDNav::P);
														}
														if (n!=n1){
															if (vn!=NULL){
																delete vn->getXML();
																delete vn;
															}
															return false;
														}
														/*if (m!=0)
														return false;*/
														m1++;
													}while(vn->toElement(VTDNav::NS));
													vn->toElement(VTDNav::P);
												}
												if (m!=m1){
													if (vn!=NULL){
														delete vn->getXML();
														delete vn;
													}
													return false;
												}
												/*if (m!=0)
												return false;*/
												l1++;
											}while(vn->toElement(VTDNav::NS));
											vn->toElement(VTDNav::P);
										}
										if (l!=l1){
											if (vn!=NULL){
												delete vn->getXML();
												delete vn;
											}
											return false;
										}
										k1++;
									}while(vn->toElement(VTDNav::NS));
									vn->toElement(VTDNav::P);
								}
								if (k1 != k){
									if (vn!=NULL){
										delete vn->getXML();
										delete vn;
									}
									return false;
								}
								j1++;
							}while(vn->toElement(VTDNav::NS));
							vn->toElement(VTDNav::P);
						}
						if (j1 != j){
							if (vn!=NULL){
								delete vn->getXML();
								delete vn;
							}
							return false;
						}
						i1++;
				 }while(vn->toElement(VTDNav::NS));				
				}
				if (i1!=i){
					if (vn!=NULL){
						delete vn->getXML();
						delete vn;
					}
					return false;
				}
				vn->toElement(VTDNav::R);

				i= vn->parseInt(vn->getAttrVal(L"attr"));
				i1 = 0;
				if(vn->toElement(VTDNav::LC)){ // to level 1
					//i1++;
					do {
						if (!vn->matchElementNS(L"level1",L"level1")){
							if (vn!=NULL){
								delete vn->getXML();
								delete vn;
							}
							return false;
					 }
						if (vn->matchElementNS(L"top",L"level1")){
							if (vn!=NULL){
								delete vn->getXML();
								delete vn;
							}
							return false;
					 }
						int j = vn->parseInt(vn->getAttrVal(L"attr"));
						int j1 = 0;
						if (vn->toElement(VTDNav::LC)){ // to level 2
							//j1++;
							do {
								if (!vn->matchElementNS(L"level2",L"level2")){
									if (vn!=NULL){
										delete vn->getXML();
										delete vn;
									}
									return false;
								}
								if (vn->matchElementNS(L"top",L"level2")){
									if (vn!=NULL){
										delete vn->getXML();
										delete vn;
									}
									return false;
								}
								int k = vn->parseInt(vn->getAttrVal(L"attr"));
								int k1 = 0;
								if (vn->toElement(VTDNav::LC)){ // level 3
									//k1++;
									do{
										if (!vn->matchElementNS(L"level3",L"level3")){
											if (vn!=NULL){
												delete vn->getXML();
												delete vn;
											}
											return false;
										}
										if (vn->matchElementNS(L"top",L"level3")){
											if (vn!=NULL){
												delete vn->getXML();
												delete vn;
											}
											return false;
										}
										int l = vn->parseInt(vn->getAttrVal(L"attr"));
										int l1 = 0;
										if (vn->toElement(VTDNav::LC)){ // level 4
											//l1++;
											do {	
												if (!vn->matchElementNS(L"level4",L"level4")){
													if (vn!=NULL){
														delete vn->getXML();
														delete vn;
													}
													return false;
												}
												if (vn->matchElementNS(L"top",L"level4")){
													if (vn!=NULL){
														delete vn->getXML();
														delete vn;
													}
													return false;
												}
												int m = vn->parseInt(vn->getAttrVal(L"attr"));
												int m1 = 0;
												if (vn->toElement(VTDNav::LC)){ // level 5
													//m1++;
													do {
														if (!vn->matchElementNS(L"level5",L"level5")){
															if (vn!=NULL){
																delete vn->getXML();
																delete vn;
															}
															return false;
														}
														if (vn->matchElementNS(L"top",L"level5")){
															if (vn!=NULL){
																delete vn->getXML();
																delete vn;
															}
															return false;
														}
														int n = vn->parseInt(vn->getAttrVal(L"attr"));
														int n1 = 0;
														if (vn->toElement(VTDNav::LC)){ // level 5
															//m1++;
															do {
																if (!vn->matchElementNS(L"level6",L"level6")){
																	if (vn!=NULL){
																		delete vn->getXML();
																		delete vn;
																	}
																	return false;
																}
																if (vn->matchElementNS(L"top",L"level6")){
																	if (vn!=NULL){
																		delete vn->getXML();
																		delete vn;
																	}
																	return false;
																}
																int o = vn->parseInt(vn->getAttrVal(L"attr"));
																int o1 = 0;
																if (vn->toElement(VTDNav::LC)){ // level 5
																	do{
																		o1++;
																	}while(vn->toElement(VTDNav::PS));
																	vn->toElement(VTDNav::P);
																	if (o1!=0){
																		if (vn!=NULL){
																			delete vn->getXML();
																			delete vn;
																		}
																		return false;
																	}
																	if (o1!=o){
																		if (vn!=NULL){
																			delete vn->getXML();
																			delete vn;
																		}
																		return false;
																	}
																}
																n1++;
															}while(vn->toElement(VTDNav::PS));
															vn->toElement(VTDNav::P);
														}
														if (n!=n1){
															if (vn!=NULL){
																delete vn->getXML();
																delete vn;
															}
															return false;
														}
														m1++;
													}while(vn->toElement(VTDNav::PS));
													vn->toElement(VTDNav::P);
												}
												if (m!=m1){
													if (vn!=NULL){
														delete vn->getXML();
														delete vn;
													}
													return false;
												}
												/*if (m!=0)
												return false;*/
												l1++;
											}while(vn->toElement(VTDNav::PS));
											vn->toElement(VTDNav::P);
										}
										if (l!=l1){
											if (vn!=NULL){
												delete vn->getXML();
												delete vn;
											}
											return false;
										}
										k1++;
									}while(vn->toElement(VTDNav::PS));
									vn->toElement(VTDNav::P);
								}
								if (k1 != k){
									if (vn!=NULL){
										delete vn->getXML();
										delete vn;
									}
									return false;
								}
								j1++;
							}while(vn->toElement(VTDNav::PS));
							vn->toElement(VTDNav::P);
						}
						if (j1 != j){
							if (vn!=NULL){
								delete vn->getXML();
								delete vn;
							}
							return false;
						}
						i1++;
					}while(vn->toElement(VTDNav::PS));				
				}
				if (vn!=NULL){
					delete vn->getXML();
					delete vn;
				}
				if (i1!=i)
					return false;
				return true;
				//System.out.println("vtdgen ok");
		 }
		 catch (...){
			 if (vn!=NULL){
				 delete vn->getXML();
				 delete vn;
				}
			 //e.printStackTrace();
			 //Console.WriteLine("other exceptions");
			 return false;
			}
		 if (vn!=NULL){
			 delete vn->getXML();
			 delete vn;
		 }
		 return true;
	}
bool test3(string s, int d){
	 VTDNav *vn = NULL;
	 FastIntBuffer *flb1 = new FastIntBuffer();
	 FastIntBuffer *flb2 = new FastIntBuffer();
	 FastIntBuffer *flb3 = new FastIntBuffer();
	 try{
		 int z = 0;
		 VTDGen *vg = new VTDGen();
		 vg->selectLcDepth(d);
		 if (vg->parseFile( true, s.c_str()) == false)
			 return false;
		 VTDNav *vn =vg->getNav();
		 if (vn->toElement(FIRST_CHILD)){
			 do{
				 z++;
				 vn->sampleState(flb1);//leve1
			 } while(vn->toElement(NEXT_SIBLING));
		 }
		 //System.out.println("level1 occurance "+z);
		 //System.out.println("flb2 size is :"+flb1.size());
		 z=0;
		 vn->toElement(ROOT);
		 if (vn->toElement(FIRST_CHILD)){
			 do{
				 if (vn->toElement(FIRST_CHILD)){
					 do{
						 z++;
						 vn->sampleState(flb1); // level2
					 } while(vn->toElement(NEXT_SIBLING));
					 vn->toElement(PARENT);
					}

			 } while(vn->toElement(NEXT_SIBLING));
		 }
		 //System.out.println("level2 occurance "+z);
		 //System.out.println("flb2 size is :"+flb1.size());
		 z = 0;
		 vn->toElement(ROOT);
		 if (vn->toElement(FIRST_CHILD)){
			 do{
				 if (vn->toElement(FIRST_CHILD)){
					 do{
						 if (vn->toElement(FIRST_CHILD)){
							 do{
							 	z++;
								vn->sampleState(flb1); // level3
							 } while(vn->toElement(NEXT_SIBLING));
							 vn->toElement(PARENT);
						 }
					 } while(vn->toElement(NEXT_SIBLING));
					 vn->toElement(PARENT);
				 }
			 } while(vn->toElement(NEXT_SIBLING));
			 vn->toElement(PARENT);
		 }
		 //System.out.println("level3 occurance "+z);
		 //System.out.println("flb2 size is :"+flb1.size());
		 z = 0;
		 vn->toElement(ROOT);
		 if (vn->toElement(FIRST_CHILD)){
			 do{
				 if (vn->toElement(FIRST_CHILD)){
					 do{
						 if (vn->toElement(FIRST_CHILD)){
							 do{
								 if (vn->toElement(FIRST_CHILD)){
									 do{
										 z++;
										 vn->sampleState(flb1); // level3								
									 } while(vn->toElement(NEXT_SIBLING));
									 vn->toElement(PARENT);
								 }
							 } while(vn->toElement(NEXT_SIBLING));
							 vn->toElement(PARENT);
						 }
					 } while(vn->toElement(NEXT_SIBLING));
					 vn->toElement(PARENT);
				 }
			 } while(vn->toElement(NEXT_SIBLING));
			 vn->toElement(PARENT);
		 }
		 //System.out.println("level4 occurance "+z);
		 //System.out.println("flb2 size is :"+flb1.size());
		 z = 0;
		 vn->toElement(ROOT);
		 if (vn->toElement(FIRST_CHILD)){
			 do{
				 if (vn->toElement(FIRST_CHILD)){
					 do{
						 if (vn->toElement(FIRST_CHILD)){
							 do{
								 if (vn->toElement(FIRST_CHILD)){
									 do{
										 if (vn->toElement(FIRST_CHILD)){
											 do{
												 z++;
												 vn->sampleState(flb1); // level3		
											 } while(vn->toElement(NEXT_SIBLING));
											 vn->toElement(PARENT);
										 }
									 } while(vn->toElement(NEXT_SIBLING));
									 vn->toElement(PARENT);
								 }
							 } while(vn->toElement(NEXT_SIBLING));
							 vn->toElement(PARENT);
						 }
					 } while(vn->toElement(NEXT_SIBLING));
					 vn->toElement(PARENT);
				 }
			 } while(vn->toElement(NEXT_SIBLING));
			 vn->toElement(PARENT);
		 }
		 //System.out.println("level5 occurance "+z);
		 //System.out.println("flb2 size is :"+flb1.size());
		 z = 0;
		 vn->toElement(ROOT);
		 if (vn->toElement(FIRST_CHILD)){
			 do{
				 if (vn->toElement(FIRST_CHILD)){
					 do{
						 if (vn->toElement(FIRST_CHILD)){
							 do{
								 if (vn->toElement(FIRST_CHILD)){
									 do{
										 if (vn->toElement(FIRST_CHILD)){
											 do{
												 if (vn->toElement(FIRST_CHILD)){
													 do{
														 z++;
														 vn->sampleState(flb1); // level3	
													 } while(vn->toElement(NEXT_SIBLING));
													 vn->toElement(PARENT);
												 }
											 } while(vn->toElement(NEXT_SIBLING));
											 vn->toElement(PARENT);
										 }
									 } while(vn->toElement(NEXT_SIBLING));
									 vn->toElement(PARENT);
								 }
							 } while(vn->toElement(NEXT_SIBLING));
							 vn->toElement(PARENT);
						 }
					 } while(vn->toElement(NEXT_SIBLING));
					 vn->toElement(PARENT);
				 }
			 } while(vn->toElement(NEXT_SIBLING));
			 vn->toElement(PARENT);
		 }
		 //System.out.println("level6 occurance "+z);
		 //System.out.println("flb2 size is :"+flb1.size());
		 z= 0;
		 vn->toElement(ROOT);
		 AutoPilot ap(vn);
		 ap.selectElement(L"level1:level1");
		 while(ap.iterate()){
			 z++;
			 //System.out.println("index "+vn->getCurrentIndex()+": "+vn->toString(vn->getCurrentIndex()));
			 vn->sampleState(flb2);
		 }
		 //System.out.println("level1 occurance "+z);
		 //System.out.println("flb2 size is :"+flb2.size());
		 z = 0;
		 vn->toElement(ROOT);
		 ap.selectElement(L"level2:level2");
		 while(ap.iterate()){
			 z++;
			 vn->sampleState(flb2);
		 }
		 //System.out.println("level2 occurance "+z);
		 //System.out.println("flb2 size is :"+flb2.size());
		 z = 0;
		 vn->toElement(ROOT);
		 ap.selectElement(L"level3:level3");
		 while(ap.iterate()){
			 z++;
			 vn->sampleState(flb2);
		 }
		 //System.out.println("level3 occurance "+z);
		 //System.out.println("flb2 size is :"+flb2.size());
		 z = 0;
		 vn->toElement(ROOT);
		 ap.selectElement(L"level4:level4");
		 while(ap.iterate()){
			 z++;			
			 vn->sampleState(flb2);
		 }
		 //System.out.println("level4 occurance "+z);
		 //System.out.println("flb2 size is :"+flb2.size());
		 z = 0;
		 vn->toElement(ROOT);
		 ap.selectElement(L"level5:level5");
		 while(ap.iterate()){
			 z++;			
			 vn->sampleState(flb2);
		 }
		 //System.out.println("level5 occurance "+z);
		 //System.out.println("flb2 size is :"+flb2.size());
		 z = 0;
		 vn->toElement(ROOT);
		 ap.selectElement(L"level6:level6");
		 while(ap.iterate()){
			 z++;			
			 vn->sampleState(flb2);
		 }
		 //System.out.println("level6 occurance "+z);
		 //System.out.println("flb2 size is :"+flb2.size());
		 delete vn->getXML();
		 delete vn;

		 z = 0;
		 if (flb1->getSize()!=flb2->getSize()){

			 return false;
		 }
		 for (int i=0;i<flb1->getSize();i++){
			 if (flb1->intAt(i)!= flb2->intAt(i))
				 return false;
		 }
	 }
	 catch (...){
		 //e.printStackTrace();
		 //Console.WriteLine("other exceptions");
		 delete vn->getXML();
		 delete vn;
		 return false;
	 }
	 if (vn!=NULL){
		 delete vn->getXML();
		 delete vn;
	 }
	 return true;		
	}
bool testIndex3(string s, int d){
		VTDNav *vn = NULL;
		FastIntBuffer flb1; //= new FastIntBuffer();
		FastIntBuffer flb2; //= new FastIntBuffer();
		FastIntBuffer flb3; //= new FastIntBuffer();
	try{
		int z = 0;
		VTDGen vg;
		vg.selectLcDepth(d);
		if (vg.parseFile(true, s.c_str()) == false){
			         
			return false;
		}
		vg.writeIndex("tmp.vxl");
		delete vg.getXML();
		VTDGen vg2;
		VTDNav *vn = vg2.loadIndex("tmp.vxl");
		if (vn->toElement(FIRST_CHILD)){
		 do{
		 	z++;
		    vn->sampleState(&flb1);//leve1
		 } while(vn->toElement(NEXT_SIBLING));
		}
		//System.out.println("level1 occurance "+z);
		//System.out.println("flb2 size is :"+flb1.size());
		z=0;
		vn->toElement(ROOT);
		if (vn->toElement(FIRST_CHILD)){
			 do{
			 	if (vn->toElement(FIRST_CHILD)){
					 do{
					 	z++;
					    vn->sampleState(&flb1); // level2
					 } while(vn->toElement(NEXT_SIBLING));
					 vn->toElement(PARENT);
					}
			 	
			 } while(vn->toElement(NEXT_SIBLING));
		}
		//System.out.println("level2 occurance "+z);
		//System.out.println("flb2 size is :"+flb1.size());
		z = 0;
		vn->toElement(ROOT);
		if (vn->toElement(FIRST_CHILD)){
			 do{
			 	if (vn->toElement(FIRST_CHILD)){
					 do{
					 	if (vn->toElement(FIRST_CHILD)){
							 do{
							 	z++;
							    vn->sampleState(&flb1); // level3
							 } while(vn->toElement(NEXT_SIBLING));
							 vn->toElement(PARENT);
						}
					 } while(vn->toElement(NEXT_SIBLING));
					 vn->toElement(PARENT);
				}
			 } while(vn->toElement(NEXT_SIBLING));
			 vn->toElement(PARENT);
		}
		//System.out.println("level3 occurance "+z);
		//System.out.println("flb2 size is :"+flb1.size());
		z = 0;
		vn->toElement(ROOT);
		if (vn->toElement(FIRST_CHILD)){
			 do{
			 	if (vn->toElement(FIRST_CHILD)){
					 do{
					 	if (vn->toElement(FIRST_CHILD)){
							 do{
								 if (vn->toElement(FIRST_CHILD)){
									 do{
										 z++;
										 vn->sampleState(&flb1); // level3								
									 } while(vn->toElement(NEXT_SIBLING));
									 vn->toElement(PARENT);
								}
							 } while(vn->toElement(NEXT_SIBLING));
							 vn->toElement(PARENT);
						}
					 } while(vn->toElement(NEXT_SIBLING));
					 vn->toElement(PARENT);
				}
			 } while(vn->toElement(NEXT_SIBLING));
			 vn->toElement(PARENT);
		}
		//System.out.println("level4 occurance "+z);
		//System.out.println("flb2 size is :"+flb1.size());
		z = 0;
		vn->toElement(ROOT);
		if (vn->toElement(FIRST_CHILD)){
			 do{
			 	if (vn->toElement(FIRST_CHILD)){
					 do{
					 	if (vn->toElement(FIRST_CHILD)){
							 do{
								 if (vn->toElement(FIRST_CHILD)){
									 do{
										 if (vn->toElement(FIRST_CHILD)){
											 do{
												 z++;
												 vn->sampleState(&flb1); // level3		
											 } while(vn->toElement(NEXT_SIBLING));
											 vn->toElement(PARENT);
										}
									 } while(vn->toElement(NEXT_SIBLING));
									 vn->toElement(PARENT);
								}
							 } while(vn->toElement(NEXT_SIBLING));
							 vn->toElement(PARENT);
						}
					 } while(vn->toElement(NEXT_SIBLING));
					 vn->toElement(PARENT);
				}
			 } while(vn->toElement(NEXT_SIBLING));
			 vn->toElement(PARENT);
		}
		//System.out.println("level5 occurance "+z);
		//System.out.println("flb2 size is :"+flb1.size());
		z = 0;
		vn->toElement(ROOT);
		if (vn->toElement(FIRST_CHILD)){
			 do{
			 	if (vn->toElement(FIRST_CHILD)){
					 do{
					 	if (vn->toElement(FIRST_CHILD)){
							 do{
								 if (vn->toElement(FIRST_CHILD)){
									 do{
										 if (vn->toElement(FIRST_CHILD)){
											 do{
												 if (vn->toElement(FIRST_CHILD)){
													 do{
														 z++;
														 vn->sampleState(&flb1); // level3	
													 } while(vn->toElement(NEXT_SIBLING));
													 vn->toElement(PARENT);
												}
											 } while(vn->toElement(NEXT_SIBLING));
											 vn->toElement(PARENT);
										}
									 } while(vn->toElement(NEXT_SIBLING));
									 vn->toElement(PARENT);
								}
							 } while(vn->toElement(NEXT_SIBLING));
							 vn->toElement(PARENT);
						}
					 } while(vn->toElement(NEXT_SIBLING));
					 vn->toElement(PARENT);
				}
			 } while(vn->toElement(NEXT_SIBLING));
			 vn->toElement(PARENT);
		}
		//System.out.println("level6 occurance "+z);
		//System.out.println("flb2 size is :"+flb1.size());
		z= 0;
		vn->toElement(ROOT);
		AutoPilot ap(vn);
		ap.selectElement(L"level1:level1");
		while(ap.iterate()){
			z++;
			//System.out.println("index "+vn->getCurrentIndex()+": "+vn->toString(vn->getCurrentIndex()));
			vn->sampleState(&flb2);
		}
		//System.out.println("level1 occurance "+z);
		//System.out.println("flb2 size is :"+flb2.size());
		z = 0;
		vn->toElement(ROOT);
		ap.selectElement(L"level2:level2");
		while(ap.iterate()){
			z++;
			vn->sampleState(&flb2);
		}
		//System.out.println("level2 occurance "+z);
		//System.out.println("flb2 size is :"+flb2.size());
		z = 0;
		vn->toElement(ROOT);
		ap.selectElement(L"level3:level3");
		while(ap.iterate()){
			z++;
			vn->sampleState(&flb2);
		}
		//System.out.println("level3 occurance "+z);
		//System.out.println("flb2 size is :"+flb2.size());
		z = 0;
		vn->toElement(ROOT);
		ap.selectElement(L"level4:level4");
		while(ap.iterate()){
			z++;			
			vn->sampleState(&flb2);
		}
		//System.out.println("level4 occurance "+z);
		//System.out.println("flb2 size is :"+flb2.size());
		z = 0;
		vn->toElement(ROOT);
		ap.selectElement(L"level5:level5");
		while(ap.iterate()){
			z++;			
			vn->sampleState(&flb2);
		}
		//System.out.println("level5 occurance "+z);
		//System.out.println("flb2 size is :"+flb2.size());
		z = 0;
		vn->toElement(ROOT);
		ap.selectElement(L"level6:level6");
		while(ap.iterate()){
			z++;			
			vn->sampleState(&flb2);
		}
		//System.out.println("level6 occurance "+z);
		//System.out.println("flb2 size is :"+flb2.size());
		
		z = 0;
		if (flb1.getSize()!=flb2.getSize()){
			if (vn!=NULL){
				delete vn->getXML();
				delete vn;
			}
			return false;
		}
		for (int i=0;i<flb1.getSize();i++){
			if (flb1.intAt(i)!= flb2.intAt(i)){
				if (vn!=NULL){
					delete vn->getXML();
					delete vn;
				}
				return false;
			}
		}

	}
	catch (...){
		//e.printStackTrace();
		//Console.WriteLine("other exceptions");
		if (vn!=NULL){
			delete vn->getXML();
			delete vn;
		}
		return false;
	}

	if (vn!=NULL){
		delete vn->getXML();
		delete vn;
	}
	return true;	
	}


bool test(string s, int d){
	UByte *xml;
	VTDNav *vn=NULL;
	//UCSChar *s=NULL;
		try{
			VTDGen vg;
			vg.selectLcDepth(d);
			if (vg.parseFile(true, s.c_str()) == false)
                return false;
			//fis.read(ba);
			//vg.setDoc(ba);
			//vg.parse(true);
			
			vn =vg.getNav();
			xml= vn->getXML();
			BookMark bm(vn);

			int i= vn->parseInt(vn->getAttrVal(L"attr"));
			int i1 = 0;
			//delete xml;
			//delete vn;

			if(vn->toElement(VTDNav::FC)){ // to level 1
				//i1++;
				do {
					bm.recordCursorPosition();
					vn->toElement(ROOT);
					bm.setCursorPosition();
					vn->push();
					vn->toElement(ROOT);
					vn->pop();
					if (!vn->matchElement(L"level1")){
						delete xml; delete vn;
						return false;
					}
					int j = vn->parseInt(vn->getAttrVal(L"attr"));
					int j1 = 0;
					if (vn->toElement(VTDNav::FC)){ // to level 2
						//j1++;
						do {
							bm.recordCursorPosition();
							vn->toElement(VTDNav::R);
							bm.setCursorPosition();
							vn->push();
							vn->toElement(VTDNav::R);
							vn->pop();
							if (!vn->matchElement(L"level2")){
								delete xml; delete vn;
								return false;
							}
							int k = vn->parseInt(vn->getAttrVal(L"attr"));
							int k1 = 0;
							if (vn->toElement(VTDNav::FC)){ // level 3
								//k1++;
								do{
									if (!vn->matchElement(L"level3")){
										delete xml; delete vn;
										return false;
									}
									int l = vn->parseInt(vn->getAttrVal(L"attr"));
									int l1 = 0;
									if (vn->toElement(VTDNav::FC)){ // level 4
										//l1++;
										do {
											bm.recordCursorPosition();
											vn->toElement(VTDNav::R);
											bm.setCursorPosition();
											vn->push();
											vn->toElement(VTDNav::R);
											vn->pop();
											if (!vn->matchElement(L"level4")){
												delete xml; delete vn;
												return false;
											}
											int m = vn->parseInt(vn->getAttrVal(L"attr"));
											int m1 = 0;
											if (vn->toElement(VTDNav::FC)){ // level 5
												//m1++;
												do {
													bm.recordCursorPosition();
													vn->toElement(VTDNav::R);
													bm.setCursorPosition();
													vn->push();
													vn->toElement(VTDNav::R);
													vn->pop();
													if (!vn->matchElement(L"level5")){
														delete xml; delete vn;
														return false;
													}
													int n = vn->parseInt(vn->getAttrVal(L"attr"));
													int n1 = 0;
													if (vn->toElement(VTDNav::FC)){
														//n1++;
														do {
															bm.recordCursorPosition();
															vn->toElement(VTDNav::R);
															bm.setCursorPosition();
															vn->push();
															vn->toElement(VTDNav::R);
															vn->pop();
															if (!vn->matchElement(L"level6")){
																delete xml; delete vn;
																return false;
															}
															int o=vn->parseInt(vn->getAttrVal(L"attr"));
															int o1 = 0;
															/*if (vn->toElement(VTDNav.FC)){
																o1++;
															}*/
															if (o!=o1){
																delete xml; delete vn;
																return false;
															}
															if (o1!=0){
																delete xml; delete vn;
																return false;
															}
															n1++;
														}while (vn->toElement(VTDNav::NS));
														vn->toElement(VTDNav::P);
													}
													if (n!=n1){
														delete xml; delete vn;
														return false;
													}
													/*if (n!=0)
														return false;*/
													m1++;
												}while(vn->toElement(VTDNav::NS));
												vn->toElement(VTDNav::P);
											}
											if (m!=m1){
												delete xml; delete vn;
												return false;
											}
											/*if (m!=0)
												return false;*/
											l1++;
										}while(vn->toElement(VTDNav::NS));
										vn->toElement(VTDNav::P);
									}
									if (l!=l1){
										delete xml; delete vn;
										return false;
									}
									k1++;
								}while(vn->toElement(VTDNav::NS));
								vn->toElement(VTDNav::P);
							}
							if (k1 != k){
								delete xml; delete vn;
								return false;
							}
							j1++;
						}while(vn->toElement(VTDNav::NS));
						vn->toElement(VTDNav::P);
					}
					if (j1 != j){
						delete xml; delete vn;
						return false;
					}
					i1++;
				}while(vn->toElement(VTDNav::NS));				
			}
			if (i1!=i){
				delete xml; delete vn;
				return false;
			}
#if 1
			vn->toElement(ROOT);
			
			i= vn->parseInt(vn->getAttrVal(L"attr"));
			i1 = 0;
			if(vn->toElement(VTDNav::LC)){ // to level 1
				//i1++;
				do {
					bm.recordCursorPosition();
					vn->toElement(VTDNav::R);
					bm.setCursorPosition();
					vn->push();
					vn->toElement(VTDNav::R);
					vn->pop();
					int j = vn->parseInt(vn->getAttrVal(L"attr"));
					int j1 = 0;
					if (vn->toElement(VTDNav::LC)){ // to level 2
						//j1++;
						do {
							bm.recordCursorPosition();
							vn->toElement(VTDNav::R);
							bm.setCursorPosition();
							vn->push();
							vn->toElement(VTDNav::R);
							vn->pop();
							int k = vn->parseInt(vn->getAttrVal(L"attr"));
							int k1 = 0;
							if (vn->toElement(VTDNav::LC)){ // level 3
								//k1++;
								do{
									bm.recordCursorPosition();
									vn->toElement(VTDNav::R);
									bm.setCursorPosition();
									vn->push();
									vn->toElement(VTDNav::R);
									vn->pop();
									int l = vn->parseInt(vn->getAttrVal(L"attr"));
									int l1 = 0;
									if (vn->toElement(VTDNav::LC)){ // level 4
										//l1++;
										do {
											bm.recordCursorPosition();
											vn->toElement(ROOT);
											bm.setCursorPosition();
											vn->push();
											vn->toElement(ROOT);
											vn->pop();
											int m = vn->parseInt(vn->getAttrVal(L"attr"));
											int m1 = 0;
											if (vn->toElement(VTDNav::LC)){ // level 5
												//m1++;
												do {
													bm.recordCursorPosition();
													bm.setCursorPosition();
													vn->push();
													vn->pop();
													int n= vn->parseInt(vn->getAttrVal(L"attr"));
													int n1 =0;
													if (vn->toElement(VTDNav::LC)){ // level 6
														//n1++;
														do{
															bm.recordCursorPosition();
															bm.setCursorPosition();
															vn->push();
															vn->pop();
															n1++;
														}while(vn->toElement(VTDNav::PS));
														vn->toElement(VTDNav::P);
													}
													if (n!=n1){
														delete xml; delete vn;
														return false;
													}
													m1++;
												}while(vn->toElement(VTDNav::PS));
												vn->toElement(VTDNav::P);
											}
											if (m!=m1){
												delete xml; delete vn;
												return false;
											}
											/*if (m!=0)
												return false;*/
											l1++;
										}while(vn->toElement(VTDNav::PS));
										vn->toElement(VTDNav::P);
									}
									if (l!=l1){
										delete xml; delete vn;
										return false;
									}
									k1++;
								}while(vn->toElement(VTDNav::PS));
								vn->toElement(VTDNav::P);
							}
							if (k1 != k){
								delete xml; delete vn;
								return false;
							}
							j1++;
						}while(vn->toElement(VTDNav::PS));
						vn->toElement(VTDNav::P);
					}
					if (j1 != j){
						delete xml; delete vn;
						return false;
					}
					i1++;
				}while(vn->toElement(VTDNav::PS));				
			}
			delete xml; delete vn;
			if (i1!=i)
				return false;
#endif
			//delete xml; delete vn;
			return true;
		    //System.out.println("vtdgen ok");
		}
		catch (...){
			//e.printStackTrace();
            //Console.WriteLine("other exceptions");
			delete xml; delete vn;
			return false;
		}
		//return true;
	}

bool testIndex(string s, int d){
	 VTDNav* vn = NULL;
	 UByte *xml;
		try{
			VTDGen vg;
			
			vg.selectLcDepth(d);
			if (vg.parseFile( true,s.c_str()) == false)
                return false;
			vg.writeIndex("tmp.vxl");
			delete vg.getXML();
			VTDGen vg2 ;
			vn = vg2.loadIndex("tmp.vxl");
			xml = vn->getXML();
			//VTDNav vn = *vn1;
			//VTDNav vn =vg.getNav();
#if 1
			int i= vn->parseInt(vn->getAttrVal(L"attr"));
			int i1 = 0;
			if(vn->toElement(VTDNav::FC)){ // to level 1
				//i1++;
				do {
					if (!vn->matchElement(L"level1")){
						delete xml; delete vn;
						return false;
					}
					int j = vn->parseInt(vn->getAttrVal(L"attr"));
					int j1 = 0;
					if (vn->toElement(VTDNav::FC)){ // to level 2
						//j1++;
						do {
							if (!vn->matchElement(L"level2")){
								delete xml; delete vn;
								return false;
							}
							int k = vn->parseInt(vn->getAttrVal(L"attr"));
							int k1 = 0;
							if (vn->toElement(VTDNav::FC)){ // level 3
								//k1++;
								do{
									if (!vn->matchElement(L"level3")){
										delete xml; delete vn;
										return false;
									}
									int l = vn->parseInt(vn->getAttrVal(L"attr"));
									int l1 = 0;
									if (vn->toElement(VTDNav::FC)){ // level 4
										//l1++;
										do {
											if (!vn->matchElement(L"level4")){
												delete xml; delete vn;
												return false;
											}
											int m = vn->parseInt(vn->getAttrVal(L"attr"));
											int m1 = 0;
											if (vn->toElement(VTDNav::FC)){ // level 5
												//m1++;
												do {
													if (!vn->matchElement(L"level5")){
														delete xml; delete vn;
														return false;
													}
													int n = vn->parseInt(vn->getAttrVal(L"attr"));
													int n1 = 0;
													if (vn->toElement(VTDNav::FC)){
														//n1++;
														do {
															if (!vn->matchElement(L"level6")){
																delete xml; delete vn;
																return false;
															}
															int o=vn->parseInt(vn->getAttrVal(L"attr"));
															int o1 = 0;
															/*if (vn->toElement(VTDNav.FC)){
																o1++;
															}*/
															if (o!=o1){
																delete xml; delete vn;															
																return false;
															}
															if (o1!=0){
																delete xml; delete vn;
																return false;
															}
															n1++;
														}while (vn->toElement(VTDNav::NS));
														vn->toElement(VTDNav::P);
													}
													if (n!=n1){
														delete xml; delete vn;
														return false;
													}
													/*if (n!=0)
														return false;*/
													m1++;
												}while(vn->toElement(VTDNav::NS));
												vn->toElement(VTDNav::P);
											}
											if (m!=m1){
												delete xml; delete vn;
												return false;
											}
											/*if (m!=0)
												return false;*/
											l1++;
										}while(vn->toElement(VTDNav::NS));
										vn->toElement(VTDNav::P);
									}
									if (l!=l1){
										delete xml; delete vn;
										return false;
									}
									k1++;
								}while(vn->toElement(VTDNav::NS));
								vn->toElement(VTDNav::P);
							}
							if (k1 != k){
								delete xml; delete vn;							
								return false;
							}
							j1++;
						}while(vn->toElement(VTDNav::NS));
						vn->toElement(VTDNav::P);
					}
					if (j1 != j){
						delete xml; delete vn;
						return false;
					}
					i1++;
				}while(vn->toElement(VTDNav::NS));				
			}
			if (i1!=i){
				delete xml; delete vn;
				return false;
			}
			vn->toElement(ROOT);
			
			i= vn->parseInt(vn->getAttrVal(L"attr"));
			i1 = 0;
			if(vn->toElement(VTDNav::LC)){ // to level 1
				//i1++;
				do {
					int j = vn->parseInt(vn->getAttrVal(L"attr"));
					int j1 = 0;
					if (vn->toElement(VTDNav::LC)){ // to level 2
						//j1++;
						do {
							int k = vn->parseInt(vn->getAttrVal(L"attr"));
							int k1 = 0;
							if (vn->toElement(VTDNav::LC)){ // level 3
								//k1++;
								do{
									int l = vn->parseInt(vn->getAttrVal(L"attr"));
									int l1 = 0;
									if (vn->toElement(VTDNav::LC)){ // level 4
										//l1++;
										do {											
											int m = vn->parseInt(vn->getAttrVal(L"attr"));
											int m1 = 0;
											if (vn->toElement(VTDNav::LC)){ // level 5
												//m1++;
												do {
													int n= vn->parseInt(vn->getAttrVal(L"attr"));
													int n1 =0;
													if (vn->toElement(VTDNav::LC)){ // level 6
														//n1++;
														do{
															n1++;
														}while(vn->toElement(VTDNav::PS));
														vn->toElement(VTDNav::P);
													}
													if (n!=n1){
														delete xml; delete vn;
														return false;
													}
													m1++;
												}while(vn->toElement(VTDNav::PS));
												vn->toElement(VTDNav::P);
											}
											if (m!=m1){
												delete xml; delete vn;
												return false;
											}
											/*if (m!=0)
												return false;*/
											l1++;
										}while(vn->toElement(VTDNav::PS));
										vn->toElement(VTDNav::P);
									}
									if (l!=l1){
										delete xml; delete vn;
										return false;
									}
									k1++;
								}while(vn->toElement(VTDNav::PS));
								vn->toElement(VTDNav::P);
							}
							if (k1 != k){
								delete xml; delete vn;
								return false;
							}
							j1++;
						}while(vn->toElement(VTDNav::PS));
						vn->toElement(VTDNav::P);
					}
					if (j1 != j){
						delete xml; delete vn;
						return false;
					}
					i1++;
				}while(vn->toElement(VTDNav::PS));				
			}

#endif
			delete xml; delete vn;
			//if (i1!=i)
			//	return false;
			return true;
		    //System.out.println("vtdgen ok");
		}
		catch (...){
			//e.printStackTrace();
            //Console.WriteLine("other exceptions");
			delete xml; delete vn;
			return false;
		}
		//return true;
	}

	bool testRecoverNode(string s, int d){
		VTDNav *vn;
		UByte *xml;
		try {
			VTDGen vg ;
			FastIntBuffer fib1;// = new FastIntBuffer();
			FastIntBuffer fib2;// = new FastIntBuffer();
			FastIntBuffer fib3;// = new FastIntBuffer();
			//VTDGen vg;
			vg.selectLcDepth(d);
			if (vg.parseFile(true, s.c_str()) == false)
                return false;
			//fis.read(ba);
			//vg.setDoc(ba);
			//vg.parse(true);
			
			vn =vg.getNav();
			//vg.selectLcDepth(d);
			//File f = new File(s);
			//byte[] ba = new byte[(int) f.length()];
			//FileInputStream fis = new FileInputStream(f);
			//fis.read(ba);
			//vg.setDoc(ba);
			//vg.parse(true);
			//vn= vg.getNav();
			xml = vn->getXML();
			AutoPilot ap(vn);
			ap.selectXPath(L"(//*)[1]|//@*");
			//ap.printExprString();
			int i=-1,k=0;

			while((i=ap.evalXPath())!=-1){
				if ((k&1)!=0 ) {
					fib1.append(i);
					vn->sampleState(&fib2);
				}
				k++;
			}
#if 1
			for (int z=0; z<fib1.getSize() ; z++ ){
				vn->recoverNode(fib1.intAt(z));
				vn->sampleState(&fib3);
			}
			if (fib2.getSize()!=fib3.getSize()){
				delete vn;
				delete xml;
				return false;
			}
			for (int g=0;g<fib2.getSize();g++){
				if (fib2.intAt(g)!= fib3.intAt(g)){
					delete vn;
					delete xml;
					return false;
				}
			}
#endif
			delete vn;
			delete xml;
			return true;
		}  catch (...) {
			//e.printStackTrace();
			//System.out.println("other exceptions");
			delete vn;
			delete xml;
			return false;
		}

	}
	class a{
	public:
		int i;
		virtual void printIdentity(){
			printf("a\n");
		}
		~a(){
			printf("deleting 1\n");
		}
	};
	class b:public a{
		int i;
		void printIdentity(){
			printf("b\n");
		}
	public:
		~b(){
			printf("deleting 2\n");
		}
	};


	void m1(string fn){
		string fileName = string("c://benchmark_2.7//xml//").append(fn);
		const char *name = fileName.c_str();
		if (test2(fileName,3) ){
			cout << name<< " passed!"<<endl;
		}
		else
			cout << name<< " failed!"<<endl;
		if (testIndex2(fileName,3 )){
			cout << name<< " passed!"<<endl;
		}else
			cout << name<< " failed!"<<endl;

		if (test2(fileName,5) ){
			cout << name<< " passed!"<<endl;
		}
		else
			cout << name<< " failed!"<<endl;
		if (testIndex2(fileName,5 )){
			cout << name<< " passed!"<<endl;
		}else
			cout << name<< " failed!"<<endl;
		//delete name;
	}

	void m11(string fn){
		string fileName = string("c://benchmark_2.7//xml//").append(fn);
		const char *name = fileName.c_str();
		
		if (test(fileName,3) ){
			cout << name<< " passed!"<<endl;
		}
		else
			cout << name<< " failed!"<<endl;
		
		if (testIndex(fileName,3 )){
			cout << name<< " passed!"<<endl;
		}else
			cout << name<< " failed!"<<endl;
		
		if (testRecoverNode(fileName,3 )){
			cout << name << " passed!"<<endl;
		}else
			cout << name << " failed!"<<endl;
		
		if (test(fileName,5) ){
			cout << name<< " passed!"<<endl;
		}
		else
			cout << name<< " failed!"<<endl;

		if (testIndex(fileName,5 )){
			cout << name<< " passed!"<<endl;
		}else
			cout << name<< " failed!"<<endl;

		if (testRecoverNode(fileName,5 )){
			cout << name << " passed!"<<endl;
		}else
			cout << name<< " failed!"<<endl;
		//delete name;
	}

	void m111(string fn){
		string fileName = string("c://benchmark_2.7//xml//").append(fn);
		const char *name = fileName.c_str();
		
		if (test3(fileName,3) ){
			cout << name<< " passed!"<<endl;
		}
		else
			cout << name<< " failed!"<<endl;
		
		if (testIndex3(fileName,3 )){
			cout << name<< " passed!"<<endl;
		}else
			cout << name<< " failed!"<<endl;

		if (test3(fileName,5) ){
			cout << name<< " passed!"<<endl;
		}
		else
			cout << name<< " failed!"<<endl;
		if (testIndex3(fileName,5 )){
			cout << name<< " passed!"<<endl;
		}else
			cout << name<< " failed!"<<endl;
		//delete name;
	}

	void testFastIntBuffer(){
		FastIntBuffer fib;
		FastLongBuffer flb;
		ArrayList al;
		VTDNav *vn = NULL;
		UByte *xml = NULL;
		VTDGen vg;
		try{
		if (vg.parseFile(true,"c://benchmark_2.7//xml//nav_1_0.xml")){
			vn = vg.getNav();
			xml=vn->getXML();
			AutoPilot ap(vn);
			ap.selectXPath(L"//a/b/c/d | @* [");
			
			delete xml;
			delete vn;
		}
		}catch(...){
			delete xml;
			delete vn;
		}
	}
int main(){
	char a[40];
	/*a a1, a2;
	a1 = *(new a());
	a2 = *(new b());
	a1.printIdentity();
	a2.printIdentity();*/
	//printf(" pointer size is %d \n",sizeof(int *));
	/*while(true){
		testFastIntBuffer();
	}*/

	//a1->printIdentity();
	//a2->printIdentity();
	
	for (int i = 0; i < 40; i++)
	{
		for (int j = 0; j < 5; j++){
			sprintf(a,"nav_%d_%d.xml",i,j);
			m11(string(a));
		}
	}
	
	for (int i = 40; i < 80; i++)
	{
		for (int j = 0; j < 5; j++){
			sprintf(a,"nav_%d_%d.xml",i,j);
			m1(string(a));
		}
	}
	for (int i = 0; i < 40; i++)
	{
		//System.out.print("i = "+i);
		for (int j = 0; j < 5; j++){
			sprintf(a,"ap_%d_%d.xml",i,j);
			m111(string(a));
		}
	}
	//cout << s1.c_str() << "  "<< a1.i <<endl;
	//sprintf();
	string s("abc");
	const char *s1 = s.c_str();
	const char *s2 = s.c_str();
	return 0;
}

