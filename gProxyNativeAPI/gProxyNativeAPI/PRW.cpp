#include "StdAfx.h"
#include "PRW.h"


PRW::PRW(unsigned int Length) : TotalLength(Length), Position(0), UnderlyingBuffer(new unsigned char[Length]), Allocated(true) { }
PRW::PRW(unsigned char *Stream, unsigned int Length) : TotalLength(Length), Position(0), UnderlyingBuffer(Stream), Allocated(false) { }
PRW::~PRW()
{
	if(Allocated)
		delete[] UnderlyingBuffer;
}
void PRW::WriteString(string Value)
{
	for(unsigned int i = 0; i < Value.length(); i++)
	{
		(*(this->UnderlyingBuffer + this->Position + i)) = (unsigned char)Value[i];
	}
	this->Position += Value.length();
}
void PRW::WriteBytes(byte * Data, UInt32 Length)
{
	for(UInt32 i = 0; i < Length; i++)
	{
		(*(this->UnderlyingBuffer + this->Position + i)) = Data[i];
	}
	this->Position += Length;
}
string PRW::ReadString(UInt32 Length)
{
	char * Tempo = new char[Length];
	for(unsigned int i = 0; i < Length; i++)
	{
		Tempo[i] = (*(this->UnderlyingBuffer + this->Position));
		this->Position++;
	}
	string RetVal(Tempo);
	delete[] Tempo;
	return RetVal;
}
byte * PRW::ReadBytes(UInt32 Length)
{
	byte * bytesRead = new byte[Length];
	for(UInt32 i = 0; i < Length; i++)
	{
		bytesRead[i] = (*(this->UnderlyingBuffer + this->Position + i));
	}
	this->Position += Length;
	return bytesRead;
}
void PRW::ZeroFill()
{
	memset(UnderlyingBuffer, 0x00, TotalLength);
}