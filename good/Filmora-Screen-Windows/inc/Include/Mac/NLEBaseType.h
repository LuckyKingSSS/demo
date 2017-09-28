#ifndef NLEBASETYPE_HEADER
#define NLEBASETYPE_HEADER

#include <stdint.h>
#include <wchar.h>
#include "typedef.hpp"
//#include "common_utility.h"


#ifndef WIN32

typedef const wchar_t *LPCWCHAR;

typedef double DOUBLE;

typedef int64_t INT64;


// handle types
//typedef void*				HANDLE;
typedef void*				HMODULE;
typedef void*				HPALETTE;
typedef void*				LPVOID, *LPCVOID;
typedef void				VOID;
typedef void*               PVOID;


// 1-byte types
typedef unsigned char		BYTE;
typedef BYTE*				LPBYTE;
typedef BYTE*				PBYTE;


// 2-bytes types
typedef  unsigned short     USHORT;
typedef short				SHORT;
typedef unsigned short		WORD;
typedef SHORT*				PSHORT;


// 4-bytes types
typedef signed int			INT;
typedef unsigned int		UINT;

//typedef unsigned long		DWORD;
//typedef unsigned long		ULONG;
//typedef long				LONG;
//typedef long				HRESULT;

//typedef UINT				WPARAM;
//typedef UINT				LPARAM;



//typedef DWORD*				PDWORD, *DWORD_PTR, *LPDWORD;
typedef WORD*				LPWORD;


// 8-bytes types
//typedef uint64_t			UINT64, ULONGLONG;
typedef long long			LONGLONG;
//typedef int64_t				__int64;


// float types
typedef float				FLOAT;
typedef FLOAT*				PFLOAT;


// char / wide char types
typedef char				CHAR;
typedef CHAR*				PSTR, *LPSTR;
typedef	const CHAR*			PCSTR, *LPCSTR;


typedef wchar_t				WCHAR;

typedef wchar_t*			PWSTR, *LPWSTR, *BSTR;
typedef const wchar_t*		PCWSTR, *LPCWSTR;

typedef WCHAR				OLECHAR;
typedef BSTR*				LPBSTR;
typedef OLECHAR*			LPOLESTR;
typedef const OLECHAR*		LPCOLESTR;

typedef const wchar_t* LPCWCHAR;

typedef void* HWND;

//#define
typedef short VARIANT_BOOL;

typedef int errno_t;

#define OUT

#define IN

#define FAR

#define _Inout_opt_

#define _In_opt_

#define _Inout_

#define STDOVERRIDEMETHODIMP HRESULT __stdcall

#define OLE2W 

#define HRESULT_FROM_WIN32 



#ifdef __cplusplus
extern "C" {
#endif
    
    
    
#ifdef __cplusplus
}
#endif



#endif

#endif

