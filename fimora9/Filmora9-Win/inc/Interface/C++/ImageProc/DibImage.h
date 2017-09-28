/////////////////////////////////////////////
// DibImage.h


#pragma once

#include "bytewap.h"
#ifndef WIN32
#include "strconv.h"
#endif

#include "WSLog.h"

#define DIBWIDTHBYTES(cx, bit)  (((bit * cx + 31) & ~31) >> 3)

typedef enum eMallocMode
{
	MalloceModeInternal = 0,
	MalloceModeExternal = 0x10,
}MalloceMode;

class DibImage
{
public:
	DibImage()
		: m_hBits    (0)
		, m_lpBmpInfo(0)
#ifdef WIN32
		, m_hPalette (0)
#endif
		, m_nPitch   (0)
		, m_eBitsMallocMode (MalloceModeInternal)	
		{ }

	DibImage(const DibImage& src) 
		: m_hBits    (0)
		, m_lpBmpInfo(0)
#ifdef WIN32
		, m_hPalette (0)
#endif
		, m_nPitch   (0)
		, m_eBitsMallocMode (MalloceModeInternal)
	{
		*this = src;
	}

	DibImage(LPBYTE lpDibMem, LPBYTE lpBits = NULL)
		: m_hBits	 (0)
		, m_lpBmpInfo(0)
#ifdef WIN32
		, m_hPalette (0)
#endif
		, m_nPitch   (0) 
		, m_eBitsMallocMode (MalloceModeInternal)
	{
		if (lpDibMem)
		{
//			m_lpBmpInfo	= (LPBITMAPINFO)lpDibMem;
//			if(lpBits == NULL) m_hBits = lpDibMem + HeaderSize();

			LPBITMAPINFO lpInfo = (LPBITMAPINFO)lpDibMem;
			if(lpBits == NULL) 
				lpBits = lpDibMem + HeaderSize(lpInfo);
			Create(lpInfo->bmiHeader.biWidth, lpInfo->bmiHeader.biHeight, lpInfo->bmiHeader.biBitCount);
			memcpy(m_lpBmpInfo, lpInfo, HeaderSize(lpInfo));
			memcpy(m_hBits, lpBits, lpInfo->bmiHeader.biSizeImage);
		}
	}

	DibImage& operator = (const DibImage& src);
	DibImage& operator = (const LPBYTE lpDibMem);
	virtual int Create(int cx, int cy, int nBitCount = 32);
	virtual int Create(int cx, int cy, void* addr,int nBitCount = 32);

	virtual ~DibImage() { Clear(); }

	void Clear();
	int Empty() const {
		return (m_hBits == NULL);
	}

	int Width() const {
		if(m_lpBmpInfo) 
			return m_lpBmpInfo->bmiHeader.biWidth;
		return 0;
	}

	int Height() const {
		if(m_lpBmpInfo) 
			return m_lpBmpInfo->bmiHeader.biHeight;
		return 0;
	}

	int BitCount() const {
		if(m_lpBmpInfo) 
			return m_lpBmpInfo->bmiHeader.biBitCount;
		return 0;
	}

	int ImageSize() const {
		if(m_lpBmpInfo) 
			return m_lpBmpInfo->bmiHeader.biSizeImage;
		return 0;
	}

	int GetPitch() const {
		if(m_nPitch) return m_nPitch;
		else if(m_lpBmpInfo) {
			((DibImage*)this)->m_nPitch = DIBWIDTHBYTES(m_lpBmpInfo->bmiHeader.biWidth, m_lpBmpInfo->bmiHeader.biBitCount);
		}
		return m_nPitch;
	}

	LPBITMAPINFO GetBitmapInfo() const {
		return m_lpBmpInfo;
	}

