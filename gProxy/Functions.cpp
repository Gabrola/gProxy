#include "StdAfx.h"
#include "Functions.h"
#include "resource.h"
#include "AES.h"
#include "IntNet.h"
#include "CLR.h"
#include <cmath>

string Functions::ByteArrayToPhp(byte* Buffer, int BufferLen)
{
	string PHPString = "";
	for(int i = 0; i < BufferLen; i++)
	{
		char str[3];
		sprintf(str, "%02X", Buffer[i]);
		PHPString += "0x" + string(str);
		if(i != BufferLen - 1)
		{
			PHPString += "-";
		}
	}

	return PHPString;
}

string Functions::ByteArrayToCSharp(byte* Buffer, int BufferLen)
{
	string PHPString = "";
	for(int i = 0; i < BufferLen; i++)
	{
		char str[3];
		sprintf(str, "%02X", Buffer[i]);
		PHPString += "0x" + string(str);
		if(i != BufferLen - 1)
		{
			PHPString += ", ";
		}
	}

	return PHPString;
}

string Functions::ByteArrayToString(byte* Buffer, int BufferLen)
{
	string PHPString = "";
	for(int i = 0; i < BufferLen; i++)
	{
		char str[3];
		sprintf(str, "%02X", Buffer[i]);
		PHPString += string(str);
		if(i != BufferLen - 1)
		{
			PHPString += "-";
		}
	}

	return PHPString;
}

vector<string>& Functions::split(const string &s, char delim, vector<string> &elems) {
	stringstream ss(s);
	string item;
	while(getline(ss, item, delim)) {
		char d0 = 0xD;
		elems.push_back(Functions::Replace(item, string(&d0, 1), ""));
	}
	return elems;
}


vector<string> Functions::split(const string &s, char delim) {
	vector<string> elems;
	return split(s, delim, elems);
}

int Functions::str2int (const string &str) {
	stringstream ss(str);
	int num;
	ss >> num;
	return num;
}

char Functions::str2char (const string &str) {
	stringstream ss(str);
	char num;
	ss >> num;
	return num;
}

char* Functions::PhpToByteArray(string Buffer)
{
	vector<string> BufSplit = split(Buffer, '-');
	char* NewBuffer = (char*) malloc(sizeof(char) * (BufSplit.size()));
	for(int i = 0; i < (int)BufSplit.size(); i++)
	{
		NewBuffer[i] = str2int(BufSplit[i]);
	}

	return NewBuffer;
}

char* Functions::read_pass(char *const prompt)
{
	cout << prompt;
	char *pass = (char*) malloc(sizeof(char) * (40));
	int size = 0;
	char l;
	while (size < 39)
	{
		l = _getch(); 
		if (l == '\r')
		{
			putchar('\n');
			break;
		}
		if (l == 8)
		{
			if (size > 0)
			{
				size = size - 1;
				pass[size + 1] = '\0';
				putchar(8); 
				putchar(' ');
				putchar(8);
			}
		}
		else if (l != '\r')
		{
			pass[size] = l;
			size++;
			printf("*");
		}
	}

	pass[size] = '\0';
	return pass;
}

