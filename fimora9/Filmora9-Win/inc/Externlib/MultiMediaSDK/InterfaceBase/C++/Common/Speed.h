// Speed.h: interface for the CSpeed class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPEED_H__2750D4C1_8695_4595_8B96_7D7D4AB6FB14__INCLUDED_)
#define AFX_SPEED_H__2750D4C1_8695_4595_8B96_7D7D4AB6FB14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#ifndef WIN32
#include "Linux32_Typedef.h"
#endif
static LONGLONG GetSecondCount()
{
	static LARGE_INTEGER liCounter = {0};
	if (0 == liCounter.QuadPart)
		QueryPerformanceFrequency(&liCounter);

	return liCounter.QuadPart;
}

// unit: current count
__inline LONGLONG GetCurCount()
{
	LARGE_INTEGER liCurrent;
	QueryPerformanceCounter(&liCurrent);
	return liCurrent.QuadPart;
}

// unit: MilliSecond (high precision)
__inline DWORD highGetTime()
{
	LARGE_INTEGER liCurrent = {0};
	QueryPerformanceCounter(&liCurrent);
	return (DWORD)(liCurrent.QuadPart * 1000 / GetSecondCount());
}

class CSpeed  
{
public:
	CSpeed() {
		m_liBegin.QuadPart = m_liEnd.QuadPart = 0;
		Begin();
	}

	void Begin() { QueryPerformanceCounter(&m_liBegin); }
	void End()   { QueryPerformanceCounter(&m_liEnd);   }
	void _End()   { QueryPerformanceCounter(&m_liEnd);   }

	double GetSecond() {
		return (m_liEnd.QuadPart - m_liBegin.QuadPart) / (double)GetSecondCount();
	}
	double GetMilliSecond() {
		return (m_liEnd.QuadPart - m_liBegin.QuadPart) * 1000.0 / (double)GetSecondCount();
	}

	DWORD GetCount() { return DWORD(m_liEnd.QuadPart - m_liBegin.QuadPart); }

private:
	LARGE_INTEGER m_liBegin;
	LARGE_INTEGER m_liEnd;
};


#endif // !defined(AFX_SPEED_H__2750D4C1_8695_4595_8B96_7D7D4AB6FB14__INCLUDED_)
