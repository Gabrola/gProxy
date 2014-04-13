#pragma once
class DatEncrypt
{
public:
	static int rotr8(int x, int n);
	static int rotl8(int x, int n);
	static byte CO2feda_KBox1024[128];

	static UInt32 DecryptData(byte* pData, UInt32 DataSize);
};

