#include "StdAfx.h"
#include "clr.h"

using namespace mscorlib;

ICorRuntimeHost* CLR::pClrHost = NULL;
_AppDomainPtr CLR::pDefaultDomain = NULL;
IUnknownPtr CLR::pAppDomainPunk = NULL; 
_AssemblyPtr CLR::DLLPluginManager;
string CLR::akey3 = "ifeJOkcZ";
string CLR::akey4 = "szFLyWvG";