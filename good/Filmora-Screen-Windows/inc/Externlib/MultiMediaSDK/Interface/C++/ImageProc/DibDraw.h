// DibDraw.h

#pragma once

#include "DibImage.h"
#include <vfw.h>
#pragma comment(lib, "vfw32.lib")

class DibDraw
{
public:
	DibDraw() 
	{ 
		InitializeCriticalSection(&m_cs);
		m_hDibDraw = DrawDibOpen();
	}

	~DibDraw() 
	{ 
		if(m_hDibDraw) 
		{
			DrawDibClose(m_hDibDraw);
			m_hDibDraw = 0;
		}
		DeleteCriticalSection(&m_cs);
	}

	int StretchBlt(HDC hDC, int x, int y, int cx, int cy, const DibImage* pDibImage,
					int xSrc = 0, int ySrc = 0, int cxSrc = -1, int cySrc = -1);

private:
    HDRAWDIB	m_hDibDraw;

	CRITICAL_SECTION	m_cs;
};

inline int DibDraw::StretchBlt(HDC hDC, int x, int y, int cx, int cy, const DibImage* pDibImage,
								 int xSrc, int ySrc, int cxSrc, int cySrc)
{
	if (0 == pDibImage)
		return FALSE;

	const DibImage& dib = *pDibImage;

	int bRet = FALSE;
	if(m_hDibDraw && !dib.Empty())
	{
		EnterCriticalSection(&m_cs);
		if(cxSrc < 0) cxSrc = dib.Width();
		if(cySrc < 0) cySrc = dib.Height();
		
		LPBITMAPINFOHEADER lpInfoHeader = reinterpret_cast<LPBITMAPINFOHEADER>(dib.GetBitmapInfo());
		if(dib.BitCount() <= 8)
		{
			DrawDibSetPalette(m_hDibDraw, HPALETTE((DibImage&)dib));
			DrawDibRealize(m_hDibDraw, hDC, 0);
		}
		
		bRet = DrawDibDraw(m_hDibDraw, hDC, x, y, cx, cy, lpInfoHeader,
			dib.GetBits(), xSrc, ySrc, cxSrc, cySrc, NULL);
		LeaveCriticalSection(&m_cs);
	}
	
	return bRet;
}
