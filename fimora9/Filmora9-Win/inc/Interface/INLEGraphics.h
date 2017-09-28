#pragma once

#include "INLEMotion.h"
#include "NLEGraphics.h"

// {9E9A4A4B-9CA6-4844-B5CA-E5DF4C9B2C15}
DEFINE_GUID(CLSID_NLEGraphics, 0x9e9a4a4b, 0x9ca6, 0x4844, 0xb5, 0xca, 0xe5, 0xdf, 0x4c, 0x9b, 0x2c, 0x15);

// {6ac9800a-e05d-43c0-88a2-34857473129f} 
DEFINE_GUID(IID_INLEGraphics, 0xa1380832, 0x1b1f, 0x4b0b, 0x88, 0x80, 0xd5, 0x15, 0x2f, 0xec, 0x0b, 0x4b);

/**
*@ingroup GroupGraphics
*@brief INLEGraphics接口
*/
class INLEGraphics :
	public INLEDrawer
{
public:
	virtual HRESULT __stdcall Line(NLEImage& image, tagNLEPointF& pt0, tagNLEPointF& pt1, COLORREF rgb, int thickness, int lineType, int shift) = 0;
	virtual HRESULT __stdcall PolyLine(NLEImage& image, vector<tagNLEPointF>& pts, bool closed, COLORREF rgb, int thickness, int lineType, int shift) = 0;
	virtual HRESULT __stdcall Circle(NLEImage& image, tagNLEPointF& center, int radius, COLORREF rgb, int thickness, int lineType, int shift) = 0;
	virtual HRESULT __stdcall Ellipse(NLEImage& image, tagNLEPointF& topLeft, tagNLEPointF& bottomRight, COLORREF rgb, int thickness, int lineType) = 0;
	virtual HRESULT __stdcall FloodFill(NLEImage& image, tagNLEPointF& seed, COLORREF rgb, int diff, int flags) = 0;

	virtual HRESULT __stdcall StrechBlt(NLEImage* imgDest, int xDest, int yDest, int wDest, int hDest, NLEImage* imgSrc, int xSrc, int ySrc, int wSrc, int hSrc, DWORD rop) = 0;

	virtual HRESULT __stdcall Crop(NLEImage* dst, NLEImage* src, NLERectF &cropRegion) = 0;
};
