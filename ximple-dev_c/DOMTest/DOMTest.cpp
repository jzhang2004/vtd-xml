// This is the main DLL file.

#include "stdafx.h"

#include <string.h>
#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

extern "C"
{
#include "xpath.h"
#include "helper.h"
#include "vtdGen.h"
}

using namespace NUnit::Framework;

namespace DOMTest
{

	struct exception_context the_exception_context[1];

	static VTDNav parseString(char* s)
    {
        VTDGen vg = new VTDGen();
        //System.Text.ASCIIEncoding encoding = new System.Text.ASCIIEncoding();

        //vg.setDoc(encoding.GetBytes(s));
		vg.

        vg.parse(true);

        return vg.getNav();
    }


	[TestFixture]
	public ref class XPathTest
	{

	public:
		[Test]
	    void Translate()
		{
            UCSChar* s = L"<test2>"
                + L"<e1 attr='1'>A</e1>" 
                + L"<e1 attr='2'>ABC</e1>"
                + L"<f1 attr='3'>_aabb_</f1>" 
                + L"<f1 attr='4' attr1='6'></f1>" 
                + L"</test2>";

            VTDNav vn = parseString(s);
        
            AutoPilot ap = new AutoPilot(vn);
        
	        Assert::AreEqual(1234,552367893);
		}
	};
}
