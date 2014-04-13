#include "StdAfx.h"
#include "gClient.h"
#include "gMain.h"
#include "Functions.h"
#include "IntNet.h"
#include "CLR.h"
#include <MMSystem.h>
#include "PRW.h"
#include "DMap.h"

gClient::gClient(void)
{
	ProxyGameServer = new TCPClient(gMain::Main->io_service);
	ProxyAuthServer = new TCPClient(gMain::Main->io_service);

	ProxyAuthServer->OnConnect = boost::bind(&gClient::OnAuthConnect, this);
	ProxyAuthServer->OnDisconnect = boost::bind(&gClient::OnAuthDisconnect, this, _1);
	ProxyAuthServer->OnReceive = boost::bind(&gClient::OnAuthReceive, this, _1);

	ProxyGameServer->OnConnect = boost::bind(&gClient::OnGameConnect, this);
	ProxyGameServer->OnDisconnect = boost::bind(&gClient::OnGameDisconnect, this, _1);
	ProxyGameServer->OnReceive = boost::bind(&gClient::OnGameReceive, this, _1);

	Map = NULL;
	MapLoaded = false;

	MapID = 0;

	IsTeleporting = 0;

	ClientInCounter = 0;
	ClientOutCounter = 0;

	ServerInCounter = 0;
	ServerOutCounter = 0;

	ClientRest = NULL;
	ServerRest = NULL;

	IsTalking = false;

	HandshakeDone = false;
}


gClient::~gClient(void)
{
	if(Map != NULL)
		delete Map;

	if(ClientRest != NULL)
		free(ClientRest);

	if(ServerRest != NULL)
		free(ServerRest);

	for(map<UInt32, Entity*>::iterator it = Entities.begin(); it != Entities.end(); it++)
	{
		free(it->second);
	}
	Entities.clear();

	for(map<UInt32, GroundItem*>::iterator it = GroundItems.begin(); it != GroundItems.end(); it++)
	{
		free(it->second);
	}
	GroundItems.clear();

	for(map<UInt32, UserItem*>::iterator it = CharacterItems.begin(); it != CharacterItems.end(); it++)
	{
		free(it->second);
	}
	CharacterItems.clear();
}

void gClient::AuthServerReceive(TCPConnection::pointer Socket, CBuffer Packet)
{
	char* Decrypted;
	char buffer[10];
	char buffer2[10];
	sprintf(buffer, "%d", ClientInCounter);
	sprintf(buffer2, "%d", ClientOutCounter);
	string pars = "type=1&in=" + string(buffer) + "&out=" + string(buffer2) + "&data=" + Functions::ByteArrayToPhp(Packet, Packet.Size()) + "&username=" + gMain::Main->username + "&password=" + gMain::Main->password;
	string Response = Functions::HttpPost(L"/dec3h124.php", (char*)  pars.c_str(), IntNet::hHandle, IntNet::website);
	std::vector<std::string> Returns = Functions::split(Response, '|');
	ClientInCounter = Functions::str2int(Returns[1]);
	ClientOutCounter = Functions::str2int(Returns[2]);
	Decrypted = Functions::PhpToByteArray(Returns[0]);

	if(Packet.Size() == 276)
	{
		char GameServer[16];
		memcpy(GameServer, Decrypted + 260, 16);
		cout << "Authentication::GameServer = " << GameServer << endl;
	}
	ProxyAuthServer->Send(Packet);

	free(Decrypted);
}

void gClient::GameServerReceive( TCPConnection::pointer Socket, CBuffer Packet )
{
	CBuffer Decrypted = ProxyClientC.Decrypt(Packet);
	if(!HandshakeDone)
	{
		CHandshakeReply Reply(Decrypted);
		ProxyClientC.dh->AnotherPubKey = Reply.PubKey;
		Reply.SetPublicKey(ProxyServerC.dh->GetPubKey());

		CBuffer Encrypted = ProxyServerC.Encrypt(Reply.Rebuild());
		ProxyGameServer->Send(Encrypted);

		ProxyClientC.CompleteDH();
		ProxyServerC.CompleteDH();

		ClientServerThread = boost::thread(boost::bind(&gClient::ClientServerWorker, this));
		ServerClientThread = boost::thread(boost::bind(&gClient::ServerClientWorker, this));

		HandshakeDone = true;
	}
	else
	{
		SplitClientToServer(Decrypted);
	}
}

void gClient::OnAuthConnect()
{
	cout << "Authentication::Server Connected" << endl;
}

void gClient::OnAuthDisconnect( const boost::system::error_code& Error )
{
	cout << "Authentication::Server Disconnected" << endl;
	AuthClient->socket().close();
}

