#include "StdAfx.h"
#include "PRW.h"


PRW::PRW(unsigned int Length) : Position(0) { UnderlyingBuffer = CBuffer(Length); }
PRW::PRW(CBuffer Stream) : Position(0) { UnderlyingBuffer = Stream; }
void PRW::WriteString(string Value)
{
	for(unsigned int i = 0; i < Value.length(); i++)
	{
		UnderlyingBuffer[this->Position + i] = (unsigned char)Value[i];
	}
	this->Position += Value.length();
}
void PRW::WriteBytes(CBuffer Data)
{
	for(UInt32 i = 0; i < Data.Size(); i++)
	{
		UnderlyingBuffer[this->Position + i] = Data[i];
	}
	this->Position += Data.Size();
}
void PRW::WriteBuffer(CBuffer Data)
{
	this->Write<UInt32>(Data.Size());
	for(UInt32 i = 0; i < Data.Size(); i++)
	{
		UnderlyingBuffer[this->Position + i] = Data[i];
	}
	this->Position += Data.Size();
}
void PRW::WriteBufferByte(CBuffer Data)
{
	this->Write<byte>((byte) Data.Size());
	for(UInt32 i = 0; i < Data.Size(); i++)
	{
		UnderlyingBuffer[this->Position + i] = Data[i];
	}
	this->Position += Data.Size();
}
string PRW::ReadString(UInt32 Length)
{
	char * Tempo = new char[Length];
	for(unsigned int i = 0; i < Length; i++)
	{
		Tempo[i] = UnderlyingBuffer[this->Position];
		this->Position++;
	}
	string RetVal(Tempo, Length);
	delete[] Tempo;
	return RetVal;
}
CBuffer PRW::ReadBytes(UInt32 Length)
{
	CBuffer bytesRead(Length);
	for(UInt32 i = 0; i < Length; i++)
	{
		bytesRead[i] = UnderlyingBuffer[this->Position + i];
	}
	this->Position += Length;
	return bytesRead;
}
void PRW::ZeroFill()
{
	memset(UnderlyingBuffer, 0x00, UnderlyingBuffer.Size());
}

CBuffer PRW::ReadBuffer()
{
	int Num = 0;
	memcpy(&Num, UnderlyingBuffer + this->Position, 4);
	Position += sizeof(4);
	CBuffer Buffer(Num);
	for(UInt32 i = 0; i < Buffer.Size(); i++)
	{
		Buffer[i] = UnderlyingBuffer[this->Position + i];
	}
	this->Position += Buffer.Size();
	return Buffer;
}
