#pragma once
class CLR
{
public:
	static ICorRuntimeHost* pClrHost;
	static mscorlib::_AppDomainPtr pDefaultDomain;
	static IUnknownPtr pAppDomainPunk;
	static mscorlib::_AssemblyPtr DLLPluginManager;
	static string akey3;
	static string akey4;
};

