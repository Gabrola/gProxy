#pragma once
class CBlowfish
{
private:
	CBuffer _decryptIv;
	CBuffer _encryptIv;
	int _decryptNum;
	int _encryptNum;
	BF_KEY* _key;

public:
	CBlowfish(char* Key);
	CBuffer Encrypt(CBuffer Data);
	CBuffer Decrypt(CBuffer Data);
	void SetKey(CBuffer Key);
	void SetIvecs(CBuffer EncryptIV, CBuffer DecryptIV);
	~CBlowfish(void);
};

