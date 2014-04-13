#include "StdAfx.h"
#include "gClientNative.h"
#include "gClient.h"
#include "gMain.h"

gClientNative::gClientNative(void)
{
}


gClientNative::~gClientNative(void)
{
}

void gClientNative::SendClient(void* Client, CBuffer Packet)
{
	((gClient*) Client)->SendClient(Packet);
}

void gClientNative::SendServer(void* Client, CBuffer Packet)
{
	((gClient*) Client)->SendServer(Packet);
}

void* gClientNative::GetClients(int* num)
{
	return gMain::Main->GetClients(num);
}

void* gClientNative::GetEntities(void* Client, int* num)
{
	gClient* client = (gClient*) Client;
	int* clients = new int[client->Entities.size()];
	map<UInt32, Entity*>::iterator it;
	int i = 0;
	for(it = client->Entities.begin(); it != client->Entities.end(); it++)
	{
		clients[i] = (int) it->second;
		i++;
	}
	*num = client->Entities.size();

	return clients;
}

void* gClientNative::GetNpcs(void* Client, int* num)
{
	gClient* client = (gClient*) Client;
	int* clients = new int[client->Npcs.size()];
	map<UInt32, Npc*>::iterator it;
	int i = 0;
	for(it = client->Npcs.begin(); it != client->Npcs.end(); it++)
	{
		clients[i] = (int) it->second;
		i++;
	}
	*num = client->Npcs.size();

	return clients;
}

void* gClientNative::GetCharItems(void* Client, int* num)
{
	gClient* client = (gClient*) Client;
	int* items = new int[client->CharacterItems.size()];
	map<UInt32, UserItem*>::iterator it;
	int i = 0;
	for(it = client->CharacterItems.begin(); it != client->CharacterItems.end(); it++)
	{
		items[i] = (int) it->second;
		i++;
	}
	*num = client->CharacterItems.size();

	return items;
}

void* gClientNative::GetGroundItems(void* Client, int* num)
{
	gClient* client = (gClient*) Client;
	int* items = new int[client->GroundItems.size()];
	map<UInt32, GroundItem*>::iterator it;
	int i = 0;
	for(it = client->GroundItems.begin(); it != client->GroundItems.end(); it++)
	{
		items[i] = (int) it->second;
		i++;
	}
	*num = client->GroundItems.size();

	return items;
}

void* gClientNative::GetNpcChoicesNum( void* Client, int* num )
{
	gClient* client = (gClient*) Client;
	byte* items = new byte[client->Choices.size()];
	int i = 0;
	for(map<byte, string>::iterator it = client->Choices.begin(); it != client->Choices.end(); it++)
	{
		items[i] = it->first;
		i++;
	}
	*num = client->Choices.size();

	return items;
}

void* gClientNative::GetNpcChoicesText( void* Client, int* num )
{
	gClient* client = (gClient*) Client;
	char** items = new char*[client->Choices.size()];
	int i = 0;
	for(map<byte, string>::iterator it = client->Choices.begin(); it != client->Choices.end(); it++)
	{
		items[i] = (char*) it->second.c_str();
		i++;
	}
	*num = client->Choices.size();

	return items;
}


void* gClientNative::GetMap( void* Client )
{
	return ((gClient*) Client)->Map;
}

void gClientNative::GetCharacterName( void* Client, const char* buffer )
{
	memcpy((void*) buffer, ((gClient*) Client)->CharacterName.c_str(), ((gClient*) Client)->CharacterName.length());
}

void gClientNative::GetSpouseName( void* Client, const char* buffer )
{
	memcpy((void*) buffer, ((gClient*) Client)->SpouseName.c_str(), ((gClient*) Client)->SpouseName.length());
}

UInt32 gClientNative::GetUID( void* Client )
{
	return ((gClient*) Client)->UID;
}

UInt32 gClientNative::GetModel( void* Client )
{
	return ((gClient*) Client)->Model;
}

UInt32 gClientNative::GetGold( void* Client )
{
	return ((gClient*) Client)->Gold;
}

