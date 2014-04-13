#include "stdafx.h"
#include "Functions.h"
#include "CLR.h"
#include "gMain.h"
#include "aes.h"
#include <Psapi.h>
#include <atlstr.h>
#include "IntNet.h"
#include "IniReader.h"

int main()
{
	HINSTANCE hInstC = GetModuleHandle(0);
	SendMessage(GetConsoleWindow(), WM_SETICON, ICON_SMALL, (LPARAM)LoadImage(hInstC, MAKEINTRESOURCE(IDI_MAIN_ICON), IMAGE_ICON, 0, 0, 0));
	SendMessage(GetConsoleWindow(), WM_SETICON, ICON_BIG, (LPARAM)LoadImage(hInstC, MAKEINTRESOURCE(IDI_MAIN_ICON), IMAGE_ICON, 0, 0, 0));

	SetConsoleTitle( L"gProxy" );


	DWORD* processList = new DWORD[300];
	DWORD size = 300;
	int num = 0;
	if(EnumProcesses(processList, sizeof(DWORD) * 300, &size))
	{
		for(int i = 0; i < size / sizeof(DWORD); i++)
		{
			HANDLE Handle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processList[i]);
			WCHAR buffer[MAX_PATH];
			if(GetModuleBaseName(Handle, 0, buffer, MAX_PATH))
			{
				if(CString(buffer).Compare(L"gProxy.exe") == 0)
					num++;
			}
		}
	}

	if(num > 1)
	{
		cout << "Another gProxy instance was found running, this instance will be closed!" << endl;
		Sleep(2000);
		exit(0);
	}

	delete[] processList;

	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2), &wsaData);

	CorBindToRuntimeEx(L"v4.0.30319", L"wks", STARTUP_LOADER_OPTIMIZATION_SINGLE_DOMAIN | STARTUP_CONCURRENT_GC, CLSID_CorRuntimeHost, IID_ICorRuntimeHost, (PVOID*)&CLR::pClrHost);
	CLR::pClrHost->Start();
	CLR::pClrHost->GetDefaultDomain(&CLR::pAppDomainPunk);
	CLR::pAppDomainPunk->QueryInterface(__uuidof(mscorlib::_AppDomain),(void**) &CLR::pDefaultDomain);
	SAFEARRAY* res1 = Functions::LoadDecryptResource(MAKEINTRESOURCE(IDR_NT1));
	CLR::DLLPluginManager = CLR::pDefaultDomain->Load_3(res1);

	SafeArrayDestroy(res1);

	gMain::Main = new gMain();

	string str;
	while(true)
	{
		cin >> str;
		if(str == "/about")
		{
			printf("\n------------------------\ngProxy v0.0.0.2\nCopyright 2010 Gabrola\n------------------------\n\n");
		}
	}

	return 0;
}