#pragma once
class CBuffer
{
private:
	int bufferSize;
	shared_ptr<byte> buffer;

public:
	CBuffer(int length, bool initialize) : bufferSize(length){
		buffer = shared_ptr<byte>(new byte[length]);
		if(initialize)
			Initialize();
	};

	CBuffer(int length) : bufferSize(length){
		buffer = shared_ptr<byte>(new byte[length]);
	};

	CBuffer(char* str)
	{
		bufferSize = strlen(str);
		buffer = shared_ptr<byte>(new byte[bufferSize]);
		memcpy(buffer.get(), str, bufferSize);
	}

	CBuffer(void){}

	void Initialize()
	{
		memset(ToVoid(), 0, bufferSize);
	}

	int Size();
	spchar NullTerminate();
	operator void*();
	operator char*();
	operator byte*();
	operator shared_ptr<byte>*();
	byte& operator[](int i);
	void* operator+(int i);
	void* ToVoid();
	char* ToChar();
	byte* ToByte();
};

