#include "StdAfx.h"
#include "Buffer.h"


int CBuffer::Size()
{
	return bufferSize;
}

CBuffer::operator void*()
{
	return (void*) buffer.get();
}

CBuffer::operator char*()
{
	return (char*) buffer.get();
}

CBuffer::operator byte*()
{
	return buffer.get();
}

byte& CBuffer::operator[]( int i )
{
	return buffer.get()[i];
}

void* CBuffer::operator+( int i )
{
	return (buffer.get() + i);
}

spchar CBuffer::NullTerminate()
{
	spchar chart(new char[this->Size() + 1]);
	memcpy(chart.get(), buffer.get(), this->Size());
	chart.get()[this->Size()] = '\0';
	return chart;
}

CBuffer::operator shared_ptr<byte>*()
{
	return &buffer;
}

void* CBuffer::ToVoid()
{
	return (void*) buffer.get();
}

char* CBuffer::ToChar()
{
	return (char*) buffer.get();
}

byte* CBuffer::ToByte()
{
	return buffer.get();
}
