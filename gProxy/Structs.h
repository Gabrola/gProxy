#pragma once

struct Entity
{
	UInt16 X;
	UInt16 Y;
	UInt32 UID;
	UInt64 StatusFlag;
	const char Name[16];
	byte Level;
	UInt16 HP;
};

struct GroundItem
{
	UInt32 GroundUID;
	UInt32 StaticID;
	UInt16 X;
	UInt16 Y;
	const char Name[100];
};

struct Npc
{
	UInt32 ID;
	UInt16 X;
	UInt16 Y;
	UInt16 Type;
	const char Name[50];
};

struct UserItem
{
	UInt32 UID;
	UInt32 StaticID;
	UInt16 Dura;
	UInt16 MaxDura;
	byte Position;
	UInt16 RebornEffect;
	byte FirstSocket;
	byte SecondSocket;
	byte Plus;
	byte Bless;
	byte Free;
	byte EnchantedHP;
	byte Locked;
	byte Color;
	UInt32 CompositionProgress;
	const char Name[100];
};

struct gPoint
{
	UInt16 X;
	UInt16 Y;
};

struct gMapPoint
{
	byte Accessible;
};

struct gMap
{
	char Name[100];
	UInt16 Width;
	UInt16 Height;
	byte* Accessibility;

	gMap(){}

	~gMap()
	{
		if(Accessibility)
			delete[] Accessibility;
	}
};

enum DirectionCalc
{
	D0X = 0,
	D0Y = 1,
	D1X = -1,
	D1Y = 1,
	D2X = -1,
	D2Y = 0,
	D3X = -1,
	D3Y = -1,
	D4X = 0,
	D4Y = -1,
	D5X = 1,
	D5Y = -1,
	D6X = 1,
	D6Y = 0,
	D7X = 1,
	D7Y = 1
};

struct HpChange
{
	UInt32 UID;
	UInt32 HP;
};

struct MpChange
{
	UInt32 UID;
	UInt32 MP;
};

struct Jump
{
	UInt32  UID;
	UInt16 X;
	UInt16 Y;
	UInt16 FromX;
	UInt16 FromY;
};

struct StatusFlagChange
{
	UInt32 UID;
	UInt64 StatusFlag;
};

struct Walk
{
	UInt32 UID;
	byte Direction;
	byte Running;
};


struct EntitySpawn
{
	UInt32 UID;
};


struct ItemDrop
{
	UInt32 GroundUID;
	UInt32 StaticID;
	UInt16 X;
	UInt16 Y;
};


struct Chat
{
	char* From;
	char* To;
	char* Message;
	UInt32 Type;
	UInt32 Color;
	byte Send;
};


struct Attack
{
	UInt32 AttackerUID;
	UInt32 TargetUID;
	UInt16 X;
	UInt16 Y;
	UInt32 Damage;
	UInt16 SpellID;
};


struct SpellCast
{
	UInt32 AttackerUID;
	UInt32 TargetUID;
	UInt16 X;
	UInt16 Y;
	UInt32 Damage;
	UInt16 SpellID;
};