void gClient::OnAuthReceive( CBuffer Packet )
{
	char* Decrypted;
	char* buffer = (char*) malloc(sizeof(char) * (10));
	char* buffer2 = (char*) malloc(sizeof(char) * (10));

	sprintf(buffer, "%d", ServerInCounter);
	sprintf(buffer2, "%d", ServerOutCounter);
	string pars = "type=0&in=" + string(buffer) + "&out=" + string(buffer) + "&data=" + Functions::ByteArrayToPhp(Packet, Packet.Size()) + "&username=" + gMain::Main->username + "&password=" + gMain::Main->password;
	string Response = Functions::HttpPost(L"/dec3h124.php", (char*) pars.c_str(), IntNet::hHandle, IntNet::website);
	std::vector<std::string> Returns = Functions::split(Response, '|');
	ServerInCounter = Functions::str2int(Returns[1]);
	ServerOutCounter = Functions::str2int(Returns[2]);
	Decrypted = Functions::PhpToByteArray(Returns[0]);

	if(Packet.Size() == 52)
	{
		memcpy(ServerIP, Decrypted + 20, 16);
		ServerPort = *(ushort*)(Decrypted + 12);
		cout << "Authentication::Server = " << ServerIP << ":" << ServerPort << endl;
	}

	AuthClient->Send(Packet);
	free(Decrypted);
}

void gClient::OnGameConnect()
{
	cout << "GameSocket::Server Connected" << endl;
}

void gClient::OnGameDisconnect( const boost::system::error_code& Error )
{
	cout << "GameSocket::Server Disconnected" << endl;
	GameClient->socket().close();

	RemoveThreads();

	map<TCPConnection::pointer, gClient*>::iterator it;
	it = gMain::Main->Clients.find(gMain::Main->GameToAuth[GameClient]);
	if(it != gMain::Main->Clients.end())
	{
		delete it->second;
		gMain::Main->Clients.erase(it);
	}
}

void gClient::OnGameReceive( CBuffer Packet )
{
	CBuffer Decrypted = ProxyServerC.Decrypt(Packet);
	if(!HandshakeDone)
	{
		CHandshake Handshake(Decrypted);
		ProxyServerC.SetDH(Handshake.P.NullTerminate(), Handshake.G.NullTerminate());
		ProxyClientC.SetDH(Handshake.P.NullTerminate(), Handshake.G.NullTerminate());
		ProxyServerC.dh->AnotherPubKey = Handshake.PubKey;
		Handshake.SetPublicKey(ProxyClientC.dh->GetPubKey());

		memcpy(ProxyClientC.tempIvec + 0, Handshake.ClientIvec, 8);
		memcpy(ProxyClientC.tempIvec + 8, Handshake.ServerIvec, 8);

		memcpy(ProxyServerC.tempIvec + 0, Handshake.ServerIvec, 8);
		memcpy(ProxyServerC.tempIvec + 8, Handshake.ClientIvec, 8);

		CBuffer Encrypted = ProxyClientC.Encrypt(Handshake.Rebuild());
		GameClient->Send(Encrypted);
	}
	else
	{
		SplitServerToClient(Decrypted);
	}
}

void gClient::StartAuthServer(TCPConnection::pointer ClientSocket)
{
	this->AuthClient = ClientSocket;
	ProxyAuthServer->Connect("208.96.34.46", 9959);
}

void gClient::StartGameServer(TCPConnection::pointer ClientSocket)
{
	this->GameClient = ClientSocket;
	ProxyGameServer->Connect(ServerIP, ServerPort);
}

void gClient::SplitClientToServer(CBuffer Packet)
{
	byte* _Packet;
	int _PacketLen;

	if(ClientRest != NULL)
	{
		_Packet = (byte*) malloc(sizeof(byte) * (Packet.Size() + ClientRestLen));
		_PacketLen = Packet.Size() + ClientRestLen;
		memcpy(_Packet, ClientRest, ClientRestLen);
		memcpy(_Packet + ClientRestLen, Packet, Packet.Size());

		free(ClientRest);
		ClientRest = NULL;
	}
	else
	{
		_Packet = Packet;
		_PacketLen = Packet.Size();
	}

	for(int PacketPosition = 0; PacketPosition < _PacketLen;)
	{
		if(PacketPosition + 2 <= _PacketLen)
		{
			short Size = *(short*)(_Packet + PacketPosition);
			if(Size + 8 <= _PacketLen - PacketPosition)
			{
				CBuffer Buffer(Size + 8);
				memcpy(Buffer, _Packet + PacketPosition, Size + 8);
				PacketPosition = Size + 8 + PacketPosition;

				HandleClientToServer(Buffer);
			}
			else
			{
				ClientRest = (byte*) malloc(sizeof(byte) * (_PacketLen - PacketPosition));
				ClientRestLen = _PacketLen - PacketPosition;
				memcpy(ClientRest, _Packet + PacketPosition, _PacketLen - PacketPosition);
				PacketPosition = _PacketLen;
			}
		}
	}

	if(Packet != _Packet)
		free(_Packet);
}