string Functions::HttpPost(LPCWSTR uri, char* pars, HINTERNET hHandle, LPCWSTR host)
{
	HINTERNET cHandle = InternetConnect(hHandle, host, INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	HINTERNET oHandle = HttpOpenRequest(cHandle, L"POST", uri, L"HTTP/1.1", NULL, NULL, INTERNET_FLAG_RELOAD, 0);
	BOOL sHandle = HttpSendRequest(oHandle, L"Content-Type: application/x-www-form-urlencoded", 47, pars, strlen(pars));
	if(sHandle)
	{
		char* buffer = (char*) malloc(sizeof(char) * (1024));
		DWORD dwVal = 0;
		BOOL eHandle = InternetReadFile(oHandle, buffer, 1024, &dwVal);
		if(eHandle)
		{
			char* buffer2 = (char*) malloc(sizeof(char) * (dwVal + 1));
			buffer2[dwVal] = '\0';
			memcpy(buffer2, buffer, dwVal);
			string str = string(buffer2);

			free(buffer);
			free(buffer2);
			return str;
		}
		free(buffer);
	}

	return "";
}

string Functions::HttpGet(LPCWSTR uri, HINTERNET hHandle, LPCWSTR host)
{
	HINTERNET cHandle = InternetConnect(hHandle, host, INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
	HINTERNET oHandle = HttpOpenRequest(cHandle, L"GET", uri, L"HTTP/1.1", NULL, NULL, INTERNET_FLAG_RELOAD, 0);
	BOOL sHandle = HttpSendRequest(oHandle, L"", 0, 0, 0);
	if(sHandle)
	{
		char* buffer = (char*) malloc(sizeof(char) * (1024));
		DWORD dwVal = 0;
		BOOL eHandle = InternetReadFile(oHandle, buffer, 1024, &dwVal);
		if(eHandle)
		{
			char* buffer2 = (char*) malloc(sizeof(char) * (dwVal + 1));
			buffer2[dwVal] = '\0';
			memcpy(buffer2, buffer, dwVal);
			string str = string(buffer2);

			free(buffer);
			free(buffer2);
			return str;
		}

		free(buffer);
	}

	return "";
}

string Functions::ByteArrayToHex(byte* Buffer, int BufferLen)
{
	string HexString = "";

	char* tempString = (char*) malloc(sizeof(char) * (3));
	for(int i = 0; i < BufferLen; i++)
	{
		sprintf(tempString, "%02X", Buffer[i]);
		HexString += tempString;
	}
	free(tempString);

	return HexString;
}

VARIANT Functions::InvokeMember(mscorlib::_AppDomainPtr pDefaultDomain, mscorlib::_AssemblyPtr dll, BSTR Typename, BSTR Method, int Numargs, ...)
{
	VARIANT returnValue;

	try
	{
		va_list vl;
		va_start(vl,Numargs);
		SAFEARRAY* psa = SafeArrayCreateVector(VT_VARIANT, 0, Numargs / 2);

		for (int i = 0; i < Numargs; i+=2)
		{
			int typeName = va_arg(vl, int);
			switch(typeName)
			{
			case 0:
				{
					BSTR wstr = SysAllocString(va_arg(vl, wchar_t*));
					VARIANT val;
					val.vt = VT_BSTR;
					val.bstrVal = wstr;
					LONG index[1] = { i / 2 };

					SafeArrayPutElement(psa, index, &val);

					SysFreeString(wstr);
					break;
				}
			case 1:
				{
					int intval = va_arg(vl, int);
					VARIANT val;
					val.vt = VT_INT;
					val.intVal = intval;
					LONG index[1] = { i / 2 };

					SafeArrayPutElement(psa, index, &val);
					break;
				}
			}
		}

		mscorlib::_ObjectPtr pObject; 

		variant_t vtobj = dll->CreateInstance(Typename);
		vtobj.pdispVal->QueryInterface(__uuidof(mscorlib::_Object),(void**)&pObject); 
		mscorlib::_TypePtr pType = pObject->GetType();

		returnValue = pType->InvokeMember_3(Method,
			mscorlib::BindingFlags_InvokeMethod,
			NULL,
			vtobj,
			psa);

		SafeArrayDestroy(psa);
		vtobj.Clear();
		pType->Release();
		pObject->Release();
	}
	catch(_com_error& error) {
		_tprintf(TEXT("ERROR: %s\n"),(_TCHAR*)error.Description());
	}

	return returnValue;
}

VARIANT Functions::InvokeMember2(mscorlib::_AppDomainPtr pDefaultDomain, BSTR dll, BSTR Typename, BSTR Method, int Numargs, ...)
{
	VARIANT returnValue;

	try
	{
		va_list vl;
		va_start(vl,Numargs);
		SAFEARRAY* psa = SafeArrayCreateVector(VT_VARIANT, 0, Numargs / 2);

		for (int i = 0; i < Numargs; i+=2)
		{
			int typeName = va_arg(vl, int);
			switch(typeName)
			{
			case 0:
				{
					BSTR wstr = SysAllocString(va_arg(vl, wchar_t*));
					VARIANT val;
					val.vt = VT_BSTR;
					val.bstrVal = wstr;
					LONG index[1] = { i / 2 };

					SafeArrayPutElement(psa, index, &val);

					SysFreeString(wstr);
					break;
				}
			case 1:
				{
					int intval = va_arg(vl, int);
					VARIANT val;
					val.vt = VT_INT;
					val.intVal = intval;
					LONG index[1] = { i / 2 };

					SafeArrayPutElement(psa, index, &val);
					break;
				}
			}
		}

		mscorlib::_ObjectPtr pObject; 
		mscorlib::_ObjectHandlePtr pObjectHandle; 

		pObjectHandle = pDefaultDomain->CreateInstanceFrom(dll, Typename);
		variant_t vtobj = pObjectHandle->Unwrap();
		vtobj.pdispVal->QueryInterface(__uuidof(mscorlib::_Object),(void**)&pObject); 

		mscorlib::_TypePtr pType = pObject->GetType();

		returnValue = pType->InvokeMember_3(Method,
			mscorlib::BindingFlags_InvokeMethod,
			NULL,
			vtobj,
			psa);

		SafeArrayDestroy(psa);
		vtobj.Clear();
		pType->Release();
		pObject->Release();
	}
	catch(_com_error& error) {
		_tprintf(TEXT("ERROR: %s\n"),(_TCHAR*)error.Description());
	}

	return returnValue;
}

SAFEARRAY* Functions::LoadResourceF(LPCWSTR resource)
{
	SizeofResource(NULL, FindResource(NULL, resource, L"NT"));
	byte* resou = (byte*) LockResource(LoadResource(NULL, FindResource(NULL, resource, L"NT")));
	SAFEARRAYBOUND bounds;
	bounds.cElements = SizeofResource(NULL, FindResource(NULL, resource, L"NT"));
	bounds.lLbound = 0;
	SAFEARRAY* psaz =  SafeArrayCreate(VT_UI1, 1, &bounds);
	BYTE* real_array = (BYTE*)psaz->pvData;
	memcpy(real_array, resou, SizeofResource(NULL, FindResource(NULL, resource, L"NT")));
	return psaz; 
}

SAFEARRAY* Functions::LoadDecryptResource(LPCWSTR resource)
{
	CRijndael oRijndael;
	char* outBuf;
	oRijndael.MakeKey(string(IntNet::akey1 + IntNet::akey2 + CLR::akey3 + CLR::akey4).c_str(), "", 32, 16);
	outBuf = oRijndael.Decrypt((const char*) LockResource(LoadResource(NULL, FindResource(NULL, resource, L"NT"))), SizeofResource(NULL, FindResource(NULL, resource, L"NT")));

	SAFEARRAYBOUND bounds;
	bounds.cElements = SizeofResource(NULL, FindResource(NULL, resource, L"NT")); // # of indexes
	bounds.lLbound = 0;
	SAFEARRAY* psaz =  SafeArrayCreate(VT_UI1, 1, &bounds);
	BYTE* real_array = (BYTE*)psaz->pvData;
	memcpy(real_array, outBuf, SizeofResource(NULL, FindResource(NULL, resource, L"NT")));

	free(outBuf);

	return psaz; 
}

wchar_t* Functions::StringToWide(string str)
{
	int len = strlen(str.c_str())+1;
	wchar_t *wText = (wchar_t*) malloc(sizeof(wchar_t) * (len));
	memset(wText,0,len);
	::MultiByteToWideChar(  CP_ACP, NULL,str.c_str(), -1, wText,len );

	return wText;
}

const char* Functions::PadNull(byte* data, int datalen)
{
	byte* DString = new byte[datalen + 1];
	memset(DString, 0, datalen + 1);
	memcpy(DString, data, datalen);

	return (char*) DString;
}

int Functions::Distance( int X1, int Y1, int X2, int Y2 )
{
	int num = abs((int)(X1 - X2));
	int num2 = abs((int)(Y1 - Y2));
	return max(num, num2);
}

std::string Functions::Replace( string base, string find, string replace )
{
	string newString = base;
	ReCheck:
	if(newString.find(find) != string::npos)
	{
		newString.replace(newString.find(find), find.length(), replace);
		goto ReCheck;
	}
	return newString;
}


void ReadBytes(byte* source, byte* destination, int length)
{
	for(int i = 0; i < length; i++)
	{
		destination[i] = source[i];
	}
}
