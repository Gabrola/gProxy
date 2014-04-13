#pragma once
#include "Blowfish.h"
#include "DiffieHellman.h"

class CGameCryptography
{
private:
	CBlowfish* bf;

public:
	CGameCryptography(void);
	~CGameCryptography(void);

	void SetDH(spchar P, spchar G);
	void ReleaseDH();
	void CompleteDH();
	CDiffieHellman* dh;

	CBuffer Decrypt(CBuffer In);
	CBuffer Encrypt(CBuffer In);

	CBuffer tempIvec;
};

