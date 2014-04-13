// gProxyNativeAPI.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "gClientNative.h"
#include "PRW.h"
#include "Structs.h"

extern "C" __declspec(dllexport) void SendClient(void* Client, CBuffer Packet)
{
	gClientNative::SendClient(Client, Packet);
}

extern "C" __declspec(dllexport) void SendServer(void* Client, CBuffer Packet)
{
	gClientNative::SendServer(Client, Packet);
}

extern "C" __declspec(dllexport) void* GetClients(int* num)
{
	return gClientNative::GetClients(num);
}

extern "C" __declspec(dllexport) void* GetEntities(void* Client, int* num)
{
	return gClientNative::GetEntities(Client, num);
}

extern "C" __declspec(dllexport) void* GetNpcs(void* Client, int* num)
{
	return gClientNative::GetNpcs(Client, num);
}

extern "C" __declspec(dllexport) void* GetMap(void* Client)
{
	return gClientNative::GetMap(Client);
}

extern "C" __declspec(dllexport) void* GetCharacterItems(void* Client, int* num)
{
	return gClientNative::GetCharItems(Client, num);
}

extern "C" __declspec(dllexport) void* GetGroundItems(void* Client, int* num)
{
	return gClientNative::GetGroundItems(Client, num);
}

extern "C" __declspec(dllexport) void* GetNpcChoicesNum(void* Client, int* num)
{
	return gClientNative::GetNpcChoicesNum(Client, num);
}

extern "C" __declspec(dllexport) void* GetNpcChoicesText(void* Client, int* num)
{
	return gClientNative::GetNpcChoicesText(Client, num);
}

extern "C" __declspec(dllexport) char* GetNpcText(void* Client)
{
	return gClientNative::GetNpcText(Client);
}

extern "C" __declspec(dllexport) void GetCharacterName(void* Client, const char* buffer)
{
	gClientNative::GetCharacterName(Client, buffer);
}

extern "C" __declspec(dllexport) void GetSpouseName( void* Client, const char* buffer )
{
	gClientNative::GetSpouseName(Client, buffer);
}

extern "C" __declspec(dllexport) UInt32 GetUID( void* Client )
{
	return gClientNative::GetUID(Client);
}

extern "C" __declspec(dllexport) UInt32 GetModel( void* Client )
{
	return gClientNative::GetModel(Client);
}

extern "C" __declspec(dllexport) UInt32 GetGold( void* Client )
{
	return gClientNative::GetGold(Client);
}

extern "C" __declspec(dllexport) UInt32 GetConquerPoints( void* Client )
{
	return gClientNative::GetConquerPoints(Client);
}

extern "C" __declspec(dllexport) UInt32 GetBoundConquerPoints( void* Client )
{
	return gClientNative::GetBoundConquerPoints(Client);
}

extern "C" __declspec(dllexport) UInt64 GetExperience( void* Client )
{
	return gClientNative::GetExperience(Client);
}

extern "C" __declspec(dllexport) UInt16 GetStrengthPoints( void* Client )
{
	return gClientNative::GetStrengthPoints(Client);
}

extern "C" __declspec(dllexport) UInt16 GetDexterityPoints( void* Client )
{
	return gClientNative::GetDexterityPoints(Client);
}

extern "C" __declspec(dllexport) UInt16 GetVitalityPoints( void* Client )
{
	return gClientNative::GetVitalityPoints(Client);
}

extern "C" __declspec(dllexport) UInt16 GetSpiritPoints( void* Client )
{
	return gClientNative::GetSpiritPoints(Client);
}

extern "C" __declspec(dllexport) UInt16 GetStatPoints( void* Client )
{
	return gClientNative::GetStatPoints(Client);
}

extern "C" __declspec(dllexport) UInt16 GetHP( void* Client )
{
	return gClientNative::GetHP(Client);
}

extern "C" __declspec(dllexport) UInt16 GetMP( void* Client )
{
	return gClientNative::GetMP(Client);
}

extern "C" __declspec(dllexport) UInt16 GetPKPoints( void* Client )
{
	return gClientNative::GetPKPoints(Client);
}

extern "C" __declspec(dllexport) byte GetLevel( void* Client )
{
	return gClientNative::GetLevel(Client);
}

extern "C" __declspec(dllexport) byte GetClass( void* Client )
{
	return gClientNative::GetClass(Client);
}

