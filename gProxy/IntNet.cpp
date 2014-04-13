#include "StdAfx.h"
#include "IntNet.h"

HINTERNET IntNet::hHandle = InternetOpen(L"gProxy 0.01", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
string IntNet::akey1 = "PplJCHzN";
string IntNet::akey2 = "sGvRtYUK";
LPCWSTR IntNet::website = L"localhost";