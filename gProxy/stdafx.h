// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#pragma warning(disable : 4996)
#define WIN32_LEAN_AND_MEAN 
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <windows.h>
#include <conio.h>
#include <WinInet.h>
#include <direct.h>
#include <assert.h>
#include <math.h>

#include "MSCorEE.h"
#include "winsock2.h"
#include "resource.h"
#include "md5.h"
#include "base64.h"

#include <openssl/blowfish.h>
#include <openssl/dh.h>
#include <openssl/bn.h>
#include <openssl/ssl.h>
#include <io.h>
#include <fstream>

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/array.hpp>
#include <boost/asio.hpp>

#import "C:\Windows\Microsoft.NET\Framework\v4.0.30319\mscorlib.tlb" auto_rename
#pragma comment(lib,"mscoree.lib") 

using boost::asio::ip::tcp;
using namespace std;

typedef __int8 Int8;
typedef __int16 Int16;
typedef __int32 Int32;
typedef __int64 Int64;
typedef unsigned __int8 UInt8;
typedef unsigned __int16 UInt16;
typedef unsigned __int32 UInt32;
typedef unsigned __int64 UInt64;
typedef unsigned char byte;
typedef unsigned __int16 ushort;
typedef unsigned __int32 uint;
typedef shared_ptr<byte> spbyte;
typedef shared_ptr<char> spchar;

#include "Buffer.h"
#include "BitArray.h"

// TODO: reference additional headers your program requires here
