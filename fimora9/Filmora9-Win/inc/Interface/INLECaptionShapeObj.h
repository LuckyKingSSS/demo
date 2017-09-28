#include <Guiddef.h>                                                            
#include <objbase.h>    
#include "NLECaptionDefine.h"
#include "INLECGGraphics.h"

#ifndef _NLE_SHAPE_PROPERTY_TYPE_
#define _NLE_SHAPE_PROPERTY_TYPE_

// {F13555A6-A4CC-496b-8111-B4209105FDDC}
DEFINE_GUID(CLSID_INLECaptionShapeObj,
	0xf13555a6, 0xa4cc, 0x496b, 0x81, 0x11, 0xb4, 0x20, 0x91, 0x5, 0xfd, 0xdc);

// {52BA999B-9237-4f55-BC05-197A891FFD8C}
DEFINE_GUID(IID_INLECaptionShapeObj,
	0x52ba999b, 0x9237, 0x4f55, 0xbc, 0x5, 0x19, 0x7a, 0x89, 0x1f, 0xfd, 0x8c);

class INLECaptionShapeObj :public IUnknown
{
public:
	virtual BOOL __stdcall SetCGFace(const NLECGFace* pFace) = 0;
	virtual BOOL __stdcall GetCGFace(NLECGFace* pFace) = 0;
	virtual BOOL __stdcall SetCGFaceEnable(int bEnable) = 0;

	virtual BOOL __stdcall SetCGFaceFillType(int iType) = 0;
	virtual BOOL __stdcall SetCGFaceSingleColor(DWORD color) = 0;
	virtual BOOL __stdcall SetCGFaceColorDir(int iDir) = 0;
	virtual BOOL __stdcall SetCGFaceColor1(DWORD color1) = 0;
	virtual BOOL __stdcall SetCGFaceColor2(DWORD color2) = 0;
	virtual BOOL __stdcall SetCGFaceAlpha(int iAlpha) = 0;
	virtual BOOL __stdcall SetCGFaceImage(BSTR pTexPath) = 0;

	virtual BOOL __stdcall SetCGBorder(const NLECGBorder* pBorder) = 0;
	virtual BOOL __stdcall GetCGBorder(NLECGBorder* pBorder) = 0;
	virtual BOOL __stdcall SetCGBorderEnable(int bEnable) = 0;

	virtual BOOL __stdcall SetCGBorderSingleColor(DWORD color) = 0;
	virtual BOOL __stdcall SetCGBorderAlpha(int iAlpha) = 0;
	virtual BOOL __stdcall SetCGBorderWidth(int iWidth) = 0;
	virtual BOOL __stdcall SetCGBorderBlur(int iBlur) = 0;

	virtual void __stdcall SetDstSceneWH(float iW, float iH) = 0;
	virtual void __stdcall SetRotate(float fRotate) = 0;
	virtual void __stdcall GetRotate(float* fRotate) = 0;
	virtual void __stdcall SetGraphicID(int iID) = 0;
	virtual int  __stdcall GetGraphicID() = 0;

	virtual void __stdcall GetTextArea(NLECGPoint* ptTop, NLECGPoint* ptBtm) = 0;
	virtual bool __stdcall HitTest(NLECGPoint* point) = 0;
	virtual void __stdcall SetCtrlPoint(NLECGPoint point, int nIdx) = 0;
};


#endif