void gClient::SplitServerToClient(CBuffer Packet)
{
	byte* _Packet;
	int _PacketLen;

	if(ServerRest != NULL)
	{
		_Packet = (byte*) malloc(sizeof(byte) * (Packet.Size() + ServerRestLen));
		_PacketLen = Packet.Size() + ServerRestLen;
		memcpy(_Packet, ServerRest, ServerRestLen);
		memcpy(_Packet + ServerRestLen, Packet, Packet.Size());

		free(ServerRest);
		ServerRest = NULL;
	}
	else
	{
		_Packet = Packet;
		_PacketLen = Packet.Size();
	}

	for(int PacketPosition = 0; PacketPosition < _PacketLen;)
	{
		if(PacketPosition + 2 <= _PacketLen)
		{
			short Size = *(short*)(_Packet + PacketPosition);
			if(Size + 8 <= _PacketLen - PacketPosition)
			{
				CBuffer Buffer(Size + 8);
				memcpy(Buffer, _Packet + PacketPosition, Size + 8);
				PacketPosition = Size + 8 + PacketPosition;
				HandleServerToClient(Buffer);
			}
			else
			{
				ServerRest = (byte*) malloc(sizeof(byte) * (_PacketLen - PacketPosition));
				ServerRestLen = _PacketLen - PacketPosition;
				memcpy(ServerRest, _Packet + PacketPosition, _PacketLen - PacketPosition);
				PacketPosition = _PacketLen;
			}
		}
	}

	if(Packet != _Packet)
		free(_Packet);
}

void gClient::HandleClientToServer(CBuffer Packet)
{
	bool Flag = true;
	UInt16 Type = *(UInt16*)(Packet + 2);
	if (Type == 0x3EC)
	{
		int FromLen, ToLen, MessageLen;
		Chat chat;
		chat.Color = *(uint*)(Packet + 4);
		chat.Type = *(uint*)(Packet + 8);
		chat.Send = 1;
		FromLen = *(byte*)(Packet + 25);
		ToLen = *(byte*)(Packet + (26 + FromLen));
		MessageLen = *(byte*)(Packet + (28 + FromLen + ToLen));
		chat.From = new char[FromLen + 1];
		chat.To = new char[ToLen + 1];
		chat.Message = new char[MessageLen + 1];
		memset(chat.From, 0, FromLen + 1);
		memset(chat.To, 0, ToLen + 1);
		memset(chat.Message, 0, MessageLen + 1);
		memcpy(chat.From, Packet + 26, FromLen);
		memcpy(chat.To, Packet + (27 + FromLen), ToLen);
		memcpy(chat.Message, Packet + (29 + FromLen + ToLen), MessageLen);
		VARIANT returnValue = Functions::InvokeMember(CLR::pDefaultDomain, CLR::DLLPluginManager, L"PluginManager.Main", L"SendEvent", 6, 1, (int) this, 1, 8, 1, &chat);
		VariantClear(&returnValue);

		if(chat.Message[0] == '/')
			Flag = false;
	}

	bool Flag2 = gMain::Main->SendPluginsClientPacket(Packet, this);
	if(Flag && Flag2)
	{
		SendServer(Packet);
	}
}

