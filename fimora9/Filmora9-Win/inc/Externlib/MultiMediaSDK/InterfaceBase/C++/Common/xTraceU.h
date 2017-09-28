// xTrace.h

#ifndef _MAGIC_TRACE
#define _MAGIC_TRACE

// Usage:
// void TRACE(LPCTSTR lpszFormat, ...);
// void XTRACE(LPCTSTR lpszFormat, ...);
// void XTRACE(COLORREF crText, LPCTSTR lpszFormat, ...);
// void XTRACE(LPBITMAPINFO lpbi, LPVOID lpBits);
// void XCOUNTER(int nItem, LPCTSTR lpszLabel, COLORREF crText = XTRACE_TEXT_COLOR);
// void XCOUNTER(int nItem, COLORREF crText, LPCTSTR lpszFormat, ...);

#ifndef WIN32 
	#define NO_XTRACE
#endif

#ifndef TRACE 
#ifdef NO_XTRACE
	#define TRACE
#else
	#define TRACE	TRACETV
#endif
#endif

#ifndef ASSERT
#ifdef _DEBUG
	#define ASSERT assert
	#include <assert.h>
#else
	#define ASSERT
#endif
#endif

#ifdef NO_XTRACE
	#define TRACETV
	#define XTRACE
	#define XCOUNTER

#else
	#define TRACETV		TraceTV		// Trace to Output of Visual Studio
	#define XTRACE		xTrace		// Trace to share memory viewer
	#define XCOUNTER	xCounter	// Trace Counter to share memory viewer

#include <stdio.h>

#include <windows.h>
#include <tchar.h>
#include "strconv.h"
//#include <atlbase.h>
//#include <atlconv.h>
//#pragma warning(disable: 4996)

#ifndef XTRACE_TEXT_COLOR
	#define XTRACE_TEXT_COLOR RGB(0, 0, 0)
#endif

#define COUNTER_COUNT	10

enum MAGIC_TRACE_MESSAGE
{
	MTM_NONE,
	MTM_NORMAL,
	MTM_COUNTER,
	MTM_DIB
};


inline void TraceTV(LPCTSTR lpszFormat, ...);
inline void xTrace(LPCTSTR lpszFormat, ...);
inline void xTrace(COLORREF textCr, LPCTSTR lpszFormat, ...);
inline void xTrace(LPBITMAPINFO lpbi, LPVOID lpBits);
inline void xCounter(int nItem, LPCTSTR lpszLabel, COLORREF crText = XTRACE_TEXT_COLOR);
inline void xCounter(int nItem, COLORREF crText, LPCTSTR lpszFormat, ...);

class DllLoad
{
public:
	DllLoad(LPCTSTR lpszName)
	{
		m_hModule = NULL;
		TCHAR szTemp[MAX_PATH] = _T("");
		if(::GetTempPath(MAX_PATH - 1, szTemp))
		{
			lstrcat(szTemp, lpszName);
			m_hModule = ::LoadLibrary(szTemp);
		}
	}
	~DllLoad()
	{	::FreeLibrary(m_hModule);	 m_hModule = NULL;
	}
	operator  HMODULE() { return m_hModule;}
protected:
	HMODULE m_hModule;
};


typedef void (WINAPI * MAGIC_TRACE_PROC)(MAGIC_TRACE_MESSAGE nMsg, WPARAM wParam, LPARAM lParam, WPARAM wParam2, LPARAM lParam2);


inline MAGIC_TRACE_PROC GetMagicTraceProc()
{
	static DllLoad xdl(_T("xTRACE.Dll"));
	static MAGIC_TRACE_PROC fnMagicTraceProc = (MAGIC_TRACE_PROC)GetProcAddress(HMODULE(xdl), "MagicTraceProc"); 
	return fnMagicTraceProc;
}

inline void xTrace(LPCTSTR lpszFormat, ...)
{
	USES_CONVERSION;
	MAGIC_TRACE_PROC pMTrace = GetMagicTraceProc();
	if (pMTrace)
	{
		va_list args;
		va_start(args, lpszFormat);
		int nBuf;
		TCHAR szBuffer[512];
		nBuf = _vstprintf(szBuffer, lpszFormat, args);
		if (nBuf) 
			pMTrace(MTM_NORMAL, XTRACE_TEXT_COLOR, (LPARAM)T2A(szBuffer), 0, 0);
		va_end(args);
	}

}

inline void xTrace(COLORREF textCr, LPCTSTR lpszFormat, ...)
{
	USES_CONVERSION;
	MAGIC_TRACE_PROC pMTrace = GetMagicTraceProc();
	if (pMTrace)
	{
		va_list args;
		va_start(args, lpszFormat);
		int nBuf;
		TCHAR szBuffer[512];
		nBuf = _vstprintf(szBuffer, lpszFormat, args);
		if (nBuf) 
			pMTrace(MTM_NORMAL, textCr, (LPARAM)T2A(szBuffer), 0, 0);
		va_end(args);
	}

}

inline void xTrace(LPBITMAPINFO lpbi, LPVOID lpBits)
{
	MAGIC_TRACE_PROC pMTrace = GetMagicTraceProc();
	if (pMTrace)
	{
		if (pMTrace)
			pMTrace(MTM_DIB, (WPARAM)lpbi, (LPARAM)lpBits, 0, 0);
	}
}

inline void xCounter(int nItem, LPCTSTR lpszLabel, COLORREF crText)
{
	USES_CONVERSION;
	MAGIC_TRACE_PROC pMTrace = GetMagicTraceProc();
	if (pMTrace)
	{
		if (pMTrace)
			pMTrace(MTM_COUNTER, (WPARAM)nItem, (LPARAM)T2A((LPTSTR)lpszLabel), (WPARAM)crText, 0);
	}
}

inline void xCounter(int nItem, COLORREF crText, LPCTSTR lpszFormat, ...)
{

	USES_CONVERSION;
	MAGIC_TRACE_PROC pMTrace = GetMagicTraceProc();
	if (pMTrace)
	{
		va_list args;
		va_start(args, lpszFormat);
		int nBuf;
		TCHAR szBuffer[512];
		nBuf = _vstprintf(szBuffer, lpszFormat, args);
		if (nBuf) 
			pMTrace(MTM_COUNTER, (WPARAM)nItem, (LPARAM)T2A(szBuffer), (WPARAM)crText, 0);
		va_end(args);
	}

}

inline void TraceTV(LPCTSTR lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);
	int nBuf;
	TCHAR szBuffer[512];
    nBuf = _vstprintf(szBuffer, lpszFormat, args);
	if (nBuf) 
		OutputDebugString(szBuffer);
	va_end(args);
}

#endif

#endif
