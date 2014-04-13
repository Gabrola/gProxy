#pragma once
#include "PRW.h"

class CHandshakeReply
{
private:
	CBuffer Data;
	CBuffer Header;
	CBuffer TQClient;
	CBuffer _Packet;

public:
	CHandshakeReply(CBuffer Packet);
	CBuffer Rebuild();
	void SetPublicKey(CBuffer Key);

	Int32 FinalSize;
	CBuffer PubKey;
};

