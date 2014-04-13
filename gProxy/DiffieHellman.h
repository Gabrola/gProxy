#pragma once
class CDiffieHellman
{
private:
	void SetPG(spchar P, spchar G);
	void SetPG(char* P, char* G);

public:
	CDiffieHellman(spchar P, spchar G);
	CDiffieHellman(char* P, char* G);
	~CDiffieHellman(void);

	CBuffer GetPubKey();
	CBuffer SetPubKey(char* Key);
	dh_st* dh;

	CBuffer AnotherPubKey;
};

