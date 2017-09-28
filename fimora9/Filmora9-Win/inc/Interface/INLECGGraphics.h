
#pragma once

#include "NLEType.h"                                                          
#include <Guiddef.h>                                                            
#include <objbase.h>    

#include "INLEBitmap.h"
#include "NLEComPtr.h"
#include "NLECommon.h"


#ifdef WIN32
#ifdef SKIAGRAPHICS_EXPORTS
#define SKIAGRAPHICS_API __declspec(dllexport)
#else
#define SKIAGRAPHICS_API __declspec(dllimport)
#endif
#else
#define SKIAGRAPHICS_API   
#endif

#ifndef SK_ENABLE_INST_COUNT
	#if WIN32
		#ifdef _DEBUG
			#define SK_ENABLE_INST_COUNT 1
		#else
			#define SK_ENABLE_INST_COUNT 0
		#endif
	#else
		#define SK_ENABLE_INST_COUNT 0
#endif

#endif




typedef enum
{
	CG_NormalPolygon = 1,   //正多边行
	CG_RightArrow = 2,      //向右箭头
	CG_Round = 3,           //圆
	CG_RoundRect = 4,       //圆角矩形
	CG_RoundRectBubble = 5, //圆角矩形气泡
	CG_RoundBubble = 6,     //圆形气泡
	CG_Line = 7,            //直线
	CG_OpenedRoundBubble = 20, //任意拖拽的圆形气泡
	CG_OpenedRoundRectBubble = 21,//任意拖拽的圆角矩形气泡
	CG_OpenedArrowLine = 22,
	CG_OpenedLine = 23, //任意拖拽的直线
	CG_Triangle = 24,		//三角形
	CG_Pentagon = 25,		//五角星
	CG_Pentagon2 = 26,		//五角形
	CG_Hexagon = 27,		//六边形
	CG_TwoWayArrawLine = 28,		//双向箭头
	 
	CG_FixedSVG = 100,      //固定、不可调节的SVG。
}CG_Graphic_Type;

struct NLECGPoint
{
	float fPosX;
	float fPosY;
};

struct NLECGRect
{
	float fLeft;
	float fTop;
	float fRight;
	float fBottom;
};

struct NLECGSingleColor
{
	unsigned char iR;
	unsigned char iG;
	unsigned char iB;
	unsigned char iA;
};
struct NLECGGradientColor
{
	int iDir;  //0-360

	unsigned char iStartR;
	unsigned char iStartG;
	unsigned char iStartB;
	unsigned char iStartA;

	unsigned char iEndR;
	unsigned char iEndG;
	unsigned char iEndB;
	unsigned char iEndA;
};

struct NLECGFace
{
	int		bEnable;
	int		iFillType;     //0:单色；1：渐变；2：纹理

	NLECGSingleColor sColor; //单色
	NLECGGradientColor gColor; //渐变色。

	NLECGFace()
	{
		bEnable = true;
		iFillType = 0;

		memset(&sColor, 255, sizeof(sColor));
		memset(&gColor, 255, sizeof(gColor));
	}
};

struct NLECGBorder
{
	int  bEnable;

	int  iFillType;   //0:单色；1：渐变；

	NLECGSingleColor sColor; //单色

	NLECGGradientColor gColor; //渐变色。
	int   iWidth;
	int   iBlur;
	NLECGBorder()
	{
		bEnable = false;
		iFillType = 0;

		memset(&sColor, 255, sizeof(sColor));
		memset(&gColor, 255, sizeof(gColor));

		iWidth = 1;
		iBlur = 0;
	}
};


//图形几何形状信息。
const int g_iMaxCtrCnt = 10;


// {E4467464-3714-4484-9F65-8C9935845B2E}
DEFINE_GUID(CLSID_INLECGGraphics, 0xe4467464, 0x3714, 0x4484, 0x9f, 0x65, 0x8c, 0x99, 0x35, 0x84, 0x5b, 0x2e);

// {840F0506-92CE-43A2-84AB-4EA542F1EAC9}
DEFINE_GUID(IID_INLECGGraphics, 0x840f0506, 0x92ce, 0x43a2, 0x84, 0xab, 0x4e, 0xa5, 0x42, 0xf1, 0xea, 0xc9);

// {34EC3FF8-EF1B-412B-92C4-ECDC6D2AAD84}
DEFINE_GUID(IID_INLECGGraphicsColor, 0x34ec3ff8, 0xef1b, 0x412b, 0x92, 0xc4, 0xec, 0xdc, 0x6d, 0x2a, 0xad, 0x84);

// {98DC1D18-3A15-4A94-B00D-180305EA7488}
DEFINE_GUID(IID_INLECGGraphicsGeometry, 0x98dc1d18, 0x3a15, 0x4a94, 0xb0, 0xd, 0x18, 0x3, 0x5, 0xea, 0x74, 0x88);


