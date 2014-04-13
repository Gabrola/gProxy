#include "StdAfx.h"
#include "Handshake.h"


CHandshake::CHandshake( CBuffer Packet )
{
	_Packet = Packet;
	PRW Reader(Packet);
	FinalSize = Packet.Size() - 8;
	RandomData = Reader.ReadBytes(11);
	TQSize = Reader.Read<Int32>();
	NonStaticRandomData = Reader.ReadBuffer();
	ClientIvec = Reader.ReadBuffer();
	ServerIvec = Reader.ReadBuffer();
	P = Reader.ReadBuffer();
	G = Reader.ReadBuffer();
	PubKey = Reader.ReadBuffer();
	TQServer = Reader.ReadBytes(8);

	if(PubKey.Size() == 126)
	{
		FinalSize += 2;
		TQSize += 2;
	}

	cout << endl;
	cout << "CHandshake::ClientIvecLength = " << ClientIvec.Size() << endl;
	cout << "CHandshake::ServerIvecLength = " << ServerIvec.Size() << endl;
	cout << "CHandshake::PLength = " << P.Size() << endl;
	cout << "CHandshake::GLength = " << G.Size() << endl;
	cout << "CHandshake::PubKeyLength = " << PubKey.Size() << endl;
	cout << endl;
}

CBuffer CHandshake::Rebuild()
{
	PRW Writer(FinalSize + 8);
	Writer.WriteBytes(RandomData);
	Writer.Write<Int32>(TQSize);
	Writer.WriteBuffer(NonStaticRandomData);
	Writer.WriteBuffer(ClientIvec);
	Writer.WriteBuffer(ServerIvec);
	Writer.WriteBuffer(P);
	Writer.WriteBuffer(G);
	Writer.WriteBuffer(PubKey);
	Writer.WriteBytes(TQServer);
	return Writer.UnderlyingBuffer;
}

void CHandshake::SetPublicKey( CBuffer Key )
{
	PubKey = Key;
}