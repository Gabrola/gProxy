// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <Mmsystem.h>
#include <WinSock2.h>
#include <iostream>
#include <boost/shared_ptr.hpp>
using namespace std;

typedef __int8 Int8;
typedef __int16 Int16;
typedef __int32 Int32;
typedef __int64 Int64;

typedef unsigned char byte;

typedef unsigned __int8 UInt8;
typedef unsigned __int16 UInt16;
typedef unsigned __int32 UInt32;
typedef unsigned __int64 UInt64;
typedef shared_ptr<byte> spbyte;
typedef shared_ptr<char> spchar;

// TODO: reference additional headers your program requires here