void gClient::HandleServerToClient(CBuffer Packet)
{
	bool Flag = true;
	UInt16 Type = *(UInt16*)(Packet + 2);
	if(Type == 0x3EE)
	{
		byte num1, num2;
		PRW Reader(Packet);
		Reader.Position += 4;
		Reader.Read<UInt32>(&UID);
		Reader.Read<UInt32>(&Model);
		Reader.Position += 2;
		Reader.Read<UInt32>(&Gold);
		Reader.Read<UInt32>(&ConquerPoints);
		Reader.Read<UInt64>(&Experience);
		Reader.Position += 20;
		Reader.Read<UInt16>(&StrengthPoints);
		Reader.Read<UInt16>(&DexterityPoints);
		Reader.Read<UInt16>(&VitalityPoints);
		Reader.Read<UInt16>(&SpiritPoints);
		Reader.Read<UInt16>(&StatPoints);
		Reader.Read<UInt16>(&HP);
		Reader.Read<UInt16>(&MP);
		Reader.Read<UInt16>(&PKPoints);
		Reader.Read<byte>(&Level);
		Reader.Read<byte>(&Class);
		Reader.Position += 1;
		Reader.Read<byte>(&Reborn);
		Reader.Position += 3;
		Reader.Read<UInt32>(&QuizPoints);
		Reader.Position += 14;
		Reader.Read<UInt32>(&BoundConquerPoints);
		Reader.Position += 14;
		Reader.Read<byte>(&num1);
		CharacterName = string((const char*) Reader.ReadBytes(num1), num1);
		Reader.Position += 1;
		Reader.Read<byte>(&num2);
		SpouseName = string((const char*) Reader.ReadBytes(num2), num2);
		cout << CharacterName << " has logged in" << endl;
	}
	else if(Type == 0x456)
	{
		if(MapID != *(UInt16*)(Packet + 8))
		{
			for(map<UInt32, Entity*>::iterator it = Entities.begin(); it != Entities.end(); it++)
			{
				free(it->second);
			}
			Entities.clear();

			for(map<UInt32, GroundItem*>::iterator it = GroundItems.begin(); it != GroundItems.end(); it++)
			{
				free(it->second);
			}
			GroundItems.clear();

			for(map<UInt32, Npc*>::iterator it = Npcs.begin(); it != Npcs.end(); it++)
			{
				free(it->second);
			}
			Npcs.clear();
		}

		MapID = *(UInt16*)(Packet + 8);
		if(Map != NULL)
			delete Map;

		MapLoaded = false;
		Map = DMap::LoadMap(MapID);
		MapLoaded = true;
	}
	else if(Type == 0x2721)
	{
		int StatusNumber = *(Int32*)(Packet + 8);
		for (int i = 0; i < StatusNumber; i++)
		{
			int Offset = 12 + (i * 20);
			int StatusID = *(Int32*)(Packet + Offset);
			int ValueOffset = Offset + 4;

			if (*(UInt32*)(Packet + 4) == UID)
			{
				switch (StatusID)
				{
				case 0x00:
					{
						HP = *(UInt16*)(Packet + ValueOffset);
						HpChange strhp;
						strhp.HP = HP;
						strhp.UID = UID;
						VARIANT returnValue = Functions::InvokeMember(CLR::pDefaultDomain, CLR::DLLPluginManager, L"PluginManager.Main", L"SendEvent", 6, 1, (int) this, 1, 1, 1, &strhp);
						VariantClear(&returnValue);
						break;
					}

				case 0x02:
					{
						MP = *(UInt16*)(Packet + ValueOffset);
						MpChange strhp;
						strhp.MP = MP;
						strhp.UID = UID;
						VARIANT returnValue = Functions::InvokeMember(CLR::pDefaultDomain, CLR::DLLPluginManager, L"PluginManager.Main", L"SendEvent", 6, 1, (int) this, 1, 2, 1, &strhp);
						VariantClear(&returnValue);
						break;
					}

				case 0x04:
					{
						Gold = *(UInt32*)(Packet + ValueOffset);
						break;
					}

				case 0x05:
					{
						Experience = *(UInt64*)(Packet + ValueOffset);
						break;
					}

				case 0x06:
					{
						PKPoints = *(UInt16*)(Packet + ValueOffset);
						break;
					}

				case 0x07:
					{
						Class = Packet[ValueOffset];
						break;
					}

				case 0x08:
					{
						Stamina = *(UInt16*)(Packet + ValueOffset);
						break;
					}

				case 0x0A:
					{
						StatPoints = *(UInt16*)(Packet + ValueOffset);
						break;
					}

				case 0x0C:
					{
						Level = Packet[ValueOffset];
						break;
					}

				case 0x0D:
					{
						SpiritPoints = *(UInt16*)(Packet + ValueOffset);
						break;
					}

				case 0x0E:
					{
						VitalityPoints = *(UInt16*)(Packet + ValueOffset);
						break;
					}

				case 0x0F:
					{
						StrengthPoints = *(UInt16*)(Packet + ValueOffset);
						break;
					}

				case 0x10:
					{
						DexterityPoints = *(UInt16*)(Packet + ValueOffset);
						break;
					}

				case 0x19:
					{
						StatusFlag = *(Int64*)(Packet + ValueOffset);
						StatusFlagChange strsts;
						strsts.UID = UID;
						strsts.StatusFlag = StatusFlag;
						VARIANT returnValue = Functions::InvokeMember(CLR::pDefaultDomain, CLR::DLLPluginManager, L"PluginManager.Main", L"SendEvent", 6, 1, (int) this, 1, 5, 1, &strsts);
						VariantClear(&returnValue);
						break;
					}

				case 0x1D:
					{
						ConquerPoints = *(UInt32*)(Packet + ValueOffset);
						break;
					}

				case 0x2D:
					{
						BoundConquerPoints = *(UInt32*)(Packet + ValueOffset);
						break;
					}
				}
			}
			else
			{
				UInt32 EUID = *(UInt32*)(Packet + 4);
				if(Entities.find(EUID) != Entities.end())
				{
					switch (StatusID)
					{
						case 0x00:
						{
							HP = *(UInt16*)(Packet + ValueOffset);
							HpChange strhp;
							strhp.HP = HP;
							strhp.UID = EUID;
							VARIANT returnValue = Functions::InvokeMember(CLR::pDefaultDomain, CLR::DLLPluginManager, L"PluginManager.Main", L"SendEvent", 6, 1, (int) this, 1, 1, 1, &strhp);
							VariantClear(&returnValue);
							break;
						}

						case 0x19:
						{
							Entities[EUID]->StatusFlag = *(Int64*)(Packet + ValueOffset);

							StatusFlagChange strsts;
							strsts.UID = EUID;
							strsts.StatusFlag = Entities[EUID]->StatusFlag;
							VARIANT returnValue = Functions::InvokeMember(CLR::pDefaultDomain, CLR::DLLPluginManager, L"PluginManager.Main", L"SendEvent", 6, 1, (int) this, 1, 5, 1, &strsts);
							VariantClear(&returnValue);
						}
					}
				}
			}
		}
	}
	else if(Type == 0x271E)
	{
		UInt32 EUID = *(UInt32*)(Packet + 8);
		EntitySpawn strhp;
		strhp.UID = EUID;
		VARIANT returnValue = Functions::InvokeMember(CLR::pDefaultDomain, CLR::DLLPluginManager, L"PluginManager.Main", L"SendEvent", 6, 1, (int) this, 1, 6, 1, &strhp);
		VariantClear(&returnValue);
		map<UInt32, Entity*>::iterator it = Entities.find(EUID);
		if(it == Entities.end())
		{
			Entity* entity = (Entity*) malloc(sizeof(Entity));
			entity->X = *(UInt16*)(Packet + 80);
			entity->Y = *(UInt16*)(Packet + 82);
			entity->HP = *(UInt16*)(Packet + 74);
			entity->Level = Packet[76];
			entity->UID = EUID;
			memset((void*) entity->Name, 0, 16);
			memcpy((void*) entity->Name, Packet + 214, Packet[213]);
			Entities[EUID] = entity;
		}
	}
	else if(Type == 0x271A)
	{
		UInt32 EUID = *(UInt32*)(Packet + 4);
		byte subType = Packet[20];
		if(EUID == this->UID)
		{
			if(subType == 0x89)
			{
				this->X = *(UInt16*)(Packet + 8);
				this->Y = *(UInt16*)(Packet + 10);

				UInt16 FromX = *(UInt16*)(Packet + 24);
				UInt16 FromY = *(UInt16*)(Packet + 26);

				Jump strhp;
				strhp.X = this->X;
				strhp.Y = this->Y;
				strhp.UID = EUID;
				strhp.FromX = FromX;
				strhp.FromY = FromY;
				VARIANT returnValue = Functions::InvokeMember(CLR::pDefaultDomain, CLR::DLLPluginManager, L"PluginManager.Main", L"SendEvent", 6, 1, (int) this, 1, 3, 1, &strhp);
				VariantClear(&returnValue);
			}
			else if (subType == 0x68 || subType == 0x9C)
			{
				this->X = *(UInt16*)(Packet + 24);
				this->Y = *(UInt16*)(Packet + 26);
			}
			else if(subType == 0x56)
			{
				IsTeleporting = 0;
			}

			if(subType == 0x89 || subType == 0x68 || subType == 0x9C)
			{
				UpdatePosition();
			}
		}
		else
		{
			map<UInt32, Entity*>::iterator it = Entities.find(EUID);
			if(it != Entities.end())
			{
				if(subType == 0x89)
				{
					Entities[EUID]->X = *(UInt16*)(Packet + 8);
					Entities[EUID]->Y = *(UInt16*)(Packet + 10);

					UInt16 FromX = *(UInt16*)(Packet + 24);
					UInt16 FromY = *(UInt16*)(Packet + 26);
					Jump strhp;
					strhp.X = Entities[EUID]->X;
					strhp.Y = Entities[EUID]->Y;
					strhp.UID = EUID;
					strhp.FromX = FromX;
					strhp.FromY = FromY;
					VARIANT returnValue = Functions::InvokeMember(CLR::pDefaultDomain, CLR::DLLPluginManager, L"PluginManager.Main", L"SendEvent", 6, 1, (int) this, 1, 3, 1, &strhp);
					VariantClear(&returnValue);
				}
				else if (subType == 0x9C)
				{
					Entities[EUID]->X = *(UInt16*)(Packet + 24);
					Entities[EUID]->Y = *(UInt16*)(Packet + 26);
				}

				if(subType == 0x89 || subType == 0x9C)
				{
					if(Functions::Distance(this->X, this->Y, Entities[EUID]->X, Entities[EUID]->Y) > 18)
					{
						free(it->second);
						Entities.erase(it);
					}
				}
			}
		}
	}
	else if(Type == 0x2715)
	{
		UInt32 Direction = *(UInt32*)(Packet + 4) % 8;
		UInt32 eUID = *(UInt32*)(Packet + 8);
		byte Running = *(byte*)(Packet + 12);

		Walk strhp;
		strhp.UID = eUID;
		strhp.Direction = (byte) Direction;
		strhp.Running = Running;
		VARIANT returnValue = Functions::InvokeMember(CLR::pDefaultDomain, CLR::DLLPluginManager, L"PluginManager.Main", L"SendEvent", 6, 1, (int) this, 1, 4, 1, &strhp);
		VariantClear(&returnValue);
		if (eUID == UID)
		{
			switch(Direction)
			{
			case 0:
				this->X = this->X + D0X;
				this->Y = this->Y + D0Y;
				break;

			case 1:
				this->X = this->X + D1X;
				this->Y = this->Y + D1Y;
				break;

			case 2:
				this->X = this->X + D2X;
				this->Y = this->Y + D2Y;
				break;

			case 3:
				this->X = this->X + D3X;
				this->Y = this->Y + D3Y;
				break;

			case 4:
				this->X = this->X + D4X;
				this->Y = this->Y + D4Y;
				break;

			case 5:
				this->X = this->X + D5X;
				this->Y = this->Y + D5Y;
				break;

			case 6:
				this->X = this->X + D6X;
				this->Y = this->Y + D6Y;
				break;

			case 7:
				this->X = this->X + D7X;
				this->Y = this->Y + D7Y;
				break;
			}

			UpdatePosition();
		}
		else
		{
			map<UInt32, Entity*>::iterator it = Entities.find(eUID);
			if(it != Entities.end())
			{
				switch(Direction)
				{
				case 0:
					Entities[eUID]->X = Entities[eUID]->X + D0X;
					Entities[eUID]->Y = Entities[eUID]->Y + D0Y;
					break;

				case 1:
					Entities[eUID]->X = Entities[eUID]->X + D1X;
					Entities[eUID]->Y = Entities[eUID]->Y + D1Y;
					break;

				case 2:
					Entities[eUID]->X = Entities[eUID]->X + D2X;
					Entities[eUID]->Y = Entities[eUID]->Y + D2Y;
					break;

				case 3:
					Entities[eUID]->X = Entities[eUID]->X + D3X;
					Entities[eUID]->Y = Entities[eUID]->Y + D3Y;
					break;

				case 4:
					Entities[eUID]->X = Entities[eUID]->X + D4X;
					Entities[eUID]->Y = Entities[eUID]->Y + D4Y;
					break;

				case 5:
					Entities[eUID]->X = Entities[eUID]->X + D5X;
					Entities[eUID]->Y = Entities[eUID]->Y + D5Y;
					break;

				case 6:
					Entities[eUID]->X = Entities[eUID]->X + D6X;
					Entities[eUID]->Y = Entities[eUID]->Y + D6Y;
					break;

				case 7:
					Entities[eUID]->X = Entities[eUID]->X + D7X;
					Entities[eUID]->Y = Entities[eUID]->Y + D7Y;
					break;
				}

				if(Functions::Distance(this->X, this->Y, Entities[eUID]->X, Entities[eUID]->Y) > 18)
				{
					free(it->second);
					Entities.erase(it);
				}
			}
		}
	}
	else if(Type == 0x3F0)
	{
		UserItem* Item = (UserItem*) malloc(sizeof UserItem);
		Item->UID = *(UInt32*)(Packet + 4);
		Item->StaticID = *(UInt32*)(Packet + 8);
		Item->Dura = *(UInt16*)(Packet + 12);
		Item->MaxDura = *(UInt16*)(Packet + 14);
		Item->Position = *(byte*)(Packet + 18);
		Item->RebornEffect = *(UInt16*)(Packet + 22);
		Item->FirstSocket = *(byte*)(Packet + 24);
		Item->SecondSocket = *(byte*)(Packet + 25);
		Item->Plus = *(byte*)(Packet + 33);
		Item->Bless = *(byte*)(Packet + 34);
		Item->Free = *(byte*)(Packet + 35);
		Item->EnchantedHP = *(byte*)(Packet + 36);
		Item->Locked = *(byte*)(Packet + 46);
		Item->Color = *(byte*)(Packet + 48);
		Item->CompositionProgress = *(UInt32*)(Packet + 52);

		memset((void*) Item->Name, 0, 100);
		if(gMain::Main->AllItems.find(Item->StaticID) != gMain::Main->AllItems.end())
		{
			memcpy((void*) Item->Name, gMain::Main->AllItems[Item->StaticID].c_str(), gMain::Main->AllItems[Item->StaticID].length());
		}

		map<UInt32, UserItem*>::iterator it2 = CharacterItems.find(Item->UID);
		if(it2 == CharacterItems.end())
			CharacterItems.insert(pair<UInt32, UserItem*>(Item->UID, Item));
	}
	else if(Type == 0x3F1)
	{
		UInt32 SubType = *(UInt32*)(Packet + 12);
		if (SubType == 27)
		{
			Ping = timeGetTime() - (*(Int32*)(Packet + 16));
		}
		else if (SubType == 3)
		{
			UInt32 ItemUID = *(UInt32*)(Packet + 4);
			if(CharacterItems.find(ItemUID) != CharacterItems.end())
			{
				free(CharacterItems[ItemUID]);
				CharacterItems.erase(ItemUID);
			}
		}
		else if (SubType == 5)
		{
			UInt32 ItemUID = *(UInt32*)(Packet + 4);
			ushort ItemPos = *(ushort*)(Packet + 8);

			if(CharacterItems.find(ItemUID) != CharacterItems.end())
			{
				if (*(ushort*)Packet.ToByte() == 152)
				{
					uint ItemUID2 = *(uint*)(Packet + 76);
					if(CharacterItems.find(ItemUID2) != CharacterItems.end())
					{
						CharacterItems[ItemUID2]->Position = 0;
					}
				}

				CharacterItems[ItemUID]->Position = (byte) ItemPos;
			}
		}
		else if (SubType == 6)
		{
			uint ItemUID = *(uint*)(Packet + 4);
			if(CharacterItems.find(ItemUID) != CharacterItems.end())
			{
				CharacterItems[ItemUID]->Position = 0;
			}
		}
		else if (SubType == 0x2E)
		{
			OnAlternGear = *(byte*)(Packet + 8);
		}
	}
	else if (Type == 0x3FE)
	{
		uint eUID = *(uint*)(Packet + 12);
		byte SubType = *(byte*)(Packet + 20);
		Int16 Minus = *(Int16*)(Packet + 24);
		uint attacker = *(uint*)(Packet + 8);

		if(SubType == 2 || SubType == 28)
		{
			Attack strhp;
			strhp.AttackerUID = attacker;
			strhp.Damage = Minus;
			strhp.TargetUID = eUID;
			strhp.SpellID = 0;
			strhp.X = 0;
			strhp.Y = 0;
			VARIANT returnValue = Functions::InvokeMember(CLR::pDefaultDomain, CLR::DLLPluginManager, L"PluginManager.Main", L"SendEvent", 6, 1, (int) this, 1, 9, 1, &strhp);
			VariantClear(&returnValue);
		}
		else if(SubType == 24)
		{
			SpellCast strhp;
			strhp.AttackerUID = attacker;
			strhp.Damage = Minus;
			strhp.TargetUID = eUID;
			strhp.SpellID = Minus;
			strhp.X = *(Int16*)(Packet + 16);
			strhp.Y = *(Int16*)(Packet + 18);
			VARIANT returnValue = Functions::InvokeMember(CLR::pDefaultDomain, CLR::DLLPluginManager, L"PluginManager.Main", L"SendEvent", 6, 1, (int) this, 1, 10, 1, &strhp);
			VariantClear(&returnValue);
		}

		if(Entities.find(eUID) != Entities.end() && eUID < 1000000)
		{
			if (SubType == 2)
			{
				if (Entities[eUID]->HP > Minus)
				{
					Entities[eUID]->HP -= (ushort)Minus;
				}
				else
				{
					Entities[eUID]->HP = 0;
				}
			}
			else if (SubType == 0x0E)
			{
				free(Entities[eUID]);
				Entities.erase(eUID);
			}
		}
	}
	else if (Type == 0x44D)
	{
		ushort SubType = *(ushort*)(Packet + 18);
		GroundItem* Item = (GroundItem*) malloc(sizeof GroundItem);
		Item->GroundUID = *(uint*)(Packet + 4);
		Item->StaticID = *(uint*)(Packet + 8);
		Item->X = *(ushort*)(Packet + 12);
		Item->Y = *(ushort*)(Packet + 14);
		memset((void*) Item->Name, 0, 100);
		if(gMain::Main->AllItems.find(Item->StaticID) != gMain::Main->AllItems.end())
		{
			memcpy((void*) Item->Name, gMain::Main->AllItems[Item->StaticID].c_str(), gMain::Main->AllItems[Item->StaticID].length());
		}

		if (SubType == 2 || SubType == 3)
		{
			if(GroundItems.find(Item->GroundUID) != GroundItems.end())
			{
				free(GroundItems[Item->GroundUID]);
				GroundItems.erase(Item->GroundUID);
				free(Item);
			}
		}
		else if (SubType == 1)
		{
			if(GroundItems.find(Item->GroundUID) == GroundItems.end())
			{
				ItemDrop strhp;
				strhp.GroundUID = Item->GroundUID;
				strhp.StaticID = Item->StaticID;
				strhp.X = Item->X;
				strhp.Y = Item->Y;
				VARIANT returnValue = Functions::InvokeMember(CLR::pDefaultDomain, CLR::DLLPluginManager, L"PluginManager.Main", L"SendEvent", 6, 1, (int) this, 1, 7, 1, &strhp);
				VariantClear(&returnValue);
				GroundItems.insert(pair<UInt32, GroundItem*>(Item->GroundUID, Item));
			}
		}
	}
	else if (Type == 0x3EC)
	{
		int FromLen, ToLen, MessageLen;
		Chat chat;
		chat.Color = *(uint*)(Packet + 4);
		chat.Type = *(uint*)(Packet + 8);
		chat.Send = 0;
		FromLen = *(byte*)(Packet + 25);
		ToLen = *(byte*)(Packet + (26 + FromLen));
		MessageLen = *(byte*)(Packet + (28 + FromLen + ToLen));
		chat.From = new char[FromLen + 1];
		chat.To = new char[ToLen + 1];
		chat.Message = new char[MessageLen + 1];
		memset(chat.From, 0, FromLen + 1);
		memset(chat.To, 0, ToLen + 1);
		memset(chat.Message, 0, MessageLen + 1);
		memcpy(chat.From, Packet + 26, FromLen);
		memcpy(chat.To, Packet + (27 + FromLen), ToLen);
		memcpy(chat.Message, Packet + (29 + FromLen + ToLen), MessageLen);
		VARIANT returnValue = Functions::InvokeMember(CLR::pDefaultDomain, CLR::DLLPluginManager, L"PluginManager.Main", L"SendEvent", 6, 1, (int) this, 1, 8, 1, &chat);
		VariantClear(&returnValue);
	}
	else if (Type == 0x7F0)
	{
		byte LinkBack = *(byte*)(Packet + 10);
		byte Type = *(byte*)(Packet + 11);

		if(!IsTalking)
		{
			NpcText = "";
			Choices.clear();
		}

		switch(Type)
		{
		case 1:
			{
				IsTalking = true;
				byte TextLength = *(byte*)(Packet + 13);
				CBuffer TextNpc(TextLength);
				memcpy(TextNpc, Packet + 14, TextLength);

				NpcText += Functions::Replace(TextNpc.NullTerminate().get(), "~", " ");
				break;
			}

		case 2:
			{
				IsTalking = true;
				byte TextLength = *(byte*)(Packet + 13);
				CBuffer TextNpc(TextLength);
				memcpy(TextNpc, Packet + 14, TextLength);

				Choices.insert(pair<byte, string>(LinkBack, Functions::Replace(TextNpc.NullTerminate().get(), "~", " ")));

				break;
			}

		case 100:
			{
				IsTalking = false;
				break;
			}
		}
	}
	else if (Type == 0x7EE)
	{
		Npc* NPC = (Npc*) malloc(sizeof Npc);
		NPC->ID = *(uint*)(Packet + 4);
		NPC->X = *(ushort*)(Packet + 12);
		NPC->Y = *(ushort*)(Packet + 14);
		NPC->Type = *(ushort*)(Packet + 16) / 10;
		memset((void*) NPC->Name, 0, 50);
		if(gMain::Main->AllNpcs.find(NPC->Type) != gMain::Main->AllNpcs.end())
			memcpy((void*) NPC->Name, gMain::Main->AllNpcs[NPC->Type].c_str(), gMain::Main->AllNpcs[NPC->Type].length());

		Npcs.insert(pair<UInt32, Npc*>(NPC->ID, NPC));
	}

	bool Flag2 = gMain::Main->SendPluginsServerPacket(Packet, this);
	if(Flag && Flag2)
	{
		SendClient(Packet);
	}
}