extern "C" __declspec(dllexport) byte GetReborn( void* Client )
{
	return gClientNative::GetReborn(Client);
}

extern "C" __declspec(dllexport) UInt32 GetQuizPoints( void* Client )
{
	return gClientNative::GetQuizPoints(Client);
}

extern "C" __declspec(dllexport) UInt16 GetMapID( void* Client )
{
	return gClientNative::GetMapID(Client);
}

extern "C" __declspec(dllexport) UInt16 GetStamina( void* Client )
{
	return gClientNative::GetStamina(Client);
}

extern "C" __declspec(dllexport) UInt64 GetStatusFlag( void* Client )
{
	return gClientNative::GetStatusFlag(Client);
}

extern "C" __declspec(dllexport) UInt16 GetX( void* Client )
{
	return gClientNative::GetX(Client);
}

extern "C" __declspec(dllexport) UInt16 GetY( void* Client )
{
	return gClientNative::GetY(Client);
}

extern "C" __declspec(dllexport) UInt32 GetPing( void* Client )
{
	return gClientNative::GetPing(Client);
}

extern "C" __declspec(dllexport) byte GetAlternativeGear( void* Client )
{
	return gClientNative::GetAlternativeGear(Client);
}


extern "C" __declspec(dllexport) void DeleteClientArray(void* arrayPtr)
{
	delete[] arrayPtr;
}

extern "C" __declspec(dllexport) void SetTeleportTrue(void* Client)
{
	gClientNative::SetTeleportTrue(Client);
}

extern "C" __declspec(dllexport) byte GetTeleportStatus(void* Client)
{
	return gClientNative::GetTeleportStatus(Client);
}

extern "C" __declspec(dllexport) void Jump(void* Client, UInt16 X, UInt16 Y)
{
	CBuffer buffer(45);
	PRW Writer(buffer);
	Writer.ZeroFill();
	Writer.Write<Int16>(37);
	Writer.Write<Int16>(0x271A);
	Writer.Write<Int32>(GetUID(Client));
	Writer.Write<Int16>(X);
	Writer.Write<Int16>(Y);
	Writer.Write<Int32>(0);
	Writer.Write<Int32>(timeGetTime());
	Writer.Write<Int32>(0x89);
	Writer.Write<Int16>(GetX(Client));
	Writer.Write<Int16>(GetY(Client));
	Writer.Write<Int16>(GetMapID(Client));
	Writer.Write<Int16>(0);
	Writer.Write<byte>(0xFF);
	Writer.Write<byte>(0xFF);
	Writer.Write<byte>(0xFF);
	Writer.Write<byte>(0xFF);
	Writer.Write<byte>(0);
	Writer.WriteString("TQClient");
	gClientNative::SendServer(Client, buffer);
}

extern "C" __declspec(dllexport) void Portal(void* Client, UInt16 num)
{
	CBuffer buffer(45);
	PRW Writer(buffer);
	Writer.ZeroFill();
	Writer.Write<Int16>(37);
	Writer.Write<Int16>(0x271A);
	Writer.Write<Int32>(GetUID(Client));
	Writer.Write<Int16>(0);
	Writer.Write<Int16>(0);
	Writer.Write<Int32>(0);
	Writer.Write<Int32>(timeGetTime());
	Writer.Write<Int32>(85);
	Writer.Write<Int16>(0);
	Writer.Write<Int16>(0);
	Writer.Write<Int32>(0);
	Writer.Write<Int32>(num);
	Writer.Write<byte>(0);
	Writer.WriteString("TQClient");
	gClientNative::SendServer(Client, buffer);
}

extern "C" __declspec(dllexport) void UpdateLocation(void* Client)
{
	CBuffer buffer(45);
	PRW Writer(buffer);
	Writer.ZeroFill();
	Writer.Write<Int16>(37);
	Writer.Write<Int16>(0x271A);
	Writer.Write<Int32>(GetUID(Client));
	Writer.Write<Int16>(GetMapID(Client));
	Writer.Write<Int64>(0);
	Writer.Write<Int16>(0);
	Writer.Write<Int32>(0x9C);
	Writer.Write<Int16>(GetX(Client));
	Writer.Write<Int16>(GetY(Client));
	Writer.Write<Int64>(0);
	Writer.Write<byte>(0);
	Writer.WriteString("TQServer");
	gClientNative::SendClient(Client, buffer);
}

