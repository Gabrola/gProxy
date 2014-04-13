#pragma once
#include "PRW.h"

class CHandshake
{
private:
	CBuffer NonStaticRandomData;
	CBuffer RandomData;
	Int32 TQSize;
	CBuffer TQServer;
	CBuffer _Packet;

public:
	CHandshake(CBuffer Packet);
	CBuffer Rebuild();
	void SetPublicKey(CBuffer Key);

	CBuffer P;
	CBuffer G;
	CBuffer ClientIvec;
	CBuffer ServerIvec;
	Int32 FinalSize;
	CBuffer PubKey;
};