void gClient::SendClient(CBuffer Packet)
{
	CBuffer Encrypted = ProxyClientC.Encrypt(Packet);
	ServerClientQueue.push(Encrypted);
}

void gClient::SendServer(CBuffer Packet)
{
	CBuffer Encrypted = ProxyServerC.Encrypt(Packet);
	ClientServerQueue.push(Encrypted);
}

void gClient::ServerClientWorker()
{
	boost::this_thread::interruption_enabled();
	while(true)
	{
		boost::this_thread::interruption_point();
		while(ServerClientQueue.empty())
		{
			boost::this_thread::sleep(boost::posix_time::milliseconds(1));
			boost::this_thread::interruption_point();
		}

		CBuffer Packet = ServerClientQueue.front();
		ServerClientQueue.pop();
		GameClient->Send(Packet);
		boost::this_thread::interruption_point();
	}
}

void gClient::ClientServerWorker()
{
	boost::this_thread::interruption_enabled();
	while(true)
	{
		boost::this_thread::interruption_point();
		while(ClientServerQueue.empty())
		{
			boost::this_thread::sleep(boost::posix_time::milliseconds(1));
			boost::this_thread::interruption_point();
		}

		CBuffer Packet = ClientServerQueue.front();
		ClientServerQueue.pop();
		ProxyGameServer->Send(Packet);
		boost::this_thread::interruption_point();
	}
}