extern "C" __declspec(dllexport) void Melee(void* Client, UInt32 UID, Int32 AttackType)
{
	CBuffer buffer(48);
	PRW Writer(buffer);
	Writer.ZeroFill();
	Writer.Write<Int16>(40);
	Writer.Write<Int16>(0x3FE);
	Writer.Write<Int32>(timeGetTime());
	Writer.Write<Int32>(GetUID(Client));
	Writer.Write<Int32>(UID);
	Writer.Write<Int16>(GetX(Client));
	Writer.Write<Int16>(GetY(Client));
	Writer.Write<Int32>(AttackType);
	Writer.Write<Int64>(0);
	Writer.Write<Int64>(0);
	Writer.WriteString("TQClient");
	gClientNative::SendServer(Client, buffer);
}

extern "C" __declspec(dllexport) void Shift(void* Client, UInt16 X, UInt16 Y)
{
	if(max(abs((int)X - (int)GetX(Client)), abs((int)Y - (int)GetY(Client))) > 7)
		return;

	CBuffer buffer(48);
	PRW Writer(buffer);
	Writer.ZeroFill();
	Writer.Write<Int16>(40);
	Writer.Write<Int16>(0x3FE);
	Writer.Write<Int32>(timeGetTime());
	Writer.Write<Int32>(GetUID(Client));
	Writer.Write<Int32>(GetUID(Client));
	Writer.Write<Int16>(X);
	Writer.Write<Int16>(Y);
	Writer.Write<Int32>(2);
	Writer.Write<Int64>(0);
	Writer.Write<Int64>(0);
	Writer.WriteString("TQClient");
	gClientNative::SendServer(Client, buffer);
	gClientNative::SetPos(Client, X, Y);
}

extern "C" __declspec(dllexport) void SetPosition(void* Client, UInt16 X, UInt16 Y)
{
	gClientNative::SetPos(Client, X, Y);
}

extern "C" __declspec(dllexport) void CastSpell(void* Client, UInt32 UID, UInt16 SpellID)
{
	P_3FE MSG = P_3FE();
	MSG.Size = 40;
	MSG.Type = 0x3FE;
	MSG.Timestamp = timeGetTime();
	MSG.UID = GetUID(Client);
	MSG.Opponent = UID;
	MSG.X = GetX(Client);
	MSG.Y = GetY(Client);
	MSG.AttackType = 24;
	MSG.Value.o.SpellID = SpellID;
	MSG.Value.o.wReserved = 0;
	MSG.Encrypt();

	CBuffer buffer(48);
	PRW Writer(buffer);
	Writer.ZeroFill();
	Writer.Write<P_3FE>(MSG);
	Writer.WriteString("TQClient");
	gClientNative::SendServer(Client, buffer);
}

extern "C" __declspec(dllexport) void CastSpellPos(void* Client, UInt16 X, UInt16 Y, UInt16 SpellID)
{
	P_3FE MSG = P_3FE();
	MSG.Size = 40;
	MSG.Type = 0x3FE;
	MSG.Timestamp = timeGetTime();
	MSG.UID = GetUID(Client);
	MSG.Opponent = 0;
	MSG.X = X;
	MSG.Y = Y;
	MSG.AttackType = 24;
	MSG.Value.o.SpellID = SpellID;
	MSG.Value.o.wReserved = 0;
	MSG.Encrypt();

	CBuffer buffer(48);
	PRW Writer(buffer);
	Writer.ZeroFill();
	Writer.Write<P_3FE>(MSG);
	Writer.WriteString("TQClient");
	gClientNative::SendServer(Client, buffer);
}

extern "C" __declspec(dllexport) void Mine(void* Client, byte Direction)
{
	CBuffer buffer(45);
	PRW Writer(buffer);
	Writer.ZeroFill();
	Writer.Write<Int16>(37);
	Writer.Write<Int16>(0x271A);
	Writer.Write<Int32>(GetUID(Client));
	Writer.Write<Int64>(0);
	Writer.Write<Int32>(timeGetTime());
	Writer.Write<Int16>(0x63);
	Writer.Write<Int64>(Direction);
	Writer.Write<Int32>(0);
	Writer.Write<Int16>(0);
	Writer.Write<byte>(0);
	Writer.WriteString("TQClient");
	gClientNative::SendServer(Client, buffer);
}

