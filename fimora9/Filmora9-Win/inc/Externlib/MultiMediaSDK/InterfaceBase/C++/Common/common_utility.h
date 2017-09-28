// common_utility.h
//

#pragma once
#ifndef WIN32
#include "Linux32_Typedef.h"
#endif
#include <wchar.h>
#include <stdio.h>
#include <string.h>

#define RECT_WIDTH(rect) (rect.right - rect.left)
#define RECT_HEIGHT(rect) (rect.bottom - rect.top)

#ifdef UNICODE
	#define FourCC2Str FourCC2StrW
#else
	#define FourCC2Str FourCC2StrA
#endif

#ifndef WIN32

__inline BOOL OffsetRect(LPRECT lprc, int dx, int dy)
{
	if (0 == lprc)
		return FALSE;
	
	lprc->left += dx;
	lprc->right += dx;
	
	lprc->top += dy;
	lprc->bottom += dy;
	

	return TRUE;
}
	
__inline BOOL PtInRect(LPRECT pRect, POINT pt)
{
	if (0 == pRect)
		return FALSE;
	
	if (   ((pt.x >= pRect->left) && (pt.x < pRect->right))
		&& ((pt.y >= pRect->top) && (pt.y < pRect->bottom)))
		return TRUE;
		
	return FALSE;
}


#endif


__inline const wchar_t* FourCC2StrW(wchar_t szBuf[5], DWORD dwFourCC)
{
	swprintf(szBuf, 5, L"%c%c%c%c", (char)(dwFourCC << 24 >> 24), (char)(dwFourCC << 16 >> 24), (char)(dwFourCC << 8 >> 24), (char)(dwFourCC >> 24));
	return szBuf;
}

__inline const char* FourCC2StrA(char szBuf[5], DWORD dwFourCC)
{
	sprintf(szBuf, "%c%c%c%c", (char)(dwFourCC << 24 >> 24), (char)(dwFourCC << 16 >> 24), (char)(dwFourCC << 8 >> 24), (char)(dwFourCC >> 24));
	return szBuf;
}

__inline DWORD Str2FourCC(char* pszFourCCStr)
{
	char szFourCC[] = "    ";
	int len = (int)strlen(pszFourCCStr);
	for (int i = 0; i < len; ++i)
	{
		szFourCC[i] = pszFourCCStr[i];
	}
	return *(DWORD*)szFourCC;
}

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



//#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p); (p)=NULL; } }
//#define SAFE_CLOSE_HANDLE(h)	{ if(h) { CloseHandle(h); (h)=NULL; } }