	void SetBitmapInfo(int cx, int cy, int nBitCount = 32, BYTE* hBits = NULL)
	{
		BITMAPINFOHEADER &header = m_lpBmpInfo->bmiHeader;
		DWORD dwMemSize = 0;
		m_nPitch = DIBWIDTHBYTES(cx, nBitCount);
		dwMemSize = m_nPitch * cy;
		header.biWidth		= cx;
		header.biHeight		= cy;
		header.biBitCount	= nBitCount;
		header.biSizeImage	= dwMemSize;

		m_hBits = hBits;		
	}

	const LPBYTE GetBits() const {
		return (LPBYTE) m_hBits;
	}

	int HeaderSize(LPBITMAPINFO lpbi = NULL) const;

#ifdef WIN32
	operator HPALETTE();
#endif

	int Load(LPCTSTR lpszPathName);
	int Load(UINT nID, HMODULE hResModule = NULL);
	int Save(LPCTSTR lpszPathName);

#ifdef WIN32
	int Blt(HDC hDC, int x, int y, int cx = -1, int cy = -1, int xSrc = 0, int ySrc = 0);
#endif

protected:
	int				m_nPitch;

	HANDLE			m_hBits;
	LPBITMAPINFO	m_lpBmpInfo;
	
#ifdef WIN32
	HPALETTE		m_hPalette;
#else 
	HPALETTE        m_hPalette;
#endif
	
	MalloceMode		m_eBitsMallocMode; //m_hBites是否是外部分配的 
};

inline void DibImage::Clear()
{
	if(m_hBits &&  m_eBitsMallocMode != MalloceModeExternal)
	{
		delete [] (LPBYTE)m_hBits;
		m_hBits=NULL;
		
	}

	if(m_lpBmpInfo)
	{
		delete [] (BYTE*)m_lpBmpInfo;
		m_lpBmpInfo = NULL;
	}

#ifdef WIN32
	if(m_hPalette)
	{
		DeleteObject(m_hPalette);
		m_hPalette = NULL;
	}
#endif

	m_nPitch    = 0;
}

inline DibImage& DibImage::operator = (const DibImage& src)
{
	if(&src != this)
	{
		if(Create(src.Width(), src.Height(), src.BitCount()))
		{
			memcpy(m_hBits, src.m_hBits, ImageSize());
			if(BitCount() <= 8)
				memcpy(m_lpBmpInfo, src.m_lpBmpInfo, src.HeaderSize());
		}
		else 
		{
			Clear();
		}
	}

	return *this;
}
	
inline DibImage& DibImage::operator = (const LPBYTE lpDibMem)
{
	if (lpDibMem)
	{
		LPBITMAPINFOHEADER lpbi = (LPBITMAPINFOHEADER)lpDibMem;
		if (Create(lpbi->biWidth, lpbi->biHeight, lpbi->biBitCount))
		{
			int hr_sz = HeaderSize((LPBITMAPINFO)lpbi);
			if (BitCount() <= 8)
			{
				int nTmpSize = m_lpBmpInfo->bmiHeader.biSize;
				memcpy((LPBYTE)m_lpBmpInfo + nTmpSize, (LPBYTE)lpbi + nTmpSize, hr_sz - nTmpSize);
			}
			memcpy(m_hBits, lpDibMem + hr_sz, m_lpBmpInfo->bmiHeader.biSizeImage);
		}
	}

	return *this;
}

