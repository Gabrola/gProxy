#pragma once
#include "gClient.h"
#include "TCPServer.h"
#include "TCPConnection.h"

class gMain
{
private:

	TCPServer* ProxyAuthClients;
	TCPServer* ProxyGameClients;

	void OnAuthConnect(TCPConnection::pointer Socket);
	void OnAuthDisconnect(TCPConnection::pointer Socket, const boost::system::error_code& Error);
	void OnAuthReceive(TCPConnection::pointer Socket, CBuffer Packet);

	void OnGameConnect(TCPConnection::pointer Socket);
	void OnGameDisconnect(TCPConnection::pointer Socket, const boost::system::error_code& Error);
	void OnGameReceive(TCPConnection::pointer Socket, CBuffer Packet);

	boost::thread ClientThread;

public:
	gMain(void);
	~gMain(void);

	string CONPath;

	map<TCPConnection::pointer, gClient*> Clients;
	map<TCPConnection::pointer, TCPConnection::pointer> GameToAuth;
	ICorRuntimeHost *pClrHost;
	string username;
	string password;
	bool SendPluginsClientPacket(CBuffer Packet, gClient* Instance);
	bool SendPluginsServerPacket(CBuffer Packet, gClient* Instance);

	static gMain* Main;
	void* GetClients(int* num);

	map<UInt32, string> AllItems;
	map<UInt16, string> AllNpcs;
	boost::asio::io_service io_service;
};

