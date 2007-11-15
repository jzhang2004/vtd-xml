#include "everything.h"
struct exception_context the_exception_context[1];
void fillCD(VTDNav *vn, UByte *title, 
			UByte *artist, UByte *country, 
			UByte *company, float price, int year){
				int i = -1;
				UByte a[10],b[10];
				sprintf(a,"%4.2f",price);
				sprintf(b,"%d",year);
				
				if (toElement(vn,FIRST_CHILD))
				{
					overWrite(vn, getText(vn), title, 0, strlen(title)); // the assumption --getText won't return 1
					toElement(vn, NEXT_SIBLING);
					overWrite(vn, getText(vn), artist,0, strlen(artist));
					toElement(vn, NEXT_SIBLING);
					overWrite(vn, getText(vn), country,0, strlen(country));
					toElement(vn, NEXT_SIBLING);
					overWrite(vn, getText(vn), company, 0, strlen(company));
					toElement(vn, NEXT_SIBLING);
					overWrite(vn, getText(vn), a, 0, strlen(a));
					toElement(vn, NEXT_SIBLING);
					overWrite(vn, getText(vn), b,0,strlen(b));
				}
				toElement(vn,PARENT);
}

int main(){
	exception e;
	VTDGen *vg = NULL;
	VTDNav *vn = NULL;
	AutoPilot *ap1 = NULL;
	FILE *f = NULL;
	UCSChar *string = NULL;

	int i;

	Try{			
		vg = createVTDGen();		
		if (parseFile(vg,TRUE,"d:/ximpleware_2.2_c/vtd-xml/codeGuru/12/cd_template.xml")==FALSE){
			free(vg->XMLDoc);
			freeVTDGen(vg);
			return 0;
		}	
		ap1 = createAutoPilot2();
		selectXPath(ap1,L"/CATALOG/CD");
		vn = getNav(vg);
		bind(ap1,vn);
		if((i=evalXPath(ap1))!=-1){
			fillCD(vn,"Empire Burlesque", "Bob Dylan", "USA", "Columbia", 10.90, 1985);		
		}	
		if((i=evalXPath(ap1))!=-1){
			fillCD(vn,"Still Got the Blues", "Gary More", "UK", "Virgin Records", 10.20, 1990);		
		}
		f= fopen("d:/ximpleware_2.2_c/vtd-xml/codeGuru/12/new_cd.xml","wb");
		fwrite(vn->XMLDoc+vn->docOffset,1,vn->docLen,f);
		fclose(f);
		free(vn->XMLDoc);
	}Catch(e){// handle various types of exceptions here
	}
	freeAutoPilot(ap1);
	freeVTDGen(vg);
	freeVTDNav(vn);
	return 0;
}