inline int DibImage::Create(int cx, int cy, int nBitCount)
{
	if (cx <= 0 || cy <= 0 || nBitCount <= 0)
	{
		WSLogEx(L"IMAGE cx = %d ,cy =%d ,nbitcount =%d \n",cx,cy,nBitCount);
	}

	if (cx == Width() && cy == Height() && nBitCount == BitCount())
		return TRUE;

	Clear();

	int nHeaderSize = sizeof(BITMAPINFOHEADER);
	if(nBitCount <= 8) nHeaderSize += ((int)(1 << nBitCount)) * sizeof(RGBQUAD);

	DWORD dwMemSize = 0;
	m_nPitch = DIBWIDTHBYTES(cx, nBitCount);
	dwMemSize = m_nPitch * cy;

	LPBITMAPINFO pNewBmpInfo = (LPBITMAPINFO)new BYTE[nHeaderSize];
	memset(pNewBmpInfo, 0, nHeaderSize);
	BITMAPINFOHEADER &header = pNewBmpInfo->bmiHeader;
	header.biSize		= sizeof(BITMAPINFOHEADER);
	header.biWidth		= cx;
	header.biHeight		= cy;
	header.biPlanes		= 1;
	header.biBitCount	= nBitCount;
	header.biSizeImage	= dwMemSize;


	HANDLE hNewMem = NULL;
	hNewMem = new BYTE[header.biSizeImage + 4];	

	if(!hNewMem)
	{
		delete [] (BYTE*)pNewBmpInfo;
		return FALSE;
	}

	memset(hNewMem, 0, header.biSizeImage);

//	memset(hNewMem, 0xFF, header.biSizeImage);

	m_hBits		= hNewMem;
	m_lpBmpInfo = pNewBmpInfo;

	return TRUE;
}

inline int DibImage::Create(int cx, int cy, void* addr,int nBitCount /*= 32*/)
{
	if (cx <= 0 || cy <= 0 || nBitCount <= 0)
	{
		WSLogEx(L"IMAGE cx = %d ,cy =%d ,nbitcount =%d \n",cx,cy,nBitCount);
	}

	if (cx == Width() && cy == Height() && nBitCount == BitCount())
		return TRUE;

	Clear();

	int nHeaderSize = sizeof(BITMAPINFOHEADER);
	if(nBitCount <= 8) nHeaderSize += ((int)(1 << nBitCount)) * sizeof(RGBQUAD);

	DWORD dwMemSize = 0;
	m_nPitch = DIBWIDTHBYTES(cx, nBitCount);
	dwMemSize = m_nPitch * cy;

	LPBITMAPINFO pNewBmpInfo = (LPBITMAPINFO)new BYTE[nHeaderSize];
	memset(pNewBmpInfo, 0, nHeaderSize);
	BITMAPINFOHEADER &header = pNewBmpInfo->bmiHeader;
	header.biSize		= sizeof(BITMAPINFOHEADER);
	header.biWidth		= cx;
	header.biHeight		= cy;
	header.biPlanes		= 1;
	header.biBitCount	= nBitCount;
	header.biSizeImage	= dwMemSize;


	void* hNewMem = NULL;
	if(addr == NULL)
	{
		hNewMem = new BYTE[header.biSizeImage+4];
		memset(hNewMem, 0, header.biSizeImage);
		m_eBitsMallocMode = MalloceModeInternal;
	}
	else 
	{
		hNewMem = addr;
		m_eBitsMallocMode = MalloceModeExternal;
	}

	if(!hNewMem)
	{
		delete [] (BYTE*)pNewBmpInfo;
		return FALSE;
	}

	m_hBits		= hNewMem;
	m_lpBmpInfo = pNewBmpInfo;

	return TRUE;
}


inline int DibImage::HeaderSize(LPBITMAPINFO lpbi) const
{
	int nSize = 0;
	if (!lpbi) 
		lpbi = m_lpBmpInfo;

	if(lpbi)
	{
		nSize = sizeof(BITMAPINFOHEADER);
		if(lpbi->bmiHeader.biBitCount <= 8)
		{
			int nColor = lpbi->bmiHeader.biClrUsed;
			if (nColor == 0) 
				nColor = 1 << lpbi->bmiHeader.biBitCount;
			nSize += sizeof(RGBQUAD) * nColor;
		}
	}

	return nSize;
}

