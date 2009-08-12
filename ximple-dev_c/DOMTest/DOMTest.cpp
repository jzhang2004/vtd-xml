// This is the main DLL file.

#include "stdafx.h"

#include "DOMTest.h"

using namespace NUnit::Framework;

[TestFixture]
public ref class CChecksumTester
{
public:
    [Test]
    void GetChecksumTest()
    {
        char* buf = "This is a test";
        
        Assert::AreEqual(1234,552367893);
    }
};

