#ifndef NLELINUXCOMMONUTILITY_HEADER
#define NLELINUXCOMMONUTILITY_HEADER

#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Linux32_Typedef.h"
#include "NLEBaseType.h"
#include "strconv.h"

#define RECT_WIDTH(rect) (rect.right - rect.left)
#define RECT_HEIGHT(rect) (rect.bottom - rect.top)

#ifdef UNICODE
#define FourCC2Str FourCC2StrW
#else
#define FourCC2Str FourCC2StrA
#endif



#include <execinfo.h>
__inline void print_stack_trace(int fd)
{
    void *array[256];
    int size;
    
    size = backtrace (array, 256);
    backtrace_symbols_fd(array, size, fd);
}



#ifndef WIN32

__inline BOOL SetRect(LPRECT lpRt, int xLeft, int yTop, int xRight, int yBottom)
{
    if (0 == lpRt)
        return FALSE;
    
    lpRt->left = xLeft;
    lpRt->top = yTop;
    lpRt->right = xRight;
    lpRt->bottom = yBottom;
    
    return TRUE;
}

__inline BOOL CopyRect(LPRECT lpDst, const RECT* lpSrc)
{
    if (0 == lpDst || 0 == lpSrc )
        return FALSE;
    
    lpDst->left = lpSrc->left;
    lpDst->top = lpSrc->top;
    lpDst->right = lpSrc->right;
    lpDst->bottom = lpSrc->bottom;
    
    return TRUE;
}

__inline BOOL UnionRect(LPRECT lpDst, const RECT* lpSrc1, const RECT* lpSrc2)
{
    if (0 == lpDst || 0 == lpSrc1 || 0 == lpSrc2)
        return FALSE;
    
    lpDst->left = lpSrc1->left < lpSrc2->left ? lpSrc1->left : lpSrc2->left;
    lpDst->top = lpSrc1->top < lpSrc2->top ? lpSrc1->top : lpSrc2->top;
    lpDst->right = lpSrc1->right > lpSrc2->right ? lpSrc1->right : lpSrc2->right;
    lpDst->bottom = lpSrc1->bottom > lpSrc2->bottom ? lpSrc1->bottom : lpSrc2->bottom;
    
    return TRUE;
}

__inline BOOL IsRectEmpty(const RECT* lpRt)
{
    if (0 == lpRt)
        return FALSE;
    
    return ((lpRt->right - lpRt->left) > 0) && ((lpRt->bottom - lpRt->top) > 0);
}

//__inline BOOL OffsetRect(LPRECT lprc, int dx, int dy)
//{
//    if (0 == lprc)
//        return FALSE;
//    
//    lprc->left += dx;
//    lprc->right += dx;
//    
//    lprc->top += dy;
//    lprc->bottom += dy;
//    
//    
//    return TRUE;
//}

//__inline BOOL PtInRect(LPRECT pRect, POINT pt)
//{
//    if (0 == pRect)
//        return FALSE;
//    
//    if (   ((pt.x >= pRect->left) && (pt.x < pRect->right))
//        && ((pt.y >= pRect->top) && (pt.y < pRect->bottom)))
//        return TRUE;
//    
//    return FALSE;
//}

__inline int _wtol(const wchar_t *_Str)
{
    int nRet = 0;
    
    swscanf(_Str, L"%d", &nRet);
    
    return nRet;
}

__inline int _wtoi64(const wchar_t *_Str)
{
    int nRet = 0;
    
    swscanf(_Str, L"%lld", &nRet);
    
    return nRet;
}

__inline int _wcsicmp(const wchar_t *cs, const wchar_t *ct)
{
    while (towlower(*cs) == towlower(*ct))
    {
        if (*cs == 0)
            return 0;
        cs++;
        ct++;
    }
    return towlower(*cs) - towlower(*ct);
}

__inline int _wtoi(const wchar_t *_Str)
{
    int nRet = 0;
    
    swscanf(_Str, L"%d", &nRet);
    
    return nRet;
}

__inline float _wtof(const wchar_t *_Str)
{
    float fRet = 0;
    
    swscanf(_Str, L"%f", &fRet);
    
    return fRet;
}

//__inline long InterlockedIncrement(long volatile *p)
//{
//    (*p) = (*p) + 1;
//    
//    return *p;
//}
//
//__inline long InterlockedDecrement(long volatile *p)
//{
//    (*p) = (*p) - 1;
//    
//    return *p;
//}


__inline void wcsncpy_s( wchar_t * Dst,int SizeInWords, const wchar_t * Src, int MaxCount)
{
    if (SizeInWords < MaxCount)
    {
        return;
    }
    for (int i = 0;i < MaxCount;i++)
    {
        *Dst = *Src;
        Dst++;
        Src++;
    }
    *Dst = '\0';
}

#include <errno.h>
__inline errno_t _wfopen_s(FILE **ppFile, const wchar_t* filename, const wchar_t* mode)
{
    char fn[MAX_PATH];
    char m[MAX_PATH];
    USES_CONVERSION
    char *p = W2A(filename);
    strcpy(fn, p);
    
    p = W2A(mode);
    strcpy(m, p);
    
    FILE *pf = fopen(fn, m);
    if (pf)
    {
        *ppFile = pf;
        return 0;
    }
    return errno;
}

