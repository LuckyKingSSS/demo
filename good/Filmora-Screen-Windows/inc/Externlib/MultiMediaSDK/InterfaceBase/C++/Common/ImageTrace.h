// ImageTrace.h 

#pragma once
#ifndef WIN32
#include "Linux32_Typedef.h"
#endif
#ifdef cppplus
extern "C"
{
#endif

#include "YUVImage.h"

#pragma warning(disable: 4013)

void ITInit(int x, int y, int cx, int cy); 
void ITClose();

void ITSaveYUV(const YUVImage * pImage, const char* pFileName);
void ITSave(const LPBITMAPINFO lpbi, const char* pFileName);
void ITTrace(const LPBITMAPINFO lpbi, const LPBYTE pBits);
void ITTraceYUV(const YUVImage * pImage);

void ITTraceText(const char* lpszFormat, ...);
void ITTraceColorText(COLORREF crText, const char* lpszFormat, ...);

enum IT_COMMOND
{
	IT_INIT_LIBRARY,
	IT_INIT,
	IT_CLOSE,
	IT_TRACE_IMAGE,
	IT_TRACE_YUV,
	IT_SAVE_IMAGE,
	IT_SAVE_YUV_2_BMP,
	IT_TEXT,
};


#ifndef WIN32
	#include <dlfcn.h>
	#include <dirent.h>

	#define FreeLibrary dlclose
	#define LoadLibrary dlopen
	#define GetProcAddress dlsym
	#define FreeLibraryA dlclose
	#define LoadLibraryA(a) dlopen(a, 0)

#endif 

#ifdef WIN32
	#define PLAYER "GLPlayer.dll"
#else
	#define PLAYER "GLPlayer.dylib"

#endif
			

__inline void __ImageTraceProc(unsigned int uCommand, WPARAM wParam, LPARAM lParam)
{
	typedef void *HEXPORTVIEWER;
	typedef void *HPLAYER;
	typedef BOOL (__stdcall* ShowImage_API)(HEXPORTVIEWER hViewer, const LPBITMAPINFO lpbi, const LPBYTE pBits);
	typedef BOOL (__stdcall* ShowYUVImage_API)(HEXPORTVIEWER hViewer, const YUVImage * pImage);
	typedef DWORD (__stdcall* SendCommand_API)(HPLAYER hPlayer, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static HMODULE m_hModule = 0;
	static HEXPORTVIEWER  m_hView = 0;

	static ShowImage_API fnEVShowImage = 0; 
	static ShowYUVImage_API fnEVShowYUVImage = 0; 
	static SendCommand_API fnVPSendCommand = 0; 


	switch(uCommand)
	{
	case IT_INIT_LIBRARY:
		{
			if (0 == m_hModule)
				m_hModule = LoadLibraryA(PLAYER);

			fnEVShowImage = (ShowImage_API)GetProcAddress(m_hModule, "EVShowImage"); 
			fnEVShowYUVImage = (ShowYUVImage_API)GetProcAddress(m_hModule, "EVShowYUVImage"); 
			fnVPSendCommand = (SendCommand_API)GetProcAddress(m_hModule, "VPSendCommand"); 
		}
		break;

	case IT_INIT:
		{
			if (0 == m_hModule)
				__ImageTraceProc(IT_INIT_LIBRARY, 0, 0);

			{
			typedef HEXPORTVIEWER (__stdcall* Create_API)(void* hParentWnd, LPRECT prcPos);
			Create_API fnEVCreate = (Create_API)GetProcAddress(m_hModule, "EVCreate"); 

			int x = LOWORD(wParam), y = HIWORD(wParam), cx = LOWORD(lParam), cy = HIWORD(lParam);
			{
			RECT rcPos = { x, y, x + cx, y + cy };
			m_hView = fnEVCreate(0, &rcPos);
			}
			}
		}
		break;

	case IT_CLOSE:
		{
			typedef void (__stdcall* Destroy_API)(HEXPORTVIEWER hViewer);
			Destroy_API fnEVDestroy = (Destroy_API)GetProcAddress(m_hModule, "EVDestroy"); 
			fnEVDestroy(m_hView);
			m_hView = 0;
		}
		FreeLibrary(m_hModule);	
		m_hModule = 0;
		fnEVShowImage = 0;
		fnEVShowYUVImage = 0;
		break;

	case IT_TRACE_IMAGE:
		if (0 == m_hView)
			ITInit(0, 0, 640, 480);
		if (fnEVShowImage)
			fnEVShowImage(m_hView, (const LPBITMAPINFO)wParam, (const LPBYTE)lParam);
		break;

	case IT_TRACE_YUV:
		if (0 == m_hView)
			ITInit(0, 0, 640, 480);
		if (fnEVShowYUVImage)
			fnEVShowYUVImage(m_hView, (const YUVImage*)wParam);
		break;

	case IT_SAVE_IMAGE:
		if (fnVPSendCommand)
			fnVPSendCommand(0, 0x0100, wParam, lParam);
		break;

	case IT_SAVE_YUV_2_BMP:
		if (fnVPSendCommand)
			fnVPSendCommand(0, 0x0101, wParam, lParam);
		break;

	case IT_TEXT:
		if (0 == m_hModule)
			__ImageTraceProc(IT_INIT_LIBRARY, 0, 0);

		if (fnVPSendCommand)
			fnVPSendCommand(0, 0x0200, wParam, lParam);

		break;
	}
}

#ifndef MAKELONG
#define MAKELONG(a, b) (((a) & 0xFFFF) | (((b) & 0xFFFF) << 16))
#endif

__inline void ITInit(int x, int y, int cx, int cy)
{
	__ImageTraceProc(IT_INIT, MAKELONG(x, y), MAKELONG(cx, cy));
}

__inline void ITClose()
{
	__ImageTraceProc(IT_CLOSE, 0, 0);
}

__inline void ITTrace(const LPBITMAPINFO lpbi, const LPBYTE pBits)
{
	__ImageTraceProc(IT_TRACE_IMAGE, (WPARAM)lpbi, (LPARAM)pBits);
}

__inline void ITTraceYUV(const YUVImage * pImage)
{
	__ImageTraceProc(IT_TRACE_YUV, (WPARAM)pImage, 0);
}

__inline void ITSave(const LPBITMAPINFO lpbi, const char* pFileName)
{
	__ImageTraceProc(IT_SAVE_IMAGE, (WPARAM)lpbi, (LPARAM)pFileName);
}

__inline void ITSaveYUV(const YUVImage * pImage, const char* pFileName)
{
	__ImageTraceProc(IT_SAVE_YUV_2_BMP, (WPARAM)pImage, (LPARAM)pFileName);
}

__inline void ITTraceText(const char* lpszFormat, ...)
{
	int nBuf;
	char szBuffer[512];
	va_list args;
	va_start(args, lpszFormat);
	nBuf = (int)vsprintf(szBuffer, lpszFormat, args);
	if (nBuf) 
	{
		__ImageTraceProc(IT_TEXT, 0, (LPARAM)szBuffer);
	}

	va_end(args);

}

__inline void ITTraceColorText(COLORREF crText, const char* lpszFormat, ...)
{
	int nBuf;
	char szBuffer[512];
	va_list args;
	va_start(args, lpszFormat);
	nBuf = (int)vsprintf(szBuffer, lpszFormat, args);
	if (nBuf) 
	{
		__ImageTraceProc(IT_TEXT, crText, (LPARAM)szBuffer);
	}

	va_end(args);

}




#ifdef cppplus
}
#endif
