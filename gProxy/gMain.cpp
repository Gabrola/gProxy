 #include "StdAfx.h"
#include "gMain.h"
#include "Functions.h"
#include "IntNet.h"
#include "CLR.h"
#include <iomanip>
#include <atlstr.h>
#include "IniReader.h"
#include "IniWriter.h"
#include "DatEncrypt.h"
#include "DMap.h"
#include <Mmsystem.h>

gMain* gMain::Main;
struct Language
{
	WORD m_wLanguage;
	WORD m_wCodePage;
};

gMain::gMain(void)
{
	ProxyAuthClients = new TCPServer(io_service, 9959);
	ProxyGameClients = new TCPServer(io_service, 5816);

	ProxyAuthClients->OnConnect = boost::bind(&gMain::OnAuthConnect, this, _1);
	ProxyAuthClients->OnDisconnect = boost::bind(&gMain::OnAuthDisconnect, this, _1, _2);
	ProxyAuthClients->OnReceive = boost::bind(&gMain::OnAuthReceive, this, _1, _2);

	ProxyGameClients->OnConnect = boost::bind(&gMain::OnGameConnect, this, _1);
	ProxyGameClients->OnDisconnect = boost::bind(&gMain::OnGameDisconnect, this, _1, _2);
	ProxyGameClients->OnReceive = boost::bind(&gMain::OnGameReceive, this, _1, _2);

	CIniReader Reader("./Settings.ini");
	char* UserString = Reader.ReadString("Settings", "GProxyUsername", "NONE");
	char* PassString = Reader.ReadString("Settings", "GProxyPassword", "NONE");

	if(string(UserString).compare("NONE") != 0)
	{
		username = UserString;
		password = md5(PassString);
		goto Validate;
	}

	EnterStuff:
	cout << "Enter gProxy Username: ";
	cin >> username;
	char* pass = Functions::read_pass("Password: ");
	password = md5(pass);

	Validate:
	string pars = "username=" + username + "&password=" + password;
	string Response = Functions::HttpPost(L"/log8fv182.php", (char*) pars.c_str(), IntNet::hHandle, IntNet::website);

	if(Response == "")
	{
		cout << "A problem has occured while connecting to the gProxy authentication server\nPress any key to retry..." << endl;
		_getch();
		system("CLS");
		goto Validate;
	}
	else if(Response == "ACCESS DENIED")
	{
		cout << "Invalid username/password combination or unactivated account\nPress any key to retry..." << endl;
		_getch();
		system("CLS");
		goto EnterStuff;
	}
	else if(Response == "AUTHORIZED")
	{
		cout << "Thank you for purchasing gProxy" << endl;
		Sleep(1000);
		system("CLS");

		if(string(UserString).compare(username) == 0 && string(md5(PassString)).compare(password) == 0)
		{
			goto Finished;
		}
	}
	else
	{
		cout << "A problem has occured while connecting to the gProxy authentication server\nPress any key to retry..." << endl;
		_getch();
		system("CLS");
		goto EnterStuff;
	}

	cout << "Save username and password? y/n: ";
	char yesorno = _getch();
	if(yesorno == 'y' || yesorno == 'Y')
	{
		CIniWriter Writer("./Settings.ini");
		Writer.WriteString("Settings", "GProxyUsername", (char*) username.c_str());
		Writer.WriteString("Settings", "GProxyPassword", (char*) pass);
	}

	cout << endl;
	free(pass);

	Finished:
	delete[] UserString;
	delete[] PassString;
	string version = Functions::HttpGet(L"/version.dat", IntNet::hHandle, IntNet::website);
	Language *pLanguageInfo = NULL;
	LPVOID pvResourceData = NULL;

	// Allocate language structure
	pLanguageInfo = new Language;
	if(pLanguageInfo)
	{
		// Clear structure
		memset(pLanguageInfo, 0, sizeof(Language));

		// Get version info
		DWORD dwHandle = 0;

		DWORD dwResourceSize = ::GetFileVersionInfoSize(L"gProxy.exe", &dwHandle);
		if(dwResourceSize)
		{
			pvResourceData = malloc(dwResourceSize);
			if(pvResourceData)
				if(::GetFileVersionInfo(L"gProxy.exe", 0, dwResourceSize, pvResourceData) != FALSE)
				{
					UINT uiSize = 0;
					VerQueryValue(pvResourceData, L"\\VarFileInfo\\Translation", reinterpret_cast<LPVOID *>(&pLanguageInfo), &uiSize);
				}
		}
	}

	// Get specific info
	UINT uiSize = 0;
	LPVOID pvValue = NULL;
	stringstream strsQueryString;

	strsQueryString << "\\StringFileInfo\\" << setw(4) << setfill('0') << hex << pLanguageInfo->m_wLanguage
		<< setw(4) << pLanguageInfo->m_wCodePage << "\\ProductVersion";

	VerQueryValue(pvResourceData,
		CString(strsQueryString.str().c_str()),
		static_cast<LPVOID *>(&pvValue),
		&uiSize);
	
	if(CString(static_cast<LPTSTR>(pvValue)).Compare(CString(version.c_str())) == 0)
	{
		cout << "gProxy is up to date" << endl;
	}
	else
	{
		cout << "gProxy is not up to date\nCurrent Version: " << version << "\nYour version: ";
		wcout << CString(static_cast<LPTSTR>(pvValue)).GetBuffer(uiSize) << endl;
		cout << "Starting updater!" << endl;
		Sleep(2000);
		system(string("\"gProxyUpdater.exe\" " + username + " " + password).c_str());
		exit(0);
	}

	free(pvResourceData);

	CheckPath:
	char* COPath = Reader.ReadString("Settings", "COPath", "NONE");
	if(string(COPath).compare("NONE") != 0)
	{
		if(GetFileAttributesA(COPath) == FILE_ATTRIBUTE_DIRECTORY)
		{
			cout << COPath << " is a valid directory" << endl;
			delete[] COPath;
		}
		else
		{
			cout << COPath << " is not a valid directory" << endl;
			cout << "Please enter your CO path: ";
			string ThePath;
			cin >> ThePath;
			cout << endl;

			CIniWriter Writer("./Settings.ini");
			Writer.WriteString("Settings", "COPath", (char*) ThePath.c_str());
			system("CLS");
			delete[] COPath;
			goto CheckPath;
		}
	}
	else
	{
		cout << "Please enter your CO path: ";
		string ThePath;
		cin >> ThePath;
		cout << endl;

		CIniWriter Writer("./Settings.ini");
		Writer.WriteString("Settings", "COPath", (char*) ThePath.c_str());
		system("CLS");
		delete[] COPath;
		goto CheckPath;
	}

	CONPath = Reader.ReadString("Settings", "COPath", "NONE");

	cout << "Loading Items... ";
	int Time = timeGetTime();
	ifstream file (CONPath + "\\ini\\itemtype.dat", ios::in|ios::binary|ios::ate);
	ifstream::pos_type size;
	char* memblock;
	if (file.is_open())
	{
		size = file.tellg();
		memblock = new char [size];
		file.seekg (0, ios::beg);
		file.read (memblock, size);
		file.close();
		DatEncrypt::DecryptData((byte*) memblock, size);
		vector<string> Lines = Functions::split(string(memblock), '\n');
		for(int i = 1; i < Lines.size(); i++)
		{
			Splitter Splitz(Lines[i], "@@");
			if(Splitz.size() >= 2)
			{
				UInt32 SID = (UInt32) Functions::str2int(Splitz[0]);
				string iName = Splitz[1];
				AllItems.insert(pair<UInt32, string>(SID, iName));
			}
		}

		delete[] memblock;
	}
	cout << (timeGetTime() - Time) << "ms" << endl;
	Time = timeGetTime();

	cout << "Loading NPCs... ";
	file = ifstream(CONPath + "\\ini\\npc.ini", ios::in|ios::binary|ios::ate);
	if (file.is_open())
	{
		size = file.tellg();
		memblock = new char [size];
		file.seekg (0, ios::beg);
		file.read (memblock, size);
		file.close();
		vector<string> Lines = Functions::split(string(memblock), '\n');
		for(int i = 0; i < Lines.size(); i++)
		{
			if(Lines[i].c_str()[0] == '[')
			{
				string MeshID = Lines[i].substr(8, Lines[i].length() - 9);
				i++;
				string Name = Lines[i].substr(5, Lines[i].length() - 5);

				AllNpcs.insert(pair<UInt16, string>(atoi(MeshID.c_str()), Name));
			}
		}

		delete[] memblock;
	}
	file = ifstream(CONPath + "\\ini\\NpcX.ini", ios::in|ios::binary|ios::ate);
	if (file.is_open())
	{
		size = file.tellg();
		memblock = new char [size];
		file.seekg (0, ios::beg);
		file.read (memblock, size);
		file.close();
		vector<string> Lines = Functions::split(string(memblock), '\n');
		for(int i = 0; i < Lines.size(); i++)
		{
			if(Lines[i].c_str()[0] == '[')
			{
				string SectionName = Lines[i].substr(1, Lines[i].length() - 2);
				i++;
				string Name = Lines[i].substr(5, Lines[i].length() - 5);

				AllNpcs.insert(pair<UInt16, string>(atoi(SectionName.c_str()), Name));
			}
		}

		delete[] memblock;
	}
	cout << (timeGetTime() - Time) << "ms" << endl;
	Time = timeGetTime();

	cout << "Loading Maps... ";
	DMap::LoadMaps(CONPath + "\\ini\\GameMap.dat");
	cout << (timeGetTime() - Time) << "ms" << endl;
	Time = timeGetTime();

	cout << "Loading Plugins... ";
	VARIANT returnValue = Functions::InvokeMember(CLR::pDefaultDomain, CLR::DLLPluginManager, L"PluginManager.Main", L"LoadPlugins", 0);
	cout << (timeGetTime() - Time) << "ms" << endl;

	ClientThread = boost::thread(boost::bind(&boost::asio::io_service::run, &io_service));
	cout << "gProxy is running!" << endl;

	VariantClear(&returnValue);
}


