========================================================================
    CONSOLE APPLICATION : vtd-xml Project Overview
========================================================================

AppWizard has created this vtd-xml application for you.  
This file contains a summary of what you will find in each of the files that
make up your vtd-xml application.


vtd-xml.vcproj
    This is the main project file for VC++ projects generated using an Application Wizard. 
    It contains information about the version of Visual C++ that generated the file, and 
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

vtd-xml.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
Other standard files:

StdAfx.h, StdAfx.cpp
    These files are used to build a precompiled header (PCH) file
    named vtd-xml.pch and a precompiled types file named StdAfx.obj.

/////////////////////////////////////////////////////////////////////////////
Other notes:

AppWizard uses "TODO:" comments to indicate parts of the source code you
should add to or customize.

setting a freed pointer to NULL is a task left to users of the library.

exception handling, use "cexcept.h" as the foundation

A global exception is defined
out_of_mem exception, if thrown, doesn't require one to free up any resource,
since that is taken care of by the exception thrower

exception type  (out of mem, parse_time, nav_time, io)
/////////////////////////////////////////////////////////////////////////////