inline int DibImage::Load(LPCTSTR lpszPathName)
{
	int bRet = FALSE;
	USES_CONVERSION;
	FILE * fp = fopen(T2A(lpszPathName), "rb");
	if(fp)
	{
		DWORD dwRead = 0;

		BITMAPFILEHEADER fd = { 0 };
		dwRead = (DWORD)fread(&fd, 1, sizeof(BITMAPFILEHEADER), fp);
		
		_B2N_16( fd.bfReserved1 );
		_B2N_16( fd.bfReserved2 );
		_B2N_16( fd.bfType );		
		_B2N_32( fd.bfOffBits );
		_B2N_32( fd.bfSize );			
		
		if(fd.bfType == 0x4d42)
		{
			BITMAPINFOHEADER id1 = { 0 };
			dwRead = (DWORD)fread(&id1, 1, sizeof(BITMAPINFOHEADER), fp);

			if(id1.biHeight < 0) {
				fclose(fp);
				return FALSE;
			}

			_B2N_16( id1.biBitCount );
			_B2N_16( id1.biPlanes );
			_B2N_32( id1.biClrImportant );		
			_B2N_32( id1.biClrUsed );
			_B2N_32( id1.biCompression );				
			_B2N_32( id1.biSize );		
			_B2N_32( id1.biSizeImage );
			_B2N_32( id1.biHeight );
			_B2N_32( id1.biWidth );		
			_B2N_32( id1.biXPelsPerMeter );
			_B2N_32( id1.biYPelsPerMeter );
									
			if(id1.biSizeImage == 0)
				id1.biSizeImage = DIBWIDTHBYTES(id1.biWidth, id1.biBitCount) * id1.biHeight;
			if(Create(id1.biWidth, id1.biHeight, id1.biBitCount))
			{
				memcpy(m_lpBmpInfo, &id1, sizeof(BITMAPINFOHEADER));
				if(id1.biBitCount <= 8)
				{
					int nColor = id1.biClrUsed;
					if(nColor == 0) 
						nColor = 1 << id1.biBitCount;
					dwRead = (DWORD)fread(m_lpBmpInfo->bmiColors, 1, sizeof(RGBQUAD) * nColor, fp);
				}

				dwRead = (DWORD)fread(GetBits(), 1, ImageSize(), fp);

				bRet = TRUE;
			}
		}

		fclose(fp);
	}

	return bRet;
}

#ifdef WIN32
inline int DibImage::Load(UINT nID, HMODULE hResModule)
{
	int bRet = FALSE;
	if(hResModule == NULL) hResModule = GetModuleHandle(NULL);

	HRSRC hSrc = ::FindResource(hResModule, (LPCTSTR)MAKEINTRESOURCE(nID), RT_BITMAP);
	if(hSrc)
	{
		HGLOBAL hResData = LoadResource(hResModule, hSrc);
		if(hResData)
		{
			LPVOID lpResData = LockResource(hResData);
			if(lpResData)
			{
				LPBITMAPINFOHEADER pInfo = (LPBITMAPINFOHEADER)lpResData;
				if(pInfo->biSize == sizeof(BITMAPINFOHEADER))
				{
					LPBYTE pBits = (LPBYTE)pInfo + sizeof(BITMAPINFOHEADER);

					int nColor = 0, nColorBits = 0;
					if(pInfo->biBitCount <= 8)
					{
						nColor = pInfo->biClrUsed;
						if(nColor == 0) nColor = 1 << pInfo->biBitCount;
						nColorBits = nColor * sizeof(RGBQUAD);

						pBits += nColorBits;
					}
					
					if(Create(pInfo->biWidth, pInfo->biHeight, pInfo->biBitCount))
					{
						memcpy(GetBits(), pBits, ImageSize());
						if(pInfo->biBitCount <= 8)
						{
							LPBYTE lpColor = (LPBYTE)m_lpBmpInfo + sizeof(BITMAPINFOHEADER);
							memcpy(lpColor, pBits - nColorBits, nColorBits);
						}

						bRet = TRUE;
					}

					if(bRet && ImageSize() == 0)
						m_lpBmpInfo->bmiHeader.biSizeImage = DIBWIDTHBYTES(Width(), BitCount()) * Height();
				}
			}
		}
	}

	return bRet;
}
#endif

