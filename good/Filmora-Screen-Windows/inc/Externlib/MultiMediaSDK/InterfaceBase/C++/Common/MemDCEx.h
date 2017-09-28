//////////////////////////////////////////////////
// MemDCEx.h

#pragma once


class MemDCEx
{
public:
	MemDCEx()
		: m_hBitmap(0)
		, m_hOldBitmap(0)
		, m_hMemDC(0)
		, m_nWidth(0)
		, m_nHeight(0)
		, m_nMemWidth(0)
		, m_nMemHeight(0)
	{
	}

	virtual ~MemDCEx()
	{
		Clear();
	}

	void Create(int cx, int cy)
	{
		m_nWidth = cx;
		m_nHeight = cy;

		if (m_hMemDC && (cx <= m_nMemWidth) && (cy <= m_nMemHeight))
		{
			return;
		}

		if (0 == m_hMemDC)
			m_hMemDC = CreateCompatibleDC(0);

		m_nMemWidth = cx + cx / 3;
		m_nMemHeight = cy + cy / 3;
		if (m_hOldBitmap)
		{
			SelectObject(m_hMemDC, m_hOldBitmap);
			DeleteObject(m_hBitmap);
			m_hOldBitmap = 0;
			m_hBitmap = 0;
		}

#define DIBWIDTHBYTES(cx, bit)  (((bit * cx + 31) & ~31) >> 3)

		BITMAPINFO BmpInfo = { 0 };
		BITMAPINFOHEADER &header = BmpInfo.bmiHeader;
		header.biSize		= sizeof(BITMAPINFOHEADER);
		header.biWidth		= m_nMemWidth;
		header.biHeight		= m_nMemHeight;
		header.biPlanes		= 1;
		header.biBitCount	= 24;
		header.biSizeImage	= DIBWIDTHBYTES(m_nMemWidth, 24) * m_nMemHeight;
		
		HANDLE hNewMem = 0;
		m_hBitmap = CreateDIBSection(m_hMemDC, &BmpInfo, DIB_RGB_COLORS, (LPVOID*)(&hNewMem), NULL, 0);
		//m_hBitmap = ::CreateCompatibleBitmap(hDC, m_nMemWidth, m_nMemHeight);
		m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);

	}

	void Clear()
	{
		if (m_hMemDC)
		{
			SelectObject(m_hMemDC, m_hOldBitmap);
			DeleteDC(m_hMemDC);
			DeleteObject(m_hBitmap);
		}

		m_hOldBitmap = 0;
		m_hMemDC = 0;
		m_hBitmap = 0;
		m_nWidth = 0;
		m_nHeight = 0;
	}

	BOOL Swap(HDC hDC, int x = 0,int y = 0, int cx = -1, int cy = -1, int src_x = 0, int src_y = 0)
	{
		if (-1 == cx)
			cx = m_nWidth;
		if (-1 == cy)
			cy = m_nHeight;

		BOOL br = BitBlt(hDC, x, y, cx, cy, m_hMemDC, src_x, src_y, SRCCOPY);
		return br;
	}

	HDC						m_hMemDC;

private:
	HBITMAP					m_hOldBitmap;
	HBITMAP					m_hBitmap;

public:
	int						m_nWidth;
	int						m_nHeight;

public:
	int						m_nMemWidth;
	int						m_nMemHeight;

};

