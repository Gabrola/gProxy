#pragma once
#include "Buffer.h"

class PRW
{
public:
	PRW(unsigned int Length);
	PRW(CBuffer Stream);
	CBuffer UnderlyingBuffer;
	unsigned int Position;
	template<typename T> void Write(T Value);
	template<typename T> T Read(T * Ret);
	template<typename T> T Read();
	void WriteString(string);
	void WriteBytes(CBuffer Data);
	void WriteBuffer(CBuffer Data);
	void WriteBufferByte(CBuffer Data);
	string ReadString(UInt32 Length);
	CBuffer ReadBytes(UInt32 Length);
	CBuffer ReadBuffer();
	void ZeroFill();
};

template<typename T> void PRW::Write(T Value)
{
	unsigned char * X = reinterpret_cast<unsigned char*>(&Value);
	for(unsigned int i = 0; i < sizeof(T); i++)
	{
		UnderlyingBuffer[this->Position] = (*(X + i));
		this->Position++;
	}
}
template<typename T> T PRW::Read(T * Ret)
{
	memcpy(Ret, UnderlyingBuffer + this->Position, sizeof(T));
	Position += sizeof(T);
	return (*(Ret));
}
template<typename T> T PRW::Read()
{
	T ret = 0;
	memcpy(&ret, UnderlyingBuffer + this->Position, sizeof(T));
	Position += sizeof(T);
	return ret;
}