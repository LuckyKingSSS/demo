	
////////////////////////////////////////////////////////////////////////////
// ViewDef.h
//

#pragma once

struct VRECT
{
	int x;
	int y;
	int cx;
	int cy;
};


#ifdef WIN32

	#include <windows.h>
	
	typedef HWND HSUBVIEW;
	typedef HWND HVIEW;
	
	typedef DWORD OSStatus;

//	#define printf

	#define RECT_WIDTH(rect) (rect.right - rect.left)
	#define RECT_HEIGHT(rect) (rect.bottom - rect.top)

#define MESSAGE_LOOP \
{ \
	MSG msg = { 0 }; \
	while (GetMessage(&msg, NULL, 0, 0)) \
	{ \
		TranslateMessage(&msg); \
		DispatchMessage(&msg); \
	} \
}
	
#else
	
#include "Linux32_Typedef.h"


	typedef void * HSUBVIEW;
#ifndef WINDOWREF
	typedef void* WindowRef;
#define WINDOWREF
#endif
    typedef WindowRef HVIEW;

	typedef unsigned short WORD;
	typedef int LONG;
	typedef unsigned int DWORD;
	typedef unsigned int UINT;
	typedef unsigned int LRESULT;
	typedef void* LPVOID;
	typedef void* HMODULE;
	typedef void* HANDLE;
//	typedef unsigned long LPARAM;
//	typedef unsigned long WPARAM;
#ifndef CGCONTEXTREF
    typedef void* CGContextRef;
#define CGCONTEXTREF
#endif
	typedef CGContextRef HDC;
	typedef unsigned char BYTE, *LPBYTE;

	#ifdef  UNICODE                     
		//typedef wchar_t TCHAR;
		#define _T(x) L ## x
	#else
		//typedef char TCHAR;
		#define _T(x) x
	#endif
	
	typedef wchar_t WCHAR;

	typedef const TCHAR* LPCTSTR;
	typedef TCHAR*LPTSTR, PTSTR;
	

#define WM_MOVE                         0x0003
#define WM_SIZE                         0x0005
#define WM_PAINT                        0x000F
#define WM_DISPLAYCHANGE                0x007E

#define WM_MOUSEMOVE                    0x0200
#define WM_LBUTTONDOWN                  0x0201
#define WM_LBUTTONUP                    0x0202
#define WM_LBUTTONDBLCLK                0x0203
#define WM_RBUTTONDOWN                  0x0204
#define WM_RBUTTONUP                    0x0205
#define WM_RBUTTONDBLCLK                0x0206
#define WM_MBUTTONDOWN                  0x0207
#define WM_MBUTTONUP                    0x0208
#define WM_MBUTTONDBLCLK                0x0209
#define WM_MOUSEWHEEL                   0x020A
#define WM_USER							0x0400
	

#define MAKELPARAM(a, b) (((a) & 0xFFFF) | (((b) & 0xFFFF) << 16))
#define LOWORD(a) ((a) & 0xFFFF)
#define HIWORD(a) (((a) >> 16) & 0xFFFF)

#define MESSAGE_LOOP RunApplicationEventLoop()
	
#endif
	
#define WM_MOUSEDRAG                    (WM_USER + 0x0300)

typedef DWORD ARGB;

#ifndef MAKE_ARGB
	#define MAKE_ARGB(a, r, g, b) ((a << 24) | (r << 16) | (g << 8) | b)
	#define A_VALUE(argb) (argb >> 24)
	#define R_VALUE(argb) ((argb >> 16) & 0x000000FF)
	#define G_VALUE(argb) ((argb >> 8) & 0x000000FF)
	#define B_VALUE(argb) (argb & 0x000000FF)
#endif



#ifdef WIN32

__inline void FillSolidRect(HDC hDC, const VRECT* lpRect, ARGB cr)
{
	int a = A_VALUE(cr);
	COLORREF clr = RGB(R_VALUE(cr) * a / 255, G_VALUE(cr) * a / 255, B_VALUE(cr) * a / 255);
	RECT rect = { lpRect->x, lpRect->y, lpRect->x + lpRect->cx, lpRect->y + lpRect->cy };
	::SetBkColor(hDC, clr);
	::ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rect, NULL, 0, NULL);
}

__inline void StrokeRect(HDC hDC, const VRECT* lpRect, ARGB cr)
{
	int a = A_VALUE(cr);
	COLORREF clr = RGB(R_VALUE(cr) * a / 255, G_VALUE(cr) * a / 255, B_VALUE(cr) * a / 255);
	RECT rect = { lpRect->x, lpRect->y, lpRect->x + lpRect->cx, lpRect->y + lpRect->cy };
	HPEN hPen = CreatePen(PS_SOLID, 1, clr);
    HPEN hOldPen = (HPEN)SelectObject(hDC, hPen); 
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hDC, GetStockObject(HOLLOW_BRUSH)); 
	Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
    SelectObject(hDC, hOldPen); 
    SelectObject(hDC, hOldBrush); 
	DeleteObject(hPen);
}

__inline void GetClientRect(HWND hWnd, VRECT* pRect)
{
	if (0 == pRect)
		return;
		
	RECT rect = { 0 };
	GetClientRect(hWnd, &rect);
	
	VRECT lrect = { rect.left, rect.top, RECT_WIDTH(rect), RECT_HEIGHT(rect) };
	*pRect = lrect;
}

__inline void InvalidateView(HVIEW hView)
{
	InvalidateRect(hView, 0, TRUE);
}

#else

#endif