//图形颜色相关接口。
class INLECGGraphicsColor :public IUnknown
{
public:
	//表面、颜色相关函数
	virtual void __stdcall SetFace(const NLECGFace& aFace) = 0;
	virtual void __stdcall GetFace(NLECGFace& aFace) = 0;

	virtual void  __stdcall SetFaceEnable(int  bEnable) = 0;
	virtual void  __stdcall SetFaceFillType(int iType) = 0;

	virtual void  __stdcall SetFaceColor(unsigned char iR, unsigned char iG, unsigned char iB) = 0;
	virtual void  __stdcall SetFaceStartColor(unsigned char iR, unsigned char iG, unsigned char iB) = 0;
	virtual void  __stdcall SetFaceEndColor(unsigned char iR, unsigned char iG, unsigned char iB) = 0;
	virtual void  __stdcall SetFaceAlpha(unsigned char iA) = 0;

	virtual void __stdcall  SetFaceDir(int iDir) = 0;


	//边框、颜色相关函数
	virtual void __stdcall SetBorder(const NLECGBorder& aBorder) = 0;
	virtual void __stdcall GetBorder(NLECGBorder& aBorder) = 0;

	virtual void  __stdcall SetBorderEnable(int bEnable) = 0;
	virtual void  __stdcall SetBorderWidth(int nWidth) = 0;
	virtual void __stdcall SetBorderBlur(int iBlur) = 0;
	virtual void  __stdcall SetBorderFillType(int iType) = 0;

	virtual void  __stdcall SetBorderColor(unsigned char iR, unsigned char iG, unsigned char iB) = 0;
	virtual void  __stdcall SetBorderStartColor(unsigned char iR, unsigned char iG, unsigned char iB) = 0;
	virtual void  __stdcall SetBorderEndColor(unsigned char iR, unsigned char iG, unsigned char iB) = 0;
	virtual void  __stdcall SetBorderAlpha(unsigned char iA) = 0;

	virtual void __stdcall SetBorderDir(int iDir) = 0;

};


//图形几何数据相关接口。
class INLECGGraphicsGeometry : public IUnknown
{
public:

	virtual void __stdcall GetOrigSceneWH(float& iW, float& iH) = 0;
	virtual void __stdcall SetDstSceneWH(float iW, float iH) = 0;

	//形状位置相关函数
	virtual void  __stdcall SetRotate(float fRotate) = 0;
	virtual void __stdcall GetRotate(float* fRotate) = 0;
	//获取图形相对于原始外部坐标的偏移量，由旋转，边框厚度引起的。
	virtual void  __stdcall GetOffSet(float& iOffSetX, float& iOffSetY) = 0;

	//获取图形上绘制文字区域的范围。
	virtual void  __stdcall GetTextArea(NLECGPoint& leftTop, NLECGPoint& rightBottom) = 0;

	//控制点函数
	virtual int   __stdcall GetControlPointCnt() = 0;

	virtual void  __stdcall SetControlPoints(NLECGPoint point, int iIndex) = 0;//原始坐标下的位置，非编辑窗口，屏幕坐标位置。
	virtual void  __stdcall GetControlPoints(NLECGPoint& point, int iIndex) = 0;

	virtual bool  __stdcall IsContainsPoint(NLECGPoint& point) = 0;

};


class INLECGGraphics :public IUnknown
{
public:
	virtual int  __stdcall GetGraphicID() = 0;
	virtual void __stdcall GetImage(NLEComPtr<INLEBitmap>& image) = 0;
	virtual void __stdcall ReleaseImage() = 0;
};


// {16141921-D72B-4731-9EE7-F640662C1FA8}
DEFINE_GUID(IID_INLECGGraphicsManager, 0x16141921, 0xd72b, 0x4731, 0x9e, 0xe7, 0xf6, 0x40, 0x66, 0x2c, 0x1f, 0xa8);


class INLECGManager :public IUnknown
{
public:
	virtual HRESULT __stdcall CreateCGGraphic(int iID, void** ppGraphic) = 0;
	virtual HRESULT __stdcall ReleaseCGGraphic(void** ppGrapic) = 0;

	virtual HRESULT __stdcall FillGradientColorAlpha(NLEComPtr<INLEBitmap>& Image, COLORREF crBegin, COLORREF crEnd, int iAngle) = 0;
	virtual HRESULT __stdcall DrawBackgroundTitleImage(NLEComPtr<INLEBitmap>& Image, COLORREF color0 = 0xFFCCCCCC, COLORREF color1 = 0xFFFFFFFF, int iW = 10, int iH = 10) = 0;

};


