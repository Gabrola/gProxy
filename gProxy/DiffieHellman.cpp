#include "StdAfx.h"
#include "DiffieHellman.h"

CDiffieHellman::CDiffieHellman(spchar P, spchar G)
{
	int codes = 0;
	this->dh = DH_new();
	SetPG(P, G);

	if (!DH_check(dh, &codes))
		cout << "DH_check Failed" << endl;

	if (!DH_generate_key(this->dh))
		cout << "DH_generate_key Failed" << endl;
}

CDiffieHellman::CDiffieHellman( char* P, char* G )
{
	int codes = 0;
	this->dh = DH_new();
	SetPG(P, G);

	if (!DH_check(dh, &codes))
		cout << "DH_check Failed" << endl;

	if (!DH_generate_key(this->dh))
		cout << "DH_generate_key Failed" << endl;
}


CDiffieHellman::~CDiffieHellman(void)
{
	DH_free(this->dh);
}

void CDiffieHellman::SetPG( spchar P, spchar G )
{
	BIGNUM* BP = NULL;
	BIGNUM* BG = NULL;
	BN_hex2bn(&BP, P.get());
	BN_hex2bn(&BG, G.get());
	dh->p = BN_dup(BP);
	dh->g = BN_dup(BG);
	BN_clear(BP);
	BN_clear(BG);
}

void CDiffieHellman::SetPG( char* P, char* G )
{
	BIGNUM* BP = NULL;
	BIGNUM* BG = NULL;
	BN_hex2bn(&BP, P);
	BN_hex2bn(&BG, G);
	dh->p = BN_dup(BP);
	dh->g = BN_dup(BG);
	BN_clear(BP);
	BN_clear(BG);
}

CBuffer CDiffieHellman::GetPubKey()
{
	char* hexKey = BN_bn2hex(dh->pub_key);
	return CBuffer(hexKey);
}

CBuffer CDiffieHellman::SetPubKey( char* Key )
{
	CBuffer rKey(DH_size(dh));
	BIGNUM* BK = NULL;
	if(BN_hex2bn(&BK, Key) == 0)
		cout << "Error #1" << endl;
	if(DH_compute_key(rKey, BK, dh) == -1)
		cout << "Error #2" << endl;
	return rKey;
}
