

#include "xpath.h"
#include "helper.h"
#include "vtdGen.h"
#include "cexcept.h"

#include "xpathTest.h"

#include <assert.h>

    void translateTest() 
    { 

        AutoPilot* ap = createAutoPilot2();

		if(selectXPath(ap,L"translate('12:30','30','45')"))
		{
			UCSChar* s = evalXPathToString(ap);
			assert(wcscmp(s, L"12:45") == 0);    

			free(s);
		}

		if(selectXPath(ap,L"translate('12:30','0123','abc')"))
		{
			UCSChar* s = evalXPathToString(ap);
			assert(wcscmp(s, L"bc:a") == 0);    

			free(s);
		}

		if(selectXPath(ap,L"translate('','','abc')"))
		{
			UCSChar* s = evalXPathToString(ap);
			assert(wcscmp(s, L"") == 0);    

			free(s);
		}

		if(selectXPath(ap,L"translate('12:30','33','3bc')"))
		{
			UCSChar* s = evalXPathToString(ap);
			assert(wcscmp(s, L"12:30") == 0);    

			free(s);
		}

		freeAutoPilot(ap);

	}