__inline FILE* _wfopen(const wchar_t* filename, const wchar_t* mode)
{
    FILE *pf = NULL;
    _wfopen_s(&pf, filename, mode);
    
    return pf;
}

__inline errno_t fopen_s(FILE **ppFile, const char *path, const char *mode)
{
    FILE *pf = fopen(path, mode);
    if (pf)
    {
        *ppFile = pf;
        return 0;
    }
    
    return errno;
}

__inline size_t fread_s(char *buf, int nMaxSize, int bytesOfUnit, int unitCount, FILE *pf)
{
    return fread(buf, bytesOfUnit, unitCount, pf);
}

#endif

__inline int _wtoihex(const wchar_t *_Str)
{
    int nRet = 0;
    
    swscanf(_Str, L"%X", &nRet);
    
    return nRet;
}

//__inline const wchar_t* FourCC2StrW(wchar_t szBuf[5], DWORD dwFourCC)
//{
//    swprintf(szBuf, 5, L"%c%c%c%c", (char)(dwFourCC << 24 >> 24), (char)(dwFourCC << 16 >> 24), (char)(dwFourCC << 8 >> 24), (char)(dwFourCC >> 24));
//    return szBuf;
//}

//__inline const char* FourCC2StrA(char szBuf[5], DWORD dwFourCC)
//{
//    sprintf(szBuf, "%c%c%c%c", (char)(dwFourCC << 24 >> 24), (char)(dwFourCC << 16 >> 24), (char)(dwFourCC << 8 >> 24), (char)(dwFourCC >> 24));
//    return szBuf;
//}

//__inline DWORD Str2FourCC(char* pszFourCCStr)
//{
//    char szFourCC[] = "    ";
//    int len = (int)strlen(pszFourCCStr);
//    for (int i = 0; i < len; ++i)
//    {
//        szFourCC[i] = pszFourCCStr[i];
//    }
//    return *(DWORD*)szFourCC;
//}

#ifndef WIN32

#define MAKELPARAM(a, b) ((a) | ((b) << 16))
#define LOWORD(a) ((a) << 16 >> 16)
#define HIWORD(a) ((a) >> 16)

#endif


#ifndef ARGB
#define ARGB DWORD
#endif


#ifndef MAKE_ARGB
#define MAKE_ARGB(a, r, g, b) ((a << 24) | (r << 16) | (g << 8) | b)
#define A_VALUE(argb) (argb >> 24)
#define R_VALUE(argb) ((argb >> 16) & 0x000000FF)
#define G_VALUE(argb) ((argb >> 8) & 0x000000FF)
#define B_VALUE(argb) (argb & 0x000000FF)
#endif




#define MKTAG(a, b, c, d)	( (DWORD)(a) | ((DWORD)(b) << 8) | ((DWORD)(c) << 16) | ((DWORD)(d) << 24) )
#define MKBETAG(a, b, c, d) ( (DWORD)(d) | ((DWORD)(c) << 8) | ((DWORD)(b) << 16) | ((DWORD)(a) << 24) )
#define MKFOURCC			MKTAG

#ifndef ZeroMemory
#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif

#ifndef LOBYTE
#define LOBYTE(w)           ((unsigned char)(((unsigned long)(w)) & 0xff))
#endif

#ifndef GetRValue
#define GetRValue(rgb)		(LOBYTE(rgb))
#endif
#ifndef GetGValue
#define GetGValue(rgb)		(LOBYTE(((WORD)(rgb)) >> 8))
#endif
#ifndef GetBValue
#define GetBValue(rgb)		(LOBYTE((rgb)>>16))
#endif

#ifndef WIN32
typedef unsigned char byte;
#endif


#ifndef wcscpy_s
#define wcscpy_s(p1, p2, p3) wcscpy(p1, p3)
#endif

#ifndef strcpy_s
#define strcpy_s(p1, p2, p3) strcpy(p1, p3)
#endif

#ifndef sscanf_s
#define sscanf_s sscanf
#endif

#ifndef wcscat_s
#define wcscat_s wcscat
#endif


#ifndef wcsncpy_s
#define wcsncpy_s wcsncpy
#endif


#ifndef swprintf_s
#define swprintf_s swprintf
#endif

#ifndef sscanf_s
#define sscanf_s sscanf
#endif

#ifndef stricmp
#define stricmp strcasecmp
#endif


#ifndef sprintf_s
#define sprintf_s(buf, max_size, fmt, ...) sprintf(buf, fmt, ##__VA_ARGS__)
#endif




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
    USES_CONVERSION
    char *pName = W2A(lpLibFileName);
    
    //		LOGGER_INFO_(L"load dylib:%ls\n", lpLibFileName);
    HMODULE hMod = dlopen(pName, RTLD_NOW);
    
    
    return hMod;
}

__inline void FreeLibrary(HMODULE hModule)
{
    dlclose(hModule);
}

typedef int (*FARPROC)();

__inline FARPROC GetProcAddress(HMODULE hMod, LPCSTR lpProcName)
{
    return (FARPROC)dlsym(hMod, lpProcName);
}

#endif

