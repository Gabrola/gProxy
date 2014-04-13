#pragma once

union DamageSpellUnion
{
	Int32 Damage;
	struct
	{
		UInt16 SpellID;
		UInt16 wReserved;
	} o;
};

struct P_3FE
{
	UInt16 Size;
	UInt16 Type;
	UInt32 Timestamp;
	UInt32 UID;
	UInt32 Opponent;
	UInt16 X;
	UInt16 Y;
	UInt32 AttackType;
	DamageSpellUnion Value;
	Int32 dwUnknown1;
	Int32 dwUnknown2;
	Int32 dwUnknown3;

private:
	UInt32 ror32(UInt32 value, Int32 amount)
	{
		return ((value >> amount) | (value << (32 - amount)));
	}

	UInt32 rol32(UInt32 value, Int32 amount)
	{
		return ((value << amount) | (value >> (32 - amount)));
	}

	UInt16 ror16(UInt16 value, Int32 amount)
	{
		return (UInt16)((value >> amount) | (value << (16 - amount)));
	}

	UInt16 rol16(UInt16 value, Int32 amount)
	{
		return (UInt16)((value << amount) | (value >> (16 - amount)));
	}

public:
	P_3FE()
	{
		dwUnknown1 = 0;
		dwUnknown2 = 0;
		dwUnknown3 = 0;
	}

	void Decrypt()
	{
		UInt32 uID = this->UID;
		UInt32 opponent = this->Opponent;
		UInt16 x = this->X;
		UInt16 y = this->Y;
		UInt16 spellID = this->Value.o.SpellID;
		UInt16 wReserved = this->Value.o.wReserved;
		UInt32 timestamp = this->Timestamp;
		this->Opponent = this->ror32(opponent, 13);
		this->Opponent = ((this->Opponent ^ 1596793955) ^ uID) - 1953450726;
		this->X = (UInt16)(this->rol16((UInt16)((x ^ 11990) ^ ((UInt16)uID)), 1) - 8942);
		this->Y = (UInt16)(this->rol16((UInt16)((y ^ 47515) ^ ((UInt16)uID)), 5) - 35106);
		this->Value.o.SpellID = (UInt16)(this->rol16((UInt16)((spellID ^ 37213) ^ ((UInt16)uID)), 3) - 60226);
		this->Value.o.wReserved = (UInt16)((((byte)wReserved) ^ 33) | (((byte)(timestamp ^ 55)) << 8));
	}

	void Encrypt()
	{
		UInt32 uID = this->UID;
		UInt32 opponent = this->Opponent;
		UInt16 x = this->X;
		UInt16 y = this->Y;
		UInt16 spellID = this->Value.o.SpellID;
		UInt16 wReserved = this->Value.o.wReserved;
		UInt32 timestamp = this->Timestamp;
		this->Opponent = ((opponent + 1953450726) ^ 1596793955) ^ uID;
		this->Opponent = this->rol32(this->Opponent, 13);
		this->X = (UInt16)((this->ror16((UInt16)(x + 8942), 1) ^ ((UInt16)uID)) ^ 11990);
		this->Y = (UInt16)((this->ror16((UInt16)(y + 35106), 5) ^ ((UInt16)uID)) ^ 47515);
		this->Value.o.SpellID = (UInt16)((this->ror16((UInt16)(spellID + 60226), 3) ^ ((UInt16)uID)) ^ 37213);
		this->Value.o.wReserved = (UInt16)((((byte)wReserved) ^ 33) | (((byte)(timestamp ^ 55)) << 8));
	}
};