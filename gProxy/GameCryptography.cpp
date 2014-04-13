#include "StdAfx.h"
#include "GameCryptography.h"

CGameCryptography::CGameCryptography(void)
{
	tempIvec = CBuffer(16);
	bf = new CBlowfish("DR654dt34trg4UI6");
}


CGameCryptography::~CGameCryptography(void)
{
	delete bf;
}

void CGameCryptography::SetDH( spchar P, spchar G )
{
	dh = new CDiffieHellman(P, G);
}

void CGameCryptography::ReleaseDH()
{
	delete dh;
}

void CGameCryptography::CompleteDH()
{
	CBuffer key = this->dh->SetPubKey(dh->AnotherPubKey.NullTerminate().get());
	this->bf->SetKey(key);
	this->ReleaseDH();
	CBuffer Enc(8);
	CBuffer Dec(8);
	memcpy(Enc, tempIvec + 0, 8);
	memcpy(Dec, tempIvec + 8, 8);
	this->bf->SetIvecs(Enc, Dec);
}

CBuffer CGameCryptography::Decrypt( CBuffer In )
{
	return this->bf->Decrypt(In);
}

CBuffer CGameCryptography::Encrypt( CBuffer In )
{
	return this->bf->Encrypt(In);
}