gMain::~gMain(void)
{
}

void gMain::OnAuthConnect( TCPConnection::pointer Socket )
{
	cout << "Authentication::Client Connected" << endl;
	Clients.insert(pair<TCPConnection::pointer, gClient*>(Socket, new gClient()));
	Clients[Socket]->StartAuthServer(Socket);
}

void gMain::OnAuthDisconnect( TCPConnection::pointer Socket, const boost::system::error_code& Error )
{
	cout << "Authentication::Client Disconnected" << endl;
	map<TCPConnection::pointer, TCPConnection::pointer>::iterator it;
	bool found = false;
	for ( it=GameToAuth.begin() ; it != GameToAuth.end(); it++ )
	{
		if(it->second == Socket)
		{
			found = true;
		}
	}

	if(!found)
		Clients.erase(Clients.find(Socket));
}

void gMain::OnAuthReceive( TCPConnection::pointer Socket, CBuffer Packet )
{
	Clients[Socket]->AuthServerReceive(Socket, Packet);
}

void gMain::OnGameConnect( TCPConnection::pointer Socket )
{
	cout << "GameSocket::Client Connected" << endl;
	map<TCPConnection::pointer, gClient*>::iterator it;
	it = Clients.end();
	it--;
	GameToAuth.insert(pair<TCPConnection::pointer, TCPConnection::pointer>(Socket, it->first));
	Clients[GameToAuth[Socket]]->StartGameServer(Socket);
}

