#pragma once

#ifndef WIN32

#define MAC64

//typedef unsigned char  uint8_t;
//typedef          char  int8_t;
//typedef unsigned short uint16_t;
//typedef          short int16_t;
//typedef unsigned int   uint32_t;
//typedef          int   int32_t;

#ifdef MAC64

//typedef unsigned long  uint64_t;
//typedef          long   int64_t;
typedef          int LONG;
typedef unsigned int ULONG;
typedef unsigned  long UINT64;
typedef long long __int64;
#elif  MAC32
//typedef unsigned long long  uint64_t;
//typedef          long long   int64_t;
typedef unsigned long ULONG;
typedef long LONG;
typedef unsigned long long UINT64;
typedef long long __int64;
#endif

typedef unsigned int DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;



typedef void* HANDLE;
typedef void* HMODULE;
typedef void* HPALETTE;
typedef BYTE* LPBYTE;
typedef BYTE* PBYTE;


typedef void* LPVOID;
typedef DWORD* PDWORD, DWORD_PTR;
typedef WORD* LPWORD;
typedef long WPARAM;
typedef long LPARAM;

typedef int		INT;
typedef unsigned int UINT;
typedef char	CHAR;
typedef short	SHORT;
typedef SHORT	*PSHORT;
typedef float	FLOAT;
typedef FLOAT	*PFLOAT;
typedef void	VOID;
typedef	const CHAR	*LPCSTR;
typedef CHAR	*LPSTR;


#else

//typedef unsigned char  uint8_t;
//typedef          char  int8_t;
//typedef unsigned short uint16_t;
//typedef          short int16_t;
//typedef unsigned int   uint32_t;
//typedef          int   int32_t;
//typedef unsigned long long  uint64_t;
//typedef          long long   int64_t;
//typedef long  LONG;
//typedef unsigned long  ULONG;
#endif