UInt32 gClientNative::GetConquerPoints( void* Client )
{
	return ((gClient*) Client)->ConquerPoints;
}

UInt32 gClientNative::GetBoundConquerPoints( void* Client )
{
	return ((gClient*) Client)->BoundConquerPoints;
}

UInt64 gClientNative::GetExperience( void* Client )
{
	return ((gClient*) Client)->Experience;
}

UInt16 gClientNative::GetStrengthPoints( void* Client )
{
	return ((gClient*) Client)->StrengthPoints;
}

UInt16 gClientNative::GetDexterityPoints( void* Client )
{
	return ((gClient*) Client)->DexterityPoints;
}

UInt16 gClientNative::GetVitalityPoints( void* Client )
{
	return ((gClient*) Client)->VitalityPoints;
}

UInt16 gClientNative::GetSpiritPoints( void* Client )
{
	return ((gClient*) Client)->SpiritPoints;
}

UInt16 gClientNative::GetStatPoints( void* Client )
{
	return ((gClient*) Client)->StatPoints;
}

UInt16 gClientNative::GetHP( void* Client )
{
	return ((gClient*) Client)->HP;
}

UInt16 gClientNative::GetMP( void* Client )
{
	return ((gClient*) Client)->MP;
}

UInt16 gClientNative::GetPKPoints( void* Client )
{
	return ((gClient*) Client)->PKPoints;
}

byte gClientNative::GetLevel( void* Client )
{
	return ((gClient*) Client)->Level;
}

byte gClientNative::GetClass( void* Client )
{
	return ((gClient*) Client)->Class;
}

byte gClientNative::GetReborn( void* Client )
{
	return ((gClient*) Client)->Reborn;
}

UInt32 gClientNative::GetQuizPoints( void* Client )
{
	return ((gClient*) Client)->QuizPoints;
}

UInt16 gClientNative::GetMapID( void* Client )
{
	return ((gClient*) Client)->MapID;
}


UInt16 gClientNative::GetStamina( void* Client )
{
	return ((gClient*) Client)->Stamina;
}


UInt64 gClientNative::GetStatusFlag( void* Client )
{
	return ((gClient*) Client)->StatusFlag;
}

UInt16 gClientNative::GetX( void* Client )
{
	return ((gClient*) Client)->X;
}

UInt16 gClientNative::GetY( void* Client )
{
	return ((gClient*) Client)->Y;
}

UInt32 gClientNative::GetPing( void* Client )
{
	return ((gClient*) Client)->Ping;
}

byte gClientNative::GetAlternativeGear( void* Client )
{
	return ((gClient*) Client)->OnAlternGear;
}

void gClientNative::SetPos( void* Client, UInt16 X, UInt16 Y )
{
	((gClient*) Client)->X = X;
	((gClient*) Client)->Y = Y;
	((gClient*) Client)->UpdatePosition();
}

char* gClientNative::GetNpcText( void* Client )
{
	return (char*) ((gClient*) Client)->NpcText.c_str();
}

void gClientNative::SetTeleportTrue( void* Client )
{
	((gClient*) Client)->IsTeleporting = 1;
}

byte gClientNative::GetTeleportStatus( void* Client )
{
	return ((gClient*) Client)->IsTeleporting;
}

void gClientNative::AddThread( void* Client, void* tHandle )
{
	((gClient*) Client)->AbortThreads.insert(((gClient*) Client)->AbortThreads.begin(), tHandle);
}

void gClientNative::RemoveThread( void* Client, void* tHandle )
{
	vector<HANDLE>::iterator it = find(((gClient*) Client)->AbortThreads.begin(), ((gClient*) Client)->AbortThreads.end(), tHandle);
	if(it != ((gClient*) Client)->AbortThreads.end())
		((gClient*) Client)->AbortThreads.erase(it);
}

void gClientNative::RemoveEntity( void* Client, int UID )
{
	((gClient*) Client)->Entities.erase(UID);
}

byte gClientNative::GetMapLoaded( void* Client )
{
	return ((gClient*) Client)->MapLoaded;
}