extern "C" __declspec(dllexport) void PickupItem(void* Client, UInt32 GUID)
{
	CBuffer buffer(32);
	PRW Writer(buffer);
	Writer.ZeroFill();
	Writer.Write<Int16>(24);
	Writer.Write<Int16>(0x44D);
	Writer.Write<Int32>(GUID);
	Writer.Write<Int32>(GetUID(Client));
	Writer.Write<Int16>(GetX(Client));
	Writer.Write<Int16>(GetY(Client));
	Writer.Write<Int16>(0);
	Writer.Write<Int16>(3);
	Writer.Write<Int32>(0);
	Writer.WriteString("TQClient");
	gClientNative::SendServer(Client, buffer);
}

extern "C" __declspec(dllexport) void SpeakNpc(void* Client, UInt32 ID)
{
	CBuffer buffer(24);
	PRW Writer(buffer);
	Writer.ZeroFill();
	Writer.Write<Int16>(16);
	Writer.Write<Int16>(0x7EF);
	Writer.Write<Int32>(ID);
	Writer.Write<Int64>(0);
	Writer.WriteString("TQClient");
	gClientNative::SendServer(Client, buffer);
}

extern "C" __declspec(dllexport) void ReplyNpc(void* Client, byte Num)
{
	CBuffer buffer(24);
	PRW Writer(buffer);
	Writer.ZeroFill();
	Writer.Write<Int16>(16);
	Writer.Write<Int16>(0x7F0);
	Writer.Write<Int32>(0);
	Writer.Write<Int16>(0);
	Writer.Write<byte>(Num);
	Writer.Write<Int32>(101);
	Writer.Write<byte>(0);
	Writer.WriteString("TQClient");
	gClientNative::SendServer(Client, buffer);
}

extern "C" __declspec(dllexport) void EndNpc(void* Client)
{
	CBuffer buffer(24);
	PRW Writer(buffer);
	Writer.ZeroFill();
	Writer.Write<Int16>(16);
	Writer.Write<Int16>(0x7F0);
	Writer.Write<Int32>(0);
	Writer.Write<Int16>(0);
	Writer.Write<byte>(255);
	Writer.Write<Int32>(101);
	Writer.Write<byte>(0);
	Writer.WriteString("TQClient");
	gClientNative::SendServer(Client, buffer);
}

extern "C" __declspec(dllexport) void SendClientMessage(void* Client, char* from, char* to, char* msg, UInt16 type, int Color)
{
	int Len = strlen(from) + strlen(to) + strlen(msg);
	CBuffer buffer(40 + Len);
	PRW Writer(buffer);
	Writer.ZeroFill();
	Writer.Write<Int16>(32 + Len);
	Writer.Write<Int16>(0x3EC);
	Writer.Write<Int32>(Color);
	Writer.Write<Int32>(type);
	Writer.Write<Int32>(1);
	Writer.Write<Int64>(0);
	Writer.Write<byte>(0x4);
	Writer.WriteBufferByte(CBuffer(from));
	Writer.WriteBufferByte(CBuffer(to));
	Writer.Write<byte>(0);
	Writer.WriteBufferByte(CBuffer(msg));
	Writer.Write<byte>(0);
	Writer.Write<byte>(0);
	Writer.Write<byte>(0);
	Writer.WriteString("TQServer");
	gClientNative::SendClient(Client, buffer);
}

extern "C" __declspec(dllexport) void AddAbortThreadOnDC(void* Client, void* tHandle)
{
	gClientNative::AddThread(Client, tHandle);
}

extern "C" __declspec(dllexport) void RemoveAbortThreadOnDC(void* Client, void* tHandle)
{
	gClientNative::RemoveThread(Client, tHandle);
}

extern "C" __declspec(dllexport) void ThreadAbort(void* tHandle)
{
	TerminateThread(tHandle, 0);
}

extern "C" __declspec(dllexport) void RemoveEntity(void* Client, int UID)
{
	gClientNative::RemoveEntity(Client, UID);
}

extern "C" __declspec(dllexport) byte GetMapLoaded( void* Client )
{
	return gClientNative::GetMapLoaded(Client);
}