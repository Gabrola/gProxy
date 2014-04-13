#pragma once
#include "Structs.h"
#include "GameCryptography.h"
#include "Handshake.h"
#include "HandshakeReply.h"
#include "TCPClient.h"
#include "TCPConnection.h"
#include <queue>

class gMain;
class gClient
{
private:
// 	gClientSocket* AuthClient;
// 	gClientSocket* GameClient;

	TCPClient* ProxyAuthServer;

	TCPConnection::pointer AuthClient;
	TCPConnection::pointer GameClient;

	int ClientInCounter;
	int ClientOutCounter;
	int ServerInCounter;
	int ServerOutCounter;

	byte* ClientRest;
	byte* ServerRest;

	int ClientRestLen;
	int ServerRestLen;

	void OnAuthConnect();
	void OnAuthDisconnect(const boost::system::error_code& Error);
	void OnAuthReceive(CBuffer Packet);

	void OnGameConnect();
	void OnGameDisconnect(const boost::system::error_code& Error);
	void OnGameReceive(CBuffer Packet);

	void SplitClientToServer(CBuffer Packet);
	void SplitServerToClient(CBuffer Packet);
	void HandleClientToServer(CBuffer Packet);
	void HandleServerToClient(CBuffer Packet);

	UInt16 ServerPort;

// 	SOCKET AuthClientSocket;
// 	SOCKET GameClientSocket;

	CGameCryptography ProxyServerC;
	CGameCryptography ProxyClientC;

	bool HandshakeDone;

	queue<CBuffer> ServerClientQueue;
	queue<CBuffer> ClientServerQueue;

	void ServerClientWorker();
	void ClientServerWorker();

	boost::thread ClientServerThread;
	boost::thread ServerClientThread;

public:
	TCPClient* ProxyGameServer;
	gClient(void);
	~gClient(void);

	char ServerIP[16];

	void AuthServerReceive(TCPConnection::pointer Socket, CBuffer Packet);
	void GameServerReceive(TCPConnection::pointer Socket, CBuffer Packet);

	void StartAuthServer(TCPConnection::pointer ClientSocket);
	void StartGameServer(TCPConnection::pointer ClientSocket);

	void SendServer(CBuffer Packet);
	void SendClient(CBuffer Packet);

	void UpdatePosition();

	void RemoveThreads();

	UInt32 UID;
	UInt32 Model;
	UInt32 Gold;
	UInt32 ConquerPoints;
	UInt32 BoundConquerPoints;
	UInt64 Experience;
	UInt16 StrengthPoints;
	UInt16 DexterityPoints;
	UInt16 VitalityPoints;
	UInt16 SpiritPoints;
	UInt16 StatPoints;
	UInt16 HP;
	UInt16 MP;
	UInt16 PKPoints;
	byte Level;
	byte Class;
	byte Reborn;
	UInt32 QuizPoints;
	string CharacterName;
	string SpouseName;

	UInt16 MapID;
	UInt16 Stamina;
	Int64 StatusFlag;

	UInt16 X;
	UInt16 Y;

	map<UInt32, Entity*> Entities;
	map<UInt32, GroundItem*> GroundItems;
	map<UInt32, UserItem*> CharacterItems;
	map<UInt32, Npc*> Npcs;

	UInt32 Ping;
	byte OnAlternGear;

	gMap* Map;
	bool MapLoaded;

	bool IsTalking;
	string NpcText;
	map<byte, string> Choices;
	byte IsTeleporting;
	vector<HANDLE> AbortThreads;
};

