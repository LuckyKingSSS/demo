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
    
    
//#include "mac_types_base.h"
    
#include <dlfcn.h>
#include <stdlib.h>
    
    typedef enum tagCLSCTX
    {
        CLSCTX_INPROC_SERVER	= 0x1,
        CLSCTX_INPROC_HANDLER	= 0x2,
        CLSCTX_LOCAL_SERVER	= 0x4,
        CLSCTX_INPROC_SERVER16	= 0x8,
        CLSCTX_REMOTE_SERVER	= 0x10,
        CLSCTX_INPROC_HANDLER16	= 0x20,
        CLSCTX_RESERVED1	= 0x40,
        CLSCTX_RESERVED2	= 0x80,
        CLSCTX_RESERVED3	= 0x100,
        CLSCTX_RESERVED4	= 0x200,
        CLSCTX_NO_CODE_DOWNLOAD	= 0x400,
        CLSCTX_RESERVED5	= 0x800,
        CLSCTX_NO_CUSTOM_MARSHAL	= 0x1000,
        CLSCTX_ENABLE_CODE_DOWNLOAD	= 0x2000,
        CLSCTX_NO_FAILURE_LOG	= 0x4000,
        CLSCTX_DISABLE_AAA	= 0x8000,
        CLSCTX_ENABLE_AAA	= 0x10000,
        CLSCTX_FROM_DEFAULT_CONTEXT	= 0x20000,
        CLSCTX_ACTIVATE_32_BIT_SERVER	= 0x40000,
        CLSCTX_ACTIVATE_64_BIT_SERVER	= 0x80000,
        CLSCTX_ENABLE_CLOAKING	= 0x100000,
        CLSCTX_APPCONTAINER	= 0x400000,
        CLSCTX_ACTIVATE_AAA_AS_IU	= 0x800000,
        CLSCTX_PS_DLL	= ( int  )0x80000000
    } 	CLSCTX;
    
    __inline HMODULE LoadLibrary(LPCWSTR lpLibFileName)
    {
        char name[MAX_PATH];
        wcstombs(name, lpLibFileName, MAX_PATH);
        
        HMODULE hMod = dlopen(name, 0);
        
        return hMod;
    }
    
    typedef int (*FARPROC)();
    
    __inline FARPROC GetProcAddress(HMODULE hMod, LPCSTR lpProcName)
    {
        return (FARPROC)dlsym(hMod, lpProcName);
    }
    
#ifdef __cplusplus
}
#endif



#endif

#endif