inline int DibImage::Save(LPCTSTR lpszPathName)
{
	int bRet = FALSE;
	int nImageSize = ImageSize();

	if(m_hBits)
	{
		USES_CONVERSION;
		FILE* fp = fopen(T2A(lpszPathName), "wb"); 
		if(fp)
		{
			BITMAPFILEHEADER fd = {
				0x4d42,
				sizeof(BITMAPFILEHEADER) + HeaderSize() + ImageSize(),
				0, 0, sizeof(BITMAPFILEHEADER) + HeaderSize()
			};

			_B2N_16( fd.bfReserved1 );
			_B2N_16( fd.bfReserved2 );
			_B2N_16( fd.bfType );		
			_B2N_32( fd.bfOffBits );
			_B2N_32( fd.bfSize );	

			_B2N_32(m_lpBmpInfo->bmiHeader.biSize);
			_B2N_32(m_lpBmpInfo->bmiHeader.biWidth);
			_B2N_32(m_lpBmpInfo->bmiHeader.biHeight);
			_B2N_16(m_lpBmpInfo->bmiHeader.biPlanes);
			_B2N_16(m_lpBmpInfo->bmiHeader.biBitCount);
			_B2N_32(m_lpBmpInfo->bmiHeader.biCompression);
			_B2N_32(m_lpBmpInfo->bmiHeader.biSizeImage);
			_B2N_32(m_lpBmpInfo->bmiHeader.biXPelsPerMeter);
			_B2N_32(m_lpBmpInfo->bmiHeader.biYPelsPerMeter);
			_B2N_32(m_lpBmpInfo->bmiHeader.biClrUsed);
			_B2N_32(m_lpBmpInfo->bmiHeader.biClrImportant);
		
			do 
			{
				DWORD dwWrited = 0;
				dwWrited = (DWORD)fwrite(&fd, 1, sizeof(BITMAPFILEHEADER), fp);
				if (dwWrited != sizeof(BITMAPFILEHEADER))
				{
					break;
				}

				dwWrited = (DWORD)fwrite(m_lpBmpInfo, 1, HeaderSize(), fp);
				if (dwWrited != HeaderSize())
				{
					break;
				}

				dwWrited = (DWORD)fwrite(GetBits(), 1, nImageSize, fp);
				if (dwWrited != nImageSize)
				{
					break;
				}

				bRet = TRUE;
			} while (FALSE);

			fclose(fp);

			//bRet = TRUE;
		}
	}

	return bRet;
}

#ifdef WIN32

inline int DibImage::Blt(HDC hDC, int x, int y, int cx, int cy, int xSrc, int ySrc)
{
	int bRet = FALSE;
	if(m_hBits)
	{
		if(cx < 0) cx = Width();
		if(cy < 0) cy = Height();

		HPALETTE hOldPal = NULL;
		if(BitCount() <= 8)
		{
			hOldPal = SelectPalette(hDC, HPALETTE(*this), TRUE);
			RealizePalette(hDC);
		}

		int nHeight = Height();
		bRet = SetDIBitsToDevice(hDC,
					x, y, cx, cy, xSrc, nHeight - ySrc - cy,
					0, nHeight, GetBits(),
					m_lpBmpInfo, DIB_RGB_COLORS);

		if(hOldPal) SelectPalette(hDC, hOldPal, FALSE);
	}

	return bRet;
}

inline DibImage::operator HPALETTE()
{
	if(m_hPalette == NULL && m_hBits && BitCount() <= 8)
	{
		int nColors = 1 << BitCount();

		LPLOGPALETTE pLogPal = (LPLOGPALETTE)GlobalAlloc(
			GPTR, sizeof(LOGPALETTE) + sizeof(PALETTEENTRY) * nColors);
		pLogPal->palVersion		= 0x300;
		pLogPal->palNumEntries	= nColors;

		const int nRGBSize = 3;
		for(int i = 0; i < nColors; ++i)
			memcpy(&pLogPal->palPalEntry[i], &m_lpBmpInfo->bmiColors[i], nRGBSize);

		m_hPalette = ::CreatePalette(pLogPal);
		GlobalFree(pLogPal);
	}

	return m_hPalette;
}
#else
#endif



