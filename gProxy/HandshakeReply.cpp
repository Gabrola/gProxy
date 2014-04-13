#include "StdAfx.h"
#include "HandshakeReply.h"


CHandshakeReply::CHandshakeReply( CBuffer Packet )
{
	_Packet = Packet;
	FinalSize = Packet.Size() - 8;
	PRW Reader(Packet);
	Header = Reader.ReadBytes(11);
	Data = Reader.ReadBuffer();
	PubKey = Reader.ReadBuffer();
	TQClient = Reader.ReadBytes(8);
	FinalSize -= PubKey.Size();

	cout << endl;
	cout << "CHandshakeReply::DataLength = " << Data.Size() << endl;
	cout << "CHandshakeReply::PubKeyLength = " << PubKey.Size() << endl;
	cout << endl;
}

CBuffer CHandshakeReply::Rebuild()
{
	memcpy(_Packet.ToChar() + 19 + Data.Size(), PubKey, PubKey.Size());

	return _Packet;
}

void CHandshakeReply::SetPublicKey( CBuffer Key )
{
	PubKey = Key;
}