void gMain::OnGameDisconnect( TCPConnection::pointer Socket, const boost::system::error_code& Error )
{
	cout << "GameSocket::Client Disconnected" << endl;
	map<TCPConnection::pointer, gClient*>::iterator it;
	it = Clients.find(GameToAuth[Socket]);
	if(it != Clients.end())
	{
		it->second->RemoveThreads();
		it->second->ProxyGameServer->Close();
		delete it->second;
		Clients.erase(it);
	}
}

void gMain::OnGameReceive( TCPConnection::pointer Socket, CBuffer Packet )
{
	Clients[GameToAuth[Socket]]->GameServerReceive(Socket, Packet);
}


bool gMain::SendPluginsClientPacket(CBuffer Packet, gClient* Instance)
{
	string HexPacket = "";
	char* InstString = (char*) malloc(sizeof(char) * (9));
	sprintf(InstString, "%08X", (int) Instance);
	HexPacket += Functions::ByteArrayToHex(Packet, Packet.Size()) + "-" + InstString;

	wchar_t* wText = Functions::StringToWide(HexPacket);
	VARIANT returnValue = Functions::InvokeMember(CLR::pDefaultDomain, CLR::DLLPluginManager, L"PluginManager.Main", L"ReceiveClientPacket", 2, 0, wText);
	VariantClear(&returnValue);
	
	free(InstString);
	free(wText);

	if(returnValue.intVal == 1)
		return true;
	else
		return false;
}

bool gMain::SendPluginsServerPacket(CBuffer Packet, gClient* Instance)
{
	string HexPacket = "";
	char* InstString = (char*) malloc(sizeof(char) * (9));
	sprintf(InstString, "%08X", (int) Instance);
	HexPacket += Functions::ByteArrayToHex(Packet, Packet.Size()) + "-" + InstString;

	wchar_t* wText = Functions::StringToWide(HexPacket);
	VARIANT returnValue = Functions::InvokeMember(CLR::pDefaultDomain, CLR::DLLPluginManager, L"PluginManager.Main", L"ReceiveServerPacket", 2, 0, wText);
	VariantClear(&returnValue);
	
	free(InstString);
	free(wText);

	if(returnValue.intVal == 1)
		return true;
	else
		return false;
}

void* gMain::GetClients( int* num )
{
	int* clients = new int[Clients.size()];
	map<TCPConnection::pointer, gClient*>::iterator it;
	int i = 0;
	for(it = Clients.begin(); it != Clients.end(); it++)
	{
		clients[i] = (int) it->second;
		i++;
	}
	*num = Clients.size();

	return clients;
}
