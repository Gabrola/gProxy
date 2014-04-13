#pragma once
class CBitArray
{
public:
	CBitArray(void);

	byte Store;
	bool Get(byte Location);
	void Set(byte Location, bool Value);
	static byte Set(byte Location, bool Value, byte Base);
};