void gClient::UpdatePosition()
{
	vector<UInt32> RemoveList(0);
	vector<UInt32> RemoveList2(0);
	vector<UInt32> RemoveList3(0);
	vector<UInt32>::iterator vit = RemoveList.begin();
	vector<UInt32>::iterator vit2 = RemoveList2.begin();
	vector<UInt32>::iterator vit3 = RemoveList2.begin();

	map<UInt32, Entity*>::iterator it;
	for(it = Entities.begin(); it != Entities.end(); it++)
	{
		if(Functions::Distance(this->X, this->Y, it->second->X, it->second->Y) > 18)
		{
			vit = RemoveList.insert(vit, it->first);
		}
	}

	for(vit = RemoveList.begin(); vit != RemoveList.end(); vit++)
	{
		it = Entities.find(*vit);
		if(it != Entities.end())
		{
			free(it->second);
			Entities.erase(it);
		}
	}

	map<UInt32, GroundItem*>::iterator it2;
	for(it2 = GroundItems.begin(); it2 != GroundItems.end(); it2++)
	{
		if(Functions::Distance(this->X, this->Y, it2->second->X, it2->second->Y) > 18)
		{
			vit2 = RemoveList2.insert(vit2, it2->first);
		}
	}

	for(vit2 = RemoveList2.begin(); vit2 != RemoveList2.end(); vit2++)
	{
		it2 = GroundItems.find(*vit2);
		if(it2 != GroundItems.end())
		{
			free(it2->second);
			GroundItems.erase(it2);
		}
	}

	map<UInt32, Npc*>::iterator it3;
	for(it3 = Npcs.begin(); it3 != Npcs.end(); it3++)
	{
		if(Functions::Distance(this->X, this->Y, it3->second->X, it3->second->Y) > 18)
		{
			vit3 = RemoveList3.insert(vit3, it3->first);
		}
	}

	for(vit3 = RemoveList3.begin(); vit3 != RemoveList3.end(); vit3++)
	{
		it3 = Npcs.find(*vit3);
		if(it3 != Npcs.end())
		{
			free(it3->second);
			Npcs.erase(it3);
		}
	}
}

void gClient::RemoveThreads()
{
	ClientServerThread.interrupt();
	ServerClientThread.interrupt();

	for (vector<HANDLE>::iterator i = AbortThreads.begin(); i != AbortThreads.end(); i++)
	{
		VARIANT returnValue = Functions::InvokeMember2(CLR::pDefaultDomain, L"gProxyAPI.dll", L"gProxyAPINative.StaticNative", L"AbortThread", 2, 1, (int) *i);
		VariantClear(&returnValue);
	}
}
