#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <string>
#include <vector>
#include <stdint.h>
#include "stdcall.h"


typedef std::string		AnsiString;
typedef unsigned char		UINT8;	// 1 byte
typedef char			INT8;	// 1 byte
typedef short			INT16;	// 2 bytes
typedef unsigned short		UINT16;	// 2 bytes
typedef int			INT32;	// 4 bytes
typedef unsigned int		UINT32;	// 4 bytes

#ifdef _WIN32
#include <windows.h>
#include <winioctl.h>
#include <winnt.h>

// Win32 type defs
//
typedef __int64             LONGLONG;
typedef unsigned __int64    __uint64;
typedef unsigned char  		BYTE;	// 1 byte
typedef unsigned short 		WORD;	// 2 bytes
typedef PWSTR               WideString;
typedef unsigned short      USHORT;
//typedef unsigned short*     PWSTR;

typedef LONG NTSTATUS;

#else

// Linux type defs
//
typedef unsigned char  		BOOL;	// 1 byte
typedef unsigned char  		BYTE;	// 1 byte
typedef unsigned short 		WORD;	// 2 bytes
typedef unsigned int   		DWORD;	// 4 bytes
//typedef short double 		DDWORD;	// 6 bytes

typedef long long			__int64;
typedef unsigned long long		__uint64;
typedef __int64				LONGLONG;

#ifndef uint64_t
typedef unsigned long long uint64_t;
typedef long long int64_t;
#ifndef uint32_t
//typedef unsigned long uint32_t;
#endif

#endif

#endif
#endif //GLOBAL_H_
