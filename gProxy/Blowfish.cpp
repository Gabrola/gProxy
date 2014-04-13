#include "StdAfx.h"
#include "Blowfish.h"


CBlowfish::CBlowfish(char* Key)
{
	_decryptIv = CBuffer(8);
	_encryptIv = CBuffer(8);

	_decryptIv.Initialize();
	_encryptIv.Initialize();

	_key = new BF_KEY();

	BF_set_key(_key, strlen(Key), (byte*) Key);
	_encryptNum = 0;
	_decryptNum = 0;
}


CBlowfish::~CBlowfish(void)
{
	delete _key;
}

CBuffer CBlowfish::Encrypt(CBuffer Data)
{
	CBuffer Buffer(Data.Size());
	BF_cfb64_encrypt(Data, Buffer, Data.Size(), _key, _encryptIv, &_encryptNum, 1);
	return Buffer;
}

CBuffer CBlowfish::Decrypt( CBuffer Data )
{
	CBuffer Buffer(Data.Size());
	BF_cfb64_encrypt(Data, Buffer, Data.Size(), _key, _decryptIv, &_decryptNum, 0);
	return Buffer;
}

void CBlowfish::SetKey( CBuffer Key )
{
	BF_set_key(_key, Key.Size(), Key);
	_encryptNum = 0;
	_decryptNum = 0;
}

void CBlowfish::SetIvecs( CBuffer EncryptIV, CBuffer DecryptIV )
{
	_decryptIv = DecryptIV;
	_encryptIv = EncryptIV;
}
