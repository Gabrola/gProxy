#pragma once
class __declspec(dllexport) gClientNative
{
public:
	gClientNative(void);
	~gClientNative(void);

	static void SendClient(void* Client, CBuffer Packet);
	static void SendServer(void* Client, CBuffer Packet);
	static void* GetClients(int* num);
	static void* GetEntities(void* Client, int* num);
	static void* GetNpcs(void* Client, int* num);
	static void* GetCharItems(void* Client, int* num);
	static void* GetGroundItems(void* Client, int* num);
	static void* GetMap(void* Client);
	static void GetCharacterName(void* Client, const char* buffer);
	static void GetSpouseName(void* Client, const char* buffer);
	static UInt32 GetUID(void* Client);
	static UInt32 GetModel(void* Client);
	static UInt32 GetGold(void* Client);
	static UInt32 GetConquerPoints(void* Client);
	static UInt32 GetBoundConquerPoints(void* Client);
	static UInt64 GetExperience(void* Client);
	static UInt16 GetStrengthPoints(void* Client);
	static UInt16 GetDexterityPoints(void* Client);
	static UInt16 GetVitalityPoints(void* Client);
	static UInt16 GetSpiritPoints(void* Client);
	static UInt16 GetStatPoints(void* Client);
	static UInt16 GetHP(void* Client);
	static UInt16 GetMP(void* Client);
	static UInt16 GetPKPoints(void* Client);
	static byte GetLevel(void* Client);
	static byte GetClass(void* Client);
	static byte GetReborn(void* Client);
	static UInt32 GetQuizPoints(void* Client);
	static UInt16 GetMapID(void* Client);
	static UInt16 GetStamina(void* Client);
	static UInt64 GetStatusFlag(void* Client);
	static UInt16 GetX(void* Client);
	static UInt16 GetY(void* Client);
	static void SetPos(void* Client, UInt16 X, UInt16 Y);
	static UInt32 GetPing(void* Client);
	static byte GetAlternativeGear(void* Client);
	static char* GetNpcText(void* Client);
	static void* GetNpcChoicesNum(void* Client, int* num);
	static void* GetNpcChoicesText(void* Client, int* num);
	static void SetTeleportTrue(void* Client);
	static byte GetTeleportStatus(void* Client);
	static void AddThread(void* Client, void* tHandle);
	static void RemoveThread(void* Client, void* tHandle);
	static void RemoveEntity(void* Client, int UID);
	static byte GetMapLoaded(void* Client);
};

